#ifndef ALCHEMY_WINDOW_SCREENS_H
#define ALCHEMY_WINDOW_SCREENS_H

/* @file */
/* The screens object's functions and tables that code outside it uses: the */
/* Status screen's cursor stops, the file list and File Screen choices, the */
/* battle menus, the menu headings and the Yes/No menu. */

#include "fixed_math.h"
#include "types.h"
#include "window/cursor.h"

struct WINDOW;
struct WINDOW_SPR;

/* A cursor stop among the Status screen's stats: its help entry, where the */
/* cursor points and the run of the row it highlights. */
typedef struct BATTLE_SUMMARY_STAT_CURSOR {
	u8 entry;
	u8 column;
	u8 row;
	u8 highlightColumn;
	u8 highlightRow;
	u8 highlightWidth;
	u8 reserved[2]; /* 0 in every row */
} BATTLE_SUMMARY_STAT_CURSOR;

/* A cursor stop on the Status screen's equipment or statuses: its help */
/* entry and where the cursor points. */
typedef struct BATTLE_SUMMARY_CURSOR {
	u8 entry;
	u8 column;
	u8 row;
	u8 reserved; /* 0 in every row */
} BATTLE_SUMMARY_CURSOR;

/* A row of Menu_BarPatternRows: the two words a menu bar's tile pair is */
/* XORed with (mnRangeIconBuildTiles). */
typedef struct BAR_PATTERN_ROW {
	u32 firstTile;
	u32 secondTile;
} BAR_PATTERN_ROW;

/* One word of a tile XORed with a row's mask: the marks */
/* mnBuildPatternTiles and mnRangeIconBuildTiles make on a range */
/* icon's two tiles. */
static __inline__ u32 XorWord(u32 word, u32 mask) { return word ^ mask; }

/* The twelve Psynergy range icons, built in two sets of six */
/* (mnBuildPatternTiles for the Status menu, */
/* mnRangeIconBuildTiles for the battle menu): range n takes icon */
/* n - 1, and ABILITY_RANGE_ALL the last, RANGE_ICON_ALL. */
#define RANGE_PATTERN_SET_COUNT 2
#define RANGE_PATTERN_COUNT		6
#define RANGE_ICON_ALL			(RANGE_PATTERN_SET_COUNT * RANGE_PATTERN_COUNT - 1)

s32 mnCommandIconReloadSlot(s32 slot, s32 index);
void mnBattleMenuUpdateEachFrame(void);
s32 mnBattleCommandRowRun(s32 mode, s32 fleeAllowed);
s32 mnBattleTargetRunSelection(s32 selection, s32 mode, u32 spread, u32 kind);
s32 mnBattleStatScrRun(u16 *list, s32 listCount, s32 member);
s32 mnBattleRefuseFlee(void);
s32 mnBattlePsynergyMenuRun(s32 unitId, u16 *actions, s32 count);
s32 mnBattleSummonMenuRun(s32 unusedA, s32 unusedB, const u8 *standby);
s32 mnBattleItemMenuRun(s32 member, u16 *entries, s32 count);
s32 mnItemCheckBattleUse(s32 member, s32 item);
s32 mnItemIconLoadIntoSlot(s32 item, s32 slot);
void mnIconMenuDrawIcons(void);
s32 mnLinkCreateCountdownLabelWindow(void);
/* Shows the advance cursor bobbing at (x, y) and waits for A, B, L or R. */
void mnBattleMessageWaitForAdvanceAtStub(s32 x, s32 y);
/* Sets the BGM volume target (BGM_UNITY_SCALE full) and its step a VBlank. */
void sySndSetBgmVolumeTargetStub(s32 target, s32 step);
/* Plays the music scQueueMusic remembered, if any. */
void scPlayQueuedMusicStub(void);
/* Saves up to 32 actors into SnapshotObjects so that they can be put back. */
void fdActorTableSnapshotStub(void);
/* Creates the four Djinn figures of the file list's top row. */
void mnDjinnFiguresInitTopRowStub(void);
/* Destroys the four Djinn figures and stops placing them. */
void mnDjinnFiguresReleaseTopRowStub(void);
/* Shows the Djinn figures (nonzero) or hides them (0). */
void mnDjinnFiguresSetShownStub(s32 shown);
/* Moves Djinn figure `index` to screen pixel (x, y), if it exists. */
void mnDjinnFiguresSetSlotPosStub(s32 index, s32 x, s32 y);
s32 mnFileListSelectFile(s32 selectedFile, s32 menuMode);
void mnTitleBob(struct WINDOW_SPR *object);
/* The 64x16 headings of MenuTitlePictures that mnTitleCreate shows, in */
/* their order: the file menu's, in FILE_MENU_MODE's order, then the Pause */
/* and Settings headings. */
typedef enum MENU_TITLE_PICTURE {
	MENU_TITLE_SAVE,
	MENU_TITLE_CONTINUE,
	MENU_TITLE_COPY,
	MENU_TITLE_ERASE,
	MENU_TITLE_BATTLE,
	MENU_TITLE_TRANSFER,
	MENU_TITLE_UPDATE,
	MENU_TITLE_PAUSE,
	MENU_TITLE_SETTINGS
} MENU_TITLE_PICTURE;

struct WINDOW_SPR *mnTitleCreate(s32 picture, struct WINDOW *window, s32 x,
								 s32 y);
void mnWindowSprPulse(struct WINDOW_SPR *obj);

/* The file menu's choices, as mnFileListSelectAction returns them */
/* (Menu_SaveSlotActionByPosition). */
typedef enum FILE_ACTION {
	SAVE_ACTION_NEW_GAME = 0,
	SAVE_ACTION_CONTINUE = 1,
	SAVE_ACTION_COPY = 2,
	SAVE_ACTION_ERASE = 3,
	SAVE_ACTION_BATTLE = 4,
	SAVE_ACTION_UPDATE = 5
} FILE_ACTION;

/* The bytes of the menu state, heap block HEAP_SLOT_MENU: SHOP_STATE's size, */
/* the largest of the screens' views of it. Each screen's run function takes */
/* the block as it opens and releases it as it closes. */
#define MENU_STATE_SIZE 3308

/* The OAM entry of a window sprite, as the Djinn preview places its stat */
/* arrows (mnDjinnMenuDrawStatArrow): from the output's byte 16 on. */
typedef struct PREVIEW_SPR {
	u32 link; /* the output's depth-queue link (WINDOW_SPR.link) */
	/* OAM attributes 0 and 1, written whole as 0x40000400 first. */
	union {
		u32 word;
		struct {
			unsigned y : 8; /* screen pixels */
			unsigned
				flg : 8; /* attribute 0's high byte: 0x04, semi-transparent */
			unsigned x : 9;	   /* screen pixels */
			unsigned high : 7; /* attribute 1's flip and size bits: 16 by 16 */
		} bits;
	} attributes;
	/* OAM attribute 2: the arrow's VRAM tile. */
	union {
		u32 value;
		struct {
			unsigned idx : 10; /* the tile in object VRAM */
			unsigned high : 6; /* priority and palette, 0 here */
		} bits;
	} tile;
} PREVIEW_SPR;

/* The field menu's commands, as mnFieldMenuSelectCommand takes and returns */
/* them (Menu_TopEntryCommandByPosition). */
typedef enum FIELD_MENU_COMMAND {
	FIELD_MENU_PSYNERGY = 1,
	FIELD_MENU_DJINN = 2,
	FIELD_MENU_ITEM = 3,
	FIELD_MENU_STATUS = 4
} FIELD_MENU_COMMAND;

/* What the file list is choosing a file for (mnFileListSelectFile, and */
/* mnSaveChooseAndLoadFile for the File Screen), which decides the files the */
/* cursor may stop on. */
typedef enum FILE_MENU_MODE {
	FILE_LIST_SAVE,		/* saving: any file, empty ones too */
	FILE_LIST_CONTINUE, /* continuing: files not yet cleared */
	FILE_LIST_COPY,		/* copying: any file with data */
	FILE_LIST_ERASE,	/* erasing: any file with data */
	/* The title's Battle: files that have found a Djinni. */
	FILE_LIST_BATTLE,
	FILE_LIST_CLEARED, /* the title's Send: cleared files only */
	/* The title's Update: files saved before FLAG_REUNION was set. */
	FILE_LIST_UPDATE
} FILE_MENU_MODE;

/* The places of the transfer method menu. */
typedef enum TRANSFER_METHOD {
	TRANSFER_METHOD_PASSWORD = 0,
	TRANSFER_METHOD_CABLE = 1
} TRANSFER_METHOD;

/* The places of the password level menu: how much of the game a password */
/* carries. */
typedef enum PASSWORD_LEVEL {
	PASSWORD_LEVEL_GOLD = 0,   /* levels, Djinn, stats, items and coins */
	PASSWORD_LEVEL_SILVER = 1, /* levels, Djinn and stats */
	PASSWORD_LEVEL_BRONZE = 2  /* levels and Djinn */
} PASSWORD_LEVEL;

/* What mnIconMenuChooseSendOption offers. */
typedef enum SEND_OPTION_MENU {
	SEND_OPTION_TRANSFER_METHOD = 0, /* Password or Cable */
	SEND_OPTION_PASSWORD_LEVEL = 1	 /* Gold, Silver or Bronze */
} SEND_OPTION_MENU;

/* What mnYesNoMenuRun and mnYesNoMenuRunAt return. */
typedef enum YES_NO { YES = 0, NO = 1 } YES_NO;

/* A status the Status screen shows as an icon, which picks both the icon */
/* and its help (MsgNormalStatus + condition). */
typedef enum BATTLE_SUMMARY_STAT {
	OWNER_CONDITION_EMPTY = -1, /* a place with no icon */
	OWNER_CONDITION_NORMAL = 0, /* no status: "Normal status." */
	OWNER_CONDITION_POISON = 1,
	OWNER_CONDITION_DEADLY_POISON = 2,
	OWNER_CONDITION_STUN = 3,
	OWNER_CONDITION_PSYNERGY_SEALED = 4,
	OWNER_CONDITION_SLEEP = 5,
	OWNER_CONDITION_DELUSION = 6,
	OWNER_CONDITION_HAUNT = 7,
	OWNER_CONDITION_GRIM_REAPER = 8, /* downed when its turns run out */
	OWNER_CONDITION_ATTACK_UP = 9,
	OWNER_CONDITION_ATTACK_DOWN = 10,
	OWNER_CONDITION_DEFENSE_UP = 11,
	OWNER_CONDITION_DEFENSE_DOWN = 12,
	OWNER_CONDITION_RESISTANCE_UP = 13,
	OWNER_CONDITION_RESISTANCE_DOWN = 14,
	OWNER_CONDITION_CURSED_ITEM = 15,
	OWNER_CONDITION_DOWNED = 16,
	OWNER_CONDITION_AGILITY_UP = 17,
	OWNER_CONDITION_AGILITY_DOWN = 18
} BATTLE_SUMMARY_STAT;

/* The two tiles of the link screen's page arrows, which mnPageArrowsDraw */
/* shifts and merges into the window border each frame. */
extern u8 PageArrowTiles[];
/* Merges two 4bpp tiles row by row into `output`: `second` moved `shift` */
/* pixels, drawn over `first`. */
void mnGraphicsMergeShiftedTileRows(u32 *first, u32 *second, u32 *output,
									s32 shift);
/* The file menu's 64x16 headings by number, each drawn as two 32x16 sprites */
/* (mnTitleLoad; graphics/ui/menu_titles.s). */
extern s32 MenuTitlePictures[];
/* The task that makes the file list's highlight glow. */
void mnFileListUpdatePulse(void);
/* The eight scales mnWindowSprPulse pulses the settings menu's selected icon */
/* through. */
extern const FIXED WindowSprite_PulseScales[];
/* Eight offsets, 0 to 3 and back, that make the name entry screen's caret */
/* circle: the x offset leads the y offset by five steps. */
extern const s8 NameEntry_CaretWave[];
/* The name entry screen keyboard's compressed tilemap, the Spanish edition's */
/* with its own letters. */
extern const u8 NameEntry_KeyboardTilemap[];

#endif
