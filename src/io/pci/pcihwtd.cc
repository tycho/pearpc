/*
 *	PearPC
 *	pcihwtd.cc
 *
 *	Copyright (C) 2008 Andriy Golovnya (andrew_golovnia@ukr.net)
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License version 2 as
 *	published by the Free Software Foundation.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <cstdlib>
#include <cstring>

#include "tools/data.h"
#include "system/arch/sysendian.h"
#include "cpu/cpu.h"
#include "cpu/debug.h"
#include "cpu/mem.h"
#include "debug/tracers.h"
#include "pci.h"
#include "pcihwtd.h"

static void *PCI_HWTD_Device_Thread(void *arg) {

	PCI_HWTD_Device *d = (PCI_HWTD_Device*)arg;

	sys_lock_semaphore(d->HWTCmdSem);

	while (1) {
		sys_wait_semaphore(d->HWTCmdSem);
		struct PCI_HWTD_CmdParam p = d->CmdParam;

		switch(d->CmdParam.cmd) {
		case PCI_HWTD_CMD_EXIT:
			return NULL;
		case PCI_HWTD_CMD_READMEM:
			d->readDeviceMemHWT(p.r, p.ap, p.data, p.size);
			break;
		case PCI_HWTD_CMD_WRITEMEM:
			d->writeDeviceMemHWT(p.r, p.ap, p.data, p.size);
			break;
		case PCI_HWTD_CMD_READIO:
			d->readDeviceIOHWT(p.r, p.ap, p.data, p.size);
			break;
		case PCI_HWTD_CMD_WRITEIO:
			d->writeDeviceIOHWT(p.r, p.ap, p.data, p.size);
			break;
		case PCI_HWTD_CMD_IDLE:
			break;
		default:
			break;
		}
		d->CmdParam.cmd = PCI_HWTD_CMD_IDLE;
	}

	return NULL;
}

PCI_HWTD_Device::PCI_HWTD_Device(const char *aName, uint8 aBus, uint8 aUnit)
	:PCI_Device(aName, aBus, aUnit)
{
	if (sys_create_semaphore(&HWTCmdSem)) {
		IO_PCI_ERR("Can't create semaphore\n");
	}

	pushHWTCmd(PCI_HWTD_CMD_IDLE, 0, 0, 0, 0);

	if(sys_create_thread(&HWTCmdThread, 0, PCI_HWTD_Device_Thread, this)) {
		IO_PCI_ERR("Can't create thread\n");
	}
}

PCI_HWTD_Device::~PCI_HWTD_Device()
{
	pushHWTCmd(PCI_HWTD_CMD_EXIT, 0, 0, 0, 0);
}

bool PCI_HWTD_Device::readDeviceMemHWT(uint r, uint32 address, uint32 &data, uint size)
{
	return false;
}

bool PCI_HWTD_Device::readDeviceIOHWT(uint r, uint32 io, uint32 &data, uint size)
{
	return false;
}

bool PCI_HWTD_Device::writeDeviceMemHWT(uint r, uint32 address, uint32 data, uint size)
{
	return false;
}

bool PCI_HWTD_Device::writeDeviceIOHWT(uint r, uint32 io, uint32 data, uint size)
{
	return false;
}

bool PCI_HWTD_Device::pushHWTCmd(uint cmd, uint r, uint32 ap, uint32 data, uint size)
{
	sys_lock_semaphore(HWTCmdSem);

	CmdParam.cmd = cmd;
	CmdParam.r = r;
	CmdParam.ap = ap;
	CmdParam.data = data;
	CmdParam.size = size;

	sys_unlock_semaphore(HWTCmdSem);
	sys_signal_semaphore(HWTCmdSem);

	return true;
}
