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

#pragma once

#include "esp_system.h"

int findLevelFiles();
struct LevelInfoType* getLevelInfos();
uint8_t* getLevelData(int levelNo);
uint8_t* getMessages();
uint8_t getNoOfLevels();
char* getGameTitle();
void selectLevelFile(int fileNo);
uint32_t *readHiscores();
void saveHiscores(uint32_t *highScores);

/*
void system_loadGameFiles();

uint8_t system_readUI8(uint8_t file);
uint16_t system_readUI16(uint8_t file);
uint32_t system_readUI32(uint8_t file, uint8_t bigEnd);
void system_fileReset(uint8_t file);
uint8_t system_eof(uint8_t file);
void system_fileSeek(uint8_t file, uint32_t position);
void system_closeFile(uint8_t file);
uint32_t system_filePos(uint8_t file);
void system_read(uint8_t file,void* buffer,  uint32_t size);
uint32_t system_getMillis();
uint16_t READ_LE_UINT16(uint8_t* pointer);
uint16_t READ_BE_UINT16(uint8_t* pointer);

*/