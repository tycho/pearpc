/*
 *	PearPC
 *	sparsedisk.cc
 *
 *	Copyright (C) 2005 Frank van Beek
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License version 2 as
 *	published by the Free Software Foundation.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "stdafx.h"

#include <cstdio>
#include <cstring>
#include <limits.h>
#include <errno.h>

#include "debug/tracers.h"
#include "ata.h"
#include "sparsedisk.h"

#include "tools/snprintf.h"

/*
 *	TODO:
 *	 
 *  - make patch and submit it 
 *  
 */ 

// #undef HOST_ENDIANESS
// #define HOST_ENDIANESS HOST_ENDIANESS_BE

void SparseDeviceFile::writeValue(uint32 value)
{
#if HOST_ENDIANESS == HOST_ENDIANESS_LE
	sys_fwrite(mFile, (byte *) &value, sizeof(value));
#else
	uint8 buffer[sizeof(value)];
	
	for (int i = 0; i < sizeof(buffer); i++)
	{
		buffer[i] = (uint8) value;
		value >>= 8;
	}
	
	sys_fwrite(mFile, (byte *) buffer, sizeof(buffer));
#endif
}


void SparseDeviceFile::writeValue(uint64 value)
{
#if HOST_ENDIANESS == HOST_ENDIANESS_LE
	sys_fwrite(mFile, (byte *) &value, sizeof(value));
#else
	uint8 buffer[sizeof(value)];
	
	for (int i = 0; i < sizeof(buffer); i++)
	{
		buffer[i] = (uint8) value;
		value >>= 8;
	}
	
	sys_fwrite(mFile, (byte *) buffer, sizeof(buffer));
#endif
}


void SparseDeviceFile::readValue(uint32 *value)
{
#if HOST_ENDIANESS == HOST_ENDIANESS_LE
	sys_fread(mFile, (byte *) value, sizeof(*value));
#else
	uint8 buffer[sizeof(*value)];
	uint32 v;
	
	// initialize buffer with the old value
	// so if sys_fread fails, value will be (partially) preserved
	v = *value;
	for (int i = 0; i < sizeof(buffer); i++)
	{
		buffer[i] = (uint8) v;
		v >>= 8;
	}

	sys_fread(mFile, (byte *) buffer, sizeof(buffer));
	
	v = 0;
	for (int i = sizeof(buffer) - 1; i >= 0 ; i--)
	{
		v = (v << 8) | (uint32) buffer[i];
	}
	
	*value = v;
#endif
}


void SparseDeviceFile::readValue(uint64 *value)
{
#if HOST_ENDIANESS == HOST_ENDIANESS_LE
	sys_fread(mFile, (byte *) value, sizeof(*value));
#else
	uint8 buffer[sizeof(*value)];
	uint64 v;

	// initialize buffer with the old value
	// so if sys_fread fails, value will be (partially) preserved
	v = *value;
	for (int i = 0; i < sizeof(buffer); i++)
	{
		buffer[i] = (uint8) v;
		v >>= 8;
	}

	sys_fread(mFile, (byte *) buffer, sizeof(buffer));

	v = 0;
	for (int i = sizeof(buffer) - 1; i >= 0 ; i--)
	{
		v = (v << 8) | (uint64) buffer[i];
	}

	*value = v;
#endif
}

void SparseDeviceFile::readMetaData()
{
	// allocate and read all cluster offsets
	clusteroffset = (uint64 *) calloc(clustercount, sizeof(clusteroffset[0]));
	// on a new disk, reading these values is allowed to fail
	sys_fseek(mFile, clusterlutoffset);
	for (uint32 i = 0; i < clustercount; i++) {
		readValue(&clusteroffset[i]);
	}
	
	// allocate and read all blockflags
	blockflags = (byte *) calloc(blockflagssize, 1);
	// on a new disk, reading these values is allowed to fail
	sys_fseek(mFile, blockflagsoffset);
	sys_fread(mFile, blockflags, blockflagssize);
	
	// sum all blockflags for each cluster
	flagtotals = (clusterflagtotal *) calloc(clustercount, sizeof(clusterflagtotal));

	for (uint64 offset = 0; offset < disksize; offset += blocksize) {
		byte flags = getBlockFlags(offset);
		
		if (flags & BLOCK_FLAG_NONZERO) {
			flagtotals[offset >> clusterbits].nonzero++;
		}
		
		if (flags & BLOCK_FLAG_DATAVALID) {
			flagtotals[offset >> clusterbits].datavalid++;
		}
	}
}


void SparseDeviceFile::initVariables()
{
	init(heads, cylinders, sectorspertrack);
	blocks = ((uint64) heads) * cylinders * sectorspertrack;
	disksize = ((uint64) blocksize) * blocks;

	clustermask = (1 << clusterbits) - 1;
	blockmask = blocksize - 1;
	
	blockbits = 0;
	while ((1UL << blockbits) < blocksize) {
		blockbits++;
	}
	
	// calculate number of clusters
	// add one cluster to make sure any partial last cluster doesn't get lost
	clustercount = (disksize >> clusterbits) + 1ULL;
		
	blockflagssize = ((blocks * BLOCK_FLAG_NUMBER_OF_BITS) + 7) >> 3;
}


void SparseDeviceFile::createSparseDeviceFile(const char *filename, uint32 cyls, uint32 cbits)
{
	if ((1 << cbits) < getBlockSize())
	{
		char buf[2560];
		ht_snprintf(buf, sizeof buf, "SparseDeviceFile::createSparseDeviceFile size of one cluster (%d) is smaller then blocksize (%d). Increase clusterbits.", 1 << cbits, getBlockSize());
		setError(buf);
		return;
	}
	
	// this code creates a new sparsedisk disk
	mFile = sys_fopen(filename, SYS_OPEN_READ | SYS_OPEN_WRITE | SYS_OPEN_CREATE);
	if (mFile) {
		magic1 = SPARSEMAGIC1;
		writeValue(magic1);
		magic2 = SPARSEMAGIC2;
		writeValue(magic2);
		version = MAX_VERSION_SUPPORTED;
		writeValue(version);
		flags = 0;
		writeValue(flags);
		
		usedclustersoffset = sizeof(magic1) + sizeof(magic2) + sizeof(version) + sizeof(flags) + sizeof(usedclustersoffset);
		writeValue(usedclustersoffset);
		
		usedclusters = 0;
		writeValue(usedclusters);
		
		// write disk geometry
		blocksize = getBlockSize();
		writeValue(blocksize);
		heads = 16;
		writeValue(heads);
		cylinders = cyls;
		writeValue(cylinders);
		sectorspertrack = 63;
		writeValue(sectorspertrack);
		
		// 2 ^ clusterbits is the size of one cluster in bytes
		clusterbits = cbits;
		writeValue(clusterbits);
		
		// some room to spare ...
		clusterlutoffset = 4096;
		writeValue(clusterlutoffset);
		
		FileOfs headerseek = sys_ftell(mFile);;
		initVariables();
		sys_fseek(mFile, headerseek);

		blockflagsoffset = clusterlutoffset + (clustercount * sizeof(clusteroffset[0]));
		writeValue(blockflagsoffset);
		
		headersize = blockflagsoffset + blockflagssize;
		
		// round headersize up to next blocksize boundary
		headersize = (headersize + blocksize - 1) & ~blockmask;
		writeValue(headersize);
		
		readMetaData();
	} else {
		char buf[2560];
		ht_snprintf(buf, sizeof buf, "%s: could not create file (%s)", filename, strerror(errno));
		setError(buf);
	}
}


SparseDeviceFile::SparseDeviceFile(const char *name, const char *filename)
	: ATADevice(name)
{
	mType = "SHDD";
	mFile = sys_fopen(filename, SYS_OPEN_READ | SYS_OPEN_WRITE);
	if (mFile) {
		readValue(&magic1);
		readValue(&magic2);

		if (magic1 != SPARSEMAGIC1 || magic2 != SPARSEMAGIC2) {
			setError("SparseDeviceFile invalid format unrecognized header");
		} else {
			version = UINT_MAX;

			readValue(&version);
			if (version > MAX_VERSION_SUPPORTED) {
				setError("SparseDeviceFile::SparseDeviceFile Unsupported version. Files version number is to high.");
				sys_fclose(mFile);
			} else {
				readValue(&flags);
				readValue(&usedclustersoffset);
				readValue(&usedclusters);
				
				// read disk geometry
				readValue(&blocksize);
				readValue(&heads);
				readValue(&cylinders);
				readValue(&sectorspertrack);
				
				if (blocksize != 512 || sectorspertrack != 63 || heads != 16) {
					char buf[2560];
					ht_snprintf(buf, sizeof buf, "SparseDeviceFile::SparseDeviceFile unsupported disk geometry: blocksize %d, sectorsize %d, heads %d", blocksize, sectorspertrack, heads);
					setError(buf);
					sys_fclose(mFile);
				} else {
					readValue(&clusterbits);
					readValue(&clusterlutoffset);
					readValue(&blockflagsoffset);
					readValue(&headersize);

					initVariables();
					readMetaData();
				}
			}
		}
	} else {
		/*
		char buf[2560];
		ht_snprintf(buf, sizeof buf, "%s: could not open file (%s)", filename, strerror(errno));
		setError(buf);
		*/
		
		// createSparseDeviceFile(filename, 256, 20); // +- 0.125 GB
		// createSparseDeviceFile(filename, 6241, 20); // 3 GB
		createSparseDeviceFile(filename, 6241 + 6241, 20); // 6 GB
	}
}


SparseDeviceFile::~SparseDeviceFile()
{
	if (clusteroffset) {
		free(clusteroffset);
		clusteroffset = 0;
	}

	if (blockflags) {
		free(blockflags);
		blockflags = 0;
	}

	if (clusteroffset) {
		free(clusteroffset);
		clusteroffset = 0;
	}
	
	if (mFile) {
		sys_fclose(mFile);
		mFile = NULL;
	}
}


bool SparseDeviceFile::seek(uint64 blockno)
{
	currentseek = blockno * blocksize;
	return true;
}


void SparseDeviceFile::flush()
{
	sys_flush(mFile);
}


void SparseDeviceFile::setClusterOffset(uint32 clusternumber, uint64 offset)
{
	// update array
	clusteroffset[clusternumber] = offset;

	// and write to disk
	sys_fseek(mFile, clusterlutoffset + clusternumber * sizeof(clusteroffset[0]));
	writeValue(clusteroffset[clusternumber]);
}


void SparseDeviceFile::newCluster()
{
	if (currentseek < disksize) {
		uint64 clusternumber = currentseek >> clusterbits;

		if (clusteroffset[clusternumber] == 0) {
			setClusterOffset(clusternumber, headersize + (usedclusters << clusterbits));
			
			// write modified usedclusters in fileheader to disk
			usedclusters++;
			sys_fseek(mFile, usedclustersoffset);
			writeValue(usedclusters);
		}
	} else {
		ht_printf("SparseDeviceFile::newCluster failed: seek = %08x %08x\n", (uint32) (currentseek >> 32), (uint32) currentseek);
	}
}


void SparseDeviceFile::freeCluster(uint32 clusternumber)
{
	uint32 highestcluster = 0;
	uint64 highestoffset = 0;
	
	// find last cluster in file
	for (uint32 i = 0; i < clustercount; i++) {
		if (clusteroffset[i] > highestoffset) {
			highestcluster = i;
			highestoffset = clusteroffset[i];
		}
	}
	
	if (highestoffset != (headersize + ((usedclusters - 1) << clusterbits))) {
		ht_printf("SparseDeviceFile::freeCluster abort: disk image inconsistent\n");
	} else {
		uint64 from = clusteroffset[highestcluster];
		uint64 to = clusteroffset[clusternumber];
		
		ht_printf("SparseDeviceFile::freeCluster moving %08x %08x -> %08x %08x\n", (uint32) (from >> 32), (uint32) from, (uint32) (to >> 32), (uint32) to);
		
		// only move data around if we're not deleting the last cluster 
		if (clusternumber != highestcluster) {
			// allocate buffer to move data
			byte *buffer = (byte *) malloc(1 << clusterbits);
			if (buffer) {
				// move cluster data
				sys_fseek(mFile, from);
				sys_fread(mFile, buffer, 1 << clusterbits);
				sys_fseek(mFile, to);
				sys_fwrite(mFile, buffer, 1 << clusterbits);
			
				setClusterOffset(highestcluster, clusteroffset[clusternumber]);
				
				free(buffer);
			} else {
				ht_printf("SparseDeviceFile::freeCluster abort: out of memory\n");
				return;
			}
		}
		
		setClusterOffset(clusternumber, 0);

		// write modified usedclusters in fileheader to disk
		usedclusters--;
		sys_fseek(mFile, usedclustersoffset);
		writeValue(usedclusters);
		
		// and finally shrink the file
		sys_truncate_sys_file(mFile, from);		
	}
}


uint16 SparseDeviceFile::getBlockFlags(uint64 offset)
{
	uint32 block = offset >> blockbits;
	uint32 bitoffset = block * BLOCK_FLAG_NUMBER_OF_BITS;
	uint32 byteoffset = bitoffset >> 3;
	uint16 allbits = blockflags[byteoffset] | (blockflags[byteoffset + 1] << 8);
	
	uint16 bits = allbits >> (bitoffset & 7);
	
	// mask out the requested bits
	bits &= (1 << BLOCK_FLAG_NUMBER_OF_BITS) - 1;
	// ht_printf("SparseDeviceFile::getBlockFlags %d %d \n", block, bitoffset);
	
	return (bits);
}


void SparseDeviceFile::setBlockFlags(uint64 offset, uint16 bits)
{
	uint32 block = offset >> blockbits;
	uint32 bitoffset = block * BLOCK_FLAG_NUMBER_OF_BITS;
	uint32 byteoffset = bitoffset >> 3;
	uint16 allbits = blockflags[byteoffset] | (blockflags[byteoffset + 1] << 8);

	// mask out the bits we want to set	
	uint16 mask = (1 << BLOCK_FLAG_NUMBER_OF_BITS) - 1;
	mask = mask << (bitoffset & 7);
	allbits &= ~mask;
	
	// keep only the lower bits
	bits &= (1 << BLOCK_FLAG_NUMBER_OF_BITS) - 1;
	
	// and shift them to the correct position
	bits = bits << (bitoffset & 7);
	
	// add the masked bits and the new bits
	allbits += bits;
	
	blockflags[byteoffset] = allbits;
	blockflags[byteoffset + 1] = allbits >> 8;	
	
	// write out the new data
	sys_fseek(mFile, blockflagsoffset + byteoffset);
	sys_fwrite(mFile, blockflags + byteoffset, 2);
}

 
int SparseDeviceFile::readBlock(byte *buf)
{
	// clip currentseek to blocksize boundaries
	currentseek &= ~blockmask;
	
	if (currentseek + blocksize <= disksize) {
		uint64 clusternumber = currentseek >> clusterbits;
		uint16 flags = getBlockFlags(currentseek);
		
		if ((flags & BLOCK_FLAG_NONZERO) && (clusteroffset[clusternumber] != 0)) {
			uint64 actualseek = clusteroffset[clusternumber] + (currentseek & clustermask);

			sys_fseek(mFile, actualseek);
			sys_fread(mFile, buf, blocksize);
		} else {
			// return zero's
			memset(buf, 0, blocksize);
		}
		
		currentseek += blocksize;
	} else {
		ht_printf("SparseDeviceFile::readBlock failed (read beyond end of disk): seek = %08x %08x\n", (uint32) (currentseek >> 32), (uint32) currentseek);
	}

	if (mMode & ATA_DEVICE_MODE_ECC) {
		// add ECC bytes..
		IO_IDE_ERR("ATADeviceFile: ECC not implemented\n");
	}
	return 0;
}


int SparseDeviceFile::writeBlock(byte *buf)
{
	// clip currentseek to blocksize boundaries
	currentseek &= ~blockmask;

	if (currentseek + blocksize < disksize) {
		uint64 clusternumber = currentseek >> clusterbits;
		uint16 oldflags = getBlockFlags(currentseek);
		uint16 newflags = BLOCK_FLAG_DATAVALID;
		
		for (uint i = 0 ; i < blocksize; i++) {
			if (buf[i]) {
				newflags |= BLOCK_FLAG_NONZERO;
				break;
			}
		}
		
		// create a new cluster if there isn't one allready
		// and we're supposed to write nonzero data
		if ((clusteroffset[clusternumber] == 0) && (newflags & BLOCK_FLAG_NONZERO)) {
			newCluster();
		}
		
		if (clusteroffset[clusternumber]) {
			uint64 actualseek = clusteroffset[clusternumber] + (currentseek & clustermask);
			
			// TODO: add some errorhandling here
			// and in ata.cc and ide.cc as well
			sys_fseek(mFile, actualseek);
			sys_fwrite(mFile, buf, blocksize);
						
			actualseek += blocksize;
		}
		
		if (newflags != oldflags) {
			// flags changed: update totals
			if (oldflags & BLOCK_FLAG_NONZERO) {
				flagtotals[clusternumber].nonzero--;
			}
			if (oldflags & BLOCK_FLAG_DATAVALID) {
				flagtotals[clusternumber].datavalid--;
			}
			if (newflags & BLOCK_FLAG_NONZERO) {
				flagtotals[clusternumber].nonzero++;
			}
			if (newflags & BLOCK_FLAG_DATAVALID) {
				flagtotals[clusternumber].datavalid++;
			}
			
			setBlockFlags(currentseek, newflags);
		}
		
		if ((flagtotals[clusternumber].nonzero == 0) && (clusteroffset[clusternumber] != 0)) {
			// this cluster only contains zeros: remove it
			freeCluster(clusternumber);
		}
		
		currentseek += blocksize;
	} else {
		ht_printf("SparseDeviceFile::writeBlock abort: write beyond end of disk. seek = %08x %08x\n", (uint32) (currentseek >> 32), (uint32) currentseek);
	}
	return 0;
}


bool SparseDeviceFile::promSeek(FileOfs pos)
{
	if (pos < disksize) {
		currentseek = pos;
		return 1;
	} else {
		return 0;
	}
}


uint SparseDeviceFile::promRead(byte *buf, uint size)
{
	byte temp[blocksize];
	uint64 firstbyte;
	uint64 origseek = currentseek;
	uint done = 0;
	
	// read individual blocks until all
	// requested data has been read
	while (size > 0 && currentseek < disksize) {
		uint bytes;
		
		// what's the first byte in the block we are interesed in
		firstbyte = currentseek & blockmask;
		// how many bytes can we read from firstbyte till end of block
		bytes = blocksize - firstbyte;
		// to many? clip it to size
		if (bytes > size) {
			bytes = size;
		}
		
		// clip currentseek to blocksize boundaries
		currentseek &= ~blockmask;
		readBlock(temp);
		
		memcpy(buf, temp + firstbyte, bytes);
		// currentseek is automatically updated by readBlock
		size -= bytes;
		done += bytes;
		buf += bytes;
	}
	
	// restore and update currentseek
	currentseek = origseek + done;
	
	return (done);
}
