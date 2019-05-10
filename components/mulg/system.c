/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "system.h"
#include "../odroid/odroid_settings.h"
#include "../odroid/odroid_sdcard.h"
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "driver/ledc.h"
#include "driver/i2s.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_task_wdt.h"
#include "esp_spiffs.h"
#include "driver/rtc_io.h"
#include "esp_partition.h"
#include "esp_ota_ops.h"
#include <string.h>
#include "level/mulg_I.h"



extern QueueHandle_t vidQueue;
//FILE* levelFile0;

uint8_t* levelFiles2;
uint8_t* messages;
int levelFileCount;
uint8_t levelCount;
int currentFileNo;


char levelFile[255];
char gameTitlexy[512];
uint8_t built_in_level = 1;

struct LevelInfoType{
	char* title;
	char* author;
	struct LevelInfoType*  previous;
	struct LevelInfoType*  next; 
} ;


uint8_t buffer[3072];


uint8_t readPdfFileInfo(char* file){
	// Read number of chunks
	
	// Read offsets/sizes for chunks
	
	// Remember File
	
	return 1;
}


char* getGameTitle(){
	return &gameTitlexy;
}

void selectLevelFile(int fileNo){

	uint16_t offset = 0;
	uint16_t size = 0;
	uint8_t* bmp = 0;


	if(currentFileNo>=built_in_level && messages!=0){
		free(messages);
	}

	currentFileNo = fileNo;
	
	if(fileNo<built_in_level){
		switch(fileNo){
			case 0: sprintf(levelFile,"/sd/roms/mulg/Mulg.pdb");
				break;
		}
	}else{
		fileNo = fileNo - built_in_level;
		printf("LevelFiles2: %p\n",levelFiles2);
		offset = (levelFiles2[2*fileNo]<<8) + levelFiles2[(2*fileNo)+1];
		printf("Offset for %d: %d \n",fileNo,offset);
		printf("Level %d is %s\n",fileNo, levelFiles2 + offset);

		sprintf(levelFile,"/sd/roms/mulg/%s",levelFiles2 + offset);
		printf("levelFile is %s\n",levelFile);
		
	}
	

	
	while (bmp > 1){
		xQueuePeek(vidQueue, &bmp, portMAX_DELAY);
	}
	
	printf("getMessages\n");
	printf("for file %d: %s\n", currentFileNo, levelFile);
	
	
	if(currentFileNo<built_in_level){
		switch(currentFileNo){
			case 0:	offset = 0x58;
					offset = (level_mulg_I[offset]<<8) + level_mulg_I[offset+1];
					messages = level_mulg_I + offset;
		}
	}else{
		FILE* f = fopen(levelFile, "r");
		
		
		
		offset = 0x58;
		fseek(f,offset,SEEK_SET);
		offset = ((uint8_t)fgetc(f)<<8) + (uint8_t)fgetc(f);
		
		// determine size
		fseek(f,6,SEEK_CUR);
		size = ((uint8_t)fgetc(f)<<8) + (uint8_t)fgetc(f) - offset;

		printf("Reading text at 0x%x, size: 0x%x\n",offset, size);
		
		fseek(f,offset,SEEK_SET);
		messages = calloc(1,size);
		
		fread(messages, 1, size, f);
		fclose(f);
		printf("Reading done.\n");
	}
	
	
}


uint32_t* readHiscores(){
	uint32_t *res = calloc(levelCount,sizeof(uint32_t));
	char hiscoreFile[255];
	
	sprintf(hiscoreFile,"%s.score",levelFile);
	
	printf("Open highsocre file %s...\n", hiscoreFile);
	
	FILE* f = fopen(hiscoreFile, "r");
	if(f!=0){
		int cnt = fread(res, sizeof(uint32_t), levelCount, f);
	
	/*
		for(int i = 0;i<levelCount;i++){
				printf("Hiscore for %d:%d\n", i, res[i]);
		}
	*/	
		
		fclose(f);
		printf("Reading highsocres done. (%d entries)\n", cnt);
	}else{
		printf("File %s not found!\n", hiscoreFile);
	}
	
	
	return res;
}

void saveHiscores(uint32_t *hiscores){
	char hiscoreFile[255];
	sprintf(hiscoreFile,"%s.score",levelFile);
	
	mkdir("/sd/roms",ACCESSPERMS);
	mkdir("/sd/roms/mulg",ACCESSPERMS);
	
	FILE* f = fopen(hiscoreFile, "w");
	if(f!=0){
		fwrite(hiscores, sizeof(uint32_t),  levelCount, f);
		fclose(f);
		printf("Writing done.\n");
	}else{
		printf("Create File %s failed!\n", hiscoreFile);
	}
}



int findLevelFiles(){
	
	// Search for File
	
	char** levelFiles;
	
	levelFileCount = odroid_sdcard_files_get("/sd/roms/mulg", ".pdb", &levelFiles);
	
	printf("Found %d files...\n",levelFileCount);
	
		//sprintf("Dumping Level files (1)... %p", levelFiles);
	int length = 0;
	for(int i=0;i<levelFileCount;i++){
		length += (strlen(levelFiles[i])+1);
		printf("%s\n", levelFiles[i]);
	}
	
	printf("Total length: %d\n", length);
	
	printf("Creating new Buffer...(size:%d)\n", levelFileCount*2+length);
	
	if(levelFileCount>0){
		levelFiles2 = calloc(levelFileCount*2+length,1);
		
		printf("Buffer created (%p)\n", levelFiles2);
		uint16_t offset = levelFileCount*2;
		
		int i,j,l;
		
		for(i=0;i<levelFileCount;i++){
			printf("Filling Buffer(%d)\n", i);
			levelFiles2[i*2]=(offset>>8) & 0xff;
			levelFiles2[(i*2)+1]=offset & 0xff;
			l = strlen(levelFiles[i]);
			for(j=0;j<l;j++){
				levelFiles2[offset+j] = levelFiles[i][j];
			}
			offset += (l+1);
		}
		
		odroid_sdcard_files_free(levelFiles, levelFileCount);
	}
	
	return levelFileCount + built_in_level;	
}

uint8_t* getMessages(){

	return messages;
	/*
	uint16_t offset = 0;
	uint16_t size = 0;
		
	uint8_t* bmp = 0;
	
	while (bmp > 1){
		xQueuePeek(vidQueue, &bmp, portMAX_DELAY);
	}
	
	printf("getMessages\n");
	printf("for file %d: %s\n", currentFileNo, levelFile);
	
	
	if(currentFileNo<built_in_level){
		switch(currentFileNo){
			case 0:	offset = 0x58;
					offset = (level_mulg_I[offset]<<8) + level_mulg_I[offset+1];
					return level_mulg_I + offset;
		}
		return 0;
	}else{
		FILE* f = fopen(levelFile, "r");
		
		
		
		offset = 0x58;
		fseek(f,offset,SEEK_SET);
		offset = ((uint8_t)fgetc(f)<<8) + (uint8_t)fgetc(f);
		
		// determine size
		fseek(f,6,SEEK_CUR);
		size = ((uint8_t)fgetc(f)<<8) + (uint8_t)fgetc(f) - offset;

		printf("Reading text at 0x%x, size: 0x%x\n",offset, size);
		
		fseek(f,offset,SEEK_SET);
		res = calloc(1,size);
		
		fread(res, 1, size, f);
		fclose(f);
		printf("Reading done.\n");
		return res;
	}
	*/
}

uint8_t getNoOfLevels(){
	
	uint8_t* bmp = 0;
	
	while (bmp > 1){
		xQueuePeek(vidQueue, &bmp, portMAX_DELAY);
	}
    
	if(currentFileNo<built_in_level){
		switch(currentFileNo){
		case 0:	sprintf(gameTitlexy, "%s",level_mulg_I);
				levelCount = (uint8_t)level_mulg_I[0x4d]-2;
				break;
		}
	}else{
		FILE* f = fopen(levelFile, "r");
		
		// Read Title
		fseek(f,0,SEEK_SET);
		
		char dummy[32];
		fread(&dummy, 1, 32, f);
		sprintf(gameTitlexy, "%s",dummy);
		
		fseek(f,0x4d,SEEK_SET);
		levelCount = (uint8_t)fgetc(f)-2;
		fclose(f);
	}
	printf("No of Levels in %s: %d\n",levelFile, levelCount);
	return levelCount;
}

uint8_t* getLevelData(int levelNo){
	
	uint16_t offset = 0;
	uint16_t size = 0;
	uint8_t levels = 0;
	
	uint8_t* bmp = 0;
	
	while (bmp > 1){
		xQueuePeek(vidQueue, &bmp, portMAX_DELAY);
	}
	
	if(currentFileNo<built_in_level){
		switch(currentFileNo){
		case 0:	
				levels = (uint8_t)level_mulg_I[0x4d]-2;
				
				offset = 0x50 + (8 * (levelNo + 2));
				offset = ((uint8_t)level_mulg_I[offset]<<8) + (uint8_t)level_mulg_I[offset+1];
		
				/*
				if(levelNo<levels-1){
					// determine size
					size = ((uint8_t)level_mulg_I[offset+7]<<8) + (uint8_t)level_mulg_I[offset+8] - offset;
				}else{
					// Read to end
					size = 2048;
				}
		
				printf("Reading level %d at 0x%x, size: 0x%x\n", levelNo, offset, size);
				*/
				return(level_mulg_I+offset);
		}
		return 0;
	}else{
		FILE* f = fopen(levelFile, "r");
		
		fseek(f,0x4d,SEEK_SET);
		levels = (uint8_t)fgetc(f)-2;
		
		offset = 0x50 + (8 * (levelNo + 2));
		fseek(f,offset,SEEK_SET);
		offset = ((uint8_t)fgetc(f)<<8) + (uint8_t)fgetc(f);
		
		if(levelNo<levels-1){
			// determine size
			fseek(f,6,SEEK_CUR);
			size = ((uint8_t)fgetc(f)<<8) + (uint8_t)fgetc(f) - offset;
		}else{
			// Read to end
			size = 2048;
		}
		
		printf("Reading level %d at 0x%x, size: 0x%x\n", levelNo, offset, size);
		
		fseek(f,offset,SEEK_SET);
		fread(&buffer, 1, size, f);
		fclose(f);
		printf("Reading done.\n");
		return &buffer;
	}
	
}
