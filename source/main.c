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
#include <network.h>
#include <string.h>
#include <wiiuse/wpad.h>

#include "../include/sys.h"

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

int main(int argc, char **argv)
{
	const char version[] = "v0.3a";
	u32 sys_version, nTitles;
	int region, lang, area, video, i;
	Addr ip_address, netmask, gateway;
	u8 mac_address[6];
	unsigned char n;

	// Initialise the video system
	VIDEO_Init();
	
	// This function initialises the attached controllers
	WPAD_Init();
	
	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);

	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	
	// Initialise the console, required for printf
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	
	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);
	
	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);
	
	// Make the display visible
	VIDEO_SetBlack(FALSE);

	// Flush the video register changes to the hardware
	VIDEO_Flush();

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

	// The console understands VT terminal escape codes
	// This positions the cursor on row 2, column 0
	// we can use variables for this with format codes too
	// e.g. printf ("\x1b[%d;%dH", row, column );
	printf("\x1b[2;0H");
	
	printf("uname %s\n", version);
	printf("2011 Benjamin Randazzo, modified by wiisixtyfour (AKA bg4545)\n\n");
	
	printf("System Menu version: ");

	sys_version = getSysMenuVersion();

	switch (sys_version)
	{
		case 33: printf("1.0 (%d)\n", sys_version); break;
		case 128: printf("2.0J (%d)\n", sys_version); break;
		case 97: printf("2.0U (%d)\n", sys_version); break;
		case 130: printf("2.0E (%d)\n", sys_version); break;
		case 162: printf("2.1E (%d)\n", sys_version); break;
		case 192: printf("2.2J (%d)\n", sys_version); break;
		case 193: printf("2.2U (%d)\n", sys_version); break;
		case 194: printf("2.2E (%d)\n", sys_version); break;
		case 224: printf("3.0J (%d)\n", sys_version); break;
		case 225: printf("3.0U (%d)\n", sys_version); break;
		case 226: printf("3.0E (%d)\n", sys_version); break;
		case 256: printf("3.1J (%d)\n", sys_version); break;
		case 257: printf("3.1U (%d)\n", sys_version); break;
		case 258: printf("3.1E (%d)\n", sys_version); break;
		case 288: printf("3.2J (%d)\n", sys_version); break;
		case 289: printf("3.2U (%d)\n", sys_version); break;
		case 290: printf("3.2E (%d)\n", sys_version); break;
		case 352: printf("3.3J (%d)\n", sys_version); break;
		case 353: printf("3.3U (%d)\n", sys_version); break;
		case 354: printf("3.3E (%d)\n", sys_version); break;
		case 326: printf("3.3K (%d)\n", sys_version); break;
		case 384: printf("3.4J (%d)\n", sys_version); break;
		case 385: printf("3.4U (%d)\n", sys_version); break;
		case 386: printf("3.4E (%d)\n", sys_version); break;
		case 390: printf("3.5K (%d)\n", sys_version); break;
		case 416: printf("4.0J (%d)\n", sys_version); break;
		case 417: printf("4.0U (%d)\n", sys_version); break;
		case 418: printf("4.0E (%d)\n", sys_version); break;
		case 448: printf("4.1J (%d)\n", sys_version); break;
		case 449: printf("4.1U (%d)\n", sys_version); break;
		case 450: printf("4.1E (%d)\n", sys_version); break;
		case 454: printf("4.1K (%d)\n", sys_version); break;
		case 480: printf("4.2J (%d)\n", sys_version); break;
		case 481: printf("4.2U (%d)\n", sys_version); break;
		case 482: printf("4.2E (%d)\n", sys_version); break;
		case 486: printf("4.2K (%d)\n", sys_version); break;
		case 512: printf("4.3J (%d)\n", sys_version); break;
		case 513: printf("4.3U (%d)\n", sys_version); break;
		case 514: printf("4.3E (%d)\n", sys_version); break;
		case 518: printf("4.3K (%d)\n", sys_version); break;
		default: printf("Unknown (%d)\n", sys_version); break;
	}

	printf("Device ID:           %d\n", getDeviceID());
	printf("boot2 version:       %d\n", getboot2version());
	printf("IOS version:         %d (rev %d)\n", IOS_GetVersion(), IOS_GetRevision());

	nTitles = getNumOfTitles();
	printf("Titles found:        %d\n", nTitles);

	printf("Region:              ");
	
	region = getRegion();
	switch (region)
	{
		case CONF_REGION_US:
			printf("US\n");
			break;
		case CONF_REGION_EU:
			printf("EU/AUS\n");
			break;
		case CONF_REGION_JP:
			printf("JAP\n");
			break;
		case CONF_REGION_KR:
			printf("KOR\n");
			break;
		case CONF_REGION_CN:
			printf("CN\n");
			break;
		default:
			printf("Unknown\n");
			break;
	}

	printf("Language:            ");
	
	lang = getLanguage();
	switch (lang)
	{
		case CONF_LANG_JAPANESE:
			printf("Japanese\n");
			break;
		case CONF_LANG_ENGLISH:
			printf("English\n");
			break;
		case CONF_LANG_GERMAN:
			printf("German\n");
			break;
		case CONF_LANG_FRENCH:
			printf("French\n");
			break;
		case CONF_LANG_SPANISH:
			printf("Spanish\n");
			break;
		case CONF_LANG_ITALIAN:
			printf("Italian\n");
			break;
		case CONF_LANG_DUTCH:
			printf("Dutch\n");
			break;
		case CONF_LANG_SIMP_CHINESE:
			printf("Chinese (Simplified)\n");
			break;
		case CONF_LANG_TRAD_CHINESE:
			printf("Chinese (Traditional)\n");
			break;
		case CONF_LANG_KOREAN:
			printf("Korean\n");
			break;
		default:
			printf("Unknown\n");
			break;
	}
	
	printf("Area:                ");
	
	area = getArea();
	switch (area)
	{
		case CONF_AREA_JPN:
			printf("Japan\n");
			break;
		case CONF_AREA_USA:
			printf("United States\n");
			break;
		case CONF_AREA_EUR:
			printf("Europe\n");
			break;
		case CONF_AREA_AUS:
			printf("Australia\n");
			break;
		case CONF_AREA_BRA:
			printf("Brazil\n");
			break;
		case CONF_AREA_TWN:
			printf("Taiwan\n");
			break;
		case CONF_AREA_ROC:
			printf("China\n");
			break;
		case CONF_AREA_KOR:
			printf("Korea\n");
			break;
		case CONF_AREA_HKG:
			printf("Hong Kong\n");
			break;
		case CONF_AREA_ASI:
			printf("Asia\n");
			break;
		case CONF_AREA_LTN:
			printf("Latin America\n");
			break;
		case CONF_AREA_SAF:
			printf("South Africa\n");
			break;
		default:
			printf("Unknown\n");
			break;
	}
	
	printf("Video Mode:          ");
	
	video = getVideo();
	switch (video)
	{
		case CONF_VIDEO_NTSC:
			printf("NTSC\n");
			break;
		case CONF_VIDEO_PAL:
			printf("PAL\n");
			break;
		case CONF_VIDEO_MPAL:
			printf("MPAL\n");
			break;
		default:
			printf("Unknown\n");
			break;
	}

	printf("CPU:                 Broadway IBM PowerPC @ %dMHz, byte order is ", CPU_FREQUENCY);
	if (little_endian())
		printf("little-endian\n");
	else
		printf("big-endian\n");

	printf("GPU:                 Hollywood v0x%x ATI graphics @ %dMHz\n", *(u32 *)0x80003138, GPU_FREQUENCY);
	printf("                     -> Starlet NEC ARM9 @ %dMHz\n", GPU_FREQUENCY);


	printf("Flash memory:        NAND (%dMB)\n", NAND_SIZE);

	printf("Main memory:         %dMB\n", MAIN_MEM_SIZE);
	printf("                     - %dMB \"internal\" 1T-SRAM\n", INT_MEM_SIZE);
	printf("                     - %dMB \"external\" GDDR3 SDRAM\n", EXT_MEM_SIZE);

	printf("\n");

	getNetworkData(&ip_address, &netmask, &gateway, mac_address);
	printf("IP address:          %d.%d.%d.%d\n", (int)ip_address.b1, (int)ip_address.b2, (int)ip_address.b3, (int)ip_address.b4);
	printf("Netmask:             %d.%d.%d.%d\n", (int)netmask.b1,(int)netmask.b2, (int)netmask.b3, (int)netmask.b4);
	printf("Gateway:             %d.%d.%d.%d\n", (int)gateway.b1, (int)gateway.b2, (int)gateway.b3, (int)gateway.b4);
	printf("MAC address:         ");

	for (i = 0; i < 5; i++)
	{
		n=*(char *)(mac_address+i);
		printf("%02X:", (int)n);
	}
	n=*(char *)(mac_address+5);
	printf("%02X\n", n);

	printf("\nPress HOME button to quit");

	while(1)
	{
		// call WPAD_ScanPads each loop, this reads the latest controller states
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);

		// return to the launcher application via exit
		if ( pressed & WPAD_BUTTON_HOME ) exit(0);

		// Wait for the next frame
		VIDEO_WaitVSync();
	}

	return 0;
}
