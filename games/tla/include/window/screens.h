#ifndef ALCHEMY_WINDOW_SCREENS_H
#define ALCHEMY_WINDOW_SCREENS_H

/// @file
/// The screens object's routines and tables that code outside it uses: the
/// Status screen's cursor stops, the file list and File Screen choices, the
/// battle menus, the menu headings and the Yes/No menu.

#include "fixed_math.h"
#include "types.h"
#include "window/cursor.h"

struct Window;
struct WindowSprite;

/// A cursor stop among the Status screen's stats: its help entry, where the
/// cursor points and the run of the row it highlights.
struct BattleSummaryStatCursor {
  u8 entry;
  u8 column;
  u8 row;
  u8 highlightColumn;
  u8 highlightRow;
  u8 highlightWidth;
  u8 reserved[2]; ///< 0 in every row
};

/// A cursor stop on the Status screen's equipment or statuses: its help
/// entry and where the cursor points.
struct BattleSummaryCursor {
  u8 entry;
  u8 column;
  u8 row;
  u8 reserved; ///< 0 in every row
};

/// A row of gMenu_BarPatternRows: the two words a menu bar's tile pair is
/// XORed with (RangeIcon_BuildTiles).
struct BarPatternRow {
  u32 firstTile;
  u32 secondTile;
};

/// One word of a tile XORed with a row's mask: the marks
/// Menu_BuildPatternTiles and RangeIcon_BuildTiles make on a range
/// icon's two tiles.
static __inline__ u32 XorWord(u32 word, u32 mask) { return word ^ mask; }

/// The twelve Psynergy range icons, built in two sets of six
/// (Menu_BuildPatternTiles for the Status menu,
/// RangeIcon_BuildTiles for the battle menu): range n takes icon
/// n - 1, and ABILITY_RANGE_ALL the last, RANGE_ICON_ALL.
#define RANGE_PATTERN_SET_COUNT 2
#define RANGE_PATTERN_COUNT 6
#define RANGE_ICON_ALL (RANGE_PATTERN_SET_COUNT * RANGE_PATTERN_COUNT - 1)

void PartyStatus_CreateWindow(s32 flags);
s32 DebugBrowser_Icons(void);
s32 DebugBrowser_Portraits(void);
s32 YesNoMenu_Run(s32 bottom, s32 message, s32 labelWidth, s32 selection);
s32 CommandIcon_ReloadSlot(s32 slot, s32 index);
void BattleMenu_UpdateEachFrame(void);
s32 BattleCommandRow_Run(s32 mode, s32 fleeAllowed);
s32 BattleTarget_RunSelection(s32 selection, s32 mode, u32 spread, u32 kind);
s32 BattleStatusScreen_Run(u16 *list, s32 listCount, s32 member);
s32 Battle_RefuseFlee(void);
s32 BattlePsynergyMenu_Run(s32 unitId, u16 *actions, s32 count);
s32 BattleSummonMenu_Run(s32 unusedA, s32 unusedB, const u8 *standby);
s32 BattleItemMenu_Run(s32 member, u16 *entries, s32 count);
s32 Item_CheckBattleUse(s32 member, s32 item);
s32 ItemIcon_LoadIntoSlot(s32 item, s32 slot);
void IconMenu_DrawIcons(void);
void PageArrows_Draw(s32 destination);
s32 Link_CreateCountdownLabelWindow(void);
/// Shows the advance cursor bobbing at (x, y) and waits for A, B, L or R.
void BattleMessage_WaitForAdvanceAtFar(s32 x, s32 y);
/// Sets the BGM volume target (BGM_UNITY_SCALE full) and its step a VBlank.
void Sound_SetBgmVolumeTargetFar(s32 target, s32 step);
/// Plays the music Scene_QueueMusic remembered, if any.
void Scene_PlayQueuedMusicFar(void);
/// Saves up to 32 actors into gSnapshotObjects so that they can be put back.
void ActorTable_SnapshotFar(void);
/// Creates the four Djinn figures of the file list's top row.
void DjinnFigures_InitializeTopRowFar(void);
/// Destroys the four Djinn figures and stops placing them.
void DjinnFigures_ReleaseTopRowFar(void);
/// Shows the Djinn figures (nonzero) or hides them (0).
void DjinnFigures_SetShownFar(s32 shown);
/// Moves Djinn figure `index` to screen pixel (x, y), if it exists.
void DjinnFigures_SetSlotPositionFar(s32 index, s32 x, s32 y);
s32 FileList_SelectFile(s32 selectedFile, s32 menuMode);
void MenuTitle_Bob(struct WindowSprite *object);
struct WindowSprite *Portrait_Create(s32 speaker, s32 option, s32 side,
                                     struct Window *window, s32 x, s32 y);
/// The 64x16 headings of gMenuTitlePictures that MenuTitle_Create shows, in
/// their order: the file menu's, in FileMenuMode's order, then the Pause
/// and Settings headings.
enum MenuTitlePicture {
  MENU_TITLE_SAVE,
  MENU_TITLE_CONTINUE,
  MENU_TITLE_COPY,
  MENU_TITLE_ERASE,
  MENU_TITLE_BATTLE,
  MENU_TITLE_TRANSFER,
  MENU_TITLE_UPDATE,
  MENU_TITLE_PAUSE,
  MENU_TITLE_SETTINGS
};

struct WindowSprite *MenuTitle_Create(s32 picture, struct Window *window, s32 x,
                                      s32 y);
void WindowSprite_Pulse(struct WindowSprite *obj);

/// The file menu's choices, as FileList_SelectAction returns them
/// (gMenu_SaveSlotActionByPosition).
enum FileAction {
  SAVE_ACTION_NEW_GAME = 0,
  SAVE_ACTION_CONTINUE = 1,
  SAVE_ACTION_COPY = 2,
  SAVE_ACTION_ERASE = 3,
  SAVE_ACTION_BATTLE = 4,
  SAVE_ACTION_UPDATE = 5
};

s32 FileList_SelectAction(void);

/// The bytes of the menu state, heap block HEAP_SLOT_MENU: ShopState's size,
/// the largest of the screens' views of it. Each screen's run function takes
/// the block as it opens and releases it as it closes.
#define MENU_STATE_SIZE 3308

/// The OAM entry of a window sprite, as the Djinn preview places its stat
/// arrows (DjinnMenu_DrawStatArrow): from the output's byte 16 on.
struct PreviewSprite {
  u32 link; ///< the output's depth-queue link (WindowSprite.link)
  /// OAM attributes 0 and 1, written whole as 0x40000400 first.
  union {
    u32 word;
    struct {
      unsigned y : 8;     ///< screen pixels
      unsigned flags : 8; ///< attribute 0's high byte: 0x04, semi-transparent
      unsigned x : 9;     ///< screen pixels
      unsigned high : 7;  ///< attribute 1's flip and size bits: 16 by 16
    } bits;
  } attributes;
  /// OAM attribute 2: the arrow's VRAM tile.
  union {
    u32 value;
    struct {
      unsigned index : 10; ///< the tile in object VRAM
      unsigned high : 6;   ///< priority and palette, 0 here
    } bits;
  } tile;
};

/// The field menu's commands, as FieldMenu_SelectCommand takes and returns them
/// (gMenu_TopEntryCommandByPosition).
enum FieldMenuCommand {
  FIELD_MENU_PSYNERGY = 1,
  FIELD_MENU_DJINN = 2,
  FIELD_MENU_ITEM = 3,
  FIELD_MENU_STATUS = 4
};

/// What the file list is choosing a file for (FileList_SelectFile, and
/// Save_ChooseAndLoadFile for the File Screen), which decides the files the
/// cursor may stop on.
enum FileMenuMode {
  FILE_LIST_SAVE,     ///< saving: any file, empty ones too
  FILE_LIST_CONTINUE, ///< continuing: files not yet cleared
  FILE_LIST_COPY,     ///< copying: any file with data
  FILE_LIST_ERASE,    ///< erasing: any file with data
  /// The title's Battle: files that have found a Djinni.
  FILE_LIST_BATTLE,
  FILE_LIST_CLEARED, ///< the title's Send: cleared files only
  /// The title's Update: files saved before FLAG_REUNION was set.
  FILE_LIST_UPDATE
};

/// The places of the transfer method menu.
enum TransferMethod { TRANSFER_METHOD_PASSWORD = 0, TRANSFER_METHOD_CABLE = 1 };

/// The places of the password level menu: how much of the game a password
/// carries.
enum PasswordLevel {
  PASSWORD_LEVEL_GOLD = 0,   ///< levels, Djinn, stats, items and coins
  PASSWORD_LEVEL_SILVER = 1, ///< levels, Djinn and stats
  PASSWORD_LEVEL_BRONZE = 2  ///< levels and Djinn
};

/// What IconMenu_ChooseSendOption offers.
enum SendOptionMenu {
  SEND_OPTION_TRANSFER_METHOD = 0, ///< Password or Cable
  SEND_OPTION_PASSWORD_LEVEL = 1   ///< Gold, Silver or Bronze
};

/// What YesNoMenu_Run and YesNoMenu_RunAt return.
enum YesNo { YES = 0, NO = 1 };

/// A status the Status screen shows as an icon, which picks both the icon
/// and its help (MsgNormalStatus + condition).
enum BattleSummaryStatus {
  OWNER_CONDITION_EMPTY = -1, ///< a place with no icon
  OWNER_CONDITION_NORMAL = 0, ///< no status: "Normal status."
  OWNER_CONDITION_POISON = 1,
  OWNER_CONDITION_DEADLY_POISON = 2,
  OWNER_CONDITION_STUN = 3,
  OWNER_CONDITION_PSYNERGY_SEALED = 4,
  OWNER_CONDITION_SLEEP = 5,
  OWNER_CONDITION_DELUSION = 6,
  OWNER_CONDITION_HAUNT = 7,
  OWNER_CONDITION_GRIM_REAPER = 8, ///< downed when its turns run out
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
};

/// The two tiles of the link screen's page arrows, which PageArrows_Draw
/// shifts and merges into the window border each frame.
extern u8 gPageArrowTiles[];
/// Merges two 4bpp tiles row by row into `output`: `second` moved `shift`
/// pixels, drawn over `first`.
void Graphics_MergeShiftedTileRows(u32 *first, u32 *second, u32 *output,
                                   s32 shift);
/// The file menu's 64x16 headings by number, each drawn as two 32x16 sprites
/// (MenuTitle_Load; graphics/ui/menu_titles.s).
extern s32 gMenuTitlePictures[];
/// The task that makes the file list's highlight glow.
void FileList_UpdatePulse(void);
/// The eight scales WindowSprite_Pulse pulses the settings menu's selected icon
/// through.
extern const FIXED gWindowSprite_PulseScales[];
/// Eight offsets, 0 to 3 and back, that make the name entry screen's caret
/// circle: the x offset leads the y offset by five steps.
extern const s8 gNameEntry_CaretWave[];
/// The name entry screen keyboard's compressed tilemap, the Spanish edition's
/// with its own letters.
extern const u8 gNameEntry_KeyboardTilemap[];

#endif
