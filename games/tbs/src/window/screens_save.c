/* Save files and the file list. Part of screens.c. */

/* A file's half in bytes: the first is written to the file's slot, the second
   to the slot BUP_FILE_COUNT on. Then the file number for no free file. */
#define BUP_PART_BYTES	  4096
#define SAVE_NO_FREE_FILE 999

/* What the save functions return when they fail, after showing the message. */
#define SAVE_ERROR_READ		 -2 /* a part could not be read */
/* Writing failed. "Failed to save." (or "Failed to erase.") */
#define SAVE_ERROR_WRITE	 -3
#define SAVE_ERROR_ERASE	 -4 /* erasing failed. "Failed to erase." */
#define SAVE_ERROR_NO_FILE	 -5 /* no empty file to copy into */
/* No backup memory. "Unable to detect backup memory." */
#define SAVE_ERROR_NO_MEMORY -9

/* mnFileListSelectFile's result when no file can be chosen in its mode. */
#define FILE_LIST_NO_FILE -2

/* The play time shown stops at PLAY_TIME_HOURS_MAX hours and 59 minutes. */
#define PLAY_TIME_HOURS_MAX 999
#define MINUTES_PER_HOUR	60
#define SECONDS_PER_MINUTE	60

/* A two-digit field for mnStringFormatNum: with 100 added, its last two digits
   keep their leading zero. */
#define WITH_LEADING_ZERO(n) ((n) + 100)

/* The words of SceneLocalState (system/ewram.s), which InventorySnapshotWork
   follows. */
#define SCENE_LOCAL_STATE_WORDS 30
/* The words from GameFlagBytes that a file's checksum sums. */
#define BUP_CHECKSUM_WORDS		968

/* The file list's glow: the middle colour that HIGHLIGHT_PALETTE's colours 8 to
   11 sway round, red twice as far as green and blue. */
#define FILE_LIST_GLOW_RED	 22
#define FILE_LIST_GLOW_GREEN 16
#define FILE_LIST_GLOW_BLUE	 20

/* Writes the play time `ticks` (at 60 ticks a second) into `out` as hours and
   minutes, "hhh:mm" with the hours right-aligned in three characters, and
   returns `out`. The time stops at 999:59. */
void *mnStringFormatPlayTime(Sint32 ticks, Uint8 *out)
{
	Uint8  digits[64];
	Uint32 totalMinutes;
	Uint32 hours;
	Sint32 minutes;
	Uint8 *digitsCursor;
	Uint8 *textCursor;

	totalMinutes = (Uint32)ticks / (TICKS_PER_SECOND * SECONDS_PER_MINUTE);
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

/* Returns the first save file whose summary shows it empty (level 0), or
   SAVE_NO_FREE_FILE when all three are in use. The summaries must be loaded. */
Uint32 mnSaveFindFreeFile(void)
{
	Uint32			  file;
	BUP_FILE_SUMMARY *summary;

	summary = BupStateWork->summary;
	for(file = 0; file < BUP_FILE_COUNT; file++, summary++) {
		if(summary->level == 0)
			return file;
	}
	return SAVE_NO_FREE_FILE;
}

/* Returns how many save files there are, leaving out those whose summary has
   its send flag set (the files saved at the end of the game) when `excludeSent`
   is nonzero, or SAVE_ERROR_NO_MEMORY when the backup memory cannot be read. It
   opens and releases the save state itself. */
Sint32 mnSaveCountFilesExcludingSent(Sint32 excludeSent)
{
	Sint32			  file;
	Sint32			  cnt;
	BUP_FILE_SUMMARY *summary;

	if(syBupInitState() != 0) {
		cnt = SAVE_ERROR_NO_MEMORY;
	} else {
		cnt = syBupLoadFileSummaries();
		if(excludeSent != 0) {
			summary = BupStateWork->summary;
			for(file = 0; file < BUP_FILE_COUNT; file++) {
				if(summary[file].sendFlg != 0)
					cnt--;
			}
		}
	}
	syBupReleaseState();
	return cnt;
}

/* Loads the save file summaries for the File Screen and returns how many files
   there are: plus SAVE_SCAN_ALL_CLEARED when every file is cleared (its send
   flag set), or SAVE_ERROR_NO_MEMORY when the backup memory cannot be read. */
Sint32 mnSaveScanFileSummaries(void)
{
	Sint32 error;
	Sint32 sentCnt;
	Sint32 fileCnt;

	error = syBupInitState();
	sentCnt = 0;
	fileCnt = SAVE_ERROR_NO_MEMORY;
	if(error == 0) {
		Sint32	   i;
		BUP_STATE *state;

		fileCnt = syBupLoadFileSummaries();
		state = BupStateWork;
		TitleSendOptionEnabled = 0;
		TitleBattleOptionEnabled = 0;
		for(i = 0; i < BUP_FILE_COUNT; i++) {
			if(state->summary[i].sendFlg != 0) {
				TitleSendOptionEnabled = 1;
				sentCnt++;
			}
			if(state->summary[i].djinnCnt != 0) {
				TitleBattleOptionEnabled = 1;
			}
		}

		if((PerData & (KEY_R | KEY_LEFT)) != (KEY_R | KEY_LEFT)) {
			TitleSendOptionEnabled = 0;
		}
	}
	syBupReleaseState();
	if(fileCnt != 0 && sentCnt == fileCnt) {
		return fileCnt + SAVE_SCAN_ALL_CLEARED;
	}
	return fileCnt;
}

/* Prepares the save buffer for writing: stamps the main routine's state with
   the time and options, then fills the file summary shown on the file menus
   (the leader's name, level and class, the play time, area, coins, Djinn of
   each element, the party, the send flag and how many Djinn have been found)
   and its checksum, the sum of BUP_CHECKSUM_WORDS words from GameFlagBytes
   on. */
Uint32 mnSaveBuildFileSummary(void)
{
	Uint16			  members[14];
	BUP_FILE_SUMMARY *summary;
	UNIT			 *leader;
	Uint8			 *src;
	Uint8			 *dst;
	Uint32			 *word;
	Sint32			  i;
	Uint32			  sum = 0;

	/* The summary's header comes before the save buffer. */
	summary = (BUP_FILE_SUMMARY *)((Uint32)BupBuffer -
		offsetof(BUP_FILE_SUMMARY, name));
	MainRoutineStateWork.buildStamp = gmBuildStampGetTimeStub(0);
	MainRoutineStateWork.playTime = PlayTimeTicks;
	/* A second copy of the play time in the file's second part, which
	   syBupLoadFileSummaries reads back into
	   BUP_FILE_SUMMARY.secondPartPlayTime:
	   the file list lists a file whose two play times differ as corrupted. */
	/* UB: the copy is written through SceneLocalState past its own bytes, into
	   InventorySnapshotWork's last word; UBFIX writes it through
	   InventorySnapshotWork. */
	{
#if FIX_POINTER_OUTSIDE_OBJECT
		Uint32 *copy = (Uint32 *)InventorySnapshotWork;
		copy[SECOND_PART_PLAY_TIME_WORD - SCENE_LOCAL_STATE_WORDS] =
			PlayTimeTicks;
#else
		Uint32 *copy = (Uint32 *)SceneLocalState;
		copy[SECOND_PART_PLAY_TIME_WORD] = PlayTimeTicks;
#endif
	}
	MainRoutineStateWork.idleSleepEnabled = IdleSleepEnabled;

	leader = gmUnitGetStub(MainRoutineStateWork.leaderActorId);
	dst = summary->name;
	src = leader->name;
	for(i = 0; i < (Sint32)sizeof(summary->name); i++)
		*dst++ = *src++;
	summary->level = leader->level;
	summary->playTime = MainRoutineStateWork.playTime;
	summary->area = fdAreaGetNameIndexStub(
		MainRoutineStateWork.next, MainRoutineStateWork.entrance);
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
		Sint32 n;
		Uint32 ticks;

		ticks = MainRoutineStateWork.buildStamp;
		/* The checksum's words, counted in fours and multiplied back below. */
		n = BUP_CHECKSUM_WORDS / 4;
		i = 0;
		n <<= 2;
		word = (Uint32 *)GameFlagBytes;
		IO_WRITE32(ticks, &summary->buildStamp);
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

/* Saves the game to the current file (BupSlot): builds the summary and writes
   both halves of the save buffer, as parts BupSlot and BupSlot +
   BUP_FILE_COUNT. */
Sint16 mnSaveWriteCurrentFile(void)
{
	Sint16 file;
	Sint16 result;
	Sint32 error;

	result = 0;
	file = BupSlot;
	if(file != -1) {
		error = syBupInitState();
		if(error != 0) {
			mnFieldMessageShow(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
			result = SAVE_ERROR_NO_MEMORY;
		} else {
			mnSaveBuildFileSummary();
			{
				void  *half = &BupBuffer;
				Sint32 cur;

				error = syBupWritePart(BupSlot, half);
				cur = BupSlot;
				half = (char *)half + BUP_PART_BYTES;
				error |= syBupWritePart(cur + BUP_FILE_COUNT, half);
				if(error != 0) {
					mnFieldMessageShow(MsgSaveFailed, FIELD_MESSAGE_ZOOM);
					result = SAVE_ERROR_WRITE;
				}
			}
		}
		syBupReleaseState();
		file = result;
	}
	return file;
}

/* Writes both halves of the save buffer, as they are, to save file `file`.
   Returns 0, or a SAVE_ERROR_* after showing its message. */
Sint32 mnSaveWriteFile(Sint32 file)
{
	Sint32 error;
	Sint16 result = 0;

	error = syBupInitState();
	if(error != 0) {
		mnFieldMessageShow(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
		result = SAVE_ERROR_NO_MEMORY;
	} else {
		void *half = &BupBuffer;

		error = syBupWritePart(file, half);
		half = (char *)half + BUP_PART_BYTES;
		error |= syBupWritePart(file + BUP_FILE_COUNT, half);
		if(error != 0) {
			mnFieldMessageShow(MsgSaveFailed, FIELD_MESSAGE_ZOOM);
			result = SAVE_ERROR_WRITE;
		}
	}
	syBupReleaseState();
	return result;
}

/* Writes the 64-byte save stamp (BattleArenaSaveStamp, which lies in the first
   half of the save buffer) into the current file's first part, leaving the rest
   of the part as it was saved. */
Sint32 mnSaveWriteLinkTallies(void)
{
	void  *part;
	Sint32 result;
	Sint32 error;
	Uint8 *stamp;

	part = syHeapAllocEWRAM(BUP_PART_BYTES);
	result = 0;
	/* BUG: returns without freeing the part buffer. */
	if(BupSlot == -1)
		return -1;
	error = syBupInitState();
	if(error != 0) {
		mnFieldMessageShow(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
		result = SAVE_ERROR_NO_MEMORY;
	} else {
		error = syBupReadPart(BupSlot, part);
		/* A failed read is reported, but the part is still written below from
		   whatever the buffer holds. */
		if(error != 0) {
			mnFieldMessageShow(MsgSaveFailed, FIELD_MESSAGE_ZOOM);
			result = SAVE_ERROR_READ;
		}
		/* The stamp's place in the part is its place in the save buffer. */
		stamp = (Uint8 *)part + (Sint32)BattleArenaSaveStamp;
		stamp -= (Sint32)BupBuffer;
		syWordsCopy(stamp, BattleArenaSaveStamp, 16);
		error = syBupWritePart(BupSlot, part);
		if(error != 0) {
			mnFieldMessageShow(MsgSaveFailed, FIELD_MESSAGE_ZOOM);
			result = SAVE_ERROR_WRITE;
		}
	}
	syBupReleaseState();
	syHeapRelease(part);
	return result;
}

/* Asks whether to save over the current file and, on Yes, writes the save stamp
   into it (mnSaveWriteLinkTallies) and says so. */
UNUSED Sint32 mnSaveConfirmAndProcessSelectedFile(void)
{
	Sint32 result;

	/* UB: on No, result is returned without ever being set. */
#if FIX_UNINITIALIZED_READ
	result = 0;
#endif

	mnMessageOpenWindow(MsgOverwriteConfirm, 8, 12, 2);
	while(mnMessageAllTyped() == 0) {
		sySynch(1);
	}
	if(mnYesNoMenuRun(1, 0, 0, 1)) {
		mnMessageCloseFinishedWindows();
	} else {
		mnMessageCloseFinishedWindows();
		sySndPlayStub(MUS_SAVED);
		result = mnSaveWriteLinkTallies();
		if(result >= 0) {
			mnFieldMessageShow(MsgGameSaved, FIELD_MESSAGE_ZOOM);
		}
	}
	return result;
}

/* The file list's Copy (scene/file_screen.c): lets the player choose a file,
   reads both of its parts into the save buffer, overwriting it, and writes them
   to the first empty file. */
Sint32 mnSaveCopyFile(void)
{
	Uint32 error;
	Sint32 file;
	Sint32 result;

	result = 0;
	error = syBupInitState();
	if(error != 0) {
		mnFieldMessageShow(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
		result = SAVE_ERROR_NO_MEMORY;
	} else {
		syBupLoadFileSummaries();
		file = mnFileListSelectFile(0, FILE_LIST_COPY);
		if(file == -1) {
			result = file;
		} else {
			void *lower = &BupBuffer;
			void *upper;

			error = syBupReadPart(file, lower);
			upper = (Uint8 *)lower + BUP_PART_BYTES;
			error |= syBupReadPart(file + BUP_FILE_COUNT, upper);
			if(error != 0) {
				mnFieldMessageShow(MsgLoadFailed, FIELD_MESSAGE_ZOOM);
				result = SAVE_ERROR_READ;
			} else {
				file = mnSaveFindFreeFile();
				if(file == SAVE_NO_FREE_FILE) {
					mnFieldMessageShow(MsgEraseFailed, FIELD_MESSAGE_ZOOM);
					result = SAVE_ERROR_NO_FILE;
				} else {
					error = syBupWritePart(file, lower);
					error |= syBupWritePart(file + BUP_FILE_COUNT, upper);
					if(error != 0) {
						mnFieldMessageShow(MsgEraseFailed, FIELD_MESSAGE_ZOOM);
						result = SAVE_ERROR_WRITE;
					} else {
						mnFieldMessageShow(MsgFileCopied, FIELD_MESSAGE_ZOOM);
					}
				}
			}
		}
	}

	syBupReleaseState();
	return result;
}

/* The file list's Erase (scene/file_screen.c): lets the player choose a file,
   asks whether to erase it and on Yes deletes both of its parts. */
Sint32 mnSaveEraseSelectedFile(void)
{
	Sint32 error;
	Sint32 file;
	Sint32 result = 0;

	error = syBupInitState();
	if(error != 0) {
		mnFieldMessageShow(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
		result = SAVE_ERROR_NO_MEMORY;
	} else {
		syBupLoadFileSummaries();
		file = mnFileListSelectFile(0, FILE_LIST_ERASE);
		if(file == -1) {
			result = file;
		} else {
			mnMessageOpenWindow(MsgEraseConfirm, 8, 1, 2);
			while(mnMessageAllTyped() == 0) {
				sySynch(1);
			}
			if(mnYesNoMenuRun(1, 0, 3, 1) != 0) {
				mnMessageCloseFinishedWindows();
			} else {
				mnMessageCloseFinishedWindows();
				error = syBupDeletePart(file);
				error |= syBupDeletePart(file + BUP_FILE_COUNT);
				if(error != 0) {
					mnFieldMessageShow(MsgEraseFailed, FIELD_MESSAGE_ZOOM);
					result = SAVE_ERROR_ERASE;
				} else {
					mnFieldMessageShow(MsgFileErased, FIELD_MESSAGE_ZOOM);
				}
			}
		}
	}
	syBupReleaseState();
	return result;
}

#if FILE_LIST_GLOW_BAR_CURSOR

/* An event that makes colours 8-11 of HIGHLIGHT_PALETTE glow: each follows a
   sine wave of the tick counter, eight ticks behind the one before. */
void mnFileListUpdatePulse(void)
{
	Sint32			*frmCnt;
	volatile Uint16 *palette;
	Sint32			 colour;
	FIXED			 sine;
	Sint32			 i;

	frmCnt = (Sint32 *)&FrameCount;
	palette = &BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 8);
	i = 0;
	do {
#if FIX_SIGNED_OVERFLOW
		/* Wrap the tick phase before sySin takes its low sixteen bits. */
		sine = sySin((((Uint32)*frmCnt + i * 8U) * (TRIG_TURN * 3 / 256U)) &
			TRIG_ANGLE_MASK);
#else
		sine = sySin((*frmCnt + i * 8) * (TRIG_TURN * 3 / 256));
#endif
		colour = sine / (toFIXED(1) / 4);
		{
#if FIX_UNDEFINED_SHIFT
			/* A negative sine must be shifted as unsigned bits. */
			Uint32 red = (Uint32)colour << 1;
#else
			Uint32 red = colour << 1;
#endif
			Uint32 green = colour;

			red += FILE_LIST_GLOW_RED;
			green += FILE_LIST_GLOW_GREEN;
			colour += FILE_LIST_GLOW_BLUE;
			IO_WRITE16(RGB(red, green, colour), palette++);
		}
	} while(++i <= 3);
}

#endif

#if FILE_LIST_GLOW_BAR_CURSOR

/* Starts the palette glow (mnFileListUpdatePulse) as an event. */
void mnFileListStartPulse(void)
{
	sySetEvent(mnFileListUpdatePulse, EVENT_PASS_TICK);
}

/* Stops the file list's glow (mnFileListUpdatePulse). */
void mnFileListStopPulse(void)
{
	syCloseEvent(mnFileListUpdatePulse);
}

/* Fills the `width` x `height` tile rectangle at (x, y) inside `win`,
   clipped to the screen as mnWindowSetInteriorPaletteBit clips, with the tile
   entry of tile 6 in HIGHLIGHT_PALETTE, the palette mnFileListUpdatePulse
   animates. */
void mnWindowFillTilemapRect(
	WINDOW *win, Sint32 x, Sint32 y, Sint32 width, Sint32 height)
{
	Uint16 *tilemap = (Uint16 *)WindowStateWork[0];
	Sint32	sum;

	sum = x + win->x;
	x = sum + 1;
	sum = y + (win)->y;
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
	/* BUG: the bottom is tested against the screen's width, so a rectangle
	   ending below row DISPLAY_TILE_HEIGHT writes past WIN_STATE.tilemap; the
	   one caller, the file list's highlight, never does. */
#if FIX_RECT_BOTTOM_CLIP
	if(y + height > DISPLAY_TILE_HEIGHT - 1)
#else
	if(y + height > DISPLAY_TILE_WIDTH - 1)
#endif
		height = DISPLAY_TILE_HEIGHT - y;

	if(width > 0 && height > 0) {
		/* Byte offsets: BG_SCREEN_WIDTH entries of 2 bytes a row. */
		Sint32 yOfs = y * (BG_SCREEN_WIDTH * 2);
		Sint32 xOfs = x << 1;
		Sint32 ofs = yOfs + xOfs;
		do {
			Sint32	remaining = width;
			Uint16 *cell = (Uint16 *)(ofs + (Sint32)tilemap);
			for(; remaining != 0; remaining--)
				*cell++ = BG_TILE_PALETTE(HIGHLIGHT_PALETTE) | 6;
			height--;
			ofs += BG_SCREEN_WIDTH * 2;
		} while(height != 0);
		((WIN_STATE *)tilemap)->dirty = 1;
	}
}

#endif

/* The file list shows the chosen file's party as standing sprites in a row, and
   four Djinn sprites, one for each element, over the file's Djinn counts. */
#define FILE_LIST_SPRITE_SPACING 3

/* Shows the party of save file `summary` (up to four party members) as sprites
   in a row from tile (x, y) inside `win`, and starts placing them every
   tick (mnFileListPlacePartySprs). */
void mnFileListShowPartySprs(
	WINDOW *win, Sint32 x, Sint32 y, BUP_FILE_SUMMARY *summary)
{
	SHOP_STATE *menu = MenuState;
	Sint32		i;

	for(i = 0; i < 4 && summary->party[i] != -1; i++) {
		SPR *spr = fdSprCreateStub(fdPartyGetCharacterSprForChildhoodStub(
			summary->party[i], summary->childhood));

		if(spr != NULL) {
			fdSprStartAnimationStub(spr, ANIM_STAND);
			spr->flg = 0;
			spr->image.pri = 0;
		}

		menu->partyMemberIcons[i] = spr;
		menu->partyMemberX[i] =
			(x + win->x + i * FILE_LIST_SPRITE_SPACING) * 8 + 16;
		menu->partyMemberY[i] = (y + win->y) * 8 + 16;
		menu->partyMemberScale[i] = toFIXED(1);
	}

	sySetEvent(mnFileListPlacePartySprs, EVENT_PASS_TICK);
}

/* Stops placing the party sprites of the file list and releases them. */
void mnFileListHidePartySprs(void)
{
	SHOP_STATE *menu = MenuState;
	Sint32		i;

	syCloseEvent(mnFileListPlacePartySprs);
	for(i = 0; i < 4; i++) {
		if(menu->partyMemberIcons[i] != NULL) {
			fdSprDestroyStub(menu->partyMemberIcons[i]);
			menu->partyMemberIcons[i] = NULL;
		}
	}
}

/* An event: draws each party sprite of the file list at its place and scale for
   this tick. */
void mnFileListPlacePartySprs(void)
{
	SHOP_STATE	 *menu = MenuState;
	SPR_SCALE	  scale;
	SPR_PLACEMENT pos;
	Sint32		  idx = 0;
	SPR			 *spr;

	while(1) {
		spr = menu->partyMemberIcons[idx];
		if(spr != 0) {
			/* Offset height and y equally to keep the visible row fixed. */
			scale.x = menu->partyMemberScale[idx];
			scale.y = menu->partyMemberScale[idx];
			pos.x = toFIXED(menu->partyMemberX[idx]);
			pos.height = toFIXED(500);
			pos.y = toFIXED(menu->partyMemberY[idx]) + toFIXED(500);
			pos.groundHeight = 0;
			fdRenderApplyProjectedPlacementStub(
				spr, &pos, &scale, FACING_SOUTH);
		}
		idx += 1;
		if(idx > 3) {
			break;
		}
	}
}

/* Shows the four Djinn sprites, one for each element, in a row from tile (x, y)
   inside `win`, and starts placing them every tick
   (mnFileListPlaceDjinnSprs). Does nothing when `win` is NULL. */
void mnFileListShowDjinnSprs(WINDOW *win, Sint32 x, Sint32 y)
{
	ITEM_MENU_STATE *menu = MenuState;
	Sint32			 i;

	if(win != NULL) {
		for(i = 0; i < 4; i++) {
			SPR *spr = fdSprCreateStub(FileListDjinnSpriteIds[i]);

			if(spr != NULL) {
				fdSprStartAnimationStub(spr, ANIM_WALK);
				spr->flg = 0;
				spr->image.pri = 0;
			}

			menu->figures[i] = spr;
			menu->figureX[i] =
				(win->x + x + i * FILE_LIST_SPRITE_SPACING) * 8 + 16;
			menu->figureY[i] = (win->y + y) * 8 + 16;
		}

		sySetEvent(mnFileListPlaceDjinnSprs, EVENT_PASS_TICK);
	}
}

/* Stops placing the Djinn sprites of the file list and releases them. */
void mnFileListHideDjinnSprs(void)
{
	ITEM_MENU_STATE *menu = MenuState;
	Sint32			 i;

	syCloseEvent(mnFileListPlaceDjinnSprs);
	for(i = 0; i < 4; i++) {
		if(menu->figures[i] != NULL) {
			fdSprDestroyStub(menu->figures[i]);
			menu->figures[i] = NULL;
		}
	}
}

/* An event: draws each Djinn sprite of the file list at its place, at scale
   1.0, for this tick. */
void mnFileListPlaceDjinnSprs(void)
{
	ITEM_MENU_STATE *menu = MenuState;
	SPR_SCALE		 scale;
	SPR_PLACEMENT	 pos;
	Sint32			 idx;

	for(idx = 0; idx <= 3; idx++) {
		SPR *spr = menu->figures[idx];

		if(spr != NULL) {
			FIXED fullSize = toFIXED(1);
			FIXED height = toFIXED(500);

			scale.x = fullSize;
			scale.y = fullSize;
			pos.x = toFIXED(menu->figureX[idx]);
			pos.height = height;
			pos.y = toFIXED(menu->figureY[idx]) + height;
			pos.groundHeight = 0;
			fdRenderApplyProjectedPlacementStub(
				spr, &pos, &scale, FACING_SOUTH);
		}
	}
}

/* Draws the Djinn counts of save file `summary`, one for each element, two
   digits each, 24 pixels apart on row 16 of `win`, under the Djinn
   sprites. */
void mnFileListDrawDjinnCounts(WINDOW *win, BUP_FILE_SUMMARY *summary)
{
	Sint32 i;

	if(win != NULL) {
		for(i = 0; i < 4; i++)
			mnTextDrawNumAtOffset(summary->djinn[i], 2, win,
				i * FILE_LIST_SPRITE_SPACING * 8, 16);
	}
}

/* Where each edition draws the "Lv" label and the coin label, in pixels. */
#define SUMMARY_LEVEL_X BY_LANGUAGE(72, 72, 72, 64, 59, 72)
#define SUMMARY_COINS_X BY_LANGUAGE(48, 48, 48, 56, 56, 48)
#define SUMMARY_DRAW_LEVEL \
	BY_LANGUAGE(mnTextDrawStringInWindow, mnTextDrawStringInWindow, \
		mnTextDrawStringInWindow, mnTextDrawStringAtOffset, \
		mnTextDrawStringAtOffset, mnTextDrawStringInWindow)

/* Draws the summary of save file `summary` into `win`: the leader's name,
   level and class, the play time and the coins. */
void mnFileListDrawSummary(WINDOW *win, BUP_FILE_SUMMARY *summary)
{
	Uint32 playTime[4];
	Sint32 y;

	if(win != 0) {
		mnWindowSprPrepareForRedraw(win);
		mnWindowDrawDividerLine(win, 0, 4, 13, 4);
		mnTextDrawStringAtOffset(summary->name, win, 0, 0);
		y = 0;
		SUMMARY_DRAW_LEVEL(
			StatusScreen_LevelLetterString, win, SUMMARY_LEVEL_X, 0);
		mnTextDrawNumAtOffset(summary->level, 2, win, 80, y);
		mnTextDrawMessageAsTiles(summary->classId + MsgClassName, win, 0, 16);
#if LANGUAGE == LANGUAGE_JAPANESE
		/* Coins on the third row, then "Play Time:" and the time. */
		y = 32;
		mnTextDrawNumInWindow(summary->coins, 6, win, 24, y);
		mnTextDrawMessageAsTiles(MsgCoins, win, 72, 32);
		mnTextDrawMessageAsTiles(MsgStatusLabel, win, 0, 48);
		mnTextDrawStringInWindow(
			mnStringFormatPlayTime(summary->playTime, (Uint8 *)playTime), win,
			48, 48);
#else
		/* "Play Time:" with the time under it, then the coins. */
		mnTextDrawMessageAsTiles(MsgStatusLabel, win, 0, 32);
		mnTextDrawStringInWindow(
			mnStringFormatPlayTime(summary->playTime, (Uint8 *)playTime), win,
			48, 40);
		y = 48;
		mnTextDrawNumInWindow(summary->coins, 6, win, 0, y);
#if LANGUAGE == LANGUAGE_FRENCH
		/* French takes the singular "Pièce" for one coin or none. */
		if(summary->coins > 1)
			mnTextDrawMessageAsTiles(MsgCoins, win, SUMMARY_COINS_X, 48);
		else
			mnTextDrawMessageAsTiles(MsgPiece, win, SUMMARY_COINS_X, 48);
#else
		mnTextDrawMessageAsTiles(MsgCoins, win, SUMMARY_COINS_X, 48);
#endif
#endif
	}
}

/* The file list: lists the three files with each one's leader and area (or why
   it cannot be chosen) and, for the file under the cursor, its summary, its
   party and its Djinn, with the file's own palette glow. */
Sint32 mnFileListSelectFile(Sint32 selectedFile, Sint32 menuMode)
{
	WINDOW *fileList;
#if FILE_LIST_GLOW_BAR_CURSOR
	BUP_STATE		 *save;
	WINDOW			 *summaryWin;
	WINDOW			 *partyWin;
	WINDOW			 *djinnWin;
	BUP_FILE_SUMMARY *summary;
	WINDOW_SPR		 *modePicture;
	Bool			  summaryDirty;
	WIN_STATE		 *state;
	Uint32			  buildStamp;
	SHOP_STATE		 *sprState;
	Sint32			  result;
	Sint32			  i;
	Sint32			  y;
#else
	WINDOW			 *summaryWin;
	BUP_STATE		 *save;
	WINDOW			 *partyWin;
	WINDOW			 *djinnWin;
	BUP_FILE_SUMMARY *summary;
	WINDOW_SPR		 *modePicture;
	Sint32			  cursorStyle;
	Bool			  summaryDirty;
	WIN_STATE		 *state;
	Uint32			  buildStamp;
	SHOP_STATE		 *sprState;
	MENU_CURSOR		  fileCursor;
	Sint32			  result;
	Sint32			  i;
	Sint32			  x;
	Sint32			  y;
	Sint32			  vramSlot;
	WINDOW_SPR		 *cursorSpr;
#endif

	sprState = syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(*sprState));
	save = BupStateWork;
	summaryWin = NULL;
	partyWin = NULL;
	djinnWin = NULL;
	summaryDirty = TRUE;
	state = WINDOW_STATE;
	buildStamp = gmBuildStampGetTimeStub();
	/* Start on the first file the mode allows. */
	/* BUG: when none is, the jump to done returns FILE_LIST_NO_FILE without
	   releasing sprState's HEAP_SLOT_MENU block. */
	if(selectedFile < 0)
		selectedFile = 0;
#if FILE_LIST_GLOW_BAR_CURSOR
	if(menuMode == FILE_LIST_CONTINUE) {
		for(i = 0; i < BUP_FILE_COUNT &&
			(save->summary[selectedFile].level == 0 ||
				save->summary[selectedFile].sendFlg != 0);
			i++) {
			selectedFile++;
			if(selectedFile == BUP_FILE_COUNT)
				selectedFile = 0;
		}
		if(i == BUP_FILE_COUNT) {
			result = FILE_LIST_NO_FILE;
			goto done;
		}
	} else if(menuMode == FILE_LIST_BATTLE) {
		for(i = 0; i < BUP_FILE_COUNT &&
			(save->summary[selectedFile].level == 0 ||
				save->summary[selectedFile].djinnCnt == 0);
			i++) {
			selectedFile++;
			if(selectedFile == BUP_FILE_COUNT)
				selectedFile = 0;
		}
		if(i == BUP_FILE_COUNT) {
			result = FILE_LIST_NO_FILE;
			goto done;
		}
	} else if(menuMode == FILE_LIST_CLEARED) {
		for(i = 0; i < BUP_FILE_COUNT &&
			(save->summary[selectedFile].level == 0 ||
				save->summary[selectedFile].sendFlg == 0);
			i++) {
			selectedFile++;
			if(selectedFile == BUP_FILE_COUNT)
				selectedFile = 0;
		}
		if(i == BUP_FILE_COUNT) {
			result = FILE_LIST_NO_FILE;
			goto done;
		}
	} else if(menuMode != FILE_LIST_SAVE) {
		for(i = 0;
			i < BUP_FILE_COUNT && (save->summary[selectedFile].level == 0);
			i++) {
			selectedFile++;
			if(selectedFile == BUP_FILE_COUNT)
				selectedFile = 0;
		}
		if(i == BUP_FILE_COUNT) {
			result = FILE_LIST_NO_FILE;
			goto done;
		}
	}

	/* The file list, and the mode's sprite above it. */
	DMA_FILL32(0, sprState, sizeof(*sprState));
	mnFileListStartPulse();
	fileList = mnWindowCreate(1, 2, 28, 7, WINDOW_FLAG_INSTANT);
	summary = save->summary;
	y = 0;
	for(i = 0; i < BUP_FILE_COUNT; i++, y += 16, summary++) {
		if(summary->level == 0) {
			mnTextDrawMessage(MsgSaveNoData, fileList, 10, y);
		} else if(summary->buildStamp < buildStamp) {
			/* Saved by an older build: "(Continue from a Sanctum)", since such
			   a file resumes at MAIN_ROUTINE_STATE.restartScene, not where it
			   was saved (scene/file_screen.c). */
			mnTextDrawMessage(MsgSaveContinue, fileList, 10, y);
		} else if(summary->playTime != summary->secondPartPlayTime) {
			mnTextDrawMessage(MsgSaveCorrupted, fileList, 10, y);
		} else if(menuMode == FILE_LIST_CLEARED && summary->sendFlg == 0) {
			mnTextDrawMessage(MsgSaveNotCleared, fileList, 10, y);
		} else {
			mnTextDrawString(summary->name, fileList, FILE_LIST_NAME_X, y);
			mnTextDrawMessage(
				summary->area + MsgDebugAreaName, fileList, 62, y);
			state->dirty = 1;
		}
	}
	mnWindowDrawDividerLine(fileList, 0, 2, 27, 2);
	mnWindowDrawDividerLine(fileList, 0, 4, 27, 4);
#else
	if(menuMode != FILE_LIST_SAVE) {
		for(i = 0;
			i < BUP_FILE_COUNT && (save->summary[selectedFile].level == 0);
			i++) {
			selectedFile++;
			if(selectedFile == BUP_FILE_COUNT)
				selectedFile = 0;
		}
		if(i == BUP_FILE_COUNT) {
			result = FILE_LIST_NO_FILE;
			goto done;
		}
	}
	if(menuMode == FILE_LIST_CONTINUE) {
		for(i = 0;
			i < BUP_FILE_COUNT && (save->summary[selectedFile].sendFlg != 0);
			i++) {
			selectedFile++;
			if(selectedFile == BUP_FILE_COUNT)
				selectedFile = 0;
		}
		if(i == BUP_FILE_COUNT) {
			result = FILE_LIST_NO_FILE;
			goto done;
		}
	}
	if(menuMode == FILE_LIST_BATTLE) {
		for(i = 0;
			i < BUP_FILE_COUNT && (save->summary[selectedFile].djinnCnt == 0);
			i++) {
			selectedFile++;
			if(selectedFile == BUP_FILE_COUNT)
				selectedFile = 0;
		}
		if(i == BUP_FILE_COUNT) {
			result = FILE_LIST_NO_FILE;
			goto done;
		}
	}
	if(menuMode == FILE_LIST_CLEARED) {
		for(i = 0;
			i < BUP_FILE_COUNT && (save->summary[selectedFile].sendFlg == 0);
			i++) {
			selectedFile++;
			if(selectedFile == BUP_FILE_COUNT)
				selectedFile = 0;
		}
		if(i == BUP_FILE_COUNT) {
			result = FILE_LIST_NO_FILE;
			goto done;
		}
	}

	/* The file list, and the mode's sprite above it. */
	DMA_FILL32(0, sprState, sizeof(*sprState));
	fileList = mnWindowCreate(1, 2, 28, 7, WINDOW_FLAG_INSTANT);

	for(i = 0, summary = save->summary; i < BUP_FILE_COUNT; i++, summary++) {
		if(save->summary[i].level == 0) {
			mnTextDrawMessage(MsgSaveNoData, fileList, 10, i * 16);
		} else if(save->summary[i].buildStamp < buildStamp) {
			/* Saved by an older build: see the other editions' list above. */
			mnTextDrawMessage(MsgSaveContinue, fileList, 10, i * 16);
		} else if(save->summary[i].playTime !=
			save->summary[i].secondPartPlayTime) {
			mnTextDrawMessage(MsgSaveCorrupted, fileList, 10, i * 16);
		} else if(menuMode == FILE_LIST_CLEARED &&
			save->summary[i].sendFlg == 0) {
			mnTextDrawMessage(MsgSaveNotCleared, fileList, 10, i * 16);
		} else {
			y = i * 16;
			mnTextDrawString(summary->name, fileList, FILE_LIST_NAME_X, y);
			mnTextDrawMessage(
				save->summary[i].area + MsgDebugAreaName, fileList, 72, y);
			state->dirty = 1;
		}
	}
	mnWindowDrawDividerLine(fileList, 0, 2, 27, 2);
	mnWindowDrawDividerLine(fileList, 0, 4, 27, 4);
	vramSlot = syVRAMCacheFindFreeEntry();
	if(vramSlot < VRAM_CACHE_ENTRY_COUNT) {
		syVRAMCacheLoad(vramSlot, 128, PointingHandTiles);
		x = -12;
		y = selectedFile * 16 + 4;
		cursorSpr = mnWindowSprCreate(
			vramSlot, WINDOW_SPRITE_SIZE_16X16, fileList, x, y);
		cursorSpr->motion = WINDOW_SPRITE_MOTION_STILL;
		fileCursor.anchor = cursorSpr;
	}
	cursorStyle = 1;
#endif
	modePicture = mnTitleCreate(menuMode, fileList, 72, -24);

	while(1) {
		/* Show the file under the cursor after it moves. */
		if(summaryDirty != 0) {
			summaryDirty = FALSE;
			if(save->summary[selectedFile].level != 0) {
				mnWindowColApply(save->summary[selectedFile].winHue,
					save->summary[selectedFile].winBrightness);
				if(summaryWin == NULL)
					summaryWin =
						mnWindowCreate(1, 10, 14, 9, WINDOW_FLAG_INSTANT);
				mnFileListDrawSummary(summaryWin, &save->summary[selectedFile]);
				sySynch(1);
				if(partyWin == NULL)
					partyWin =
						mnWindowCreate(16, 10, 13, 3, WINDOW_FLAG_INSTANT);
				mnFileListHidePartySprs();
				mnFileListShowPartySprs(
					partyWin, 0, 0, &save->summary[selectedFile]);
				sySynch(1);
				if(save->summary[selectedFile].djinn[0] +
						save->summary[selectedFile].djinn[1] +
						save->summary[selectedFile].djinn[2] +
						save->summary[selectedFile].djinn[3] !=
					0) {
					if(djinnWin == NULL)
						djinnWin =
							mnWindowCreate(16, 14, 13, 5, WINDOW_FLAG_INSTANT);
					mnFileListDrawDjinnCounts(
						djinnWin, &save->summary[selectedFile]);
					mnFileListHideDjinnSprs();
					mnFileListShowDjinnSprs(djinnWin, 0, 0);
				} else {
					mnFileListHideDjinnSprs();
					mnWindowClose(djinnWin, 2);
					djinnWin = NULL;
				}
			} else {
				mnWindowColApply(MainRoutineStateWork.winHue,
					MainRoutineStateWork.winBrightness);
				mnFileListHideDjinnSprs();
				mnFileListHidePartySprs();
				mnWindowClose(djinnWin, 2);
				mnWindowClose(partyWin, 2);
				mnWindowClose(summaryWin, 2);
				djinnWin = NULL;
				partyWin = NULL;
				summaryWin = NULL;
			}
#if FILE_LIST_GLOW_BAR_CURSOR
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
		   allows every file, an empty one too. */
		if(PerRepeat & KEY_UP) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			summaryDirty = TRUE;
			do {
				selectedFile =
					(selectedFile + BUP_FILE_COUNT - 1) % BUP_FILE_COUNT;
				if(menuMode == FILE_LIST_SAVE)
					break;
			} while((save->summary[selectedFile].level == 0 ||
				(menuMode == FILE_LIST_CONTINUE &&
					save->summary[selectedFile].sendFlg != 0) ||
				(menuMode == FILE_LIST_BATTLE &&
					save->summary[selectedFile].djinnCnt == 0) ||
				(menuMode == FILE_LIST_CLEARED &&
					save->summary[selectedFile].sendFlg == 0)));
		} else if(PerRepeat & KEY_DOWN) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			summaryDirty = TRUE;
			do {
				selectedFile =
					(selectedFile + BUP_FILE_COUNT + 1) % BUP_FILE_COUNT;
				if(menuMode == FILE_LIST_SAVE)
					break;
			} while((save->summary[selectedFile].level == 0 ||
				(menuMode == FILE_LIST_CONTINUE &&
					save->summary[selectedFile].sendFlg != 0) ||
				(menuMode == FILE_LIST_BATTLE &&
					save->summary[selectedFile].djinnCnt == 0) ||
				(menuMode == FILE_LIST_CLEARED &&
					save->summary[selectedFile].sendFlg == 0)));
		} else if(PerPush & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		} else if(PerPush & KEY_A) {
			sySndPlayStub(SE_MENU_CONFIRM);
			result = selectedFile;
			break;
		}
	}
	/* Close the menu and return to the current game's palette glow. */
	mnFileListHideDjinnSprs();
	mnFileListHidePartySprs();
	mnWindowClose(djinnWin, 2);
	mnWindowClose(partyWin, 2);
	mnWindowClose(summaryWin, 2);
	mnWindowClose(fileList, 2);
#if FILE_LIST_GLOW_BAR_CURSOR
	mnFileListStopPulse();
#endif
	syHeapSlotRelease(HEAP_SLOT_MENU);
	mnWindowColApply(
		MainRoutineStateWork.winHue, MainRoutineStateWork.winBrightness);
	sySynch(1);
done:
	return result;
}

/* The Save command: lets the player choose a file (the current one first), asks
   whether to save over it when it has data, then saves the game to it,
   ActorsWork included, and makes it the current file. */
Sint32 mnSaveToChosenFile(void)
{
	Sint32	   result;
	Sint32	   file;
	Sint32	   error;
	BUP_STATE *state;

	result = 0;
	error = syBupInitState();
	if(error != 0) {
		mnFieldMessageShow(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
		result = SAVE_ERROR_NO_MEMORY;
	} else {
		syBupLoadFileSummaries();
		state = BupStateWork;
		file = mnFileListSelectFile(BupSlot, FILE_LIST_SAVE);
		if(file == -1) {
			result = file;
		} else {
			if(state->summary[file].level != 0) {
				mnFieldMessageShow(MsgOverwriteConfirm,
					FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN |
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
			BupSlot = file;
			sySndPlayStub(MUS_SAVED);
			mnFieldMessageShow(MsgSaving,
				FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN |
					FIELD_MESSAGE_LOWER);
			while(mnMessageAllTyped() == 0) {
				sySynch(1);
			}
			mnSaveBuildFileSummary();
			fdActorSnapshotStub();
			error = syBupWritePart(file, BupBuffer);
			error |= syBupWritePart(
				file + BUP_FILE_COUNT, BupBuffer + BUP_PART_BYTES);
			mnMessageCloseFinishedWindows();
			if(error != 0) {
				mnFieldMessageShow(MsgSaveFailed, FIELD_MESSAGE_ZOOM);
				result = SAVE_ERROR_WRITE;
			} else {
				mnFieldMessageShow(
					MsgGameSaved, FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_LOWER);
			}
		}
	}
done:
	syBupReleaseState();
	return result;
}

/* Lets the player choose a file in file list `mode` (see FILE_MENU_MODE) and
   loads it into the save buffer as the current file, restoring its play time
   and options. */
Sint32 mnSaveChooseAndLoadFile(Sint32 mode)
{
	Sint32 result = 0;
	Sint32 error = syBupInitState();

	if(error != 0) {
		mnFieldMessageShow(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
		result = SAVE_ERROR_NO_MEMORY;
	} else {
		Sint32 file;

		syBupLoadFileSummaries();
		file = mnFileListSelectFile(BupSlot, mode);
		if(file == -1) {
			result = file;
		} else {
			void *half = &BupBuffer;

			error = syBupReadPart(file, half);
			half = (char *)half + BUP_PART_BYTES;
			error |= syBupReadPart(file + BUP_FILE_COUNT, half);
			if(error != 0) {
				mnFieldMessageShow(MsgLoadFailed, FIELD_MESSAGE_ZOOM);
				result = SAVE_ERROR_READ;
			} else {
				PlayTimeTicks = MainRoutineStateWork.playTime;
				IdleSleepEnabled = MainRoutineStateWork.idleSleepEnabled;
				IdleTicks = 0;
				BupSlot = file;
			}
		}
	}
	syBupReleaseState();
	return result;
}

/* Loads the pointing hand cursor's tiles (PointingHandTiles, 128 bytes) into a
   free VRAM slot and returns the slot, which the caller frees with
   syVRAMCacheFreeEntry. */
Sint32 mnPointingHandLoadIntoFreeSlot(void)
{
	Sint32 slot;

	slot = syVRAMCacheFindFreeEntry();
	syVRAMCacheLoad(slot, 128, PointingHandTiles);
	return slot;
}
