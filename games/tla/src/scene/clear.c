/* The File Screen overlay (SceneIdClear), the first thing the player sees
   after the title: the title logo, the file menu (new game, continue, copy,
   erase, the Battle Arena and Update) and what a new game asks. A new game may
   bring The Broken Seal's data over, by password or link cable
   (scClearEnterPassword, scClearReceiveByLink), applied to Isaac's party
   (scClearApplyImportedData), and two secret button codes on the screen
   (scClearCheckButtonCodes) name more of the party. */

/* This unit declares its own form of mnPointingHandLoadIntoSlot, below. */
#define OWN_mnPointingHandLoadIntoSlot
/* This unit declares its own form of mnNameCaretLoadIntoSlot, below. */
#define OWN_mnNameCaretLoadIntoSlot

#include "game/game.h"
#include "menu/menu.h"
#include "field/script_inline.h"
#include "field/field_mode_script.h"
#include "scene/scene.h"
#include "scene/title.h"
#include "window/screens.h"
#include "window/window.h"
#include "edition.h"
#include "field/call.h"
#include "macros.h"
#include "item_ids.h"
#include "field/field_mode.h"
#include "graphics/text_font.h"
#include "shop/shop.h"

/* The two secret button codes: unlocked, held and progress of each. */
extern Sint16 ClearCodeUnlocked;

extern Uint16 ClearPasswordStackItems[];

/* The step of the logo's fade in (scClearFadeInBlend), then the second code's
   unlock and both codes' progress and held keys. */
extern Uint16 ClearBlendTick[];
extern Sint16 ClearExtraCodeUnlocked;
extern Sint16 ClearCodeProgress;
extern Sint16 ClearExtraCodeProgress;
extern Sint16 ClearCodeHeld;
extern Sint16 ClearExtraCodeHeld;
extern Uint16 ClearButtonCode[];
extern Uint16 ClearExtraButtonCode[];

Sint32 scClearReceiveByLink(void);

Sint32 scClearEnterPassword(void);

extern Sint8 ClearCursorBob[];
void		 scClearApplyImportedData(Sint32 mode, Uint8 *data);
/* Decodes length symbols of an entered password (src) into dst, undoing The
   Broken Seal's FileScreenEncodePassword: each symbol less its place, the
   checksum symbols skipped, six bits a symbol, then every byte but the last
   XORed with the last. Returns the bytes written. */
Sint32		  scClearDecodePassword(Uint8 *src, Sint32 length, Uint8 *dst);
extern Uint16 ClearImportedFlags[];

struct WINDOW;

extern Uint8 ClearFirstPasswordRow[];
extern Uint8 ClearSecondPasswordRow[];
extern Uint8 ClearThirdPasswordRow[];

/* The password screen's grid of kana, a 24 by 13 window tilemap (tilemap.s). */
extern Uint8 ClearPasswordGrid[];

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ActorPlacements;
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

/* Loads the title logo onto BG2 as scTitleLoadBackground (title.c) does, and
   stops the leader's motion. */
void scClearLoadBackground(void)
{
	Uint8	  *resource;
	Sint32	   resourceId;
	Uint16	  *tilemap;
	Uint32	   x;
	Uint32	   y;
	Uint16	   tile;
	BG_SCROLL *scroll;
	Sint32	   blankTile;

	resourceId = RESOURCE_LOGO_ON_BLACK;
	syScreenFadeToBlack(0);
	REG_BG2CNT = LOGO_BGCNT;
	BgScrollWork[2].y = 0;
	blankTile = LOGO_BLANK_TILE;
	resource = syResourceGetTableEntry(resourceId);
	syDMAXCopy(resource, (void *)BG_PLTT,
		DMA_COPY_WORDS(LOGO_PALETTE_COLORS / 2), REG_DMA3);
	resource += LOGO_PALETTE_COLORS * 2;
	syFieldDecompressTaggedLZ(resource, RamMapMetatileBuffer);
	/* A screen's worth of 8-bit tiles. */
	syDMAXCopy((void *)RamMapMetatileBuffer, LOGO_TILES,
		DMA_COPY_WORDS(DISPLAY_WIDTH * DISPLAY_HEIGHT / 4), REG_DMA3);

	tilemap = LOGO_MAP;
	tile = LOGO_FIRST_TILE;
	for(y = 0; y < DISPLAY_TILE_HEIGHT; y++) {
		for(x = 0; x < DISPLAY_TILE_WIDTH; x++)
			*tilemap++ = tile++;
		*tilemap++ = blankTile;
		*tilemap++ = blankTile;
	}
	scroll = BgScrollWork;
	for(y = 0; y <= 3; y++) {
		scroll->y = 0;
		scroll->x = 0;
		scroll++;
	}
	syDMAXCopy(BgScrollWork, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
	((MAP_STATE *)RamHeapSlots->mapGrid)->dispcnt =
		DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
	fdActorGet(MainRoutineStateWork.leaderActorId)->motionFlg = 0;
}

/* The event that fades BG0 and the sprites in over the logo, a step of the
   blend every second tick. */
void scClearFadeInBlend(void)
{
	Sint32 frm;
	Uint32 level;

	frm = ClearBlendTick[0] + 1;
	ClearBlendTick[0] = frm;
	level = (Uint16)frm >> 1;
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDCNT,
		BLDCNT_TGT1_BG0 | BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_BLEND |
			BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 |
			BLDCNT_TGT2_BD,
		IO_WRITE_HALFWORD);
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDALPHA,
		(BLEND_WEIGHT_MAX - (Uint16)level) << 8 | (Uint16)level,
		IO_WRITE_HALFWORD);
	if(level >= BLEND_WEIGHT_MAX)
		syFieldCloseEvent(scClearFadeInBlend);
}

/* Follows the two secret button codes: each held key that matches the next key
   of a code advances it, a wrong key starts it again, and finishing a code
   unlocks it with a sound. */
void scClearCheckButtonCodes(void)
{
	if(ClearCodeUnlocked == 0) {
		if(ClearCodeHeld != 0) {
			if(PerWork.data == 0)
				ClearCodeHeld = 0;
		} else if(PerWork.data != 0) {
			if(PerWork.data == ClearButtonCode[ClearCodeProgress]) {
				ClearCodeProgress++;
				ClearCodeHeld = 1;
				if(ClearButtonCode[ClearCodeProgress] == 0) {
					ClearCodeUnlocked = 1;
					fdScriptPlaySnd(SE_CHIME);
				}
			} else {
				ClearCodeProgress = 0;
			}
		}
	}
	if(ClearExtraCodeUnlocked == 0) {
		if(ClearExtraCodeHeld != 0) {
			if(PerWork.data == 0)
				ClearExtraCodeHeld = 0;
		} else if(PerWork.data != 0) {
			if(PerWork.data == ClearExtraButtonCode[ClearExtraCodeProgress]) {
				ClearExtraCodeProgress++;
				ClearExtraCodeHeld = 1;
				if(ClearExtraButtonCode[ClearExtraCodeProgress] == 0) {
					ClearExtraCodeUnlocked = 1;
					fdScriptPlaySnd(SE_CHIME);
				}
			} else {
				ClearExtraCodeProgress = 0;
			}
		}
	}
}

/* Nonzero (-1) when the save was made where Retreat is blocked, unless in a
   town or on the world map: such a save cannot restart at the Sanctum, by L and
   Start held as it loads or as a corrupted save is recovered. */
Sint32 scClearIsRetreatBlocked(void)
{
	MAIN_ROUTINE_STATE *party;

	if(fdScriptGetFlag(FLAG_RETREAT_BLOCKED) == 0)
		return 0;
	party = &MainRoutineStateWork;

	if(party->sceneType == SCENE_TYPE_TOWN)
		return 0;
	if(party->next == SCENE_WORLD_MAP)
		return 0;
	return -1;
}

#include "text/msg_ids.h"

/* The play time the save kept in the scene's own state, which a sound save
   matches. */
#define SAVED_PLAY_TIME (*(Uint32 *)SceneLocalState)

/* The game's state as a file holds it, from BupBuffer (system/definitions.h's
   save image): a save part's three sectors. */
#define BUP_IMAGE_BYTES	 (BUP_SECTORS_PER_PART * BUP_SECTOR_SIZE)
/* Where the data a password or link screen brings in waits for
   gmDataTransferApply, as a save image: after the state set aside at the start
   of the map metatile buffer, free on this screen. */
#define TRANSFERRED_SAVE (RamMapMetatileBuffer + BUP_IMAGE_BYTES)
/* A save image from the game flags to the units' end: the flags, the main
   routine's state and the party's units. */
#define BUP_IMAGE_STATE_BYTES \
	(BUP_IMAGE_UNITS + sizeof(CharacterUnits) - BUP_IMAGE_GAME_FLAGS)

/* The width of the answers "Yeah!" and "Wait a sec" to "Play on EASY level?"
   and the two questions after it, as each language's words need. */
#define YEAH_WIDTH BY_LANGUAGE(6, 8, 8, 10, 8, 8)

/* The password and link screens read The Broken Seal's data into the game's
   state: KEEP_SAVE_IMAGE sets the state aside in the map metatile buffer first,
   and RESTORE_SAVE_IMAGE copies what the screen left to TRANSFERRED_SAVE and
   puts the state back. */
#define COPY_SAVE_IMAGE(to, from) syWordsCopy(to, from, BUP_IMAGE_BYTES)
#define KEEP_SAVE_IMAGE() \
	Uint8 *image = BupBuffer; \
\
	COPY_SAVE_IMAGE(RamMapMetatileBuffer, image)
#define RESTORE_SAVE_IMAGE() \
	{ \
		COPY_SAVE_IMAGE(TRANSFERRED_SAVE, image); \
		COPY_SAVE_IMAGE(image, RamMapMetatileBuffer); \
	}

/* Goes back to the saved scene and entrance. */
#define RETURN_TO_SAVED(party) \
	{ \
		(party)->next = (party)->restartScene; \
		(party)->entrance = (party)->restartEntrance; \
		fdWorldMapSetReturnLeavingScene((party)->next, (party)->entrance); \
		fdScriptClearFlag(FLAG_KEEP_PARTY_POSITION); \
	}

Sint32 scInit(void)
{
	Bool   started = FALSE;
	Uint32 mask;
	Sint32 result, i, limit, stat, msg, recovery;

	scClearLoadBackground();
	sySetEvent(scClearCheckButtonCodes, EVENT_PASS_TICK);
	FieldStateWork->transition = started;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	/* After the epilogue: the cleared game may be saved, then the title. */
	if(MainRoutineStateWork.entrance == CLEAR_FROM_EPILOGUE) {
		do {
			msg = (Sint32)MsgSaveYourAdventure;
			/* "Save your adventure?" */
			mnFieldMessageShow(
				msg, FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN);
			i = fdYesNoAsk(1);
			mnMessageCloseFinishedWindows();
			if(i != YES) {
				break;
			}
			/* "You cannot resume your adventure with this data." */
			mnFieldMessageShow(msg + 1, FIELD_MESSAGE_ZOOM);
			/* Marks the file cleared (BupFileSummary's sendFlg). */
			MainRoutineStateWork.sendFlg = 1;
			gmPartyAddMember(JENNA);
			gmPartyAddMember(SHEBA);
			gmPartyAddMember(PIERS);
			gmPartyAddMember(GARET);
			gmPartyAddMember(IVAN);
			gmPartyAddMember(MIA);
			i = mnSaveToChosenFile();
		} while(i == -1);
		fdScriptWait(60);
		fdScriptFinishScreenTransition();
		sySndPlay(SOUND_FADE_OUT_BGM);
		fdScriptWait(300);
		fdScriptSetDestination(SCENE_TITLE, TITLE_START);
		return 0;
	}
	SleepDisabled = started;
top:
	result = mnSaveScanFileSummaries();
	if(result < 0 && DebugMode) {
		MainRoutineStateWork.idleSleepEnabled = 1;
		IdleSleepEnabled = 1;
		/* "Unable to detect backup memory." */
		mnFieldMessageShowCentered((Sint32)MsgNoBackupMemory, 1, 8);
	}
	if(result == 0 && !started) {
		sySynch(30);
		sySetEvent(scClearFadeInBlend, EVENT_PASS_TICK);
		sySynch(1);
		started = TRUE;
	}
	if(result > 0)
		result = mnFileListSelectAction();
	else
		result = 0;
	if(result == SAVE_ACTION_NEW_GAME) {
		/* Felix is named, and with either code Jenna, Sheba and Piers too. */
		gmMainRoutineStateInitDefaults();
		mnWindowColApply(
			MainRoutineStateWork.winHue, MainRoutineStateWork.winBrightness);
		i = FELIX;
		limit = FELIX + 1;
		for(;;) {
			sySynch(6);
			result = mnNameEntryEditMemberName(i);
			if(result == -1) {
				if(i == FELIX) {
					goto top;
				}
				i--;
				continue;
			}
			if(ClearCodeUnlocked != 0 || ClearExtraCodeUnlocked != 0) {
				limit = PIERS + 1;
			}
			if(++i >= limit) {
				break;
			}
		}
	ask:
		/* "Transfer data from Golden Sun?" */
		mnFieldMessageShow(
			(Sint32)MsgTransferDataFromGoldenSun, QUESTION_MESSAGE);
		result = mnYesNoMenuRun(1, 0, 0, 0);
		mnMessageCloseFinishedWindows();
		if(result == YES) {
			for(;;) {
				result =
					mnIconMenuChooseSendOption(SEND_OPTION_TRANSFER_METHOD);
				if(result == -1) {
					goto top;
				}
				if(result == TRANSFER_METHOD_CABLE) {
					KEEP_SAVE_IMAGE();
					stat = scClearReceiveByLink();
					if(stat != -1) {
						mnStatScreenRun();
					}
					RESTORE_SAVE_IMAGE()
					if(stat == -1) {
						continue;
					}
					Call3((void (*)())gmDataTransferApply,
						(Sint32)TRANSFERRED_SAVE, 0, 0);
				} else if(result == TRANSFER_METHOD_PASSWORD) {
					KEEP_SAVE_IMAGE();
					gmMainRoutineStateInitDefaults();
#if !NEW_GAME_IGNORES_PASSWORD_CANCEL
					stat = scClearEnterPassword();
					RESTORE_SAVE_IMAGE()
					if(stat == -1) {
						continue;
					}
#else
					scClearEnterPassword();
					RESTORE_SAVE_IMAGE()
#endif
					Call3((void (*)())gmDataTransferApply,
						(Sint32)TRANSFERRED_SAVE, 1, 0);
				}
				break;
			}
		} else {
			gmDataTransferGiveDefaultDjinn();
		}
		/* Isaac is named, and with the second code Garet, Ivan and Mia too. */
		i = ISAAC;
		limit = ISAAC + 1;
		for(;;) {
			sySynch(6);
			result = mnNameEntryEditMemberName(i);
			if(result == -1) {
				if(i == ISAAC) {
					goto ask;
				}
				i--;
				continue;
			}
			if(ClearExtraCodeUnlocked != 0) {
				limit = MIA + 1;
			}
			if(++i >= limit) {
				break;
			}
		}
		if(AnyFileCleared != 0) {
		save:
			msg = (Sint32)MsgPlayOnEasyLevel;
			/* "Play on EASY level? (Characters start at Lost Age Clear Data
			   levels)" */
			mnFieldMessageShow(msg, QUESTION_MESSAGE);
			i = mnYesNoMenuRun(1, (Sint32)MsgYeah, YEAH_WIDTH, 0);
			mnMessageCloseFinishedWindows();
			if(i == YES) {
				if(AnyFileCleared == 0) {
					/* "...Sorry, you have no Clear Data." */
					mnFieldMessageShow(msg + 1, FIELD_MESSAGE_ZOOM);
				} else {
					KEEP_SAVE_IMAGE();
					result = mnSaveChooseAndLoadFile(FILE_LIST_CLEARED);
					if(result != 0)
						goto save;
					{
						MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

						mnWindowColApply(party->winHue, party->winBrightness);
						RESTORE_SAVE_IMAGE()
						gmClearDataApply(TRANSFERRED_SAVE);
						party->difficulty = DIFFICULTY_EASY;
					}
					goto leave;
				}
			}
			/* "Play on HARD level? (Monsters are stronger)" */
			mnFieldMessageShow((Sint32)MsgPlayOnHardLevel, QUESTION_MESSAGE);
			i = mnYesNoMenuRun(1, (Sint32)MsgYeah, YEAH_WIDTH, 0);
			mnMessageCloseFinishedWindows();
			if(i == YES) {
				fdScriptSetFlag(FLAG_HARD_LEVEL);
				MainRoutineStateWork.difficulty = DIFFICULTY_HARD;
				goto leave;
			}
			/* "Play on Normal level? (Default difficulty)" */
			mnFieldMessageShow((Sint32)MsgPlayOnNormalLevel, QUESTION_MESSAGE);
			i = mnYesNoMenuRun(1, (Sint32)MsgYeah, YEAH_WIDTH, 0);
			mnMessageCloseFinishedWindows();
			if(i != YES)
				goto save;
		}
	leave:
		fdScriptSetDestination(SCENE_TITLE, TITLE_STORY_CRAWL);
		goto finish;
	}
	if(result == SAVE_ACTION_CONTINUE) {
		result = mnSaveChooseAndLoadFile(FILE_LIST_CONTINUE);
		if(result == -1)
			goto top;
		fdScriptSetFlag(FLAG_KEEP_PARTY_POSITION);
		{
			MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

			mnWindowColApply(party->winHue, party->winBrightness);
			if(party->buildStamp != gmBuildStampGetTime()) {
				RETURN_TO_SAVED(party)
			} else if(mask = KEY_L | KEY_START, (PerWork.data & mask) == mask) {
				if(scClearIsRetreatBlocked()) {
					/* "Unfortunately, your data could not be recovered." */
					mnFieldMessageShow((Sint32)MsgDataCouldNotBeRecovered,
						FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_LOWER);
					goto top;
				}
				RETURN_TO_SAVED(party)
				fdScriptSetFlag(FLAG_SAVE_RESTARTED_AT_SANCTUM);
			} else if(party->playTime != SAVED_PLAY_TIME) {
				recovery = (Sint32)MsgSomeDataIsCorrupted;
				/* "Some data is corrupted and cannot be recovered." */
				mnFieldMessageShow(
					recovery, FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_LOWER);
				/* "Do you want to try to recover from a Sanctum?" */
				mnFieldMessageShow(recovery + 1, QUESTION_MESSAGE);
				if(mnYesNoMenuRun(1, 0, 0, 0) != YES) {
					mnMessageCloseFinishedWindows();
					goto top;
				}
				mnMessageCloseFinishedWindows();
				if(scClearIsRetreatBlocked()) {
					/* "Unfortunately, your data could not be recovered." */
					mnFieldMessageShow(
						recovery + 2, FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_LOWER);
					goto top;
				}
				RETURN_TO_SAVED(party)
				fdScriptSetFlag(FLAG_SAVE_RECOVERED_AT_SANCTUM);
			}
		}
		fdScriptClearFlag(FLAG_MENU_OPEN);
		goto finish;
	}
	if(result == SAVE_ACTION_COPY) {
		mnSaveCopyFile();
		goto top;
	}
	if(result == SAVE_ACTION_ERASE) {
		mnSaveEraseSelectedFile();
		goto top;
	}
	if(result == SAVE_ACTION_BATTLE) {
		result = mnSaveChooseAndLoadFile(FILE_LIST_BATTLE);
		if(result == -1)
			goto top;
		{
			MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

			party->leaderActorId = ACTOR_FELIX;
			if(fdScriptGetFlag(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT))
				gmInventorySnapshotRestore();
			mnWindowColApply(party->winHue, party->winBrightness);
			fdScriptClearFlag(FLAG_KEEP_PARTY_POSITION);
			fdScriptClearFlag(FLAG_MENU_OPEN);
			party->ownerOverride = 0;
			SleepDisabled = 1;
			fdScriptSetDestination(SCENE_LOBBY, 1);
		}
		goto finish;
	}
	if(result != SAVE_ACTION_UPDATE)
		goto top;
link:
	result = mnSaveChooseAndLoadFile(FILE_LIST_UPDATE);
	if(result == -1)
		goto top;
	mnWindowColApply(
		MainRoutineStateWork.winHue, MainRoutineStateWork.winBrightness);
	for(;;) {
		/* The menu reads only its mode's lowest bit: the transfer method. */
		result = mnIconMenuChooseSendOption(2);
		if(result == -1) {
			goto link;
		}
		if(result == TRANSFER_METHOD_CABLE) {
			KEEP_SAVE_IMAGE();
			stat = scClearReceiveByLink();
			if(stat != -1) {
				mnStatScreenRun();
			}
			RESTORE_SAVE_IMAGE()
			if(stat == -1) {
				continue;
			}
			Call3((void (*)())gmDataTransferApply, (Sint32)TRANSFERRED_SAVE, 0,
				1);
		} else if(result == TRANSFER_METHOD_PASSWORD) {
			KEEP_SAVE_IMAGE();
			gmMainRoutineStateInitDefaults();
			stat = scClearEnterPassword();
			RESTORE_SAVE_IMAGE()
			if(stat == -1) {
				continue;
			}
			Call3((void (*)())gmDataTransferApply, (Sint32)TRANSFERRED_SAVE, 1,
				1);
		}
		break;
	}
	if(ClearCodeUnlocked != 0 || ClearExtraCodeUnlocked != 0) {
		i = ISAAC;
		limit = ISAAC + 1;
		for(;;) {
			sySynch(6);
			result = mnNameEntryEditMemberName(i);
			if(result == -1) {
				if(i == ISAAC) {
					goto link;
				}
				i--;
				continue;
			}
			if(ClearExtraCodeUnlocked != 0) {
				limit = MIA + 1;
			}
			if(++i >= limit) {
				break;
			}
		}
	}
#if UPDATE_KEEPS_PARTY_POSITION
	fdScriptSetFlag(FLAG_KEEP_PARTY_POSITION);
#endif
finish:
	FieldStateWork->exit = EXIT_KEEP_DESTINATION;
	fdScriptWait(30);
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(60);
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

/* The symbols of each level's password: The Broken Seal packs 173, 39 or 9
   bytes (FileScreenPackPasswordData), then two of its checksum, six bits a
   symbol and a tenth symbol after every nine. */
#define PASSWORD_GOLD_LENGTH	  260
#define PASSWORD_SILVER_LENGTH	  61
#define PASSWORD_BRONZE_LENGTH	  16
/* The bytes each level decodes to, its checksum's two included. */
#define PASSWORD_GOLD_BYTES		  175
#define PASSWORD_SILVER_BYTES	  41
#define PASSWORD_BRONZE_BYTES	  11
#define PASSWORD_SYMBOLS_PER_PAGE 50 /* five rows of ten */
#define PASSWORD_SYMBOLS_PER_ROW  10
#define PASSWORD_SYMBOL_MASK	  0x3f /* six bits: 64 symbols */
/* A place of the entered password that holds no symbol yet. */
#define PASSWORD_SYMBOL_EMPTY	  99
/* The checksum's two bytes at the end of each level's
   (scClearCalcPasswordCrc16). */
#define PASSWORD_CRC_BYTES		  2
#define PASSWORD_CRC_SEED		  0xffff
#define PASSWORD_CRC_POLYNOMIAL	  0x1021
#define PASSWORD_CRC_TOP_BIT	  0x8000 /* of its 16 bits */
/* Where a password's parts start among its bytes: the four levels and the Djinn
   from the first, the story flags, then Silver's and Bronze's Psynergy items,
   which Gold has not; the stats rows after those, two party members to a row;
   and Gold's alone, the party's items, 15 a member in nine bits each, their
   quantities, five bits for each of the 23 items that stack
   (ClearImportedStackItems), and the coins, high byte first. */
#define PASSWORD_FLAGS_BYTE		  7
#define PASSWORD_KEY_ITEMS_BYTE	  8
#define PASSWORD_STATS_ROW_BYTES  15
#define PASSWORD_ITEMS_BYTE		  39
#define PASSWORD_QUANTITIES_BYTE  107
#define PASSWORD_COINS_BYTE		  165
#define PASSWORD_STACK_ITEMS	  23
/* A stacking item's quantity in the password: its five bits. */
#define PASSWORD_QUANTITY_MASK \
	(INVENTORY_QUANTITY_MASK >> INVENTORY_QUANTITY_SHIFT)
/* A party member's base stats in the password, two words of fields: HP (11
   bits), PP (11) and Attack (10); then Defense (10), Agility (10) and Luck (8)
   over four bits of 0. This game reads Attack, Defense and Agility back in nine
   bits and Luck in seven. */
#define PASSWORD_HP_SHIFT	   21
#define PASSWORD_PP_SHIFT	   10
#define PASSWORD_DEFENSE_SHIFT 22
#define PASSWORD_AGILITY_SHIFT 12
#define PASSWORD_LUCK_SHIFT	   4
#define PASSWORD_PP_MASK	   0x7ff /* eleven bits */
#define PASSWORD_STAT_MASK	   0x1ff /* nine bits: Attack, Defense and Agility */
#define PASSWORD_LUCK_MASK	   0x7f	 /* seven bits */
/* A level in the password's first bytes: seven bits each. */
#define PASSWORD_LEVEL_MASK	   0x7f
/* A password glyph's sprite: four tiles, 16 pixels square. */
#define GLYPH_SPRITE_BYTES	   (4 * TILE_SIZE_4BPP)
/* A unit's Djinn, djinnHeld to djinnSetCnts. */
#define UNIT_DJINN_BYTES	   40

/* The password as entered, a symbol a byte: a Gold password is 260 symbols,
   which the Japanese buffer holds exactly, as in The Broken Seal. The other
   editions reserve 320 symbols for it, and the Japanese 60 bytes after the
   decoded password; nothing in this game writes or reads past 260 symbols or
   the decoded bytes, so the extra bytes are never used. */
#define PASSWORD_BUFFER_SIZE \
	WHEN(LARGER_PASSWORD_BUFFER, 320, PASSWORD_GOLD_LENGTH)
/* The password as the player enters it, a symbol a byte. */
Uint8 ClearPasswordText[PASSWORD_BUFFER_SIZE] BSS_DATA;
/* The entered password's bytes, decoded (scClearDecodePassword). */
Uint8 ClearDecodedPassword[PASSWORD_GOLD_BYTES] BSS_DATA;
#if !LARGER_PASSWORD_BUFFER
/* Unused: the Japanese edition's 60 bytes after the decoded password. */
Uint8 ClearPasswordSpare[60] BSS_DATA;
#endif

/* The password grid's glyph sprites, one for each place of a page. */
typedef struct PASSWORD_STATE {
	void *glyphs[PASSWORD_SYMBOLS_PER_PAGE];
	void *metatiles[PASSWORD_SYMBOLS_PER_PAGE];
	Uint8 shown[PASSWORD_SYMBOLS_PER_PAGE]; /* the symbol each place shows */
	Uint8 unused[26]; /* cleared on entry, read by nothing */
} PASSWORD_STATE;

/* The password's symbols in the font: in the Japanese edition kana in their
   order (charmap_tla_ja.tsv's codes), あ to そ, た to ん and を, then with
   the voiced mark が to ぞ, だ, で, ど and ば to ぼ; in the others
   capitals without I and O, the digits 2 to 9, small letters without l and o,
   then ! ? # & $ % + and =. */
#if LANGUAGE == LANGUAGE_JAPANESE
#define SYMBOL_TA	 15
#define SYMBOL_N	 44
#define SYMBOL_WO	 45
#define SYMBOL_GA	 46
#define SYMBOL_DA	 56
#define SYMBOL_BA	 59
/* The character codes of kana that start runs in the font's order, as The
   Broken Seal's window/text_font.h names them. */
#define KANA_WO		 134 /* を */
#define KANA_A		 145 /* あ, then い to そ */
#define KANA_KA		 150 /* か, then き to そ */
#define KANA_TA		 224 /* た, then ち to ん */
#define KANA_TE		 227 /* て */
#define KANA_TO		 228 /* と */
#define KANA_HA		 234 /* は, then ひ to ほ */
#define KANA_N		 253 /* ん */
/* A kana's code with the voiced mark, FontMarkGlyphCodes's second. */
#define VOICED(code) ((code) | 1 << TEXT_MARK_SHIFT)
#else
#define SYMBOL_DIGITS		 24
#define SYMBOL_SMALL_LETTERS 32
#define SYMBOL_SIGNS		 56
#endif

/* The character a password symbol stands for, in the font's codes, a kana's
   voiced mark in its top bits. */
void scClearGetPasswordCharacter(Sint32 metatile, Uint16 *text)
{
	text[1] = 0;
	text[2] = 0;
#if LANGUAGE != LANGUAGE_JAPANESE
	if(metatile <= 'H' - 'A')
		text[0] = metatile + 'A';
	else if(metatile <= 'N' - 'A' - 1)
		text[0] = metatile + 'A' + 1;
	else if(metatile <= 'Z' - 'A' - 2)
		text[0] = metatile + 'A' + 2;
	else if(metatile < SYMBOL_SMALL_LETTERS)
		text[0] = metatile - SYMBOL_DIGITS + '2';
	else if(metatile <= SYMBOL_SMALL_LETTERS + 'k' - 'a')
		text[0] = metatile - SYMBOL_SMALL_LETTERS + 'a';
	else if(metatile <= SYMBOL_SMALL_LETTERS + 'n' - 'a' - 1)
		text[0] = metatile - SYMBOL_SMALL_LETTERS + 'a' + 1;
	else if(metatile <= SYMBOL_SMALL_LETTERS + 'z' - 'a' - 2)
		text[0] = metatile - SYMBOL_SMALL_LETTERS + 'a' + 2;
	else if(metatile == SYMBOL_SIGNS)
		text[0] = '!';
	else if(metatile == SYMBOL_SIGNS + 1)
		text[0] = '?';
	else if(metatile == SYMBOL_SIGNS + 2)
		text[0] = '#';
	else if(metatile == SYMBOL_SIGNS + 3)
		text[0] = '&';
	else if(metatile == SYMBOL_SIGNS + 4)
		text[0] = '$';
	else if(metatile == SYMBOL_SIGNS + 5)
		text[0] = '%';
	else if(metatile == SYMBOL_SIGNS + 6)
		text[0] = '+';
	else
		text[0] = '=';
#else
	if(metatile < SYMBOL_TA)
		text[0] = metatile + KANA_A;
	else if(metatile < SYMBOL_N)
		text[0] = metatile - SYMBOL_TA + KANA_TA;
	else if(metatile == SYMBOL_WO)
		text[0] = KANA_WO;
	else if(metatile == SYMBOL_N)
		text[0] = KANA_N;
	else if(metatile < SYMBOL_DA)
		text[0] = VOICED(metatile - SYMBOL_GA + KANA_KA);
	else if(metatile == SYMBOL_DA)
		text[0] = VOICED(KANA_TA);
	else if(metatile == SYMBOL_DA + 1)
		text[0] = VOICED(KANA_TE);
	else if(metatile == SYMBOL_DA + 2)
		text[0] = VOICED(KANA_TO);
	else
		text[0] = VOICED(metatile - SYMBOL_BA + KANA_HA);
#endif
}

/* The transferred data's game flags, and its main routine's state and what
   follows (system/definitions.h's save image). */
#define TRANSFERRED_FLAGS (TRANSFERRED_SAVE + BUP_IMAGE_GAME_FLAGS)
#define TRANSFERRED_MAIN_ROUTINE_STATE \
	(TRANSFERRED_SAVE + BUP_IMAGE_MAIN_ROUTINE_STATE)

/* The first letter of the greeting this screen answers The Broken Seal with,
   before V, T and S: the one its FILE_SCREEN_LINK_LANGUAGE_TAG waits for. */
#define LINK_LANGUAGE_TAG BY_LANGUAGE('C', 'U', 'E', 'S', 'F', 'I')

/* Receiving by cable stops waiting after this long, almost three hours, and
   ends with "Data transfer was interrupted" once The Broken Seal goes unheard
   this long, as its LINK_SEND_TICKS and LINK_SILENT_TICKS. */
#define LINK_RECEIVE_TICKS 600000
#define LINK_SILENT_TICKS  10

/* The windows are as wide as each language's text needs. */
#define RECEIVE_WINDOW_X	 BY_LANGUAGE(6, 6, 5, 5, 2, 6)
#define RECEIVE_WINDOW_WIDTH BY_LANGUAGE(18, 18, 20, 20, 26, 18)
#define STATUS_WINDOW_X		 FRENCH_OR(2, 5)
#define STATUS_WINDOW_WIDTH	 FRENCH_OR(26, 20)

/* Receives The Broken Seal's data over the link cable: waits for that game to
   greet with "0000", answers with the greeting it waits for, receives its save
   image into TRANSFERRED_SAVE and reads the image's flags and units into the
   game's state. Returns 1, or -1 when B gives up or the link is lost. */
Sint32 scClearReceiveByLink(void)
{
	void   *win;
	Sint32	i, idle, ok, shown;
	Sint32	mask, linkMask;
	Sint32	msg;
	Uint8  *flg;
	Uint16 *request;

	win = mnWindowCreate(
		RECEIVE_WINDOW_X, 5, RECEIVE_WINDOW_WIDTH, 8, WINDOW_FLAG_INSTANT);
	/* "Preparing to receive", "saved Clear Data" and "B Button: Quit". */
	msg = (Sint32)MsgFieldPreparingToReceive;
	mnTextDrawMessage(msg, win, 0, 4);
	mnTextDrawMessage(msg + 1, win, 0, 16);
	mnTextDrawMessage(msg + 3, win, 0, 36);
	sySerialInit();
	syWait(10);
restart:
	mask = LINK_STATUS_HEARD_BOTH;
	goto poll;
scan:
	if((LinkStatusWork & mask) == mask) {
		Uint16 *signature = LinkPeerSignatures[SIO_ID_OF(REG_SIOCNT) ^ 1];

		/* The Broken Seal sets its greeting to "0000" as it waits to send. */
		if(signature[0] == '0' && signature[1] == '0' && signature[2] == '0' &&
			signature[3] == '0') {
			request = (Uint16 *)SerialTransfer.payload;
			request[0] = LINK_LANGUAGE_TAG;
			request[1] = 'V';
			request[2] = 'T';
			request[3] = 'S';
			goto receive;
		}
	}
	syWait(1);
poll:
	if(!(PerWork.push & KEY_B))
		goto scan;
	fdScriptPlaySnd(SE_MENU_CANCEL);
	goto close;
receive:
	syWait(10);
	sySerialStartReceive(TRANSFERRED_SAVE);
	syWait(10);
	linkMask = LINK_STATUS_HEARD_BOTH;
	idle = 0;
	ok = 1;
	shown = 0;
	i = 0;
transfer:
	if(!shown && SerialReceivedSize != 0) {
		shown = 1;
		mnWindowClose(win, 2);
		win = mnWindowCreate(
			STATUS_WINDOW_X, 10, STATUS_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
		mnTextDrawMessage((Sint32)MsgFieldTransferringData, win, 0, 4);
	}
	if(i == 9 && SerialReceivedSize == 0) {
		request = (Uint16 *)SerialTransfer.payload;
		request[0] = 0;
		request[1] = 0;
		request[2] = 0;
		request[3] = 0;
		sySerialRemoveIRQHandlers();
		syWait(10);
		sySerialInit();
		syWait(10);
		goto restart;
	}
	idle++;
	if((LinkStatusWork & linkMask) == linkMask)
		idle = 0;
	if(idle == LINK_SILENT_TICKS) {
		ok = 0;
		goto done;
	}
	if(SerialReceiveDestination != 0) {
		syWait(1);
		i++;
		if(i < LINK_RECEIVE_TICKS)
			goto transfer;
	}
done:
	if(!ok) {
		mnWindowSprPrepareForRedraw(win);
		mnTextDrawMessage(
			(Sint32)MsgFieldDataTransferWasInterrupted, win, 0, 4);
		do {
			syWait(1);
		} while(!(PerWork.push & KEY_A));
	close:
		mnWindowClose(win, 2);
		return -1;
	}
	syWait(10);
	sySerialRemoveIRQHandlers();
	syWait(10);
	mnWindowClose(win, 2);
	/* The flags and the fields before the Djinn recovery list come over in
	   place, as both games keep them; then The Broken Seal's main routine's
	   state and units, moved on by as much as this game's list is longer, so
	   that its units land on this game's. */
	flg = GameFlagBytes;
	syWordsCopy(flg, TRANSFERRED_FLAGS,
		BUP_IMAGE_MAIN_ROUTINE_STATE - BUP_IMAGE_GAME_FLAGS +
			offsetof(MAIN_ROUTINE_STATE, djinnRecovery));
	syWordsCopy(flg + (BUP_IMAGE_MAIN_ROUTINE_STATE - BUP_IMAGE_GAME_FLAGS) +
			(BUP_IMAGE_UNITS - BROKEN_SEAL_SAVE_IMAGE_UNITS),
		TRANSFERRED_MAIN_ROUTINE_STATE,
		BROKEN_SEAL_SAVE_IMAGE_UNITS + sizeof(CharacterUnits) -
			BUP_IMAGE_MAIN_ROUTINE_STATE);
	gmUnitCalc(0);
	gmUnitCalc(1);
	gmUnitCalc(2);
	gmUnitCalc(3);
	syWordsCopy(TRANSFERRED_FLAGS, flg, BUP_IMAGE_STATE_BYTES);
	syFieldWindowColApply(
		MainRoutineStateWork.winHue, MainRoutineStateWork.winBrightness);
	return 1;
}

#include "../window/screens_set_text_tile_rect.c"

extern Uint8 ClearFourthPasswordRow[];

extern Uint8 MsgFieldPasswordKana[];

Uint16 scClearCalcPasswordCrc16(Uint32 length, Uint8 *p);
/* The ROM's caller here is compiled against this form, which takes no argument
   where the definition takes the slot. */
void mnPointingHandLoadIntoSlot(void);
/* The ROM's caller here is compiled against this form, which takes no argument
   where the definition takes the slot. */
void mnNameCaretLoadIntoSlot(void);

/* Gives a cursor its sprite. */
static inline void scCursorAttach(MENU_CURSOR *cursor, WINDOW_SPR *spr)
{
	cursor->anchor = spr;
}

/* Sets a cursor where it starts. */
static inline void scCursorPlace(MENU_CURSOR *cursor, Sint32 x, Sint32 y)
{
	mnCursorSetPosImmediate(cursor, x, y);
}

/* Moves a cursor and shows it. */
static inline void scCursorShow(MENU_CURSOR *cursor, Sint32 x, Sint32 y)
{
	mnCursorSet(cursor, x, y, 1);
	mnCursorAdvance(cursor);
}

#if (LANGUAGE == LANGUAGE_JAPANESE)
/* The Japanese grid has 20 columns of kana and its side column is 20. */
#define SIDE_COLUMN					  20
#define SIDE_HIGHLIGHT_WIDTH		  3
#define CHECK_WINDOW_X				  8
#define CHECK_WINDOW_WIDTH			  14
#define CURSOR_ON_SIDE_COLUMN(column) ((column) == SIDE_COLUMN)
#else
#define SIDE_COLUMN					  16
#define SIDE_HIGHLIGHT_WIDTH		  6
#define CHECK_WINDOW_X				  5
#define CHECK_WINDOW_WIDTH			  20
#define CURSOR_ON_SIDE_COLUMN(column) ((column) > SIDE_COLUMN - 1)
#endif

#if (LANGUAGE == LANGUAGE_JAPANESE)
/* Draws the kana to choose from and the side column's Space, Return and Finish
   into the lower window. */
#define DRAW_KEYS(window) \
	{ \
		msg = (Sint32)MsgFieldPasswordKana; \
		mnTextDrawMessageAsTiles(msg++, window, 0, 0); \
		mnTextDrawMessageAsTiles(msg++, window, 0, 8); \
		mnTextDrawMessageAsTiles(msg++, window, 0, 16); \
		mnTextDrawMessageAsTiles(msg++, window, 0, 24); \
		mnTextDrawMessageAsTiles(msg++, window, 48, 0); \
		mnTextDrawMessageAsTiles(msg++, window, 48, 8); \
		mnTextDrawMessageAsTiles(msg++, window, 48, 16); \
		mnTextDrawMessageAsTiles(msg++, window, 48, 24); \
		mnTextDrawMessageAsTiles(msg++, window, 96, 0); \
		mnTextDrawMessageAsTiles(msg++, window, 96, 8); \
		mnTextDrawMessageAsTiles(msg++, window, 96, 16); \
		mnTextDrawMessageAsTiles(msg++, window, 96, 24); \
		mnTextDrawMessageAsTiles(msg++, window, 144, 0); \
		mnTextDrawMessageAsTiles(msg++, window, 144, 8); \
		mnWindowDrawDividerLine(window, 24, 0, 24, 5); \
		mnTextDrawMessageAsTiles(msg++, window, 192, 0); \
		mnTextDrawMessageAsTiles(msg++, window, 192, 8); \
		mnTextDrawMessageAsTiles(msg, window, 192, 24); \
	}
#else
/* Draws the symbols to choose from and the side column's Space, Return and
   Finish into the lower window. */
#define DRAW_KEYS(window) \
	{ \
		mnTextDrawStringInWindow(ClearFirstPasswordRow, window, 0, 0); \
		mnTextDrawStringInWindow(ClearSecondPasswordRow, window, 0, 8); \
		mnTextDrawStringInWindow(ClearThirdPasswordRow, window, 0, 16); \
		mnTextDrawStringInWindow(ClearFourthPasswordRow, window, 0, 24); \
		msg = (Sint32)MsgFieldSpace; \
		mnWindowDrawDividerLine(window, 20, 0, 20, 5); \
		mnTextDrawMessageAsTiles(msg++, window, 160, 0); \
		mnTextDrawMessageAsTiles(msg++, window, 160, 8); \
		mnTextDrawMessageAsTiles(msg, window, 160, 24); \
	}
#endif

/* The rows of symbols to choose from, and the side column's keys beside them:
   Space moves on a place as R does, Return back a place as L does, and Finish
   checks the password as Start does. The third row has no side key. */
enum {
	KEY_ROW_SPACE = 0,
	KEY_ROW_RETURN = 1,
	KEY_ROW_WITHOUT_SIDE_KEY = 2,
	KEY_ROW_FINISH = 3,
	KEY_ROWS = 4
};

/* What A chose this tick: a symbol of the grid or a side key. */
typedef enum PASSWORD_ACTION {
	PASSWORD_ACTION_NONE = -1,
	PASSWORD_ACTION_SYMBOL,
	PASSWORD_ACTION_SPACE,
	PASSWORD_ACTION_RETURN,
	PASSWORD_ACTION_FINISH
} PASSWORD_ACTION;

/* The password screen's stages: entering symbols; the last place just filled,
   which is drawn for a tick before the password is checked; and checking. */
typedef enum PASSWORD_STAGE {
	PASSWORD_STATE_ENTERING,
	PASSWORD_STATE_CHECK,
	PASSWORD_STATE_LAST_ENTERED
} PASSWORD_STAGE;

/* The password screen: chooses the password's level, then lets the player enter
   it on the grid of symbols, checks it and applies it; returns -1 when
   cancelled. */
Sint32 scClearEnterPassword(void)
{
	MENU_CURSOR		cursorStore;
	MENU_CURSOR		gridStore;
	Uint16			glyph[12];
	Sint32			mode;
	Sint32			cursor;
	WINDOW		   *grid;
	Sint32			column;
	Sint32			row;
	Sint32			length;
	Sint32			pages;
	PASSWORD_STATE *passwordState;
	Sint32			drawColumn;
	Sint32			drawRow;
	Sint32			drawMode;
	Sint32			cursorX;
	Sint32			cursorY;
	Sint32			state;
	Sint32			advance;
	WINDOW		   *win;
	Sint32			action;
	Sint32			cnt;
	Sint32			page;
	Sint32			i;
	Sint32			msg;
	Sint32			frm;
	Sint32			crcLength;
	Sint32			entry;
	WINDOW_SPR	   *spr;
	Uint16		   *palette;
	Sint32			height;

	syWindowLoadGraphics();
	mnWindowColApply(
		MainRoutineStateWork.winHue, MainRoutineStateWork.winBrightness);
	/* The highlight is the window palette with a grey background. */
	palette = (Uint16 *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0);
	syDMAXCopy((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0), palette,
		DMA_COPY_WORDS(8), REG_DMA3);
	palette[WINDOW_BACKGROUND_COLOR] = RGB(24, 24, 24);
	mode = mnIconMenuChooseSendOption(SEND_OPTION_PASSWORD_LEVEL);
	if(mode == -1)
		return -1;
	sySndPlay(MUS_PASSWORD_ENTRY);
	cursor = 0;
	column = 0;
	row = 0;
	passwordState = syHeapAllocEWRAM(sizeof(PASSWORD_STATE));
	advance = 0;
	mnGraphicsExpandVRAMTilesByColTable(
		BG_CHAR_BLOCK(0) + PAGE_DIGITS_TILE * TILE_SIZE_4BPP);
	if(mode == PASSWORD_LEVEL_GOLD) {
		length = PASSWORD_GOLD_LENGTH;
	} else {
		length = PASSWORD_SILVER_LENGTH;
		if(mode != PASSWORD_LEVEL_SILVER)
			length = PASSWORD_BRONZE_LENGTH;
	}
	pages = length / PASSWORD_SYMBOLS_PER_PAGE + 1;
	win = mnWindowCreate(1, 13, 29, 6, WINDOW_FLAG_INSTANT);
	DRAW_KEYS(win)
	grid = mnWindowCreate(5, 1, 20, 12, WINDOW_FLAG_INSTANT);
	for(i = 0; i != ARRAY_COUNT(passwordState->unused); i++)
		passwordState->unused[i] = 0;
	for(i = 0; i != PASSWORD_GOLD_LENGTH; i++)
		ClearPasswordText[i] = PASSWORD_SYMBOL_EMPTY;
	for(i = 0; i != PASSWORD_SYMBOLS_PER_PAGE; i++) {
		Sint32 col = i % PASSWORD_SYMBOLS_PER_ROW;
		Sint32 y;
		Sint32 x;

		/* Ten places a row, 12 pixels apart, with a gap after the fifth. */
		x = col * 12 + 8;
		y = i / PASSWORD_SYMBOLS_PER_ROW * 16 + 2;
		if(col > 4)
			x = col * 12 + 18;
		passwordState->metatiles[i] =
			mnGlyphAddBlankSpr(grid, passwordState->glyphs[i], x, y);
		passwordState->shown[i] = PASSWORD_SYMBOL_EMPTY;
		syWordsClearEntry(passwordState->metatiles[i], GLYPH_SPRITE_BYTES);
	}
	entry = syVRAMCacheFindFreeEntry();
	if(entry < VRAM_CACHE_ENTRY_COUNT) {
		mnPointingHandLoadIntoSlot();
		spr = mnWindowSprCreate(
			entry, WINDOW_SPRITE_SIZE_16X16, (WINDOW *)win, 0, 0);
		scCursorAttach(&cursorStore, spr);
		scCursorPlace(&cursorStore, win->x * 8 - 2, win->y * 8 + 12);
	}
	entry = syVRAMCacheFindFreeEntry();
	if(entry < VRAM_CACHE_ENTRY_COUNT) {
		mnNameCaretLoadIntoSlot();
		spr = mnWindowSprCreate(
			entry, WINDOW_SPRITE_SIZE_16X16, (WINDOW *)win, 0, 0);
		scCursorAttach(&gridStore, spr);
		spr->depth = OAM_DEPTH_FRONT;
		spr->spr.oam.pri = 0;
		scCursorPlace(&gridStore, grid->x * 8 - 4, grid->y * 8 + 12);
	}
	height = 1;
	drawColumn = 0;
	drawRow = 0;
	drawMode = 1;
	cursorX = win->x * 8 - 8;
	cursorY = win->y * 8 + 11;
	state = PASSWORD_STATE_ENTERING;
	for(;;) {
		action = PASSWORD_ACTION_NONE;
		if(state == PASSWORD_STATE_LAST_ENTERED)
			state = PASSWORD_STATE_CHECK;
		mnWindowSetTextTileRect(
			win, drawColumn, drawRow, drawMode, height, WINDOW_PALETTE);
		if(state == PASSWORD_STATE_ENTERING) {
			if(PerWork.repeat & KEY_RIGHT) {
				sySndPlay(SE_MENU_CURSOR_MOVE);
				{
					Sint32 next = column + 1;

					if(row == KEY_ROW_WITHOUT_SIDE_KEY)
						column = next % SIDE_COLUMN;
					else
						column = next % (SIDE_COLUMN + 1);
				}
			}
			if(PerWork.repeat & KEY_LEFT) {
				sySndPlay(SE_MENU_CURSOR_MOVE);
				if(row == KEY_ROW_WITHOUT_SIDE_KEY)
					column = (column + SIDE_COLUMN - 1) % SIDE_COLUMN;
				else
					column = (column + SIDE_COLUMN) % (SIDE_COLUMN + 1);
			}
			if(PerWork.repeat & KEY_DOWN) {
				sySndPlay(SE_MENU_CURSOR_MOVE);
				row = (row + 1) % KEY_ROWS;
				if(column == SIDE_COLUMN && row == KEY_ROW_WITHOUT_SIDE_KEY)
					row = KEY_ROW_FINISH;
			}
			if(PerWork.repeat & KEY_UP) {
				sySndPlay(SE_MENU_CURSOR_MOVE);
				row = (row + KEY_ROWS - 1) % KEY_ROWS;
				if(column == SIDE_COLUMN && row == KEY_ROW_WITHOUT_SIDE_KEY)
					row = KEY_ROW_RETURN;
			}
		}
		if(column == SIDE_COLUMN)
			drawMode = SIDE_HIGHLIGHT_WIDTH;
		else
			drawMode = 1;
		drawColumn = column;
		drawRow = row;
		/* The keys stand in groups of five, a blank tile between groups and
		   before the side column. */
		if(column > 4)
			drawColumn++;
		if(column > 9)
			drawColumn++;
		if(column > 14)
			drawColumn++;
		if(column > SIDE_COLUMN - 1)
			drawColumn++;
		mnWindowSetTextTileRect(
			win, drawColumn, row, drawMode, height, HIGHLIGHT_PALETTE);
		{
			Sint32 x = (win->x + column) * 8 - 8;
			Sint32 y = (win->y + row) * 8 + 11;

			if(column > 4)
				x = (win->x + column) * 8;
			if(column > 9)
				x += 8;
			if(column > 14)
				x += 8;
			if(CURSOR_ON_SIDE_COLUMN(column))
				x += 8;
			cursorX += (x - cursorX) / 2;
			cursorY += (y - cursorY) / 2;
		}
		scCursorShow(&cursorStore, cursorX, cursorY);
		{
			Sint32 metatile = cursor % PASSWORD_SYMBOLS_PER_PAGE;
			Sint32 y =
				(metatile / PASSWORD_SYMBOLS_PER_ROW * 2 + grid->y) * 8 - 2;
			Sint32 col;
			Sint32 x;

			frm = (FrameCount >> 1) & 7;
			col = metatile % PASSWORD_SYMBOLS_PER_ROW;
			x = col * 12 + grid->x * 8 + 16;

			if(col > 4)
				x += 10;
			x += ClearCursorBob[frm];
			y += ClearCursorBob[(frm + 5) & 7];
			scCursorShow(&gridStore, x, y);
		}
		if(advance > 0 && ++advance == 8) {
			advance = 0;
			cursor++;
		}
		if(advance == 0) {
			Uint16		 raw;
			INPUT_STATE *input = &PerWork;

			/* Space and Return repeat while A is held, as L and R do. */
			if(column == SIDE_COLUMN && row == KEY_ROW_SPACE)
				raw = input->repeat;
			else if(column == SIDE_COLUMN && row == KEY_ROW_RETURN)
				raw = input->repeat;
			else
				raw = input->push;
			if((raw & KEY_A) && state == PASSWORD_STATE_ENTERING) {
				action = PASSWORD_ACTION_SYMBOL;
				if(column == SIDE_COLUMN) {
					if(row == KEY_ROW_SPACE)
						action = PASSWORD_ACTION_SPACE;
					else if(row == KEY_ROW_RETURN)
						action = PASSWORD_ACTION_RETURN;
					else if(row == KEY_ROW_FINISH)
						action = PASSWORD_ACTION_FINISH;
				} else if(cursor < length) {
					Sint32 code = ClearPasswordGrid[column + row * SIDE_COLUMN];

					if(code <= PASSWORD_SYMBOL_MASK) {
						sySndPlay(SE_MENU_CONFIRM);
						ClearPasswordText[cursor] = code;
						/* DEAD: a remainder nothing reads; the ROM keeps the
						   call. */
						__modsi3(cursor, PASSWORD_SYMBOLS_PER_ROW);
						if(cursor < length - 1) {
							if(cursor % PASSWORD_SYMBOLS_PER_PAGE ==
								PASSWORD_SYMBOLS_PER_PAGE - 1)
								advance = 1;
							else
								cursor++;
						} else {
							state = PASSWORD_STATE_LAST_ENTERED;
						}
					}
				}
			}
			if(action != PASSWORD_ACTION_SYMBOL ||
				state == PASSWORD_STATE_CHECK) {
				if(PerWork.push & KEY_START ||
					action == PASSWORD_ACTION_FINISH ||
					state == PASSWORD_STATE_CHECK) {
					Uint16				check;
					WINDOW			   *box;
					MAIN_ROUTINE_STATE *party;
					Sint16				crc;
					Sint32				decodedLength;

					decodedLength = scClearDecodePassword(
						ClearPasswordText, length, ClearDecodedPassword);
					crcLength = decodedLength - PASSWORD_CRC_BYTES;
					check = scClearCalcPasswordCrc16(
						crcLength, ClearDecodedPassword);

					state = PASSWORD_STATE_ENTERING;
					box = mnWindowCreate(CHECK_WINDOW_X, 12, CHECK_WINDOW_WIDTH,
						3, WINDOW_FLAG_INSTANT);
					crc = check;
					if(ClearDecodedPassword[crcLength] == (Uint16)crc >> 8 &&
						ClearDecodedPassword[decodedLength - 1] ==
							(Uint16)crc % 256) {
						mnTextDrawMessageAsTiles(
							(Sint32)MsgFieldPasswordAccepted, box, 0, 0);
						sySndPlay(SE_MENU_CONFIRM);
						do {
							syWait(1);
						} while(!(PerWork.push & KEY_A));
						sySndPlay(SE_MENU_CONFIRM);
						gmMainRoutineStateInitDefaults();
						party = &MainRoutineStateWork;
						party->leaderActorId = ACTOR_ISAAC;
						gmPartyRemoveMember(FELIX);
						gmPartyRemoveMember(JENNA);
						gmPartyRemoveMember(SHEBA);
						gmPartyRemoveMember(PIERS);
						gmPartyAddMember(ISAAC);
						gmPartyAddMember(GARET);
						gmPartyAddMember(IVAN);
						gmPartyAddMember(MIA);
						scClearApplyImportedData(mode, ClearDecodedPassword);
						mnWindowClose(box, 2);
						mnWindowClose(grid, 2);
						mnWindowClose(win, 2);
						mnWindowColApply(party->winHue, party->winBrightness);
						sySndPlay(MUS_THE_ADVENTURE_RESUMES);
						syWait(6);
						mnNameEntryEditMemberName(0);
						mnStatScreenRun();
						/* DEAD: the flags and party copied where
						   RESTORE_SAVE_IMAGE copies the whole state next; the
						   ROM keeps the copy. */
						syWordsCopy(TRANSFERRED_FLAGS, GameFlagBytes,
							BUP_IMAGE_STATE_BYTES);
						return 0;
					}
					sySndPlay(SE_MENU_CANCEL);
					mnTextDrawMessageAsTiles(
						(Sint32)MsgFieldInvalidPassword, box, 0, 0);
					do {
						syWait(1);
					} while(!(PerWork.push & KEY_A) && !(PerWork.push & KEY_B));
					mnWindowClose(box, 2);
					mnWindowSprRedrawSavedRect(win);
					DRAW_KEYS(win)
				} else if(PerWork.repeat & KEY_L ||
					action == PASSWORD_ACTION_RETURN) {
					if(cursor > 0) {
						sySndPlay(SE_MENU_CURSOR_MOVE);
						cursor--;
					}
				} else if(PerWork.repeat & KEY_R ||
					action == PASSWORD_ACTION_SPACE) {
					if(cursor < length - 1) {
						sySndPlay(SE_MENU_CURSOR_MOVE);
						cursor++;
					}
				} else if(PerWork.push & KEY_B) {
					if(cursor > 0) {
						sySndPlay(SE_MENU_CURSOR_MOVE);
						cursor--;
					} else {
						sySndPlay(SE_MENU_CANCEL);
						mnWindowClose(grid, 2);
						mnWindowClose(win, 2);
						sySndPlay(MUS_THE_ADVENTURE_RESUMES);
						return -1;
					}
				}
			}
		}
		page = cursor / PASSWORD_SYMBOLS_PER_PAGE;
		mnWindowSprRedrawSavedRect(grid);
		{
			Sint32 y;

			for(i = 0, y = 2; i != 4; i++, y += 2)
				mnWindowDrawDividerLine(grid, 0, y, 18, y);
		}
		for(i = 0; i != pages; i++) {
			Sint32 tile = i + PAGE_NUMBER_TILE;

			if(i == page)
				tile = i + PAGE_NUMBER_LIT_TILE;
			mnWindowSetTilemapEntry(grid, tile, i - pages + 18, 10, 0);
		}
		if(length < PASSWORD_SYMBOLS_PER_PAGE)
			cnt = length;
		else if(pages - 1 == page)
			cnt = length % PASSWORD_SYMBOLS_PER_PAGE;
		else
			cnt = PASSWORD_SYMBOLS_PER_PAGE;
		for(i = 0; i != cnt; i++) {
			Uint8 *entered = ClearPasswordText;
			Sint32 idx = i + page * PASSWORD_SYMBOLS_PER_PAGE;
			Sint32 raw = entered[idx];
			Uint8  code = raw;

			if(code != passwordState->shown[i]) {
				passwordState->shown[i] = raw;
				if(code == PASSWORD_SYMBOL_EMPTY) {
					syWordsClearEntry(
						passwordState->metatiles[i], GLYPH_SPRITE_BYTES);
				} else {
					scClearGetPasswordCharacter(entered[idx], glyph);
					mnGlyphDrawToTiles(glyph[0], passwordState->metatiles[i]);
				}
			}
		}
		for(; i != PASSWORD_SYMBOLS_PER_PAGE; i++) {
			if(passwordState->shown[i] != PASSWORD_SYMBOL_EMPTY) {
				passwordState->shown[i] = PASSWORD_SYMBOL_EMPTY;
				syWordsClearEntry(
					passwordState->metatiles[i], GLYPH_SPRITE_BYTES);
			}
		}
		syWait(1);
	}
}

/* The grid of the window tilemap the two unused functions below fill: 16 by 8
   entries; the second's tiles start at TILE_GRID_FIRST_TILE. */
#define TILE_GRID_COLUMNS	 16
#define TILE_GRID_ROWS		 8
#define TILE_GRID_FIRST_TILE 32

/* Unused: unpacks the tiles into a scratch buffer, then fills a grid of the
   window tilemap, in VRAM and in its shadow, at the window's metatile: each
   metatile's tile the row times the window's width plus the column. */
UNUSED void scClearFillTileGrid(WINDOW *win, const void *compressedTiles)
{
	WIN_STATE *state = (WIN_STATE *)RamHeapSlots->winState;
	Uint16	  *shadow;
	Uint16	  *vram;
	void	  *decodedTiles;
	Sint32	   row;
	Sint32	   col;
	Sint32	   metatile;

	decodedTiles = syHeapAllocEWRAM(768);
	syDecompressTaggedLZ(compressedTiles, decodedTiles);
	metatile = win->y * BG_SCREEN_WIDTH + win->x;
	vram = (Uint16 *)WINDOW_TILEMAP_VRAM + metatile;
	shadow = &state->tilemap[0][metatile];
	for(row = 0; row < TILE_GRID_ROWS; row++) {
		for(col = 0; col < TILE_GRID_COLUMNS; col++) {
			Sint16 tile = WINDOW_TILE(win->width * row + col);

			*vram++ = tile;
			*shadow++ = tile;
		}
		vram += BG_SCREEN_WIDTH - TILE_GRID_COLUMNS;
		shadow += BG_SCREEN_WIDTH - TILE_GRID_COLUMNS;
	}
	syHeapRelease(decodedTiles);
}

/* Unused: fills a grid of the window tilemap with the tiles from
   TILE_GRID_FIRST_TILE on. */
UNUSED void scClearFillTileGridInOrder(WINDOW *win)
{
	WIN_STATE *state = (WIN_STATE *)RamHeapSlots->winState;
	Uint16	  *shadow;
	Uint16	  *vram;
	void	  *buf;
	Sint32	   row;
	Sint32	   col;
	Sint32	   metatile;

	buf = syHeapAllocEWRAM(768);
	metatile = win->y * BG_SCREEN_WIDTH + win->x;
	vram = (Uint16 *)WINDOW_TILEMAP_VRAM + metatile;
	shadow = &state->tilemap[0][metatile];
	for(row = 0; row < TILE_GRID_ROWS; row++) {
		for(col = 0; col < TILE_GRID_COLUMNS; col++) {
			Sint16 tile = WINDOW_TILE(
				row * TILE_GRID_COLUMNS + col + TILE_GRID_FIRST_TILE);

			*vram++ = tile;
			*shadow++ = tile;
		}
		vram += BG_SCREEN_WIDTH - TILE_GRID_COLUMNS;
		shadow += BG_SCREEN_WIDTH - TILE_GRID_COLUMNS;
	}
	syHeapRelease(buf);
}

extern Sint32 ClearImportedMembers[];
extern Uint16 ClearImportedKeyItems[];
extern Uint16 ClearImportedStackItems[];

/* Applies a decoded password of level mode (PASSWORD_LEVEL) to Isaac's party:
   the unpacking of The Broken Seal's FileScreenPackPasswordData. */
void scClearApplyImportedData(Sint32 mode, Uint8 *in)
{
	Uint32 levelBits;
	Uint32 djinnBits;
	Uint8  flgBits;
	Uint8  keyItemBits;
	Uint32 packedStats[8];
	Sint32 i;
	Sint32 j;
	Sint32 n;
	Sint32 m;
	Sint32 pos;
	Sint32 bit;

	/* Bytes 0 to 6 hold the four levels, seven bits each, then the 28 Djinn, a
	   bit each, sharing byte 3. */
	levelBits = in[0] | in[1] << 8 | in[2] << 16 | (in[3] & 0xf0) << 20;
	djinnBits = (in[3] & 0x0f) | in[4] << 4 | in[5] << 12 | in[6] << 20;
	flgBits = in[PASSWORD_FLAGS_BYTE];
	keyItemBits = in[PASSWORD_KEY_ITEMS_BYTE];

	/* Silver and Gold carry the stats rows. Each packs two party members in
	   PASSWORD_STATS_ROW_BYTES: each one's first word and the top 28 bits of
	   its second, whose low four bits are always 0. */
	if(mode != PASSWORD_LEVEL_BRONZE) {
		Sint32 statsOfs =
			PASSWORD_KEY_ITEMS_BYTE + (mode != PASSWORD_LEVEL_GOLD);

		for(i = 0; i != 2; i++) {
			packedStats[i * 4] = in[statsOfs] << 24 | in[statsOfs + 1] << 16 |
				in[statsOfs + 2] << 8 | in[statsOfs + 3];
			packedStats[i * 4 + 1] = in[statsOfs + 4] << 24 |
				in[statsOfs + 5] << 16 | in[statsOfs + 6] << 8 |
				in[statsOfs + 7];
			packedStats[i * 4 + 2] = in[statsOfs + 7] << 28 |
				in[statsOfs + 8] << 20 | in[statsOfs + 9] << 12 |
				in[statsOfs + 10] << 4 | in[statsOfs + 11] >> 4;
			packedStats[i * 4 + 3] = in[statsOfs + 11] << 28 |
				in[statsOfs + 12] << 20 | in[statsOfs + 13] << 12 |
				in[statsOfs + 14] << 4;
			statsOfs += PASSWORD_STATS_ROW_BYTES;
		}
		for(i = 0; i != 4; i++) {
			BATTLE_BASE_STATS *stats =
				(BATTLE_BASE_STATS *)&syUnitGet(ClearImportedMembers[i])
					->baseHp;
			Sint32 at = i * 8;

/* The word of the packed stats at byte at. */
#define WORD (*(Uint32 *)((Uint8 *)packedStats + at))
			stats->baseHp = WORD >> PASSWORD_HP_SHIFT;
			stats->basePp = WORD >> PASSWORD_PP_SHIFT & PASSWORD_PP_MASK;
			stats->baseAttack = WORD & PASSWORD_STAT_MASK;
			at += 4;
			stats->baseDefense =
				WORD >> PASSWORD_DEFENSE_SHIFT & PASSWORD_STAT_MASK;
			stats->baseAgility =
				WORD >> PASSWORD_AGILITY_SHIFT & PASSWORD_STAT_MASK;
			stats->baseLuck = WORD >> PASSWORD_LUCK_SHIFT & PASSWORD_LUCK_MASK;
#undef WORD
		}
	}

	for(i = 0; i != 6; i++) {
		if((flgBits >> i) & 1)
			syGameFlagSet(ClearImportedFlags[i]);
		else
			syGameFlagClear(ClearImportedFlags[i]);
	}

	for(i = 0; i != 4; i++) {
		UNIT *unit = syUnitGet(ClearImportedMembers[i]);

		*(Uint8 *)&unit->level = levelBits >> (i * 7) & PASSWORD_LEVEL_MASK;
#if PASSWORD_LEVEL_SETS_EXPERIENCE
		unit->experience = gmUnitGetExperienceForLevel(i, unit->level);
#endif
	}

	gmDjinnGetRecoveryTable(0)->list.cnt = 0;
	for(i = 0; i != 4; i++) {
		UNIT *unit = syUnitGet(ClearImportedMembers[i]);

		/* DEAD: the three clears after the first; the ROM keeps the loop. */
		for(n = 0; n != 4; n++)
			syWordsClearEntry(unit->djinnHeld, UNIT_DJINN_BYTES);
	}

	for(i = 0; i != ELEMENT_COUNT * TRANSFERRED_DJINN_PER_ELEMENT; i++) {
		Sint32 element = i / TRANSFERRED_DJINN_PER_ELEMENT;
		Sint32 number = i % TRANSFERRED_DJINN_PER_ELEMENT;

		syGameFlagClear(FLAG_DJINNI(element * DJINN_PER_ELEMENT + number));
	}

	for(i = 0; i != ELEMENT_COUNT * TRANSFERRED_DJINN_PER_ELEMENT; i++) {
		if((djinnBits >> i) & 1)
			gmDjinnGiveToParty(i / TRANSFERRED_DJINN_PER_ELEMENT,
				i % TRANSFERRED_DJINN_PER_ELEMENT);
	}

	if(mode != PASSWORD_LEVEL_GOLD) {
		syUnitGet(ClearImportedMembers[0]);
		for(i = 0; i != 8; i++) {
			if((keyItemBits >> i) & 1)
				gmPartyInventoryAdd(ClearImportedKeyItems[i]);
		}
	}

	if(mode == PASSWORD_LEVEL_GOLD) {
		pos = PASSWORD_ITEMS_BYTE;
		bit = 0;
		for(i = 0; i != 4; i++) {
			UNIT *unit = syUnitGet(ClearImportedMembers[i]);

			for(n = 0; n != INVENTORY_SLOTS; n++) {
				unit->inventory[n] =
					(in[pos] << (bit + 1) | in[pos + 1] >> (7 - bit)) &
					INVENTORY_ITEM_ID_MASK;
				bit++;
				pos++;
				if(bit == 7) {
					bit = 0;
					pos++;
				}
			}
		}

		pos = PASSWORD_QUANTITIES_BYTE;
		bit = -1;
		for(i = 0; i != 4; i++) {
			UNIT *unit = syUnitGet(ClearImportedMembers[i]);

			for(m = 0; m != PASSWORD_STACK_ITEMS; m++) {
				j = 0;
				if(bit < 0) {
					j = in[pos] << -bit & PASSWORD_QUANTITY_MASK;
					pos++;
					bit += 8;
				}
				j += in[pos] >> bit & PASSWORD_QUANTITY_MASK;
				bit -= 5;
				if(bit == -5) {
					pos++;
					bit = 3;
				}
				j &= PASSWORD_QUANTITY_MASK;
				for(n = 0; n != INVENTORY_SLOTS; n++) {
					if((unit->inventory[n] & INVENTORY_ITEM_ID_MASK) ==
						ClearImportedStackItems[m])
						unit->inventory[n] |= j << INVENTORY_QUANTITY_SHIFT;
				}
			}
		}

		MainRoutineStateWork.coins = in[PASSWORD_COINS_BYTE] << 16 |
			in[PASSWORD_COINS_BYTE + 1] << 8 | in[PASSWORD_COINS_BYTE + 2];
	}

	for(i = 0; i != 4; i++) {
		gmUnitUpdateClass(ClearImportedMembers[i]);
		gmUnitCalc(ClearImportedMembers[i]);
	}
}

extern Uint16 ClearPasswordFlags[];
extern Sint32 ClearPasswordMembers[];
extern Uint16 ClearPasswordKeyItems[];

/* Unused: packs the four heroes' data for a password of level mode (enum
   PASSWORD_LEVEL) as The Broken Seal's file screen does
   (FileScreenPackPasswordData), and returns the byte count. Nothing in this
   game calls it. */
UNUSED Sint32 scClearPackPasswordData(Sint32 unused, Sint32 mode, Uint8 *out)
{
	Sint32 length;
	Uint32 levelBits;
	Uint32 djinnBits;
	Uint8  keyItemBits;
	Uint8  flgBits;
	Uint32 packedStats[8];
	Sint32 i;
	Sint32 outputIdx;
	Sint32 bitOfs;
	Sint32 j;
	Sint32 k;
	Sint32 n;
	Sint32 m;

	length = PASSWORD_BRONZE_BYTES;
	switch(mode) {
	case PASSWORD_LEVEL_GOLD:
		length = PASSWORD_GOLD_BYTES - PASSWORD_CRC_BYTES;
		break;
	case PASSWORD_LEVEL_SILVER:
		length = PASSWORD_SILVER_BYTES - PASSWORD_CRC_BYTES;
		break;
	case PASSWORD_LEVEL_BRONZE:
		length = PASSWORD_BRONZE_BYTES - PASSWORD_CRC_BYTES;
		break;
	}
	for(i = 0; i != length; i++)
		out[i] = 0;
	levelBits = 0;
	djinnBits = 0;
	keyItemBits = 0;
	flgBits = 0;
	for(i = 0; i != 8; i++)
		packedStats[i] = 0;
	for(i = 0; i != 6; i++) {
		if(syGameFlagGet(ClearPasswordFlags[i]))
			flgBits |= 1 << i;
	}
	i = 0;
	outputIdx = 4;
	for(; i != 4; i++) {
		UNIT			  *unit = syUnitGet(ClearPasswordMembers[i]);
		BATTLE_BASE_STATS *base = (BATTLE_BASE_STATS *)&unit->baseHp;
		Sint32			   value;

		if(base->baseHp > UNIT_CHARACTER_HP_MAX)
			base->baseHp = UNIT_CHARACTER_HP_MAX;
		value = (Uint16)base->baseHp;
		if((Sint16)value < 0)
			base->baseHp = 0;
		if(base->basePp > UNIT_CHARACTER_HP_MAX)
			base->basePp = UNIT_CHARACTER_HP_MAX;
		value = (Uint16)base->basePp;
		if((Sint16)value < 0)
			base->basePp = 0;
		if(base->baseAttack > UNIT_STAT_MAX)
			base->baseAttack = UNIT_STAT_MAX;
		value = base->baseAttack;
		if(value < 0)
			value = 0;
		base->baseAttack = value;
		if(base->baseDefense > UNIT_STAT_MAX)
			base->baseDefense = UNIT_STAT_MAX;
		value = base->baseDefense;
		if(value < 0)
			value = 0;
		base->baseDefense = value;
		if(base->baseAgility > UNIT_STAT_MAX)
			base->baseAgility = UNIT_STAT_MAX;
		value = base->baseAgility;
		if(value < 0)
			value = 0;
		base->baseAgility = value;
		if(base->baseLuck > UNIT_LUCK_MAX)
			base->baseLuck = UNIT_LUCK_MAX;
		value = base->baseLuck;
		if(value < 0)
			value = 0;
		base->baseLuck = value;
		packedStats[i * 2] = (Uint32)base->baseHp << PASSWORD_HP_SHIFT |
			base->basePp << PASSWORD_PP_SHIFT | base->baseAttack;
		*(Uint32 *)((Uint8 *)packedStats + outputIdx) =
			(Uint32)base->baseDefense << PASSWORD_DEFENSE_SHIFT |
			base->baseAgility << PASSWORD_AGILITY_SHIFT |
			base->baseLuck << PASSWORD_LUCK_SHIFT;
		if(unit->level > UNIT_LEVEL_MAX)
			unit->level = UNIT_LEVEL_MAX;
		value = unit->level;
		if(value == 0)
			unit->level = 1;
		levelBits |= unit->level << (i * 7);
		for(j = 0; j != 4; j++)
			djinnBits += unit->djinnHeld[j] << (j * 7);
		/* Which of the eight Psynergy items (ClearPasswordKeyItems) any party
		   member carries. */
		for(j = 0; j != INVENTORY_SLOTS; j++) {
			Sint32 item = unit->inventory[j] & INVENTORY_ITEM_ID_MASK;

			for(k = 0; k != 8; k++) {
				if(item == ClearPasswordKeyItems[k])
					keyItemBits |= 1 << k;
			}
		}
		outputIdx += 8;
	}
	/* The largest password also carries inventories, stack counts and coins. */
	if(mode == PASSWORD_LEVEL_GOLD) {
		outputIdx = PASSWORD_ITEMS_BYTE;
		bitOfs = 0;
		for(i = 0; i != 4; i++) {
			UNIT *unit = syUnitGet(ClearPasswordMembers[i]);

			for(n = 0; n != INVENTORY_SLOTS; n++) {
				Uint16 item = unit->inventory[n] & INVENTORY_ITEM_ID_MASK;

				out[outputIdx] += item >> (bitOfs + 1);
				out[outputIdx + 1] += item << (7 - bitOfs);
				outputIdx++;
				bitOfs++;
				if(bitOfs == 7) {
					bitOfs = 0;
					outputIdx++;
				}
			}
		}
		outputIdx = PASSWORD_QUANTITIES_BYTE;
		bitOfs = -1;
		for(i = 0; i != 4; i++) {
			UNIT *unit = syUnitGet(ClearPasswordMembers[i]);

			for(m = 0; m != PASSWORD_STACK_ITEMS; m++) {
				Sint32 cnt = 0;
				Uint16 quantity;
				/* FAKEMATCH(dead): the form kept; as no quantity copy. */

				for(n = 0; n != INVENTORY_SLOTS; n++) {
					if((unit->inventory[n] & INVENTORY_ITEM_ID_MASK) ==
						ClearPasswordStackItems[m])
						cnt = unit->inventory[n] >> INVENTORY_QUANTITY_SHIFT;
				}
				quantity = cnt;
				if(bitOfs < 0) {
					out[outputIdx] += (Uint16)cnt >> -bitOfs;
					bitOfs += 8;
					outputIdx++;
				}
				out[outputIdx] += (Uint16)cnt << bitOfs;
				bitOfs -= 5;
				if(bitOfs == -5) {
					outputIdx++;
					bitOfs = 3;
				}
			}
		}
		out[PASSWORD_COINS_BYTE] = MainRoutineStateWork.coins >> 16;
		out[PASSWORD_COINS_BYTE + 1] = MainRoutineStateWork.coins >> 8;
		out[PASSWORD_COINS_BYTE + 2] = MainRoutineStateWork.coins;
	}
	/* Silver and Gold carry the stats rows. Each packs two party members in
	   PASSWORD_STATS_ROW_BYTES: each one's first word and the top 28 bits of
	   its second, whose low four bits are always 0. */
	if(mode != PASSWORD_LEVEL_BRONZE) {
		Sint32 statsOfs =
			PASSWORD_KEY_ITEMS_BYTE + (mode != PASSWORD_LEVEL_GOLD);

		for(i = 0; i != 2; i++) {
			out[statsOfs] = packedStats[i * 4] >> 24;
			out[statsOfs + 1] = packedStats[i * 4] >> 16;
			out[statsOfs + 2] = packedStats[i * 4] >> 8;
			out[statsOfs + 3] = packedStats[i * 4];
			out[statsOfs + 4] = packedStats[i * 4 + 1] >> 24;
			out[statsOfs + 5] = packedStats[i * 4 + 1] >> 16;
			out[statsOfs + 6] = packedStats[i * 4 + 1] >> 8;
			out[statsOfs + 7] = packedStats[i * 4 + 1];
			out[statsOfs + 7] |= packedStats[i * 4 + 2] >> 28;
			out[statsOfs + 8] = packedStats[i * 4 + 2] >> 20;
			out[statsOfs + 9] = packedStats[i * 4 + 2] >> 12;
			out[statsOfs + 10] = packedStats[i * 4 + 2] >> 4;
			out[statsOfs + 11] = packedStats[i * 4 + 2] << 4;
			out[statsOfs + 11] |= packedStats[i * 4 + 3] >> 28;
			out[statsOfs + 12] = packedStats[i * 4 + 3] >> 20;
			out[statsOfs + 13] = packedStats[i * 4 + 3] >> 12;
			out[statsOfs + 14] = packedStats[i * 4 + 3] >> 4;
			statsOfs += PASSWORD_STATS_ROW_BYTES;
		}
	}
	out[0] = levelBits;
	out[1] = levelBits >> 8;
	out[2] = levelBits >> 16;
	out[3] = (levelBits >> 20 & 0xf0) | (djinnBits & 0x0f);
	out[4] = djinnBits >> 4;
	out[5] = djinnBits >> 12;
	out[6] = djinnBits >> 20;
	out[PASSWORD_FLAGS_BYTE] = flgBits;
	if(mode != PASSWORD_LEVEL_GOLD)
		out[PASSWORD_KEY_ITEMS_BYTE] = keyItemBits;
	return length;
}

Sint32 scClearDecodePassword(Uint8 *src, Sint32 length, Uint8 *dst)
{
	Uint8  saved[PASSWORD_GOLD_LENGTH];
	Sint32 i;
	Sint32 shift = 2;
	Sint32 k = 0;
	Sint32 n = 0;
	Sint32 size = PASSWORD_BRONZE_BYTES;
	Uint8  last;
	Uint8  c;

	if(length != PASSWORD_BRONZE_LENGTH) {
		size = PASSWORD_SILVER_BYTES;
		if(length != PASSWORD_SILVER_LENGTH)
			size = PASSWORD_GOLD_BYTES;
	}
	for(i = 0; i != size; i++)
		dst[i] = 0;
	for(i = 0; i != length; i++) {
		saved[i] = src[i];
		src[i] = (saved[i] - i) & PASSWORD_SYMBOL_MASK;
	}

	for(i = 0; i != length; i++, k++) {
		/* Every tenth symbol is the checksum of the nine before it. */
		if(i % PASSWORD_SYMBOLS_PER_ROW > 8)
			continue;
		c = src[k];
		if(shift < 0) {
			dst[n] += c >> -shift;
			n++;
			if(n == size)
				break;
			shift += 8;
		}
		dst[n] += c << shift;
		shift -= 6;
		if(shift == -6) {
			shift = 2;
			n++;
			if(n == size)
				break;
		}
	}
	last = dst[n - 1];
	for(i = 0; i != n - 1; i++)
		dst[i] ^= last;
	for(i = 0; i != length; i++)
		src[i] = saved[i];
	return n;
}

/* The password's checksum, as The Broken Seal's
   FileScreenCalculatePasswordCrc16 makes it: over length bytes at p, shifted a
   bit at a time from PASSWORD_CRC_SEED, PASSWORD_CRC_POLYNOMIAL subtracted when
   the top bit falls out, and the result complemented. The password screen
   checks it against a decoded password's last PASSWORD_CRC_BYTES. */
Uint16 scClearCalcPasswordCrc16(Uint32 length, Uint8 *p)
{
	Uint32 crc;
	Uint32 i;
	Uint32 j;

	crc = PASSWORD_CRC_SEED;
	i = 0;
	if(length != 0) {
		do {
			crc ^= *p << 8;
			j = 0;
			do {
				Uint32 high = crc & PASSWORD_CRC_TOP_BIT;
				Uint32 next = crc << 1;

				j++;
				crc = next - PASSWORD_CRC_POLYNOMIAL;
				if(high == 0)
					crc = next;
			} while(j != 8);
			i++;
			p++;
		} while(i != length);
	}
	return ~crc;
}

/* clang-format off */
/* The first secret code, as The Broken Seal's, ended by 0: Select three */
/* times. With it a new game names Jenna, Sheba and Piers as well as Felix */
/* (scInit). */
Uint16 ClearButtonCode[] = {
	KEY_SELECT, KEY_SELECT, KEY_SELECT, 0,
};

/* The second, longer code. With it a new game also names Garet, Ivan and */
/* Mia. */
Uint16 ClearExtraButtonCode[] = {
	KEY_UP, KEY_DOWN, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_LEFT, KEY_RIGHT,
	KEY_UP, KEY_RIGHT, KEY_DOWN, KEY_LEFT, KEY_UP, KEY_SELECT, 0,
#if EXTRA_BUTTON_CODE_PADDING
	0,
#endif
};

/* The grid cursor's bobbing offsets in pixels, which scClearEnterPassword */
/* adds to its x at step (FrameCount >> 1) & 7 and to its y five steps on, */
/* so that it circles. */
Sint8 ClearCursorBob[] = {
	0, 1, 2, 3, 3, 2, 1, 0,
};

#if (LANGUAGE != LANGUAGE_JAPANESE)
/* The password grid's four rows of symbols as DRAW_KEYS writes them, five to */
/* a group (the Japanese grid is kana, MsgFieldPasswordKana). */
Uint8 ClearFirstPasswordRow[] = "ABCDE FGHJK LMNPQ R";
Uint8 ClearSecondPasswordRow[] = "STUVW XYZ23 45678 9";
Uint8 ClearThirdPasswordRow[] = "abcde fghij kmnpq r";
Uint8 ClearFourthPasswordRow[] = "stuvw xyz!? #&$%+ =";
#endif

/* The six story flags of a password's PASSWORD_FLAGS_BYTE, by The Broken */
/* Seal's numbers, which gmDataTransferApply reads them by. */
Uint16 ClearImportedFlags[] = {
	FLAG_BROKEN_SEAL_DODONPA_LOCKED_UP,
	FLAG_BROKEN_SEAL_WON_COLOSSO,
	FLAG_BROKEN_SEAL_HSU_RESCUED_WITHOUT_PARTY,
	FLAG_BROKEN_SEAL_CROSSBONE_ISLE_DEADBEARD_DEFEATED,
	FLAG_BROKEN_SEAL_VISITED_SICK_DORA,
	FLAG_BROKEN_SEAL_VAULT_WARNED_OF_ESCAPED_THIEVES,
};
/* clang-format on */

#include "shared/scene_entrances_mapless.c"

Uint32 SceneExits[] = {
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	{SCENE_TABLE_END},
};

CUE SceneCues[] = {
	{CUE_TABLE_END},
};

Uint16 ClearBlendTick[] = {
	0,
};

Sint16 ClearCodeUnlocked = 0;

Sint16 ClearExtraCodeUnlocked = 0;

Sint16 ClearCodeProgress = 0;

Sint16 ClearExtraCodeProgress = 0;

Sint16 ClearCodeHeld = 0;

Sint16 ClearExtraCodeHeld = 0;

#undef YEAH_WIDTH
#undef COPY_SAVE_IMAGE
#undef BUP_IMAGE_BYTES
#undef KEEP_SAVE_IMAGE
#undef RESTORE_SAVE_IMAGE
#undef RETURN_TO_SAVED
#undef TRANSFERRED_FLAGS
#undef TRANSFERRED_MAIN_ROUTINE_STATE
#undef TRANSFERRED_SAVE
#undef LINK_LANGUAGE_TAG
#undef RECEIVE_WINDOW_X
#undef RECEIVE_WINDOW_WIDTH
#undef STATUS_WINDOW_X
#undef STATUS_WINDOW_WIDTH
#undef SIDE_COLUMN
#undef SIDE_HIGHLIGHT_WIDTH
#undef CHECK_WINDOW_X
#undef CHECK_WINDOW_WIDTH
#undef CURSOR_ON_SIDE_COLUMN
#undef DRAW_KEYS
#undef TILE_GRID_COLUMNS
#undef TILE_GRID_ROWS
#undef TILE_GRID_FIRST_TILE
#undef WORD

/* The four whose data a Broken Seal password brings in, in its order. */
/* clang-format off */
LATE_DATA Sint32 ClearImportedMembers[] = {
	ISAAC, GARET, IVAN, MIA,
};

/* The key items a Broken Seal password gives Isaac's party, in its order. */
LATE_DATA Uint16 ClearImportedKeyItems[] = {
	ITEM_ORB_OF_FORCE, ITEM_DOUSE_DROP, ITEM_FROST_JEWEL, ITEM_LIFTING_GEM,
	ITEM_HALT_GEM, ITEM_CLOAK_BALL, ITEM_CARRY_STONE, ITEM_CATCH_BEADS,
};

/* The items a Broken Seal password gives a count of, in its order. */
LATE_DATA Uint16 ClearImportedStackItems[] = {
	ITEM_HERB, ITEM_NUT, ITEM_VIAL, ITEM_POTION, ITEM_PSY_CRYSTAL,
	ITEM_ANTIDOTE, ITEM_ELIXIR, ITEM_WATER_OF_LIFE, ITEM_POWER_BREAD,
	ITEM_COOKIE, ITEM_APPLE, ITEM_HARD_NUT, ITEM_MINT, ITEM_LUCKY_PEPPER,
	ITEM_SMOKE_BOMB, ITEM_SLEEP_BOMB, ITEM_GAME_TICKET, ITEM_LUCKY_MEDAL,
	ITEM_SACRED_FEATHER, ITEM_OIL_DROP, ITEM_WEASELS_CLAW, ITEM_BRAMBLE_SEED,
	ITEM_CRYSTAL_POWDER, ITEM_NONE,
};

/* The same list as ClearImportedMembers, under another name. */
LATE_DATA Sint32 ClearPasswordMembers[] = {
	ISAAC, GARET, IVAN, MIA,
};

/* The same flags, as the unused scClearPackPasswordData reads them. */
LATE_DATA Uint16 ClearPasswordFlags[] = {
	FLAG_BROKEN_SEAL_DODONPA_LOCKED_UP,
	FLAG_BROKEN_SEAL_WON_COLOSSO,
	FLAG_BROKEN_SEAL_HSU_RESCUED_WITHOUT_PARTY,
	FLAG_BROKEN_SEAL_CROSSBONE_ISLE_DEADBEARD_DEFEATED,
	FLAG_BROKEN_SEAL_VISITED_SICK_DORA,
	FLAG_BROKEN_SEAL_VAULT_WARNED_OF_ESCAPED_THIEVES,
};

/* The same list as ClearImportedKeyItems, under another name. */
LATE_DATA Uint16 ClearPasswordKeyItems[] = {
	ITEM_ORB_OF_FORCE, ITEM_DOUSE_DROP, ITEM_FROST_JEWEL, ITEM_LIFTING_GEM,
	ITEM_HALT_GEM, ITEM_CLOAK_BALL, ITEM_CARRY_STONE, ITEM_CATCH_BEADS,
};

/* The same list as ClearImportedStackItems, under another name. */
LATE_DATA Uint16 ClearPasswordStackItems[] = {
	ITEM_HERB, ITEM_NUT, ITEM_VIAL, ITEM_POTION, ITEM_PSY_CRYSTAL,
	ITEM_ANTIDOTE, ITEM_ELIXIR, ITEM_WATER_OF_LIFE, ITEM_POWER_BREAD,
	ITEM_COOKIE, ITEM_APPLE, ITEM_HARD_NUT, ITEM_MINT, ITEM_LUCKY_PEPPER,
	ITEM_SMOKE_BOMB, ITEM_SLEEP_BOMB, ITEM_GAME_TICKET, ITEM_LUCKY_MEDAL,
	ITEM_SACRED_FEATHER, ITEM_OIL_DROP, ITEM_WEASELS_CLAW, ITEM_BRAMBLE_SEED,
	ITEM_CRYSTAL_POWDER,
};
/* clang-format on */
