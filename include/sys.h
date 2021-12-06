/*
 * uname - print system information (Wii)
 *
 * Copyright 2011  Benjamin Randazzo <benjamin@linuxcrashing.org>
 *
 * This code is licensed to you under the terms of the GNU GPL, version 2;
 * see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef SYS_H
#define SYS_H

#ifdef __cplusplus
extern "C"
{
#endif

#define CPU_FREQUENCY	729
#define GPU_FREQUENCY	243
#define NAND_SIZE	512
#define MAIN_MEM_SIZE	88
#define INT_MEM_SIZE	24
#define EXT_MEM_SIZE	64

typedef struct Addr {
	unsigned char b1;
	unsigned char b2;
	unsigned char b3;
	unsigned char b4;
} Addr;

typedef struct in_addr In_addr;

u32 getSysMenuVersion();
u32 getDeviceID();
u32 getboot2version();
int getRegion();
void getNetworkData(Addr *ip_address, Addr *netmask,  Addr *gateway, u8 *mac_address);
u32 getNumOfTitles();
int little_endian();

#ifdef __cplusplus
}
#endif

#endif
