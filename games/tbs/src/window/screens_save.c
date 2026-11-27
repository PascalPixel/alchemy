/* @file */
/* Save files and the file list. */
/* Part of screens.c. */

/* A file's half in bytes: the first is written to the file's slot, the */
/* second to the slot SAVE_FILE_COUNT on. Then the file number for no free */
/* file. */
#define SAVE_PART_BYTES	  4096
#define SAVE_NO_FREE_FILE 999

/* What the save functions return when they fail, after showing the message. */
#define SAVE_ERROR_READ -2 /* a part could not be read */
/* Writing failed. "Failed to save." (or "Failed to erase.") */
#define SAVE_ERROR_WRITE   -3
#define SAVE_ERROR_ERASE   -4 /* erasing failed. "Failed to erase." */
#define SAVE_ERROR_NO_FILE -5 /* no empty file to copy into */
/* No backup memory. "Unable to detect backup memory." */
#define SAVE_ERROR_NO_MEMORY -9

/* mnFileListSelectFile's result when no file can be chosen in its mode. */
#define FILE_LIST_NO_FILE -2

/* The play time shown stops at PLAY_TIME_HOURS_MAX hours and 59 minutes. */
#define PLAY_TIME_HOURS_MAX 999
#define MINUTES_PER_HOUR	60
#define SECONDS_PER_MINUTE	60

/* A two-digit field for mnStringFormatNum: with 100 added, its last two */
/* digits keep their leading zero. */
#define WITH_LEADING_ZERO(n) ((n) + 100)

/* The words of SceneLocalState (system/ewram.s), which InventorySnapshotWork */
/* follows. */
#define SCENE_LOCAL_STATE_WORDS 30
/* The words from GameFlagBytes that a file's checksum sums. */
#define SAVE_CHECKSUM_WORDS 968

/* The file list's glow: the middle colour that HIGHLIGHT_PALETTE's colours 8 */
/* to 11 sway round, red twice as far as green and blue. */
#define FILE_LIST_GLOW_RED	 22
#define FILE_LIST_GLOW_GREEN 16
#define FILE_LIST_GLOW_BLUE	 20

/* Writes the play time `frames` (at 60 frames a second) into `out` as hours */
/* and minutes, "hhh:mm" with the hours right-aligned in three characters, and
 */
/* returns `out`. The time stops at 999:59. */
void *mnStringFormatPlayTime(s32 frames, u8 *out) {
	u8 digits[64];
	u32 totalMinutes;
	u32 hours;
	s32 minutes;
	u8 *digitsCursor;
	u8 *textCursor;

	totalMinutes = (u32)frames / (FRAMES_PER_SECOND * SECONDS_PER_MINUTE);
	if(totalMinutes >
	   PLAY_TIME_HOURS_MAX * MINUTES_PER_HOUR + MINUTES_PER_HOUR - 1)
		totalMinutes =
			PLAY_TIME_HOURS_MAX * MINUTES_PER_HOUR + MINUTES_PER_HOUR - 1;

	hours = totalMinutes / MINUTES_PER_HOUR;
	minutes = totalMinutes % MINUTES_PER_HOUR;

	digitsCursor = mnStringFormatNum(digits, hours, 3);
	*out = *digitsCursor;
	digitsCursor++;
	textCursor = out + 1;
	*textCursor = *digitsCursor;
	textCursor++;
	*textCursor = digitsCursor[1];
	minutes = WITH_LEADING_ZERO(minutes);
	textCursor++;
	*textCursor = ':';

	digitsCursor = mnStringFormatNum(digits, minutes, 2);
	textCursor++;
	*textCursor = digitsCursor[0];
	textCursor++;
	*textCursor = digitsCursor[1];
	textCursor[1] = 0;

	return out;
}

/* Returns the first save file whose summary shows it empty (level 0), or */
/* SAVE_NO_FREE_FILE when all three are in use. The summaries must be loaded. */
u32 mnSaveFindFreeFile(void) {
	u32 file;
	SAVE_FILE_SUMMARY *summary;

	summary = SaveStateWork->summary;
	for(file = 0; file < SAVE_FILE_COUNT; file++, summary++) {
		if(summary->level == 0)
			return file;
	}
	return SAVE_NO_FREE_FILE;
}

/* Returns how many save files there are, leaving out those whose summary has */
/* its send flag set (the files saved at the end of the game) when */
/* `excludeSent` is nonzero, or SAVE_ERROR_NO_MEMORY when the backup memory */
/* cannot be read. It opens and releases the save state itself. */
s32 mnSaveCountFilesExcludingSent(s32 excludeSent) {
	s32 file;
	s32 count;
	SAVE_FILE_SUMMARY *summary;

	if(sySaveInitState() != 0) {
		count = SAVE_ERROR_NO_MEMORY;
	}
	else {
		count = sySaveLoadFileSummaries();
		if(excludeSent != 0) {
			summary = SaveStateWork->summary;
			for(file = 0; file < SAVE_FILE_COUNT; file++) {
				if(summary[file].sendFlg != 0)
					count--;
			}
		}
	}
	sySaveReleaseState();
	return count;
}

/* Loads the save file summaries for the File Screen and returns how many files
 */
/* there are: plus SAVE_SCAN_ALL_CLEARED when every file is cleared (its send */
/* flag set), or SAVE_ERROR_NO_MEMORY when the backup memory cannot be read. */
s32 mnSaveScanFileSummaries(void) {
	s32 error;
	s32 sentCount;
	s32 fileCount;

	error = sySaveInitState();
	sentCount = 0;
	fileCount = SAVE_ERROR_NO_MEMORY;
	if(error == 0) {
		s32 i;
		SAVE_STATE *state;

		fileCount = sySaveLoadFileSummaries();
		state = SaveStateWork;
		TitleSendOptionEnabled = 0;
		TitleBattleOptionEnabled = 0;
		for(i = 0; i < SAVE_FILE_COUNT; i++) {
			if(state->summary[i].sendFlg != 0) {
				TitleSendOptionEnabled = 1;
				sentCount++;
			}
			if(state->summary[i].djinnCnt != 0) {
				TitleBattleOptionEnabled = 1;
			}
		}

		if((KeysHeld & (KEY_R | KEY_LEFT)) != (KEY_R | KEY_LEFT)) {
			TitleSendOptionEnabled = 0;
		}
	}
	sySaveReleaseState();
	if(fileCount != 0 && sentCount == fileCount) {
		return fileCount + SAVE_SCAN_ALL_CLEARED;
	}
	return fileCount;
}

/* Prepares the save buffer for writing: stamps the main routine's state with */
/* the time and options, then fills the file summary shown on the file menus */
/* (the leader's name, level and class, the play time, area, coins, Djinn of */
/* each element, the party, the send flag and how many Djinn have been found) */
/* and its checksum, the sum of SAVE_CHECKSUM_WORDS words from GameFlagBytes */
/* on. */
u32 mnSaveBuildFileSummary(void) {
	u16 members[14];
	SAVE_FILE_SUMMARY *summary;
	UNIT *leader;
	u8 *source;
	u8 *destination;
	u32 *word;
	s32 i;
	u32 sum = 0;

	/* The summary's header comes before the save buffer. */
	summary = (SAVE_FILE_SUMMARY *)((u32)SaveBuffer -
									offsetof(SAVE_FILE_SUMMARY, name));
	MainRoutineStateWork.buildStamp = gmBuildStampGetTimeStub(0);
	MainRoutineStateWork.playTime = PlayTimeFrames;
	/* A second copy of the play time in the file's second part, which */
	/* sySaveLoadFileSummaries reads back into
	 * SAVE_FILE_SUMMARY.secondPartPlayTime: */
	/* the file list lists a file whose two play times differ as corrupted. */
	/* UB: the copy is written through SceneLocalState past its own bytes, into
	 */
	/* InventorySnapshotWork's last word; UBFIX writes it through */
	/* InventorySnapshotWork. */
	{
#ifdef UBFIX
		u32 *copy = (u32 *)InventorySnapshotWork;
		copy[SECOND_PART_PLAY_TIME_WORD - SCENE_LOCAL_STATE_WORDS] =
			PlayTimeFrames;
#else
		u32 *copy = (u32 *)SceneLocalState;
		copy[SECOND_PART_PLAY_TIME_WORD] = PlayTimeFrames;
#endif
	}
	MainRoutineStateWork.idleSleepEnabled = IdleSleepEnabled;

	leader = gmUnitGetStub(MainRoutineStateWork.leaderActorId);
	destination = summary->name;
	source = leader->name;
	for(i = 0; i < (s32)sizeof(summary->name); i++)
		*destination++ = *source++;
	summary->level = leader->level;
	summary->playTime = MainRoutineStateWork.playTime;
	summary->area = fdAreaGetNameIndexStub(MainRoutineStateWork.next,
										   MainRoutineStateWork.entrance);
	summary->classId = leader->classId;
	summary->coins = MainRoutineStateWork.holdings.coins;
	summary->djinn[ELEMENT_VENUS] = gmPartySumDjinnCountsStub(ELEMENT_VENUS);
	summary->djinn[ELEMENT_MERCURY] =
		gmPartySumDjinnCountsStub(ELEMENT_MERCURY);
	summary->djinn[ELEMENT_MARS] = gmPartySumDjinnCountsStub(ELEMENT_MARS);
	summary->djinn[ELEMENT_JUPITER] =
		gmPartySumDjinnCountsStub(ELEMENT_JUPITER);
	/* Up to four party members, ended by -1. */
	gmPartyListMembersStub(members);
	for(i = 0; i <= 3 && members[i] != BATTLE_UNIT_LIST_END; i++)
		summary->party[i] = members[i];
	summary->party[i] = -1;
	summary->winHue = MainRoutineStateWork.winHue;
	summary->winBrightness = MainRoutineStateWork.winBrightness;
	summary->sendFlg = MainRoutineStateWork.sendFlg;
	summary->djinnCnt = 0;
	for(i = FLAG_DJINN_FIRST;
		i < FLAG_DJINNI(ELEMENT_COUNT * DJINN_PER_ELEMENT); i++) {
		if(gmFlagGetStub(i))
			summary->djinnCnt++;
	}
	summary->childhood = gmFlagGetStub(FLAG_CHILDHOOD) != 0;
	{
		s32 n;
		u32 frames;

		frames = MainRoutineStateWork.buildStamp;
		/* The checksum's words, counted in fours and multiplied back below. */
		n = SAVE_CHECKSUM_WORDS / 4;
		i = 0;
		n <<= 2;
		word = (u32 *)GameFlagBytes;
		scIoWrite32(frames, &summary->buildStamp);
		/* A while loop, entered at its test. */
		goto test;
		do {
			sum += *word++;
			i++;
		test:;
		} while(i < n);
	}
	summary->checksum = sum;
	return sum;
}

/* Saves the game to the current file (SaveSlot): builds the summary and */
/* writes both halves of the save buffer, as parts SaveSlot and SaveSlot + */
/* SAVE_FILE_COUNT. */
s16 mnSaveWriteCurrentFile(void) {
	s16 file;
	s16 result;
	s32 error;

	result = 0;
	file = SaveSlot;
	if(file != -1) {
		error = sySaveInitState();
		if(error != 0) {
			mnFieldMessageShow(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
			result = SAVE_ERROR_NO_MEMORY;
		}
		else {
			mnSaveBuildFileSummary();
			{
				void *half = &SaveBuffer;
				s32 current;

				error = sySaveWritePart(SaveSlot, half);
				current = SaveSlot;
				half = (char *)half + SAVE_PART_BYTES;
				error |= sySaveWritePart(current + SAVE_FILE_COUNT, half);
				if(error != 0) {
					mnFieldMessageShow(MsgSaveFailed, FIELD_MESSAGE_ZOOM);
					result = SAVE_ERROR_WRITE;
				}
			}
		}
		sySaveReleaseState();
		file = result;
	}
	return file;
}

/* Writes both halves of the save buffer, as they are, to save file `file`. */
/* Returns 0, or a SAVE_ERROR_* after showing its message. */
s32 mnSaveWriteFile(s32 file) {
	s32 error;
	s16 result = 0;

	error = sySaveInitState();
	if(error != 0) {
		mnFieldMessageShow(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
		result = SAVE_ERROR_NO_MEMORY;
	}
	else {
		void *half = &SaveBuffer;

		error = sySaveWritePart(file, half);
		half = (char *)half + SAVE_PART_BYTES;
		error |= sySaveWritePart(file + SAVE_FILE_COUNT, half);
		if(error != 0) {
			mnFieldMessageShow(MsgSaveFailed, FIELD_MESSAGE_ZOOM);
			result = SAVE_ERROR_WRITE;
		}
	}
	sySaveReleaseState();
	return result;
}

/* Writes the 64-byte save stamp (BattleArenaSaveStamp, which lies in the */
/* first half of the save buffer) into the current file's first part, leaving */
/* the rest of the part as it was saved. */
s32 mnSaveWriteLinkTallies(void) {
	void *part;
	s32 result;
	s32 error;
	u8 *stamp;

	part = syHeapAllocEwram(SAVE_PART_BYTES);
	result = 0;
	/* BUG: returns without freeing the part buffer. */
	if(SaveSlot == -1)
		return -1;
	error = sySaveInitState();
	if(error != 0) {
		mnFieldMessageShow(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
		result = SAVE_ERROR_NO_MEMORY;
	}
	else {
		error = sySaveReadPart(SaveSlot, part);
		/* A failed read is reported, but the part is still written below from
		 */
		/* whatever the buffer holds. */
		if(error != 0) {
			mnFieldMessageShow(MsgSaveFailed, FIELD_MESSAGE_ZOOM);
			result = SAVE_ERROR_READ;
		}
		/* The stamp's place in the part is its place in the save buffer. */
		stamp = (u8 *)part + (s32)BattleArenaSaveStamp;
		stamp -= (s32)SaveBuffer;
		syWordsCopy(stamp, BattleArenaSaveStamp, 16);
		error = sySaveWritePart(SaveSlot, part);
		if(error != 0) {
			mnFieldMessageShow(MsgSaveFailed, FIELD_MESSAGE_ZOOM);
			result = SAVE_ERROR_WRITE;
		}
	}
	sySaveReleaseState();
	syHeapRelease(part);
	return result;
}

/* Asks whether to save over the current file and, on Yes, writes the save */
/* stamp into it (mnSaveWriteLinkTallies) and says so. */
UNUSED s32 mnSaveConfirmAndProcessSelectedFile(void) {
	s32 result;

	/* UB: on No, result is returned without ever being set. */
#ifdef UBFIX
	result = 0;
#endif

	mnMessageOpenWindow(MsgOverwriteConfirm, 8, 12, 2);
	while(mnMessageAllTyped() == 0) {
		sySynch(1);
	}
	if(mnYesNoMenuRun(1, 0, 0, 1)) {
		mnMessageCloseFinishedWindows();
	}
	else {
		mnMessageCloseFinishedWindows();
		sySndPlayStub(MUS_SAVED);
		result = mnSaveWriteLinkTallies();
		if(result >= 0) {
			mnFieldMessageShow(MsgGameSaved, FIELD_MESSAGE_ZOOM);
		}
	}
	return result;
}

/* The file list's Copy (scene/file_screen.c): lets the player choose a file, */
/* reads both of its parts into the save buffer, overwriting it, and writes */
/* them to the first empty file. */
s32 mnSaveCopyFile(void) {
	u32 error;
	s32 file;
	s32 result;

	result = 0;
	error = sySaveInitState();
	if(error != 0) {
		mnFieldMessageShow(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
		result = SAVE_ERROR_NO_MEMORY;
	}
	else {
		sySaveLoadFileSummaries();
		file = mnFileListSelectFile(0, FILE_LIST_COPY);
		if(file == -1) {
			result = file;
		}
		else {
			void *lower = &SaveBuffer;
			void *upper;

			error = sySaveReadPart(file, lower);
			upper = (u8 *)lower + SAVE_PART_BYTES;
			error |= sySaveReadPart(file + SAVE_FILE_COUNT, upper);
			if(error != 0) {
				mnFieldMessageShow(MsgLoadFailed, FIELD_MESSAGE_ZOOM);
				result = SAVE_ERROR_READ;
			}
			else {
				file = mnSaveFindFreeFile();
				if(file == SAVE_NO_FREE_FILE) {
					mnFieldMessageShow(MsgEraseFailed, FIELD_MESSAGE_ZOOM);
					result = SAVE_ERROR_NO_FILE;
				}
				else {
					error = sySaveWritePart(file, lower);
					error |= sySaveWritePart(file + SAVE_FILE_COUNT, upper);
					if(error != 0) {
						mnFieldMessageShow(MsgEraseFailed, FIELD_MESSAGE_ZOOM);
						result = SAVE_ERROR_WRITE;
					}
					else {
						mnFieldMessageShow(MsgFileCopied, FIELD_MESSAGE_ZOOM);
					}
				}
			}
		}
	}

	sySaveReleaseState();
	return result;
}

/* The file list's Erase (scene/file_screen.c): lets the player choose a file,
 */
/* asks whether to erase it and on Yes deletes both of its parts. */
s32 mnSaveEraseSelectedFile(void) {
	s32 error;
	s32 file;
	s32 result = 0;

	error = sySaveInitState();
	if(error != 0) {
		mnFieldMessageShow(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
		result = SAVE_ERROR_NO_MEMORY;
	}
	else {
		sySaveLoadFileSummaries();
		file = mnFileListSelectFile(0, FILE_LIST_ERASE);
		if(file == -1) {
			result = file;
		}
		else {
			mnMessageOpenWindow(MsgEraseConfirm, 8, 1, 2);
			while(mnMessageAllTyped() == 0) {
				sySynch(1);
			}
			if(mnYesNoMenuRun(1, 0, 3, 1) != 0) {
				mnMessageCloseFinishedWindows();
			}
			else {
				mnMessageCloseFinishedWindows();
				error = sySaveDeletePart(file);
				error |= sySaveDeletePart(file + SAVE_FILE_COUNT);
				if(error != 0) {
					mnFieldMessageShow(MsgEraseFailed, FIELD_MESSAGE_ZOOM);
					result = SAVE_ERROR_ERASE;
				}
				else {
					mnFieldMessageShow(MsgFileErased, FIELD_MESSAGE_ZOOM);
				}
			}
		}
	}
	sySaveReleaseState();
	return result;
}

#if TBS_PATCH_1

/* A task that makes colours 8-11 of HIGHLIGHT_PALETTE glow: each follows a */
/* sine wave of the frame counter, eight frames behind the one before. */
void mnFileListUpdatePulse(void) {
	s32 *vblankCount;
	volatile u16 *palette;
	s32 colour;
	FIXED sine;
	s32 i;

	vblankCount = (s32 *)&VBlankCount;
	palette = &BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 8);
	i = 0;
	do {
#ifdef UBFIX
		/* Wrap the frame phase before sySin takes its low sixteen bits. */
		sine = sySin((((u32)*vblankCount + i * 8U) * (TRIG_TURN * 3 / 256U)) &
					 TRIG_ANGLE_MASK);
#else
		sine = sySin((*vblankCount + i * 8) * (TRIG_TURN * 3 / 256));
#endif
		colour = sine / (FIXED_ONE / 4);
		{
#ifdef UBFIX
			/* A negative sine must be shifted as unsigned bits. */
			u32 red = (u32)colour << 1;
#else
			u32 red = colour << 1;
#endif
			u32 green = colour;

			red += FILE_LIST_GLOW_RED;
			green += FILE_LIST_GLOW_GREEN;
			colour += FILE_LIST_GLOW_BLUE;
			scIoWrite16(RGB(red, green, colour), palette++);
		}
	} while(++i <= 3);
}

#endif

#if TBS_PATCH_1

/* Starts the palette glow (mnFileListUpdatePulse) as a task. */
void mnFileListStartPulse(void) {
	sySetEvent(mnFileListUpdatePulse, TASK_PASS_FRAME);
}

/* Stops the file list's glow (mnFileListUpdatePulse). */
void mnFileListStopPulse(void) { syCloseEvent(mnFileListUpdatePulse); }

/* Fills the `width` x `height` tile rectangle at (x, y) inside `window`, */
/* clipped to the screen as mnWindowSetInteriorPaletteBit clips, with the tile
 */
/* entry of tile 6 in HIGHLIGHT_PALETTE, the palette mnFileListUpdatePulse */
/* animates. */
void mnWindowFillTilemapRect(WINDOW *window, s32 x, s32 y, s32 width,
							 s32 height) {
	u16 *tilemap = (u16 *)WindowStateWork[0];
	s32 sum;

	sum = x + window->x;
	x = sum + 1;
	sum = y + (window)->y;
	y = sum + 1;

	if(x < 0) {
		width += x;
		x = 0;
	}
	if(x + width > DISPLAY_TILE_WIDTH - 1)
		width = DISPLAY_TILE_WIDTH - x;

	if(y < 0) {
		height += y;
		y = 0;
	}
	/* BUG: the bottom is tested against the screen's width, so a rectangle */
	/* ending below row DISPLAY_TILE_HEIGHT writes past WIN_STATE.tilemap; the
	 */
	/* one caller, the file list's highlight, never does. */
#ifdef BUGFIX
	if(y + height > DISPLAY_TILE_HEIGHT - 1)
#else
	if(y + height > DISPLAY_TILE_WIDTH - 1)
#endif
		height = DISPLAY_TILE_HEIGHT - y;

	if(width > 0 && height > 0) {
		/* Byte offsets: BG_SCREEN_WIDTH entries of 2 bytes a row. */
		s32 yOffset = y * (BG_SCREEN_WIDTH * 2);
		s32 xOffset = x << 1;
		s32 offset = yOffset + xOffset;
		do {
			s32 remaining = width;
			u16 *cell = (u16 *)(offset + (s32)tilemap);
			for(; remaining != 0; remaining--)
				*cell++ = BG_TILE_PALETTE(HIGHLIGHT_PALETTE) | 6;
			height--;
			offset += BG_SCREEN_WIDTH * 2;
		} while(height != 0);
		((WIN_STATE *)tilemap)->dirty = 1;
	}
}

#endif

/* The file list shows the chosen file's party as standing sprites in a row, */
/* and four Djinn sprites, one for each element, over the file's Djinn counts.
 */
#define FILE_LIST_SPRITE_SPACING 3

/* Shows the party of save file `summary` (up to four party members) as sprites
 */
/* in a row from tile (x, y) inside `window`, and starts placing them every */
/* frame (mnFileListPlacePartySprs). */
void mnFileListShowPartySprs(WINDOW *window, s32 x, s32 y,
							 SAVE_FILE_SUMMARY *summary) {
	SHOP_STATE *menu = MenuState;
	s32 i;

	for(i = 0; i < 4 && summary->party[i] != -1; i++) {
		SPR *sprite = fdSprCreateStub(gmPartyGetCharacterSprForChildhoodStub(
			summary->party[i], summary->childhood));

		if(sprite != NULL) {
			fdSprStartAnimationStub(sprite, ANIM_STAND);
			sprite->flg = 0;
			sprite->image.pri = 0;
		}

		menu->partyMemberIcons[i] = sprite;
		menu->partyMemberX[i] =
			(x + window->x + i * FILE_LIST_SPRITE_SPACING) * 8 + 16;
		menu->partyMemberY[i] = (y + window->y) * 8 + 16;
		menu->partyMemberScale[i] = FIXED_ONE;
	}

	sySetEvent(mnFileListPlacePartySprs, TASK_PASS_FRAME);
}

/* Stops placing the party sprites of the file list and releases them. */
void mnFileListHidePartySprs(void) {
	SHOP_STATE *menu = MenuState;
	s32 i;

	syCloseEvent(mnFileListPlacePartySprs);
	for(i = 0; i < 4; i++) {
		if(menu->partyMemberIcons[i] != NULL) {
			fdSprDestroyStub(menu->partyMemberIcons[i]);
			menu->partyMemberIcons[i] = NULL;
		}
	}
}

/* A task: draws each party sprite of the file list at its */
/* place and scale for this frame. */
void mnFileListPlacePartySprs(void) {
	SHOP_STATE *menu = MenuState;
	SPR_SCALE scale;
	SPR_PLACEMENT position;
	s32 index = 0;
	SPR *sprite;

	while(1) {
		sprite = menu->partyMemberIcons[index];
		if(sprite != 0) {
			/* Offset height and y equally to keep the visible row fixed. */
			scale.x = menu->partyMemberScale[index];
			scale.y = menu->partyMemberScale[index];
			position.x = toFIXED(menu->partyMemberX[index]);
			position.height = toFIXED(500);
			position.y = toFIXED(menu->partyMemberY[index]) + toFIXED(500);
			position.groundHeight = 0;
			fdRenderApplyProjectedPlacementStub(sprite, &position, &scale,
												FACING_SOUTH);
		}
		index += 1;
		if(index > 3) {
			break;
		}
	}
}

/* Shows the four Djinn sprites, one for each element, in a row from tile */
/* (x, y) inside `window`, and starts placing them every frame */
/* (mnFileListPlaceDjinnSprs). Does nothing when `window` is NULL. */
void mnFileListShowDjinnSprs(WINDOW *window, s32 x, s32 y) {
	ITEM_MENU_STATE *menu = MenuState;
	s32 i;

	if(window != NULL) {
		for(i = 0; i < 4; i++) {
			SPR *sprite = fdSprCreateStub(FileList_DjinnSpriteIds[i]);

			if(sprite != NULL) {
				fdSprStartAnimationStub(sprite, ANIM_WALK);
				sprite->flg = 0;
				sprite->image.pri = 0;
			}

			menu->figures[i] = sprite;
			menu->figureX[i] =
				(window->x + x + i * FILE_LIST_SPRITE_SPACING) * 8 + 16;
			menu->figureY[i] = (window->y + y) * 8 + 16;
		}

		sySetEvent(mnFileListPlaceDjinnSprs, TASK_PASS_FRAME);
	}
}

/* Stops placing the Djinn sprites of the file list and releases them. */
void mnFileListHideDjinnSprs(void) {
	ITEM_MENU_STATE *menu = MenuState;
	s32 i;

	syCloseEvent(mnFileListPlaceDjinnSprs);
	for(i = 0; i < 4; i++) {
		if(menu->figures[i] != NULL) {
			fdSprDestroyStub(menu->figures[i]);
			menu->figures[i] = NULL;
		}
	}
}

/* A task: draws each Djinn sprite of the file list at its */
/* place, at scale 1.0, for this frame. */
void mnFileListPlaceDjinnSprs(void) {
	ITEM_MENU_STATE *menu = MenuState;
	SPR_SCALE scale;
	SPR_PLACEMENT position;
	s32 index;

	for(index = 0; index <= 3; index++) {
		SPR *sprite = menu->figures[index];

		if(sprite != NULL) {
			FIXED fullSize = FIXED_ONE;
			FIXED height = toFIXED(500);

			scale.x = fullSize;
			scale.y = fullSize;
			position.x = toFIXED(menu->figureX[index]);
			position.height = height;
			position.y = toFIXED(menu->figureY[index]) + height;
			position.groundHeight = 0;
			fdRenderApplyProjectedPlacementStub(sprite, &position, &scale,
												FACING_SOUTH);
		}
	}
}

/* Draws the Djinn counts of save file `summary`, one for each element, two */
/* digits each, 24 pixels apart on row 16 of `window`, under the Djinn sprites.
 */
void mnFileListDrawDjinnCounts(WINDOW *window, SAVE_FILE_SUMMARY *summary) {
	s32 i;

	if(window != NULL) {
		for(i = 0; i < 4; i++)
			mnTextDrawNumAtOffset(summary->djinn[i], 2, window,
								  i * FILE_LIST_SPRITE_SPACING * 8, 16);
	}
}

/* Where each edition draws the "Lv" label and the coin label, in pixels. */
#if LANGUAGE == LANGUAGE_SPANISH
#define SUMMARY_LEVEL_X	   64
#define SUMMARY_COINS_X	   56
#define SUMMARY_DRAW_LEVEL mnTextDrawStringAtOffset
#elif LANGUAGE == LANGUAGE_FRENCH
#define SUMMARY_LEVEL_X	   59
#define SUMMARY_COINS_X	   56
#define SUMMARY_DRAW_LEVEL mnTextDrawStringAtOffset
#else
#define SUMMARY_LEVEL_X	   72
#define SUMMARY_COINS_X	   48
#define SUMMARY_DRAW_LEVEL mnTextDrawStringInWindow
#endif

/* Draws the summary of save file `summary` into `window`: the leader's name, */
/* level and class, the play time and the coins. */
void mnFileListDrawSummary(WINDOW *window, SAVE_FILE_SUMMARY *summary) {
	u32 playTime[4];
	s32 y;

	if(window != 0) {
		mnWindowSprPrepareForRedraw(window);
		mnWindowDrawDividerLine(window, 0, 4, 13, 4);
		mnTextDrawStringAtOffset(summary->name, window, 0, 0);
		y = 0;
		SUMMARY_DRAW_LEVEL(StatusScreen_LevelLetterString, window,
						   SUMMARY_LEVEL_X, 0);
		mnTextDrawNumAtOffset(summary->level, 2, window, 80, y);
		mnTextDrawMessageAsTiles(summary->classId + MsgClassName, window, 0,
								 16);
#if LANGUAGE == LANGUAGE_JAPANESE
		/* Coins on the third row, then "Play Time:" and the time. */
		y = 32;
		mnTextDrawNumInWindow(summary->coins, 6, window, 24, y);
		mnTextDrawMessageAsTiles(MsgCoins, window, 72, 32);
		mnTextDrawMessageAsTiles(MsgStatusLabel, window, 0, 48);
		mnTextDrawStringInWindow(
			mnStringFormatPlayTime(summary->playTime, (u8 *)playTime), window,
			48, 48);
#else
		/* "Play Time:" with the time under it, then the coins. */
		mnTextDrawMessageAsTiles(MsgStatusLabel, window, 0, 32);
		mnTextDrawStringInWindow(
			mnStringFormatPlayTime(summary->playTime, (u8 *)playTime), window,
			48, 40);
		y = 48;
		mnTextDrawNumInWindow(summary->coins, 6, window, 0, y);
#if LANGUAGE == LANGUAGE_FRENCH
		/* French takes the singular "Pièce" for one coin or none. */
		if(summary->coins > 1)
			mnTextDrawMessageAsTiles(MsgCoins, window, SUMMARY_COINS_X, 48);
		else
			mnTextDrawMessageAsTiles(MsgPiece, window, SUMMARY_COINS_X, 48);
#else
		mnTextDrawMessageAsTiles(MsgCoins, window, SUMMARY_COINS_X, 48);
#endif
#endif
	}
}

/* The file list: lists the three files with each one's leader and area (or why
 */
/* it cannot be chosen) and, for the file under the cursor, its summary, its */
/* party and its Djinn, with the file's own palette glow. */
s32 mnFileListSelectFile(s32 selectedFile, s32 menuMode) {
	WINDOW *fileList;
#if TBS_PATCH_1
	SAVE_STATE *save;
	WINDOW *summaryWindow;
	WINDOW *partyWindow;
	WINDOW *djinnWindow;
	SAVE_FILE_SUMMARY *summary;
	WINDOW_SPR *modePicture;
	s32 summaryDirty;
	WIN_STATE *state;
	u32 buildStamp;
	SHOP_STATE *spriteState;
	s32 result;
	s32 i;
	s32 y;
#else
	WINDOW *summaryWindow;
	SAVE_STATE *save;
	WINDOW *partyWindow;
	WINDOW *djinnWindow;
	SAVE_FILE_SUMMARY *summary;
	WINDOW_SPR *modePicture;
	s32 cursorStyle;
	s32 summaryDirty;
	WIN_STATE *state;
	u32 buildStamp;
	SHOP_STATE *spriteState;
	MENU_CURSOR fileCursor;
	s32 result;
	s32 i;
	s32 x;
	s32 y;
	s32 vramSlot;
	WINDOW_SPR *cursorSprite;
#endif

	spriteState = syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(*spriteState));
	save = SaveStateWork;
	summaryWindow = NULL;
	partyWindow = NULL;
	djinnWindow = NULL;
	summaryDirty = 1;
	state = WINDOW_STATE;
	buildStamp = gmBuildStampGetTimeStub();
	/* Start on the first file the mode allows. */
	/* BUG: when none is, the jump to done returns FILE_LIST_NO_FILE without */
	/* releasing spriteState's HEAP_SLOT_MENU block. */
	if(selectedFile < 0)
		selectedFile = 0;
#if TBS_PATCH_1
	if(menuMode == FILE_LIST_CONTINUE) {
		for(i = 0;
			i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0 ||
									save->summary[selectedFile].sendFlg != 0);
			i++) {
			selectedFile++;
			if(selectedFile == SAVE_FILE_COUNT)
				selectedFile = 0;
		}
		if(i == SAVE_FILE_COUNT) {
			result = FILE_LIST_NO_FILE;
			goto done;
		}
	}
	else if(menuMode == FILE_LIST_BATTLE) {
		for(i = 0;
			i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0 ||
									save->summary[selectedFile].djinnCnt == 0);
			i++) {
			selectedFile++;
			if(selectedFile == SAVE_FILE_COUNT)
				selectedFile = 0;
		}
		if(i == SAVE_FILE_COUNT) {
			result = FILE_LIST_NO_FILE;
			goto done;
		}
	}
	else if(menuMode == FILE_LIST_CLEARED) {
		for(i = 0;
			i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0 ||
									save->summary[selectedFile].sendFlg == 0);
			i++) {
			selectedFile++;
			if(selectedFile == SAVE_FILE_COUNT)
				selectedFile = 0;
		}
		if(i == SAVE_FILE_COUNT) {
			result = FILE_LIST_NO_FILE;
			goto done;
		}
	}
	else if(menuMode != FILE_LIST_SAVE) {
		for(i = 0;
			i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0);
			i++) {
			selectedFile++;
			if(selectedFile == SAVE_FILE_COUNT)
				selectedFile = 0;
		}
		if(i == SAVE_FILE_COUNT) {
			result = FILE_LIST_NO_FILE;
			goto done;
		}
	}

	/* The file list, and the mode's sprite above it. */
	btDmaFill32(0, spriteState, sizeof(*spriteState));
	mnFileListStartPulse();
	fileList = mnWindowCreate(1, 2, 28, 7, WINDOW_FLAG_INSTANT);
	summary = save->summary;
	y = 0;
	for(i = 0; i < SAVE_FILE_COUNT; i++, y += 16, summary++) {
		if(summary->level == 0) {
			mnTextDrawMessage(MsgSaveNoData, fileList, 10, y);
		}
		else if(summary->buildStamp < buildStamp) {
			/* Saved by an older build: "(Continue from a Sanctum)", since such
			 * a */
			/* file resumes at MAIN_ROUTINE_STATE.restartScene, not where it was
			 * saved */
			/* (scene/file_screen.c). */
			mnTextDrawMessage(MsgSaveContinue, fileList, 10, y);
		}
		else if(summary->playTime != summary->secondPartPlayTime) {
			mnTextDrawMessage(MsgSaveCorrupted, fileList, 10, y);
		}
		else if(menuMode == FILE_LIST_CLEARED && summary->sendFlg == 0) {
			mnTextDrawMessage(MsgSaveNotCleared, fileList, 10, y);
		}
		else {
			mnTextDrawString(summary->name, fileList, FILE_LIST_NAME_X, y);
			mnTextDrawMessage(summary->area + MsgDebugAreaName, fileList, 62,
							  y);
			state->dirty = 1;
		}
	}
	mnWindowDrawDividerLine(fileList, 0, 2, 27, 2);
	mnWindowDrawDividerLine(fileList, 0, 4, 27, 4);
#else
	if(menuMode != FILE_LIST_SAVE) {
		for(i = 0;
			i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0);
			i++) {
			selectedFile++;
			if(selectedFile == SAVE_FILE_COUNT)
				selectedFile = 0;
		}
		if(i == SAVE_FILE_COUNT) {
			result = FILE_LIST_NO_FILE;
			goto done;
		}
	}
	if(menuMode == FILE_LIST_CONTINUE) {
		for(i = 0;
			i < SAVE_FILE_COUNT && (save->summary[selectedFile].sendFlg != 0);
			i++) {
			selectedFile++;
			if(selectedFile == SAVE_FILE_COUNT)
				selectedFile = 0;
		}
		if(i == SAVE_FILE_COUNT) {
			result = FILE_LIST_NO_FILE;
			goto done;
		}
	}
	if(menuMode == FILE_LIST_BATTLE) {
		for(i = 0;
			i < SAVE_FILE_COUNT && (save->summary[selectedFile].djinnCnt == 0);
			i++) {
			selectedFile++;
			if(selectedFile == SAVE_FILE_COUNT)
				selectedFile = 0;
		}
		if(i == SAVE_FILE_COUNT) {
			result = FILE_LIST_NO_FILE;
			goto done;
		}
	}
	if(menuMode == FILE_LIST_CLEARED) {
		for(i = 0;
			i < SAVE_FILE_COUNT && (save->summary[selectedFile].sendFlg == 0);
			i++) {
			selectedFile++;
			if(selectedFile == SAVE_FILE_COUNT)
				selectedFile = 0;
		}
		if(i == SAVE_FILE_COUNT) {
			result = FILE_LIST_NO_FILE;
			goto done;
		}
	}

	/* The file list, and the mode's sprite above it. */
	btDmaFill32(0, spriteState, sizeof(*spriteState));
	fileList = mnWindowCreate(1, 2, 28, 7, WINDOW_FLAG_INSTANT);

	for(i = 0, summary = save->summary; i < SAVE_FILE_COUNT; i++, summary++) {
		if(save->summary[i].level == 0) {
			mnTextDrawMessage(MsgSaveNoData, fileList, 10, i * 16);
		}
		else if(save->summary[i].buildStamp < buildStamp) {
			/* Saved by an older build: see the other editions' list above. */
			mnTextDrawMessage(MsgSaveContinue, fileList, 10, i * 16);
		}
		else if(save->summary[i].playTime !=
				save->summary[i].secondPartPlayTime) {
			mnTextDrawMessage(MsgSaveCorrupted, fileList, 10, i * 16);
		}
		else if(menuMode == FILE_LIST_CLEARED &&
				save->summary[i].sendFlg == 0) {
			mnTextDrawMessage(MsgSaveNotCleared, fileList, 10, i * 16);
		}
		else {
			y = i * 16;
			mnTextDrawString(summary->name, fileList, FILE_LIST_NAME_X, y);
			mnTextDrawMessage(save->summary[i].area + MsgDebugAreaName,
							  fileList, 72, y);
			state->dirty = 1;
		}
	}
	mnWindowDrawDividerLine(fileList, 0, 2, 27, 2);
	mnWindowDrawDividerLine(fileList, 0, 4, 27, 4);
	vramSlot = syVramCacheFindFreeEntry();
	if(vramSlot < VRAM_CACHE_ENTRY_COUNT) {
		syVramCacheLoad(vramSlot, 128, PointingHandTiles);
		x = -12;
		y = selectedFile * 16 + 4;
		cursorSprite = mnWindowSprCreate(vramSlot, WINDOW_SPRITE_SIZE_16X16,
										 fileList, x, y);
		cursorSprite->motion = WINDOW_SPRITE_MOTION_STILL;
		fileCursor.anchor = cursorSprite;
	}
	cursorStyle = 1;
#endif
	modePicture = mnTitleCreate(menuMode, fileList, 72, -24);

	while(1) {
		/* Show the file under the cursor after it moves. */
		if(summaryDirty != 0) {
			summaryDirty = 0;
			if(save->summary[selectedFile].level != 0) {
				mnWindowColApply(save->summary[selectedFile].winHue,
								 save->summary[selectedFile].winBrightness);
				if(summaryWindow == NULL)
					summaryWindow =
						mnWindowCreate(1, 10, 14, 9, WINDOW_FLAG_INSTANT);
				mnFileListDrawSummary(summaryWindow,
									  &save->summary[selectedFile]);
				sySynch(1);
				if(partyWindow == NULL)
					partyWindow =
						mnWindowCreate(16, 10, 13, 3, WINDOW_FLAG_INSTANT);
				mnFileListHidePartySprs();
				mnFileListShowPartySprs(partyWindow, 0, 0,
										&save->summary[selectedFile]);
				sySynch(1);
				if(save->summary[selectedFile].djinn[0] +
					   save->summary[selectedFile].djinn[1] +
					   save->summary[selectedFile].djinn[2] +
					   save->summary[selectedFile].djinn[3] !=
				   0) {
					if(djinnWindow == NULL)
						djinnWindow =
							mnWindowCreate(16, 14, 13, 5, WINDOW_FLAG_INSTANT);
					mnFileListDrawDjinnCounts(djinnWindow,
											  &save->summary[selectedFile]);
					mnFileListHideDjinnSprs();
					mnFileListShowDjinnSprs(djinnWindow, 0, 0);
				}
				else {
					mnFileListHideDjinnSprs();
					mnWindowClose(djinnWindow, 2);
					djinnWindow = NULL;
				}
			}
			else {
				mnWindowColApply(MainRoutineStateWork.winHue,
								 MainRoutineStateWork.winBrightness);
				mnFileListHideDjinnSprs();
				mnFileListHidePartySprs();
				mnWindowClose(djinnWindow, 2);
				mnWindowClose(partyWindow, 2);
				mnWindowClose(summaryWindow, 2);
				djinnWindow = NULL;
				partyWindow = NULL;
				summaryWindow = NULL;
			}
#if TBS_PATCH_1
			mnWindowSprRedrawSavedRect(fileList);
			mnWindowDrawDividerLine(fileList, 0, 2, 27, 2);
			mnWindowDrawDividerLine(fileList, 0, 4, 27, 4);
			mnWindowFillTilemapRect(fileList, 0, selectedFile * 2, 26, 1);
		}
#else
			x = fileList->x * 8 - 4;
			y = (fileList->y << 3) + selectedFile * 16 + 12;
			mnCursorSetStub(&fileCursor, x, y, cursorStyle);
			cursorStyle = 3;
		}
		mnCursorAdvanceStub(&fileCursor);
#endif
		mnTitleBob(modePicture);
		sySynch(1);
		/* Up and down move to the previous or next file the mode allows; saving
		 */
		/* allows every file, an empty one too. */
		if(KeysRepeat & KEY_UP) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			summaryDirty = 1;
			do {
				selectedFile =
					(selectedFile + SAVE_FILE_COUNT - 1) % SAVE_FILE_COUNT;
				if(menuMode == FILE_LIST_SAVE)
					break;
			} while((save->summary[selectedFile].level == 0 ||
					 (menuMode == FILE_LIST_CONTINUE &&
					  save->summary[selectedFile].sendFlg != 0) ||
					 (menuMode == FILE_LIST_BATTLE &&
					  save->summary[selectedFile].djinnCnt == 0) ||
					 (menuMode == FILE_LIST_CLEARED &&
					  save->summary[selectedFile].sendFlg == 0)));
		}
		else if(KeysRepeat & KEY_DOWN) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			summaryDirty = 1;
			do {
				selectedFile =
					(selectedFile + SAVE_FILE_COUNT + 1) % SAVE_FILE_COUNT;
				if(menuMode == FILE_LIST_SAVE)
					break;
			} while((save->summary[selectedFile].level == 0 ||
					 (menuMode == FILE_LIST_CONTINUE &&
					  save->summary[selectedFile].sendFlg != 0) ||
					 (menuMode == FILE_LIST_BATTLE &&
					  save->summary[selectedFile].djinnCnt == 0) ||
					 (menuMode == FILE_LIST_CLEARED &&
					  save->summary[selectedFile].sendFlg == 0)));
		}
		else if(KeysPressed & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		else if(KeysPressed & KEY_A) {
			sySndPlayStub(SE_MENU_CONFIRM);
			result = selectedFile;
			break;
		}
	}
	/* Close the menu and return to the current game's palette glow. */
	mnFileListHideDjinnSprs();
	mnFileListHidePartySprs();
	mnWindowClose(djinnWindow, 2);
	mnWindowClose(partyWindow, 2);
	mnWindowClose(summaryWindow, 2);
	mnWindowClose(fileList, 2);
#if TBS_PATCH_1
	mnFileListStopPulse();
#endif
	syHeapSlotRelease(HEAP_SLOT_MENU);
	mnWindowColApply(MainRoutineStateWork.winHue,
					 MainRoutineStateWork.winBrightness);
	sySynch(1);
done:
	return result;
}

/* The Save command: lets the player choose a file (the current one first), */
/* asks whether to save over it when it has data, then saves the game to it, */
/* ActorsWork included, and makes it the current file. */
s32 mnSaveToChosenFile(void) {
	s32 result;
	s32 file;
	s32 error;
	SAVE_STATE *state;

	result = 0;
	error = sySaveInitState();
	if(error != 0) {
		mnFieldMessageShow(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
		result = SAVE_ERROR_NO_MEMORY;
	}
	else {
		sySaveLoadFileSummaries();
		state = SaveStateWork;
		file = mnFileListSelectFile(SaveSlot, FILE_LIST_SAVE);
		if(file == -1) {
			result = file;
		}
		else {
			if(state->summary[file].level != 0) {
				mnFieldMessageShow(MsgOverwriteConfirm,
								   FIELD_MESSAGE_ZOOM |
									   FIELD_MESSAGE_KEEP_OPEN |
									   FIELD_MESSAGE_LOWER);
				while(mnMessageAllTyped() == 0) {
					sySynch(1);
				}
				if(mnYesNoMenuRun(1, 0, 0, 1) != 0) {
					mnMessageCloseFinishedWindows();
					goto done;
				}
				mnMessageCloseFinishedWindows();
			}
			SaveSlot = file;
			sySndPlayStub(MUS_SAVED);
			mnFieldMessageShow(MsgSaving, FIELD_MESSAGE_ZOOM |
											  FIELD_MESSAGE_KEEP_OPEN |
											  FIELD_MESSAGE_LOWER);
			while(mnMessageAllTyped() == 0) {
				sySynch(1);
			}
			mnSaveBuildFileSummary();
			fdActorSnapshotStub();
			error = sySaveWritePart(file, SaveBuffer);
			error |= sySaveWritePart(file + SAVE_FILE_COUNT,
									 SaveBuffer + SAVE_PART_BYTES);
			mnMessageCloseFinishedWindows();
			if(error != 0) {
				mnFieldMessageShow(MsgSaveFailed, FIELD_MESSAGE_ZOOM);
				result = SAVE_ERROR_WRITE;
			}
			else {
				mnFieldMessageShow(MsgGameSaved,
								   FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_LOWER);
			}
		}
	}
done:
	sySaveReleaseState();
	return result;
}

/* Lets the player choose a file in file list `mode` (see FILE_MENU_MODE) and */
/* loads it into the save buffer as the current file, restoring its play time */
/* and options. */
s32 mnSaveChooseAndLoadFile(s32 mode) {
	s32 result = 0;
	s32 error = sySaveInitState();

	if(error != 0) {
		mnFieldMessageShow(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
		result = SAVE_ERROR_NO_MEMORY;
	}
	else {
		s32 file;

		sySaveLoadFileSummaries();
		file = mnFileListSelectFile(SaveSlot, mode);
		if(file == -1) {
			result = file;
		}
		else {
			void *half = &SaveBuffer;

			error = sySaveReadPart(file, half);
			half = (char *)half + SAVE_PART_BYTES;
			error |= sySaveReadPart(file + SAVE_FILE_COUNT, half);
			if(error != 0) {
				mnFieldMessageShow(MsgLoadFailed, FIELD_MESSAGE_ZOOM);
				result = SAVE_ERROR_READ;
			}
			else {
				PlayTimeFrames = MainRoutineStateWork.playTime;
				IdleSleepEnabled = MainRoutineStateWork.idleSleepEnabled;
				IdleFrames = 0;
				SaveSlot = file;
			}
		}
	}
	sySaveReleaseState();
	return result;
}

/* Loads the pointing hand cursor's tiles (PointingHandTiles, 128 bytes) into */
/* a free VRAM slot and returns the slot, which the caller frees with */
/* syVramCacheFreeEntry. */
s32 mnPointingHandLoadIntoFreeSlot(void) {
	s32 slot;

	slot = syVramCacheFindFreeEntry();
	syVramCacheLoad(slot, 128, PointingHandTiles);
	return slot;
}
