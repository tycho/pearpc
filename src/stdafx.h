/*
 *	PearPC
 *
 *	Copyright (C) 2009 Steven Noonan (steven@uplinklabs.net)
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

#ifndef __included_ppc_stdafx_h
#define __included_ppc_stdafx_h

#include "platform_detect.h"

#if defined   TARGET_OS_MACOSX
#  include "config/macosx.h"
#elif defined TARGET_OS_LINUX
#  include "config/linux.h"
#elif defined TARGET_OS_WINDOWS
#  include "config/windows.h"
#else
#  error "No configuration has been set up for your platform."
#endif

#endif
