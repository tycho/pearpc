/*
 *	PearPC
 *	pcihwtd.h
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

#ifndef __IO_PCIHWTD_H__
#define __IO_PCIHWTD_H__

#include "tools/data.h"
#include "system/types.h"
#include "system/display.h"
#include "system/systhread.h"

/*! HW thread cmd/state values. */
enum {
	PCI_HWTD_CMD_EXIT = 0,		/*!< Exit HW thread command. */
	PCI_HWTD_CMD_READMEM,		/*!< HW thread read mem command. */
	PCI_HWTD_CMD_WRITEMEM,		/*!< HW thread write mem command. */
	PCI_HWTD_CMD_READIO,		/*!< HW thread read IO command. */
	PCI_HWTD_CMD_WRITEIO,		/*!< HW thread write IO command. */
	PCI_HWTD_CMD_IDLE,		/*!< HW thread idle state. */
};

struct PCI_HWTD_CmdParam {
	uint		cmd;
	uint		r;
	uint32		ap;		// address or port
	uint32		data;
	uint		size;
};

/*! PCI device object with HW thread. */
class PCI_HWTD_Device: public PCI_Device {
public:
	sys_semaphore	HWTCmdSem;
	sys_thread	HWTCmdThread;
	struct PCI_HWTD_CmdParam CmdParam;

			PCI_HWTD_Device(const char *name, uint8 bus, uint8 unit);
			~PCI_HWTD_Device();

	virtual bool	readDeviceMemHWT(uint r, uint32 address, uint32 &data, uint size);
	virtual bool	readDeviceIOHWT(uint r, uint32 port, uint32 &data, uint size);
	virtual bool	writeDeviceMemHWT(uint r, uint32 address, uint32 data, uint size);
	virtual bool	writeDeviceIOHWT(uint r, uint32 port, uint32 data, uint size);

	bool pushHWTCmd(uint cmd, uint r, uint32 ap, uint32 data, uint size);
};

#endif
