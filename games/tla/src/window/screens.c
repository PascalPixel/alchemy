/// @file
/// The screens object: the screens built on the windows, from the window
/// sprites to the Pause menu, which The Broken Seal also links as screens.c.
/// Its code calls several window routines with no declaration in scope, as
/// the twin's screens.c does.
///
/// window_internal.h holds what both objects share; the code is in the
/// screens_*.c files under window/, included here in the order it has in the
/// ROM.

#include "window_internal.h"
#include "resource_ids.h"
#include "system/system.h"
#include "field/field_work.h"
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

extern u16 gWindowSprite_BounceScales[];
extern const s16 gPortrait_CharacterMap[];
extern const s16 gPortrait_SpriteMap[];

#ifdef MODERN
// The screens object calls the window sprite list's routines with no
// declaration in scope, as its bytes need; a current GCC gets them here.
#include "graphics/output.h"
#endif

/// One icon of the command row (struct BattleCommandRowWork): its sprite, where
/// it stands and the command it stands for. menu_titles.c loads it,
/// battle_command_row.c runs the row.
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
  s32 partyMemberScale[4]; ///< 16.16 fixed point, FIXED_ONE full size
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

extern const u8 gPartyStatus_DigitTiles[];

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
