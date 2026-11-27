#ifndef ALCHEMY_WINDOW_SCREENS_H
#define ALCHEMY_WINDOW_SCREENS_H

// The screens object's functions and tables that code outside it uses: the Status screen's cursor
// stops, the file list and File Screen choices, the battle menus, the menu headings and the Yes/No
// menu.

#include "fixed_math.h"
#include "types.h"
#include "window/cursor.h"

struct WINDOW;
struct WINDOW_SPR;

// A cursor stop among the Status screen's stats: its help entry, where the cursor points and the
// run of the row it highlights.
typedef struct BATTLE_SUMMARY_STAT_CURSOR
{
    Uint8 entry;
    Uint8 column;
    Uint8 row;
    Uint8 highlightColumn;
    Uint8 highlightRow;
    Uint8 highlightWidth;
    Uint8 reserved[2]; // 0 in every row
} BATTLE_SUMMARY_STAT_CURSOR;

// A cursor stop on the Status screen's equipment or statuses: its help entry and where the cursor
// points.
typedef struct BATTLE_SUMMARY_CURSOR
{
    Uint8 entry;
    Uint8 column;
    Uint8 row;
    Uint8 reserved; // 0 in every row
} BATTLE_SUMMARY_CURSOR;

// A row of MenuBarPatternRows: the two words a menu bar's tile pair is XORed with
// (mnRangeIconBuildTiles).
typedef struct BAR_PATTERN_ROW
{
    Uint32 firstTile;
    Uint32 secondTile;
} BAR_PATTERN_ROW;

// One word of a tile XORed with a row's mask: the marks mnBuildPatternTiles and
// mnRangeIconBuildTiles make on a range icon's two tiles.
static __inline__ Uint32 mnXorWord(Uint32 word, Uint32 mask)
{
    return word ^ mask;
}

// The twelve Psynergy range icons, built in two sets of six (mnBuildPatternTiles for the Status
// menu, mnRangeIconBuildTiles for the battle menu): range n takes icon n - 1, and ABILITY_RANGE_ALL
// the last, RANGE_ICON_ALL.
#define RANGE_PATTERN_SET_COUNT 2
#define RANGE_PATTERN_COUNT     6
#define RANGE_ICON_ALL          (RANGE_PATTERN_SET_COUNT * RANGE_PATTERN_COUNT - 1)

Sint32 mnCommandIconReloadSlot(Sint32 slot, Sint32 index);
void   mnBattleMenuUpdateEachTick(void);
Sint32 mnBattleCommandRowRun(Sint32 mode, Sint32 fleeAllowed);
Sint32 mnBattleTargetRunSelection(Sint32 selection, Sint32 mode, Uint32 spread, Uint32 kind);
Sint32 mnBattleStatusScreenRun(Uint16 *list, Sint32 listCount, Sint32 member);
Sint32 mnBattleRefuseFlee(void);
Sint32 mnBattlePsynergyMenuRun(Sint32 unitId, Uint16 *actions, Sint32 count);
Sint32 mnBattleSummonMenuRun(Sint32 unusedA, Sint32 unusedB, const Uint8 *standby);
Sint32 mnBattleItemMenuRun(Sint32 member, Uint16 *entries, Sint32 count);
Sint32 mnItemCheckBattleUse(Sint32 member, Sint32 item);
Sint32 mnItemIconLoadIntoSlot(Sint32 item, Sint32 slot);
void   mnIconMenuDrawIcons(void);
Sint32 mnLinkCreateCountdownLabelWindow(void);
// Shows the advance cursor bobbing at (x, y) and waits for A, B, L or R.
void   btMessageWaitForAdvanceAtStub(Sint32 x, Sint32 y);
// Sets the BGM volume target (BGM_UNITY_SCALE full) and its step a frame.
void   sySoundSetBGMVolumeTargetStub(Sint32 target, Sint32 step);
// Plays the music fdQueueMusic remembered, if any.
void   fdPlayQueuedMusicStub(void);
// Saves up to 32 actors into SnapshotObjects so that they can be put back.
void   fdActorTableSnapshotStub(void);
// Creates the four Djinn figures of the file list's top row.
void   mnDjinnFiguresInitTopRowStub(void);
// Destroys the four Djinn figures and stops placing them.
void   mnDjinnFiguresReleaseTopRowStub(void);
// Shows the Djinn figures (nonzero) or hides them (0).
void   mnDjinnFiguresSetShownStub(Sint32 shown);
// Moves Djinn figure `index` to screen pixel (x, y), if it exists.
void   mnDjinnFiguresSetSlotPosStub(Sint32 index, Sint32 x, Sint32 y);
Sint32 mnFileListSelectFile(Sint32 selectedFile, Sint32 menuMode);
void   mnTitleBob(struct WINDOW_SPR *object);

// The 64x16 headings of MenuTitlePictures that mnTitleCreate shows, in their order: the file
// menu's, in FILE_MENU_MODE's order, then the Pause and Settings headings.
typedef enum MENU_TITLE_PICTURE
{
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

struct WINDOW_SPR *mnTitleCreate(Sint32 picture, struct WINDOW *window, Sint32 x, Sint32 y);
void               mnWindowSpritePulse(struct WINDOW_SPR *object);

// The file menu's choices, as mnFileListSelectAction returns them (MenuSaveSlotActionByPosition).
typedef enum FILE_ACTION
{
    SAVE_ACTION_NEW_GAME = 0,
    SAVE_ACTION_CONTINUE = 1,
    SAVE_ACTION_COPY     = 2,
    SAVE_ACTION_ERASE    = 3,
    SAVE_ACTION_BATTLE   = 4,
    SAVE_ACTION_UPDATE   = 5
} FILE_ACTION;

// The bytes of the menu state, heap block HEAP_SLOT_MENU: SHOP_STATE's size, the largest of the
// screens' views of it. Each screen's run function takes the block as it opens and releases it as
// it closes.
#define MENU_STATE_SIZE 3308

// The OAM entry of a window sprite, as the Djinn preview places its stat arrows
// (mnDjinnMenuDrawStatArrow): from the output's byte 16 on.
typedef struct PREVIEW_SPR
{
    Uint32 link; // the output's depth-queue link (WINDOW_SPR.link)

                 // OAM attributes 0 and 1, written whole as 0x40000400 first.
    union
    {
        Uint32 word;

        struct
        {
            unsigned y     : 8; // screen pixels
            // attribute 0's high byte: 0x04, semi-transparent
            unsigned flags : 8;
            unsigned x     : 9; // screen pixels
            unsigned high  : 7; // attribute 1's flip and size bits: 16 by 16
        } bits;
    } attributes;

    // OAM attribute 2: the arrow's VRAM tile.
    union
    {
        Uint32 value;

        struct
        {
            unsigned index : 10; // the tile in object VRAM
            unsigned high  : 6;  // priority and palette, 0 here
        } bits;
    } tile;
} PREVIEW_SPR;

// The field menu's commands, as mnFieldMenuSelectCommand takes and returns them
// (MenuTopEntryCommandByPosition).
typedef enum FIELD_MENU_COMMAND
{
    FIELD_MENU_PSYNERGY = 1,
    FIELD_MENU_DJINN    = 2,
    FIELD_MENU_ITEM     = 3,
    FIELD_MENU_STATUS   = 4
} FIELD_MENU_COMMAND;

// What the file list is choosing a file for (mnFileListSelectFile, and mnSaveChooseAndLoadFile for
// the File Screen), which decides the files the cursor may stop on.
typedef enum FILE_MENU_MODE
{
    FILE_LIST_SAVE,     // saving: any file, empty ones too
    FILE_LIST_CONTINUE, // continuing: files not yet cleared
    FILE_LIST_COPY,     // copying: any file with data
    FILE_LIST_ERASE,    // erasing: any file with data
    // The title's Battle: files that have found a Djinni.
    FILE_LIST_BATTLE,
    FILE_LIST_CLEARED, // the title's Send: cleared files only
    // The title's Update: files saved before FLAG_REUNION was set.
    FILE_LIST_UPDATE
} FILE_MENU_MODE;

// The places of the transfer method menu.
typedef enum TRANSFER_METHOD
{
    TRANSFER_METHOD_PASSWORD = 0,
    TRANSFER_METHOD_CABLE    = 1
} TRANSFER_METHOD;

// The places of the password level menu: how much of the game a password carries.
typedef enum PASSWORD_LEVEL
{
    PASSWORD_LEVEL_GOLD   = 0, // levels, Djinn, stats, items and coins
    PASSWORD_LEVEL_SILVER = 1, // levels, Djinn and stats
    PASSWORD_LEVEL_BRONZE = 2  // levels and Djinn
} PASSWORD_LEVEL;

// What mnIconMenuChooseSendOption offers.
typedef enum SEND_OPTION_MENU
{
    SEND_OPTION_TRANSFER_METHOD = 0, // Password or Cable
    SEND_OPTION_PASSWORD_LEVEL  = 1  // Gold, Silver or Bronze
} SEND_OPTION_MENU;

// What mnYesNoMenuRun and mnYesNoMenuRunAt return.
typedef enum YES_NO
{
    YES = 0,
    NO  = 1
} YES_NO;

// A status the Status screen shows as an icon, which picks both the icon and its help
// (MsgNormalStatus + condition).
typedef enum BATTLE_SUMMARY_STAT
{
    OWNER_CONDITION_EMPTY           = -1, // a place with no icon
    OWNER_CONDITION_NORMAL          = 0,  // no status: "Normal status."
    OWNER_CONDITION_POISON          = 1,
    OWNER_CONDITION_DEADLY_POISON   = 2,
    OWNER_CONDITION_STUN            = 3,
    OWNER_CONDITION_PSYNERGY_SEALED = 4,
    OWNER_CONDITION_SLEEP           = 5,
    OWNER_CONDITION_DELUSION        = 6,
    OWNER_CONDITION_HAUNT           = 7,
    OWNER_CONDITION_GRIM_REAPER     = 8, // downed when its turns run out
    OWNER_CONDITION_ATTACK_UP       = 9,
    OWNER_CONDITION_ATTACK_DOWN     = 10,
    OWNER_CONDITION_DEFENSE_UP      = 11,
    OWNER_CONDITION_DEFENSE_DOWN    = 12,
    OWNER_CONDITION_RESISTANCE_UP   = 13,
    OWNER_CONDITION_RESISTANCE_DOWN = 14,
    OWNER_CONDITION_CURSED_ITEM     = 15,
    OWNER_CONDITION_DOWNED          = 16,
    OWNER_CONDITION_AGILITY_UP      = 17,
    OWNER_CONDITION_AGILITY_DOWN    = 18
} BATTLE_SUMMARY_STAT;

// The two tiles of the link screen's page arrows, which mnPageArrowsDraw shifts and merges into the
// window border each tick.
extern Uint8 PageArrowTiles[];
// Merges two 4bpp tiles row by row into `output`: `second` moved `shift` pixels, drawn over
// `first`.
void mnGraphicsMergeShiftedTileRows(Uint32 *first, Uint32 *second, Uint32 *output, Sint32 shift);
// The file menu's 64x16 headings by number, each drawn as two 32x16 sprites (mnTitleLoad;
// graphics/ui/menu_titles.s).
extern Sint32      MenuTitlePictures[];
// The event that makes the file list's highlight glow.
void               mnFileListUpdatePulse(void);
// The eight scales mnWindowSpritePulse pulses the settings menu's selected icon through.
extern const FIXED WindowSpritePulseScales[];
// Eight offsets, 0 to 3 and back, that make the name entry screen's caret circle: the x offset
// leads the y offset by five steps.
extern const Sint8 NameEntryCaretWave[];
// The name entry screen keyboard's compressed tilemap, the Spanish edition's with its own letters.
extern const Uint8 NameEntryKeyboardTilemap[];

#endif
