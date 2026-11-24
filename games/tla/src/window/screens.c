/// @file
/// The screens object: the screens built on the windows, from the window
/// sprites, the party status window and the file list to the battle menus,
/// the icon menus and the Pause menu, as in The Broken Seal's screens.c. Its
/// code calls several window routines with no declaration in scope, as The
/// Broken Seal's does.
///
/// window_internal.h holds what both objects share; the code is in the
/// screens_*.c files under window/, included here in the order it has in the
/// ROM.

#include "window_internal.h"
#include "resource_ids.h"
#include "system/system.h"
#include "field/field_state.h"
#include "flags.h"
#include "sounds.h"
#include "field/particle.h"
#include "window/menu_list.h"
#include "fixed_math.h"
#include "window/settings_menu.h"
#include "scene_ids.h"
#include "item_ids.h"
#include "game/character_ids.h"
#include "field/scene_tables.h"

/// A bounce of 32 steps, 8.8 fixed-point scales: the selected icons pulse by
/// it.
extern const u16 gWindowSprite_BounceScales[];
/// The portraits of the characters and of the other speakers, {who, portrait}
/// pairs ending in -1.
extern const s16 gPortrait_CharacterMap[];
extern const s16 gPortrait_SpriteMap[];

#ifdef MODERN
// The screens object calls the window sprite list's routines and
// Oam_AddSpriteAtDepth with no declaration in scope, as its bytes need; a
// current GCC gets them here.
#include "graphics/output.h"
#endif

/// One icon of the command row (struct BattleCommandRowState): its sprite,
/// where it stands and the command it stands for. screens_menu_titles.c loads
/// it, screens_battle_command_row.c runs the row.
struct BattleCommandRowIcon {
  /// The icon as a 32x32 sprite, drawn only while it is selected (pulsing);
  /// the others show as tiles of BG1.
  struct MenuSprite sprite;
  /// resource: the VRAM cache entry its sprite's tiles go into, one 1 KB
  /// entry that every icon of the row shares, reloaded with the selected
  /// icon. unused is never read.
  u16 resource, unused;
  /// Screen pixels: the sprite's top left before the pulse moves it.
  s32 positionX, positionY;
  s32 kind; ///< its command: MsgCommandName + kind names it
};

/// The pulsing icon's transform: its scale, 8.8 fixed point, and angle.
struct CommandRowAffine {
  u16 scaleX;
  u16 scaleY;
  u16 angle;
};

/// The command row's state: the icons' shared palette, up to six icons,
/// the label window, the pulsing icon's transform and the selection.
struct BattleCommandRowState {
  /// BG palette slots for the icons' 256-colour pixels, filled as each icon
  /// is loaded (BattleCommandRow_LoadIconTiles).
  struct GlyphPalette palette;
  struct BattleCommandRowIcon entries[6];
  void *window;
  s32 iconCount;
  struct CommandRowAffine affine;
  s32 affineIndex;
  s32 selectedIndex;
  s32 drawnIndex;
  s32 commandKinds[6];
};

/// A sprite as the summary screen queues it (Oam_AddSpriteAtDepth): the depth
/// queue's link, then its OAM attributes as words or as fields.
struct SummaryOam {
  u8 link[4];
  union {
    struct {
      u32 attr01;
      u32 attr23;
    } word;
    struct {
      u8 y;
      u8 attr0high;
      u16 x : 9;
      u16 affineIndex : 5;
      u16 size : 2;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
      u16 high;
    } f;
  } oam;
};

/// Where the Status screen's cursor points, in pixels.
struct SummaryPoint {
  s32 x;
  s32 y;
};

/// The Status screen's cursor: its place in each group, the help entry it
/// shows and where it points.
struct StatusCursor {
  s8 groupPositions[8];
  s32 helpEntry;
  s32 drawnHelpEntry;
  s32 group;
  s32 groupSize;
  s32 column;
  s32 row;
};

/// The name entry screen's cursors: the hand on the keyboard and the caret
/// under the name, each a window sprite moved as a MenuCursor.
union NameCursor {
  struct WindowSprite *sprite;
  struct MenuCursor cursor;
};

/// The menus' state (heap slot HEAP_SLOT_MENU) as the file list reads it: the
/// party row of the shop's ShopState (shop/shop_internal.h), holding the party
/// sprites of the file under the cursor, where each stands and its scale.
struct FileListState {
  u8 menuScreens[248]; ///< the windows and lists before the party row
  /// Up to four party sprites, null for none; the row has room for eight.
  void *partyMemberIcons[8];
  s16 partyMemberX[8];       ///< pixels
  s16 partyMemberY[8];       ///< pixels
  FIXED partyMemberScale[4]; ///< 16.16 fixed point, FIXED_ONE full size
};

/// The battle menus' message buffers, in halfwords, and the copier that fills
/// them: the Japanese edition decodes the message (Message_DecodeRaw), the
/// others copy its text (Message_CopyExpanded).
#if LANGUAGE != LANGUAGE_JAPANESE
#define BATTLE_MENU_MESSAGE_CAPACITY 52
#define BATTLE_MENU_MESSAGE_COPY Message_CopyExpanded
#else
#define BATTLE_MENU_MESSAGE_CAPACITY 32
#define BATTLE_MENU_MESSAGE_COPY Message_DecodeRaw
#endif

/// The two BG tiles of the Djinn menu's arrow: DJINN_MENU_ARROW_TILE blinks,
/// the one after it stays (gDjinnMenu_BlinkTile).
#define DJINN_MENU_ARROW_TILE 808

/// The arrow tile BattleDjinnMenu_Run copies to the two BG tiles from
/// DJINN_MENU_ARROW_TILE each frame: the first blinks (four frames shown, four
/// blank), the second stays.
extern const u8 gDjinnMenu_BlinkTile[32];

/// Where each element's icon and standby count sit in the party status
/// window's Djinn column, in tiles, for the Djinn menu to blink them.
extern u8 gDjinnMenu_ElementXOffsets[4];
extern u8 gDjinnMenu_ElementYOffsets[4];

/// The Status screen's cursor stops: the stats, the equipment and the
/// conditions.
extern const struct BattleSummaryStatCursor gOwnerStatus_StatCursors[];
extern const struct BattleSummaryCursor gOwnerStatus_EquipmentCursors[];
extern const struct BattleSummaryCursor gOwnerStatus_ConditionCursors[];
/// "Lv", before the level on the Status screen.
extern const u8 gOwnerStatus_LevelString[];

/// The ability of each summon, by its number.
extern const u16 gSummon_Abilities[];

/// The heading pair's bob (MenuTitle_Bob), in pixels, a step every four
/// VBlanks.
extern const u8 gMenuTitle_BobOffsets[];

/// The rows the range icons are marked with (Menu_BuildLocalizedPatternTiles).
extern const struct BarPatternRow gMenu_BarPatternRows[];

/// The standby Djinn counts 10 to 18, one tile each, from BG tile
/// DJINN_COUNT_TENS_TILE: the party status window loads them
/// (gPartyStatus_DigitTiles) when a count passes 9.
extern const u8 gPartyStatus_DigitTiles[];
/// That tile, and its place in VRAM.
#define DJINN_COUNT_TENS_TILE 416
#define DJINN_COUNT_TENS_VRAM                                                  \
  ((void *)(BG_CHAR_BLOCK(0) + DJINN_COUNT_TENS_TILE * TILE_SIZE_4BPP))
/// The window tilemap entry of standby count `n`: a digit, or a tile from
/// DJINN_COUNT_TENS_TILE for 10 on.
#define DJINN_COUNT_TILE(n)                                                    \
  ((n) > 9 ? (n) + WINDOW_TILE(DJINN_COUNT_TENS_TILE - 10)                     \
           : WINDOW_TILE((n) + '0'))

/// The saving screen's progress bar: its seven stages, a row of tile numbers
/// each.
extern const u8 gWindow_ProgressBarRows[];
/// The progress bar's tiles, compressed.
extern const u8 gWindow_ProgressBarTiles[];

/// The ARM routine that reads the windows' edges off the UI tilemap for each
/// scan line (cell_tiles.c), copied to RAM before it runs.
extern u8 Tile_BuildCellTilesCode[];

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
#include "screens_window_sprites.c"
#include "screens_party_status.c"
#include "screens_save.c"
#include "screens_name_entry.c"
#include "screens_joined.c"
#include "screens_menu_titles.c"
#include "screens_battle_command_row.c"
#include "screens_battle_djinn_preview.c"
#include "screens_battle_status.c"
#include "screens_battle_djinn_menu.c"
#include "screens_battle_summon_menu.c"
#include "screens_battle_item_menu.c"
#include "screens_battle_psynergy_menu.c"

#undef MenuCursor_SetPositionImmediateFar

/// A unit's place projected onto the screen, as the target cursor reads it.
struct TargetPoint {
  s32 x;
  s32 y;
  s32 z;
};

/// One of the target cursor's markers: where it stands, whether it shows and
/// which listed unit it marks.
struct TargetTrack {
  u8 x;
  u8 y;
  u8 flags;
  s8 index;
};

/// One line's horizontal window edges, each a WIN_RANGE value.
struct WindowLine {
  u16 window0; ///< for WIN0H
  u16 window1; ///< for WIN1H
};

/// A page of the icon menu's window shape: one WindowLine for each of the
/// screen's 160 lines.
struct WindowBuffer {
  u8 winInOut[4]; ///< WININ and WINOUT, not touched here
  struct WindowLine lines[160];
};

/// The debug icon browser's headings and page label.
extern const u8 gIconBrowser_ItemTitle[];
extern const u8 gIconBrowser_EnergyTitle[];
extern const u8 gIconBrowser_StatusTitle[];
extern const u8 gIconBrowser_PageLabel[];
extern const u8 gIconBrowser_RangeDash[];

/// The File Screen's icon menu: the action at each icon position.
extern s8 gMenu_SaveSlotActionByPosition[];

// The parts, in ROM order.
#include "screens_battle_targets.c"
#include "screens_link_timer.c"
#include "screens_battle_party_commands.c"
#include "screens_icon_menu.c"
#include "screens_debug_destinations.c"
#include "screens_debug_flags.c"
#include "screens_pause_menu.c"
