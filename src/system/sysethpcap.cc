/*
 *	PearPC
 *	sysethpcap.cc
 *
 *	pcap-based ethernet access
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

#include "stdafx.h"

#include <errno.h>
#include <stdio.h>
#include <cstring>

#include "system/syseth.h"
#include "tools/except.h"
#include "tools/snprintf.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_PCAP_H
/* Build emulator with support of PCAP network access. */

#include <pcap.h>

/**
@{
\name Dynamic linking framework for win32 and unix.

\def dll_hdl
	\brief Dll handle.
\def dll_proc
	\brief Pointer to symbol in dll,
\def dll_open(str)
	\brief Open a dll. Load a library and do (lazy) dynamic linking.
	\param str Library name string.
	\return Dll handle or NULL.
\def dll_sym(hdl, str)
	\brief Find symbol in dll.
	\param hdl Dll handle.
	\param str Name of symbol to find to.
	\return Pointer to synbol or NULL.
\def dll_close(hdl)
	\brief Close dll handle. Unload a library.
	\param hdl Dll handle.
*/
#if defined(__MINGW32__) || defined(WIN32)
#include "windows.h"
#define dll_hdl			HINSTANCE
#define dll_proc		FARPROC
#define dll_open(str)		LoadLibrary(str)
#define dll_sym(hdl, str)	GetProcAddress(hdl, str)
#define dll_close(hdl)		FreeLibrary(hdl)
#else /*UNIX*/
#include <dlfcn.h>
#define dll_hdl			void*
#define dll_proc		void*
#define dll_open(str)		dlopen(str, RTLD_LAZY)
#define dll_sym(hdl, str)	dlsym(hdl, str)
#define dll_close(hdl)		dlclose(hdl)
#endif
/**@}*/

/**
\def printm(s...)
\brief Vaariadic macro for debug and progress message printing.
 */
#define printm(...)	ht_printf("[PCAP] ", __VA_ARGS__)
#define MAX_PACKET_SIZE	16384

/**
\def LIBPCAP_NAME
\brief The name of PCAP library.
*/
#if defined(__MINGW32__) || defined(WIN32) || defined(__CYGWIN__)
#define LIBPCAP_NAME		"wpcap.dll"
#else
#define LIBPCAP_NAME		"libpcap.so"
#endif

/** \brief Function type for pcap_open_live().  */
typedef pcap_t* (*t_pcap_open_live) (
	const char *device,
	int snaplen,
	int promisc,
	int to_ms,
	char *ebuf);
/** \brief Function type for pcap_close().  */
typedef void (*t_pcap_close) (pcap_t *p);
/** \brief Function type for pcap_next_ex().  */
typedef int (*t_pcap_next_ex) (
	pcap_t *p,
	struct pcap_pkthdr **pkt_header,
	const unsigned char **pkt_data);
/** \brief Function type for pcap_sendpacket().  */
typedef int (*t_pcap_sendpacket) (
	pcap_t *p,
	unsigned char *buf,
	int size);

/**
 *	PCAP interface implementation class.
 */
class GenericEthPcapDevice: public EthPcapDevice {
protected:
	/** \brief PCAP interface name. */
	char mInterfaceName[256];
	/** \brief PCAP interface handle. */
	pcap_t *mPcapHdl;
	/** \brief PCAP dll link handle. */
	dll_hdl mLibPcap;
	/** \brief Open an Ethernet interface for live capture. */
	t_pcap_open_live m_pcap_open_live;
	/** \brief Close PCAP interface. */
	t_pcap_close m_pcap_close;
	/** \brief Read a packet from an PCAP interface. */
	t_pcap_next_ex m_pcap_next_ex;
	/** \brief Send a packet. */
	int (*m_pcap_sendpacket) (
		pcap_t *p,
		unsigned char *buf,
		int size);
	/** \brief Read buffer pointer. */
	unsigned char *mBuf;
	/** \brief Buffer length. */
	unsigned int mBufLen;
	/** \brief Error message buffer. */
	char mErrBuf[PCAP_ERRBUF_SIZE];

/**
\brief Load pcap library and find all symbols.
\return \a 0 if dll load w/o problems, or /a 1 in case of error.
*/
int load_pcap(void) {

	if(mLibPcap != NULL) {
		return 0;
	}

	mLibPcap = dll_open(LIBPCAP_NAME);
	if(mLibPcap == NULL) {
		printm("No " LIBPCAP_NAME " library found! Make sure PCAP is installed.\n");
		return 1;
	}

	m_pcap_open_live = (t_pcap_open_live)dll_sym(mLibPcap, "pcap_open_live");
	if(m_pcap_open_live == NULL) {
		printm("Name pcap_open_live not found! Make sure PCAP is installed.\n");
		unload_pcap();
		return 1;
	}

	m_pcap_close = (t_pcap_close)dll_sym(mLibPcap, "pcap_close");
	if(m_pcap_close == NULL) {
		printm("Name pcap_close not found! Make sure PCAP is installed.\n");
		unload_pcap();
		return 1;
	}

	m_pcap_next_ex = (t_pcap_next_ex)dll_sym(mLibPcap, "pcap_next_ex");
	if(m_pcap_next_ex == NULL) {
		printm("Name pcap_next_ex not found! Make sure PCAP is installed.\n");
		unload_pcap();
		return 1;
	}

	m_pcap_sendpacket = (t_pcap_sendpacket)dll_sym(mLibPcap, "pcap_sendpacket");
	if(m_pcap_sendpacket == NULL) {
		printm("Name pcap_sendpacket not found! Make sure PCAP is installed.\n");
		unload_pcap();
		return 1;
	}

	return 0;
}

/**
\brief Unload pcap library.
\return \a 0.
*/
int unload_pcap(void) {

	if(mLibPcap != NULL) {
		dll_close(mLibPcap);
	}

	mLibPcap = NULL;

	m_pcap_open_live = NULL;
	m_pcap_close = NULL;
	m_pcap_next_ex = NULL;
	m_pcap_sendpacket = NULL;

	return 0;
}

public:
GenericEthPcapDevice(char *interfaceName) {
	mLibPcap = NULL;
	strncpy(mInterfaceName, interfaceName, 255);

	if(load_pcap() != 0) {
		throw MsgException("Can't load PCAP.");
	}
}

virtual ~GenericEthPcapDevice() {

	if(mPcapHdl != NULL) {
		shutdownDevice();
	}

	unload_pcap();
}

virtual uint recvPacket(void *buf, uint size) {

	if(mPcapHdl == NULL) {
		return 0;
	}

	if (mBufLen > size) {
		// no partial packets. drop it.
		mBufLen = 0;
		return 0;
	}
	memcpy(buf, mBuf, mBufLen);

	uint ret = mBufLen;
	mBufLen = 0;

	return ret;
}

virtual int waitRecvPacket() {

	struct pcap_pkthdr *header;
	const unsigned char *pkt_data;
	int ret;

	if(mPcapHdl == NULL) {
		return 0;
	}

	while((ret = m_pcap_next_ex(mPcapHdl, &header, &pkt_data)) == 0);

	if(ret == 1) { // OK
		mBuf = (unsigned char *)pkt_data;
		mBufLen = header->len;
	} else { // error
		mBufLen = 0;
		printm("Error: %s\n", mErrBuf);
		return EIO;
	}

	return 0;
}

virtual uint getWriteFramePrefix() {
	return 0;
}

virtual uint sendPacket(void *buf, uint size) {

	if(mPcapHdl == NULL) {
		return 0;
	}

	if(m_pcap_sendpacket(mPcapHdl, (unsigned char*)buf, size) == 0) {
		return size;
	}

	return 0;
}

virtual int initDevice() {

	mPcapHdl = m_pcap_open_live(
		mInterfaceName,
		MAX_PACKET_SIZE,
		1,
		1,
		mErrBuf);
	if(mPcapHdl == NULL) {
		throw MsgfException("Can't open interface %s\n", mInterfaceName);
	}

	// add filter here?

	return 0;
}

virtual int shutdownDevice() {

	if(mPcapHdl == NULL) {
		return 0;
	}

	m_pcap_close(mPcapHdl);
	mPcapHdl = NULL;

	return 0;
}

}; // end of GenericEthPcapDevice

/**
 *	Create an pcap-conection to real a phisycal Ethernet card.
 */
EthDevice *createEthernetPcap(char *interfaceName) {
	return new GenericEthPcapDevice(interfaceName);
}

#else/*HAVE_PCAP_H*/
/* Build emulator w/o support of PCAP network access. */

/**
 *	Fake create an pcap-conection function.
 */
EthDevice *createEthernetPcap(char *interfaceName) {
	throw MsgException("Your build doesn't support PCAP.");
}

#endif/*HAVE_PCAP_H*/
