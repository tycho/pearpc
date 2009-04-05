/*
 *	PearPC
 *	sparsedisk.h
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

#ifndef __SPARSEDISK_H__
#define __SPARSEDISK_H__

#include "system/file.h"
#include "idedevice.h"
#include "ata.h"

class SparseDeviceFile: public ATADevice {
	SYS_FILE *mFile;

private:
	void 	initVariables();
	void 	readMetaData();
	void 	newCluster();
	uint16 	getBlockFlags(uint64 offset);
	void 	setBlockFlags(uint64 offset, uint16 bits);
	void	freeCluster(uint32 clusternumber);

	// WARNING !
	// The values below are written directly to disk.
	// Changing a variable from uint32 to uint64 will
	// create a new header layout unless you cast it
	// before writing and reading.
	
	// If you do need to change them, increase the version number
	// so old implementations will not try to use the disk.

	uint32	magic1, magic2;		// file identifiers	
	uint32 	version;			// version number to distinguish different header formats
	
#define MAX_VERSION_SUPPORTED 1	// maximum version we support currently
	
	uint32	flags;				// none used currently
	uint64	usedclustersoffset;	// remember where usedclusters is stored in file	
	uint64 	usedclusters;		// number of clusters in use

	uint32 	blocksize;			// size of one block in bytes
	uint32 	heads;				// disk geometry
	uint32 	cylinders;
	uint32 	sectorspertrack;
	
	uint32 	clusterbits;		// size of one cluster = (1 << clusterbits) bytes
	uint64 	clusterlutoffset;	// offset in file at which cluster lut starts

	uint64 	blockflagsoffset;	// offset in file at which block flags start
	uint64 	headersize;			// offset at which first cluster is written

	//  The values below are calculated and safe to change

	uint64 	disksize;			// calculated
	uint64 	currentseek;		// currentseek points to a location on disk (not file !)
	
	// calculated from blocksize
	uint32 	blockbits;			// (1 << blockbits) = blocksize
	uint64 	blockmask;			// mask to get the byte offset within a block


	uint64 	clustermask;		// mask to get the byte offset within a cluster		
	uint64 	*clusteroffset;		// cluster offsets
	uint32 	clustercount;		// calculated from disksize and clusterbits
	
	byte 	*blockflags;		// flags with block meta data
	uint64 	blockflagssize;
	
	
	// to keep total count of used blocks in
	// a cluster. This will be used for shrinking disks
	
	typedef struct clusterflagtotal {
		uint32	nonzero;
		uint32	datavalid;	
	} clusterflagtotal;

	clusterflagtotal *flagtotals;	// counts of flags per cluster

	
public:
	virtual void 	writeValue(uint32 value);
	virtual void	writeValue(uint64 value);
	virtual void 	readValue(uint32 *value);
	virtual void 	readValue(uint64 *value);
	virtual void	createSparseDeviceFile(const char *filename, uint32 cyls, uint32 cbits);
	virtual void	setClusterOffset(uint32 clusternumber, uint64 offset);
	SparseDeviceFile(const char *name, const char *filename);
	virtual ~SparseDeviceFile();

	virtual bool	seek(uint64 blockno);
	virtual void	flush();
	virtual int		readBlock(byte *buf);
	virtual int		writeBlock(byte *buf);

	virtual bool	promSeek(uint64 pos);
	virtual uint	promRead(byte *buf, uint size);
	
};

 
enum {
	BLOCK_FLAG_NONZERO_BIT = 0,
	BLOCK_FLAG_DATAVALID_BIT,
	BLOCK_FLAG_NUMBER_OF_BITS
};

#define BLOCK_FLAG_NONZERO (1 << BLOCK_FLAG_NONZERO_BIT)
#define BLOCK_FLAG_DATAVALID (1 << BLOCK_FLAG_DATAVALID_BIT)

#define SPARSEMAGIC1 (((uint32) 'S' << 0) | ((uint32) 'p' << 8) | ((uint32) 'a' << 16) | ((uint32) 'r' << 24))
#define SPARSEMAGIC2 (((uint32) 's' << 0) | ((uint32) 'e' << 8) | ((uint32) 'H' << 16) | ((uint32) 'D' << 24))

#endif
