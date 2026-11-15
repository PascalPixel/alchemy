/// @file
/// The window object: the windows drawn into a shared BG tilemap, the messages
/// typed into them and the decoder behind them, the sprites windows carry,
/// icons, the selection screen, the settings and Pause menus, the debug menus
/// and text drawn as tiles; then the screens built on them, which The Broken
/// Seal links as an object of its own (screens.c), from the window sprites to
/// the Pause menu.
///
/// window_internal.h holds what every part shares (its includes and the
/// declarations the parts disagreed on); the code is in the files under
/// window/, included here in the order it has in the ROM. Camelot's own files
/// are not recoverable: the ROM shows one object, and these parts are ours,
/// cut where The Broken Seal's are. debug.c holds the debug menus, which The
/// Lost Age keeps after the settings and Pause menus, where The
/// Broken Seal keeps them in its screens object (screens/debug.c). Where to
/// start:
///
///   Windows         windows.c: Window_Create, Window_Close;
///                   borders.c: Window_DrawBorder, Window_UpdateZooms
///   Messages        borders.c: Message_OpenWindow, FieldMessage_Show;
///                   typing.c: Message_UpdateAll; decode.c: Message_Decode
///   Text drawing    sprites.c: WindowSprite_UpdateAll; text_tiles.c:
///                   Text_DrawStringAsTiles, Text_DrawString
///   Screens         selection.c: FieldMenu_Run; debug.c:
///                   DebugWarp_Run

#include "window_internal.h"

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
#include "window_windows.c"
#include "window_typing.c"
#include "window_borders.c"
#include "window_glyphs.c"
#include "window_decode.c"
#include "window_measure.c"
#include "window_sprites.c"
#include "window_messages.c"
#include "window_icons.c"
#include "window_selection.c"
#include "window_shortcuts.c"
#include "window_settings_menu.c"
#include "window_debug.c"
#include "window_text_tiles.c"

#undef Ability_Get
#undef Portrait_Create
#undef CommandIcon_Create
#undef Event_SetDestinationFar

/* VIEW(screens): routine Text_DrawPrefixedNumberAtOffset as int
 * Text_DrawPrefixedNumberAtOffset(), called with no declaration in scope;
 * another part has its declaration; tried as the definition's void
 * Text_DrawPrefixedNumberAtOffset(s32 value, struct RenderInput *work, s32
 * offsetX, s32 offsetY, s32 variant), PartyStatus_Draw moves: 1 of 495
 * instructions in tla-ja. */
s32 Text_DrawPrefixedNumberAtOffset__screens() __asm__(
    "Text_DrawPrefixedNumberAtOffset");
#define Text_DrawPrefixedNumberAtOffset Text_DrawPrefixedNumberAtOffset__screens
/* VIEW(screens): routine Text_DrawWideStringInWindow as int
 * Text_DrawWideStringInWindow(), called with no declaration in scope; another
 * part has its declaration; tried as the one declaration s32
 * Text_DrawWideStringInWindow(), BattleDjinnMenu_ShowChangePreview moves: 6 of
 * 684 instructions in tla-ja. */
s32 Text_DrawWideStringInWindow__screens() __asm__(
    "Text_DrawWideStringInWindow");
#define Text_DrawWideStringInWindow Text_DrawWideStringInWindow__screens
/* VIEW(screens): routine Window_MapTextCanvasTiles as int
 * Window_MapTextCanvasTiles(), called with no declaration in scope; another
 * part has its declaration; tried as the definition's void
 * Window_MapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height, s32
 * fullWidth), BattleStatusScreen_Run moves: 3 of 3110 instructions in tla-ja.
 */
s32 Window_MapTextCanvasTiles__screens() __asm__("Window_MapTextCanvasTiles");
#define Window_MapTextCanvasTiles Window_MapTextCanvasTiles__screens

/// A sprite of the bobbing pair of the menu titles: its render slot link, its
/// resting y and its y this frame.
struct BobPairObject;

/// One icon of the command row (struct BattleCommandRowWork): its sprite, where
/// it stands and the command it stands for. menu_titles.c loads it,
/// battle_command_row.c runs the row.
struct BattleCommandRowIcon {
  u8 unk00[4];
  u8 y;
  u8 objMode : 2; ///< 3: double-size affine
  u8 high05 : 6;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u8 unk08[4];
  u16 resource;
  u16 unused;
  s32 positionX;
  s32 positionY;
  s32 kind;
};

/// The pulsing icon's transform: its scale, 8.8 fixed point, and angle.
struct CommandRowAffine {
  u16 scaleX;
  u16 scaleY;
  u16 angle;
};

/// The command row's work block: the icons' shared palette, up to six icons,
/// the label window, the pulsing icon's transform and the selection.
struct BattleCommandRowWork {
  u8 paletteSlots[256];
  s32 paletteCount;
  struct BattleCommandRowIcon entries[6];
  void *window;
  s32 iconCount;
  struct CommandRowAffine affine;
  s32 affineIndex;
  s32 selectedIndex;
  s32 drawnIndex;
  s32 commandKinds[6];
};

/// A unit as the summary screen reads it.
struct SummaryOam {
  u8 unk00[4];
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
      u16 unk0a;
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

/// A cursor stop among the Status screen's stats: its help entry, where the
/// cursor points and the run of the row it highlights.
struct BattleSummaryStatCursor {
  u8 entry;
  u8 column;
  u8 row;
  u8 highlightColumn;
  u8 highlightRow;
  u8 highlightWidth;
  u8 unk06[2];
};

/// A cursor stop on the Status screen's equipment or statuses: its help
/// entry and where the cursor points.
struct BattleSummaryCursor {
  u8 entry;
  u8 column;
  u8 row;
  u8 unk03;
};

/// A VRAM cache entry as the Status screen reads its tile offset.
struct SummaryResourceEntry {
  u16 unk00;
  u16 offset;
};

struct BobPairObject {
  struct BobPairObject *next;
  u8 unk04[4];
  u8 baseY;
  u8 unk09[11];
  u8 y;
};

/// A row of gMenu_BarPatternRows: the two words a menu bar's tile pair is
/// XORed with (Menu_BuildLocalizedPatternTiles).
struct BarPatternRow {
  u32 firstTile;
  u32 secondTile;
};

/// The name entry screen's cursors: the hand on the keyboard and the caret
/// under the name, each a window sprite moved as a NameCursorMotion.
union NameCursor {
  struct WindowSprite *sprite;
  struct NameCursorMotion cursor;
};

/// The first two words of gMainRoutineWork as the save writes them: the build
/// stamp and the play time.
struct SaveGameWords {
  u32 buildStamp;
  u32 playTime;
};

/// The menu work as the file list reads it, ShopWork's party row: the party
/// sprites of the file under the cursor, where each stands and its scale.
struct FileListWork {
  u8 unk000[0xf8];
  void *partyMemberIcons[4]; ///< up to four party sprites, null for none
  u8 unk108[0x10];
  s16 partyMemberX[8];     ///< pixels
  s16 partyMemberY[8];     ///< pixels
  s32 partyMemberScale[4]; ///< 16.16 fixed point, Q16_ONE full size
};

extern const u8 gDjinnMenu_BlinkTile[32];

extern u8 gDjinnMenu_ElementXOffsets[4];

extern u8 gDjinnMenu_ElementYOffsets[4];

extern const struct BattleSummaryStatCursor gOwnerStatus_StatCursors[];

extern const struct BattleSummaryCursor gOwnerStatus_EquipmentCursors[];

extern const struct BattleSummaryCursor gOwnerStatus_ConditionCursors[];

extern const u8 gOwnerStatus_LevelString[];

/// The ability of each summon, by its number.
extern const u16 gSummon_Abilities[];

extern const u8 gWindow_PairBobOffsets[];

extern const struct BarPatternRow gMenu_BarPatternRows[];

extern const u8 Data_080aa1b8[];

extern const u8 gWindow_ProgressBarRows[];

extern const u8 gWindow_ProgressBarTiles[];

extern u8 Tile_BuildCellPicturesCode[];

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
#undef Message_Start

#undef Text_DrawPrefixedNumberAtOffset
#undef Text_DrawWideStringInWindow
#undef Window_MapTextCanvasTiles

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

/// One line of a window's HDMA shape: its left edge and its span.
struct WindowLine {
  u16 left;
  u16 span;
};

/// A page of the icon menu's window shape: one WindowLine for each of the
/// screen's 160 lines.
struct WindowBuffer {
  u8 unk000[4];
  struct WindowLine lines[160];
};

extern u8 gWorkspace_ItemTitle[];
extern u8 gWorkspace_EnergyTitle[];
extern u8 gWorkspace_StatusTitle[];
extern u8 gWorkspace_PageLabel[];
extern u8 gWorkspace_RangeDash[];

extern s8 gMenu_SaveSlotActionByPosition[];

// The parts, in ROM order.
#include "screens_battle_targets.c"
#include "screens_link_timer.c"
#include "screens_battle_party_commands.c"
#include "screens_icon_menu.c"
#include "screens_debug_destinations.c"
#include "screens_debug_flags.c"
#include "screens_pause_menu.c"

#undef BattleDjinnMenu_Run
