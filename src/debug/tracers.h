/*
 *	PearPC
 *	tracers.h
 *
 *	Copyright (C) 2003 Sebastian Biallas (sb@biallas.net)
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

#ifndef __TRACERS_H__
#define __TRACERS_H__

#include "system/types.h"
#include "tools/snprintf.h"

#define PPC_CPU_TRACE(...) ht_printf("[CPU/CPU] " __VA_ARGS__)
#define PPC_ALU_TRACE(...) ht_printf("[CPU/ALU] " __VA_ARGS__)
#define PPC_FPU_TRACE(...) ht_printf("[CPU/FPU] " __VA_ARGS__)
#define PPC_DEC_TRACE(...) ht_printf("[CPU/DEC] " __VA_ARGS__)
#define PPC_ESC_TRACE(...) ht_printf("[CPU/ESC] " __VA_ARGS__)
//#define PPC_EXC_TRACE(...) ht_printf("[CPU/EXC] " __VA_ARGS__)
#define PPC_MMU_TRACE(...) ht_printf("[CPU/MMU] " __VA_ARGS__)
#define PPC_OPC_TRACE(...) ht_printf("[CPU/OPC] " __VA_ARGS__)
#define IO_PROM_TRACE(...) ht_printf("[IO/PROM] " __VA_ARGS__)
//#define IO_PROM_FS_TRACE(...) ht_printf("[IO/PROM/FS] " __VA_ARGS__)
//#define IO_3C90X_TRACE(...) ht_printf("[IO/3c90x] " __VA_ARGS__)
//#define IO_RTL8139_TRACE(...) ht_printf("[IO/rtl8139] " __VA_ARGS__)
//#define IO_GRAPHIC_TRACE(...) ht_printf("[IO/GCARD] " __VA_ARGS__)
//#define IO_CUDA_TRACE(...) ht_printf("[IO/CUDA] " __VA_ARGS__)
//#define IO_PIC_TRACE(...) ht_printf("[IO/PIC] " __VA_ARGS__)
//#define IO_PCI_TRACE(...) ht_printf("[IO/PCI] " __VA_ARGS__)
//#define IO_MACIO_TRACE(...) ht_printf("[IO/MACIO] " __VA_ARGS__)
//#define IO_NVRAM_TRACE(...) ht_printf("[IO/NVRAM] " __VA_ARGS__)
//#define IO_IDE_TRACE(...) ht_printf("[IO/IDE] " __VA_ARGS__)
//#define IO_USB_TRACE(...) ht_printf("[IO/USB] " __VA_ARGS__)
#define IO_SERIAL_TRACE(...) ht_printf("[IO/SERIAL] " __VA_ARGS__)
#define IO_CORE_TRACE(...) ht_printf("[IO/Generic] " __VA_ARGS__)

#define PPC_CPU_WARN(...) ht_printf("[CPU/CPU] <Warning> " __VA_ARGS__)
#define PPC_ALU_WARN(...) ht_printf("[CPU/ALU] <Warning> " __VA_ARGS__)
#define PPC_FPU_WARN(...) ht_printf("[CPU/FPU] <Warning> " __VA_ARGS__)
#define PPC_DEC_WARN(...) ht_printf("[CPU/DEC] <Warning> " __VA_ARGS__)
#define PPC_ESC_WARN(...) ht_printf("[CPU/ESC] <Warning> " __VA_ARGS__)
#define PPC_EXC_WARN(...) ht_printf("[CPU/EXC] <Warning> " __VA_ARGS__)
#define PPC_MMU_WARN(...) ht_printf("[CPU/MMU] <Warning> " __VA_ARGS__)
#define PPC_OPC_WARN(...) ht_printf("[CPU/OPC] <Warning> " __VA_ARGS__)
#define IO_PROM_WARN(...) ht_printf("[IO/PROM] <Warning> " __VA_ARGS__)
#define IO_PROM_FS_WARN(...) ht_printf("[IO/PROM/FS] <Warning> " __VA_ARGS__)
#define IO_3C90X_WARN(...) ht_printf("[IO/3c90x] <Warning> " __VA_ARGS__)
#define IO_RTL8139_WARN(...) ht_printf("[IO/rtl8139] <Warning> " __VA_ARGS__)
#define IO_GRAPHIC_WARN(...) ht_printf("[IO/GCARD] <Warning> " __VA_ARGS__)
#define IO_CUDA_WARN(...) ht_printf("[IO/CUDA] <Warning> " __VA_ARGS__)
#define IO_PIC_WARN(...) ht_printf("[IO/PIC] <Warning> " __VA_ARGS__)
#define IO_PCI_WARN(...) ht_printf("[IO/PCI] <Warning> " __VA_ARGS__)
#define IO_MACIO_WARN(...) ht_printf("[IO/MACIO] <Warning> " __VA_ARGS__)
#define IO_NVRAM_WARN(...) ht_printf("[IO/NVRAM] <Warning> " __VA_ARGS__)
#define IO_IDE_WARN(...) ht_printf("[IO/IDE] <Warning> " __VA_ARGS__)
#define IO_USB_WARN(...) ht_printf("[IO/USB] <Warning> " __VA_ARGS__)
#define IO_SERIAL_WARN(...) ht_printf("[IO/SERIAL] <Warning> " __VA_ARGS__)
#define IO_CORE_WARN(...) ht_printf("[IO/Generic] <Warning> " __VA_ARGS__)

#define PPC_CPU_ERR(...) {ht_printf("[CPU/CPU] <Error> " __VA_ARGS__);exit(1); } 
#define PPC_ALU_ERR(...) {ht_printf("[CPU/ALU] <Error> " __VA_ARGS__);exit(1); }
#define PPC_FPU_ERR(...) {ht_printf("[CPU/FPU] <Error> " __VA_ARGS__);exit(1); }
#define PPC_DEC_ERR(...) {ht_printf("[CPU/DEC] <Error> " __VA_ARGS__);exit(1); }
#define PPC_ESC_ERR(...) {ht_printf("[CPU/ESC] <Error> " __VA_ARGS__);exit(1); }
#define PPC_EXC_ERR(...) {ht_printf("[CPU/EXC] <Error> " __VA_ARGS__);exit(1); }
#define PPC_MMU_ERR(...) {ht_printf("[CPU/MMU] <Error> " __VA_ARGS__);exit(1); }
#define PPC_OPC_ERR(...) {ht_printf("[CPU/OPC] <Error> " __VA_ARGS__);exit(1); }
#define IO_PROM_ERR(...) {ht_printf("[IO/PROM] <Error> " __VA_ARGS__);exit(1); }
#define IO_PROM_FS_ERR(...) {ht_printf("[IO/PROM/FS] <Error> " __VA_ARGS__);exit(1); }
#define IO_3C90X_ERR(...) {ht_printf("[IO/3c90x] <Error> " __VA_ARGS__);exit(1); }
#define IO_RTL8139_ERR(...) {ht_printf("[IO/rtl8139] <Error> " __VA_ARGS__);exit(1); }
#define IO_GRAPHIC_ERR(...) {ht_printf("[IO/GCARD] <Error> " __VA_ARGS__);exit(1); }
#define IO_CUDA_ERR(...) {ht_printf("[IO/CUDA] <Error> " __VA_ARGS__);exit(1); }
#define IO_PIC_ERR(...) {ht_printf("[IO/PIC] <Error> " __VA_ARGS__);exit(1); }
#define IO_PCI_ERR(...) {ht_printf("[IO/PCI] <Error> " __VA_ARGS__);exit(1); }
#define IO_MACIO_ERR(...) {ht_printf("[IO/MACIO] <Error> " __VA_ARGS__);exit(1); }
#define IO_NVRAM_ERR(...) {ht_printf("[IO/NVRAM] <Error> " __VA_ARGS__);exit(1); }
#define IO_IDE_ERR(...) {ht_printf("[IO/IDE] <Error> " __VA_ARGS__);exit(1); }
#define IO_USB_ERR(...) {ht_printf("[IO/IDE] <Error> " __VA_ARGS__);exit(1); }
#define IO_SERIAL_ERR(...) {ht_printf("[IO/SERIAL] <Error> " __VA_ARGS__);exit(1); }
#define IO_CORE_ERR(...) {ht_printf("[IO/Generic] <Error> " __VA_ARGS__);exit(1); }

/*
 *
 */
#ifndef PPC_CPU_TRACE
#define PPC_CPU_TRACE(...)
#endif

#ifndef PPC_ALU_TRACE
#define PPC_ALU_TRACE(...)
#endif

#ifndef PPC_FPU_TRACE
#define PPC_FPU_TRACE(...)
#endif

#ifndef PPC_DEC_TRACE
#define PPC_DEC_TRACE(...)
#endif

#ifndef PPC_EXC_TRACE
#define PPC_EXC_TRACE(...)
#endif

#ifndef PPC_ESC_TRACE
#define PPC_ESC_TRACE(...)
#endif

#ifndef PPC_MMU_TRACE
#define PPC_MMU_TRACE(...)
#endif

#ifndef PPC_OPC_TRACE
#define PPC_OPC_TRACE(...)
#endif

#ifndef PPC_OPC_WARN
#define PPC_OPC_WARN(...)
#endif

#ifndef IO_PROM_TRACE
#define IO_PROM_TRACE(...)
#endif

#ifndef IO_PROM_FS_TRACE
#define IO_PROM_FS_TRACE(...)
#endif

#ifndef IO_GRAPHIC_TRACE
#define IO_GRAPHIC_TRACE(...)
#endif

#ifndef IO_CUDA_TRACE
#define IO_CUDA_TRACE(...)
#endif

#ifndef IO_PIC_TRACE
#define IO_PIC_TRACE(...)
#endif

#ifndef IO_PCI_TRACE
#define IO_PCI_TRACE(...)
#endif

#ifndef IO_MACIO_TRACE
#define IO_MACIO_TRACE(...)
#endif

#ifndef IO_ISA_TRACE
#define IO_ISA_TRACE(...)
#endif

#ifndef IO_IDE_TRACE
#define IO_IDE_TRACE(...)
#endif

#ifndef IO_CORE_TRACE
#define IO_CORE_TRACE(...)
#endif

#ifndef IO_NVRAM_TRACE
#define IO_NVRAM_TRACE(...)
#endif

#ifndef IO_USB_TRACE
#define IO_USB_TRACE(...)
#endif

#ifndef IO_SERIAL_TRACE
#define IO_SERIAL_TRACE(...)
#endif

#ifndef IO_3C90X_TRACE
#define IO_3C90X_TRACE(...)
#endif

#ifndef IO_RTL8139_TRACE
#define IO_RTL8139_TRACE(...)
#endif

#endif

