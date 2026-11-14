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
#include "window/windows.c"
#include "window/typing.c"
#include "window/borders.c"
#include "window/glyphs.c"
#include "window/decode.c"
#include "window/measure.c"
#include "window/sprites.c"
#include "window/messages.c"
#include "window/icons.c"
#include "window/selection.c"
#include "window/shortcuts.c"
#include "window/settings_menu.c"
#include "window/debug.c"
#include "window/text_tiles.c"

#undef Unit_Get
#undef Ability_Get
#undef Portrait_Create
#undef MenuSprite
#undef CommandIcon_Create
#undef EffectPosition
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

// The screens object's functions its parts call before their definitions.
struct BobPairObject;

struct CommandRowIcon {
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

struct CommandRowAffine {
  u16 scaleX;
  u16 scaleY;
  u16 angle;
};

struct CommandRowWork {
  u8 paletteSlots[256];
  s32 paletteCount;
  struct CommandRowIcon entries[6];
  void *window;
  s32 iconCount;
  struct CommandRowAffine affine;
  s32 affineIndex;
  s32 selectedIndex;
  s32 drawnIndex;
  s32 commandKinds[6];
};

/// The battle menu's navigation work (heap slot 57).
struct CommandRowWindowWork {
  u8 unk00[3];
  u8 dirty;
  u8 unk04;
  u8 flags05;
  u8 holdTilemapUpload;
  u8 unk07;
  u16 tilemap[0x400];
};

struct BgScrollPair {
  s16 x;
  s16 y;
};

struct DjinnSlot {
  u16 id;
  u16 state;
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

struct SummaryPoint {
  s32 x;
  s32 y;
};

struct SummaryCursor {
  s8 groupPositions[8];
  s32 helpEntry;
  s32 drawnHelpEntry;
  s32 group;
  s32 groupSize;
  s32 column;
  s32 row;
};

struct SummaryStatCursor {
  u8 entry;
  u8 column;
  u8 row;
  u8 highlightColumn;
  u8 highlightRow;
  u8 highlightWidth;
  u8 unk06[2];
};

struct SummaryCursorStop {
  u8 entry;
  u8 column;
  u8 row;
  u8 unk03;
};

struct SummaryRecoveryEntry {
  u8 element;
  u8 index;
  u8 owner;
  s8 turns;
};

struct SummaryRecoveryList {
  struct SummaryRecoveryEntry entries[72];
  s32 count;
};

struct SummaryRecovery {
  u8 unk000[8];
  struct SummaryRecoveryList list;
};

struct SummaryResourceEntry {
  u16 unk00;
  u16 offset;
};

struct SummonMenuWork {
  u8 unk00[3];
  u8 dirty;
  u8 unk04[2];
  u8 holdTilemapUpload;
};

struct BobPairObject {
  struct BobPairObject *next;
  u8 unk04[4];
  u8 baseY;
  u8 unk09[11];
  u8 y;
};

struct BarPatternRow {
  u32 word0;
  u32 word1;
};

union NameCursor {
  struct WindowSprite *sprite;
  struct NameCursorMotion cursor;
};

struct SaveGameWords {
  u32 buildStamp;
  u32 playTime;
};

struct ObjectRowWork {
  u8 unk000[0xf8];
  void *icons[4];
  u8 unk108[0x10];
  s16 x[8];
  s16 y[8];
  s32 scale[4];
};

struct ObjectRowState {
  u8 unk000[248];
  void *objects[4];
  u8 unk108[16];
  s16 x[8];
  s16 y[8];
  s32 scale[4];
};

/// The portion of the file summary (SaveSummary) read here.
struct NumberRow {
  u8 unk00[0x28];
  s8 values[4]; ///< Djinn count for each element.
};

extern const u8 gDjinnMenu_BlinkTile[32];

extern u8 gDjinnMenu_ElementXOffsets[4];

extern u8 gDjinnMenu_ElementYOffsets[4];

extern u8 Msg_0d10[];

extern u8 Msg_0d11[];

extern u8 Msg_0d12[];

extern u8 Msg_0d13[];

extern u8 Msg_0d15[];

extern u8 Msg_0d16[];

extern u8 Msg_0d17[];

extern u8 Msg_0d19[];

extern const struct SummaryStatCursor gOwnerStatus_StatCursors[];

extern const struct SummaryCursorStop gOwnerStatus_EquipmentCursors[];

extern const struct SummaryCursorStop gOwnerStatus_ConditionCursors[];

extern const u8 gOwnerStatus_LevelString[];

extern const u16 Data_0805f676[];

extern const u8 gWindow_PairBobOffsets[];

extern const struct BarPatternRow gMenu_BarPatternRows[];

extern const u8 Data_080aa1b8[];

extern const u8 gWindow_ProgressBarRows[];

extern const u8 gWindow_ProgressBarTiles[];

extern u8 Tile_BuildCellPicturesCode[];

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
#include "window/window_sprites.c"
#include "window/party_status.c"
#include "window/save.c"
#include "window/name_entry.c"
#include "window/joined.c"
#include "window/menu_titles.c"
#include "window/battle_command_row.c"
#include "window/battle_djinn_preview.c"
#include "window/battle_status.c"
#include "window/battle_djinn_menu.c"
#include "window/battle_summon_menu.c"
#include "window/battle_item_menu.c"
#include "window/battle_psynergy_menu.c"

#undef MenuCursor_SetPositionImmediateFar
#undef Message_Start

#undef Text_DrawPrefixedNumberAtOffset
#undef Text_DrawWideStringInWindow
#undef Window_MapTextCanvasTiles

struct TargetBattle {
  u8 unk00[0x58];
  s16 partyUnits[7];
  s16 enemyIds[7];
};

struct TargetPoint {
  s32 x;
  s32 y;
  s32 z;
};

struct TargetTrack {
  u8 x;
  u8 y;
  u8 flags;
  s8 index;
};

struct TargetSprite {
  u8 unk00[4];
  u8 y;
  u8 affine : 2;
  u8 mode : 2;
  u8 high05 : 4;
  u16 x : 9;
  u16 matrix : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 attr2high : 6;
  u16 unk0a;
};

struct TargetAffine {
  u16 scaleX;
  u16 scaleY;
  u16 angle;
  u16 unk06;
};

struct TextObject {
  u8 storage[12];
};

struct WindowLine {
  u16 left;
  u16 span;
};

struct WindowBuffer {
  u8 unk000[4];
  struct WindowLine lines[160];
};

struct MenuSelectionState {
  u8 padding000[0x78];
  void *work;
  u8 padding07c[8];
  u8 resourceIds[8];
  s16 selection;
  s16 itemCount;
  s16 field090;
  s16 resourceBase;
};

struct MenuModeLabelState {
  u8 unk00[0x7c];
  void *window;
  u8 unk80[0x0c];
  s16 mode;
  u8 unk8e[8];
  s16 previousMode;
};

struct CountdownDisplayEntry {
  u8 data[12];
};

struct LinkSignature {
  u8 padding000[8];
  u16 e;
  u16 d;
  u16 v;
  u16 s;
  u8 padding010[8];
};

struct LinkCountdownState {
  struct CountdownDisplayEntry entries[3];
  u8 entryActive[3];
  u8 padding027;
  s32 targetOffset;
  s32 currentOffset;
  u8 padding030[20];
  s32 displayHandle;
  s32 secondaryHandle;
  s32 timer;
  s32 enabled;
};

struct LinkRuntimeState {
  u8 padding000[80];
  u8 side;
  u8 padding051;
  u8 paused;
};

extern u8 gWorkspace_ItemTitle[];
extern u8 gWorkspace_EnergyTitle[];
extern u8 gWorkspace_StatusTitle[];
extern u8 gWorkspace_PageLabel[];
extern u8 gWorkspace_RangeDash[];

extern s8 gMenu_SaveSlotActionByPosition[];

extern struct LinkCountdownState *gLinkCountdownWork;

// The parts, in ROM order.
#include "window/battle_targets.c"
#include "window/link_timer.c"
#include "window/battle_party_commands.c"
#include "window/icon_menu.c"
#include "window/debug_destinations.c"
#include "window/debug_flags.c"
#include "window/pause_menu.c"

#undef BattleDjinnMenu_Run
