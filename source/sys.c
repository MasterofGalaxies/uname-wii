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

#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <malloc.h>
#include <string.h>
#include <network.h>
#include <wiiuse/wpad.h>
#include <ogc/conf.h>

#include "../include/sys.h"

u32 getSysMenuVersion()
{
	// get the System Menu version from TMD (title metadata)

	static u64 TitleID ATTRIBUTE_ALIGN(32) = 0x0000000100000002LL;
	static u32 tmdSize ATTRIBUTE_ALIGN(32);
	
	if (ES_GetStoredTMDSize(TitleID, &tmdSize) < 0)
	{
		printf("\n[!] failed to get the stored TMD size for the system menu\n\n");
		
		return false;
	}
	
	signed_blob *TMD = (signed_blob *)memalign(32, (tmdSize+32)&(~31));
	memset(TMD, 0, tmdSize);
	
	if (ES_GetStoredTMD(TitleID, TMD, tmdSize) < 0)
	{
		printf("\n[!] failed to get the stored TMD for the system menu\n\n");
		
		return false;
	}
	
	tmd *rTMD = (tmd *)(TMD+(0x140/sizeof(tmd *)));
	u32 version = rTMD->title_version;
	
	free(TMD);
	return version;
}

u32 getDeviceID()
{
	u32 deviceID = 0;
	
	if (ES_GetDeviceID(&deviceID) < 0)
	{
		printf("\n[!] failed getting the console ID\n\n");
		deviceID = 0;
	}
	
	return deviceID;
}

u32 getboot2version()
{
	u32 version = 0;
	
	if (ES_GetBoot2Version(&version) < 0)
	{
		printf("\n[!] failed getting boot2 version\n\n");
		version = 0;
	}
	
	return version;
}

int getRegion()
{
	int region = CONF_GetRegion();
	
	return region;
}
int getLanguage()
{
	int lang = CONF_GetLanguage();
	
	return lang;
}
int getArea()
{
	int area = CONF_GetArea();
	
	return area;
}
int getVideo()
{
	int video = CONF_GetVideo();
	
	return video;
}

void getNetworkData(Addr *ip_address, Addr *netmask,  Addr *gateway, u8 *mac_address)
{
	if (if_configex((In_addr *)ip_address, (In_addr *)netmask, (In_addr *)gateway, true, 5) < 0)
		printf("\n[!] failed getting network data\n\n");

	if (net_get_mac_address(mac_address) < 0)
		printf("\n[!] failed getting MAC address\n\n");
}

u32 getNumOfTitles()
{
	u32 nTitles;

	if (ES_GetNumTitles(&nTitles) < 0)
		printf("\n[!] failed getting number of titles\n\n");

	return nTitles;
}

int little_endian()
{
	int integer = 0x0a0b0c0d;
	char *ptr = (char *)&integer;

	if (ptr[0] == 0x0a)
		return 0;
	else
		return 1;
}
