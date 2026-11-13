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
#undef Message_DecodeRawUnbounded
#undef MenuTitle_Bob
#undef gKeyState
#undef MenuCursor_AdvanceFar
#undef MenuCursor_MoveTowardTargetFar
#undef Portrait_Create
#undef MenuSprite
#undef PartyInventory_AddFar
#undef CommandIcon_Create
#undef WindowSprite_Pulse
#undef EffectPosition
#undef Math_UnsignedMulHighAdd
#undef Event_SetDestinationFar

// The screens built on the window system, as The Broken Seal's screens object
// holds them: creating window sprites and the portraits, the party status
// window, the save files, the file list and the saving screen's progress bar,
// the name entry screen, the joined-party and found-Djinni messages, the
// pictures and command icons loaded for the menus, the command row, the Djinn
// change preview, the Status screen and the battle Djinn, summon, item and
// Psynergy menus. They see the names below as they were matched, each a view
// until the parts agree. Where to start:
//
//   Party status    party_status.c: PartyStatus_CreateWindow,
//                   PartyStatus_Draw
//   Saving          save.c: Save_WriteFile, Save_ChooseAndLoadFile
//   Menus           menu_titles.c: MenuTitle_Create,
//                   CommandIcon_Create
//   Battle menus    battle_command_row.c: BattleCommandRow_Run

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
 * part has its declaration; tried as the rest of the object declares it,
 * BattleDjinnMenu_ShowChangePreview moves: 6 of 684 instructions in tla-ja. */
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

/* VIEW(screens): routine Message_OpenWindow as s32 Message_OpenWindow(s32
 * message, s32 x, s32 y, s32 style); window/window has struct Window
 * *Message_OpenWindow(s32 message, s32 x, s32 y, u32 packed) {. */
s32 Message_OpenWindow__screens(s32 message, s32 x, s32 y,
                                s32 style) __asm__("Message_OpenWindow");
#define Message_OpenWindow Message_OpenWindow__screens

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
struct CommandRowNavigation {
  u8 unk00[0x3c];
  s32 commandSelection[2];
  u8 unk44[8];
  s32 timer; ///< the link battle's time left; 0 takes the first command
  u8 unk50[0x88];
  s32 tutorialStep;
  s32 tutorialDelay;
  s32 commandActor;
  u8 unke4[0x20];
  s32 reserveChoice; ///< -1 while no reserve member is chosen
};

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

struct DjinnMenuWindow {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
};

struct DjinnMenuNavigation {
  u8 unk00[0x30];
  s32 row;
  s32 page;
  s32 preferredRow;
  u8 unk3c[0x10];
  s32 timer;
  u8 unk50[0x88];
  s32 tutorialStep;
  s32 tutorialDelay;
  u8 unke0[4];
  u32 chosenDjinn[8];
};

struct DjinnMenuUnit {
  u8 unk000[0xf8];
  u32 djinnHeld[4];
  u32 djinnSet[4];
};

struct DjinnMenuPartyStatus {
  struct DjinnMenuWindow *window;
  u8 unk04[8];
  u16 flags;
};

struct DjinnMenuSprite {
  u8 unk00[4];
  u32 attr01;
  u32 attr23;
};

struct DjinnMenuOam {
  u8 unk00[4];
  u8 y;
  u8 attr0high;
  u16 x : 9;
  u16 attr1high : 7;
  u16 tile : 10;
  u16 attr2high : 6;
};

struct DjinnSlot {
  u16 id;
  u16 state;
};

struct PreviewAbility {
  u8 unk00[2];
  u8 element; ///< 4 for none
  u8 unk03[5];
  u8 range; ///< 255 for everyone
  u8 ppCost;
};

struct ItemMenuWindow {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
};

struct ItemMenuNavigation {
  u8 unk00[0x30];
  s32 row;
  s32 page;
  s32 preferredRow;
  u8 unk3c[0x10];
  s32 timer;
};

struct ItemMenuSprite {
  struct ItemMenuSprite *next;
  union {
    struct {
      unsigned long attr01;
      unsigned long attr23;
    } word;
    struct {
      u16 y : 8;
      u16 affine : 2;
      u16 mode : 2;
      u16 mosaic : 1;
      u16 colors : 1;
      u16 shape : 2;
      u16 x : 9;
      u16 affineIndex : 5;
      u16 size : 2;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
      u16 unused;
    } f;
  } oam;
};

struct ItemMenuOam {
  struct ItemMenuOam *next;
  u16 y : 8;
  u16 affine : 2;
  u16 mode : 2;
  u16 mosaic : 1;
  u16 colors : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unused;
};

struct PsynergyMenuWindow {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
};

struct PsynergyMenuNavigation {
  u8 unk00[0x30];
  s32 row;
  s32 page;
  s32 preferredRow;
  u8 unk3c[0x10];
  s32 timer;
};

struct PsynergyMenuUnit {
  u8 unk000[0x3a];
  s16 pp;
  u8 unk03c[0x101];
  u8 psySeal;
};

struct PsynergyAbility {
  u8 unk00;
  u8 flags; ///< bit 7: usable in battle
  u8 element;
  u8 unk03[5];
  u8 range;
  u8 ppCost;
};

struct PsynergyMenuSprite {
  struct PsynergyMenuSprite *next;
  union {
    struct {
      unsigned long attr01;
      unsigned long attr23;
    } word;
    struct {
      u16 y : 8;
      u16 affine : 2;
      u16 mode : 2;
      u16 mosaic : 1;
      u16 colors : 1;
      u16 shape : 2;
      u16 x : 9;
      u16 affineIndex : 5;
      u16 size : 2;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
      u16 unused;
    } f;
  } oam;
};

struct PsynergyMenuOam {
  struct PsynergyMenuOam *next;
  u16 y : 8;
  u16 affine : 2;
  u16 mode : 2;
  u16 mosaic : 1;
  u16 colors : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unused;
};

struct SummaryElement {
  s16 power;
  s16 resist;
};

struct SummaryPsynergy {
  u16 id;
  u16 unk02;
};

/// A unit as the summary screen reads it.
struct SummaryUnit {
  u8 name[15];
  u8 level;
  u8 unk10[0x24];
  s16 hpMax;
  s16 ppMax;
  s16 hp;
  s16 pp;
  u16 atk;
  u16 def;
  u16 agi;
  u8 luck;
  u8 unk43[5];
  struct SummaryElement element[4];
  struct SummaryPsynergy psynergy[32];
  u16 items[15];
  u8 unkf6[2];
  u32 djinnOwned[4];
  u32 djinnSet[4];
  u8 djinnHeld[4];
  u8 djinnSetCount[4];
  u8 unk120[4];
  u32 experience;
  u8 unk128;
  u8 classId;
  u8 unk12a[6];
  s8 curse;
  s8 poison;
  u8 attackTurns;
  s8 attackStage;
  u8 defenseTurns;
  s8 defenseStage;
  u8 resistanceTurns;
  s8 resistanceStage;
  u8 delusion;
  u8 unk139[2];
  u8 stun;
  u8 sleep;
  u8 psySeal;
  u8 unk13e[2];
  u8 haunt;
  u8 grimReaper;
  u8 unk142[5];
  s8 agilityStage;
  u8 unk148[4];
};

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

struct SummaryWindow {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
};

struct SummaryWindowWork {
  u8 unk00[3];
  u8 dirty;
  u8 unk04[2];
  u8 holdTilemapUpload;
  u8 textColor;
};

struct SummaryNavigation {
  u8 unk00[0x44];
  void *timerWindow;
  s32 timerHidden;
  s32 timer;
  u8 unk50[0x46];
  u16 list[64];
};

struct SummaryAbility {
  u8 unk00;
  u8 flags; ///< bit 7: usable in battle
  u8 element;
  u8 unk03[5];
  u8 range;
  u8 ppCost;
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

struct SummonMenuWindow {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
};

struct SummonMenuWork {
  u8 unk00[3];
  u8 dirty;
  u8 unk04[2];
  u8 holdTilemapUpload;
};

struct SummonMenuNavigation {
  u8 unk00[0x30];
  s32 row;
  s32 page;
  s32 preferredRow;
  u8 unk3c[0x10];
  s32 timer;
  u8 unk50[0x88];
  s32 tutorialStep;
  s32 tutorialDelay;
};

struct SummonMenuSprite {
  struct SummonMenuSprite *next;
  union {
    struct {
      unsigned long attr01;
      unsigned long attr23;
    } word;
    struct {
      u16 y : 8;
      u16 affine : 2;
      u16 mode : 2;
      u16 mosaic : 1;
      u16 colors : 1;
      u16 shape : 2;
      u16 x : 9;
      u16 affineIndex : 5;
      u16 size : 2;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
      u16 unused;
    } f;
  } oam;
};

struct SummonMenuOam {
  struct SummonMenuOam *next;
  u16 y : 8;
  u16 affine : 2;
  u16 mode : 2;
  u16 mosaic : 1;
  u16 colors : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unused;
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

struct UiWindowBounds {
  s32 handle;
  u16 left;
  u16 top;
  u16 right;
  u16 height;
  u16 flags;
};

struct StatusUnit {
  u8 name[0x34];
  s16 maxHp;
  s16 maxPp;
  s16 hp;
  s16 pp;
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

extern u8 gData_02000504[];

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

#undef Message_OpenWindow
#undef Portrait_OpenWindow
#undef MenuCursor_SetPositionImmediateFar
#undef Message_Start
#undef gWindowSprite_BounceScales
#undef Text_DrawWideStringAtOffset
#undef Window_PutGlyph

#undef Text_DrawPrefixedNumberAtOffset
#undef Text_DrawWideStringInWindow
#undef Window_MapTextCanvasTiles

// The rest of the screens, as The Broken Seal's screens object ends: the
// battle target selection, the link timer, the party's battle commands, the
// command icon menus, the debug menus' destination row and the Pause menu,
// with the names they see as they were matched.

struct TargetNavigation {
  u8 unk00[0x28];
  s32 targetViewOffset;
  u8 unk2c[0x20];
  s32 timer;
  u8 unk50[0x88];
  s32 tutorialStep;
  s32 tutorialDelay;
  u8 unke0[0x2c];
  s32 lastTarget;
};

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

/* VIEW(menus): routine Portrait_Load as s32 Portrait_Load(s32 portrait, s32
 * slot); window/screens has s32 Portrait_Load(u32 entry, s32 slot) {. */
s32 Portrait_Load__menus(s32 portrait, s32 slot) __asm__("Portrait_Load");
#define Portrait_Load Portrait_Load__menus

extern s8 gMenu_SaveSlotActionByPosition[];

extern struct LinkCountdownState *gLinkCountdownWork;

struct TargetWindow {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
};

// The parts, in ROM order.
#include "window/battle_targets.c"
#include "window/link_timer.c"
#include "window/battle_party_commands.c"
#include "window/icon_menu.c"
#include "window/debug_destinations.c"
#include "window/debug_flags.c"
#include "window/pause_menu.c"

#undef Portrait_Load
#undef Text_DrawWideStringAtOffset
#undef Menu_BuildLocalizedPatternTiles
#undef Window_PutGlyph
#undef BattleDjinnMenu_Run
#undef Message_ShowAndWait
#undef Window_ClearInteriorTiles
#undef Text_DrawNumber
#undef Icon_BuildItemIconTiles
#undef Icon_BuildMiscIconTiles
#undef gPortrait_CharacterMap
#undef gPortrait_SpriteMap
#undef Portrait_OpenWindow
