/* Save files, the file list and the saving screen's progress bar. Part of
   screens.c. */

#include "game/stubs.h"

/* What mnSaveFindFreeFile returns when all BUP_FILE_COUNT files are in use. */
#define SAVE_NO_FREE_FILE 999

/* What the save functions return when they fail, after showing the message. */
#define SAVE_ERROR_READ		 -2 /* a part could not be read */
/* Writing failed. "Failed to save." (or "Failed to erase.") */
#define SAVE_ERROR_WRITE	 -3
#define SAVE_ERROR_ERASE	 -4 /* erasing failed. "Failed to erase." */
#define SAVE_ERROR_NO_FILE	 -5 /* no empty file to copy into */
/* No backup memory. "Unable to detect backup memory." */
#define SAVE_ERROR_NO_MEMORY -9

/* The play time shown stops at PLAY_TIME_HOURS_MAX hours and 59 minutes. */
#define PLAY_TIME_HOURS_MAX 999
#define MINUTES_PER_HOUR	60
#define SECONDS_PER_MINUTE	60

/* A two-digit field for mnStringFormatNum: with 100 added, its last two digits
   keep their leading zero. */
#define WITH_LEADING_ZERO(n) ((n) + 100)

/* The words from GameFlagBytes that a file's checksum sums. */
#define BUP_CHECKSUM_WORDS 968

/* The file list's glow: the middle colour that HIGHLIGHT_PALETTE's colours 8 to
   11 sway round, red twice as far as green and blue. */
#define FILE_LIST_GLOW_RED	 22
#define FILE_LIST_GLOW_GREEN 16
#define FILE_LIST_GLOW_BLUE	 20

/* The tiles between two of the file list's party sprites, and between two of
   its Djinn counts, one for each element. */
#define FILE_LIST_SPRITE_SPACING 3
/* How high the file list lifts a party sprite, in pixels, and how far it brings
   it forward: the two cancel on screen, and its depth runs past the field's
   last slot, so it is drawn in front of every field sprite and behind the
   windows' (OAM_DEPTH_WINDOW). */
#define FILE_LIST_SPRITE_LIFT	 500

#include "field/stubs.h"

/* The progress bar: twelve tiles wide at tile column 9, row 9 of the window
   map, drawn from tiles 128 on (mnSaveProgressOpen loads them), in seven
   stages. */
#define PROGRESS_BAR_X			9
#define PROGRESS_BAR_Y			9
#define PROGRESS_BAR_WIDTH		12
#define PROGRESS_BAR_LAST_ROW	6
#define PROGRESS_BAR_FIRST_TILE 128

/* The music fades out under the progress screen and back after it by this much
   of BGM_UNITY_SCALE a frame. */
#define PROGRESS_MUSIC_FADE_STEP 30

/* The progress bar's serial callback: advances the stored progress by step and
   draws row min(progress, 6) of WindowProgressBarRows. */
void mnSaveProgressAdvance(Sint32 step)
{
	BUP_WORKSPACE *save = RamHeapSlots->bupState;
	Uint32		   row;
	const Uint8	  *src;
	Uint16		  *dst;
	Sint32		   i;

	/* A three-sector write moves the bar twice as far a sector. */
	if((Sint16)save->sectorTotal <= 3)
		step <<= 1;
	row = (Sint16)(save->sectorCnt += step);
	if(row > PROGRESS_BAR_LAST_ROW)
		row = PROGRESS_BAR_LAST_ROW;
	dst = (Uint16 *)BG_SCREEN_BLOCK(4) + PROGRESS_BAR_Y * BG_SCREEN_WIDTH +
		PROGRESS_BAR_X;
	src = WindowProgressBarRows + row * PROGRESS_BAR_WIDTH;
	for(i = 0; i < PROGRESS_BAR_WIDTH; i++)
		*dst++ = *src++ + WINDOW_TILE(PROGRESS_BAR_FIRST_TILE);
}

/* Heading x coordinate: only French differs. */
#define SAVING_X  FRENCH_OR(40, 76)
#define ERASING_X FRENCH_OR(48, 92)
#define COPYING_X FRENCH_OR(48, 84)

/* What the progress screen says it is doing. */
typedef enum SAVE_PROGRESS_KIND {
	SAVE_PROGRESS_SAVING = 0,  /* "Saving..." (MsgSaving) */
	SAVE_PROGRESS_ERASING = 1, /* "Erasing..." (MsgErasing) */
	SAVE_PROGRESS_COPYING = 2  /* "Copying..." (MsgCopying) */
} SAVE_PROGRESS_KIND;

/* Opens the progress screen for a SAVE_PROGRESS_KIND. */
WINDOW *mnSaveProgressOpen(Uint32 kind)
{
	WINDOW *win;
	void   *tiles;
	Sint32	msg;

	win = mnWindowCreate(0, 0, 30, 20, 0);
	tiles = syHeapAllocEWRAM(256);
	syDecompressTaggedLZ(WindowProgressBarTiles, tiles);
	syDMAXCopy(tiles, (void *)BG_SCREEN_BLOCK(2), DMA_COPY_WORDS(64), REG_DMA3);
	switch(kind) {
	case SAVE_PROGRESS_SAVING:
		mnTextDrawMessage((Sint32)&MsgSaving, (Sint32)win, SAVING_X, 40);
		break;
	case SAVE_PROGRESS_ERASING:
		mnTextDrawMessage((Sint32)&MsgErasing, (Sint32)win, ERASING_X, 40);
		break;
	case SAVE_PROGRESS_COPYING:
		mnTextDrawMessage((Sint32)&MsgCopying, (Sint32)win, COPYING_X, 40);
		break;
	}
	msg = (Sint32)&MsgDoNotTurnThePower;
	mnTextDrawMessage(msg, (Sint32)win, 40, 88);
	mnTextDrawMessage(msg + 1, (Sint32)win, 40, 104);
	sySndSetBGMVolumeTargetStub(0, PROGRESS_MUSIC_FADE_STEP);
	sySndPlayStub(SE_STOP_EXIT_SE);
	sySynch(10);
	syHeapRelease(tiles);
	return win;
}

/* Closes the progress screen. */
WINDOW *mnSaveProgressClose(WINDOW *win)
{
	sySynch(10);
	fdPlayQueuedMusicStub();
	sySndSetBGMVolumeTargetStub(BGM_UNITY_SCALE, PROGRESS_MUSIC_FADE_STEP);
	mnWindowClose(win, 0);
	return win;
}

/* Stops DMA channel n. */
#define DMA_STOP(n) \
	do { \
		volatile Uint16 *dma = REG_DMA0 + (n) * 6; \
		dma[5] &= (Uint16)~DMA_START_TIMING_DRQ_REPEAT; \
		dma[5] &= (Uint16)~DMA_ENABLE; \
		dma[5]; \
	} while(0)

/* Runs syBupWritePart with interrupts masked and all four DMA channels stopped,
   setting mnSaveProgressAdvance as the save's progress callback around it, then
   restores IE and IME. mnSaveDeletePartMasked does the same around
   syBupDeletePart. */
Sint32 mnSaveWritePartMasked(Sint32 partId, void *buf)
{
	volatile Uint32 ime;
	volatile Uint32 ie;
	Sint32			result;

	IRQ_LOCK(ime);
	ie = REG_IE;
	syIOWrite16(0, &REG_IE);
	DMA_STOP(0);
	DMA_STOP(1);
	DMA_STOP(2);
	DMA_STOP(3);
	IRQ_UNLOCK(ime);
	syBupSetProgressCallback(mnSaveProgressAdvance);
	result = syBupWritePart(partId, buf);
	syBupSetProgressCallback(0);
	IRQ_LOCK(ime);
	REG_IE = ie;
	IRQ_UNLOCK(ime);
	return result;
}

/* Deletes a part of a save file (syBupDeletePart) with interrupts masked and
   all four DMA channels stopped, setting mnSaveProgressAdvance as the save's
   progress callback around it, as mnSaveWritePartMasked does around
   syBupWritePart. */
Sint32 mnSaveDeletePartMasked(Sint32 partId)
{
	volatile Uint32 ime;
	volatile Uint32 ie;
	Sint32			result;

	IRQ_LOCK(ime);
	ie = REG_IE;
	syIOWrite16(0, &REG_IE);
	DMA_STOP(0);
	DMA_STOP(1);
	DMA_STOP(2);
	DMA_STOP(3);
	IRQ_UNLOCK(ime);
	syBupSetProgressCallback(mnSaveProgressAdvance);
	result = syBupDeletePart(partId);
	syBupSetProgressCallback(0);
	IRQ_LOCK(ime);
	REG_IE = ie;
	IRQ_UNLOCK(ime);
	return result;
}

/* Unused: writes ticks as "HHH:MM:SS:CC", hours to hundredths, into out and
   returns out. Its stub mnStringFormatStopwatchStub is exported, but no overlay
   imports it and nothing calls it. */
Uint8 *mnStringFormatStopwatch(Uint32 ticks, Uint8 *out)
{
	Uint8  digits[16];
	Uint8 *digitsCursor;
	Uint8 *textCursor;
	Uint32 fraction;
	Uint32 seconds;
	Uint32 minutes;
	Uint32 hours;
	Uint32 hundredths;

	fraction = ticks % TICKS_PER_SECOND;
	ticks /= TICKS_PER_SECOND;
	hundredths = fraction * 100 / TICKS_PER_SECOND;
	seconds = ticks % SECONDS_PER_MINUTE;
	ticks /= SECONDS_PER_MINUTE;
	minutes = ticks % MINUTES_PER_HOUR;
	hours = ticks / MINUTES_PER_HOUR;

	digitsCursor = mnStringFormatNum(digits, hours, 3);
	*out = *digitsCursor;
	digitsCursor++;
	textCursor = out + 1;
	*textCursor = *digitsCursor;
	textCursor++;
	*textCursor = digitsCursor[1];
	textCursor++;
	*textCursor = ':';

	digitsCursor = mnStringFormatNum(digits, WITH_LEADING_ZERO(minutes), 2);
	textCursor++;
	*textCursor = digitsCursor[0];
	textCursor++;
	*textCursor = digitsCursor[1];
	textCursor++;
	*textCursor = ':';

	digitsCursor = mnStringFormatNum(digits, WITH_LEADING_ZERO(seconds), 2);
	textCursor++;
	*textCursor = digitsCursor[0];
	textCursor++;
	*textCursor = digitsCursor[1];
	textCursor++;
	*textCursor = ':';

	digitsCursor = mnStringFormatNum(digits, WITH_LEADING_ZERO(hundredths), 2);
	textCursor++;
	*textCursor = digitsCursor[0];
	textCursor++;
	*textCursor = digitsCursor[1];
	textCursor[1] = 0;

	return out;
}

/* Writes the play time `ticks` into `out` as hours and minutes, "hhh:mm" with
   the hours right-aligned in three characters, and returns `out`. The time
   stops at 999:59. */
void *mnStringFormatPlayTime(Sint32 ticks, Uint8 *out)
{
	Uint8  digits[16];
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
	BUP_WORKSPACE *state = RamHeapSlots->bupState;
	BUP_SUMMARY	  *summary;
	Uint32		   i;

	summary = state->summary;
	for(i = 0; i < BUP_FILE_COUNT; i++, summary++) {
		if(summary->level == 0)
			return i;
	}
	return SAVE_NO_FREE_FILE;
}

/* Returns how many save files there are, leaving out those whose summary has
   its send flag set (the files saved at the end of the game) when `excludeSent`
   is nonzero, or SAVE_ERROR_NO_MEMORY when the backup memory cannot be read. It
   opens and releases the save workspace itself. */
Sint32 mnSaveCountFilesExcludingSent(Sint32 excludeSent)
{
	Sint32		   i;
	Sint32		   cnt;
	BUP_SUMMARY	  *summary;
	BUP_WORKSPACE *state;

	if(syBupInitState() != 0) {
		cnt = SAVE_ERROR_NO_MEMORY;
	} else {
		cnt = syBupLoadFileSummaries();
		if(excludeSent != 0) {
			state = RamHeapSlots->bupState;
			summary = state->summary;
			for(i = 0; i < BUP_FILE_COUNT; i++) {
				if(summary[i].sendFlg != 0)
					cnt--;
			}
		}
	}
	syBupReleaseState();
	return cnt;
}

/* Loads the save file summaries for the File Screen and returns how many files
   there are: plus SAVE_SCAN_ALL_CLEARED when every file is cleared (its send
   flag set), or SAVE_ERROR_NO_MEMORY when the backup memory cannot be read.
   Sets the title's options from them: Battle when a file has found a Djinni,
   Update when a file has a level and FLAG_REUNION clear. */
Sint32 mnSaveScanFileSummaries(void)
{
	Sint32 error;
	Sint32 sentCnt;
	Sint32 fileCnt;

	error = syBupInitState();
	sentCnt = 0;
	fileCnt = SAVE_ERROR_NO_MEMORY;
	if(error == 0) {
		Sint32		   i;
		BUP_WORKSPACE *state;

		fileCnt = syBupLoadFileSummaries();
		state = RamHeapSlots->bupState;
		AnyFileCleared = 0;
		TitleBattleOptionEnabled = 0;
		TitleUpdateOptionEnabled = 0;
		for(i = 0; i < BUP_FILE_COUNT; i++) {
			if(state->summary[i].sendFlg != 0) {
				AnyFileCleared = 1;
				sentCnt++;
			}
			if(state->summary[i].djinnCnt != 0)
				TitleBattleOptionEnabled = 1;
			if(state->summary[i].level != 0 && state->summary[i].reunited == 0)
				TitleUpdateOptionEnabled = 1;
		}
	}
	syBupReleaseState();
	if(fileCnt != 0 && sentCnt == fileCnt)
		return fileCnt + SAVE_SCAN_ALL_CLEARED;
	return fileCnt;
}

/* Prepares the save buffer for writing: stamps the main routine's state with
   the time and options, then fills the file summary shown on the file menus
   (the leader's name, level and class, the play time, area, coins, Djinn of
   each element, the party, the send flag and how many Djinn it has found) and
   its checksum, the sum of the 968 words from GameFlagBytes on. */
Sint32 mnSaveBuildFileSummary(void)
{
	Uint16		 members[14];
	BUP_SUMMARY *summary;
	UNIT		*leader;
	Uint8		*src;
	Uint8		*dst;
	Uint32		*word;
	Sint32		 i;
	Uint32		 sum = 0;

	/* The summary's header comes before the save buffer. */
	summary = (BUP_SUMMARY *)(BupBuffer - offsetof(BUP_SUMMARY, name));
	MainRoutineStateWork.buildStamp = gmBuildStampGetTimeStub(0);
	MainRoutineStateWork.playTime = PlayTimeTicks;
	/* A second copy of the play time, at the start of SceneLocalState. */
	*(Uint32 *)SceneLocalState = PlayTimeTicks;
	MainRoutineStateWork.idleSleepEnabled = IdleSleepEnabled;
	leader = syUnitGet(MainRoutineStateWork.leaderActorId);
	dst = summary->name;
	src = leader->name;
	for(i = 0; i < (Sint32)sizeof(summary->name); i++)
		*dst++ = *src++;
	summary->level = leader->level;
	summary->playTime = MainRoutineStateWork.playTime;
	summary->area = fdAreaGetNameIndexStub(
		MainRoutineStateWork.next, MainRoutineStateWork.entrance);
	summary->classId = leader->classId;
	summary->coins = MainRoutineStateWork.coins;
	summary->djinn[0] = gmPartySumDjinnCountsStub(0);
	summary->djinn[1] = gmPartySumDjinnCountsStub(1);
	summary->djinn[2] = gmPartySumDjinnCountsStub(2);
	summary->djinn[3] = gmPartySumDjinnCountsStub(3);
	/* Up to four party members, ended by -1. */
	gmPartyListMembersStub((Sint16 *)members);
	for(i = 0; i <= 3 && members[i] != BATTLE_UNIT_LIST_END; i++)
		summary->party[i] = members[i];
	summary->party[i] = -1;
	summary->winHue = MainRoutineStateWork.winHue;
	summary->winBrightness = MainRoutineStateWork.winBrightness;
	summary->sendFlg = MainRoutineStateWork.sendFlg;
	summary->djinnCnt = 0;
	for(i = FLAG_DJINN_FIRST; i <= FLAG_DJINNI(DJINNI_WIND19); i++) {
		if(syGameFlagGet(i))
			summary->djinnCnt++;
	}
	summary->reunited = syGameFlagGet(FLAG_REUNION) != 0;
	{
		Sint32 n;
		Uint32 ticks;

		ticks = MainRoutineStateWork.buildStamp;
		word = (Uint32 *)GameFlagBytes;
		/* The checksum's words, counted in fours and multiplied back below. */
		n = BUP_CHECKSUM_WORDS / 4;
		i = 0;
		n <<= 2;
		syIOWrite32(ticks, &summary->buildStamp);
		goto test;
		do {
			sum += *word++;
			i++;
		test:;
		} while(i < n);
	}
	summary->checksum = sum;
	return 0;
}

/* Writes both halves of the save buffer, as they are, to save file `file`.
   Returns 0, or a SAVE_ERROR_* after showing its message. */
Sint32 mnSaveWriteFile(Sint32 file)
{
	Sint32 result = 0;

	if(syBupInitState() != 0) {
		mnFieldMessageShow((Sint32)&MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
		result = SAVE_ERROR_NO_MEMORY;
	} else if(mnSaveWritePartMasked(file, BupBuffer) != 0) {
		mnFieldMessageShow((Sint32)&MsgSaveFailed, FIELD_MESSAGE_ZOOM);
		result = SAVE_ERROR_WRITE;
	}
	syBupReleaseState();
	return result;
}

/* Writes the link lobby's tallies (MainRoutineStateWork.linkTallies) into the
   current file's first part (BupSlot), under the saving screen, leaving the
   rest of the part as it was saved. Returns -1 when no file is current. */
Sint32 mnSaveWriteLinkTallies(void)
{
	void  *part;
	Sint32 value;
	Sint32 result;
	Sint32 error;
	Sint32 progress;

	result = 0;
	part = syHeapAllocEWRAM(BUP_SECTORS_PER_PART * BUP_SECTOR_SIZE);
	value = BupSlot;
	if(value != -1) {
		error = syBupInitState();
		if(error != 0) {
			mnFieldMessageShow((Sint32)&MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
			result = SAVE_ERROR_NO_MEMORY;
		} else {
			Uint8 *tallies;

			error = syBupReadPart(BupSlot, part);
			/* A failed read is reported, but the part is still written below
			   from whatever the buffer holds. */
			if(error != 0) {
				mnFieldMessageShow((Sint32)&MsgSaveFailed, FIELD_MESSAGE_ZOOM);
				result = SAVE_ERROR_READ;
			}
			/* The tallies' place in the part is their place in the save
			   buffer. */
			tallies = (Uint8 *)part + (Sint32)MainRoutineStateWork.linkTallies;
			tallies -= (Sint32)BupBuffer;
			syWordsCopyEntry(tallies, MainRoutineStateWork.linkTallies,
				sizeof(MainRoutineStateWork.linkTallies));
			progress = (Sint32)mnSaveProgressOpen(SAVE_PROGRESS_SAVING);
			error = mnSaveWritePartMasked(BupSlot, part);
			mnSaveProgressClose((WINDOW *)progress);
			if(error != 0) {
				mnFieldMessageShow((Sint32)&MsgSaveFailed, FIELD_MESSAGE_ZOOM);
				result = SAVE_ERROR_WRITE;
			}
		}
		syBupReleaseState();
		syHeapRelease(part);
		value = result;
	}
	return value;
}

/* Saves the game to slot (at most 2). */
Sint32 mnSaveToFile(Uint32 slot)
{
	Sint32 result = 0;
	Sint32 error;

	if(slot > 2)
		slot = 2;
	if(syBupInitState() != 0) {
		mnFieldMessageShow((Sint32)&MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
		result = SAVE_ERROR_NO_MEMORY;
	} else {
		syBupLoadFileSummaries();
		BupSlot = slot;
		sySndPlayStub(MUS_SAVED);
		mnFieldMessageShow((Sint32)&MsgSaving,
			FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN | FIELD_MESSAGE_LOWER);
		mnSaveBuildFileSummary();
		fdActorTableSnapshotStub();
		error = mnSaveWritePartMasked(slot, BupBuffer);
		while(mnMessageAllTyped() == 0)
			sySynch(1);
		mnMessageCloseFinishedWindows();
		if(error != 0) {
			mnFieldMessageShow((Sint32)&MsgSaveFailed, FIELD_MESSAGE_ZOOM);
			result = SAVE_ERROR_WRITE;
		} else {
			mnFieldMessageShow((Sint32)&MsgGameSaved,
				FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_LOWER);
		}
	}
	syBupReleaseState();
	return result;
}

/* Lets the player choose a file and saves the game to it, asking first before
   it writes over a file in use. */
Sint32 mnSaveToChosenFile(void)
{
	Sint32		   result = 0;
	Sint32		   slot;
	Sint32		   error;
	Sint32		   progress;
	BUP_WORKSPACE *state;

	if(syBupInitState() != 0) {
		mnFieldMessageShow((Sint32)&MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
		result = SAVE_ERROR_NO_MEMORY;
	} else {
		syBupLoadFileSummaries();
		slot = mnFileListSelectFile(BupSlot, FILE_LIST_SAVE);
		if(slot == -1) {
			result = slot;
		} else {
			state = RamHeapSlots->bupState;
			if(state->summary[slot].level != 0) {
				mnFieldMessageShow((Sint32)&MsgOverwriteConfirm,
					FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN |
						FIELD_MESSAGE_LOWER);
				while(mnMessageAllTyped() == 0)
					sySynch(1);
				if(mnYesNoMenuRun(1, 0, 0, NO) != YES) {
					mnMessageCloseFinishedWindows();
					goto done;
				}
				mnMessageCloseFinishedWindows();
			}
			BupSlot = slot;
			progress = (Sint32)mnSaveProgressOpen(SAVE_PROGRESS_SAVING);
			mnSaveBuildFileSummary();
			fdActorTableSnapshotStub();
			error = mnSaveWritePartMasked(slot, BupBuffer);
			mnSaveProgressClose((WINDOW *)progress);
			mnMessageCloseFinishedWindows();
			if(error != 0) {
				mnFieldMessageShow((Sint32)&MsgSaveFailed, FIELD_MESSAGE_ZOOM);
				result = SAVE_ERROR_WRITE;
			} else {
				mnFieldMessageShow((Sint32)&MsgGameSaved,
					FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_LOWER);
			}
		}
	}
done:
	syBupReleaseState();
	return result;
}

/* Selects a file and loads it. */
Sint32 mnSaveChooseAndLoadFile(Sint32 mode)
{
	Sint32 result = 0;
	Sint32 file;

	if(syBupInitState() != 0) {
		mnFieldMessageShow((Sint32)&MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
		result = SAVE_ERROR_NO_MEMORY;
	} else {
		syBupLoadFileSummaries();
		file = mnFileListSelectFile(BupSlot, mode);
		if(file == -1) {
			result = file;
		} else if(syBupReadPart(file, BupBuffer) != 0) {
			mnFieldMessageShow((Sint32)&MsgLoadFailed, FIELD_MESSAGE_ZOOM);
			result = SAVE_ERROR_READ;
		} else {
			PlayTimeTicks = MainRoutineStateWork.playTime;
			IdleSleepEnabled = MainRoutineStateWork.idleSleepEnabled;
			IdleTicks = 0;
			BupSlot = file;
		}
	}
	syBupReleaseState();
	return result;
}

/* Selects a file and copies it to a free file. Its failures say "Failed to
   erase." (MsgEraseFailed), as erasing's do. */
Sint32 mnSaveCopyFile(void)
{
	Sint32 result = 0;
	Sint32 value;
	Sint32 progress;

	if(syBupInitState() != 0) {
		mnFieldMessageShow((Sint32)&MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
		result = SAVE_ERROR_NO_MEMORY;
	} else {
		syBupLoadFileSummaries();
		value = mnFileListSelectFile(0, FILE_LIST_COPY);
		if(value == -1) {
			result = value;
		} else {
			void *buf = BupBuffer;

			value = syBupReadPart(value, buf);
			if(value != 0) {
				mnFieldMessageShow((Sint32)&MsgLoadFailed, FIELD_MESSAGE_ZOOM);
				result = SAVE_ERROR_READ;
			} else {
				value = mnSaveFindFreeFile();
				if(value == SAVE_NO_FREE_FILE) {
					mnFieldMessageShow(
						(Sint32)&MsgEraseFailed, FIELD_MESSAGE_ZOOM);
					result = SAVE_ERROR_NO_FILE;
				} else {
					progress =
						(Sint32)mnSaveProgressOpen(SAVE_PROGRESS_COPYING);
					value = mnSaveWritePartMasked(value, buf);
					mnSaveProgressClose((WINDOW *)progress);
					if(value != 0) {
						mnFieldMessageShow(
							(Sint32)&MsgEraseFailed, FIELD_MESSAGE_ZOOM);
						result = SAVE_ERROR_WRITE;
					} else {
						mnFieldMessageShow(
							(Sint32)&MsgFileCopied, FIELD_MESSAGE_ZOOM);
					}
				}
			}
		}
	}
	syBupReleaseState();
	return result;
}

#if (LANGUAGE == LANGUAGE_FRENCH)
/* The French confirmation window is farther left. */
#define ERASE_CONFIRM_X 4
#else
#define ERASE_CONFIRM_X 8
#endif

/* Selects a file and erases it. */
Sint32 mnSaveEraseSelectedFile(void)
{
	Sint32 result = 0;
	Sint32 value;
	Sint32 progress;
	Sint32 error;

	if(syBupInitState() != 0) {
		mnFieldMessageShow((Sint32)&MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
		result = SAVE_ERROR_NO_MEMORY;
	} else {
		syBupLoadFileSummaries();
		value = mnFileListSelectFile(0, FILE_LIST_ERASE);
		if(value == -1) {
			result = value;
		} else {
			mnMessageOpenWindow(
				(Sint32)&MsgEraseConfirm, ERASE_CONFIRM_X, 1, 2);
			while(mnMessageAllTyped() == 0)
				sySynch(1);
			if(mnYesNoMenuRun(1, 0, 3, NO) != YES) {
				mnMessageCloseFinishedWindows();
			} else {
				mnMessageCloseFinishedWindows();
				progress = (Sint32)mnSaveProgressOpen(SAVE_PROGRESS_ERASING);
				error = mnSaveDeletePartMasked(value);
				mnSaveProgressClose((WINDOW *)progress);
				if(error != 0) {
					mnFieldMessageShow(
						(Sint32)&MsgEraseFailed, FIELD_MESSAGE_ZOOM);
					result = SAVE_ERROR_ERASE;
				} else {
					mnFieldMessageShow(
						(Sint32)&MsgFileErased, FIELD_MESSAGE_ZOOM);
				}
			}
		}
	}
	syBupReleaseState();
	return result;
}

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
	palette = (volatile Uint16 *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 8);
	for(i = 0; i <= 3; i++) {
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
			syIOWrite16(RGB(red, green, colour), palette++);
		}
	}
}

/* Starts the palette glow (mnFileListUpdatePulse) as an event. */
void mnFileListStartPulse(void)
{
	sySetEvent((Sint32)mnFileListUpdatePulse, EVENT_PASS_TICK);
}

/* Stops the palette glow. */
void mnFileListStopPulse(void)
{
	syCloseEvent((Uint32)((Sint32)mnFileListUpdatePulse));
}

/* The window tile the file list's chosen row is filled with, drawn in
   HIGHLIGHT_PALETTE, whose colours mnFileListUpdatePulse makes glow. */
#define FILE_LIST_HIGHLIGHT_TILE 6

/* Fills the `width` x `height` tile rectangle at (x, y) inside `win`,
   clipped to the screen as mnWindowSetTextTileBitRect clips, with
   FILE_LIST_HIGHLIGHT_TILE: the file list's glowing bar under the chosen
   file. */
void mnFileListFillHighlight(
	WINDOW *win, Sint32 x, Sint32 y, Sint32 width, Sint32 height)
{
	WIN_STATE *state = (WIN_STATE *)RamHeapSlots->winState;
	Sint32	   sum;

	sum = x + win->x;
	x = sum + 1;
	sum = y + win->y;
	y = sum + 1;

	if(x < 0) {
		width += x;
		x = 0;
	}
	if(x + width > (DISPLAY_TILE_WIDTH - 1))
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
		Sint32 ofs = yOfs + ((Sint32)state + xOfs);
		do {
			Sint32	remaining = width;
			Uint16 *cell = (Uint16 *)(ofs + offsetof(WIN_STATE, tilemap));
			if(remaining != 0) {
				do {
					remaining--;
					*cell++ = BG_TILE_PALETTE(HIGHLIGHT_PALETTE) |
						FILE_LIST_HIGHLIGHT_TILE;
				} while(remaining != 0);
			}
			height--;
			ofs += BG_SCREEN_WIDTH * 2;
		} while(height != 0);
		state->dirty = 1;
	}
}

/* Shows the party of save file `summary` (up to four party members) as sprites
   in a row from tile (x, y) inside `win`, and starts placing them every
   tick (mnFileListPlacePartySprs). */
void mnFileListShowPartySprs(
	WINDOW *win, Sint32 x, Sint32 y, BUP_SUMMARY *summary)
{
	FILE_LIST_STATE *menu = RamHeapSlots->menuState;
	Sint32			 i;

	for(i = 0; i < 4 && summary->party[i] != -1; i++) {
		FIELD_SPR *spr = (FIELD_SPR *)fdSprCreateStub(summary->party[i]);

		if(spr != 0) {
			fdSprStartAnimationStub(spr, ANIM_STAND);
			spr->flg = 0;
			spr->pri = 0;
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
	FILE_LIST_STATE *menu = RamHeapSlots->menuState;
	Sint32			 i;

	syCloseEvent((Sint32)mnFileListPlacePartySprs);
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
	FILE_LIST_STATE *state = (FILE_LIST_STATE *)RamHeapSlots->menuState;
	Sint32			 scale[2];
	FIXED			 pos[4];
	Sint32			 idx = 0;
	Sint32			 spr;

	while(1) {
		spr = (Sint32)state->partyMemberIcons[idx];
		if(spr != 0) {
			scale[0] = state->partyMemberScale[idx];
			scale[1] = state->partyMemberScale[idx];
			pos[0] = toFIXED(state->partyMemberX[idx]);
			pos[1] = toFIXED(FILE_LIST_SPRITE_LIFT);
			pos[2] = toFIXED(state->partyMemberY[idx]) +
				toFIXED(FILE_LIST_SPRITE_LIFT);
			pos[3] = 0;
			fdRenderApplyProjectedPlacementStub(
				(SPR *)spr, pos, scale, FACING_SOUTH);
		}
		idx += 1;
		if(idx > 3) {
			break;
		}
	}
}

/* Draws a saved game's Djinn of each element, four two-digit numbers 24 pixels
   apart at y = 16, into win; nothing when win is null. */
void mnFileListDrawDjinnCounts(WINDOW *win, BUP_SUMMARY *summary)
{
	Sint32 i;

	if(win != 0) {
		for(i = 0; i < 4; i++)
			mnTextDrawNumAtOffset(summary->djinn[i], 2, win,
				i * FILE_LIST_SPRITE_SPACING * 8, 16);
	}
}

/* Edition-specific x positions of the level and coin labels, in pixels. */
#if (LANGUAGE == LANGUAGE_SPANISH)
#define SUMMARY_LEVEL_X 64
#define SUMMARY_COINS_X 56
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define SUMMARY_LEVEL_X 59
#define SUMMARY_COINS_X 56
#elif (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_ITALIAN)
#define SUMMARY_COINS_X 56
#else
#define SUMMARY_COINS_X 48
#endif

/* Draws a file's summary into window, when there is one: the leader's name,
   level and class, the play time and the coins. */
void mnFileListDrawSummary(WINDOW *win, BUP_SUMMARY *summary)
{
	Uint32 playTime[4]; /* the play time text */
	Sint32 y;

	if(win != 0) {
		mnWindowSprPrepareForRedraw(win);
		mnWindowDrawDividerLine(win, 0, 4, 13, 4);
		mnTextDrawStringAtOffset(summary->name, win, 0, 0);
		y = 0;
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH)
		mnTextDrawStringAtOffset(
			(Uint8 *)StatusMenuLevelLetterString, win, SUMMARY_LEVEL_X, 0);
#else
		mnTextDrawStringInWindow(
			(Uint8 *)StatusMenuLevelLetterString, (Sint32)win, 72, 0);
#endif
		mnTextDrawNumAtOffset(summary->level, 2, win, 80, y);
		mnTextDrawMessageAsTiles(
			summary->classId + (Sint32)MsgClassName, win, 0, 16);
#if (LANGUAGE == LANGUAGE_JAPANESE)
		y = 32;
		mnTextDrawNumInWindow(summary->coins, 6, (Sint32)win, 24, y);
		mnTextDrawMessageAsTiles((Sint32)MsgCoins, win, 72, 32);
		mnTextDrawMessageAsTiles((Sint32)MsgStatusLabel, win, 0, 48);
		mnTextDrawStringInWindow(
			mnStringFormatPlayTime(summary->playTime, (Uint8 *)playTime),
			(Sint32)win, 48, 48);
#else
		mnTextDrawMessageAsTiles((Sint32)MsgStatusLabel, win, 0, 32);
		mnTextDrawStringInWindow(
			mnStringFormatPlayTime(summary->playTime, (Uint8 *)playTime),
			(Sint32)win, 48, 40);
		y = 48;
		mnTextDrawNumInWindow(summary->coins, 6, (Sint32)win, 0, y);
#if (LANGUAGE == LANGUAGE_FRENCH)
		/* French takes the singular "Pièce" for one coin or none. */
		if(summary->coins > 1)
			mnTextDrawMessageAsTiles(
				(Sint32)MsgCoins, win, SUMMARY_COINS_X, 48);
		else
			mnTextDrawMessageAsTiles(
				(Sint32)MsgPiece, win, SUMMARY_COINS_X, 48);
#else
		mnTextDrawMessageAsTiles((Sint32)MsgCoins, win, SUMMARY_COINS_X, 48);
#endif
#endif
	}
}

/* Where the leader's name and the area name start in a file's row of the file
   list, in pixels: the European languages start both farther left. */
#define FILE_LIST_LEADER_X BY_LANGUAGE(12, 12, 6, 6, 6, 6)
#define FILE_LIST_AREA_X   BY_LANGUAGE(72, 72, 62, 62, 62, 62)

/* mnFileListSelectFile's result when no file can be chosen in its mode. */
#define SAVE_MENU_NO_FILE -2

/* The file list: lists the three files with each one's leader and area (or why
   it cannot be chosen) and, for the file under the cursor, its summary, its
   party and its Djinn, with the file's own palette glow. Returns the file
   chosen in mode `menuMode` (FILE_MENU_MODE), -1 when cancelled. */
Sint32 mnFileListSelectFile(Sint32 selectedFile, Sint32 menuMode)
{
	WINDOW		  *fileList;
	BUP_WORKSPACE *save;
	WINDOW		  *summaryWin;
	WINDOW		  *partyWin;
	WINDOW		  *djinnWin;
	BUP_SUMMARY	  *summary;
	void		  *modePicture;
	Bool		   summaryDirty;
	Uint32		   buildStamp;
	void		  *sprState;
	Sint32		   result;
	Sint32		   i;
	Sint32		   y;

	sprState = syHeapSlotAlloc(HEAP_SLOT_MENU, MENU_STATE_SIZE);
	save = RamHeapSlots->bupState;
	summaryWin = NULL;
	partyWin = NULL;
	djinnWin = NULL;
	summaryDirty = TRUE;
	buildStamp = gmBuildStampGetTimeStub();
	if(selectedFile < 0)
		selectedFile = 0;
	if(menuMode == FILE_LIST_CONTINUE) {
		for(i = 0; i < BUP_FILE_COUNT &&
			(save->summary[selectedFile].level == 0 ||
				save->summary[selectedFile].sendFlg != 0);
			i++) {
			selectedFile++;
			if(selectedFile > 2)
				selectedFile = 0;
		}
		if(i == BUP_FILE_COUNT) {
			result = SAVE_MENU_NO_FILE;
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
			result = SAVE_MENU_NO_FILE;
			goto done;
		}
	} else if(menuMode == FILE_LIST_CLEARED) {
		for(i = 0; i < BUP_FILE_COUNT &&
			(save->summary[selectedFile].level == 0 ||
				save->summary[selectedFile].sendFlg == 0);
			i++) {
			selectedFile++;
			if(selectedFile > 2)
				selectedFile = 0;
		}
		if(i == BUP_FILE_COUNT) {
			result = SAVE_MENU_NO_FILE;
			goto done;
		}
	} else if(menuMode == FILE_LIST_UPDATE) {
		for(i = 0; i < BUP_FILE_COUNT &&
			(save->summary[selectedFile].level == 0 ||
				save->summary[selectedFile].reunited != 0);
			i++) {
			selectedFile++;
			if(selectedFile > 2)
				selectedFile = 0;
		}
		if(i == BUP_FILE_COUNT) {
			result = SAVE_MENU_NO_FILE;
			goto done;
		}
	} else if(menuMode != FILE_LIST_SAVE) {
		for(i = 0; i < BUP_FILE_COUNT && save->summary[selectedFile].level == 0;
			i++) {
			selectedFile++;
			if(selectedFile > 2)
				selectedFile = 0;
		}
		if(i == BUP_FILE_COUNT) {
			result = SAVE_MENU_NO_FILE;
			goto done;
		}
	}

	DMA_FILL32(0, sprState, MENU_STATE_SIZE);
	mnFileListStartPulse();
	fileList = mnWindowCreate(1, 2, 28, 7, WINDOW_FLAG_INSTANT);
	for(i = 0, summary = save->summary; i < BUP_FILE_COUNT; i++, summary++) {
		if(save->summary[i].level == 0) {
			mnTextDrawMessage(
				(Sint32)MsgSaveNoData, (Sint32)fileList, 10, i * 16);
		} else if(save->summary[i].buildStamp < buildStamp) {
			mnTextDrawMessage(
				(Sint32)MsgSaveContinue, (Sint32)fileList, 10, i * 16);
		} else if(save->summary[i].playTime !=
			save->summary[i].secondPartPlayTime) {
			mnTextDrawMessage(
				(Sint32)MsgSaveCorrupted, (Sint32)fileList, 10, i * 16);
		} else if(menuMode == FILE_LIST_CLEARED &&
			save->summary[i].sendFlg == 0) {
			mnTextDrawMessage(
				(Sint32)MsgSaveNotCleared, (Sint32)fileList, 10, i * 16);
		} else if(menuMode == FILE_LIST_UPDATE &&
			save->summary[i].reunited != 0) {
			mnTextDrawMessage(
				(Sint32)MsgSavePostReunion, (Sint32)fileList, 10, i * 16);
		} else {
			y = i * 16;
			mnTextDrawString(
				summary->name, (Sint32)fileList, FILE_LIST_LEADER_X, y);
			mnTextDrawMessage(save->summary[i].area + (Sint32)MsgDebugAreaName,
				(Sint32)fileList, FILE_LIST_AREA_X, y);
		}
	}
	modePicture = mnTitleCreate(menuMode, (WINDOW *)fileList, 72, -24);
	mnDjinnFiguresInitTopRowStub();
	for(i = 0, y = 144; i < 4; i++, y += 24)
		mnDjinnFiguresSetSlotPosStub(i, y, 128);
	mnDjinnFiguresSetShownStub(0);
	sySynch(1);

	while(1) {
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
					(WINDOW *)partyWin, 0, 0, &save->summary[selectedFile]);
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
						(WINDOW *)djinnWin, &save->summary[selectedFile]);
					mnDjinnFiguresSetShownStub(1);
				} else {
					mnDjinnFiguresSetShownStub(0);
					mnWindowClose(djinnWin, 2);
					djinnWin = NULL;
				}
			} else {
				mnWindowColApply(MainRoutineStateWork.winHue,
					MainRoutineStateWork.winBrightness);
				mnDjinnFiguresSetShownStub(0);
				mnFileListHidePartySprs();
				mnWindowClose(djinnWin, 2);
				mnWindowClose(partyWin, 2);
				mnWindowClose(summaryWin, 2);
				djinnWin = NULL;
				partyWin = NULL;
				summaryWin = NULL;
			}
			mnWindowSprRedrawSavedRect(fileList);
			mnWindowDrawDividerLine(fileList, 0, 2, 27, 2);
			mnWindowDrawDividerLine(fileList, 0, 4, 27, 4);
			mnFileListFillHighlight(fileList, 0, selectedFile * 2, 26, 1);
		}
		mnTitleBob(modePicture);
		sySynch(1);
		if(PerWork.repeat & KEY_UP) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			summaryDirty = TRUE;
			do {
				selectedFile =
					(selectedFile + BUP_FILE_COUNT - 1) % BUP_FILE_COUNT;
				if(menuMode == FILE_LIST_SAVE)
					break;
			} while(save->summary[selectedFile].level == 0 ||
				(menuMode == FILE_LIST_CONTINUE &&
					save->summary[selectedFile].sendFlg != 0) ||
				(menuMode == FILE_LIST_BATTLE &&
					save->summary[selectedFile].djinnCnt == 0) ||
				(menuMode == FILE_LIST_CLEARED &&
					save->summary[selectedFile].sendFlg == 0) ||
				(menuMode == FILE_LIST_UPDATE &&
					save->summary[selectedFile].reunited != 0));
		} else if(PerWork.repeat & KEY_DOWN) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			summaryDirty = TRUE;
			do {
				selectedFile =
					(selectedFile + BUP_FILE_COUNT + 1) % BUP_FILE_COUNT;
				if(menuMode == FILE_LIST_SAVE)
					break;
			} while(save->summary[selectedFile].level == 0 ||
				(menuMode == FILE_LIST_CONTINUE &&
					save->summary[selectedFile].sendFlg != 0) ||
				(menuMode == FILE_LIST_BATTLE &&
					save->summary[selectedFile].djinnCnt == 0) ||
				(menuMode == FILE_LIST_CLEARED &&
					save->summary[selectedFile].sendFlg == 0) ||
				(menuMode == FILE_LIST_UPDATE &&
					save->summary[selectedFile].reunited != 0));
		} else if(PerWork.push & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		} else if(PerWork.push & KEY_A) {
			sySndPlayStub(SE_MENU_CONFIRM);
			result = selectedFile;
			break;
		}
	}
	mnDjinnFiguresReleaseTopRowStub();
	mnFileListHidePartySprs();
	mnWindowClose(djinnWin, 2);
	mnWindowClose(partyWin, 2);
	mnWindowClose(summaryWin, 2);
	mnWindowClose(fileList, 2);
	mnFileListStopPulse();
	syHeapSlotRelease(HEAP_SLOT_MENU);
	mnWindowColApply(
		MainRoutineStateWork.winHue, MainRoutineStateWork.winBrightness);
	sySynch(1);
done:
	return result;
}

#undef SAVING_X
#undef ERASING_X
#undef COPYING_X
#undef DMA_STOP
#undef ERASE_CONFIRM_X
#undef SUMMARY_LEVEL_X
#undef SUMMARY_COINS_X
#undef FILE_LIST_LEADER_X
#undef FILE_LIST_AREA_X
#undef SAVE_MENU_NO_FILE
#undef FILE_LIST_HIGHLIGHT_TILE
#undef BUP_CHECKSUM_WORDS
#undef FILE_LIST_GLOW_RED
#undef FILE_LIST_GLOW_GREEN
#undef FILE_LIST_GLOW_BLUE
