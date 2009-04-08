/*
 *	PearPC
 *	info.h
 *
 *	Copyright (C) 2003-2005 Sebastian Biallas (sb@biallas.net)
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

#ifndef __INFO_H__
#define __INFO_H__

#include "build_number.h"

#define APPNAME "PearPC"
#define APPVERSION PPC_VERSION_STRING
#define APPVERSION_IN_NUMBERS PPC_RESOURCE_VERSION
#define COPYRIGHT "(c) 2009 Steven Noonan <steven@uplinklabs.net>\n(c) 2003-2005 Sebastian Biallas <sb@biallas.net>"

#define EMULATOR_MODEL "PowerPC ("APPNAME" "APPVERSION")"

//#define PPC_CPU_ENABLE_SINGLESTEP

#endif

