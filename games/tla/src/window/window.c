/// @file
/// The window object: the windows drawn into a shared BG tilemap, the messages
/// typed into them and the decoder behind them, the sprites windows carry,
/// icons, the selection screen, the settings and Pause menus, the debug menus
/// and text drawn as tiles.
///
/// window_internal.h holds what every part shares (its includes and the
/// declarations the parts disagreed on); the code is in the files under
/// window/, included here in the order it has in the ROM. Camelot's own files
/// are not recoverable: the ROM shows one object, and these parts are ours,
/// cut where The Broken Seal's are. debug.c holds the debug menus, which The
/// Lost Age keeps here, between the Pause menu and the text tiles, where The
/// Broken Seal keeps them in its screens object (screens/debug.c). The object
/// ends at Text_DrawPrefixedNumberAtOffset, as The Broken Seal's does. Where to
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
#undef Unit_Get
#undef Portrait_Create
#undef Ability_Get
#undef MenuSprite
#undef MenuCursor_AdvanceFar
#undef MenuCursor_MoveTowardTargetFar
#undef WindowSprite_Pulse
#undef CommandIcon_Create
#undef MenuTitle_Bob
#undef Event_SetDestinationFar

// ---- window/screens.c ----

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

/// The screens object: the screens built on the window system (window.c). It
/// holds creating window sprites and the portraits, the party status window,
/// the save files, the file list and the saving screen's progress bar, the
/// name entry screen, the joined-party and found-Djinni messages, the pictures
/// and command icons loaded for the menus, the command row, the Djinn change
/// preview, the Status screen and the battle Djinn, summon, item and Psynergy
/// menus.
///
/// screens_internal.h holds what every part shares (its includes and the
/// declarations the parts disagreed on); the code is in the files under
/// screens/, included here in the order it has in the ROM. Camelot's own files
/// are not recoverable: the ROM shows one object, and these parts are ours,
/// cut where The Broken Seal's are. The object ends at the battle Psynergy menu
/// (battle_psynergy_menu.c): the target selection after it,
/// BattleTarget_RunSelection, keeps Oam_AddSpriteAtDepth declared returning s32
/// (a fake match its tag measures), which cannot share a translation unit with
/// the routine's true declaration, so it and the functions after it stay in
/// objects of their own. Where to start:
///
///   Party status    party_status.c: PartyStatus_CreateWindow,
///                   PartyStatus_Draw
///   Saving          save.c: Save_WriteFile, Save_ChooseAndLoadFile
///   Menus           menu_titles.c: MenuTitle_Create,
///                   CommandIcon_Create
///   Battle menus    battle_command_row.c: BattleCommandRow_Run

#include "screens_internal.h"

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
#include "screens/window_sprites.c"
#include "screens/party_status.c"
#include "screens/save.c"
#include "screens/name_entry.c"
#include "screens/joined.c"
#include "screens/menu_titles.c"
#include "screens/battle_command_row.c"
#include "screens/battle_djinn_preview.c"
#include "screens/battle_status.c"
#include "screens/battle_djinn_menu.c"
#include "screens/battle_summon_menu.c"
#include "screens/battle_item_menu.c"
#include "screens/battle_psynergy_menu.c"

#undef Message_OpenWindow
#undef Portrait_OpenWindow
#undef MenuCursor_SetPositionImmediateFar
#undef Message_Start
#undef gWindowSprite_BounceScales
#undef Text_DrawWideStringAtOffset
#undef Window_PutGlyph
#undef Portrait_OpenWindow
#undef gWindowSprite_BounceScales
#undef Text_DrawWideStringAtOffset
#undef Window_PutGlyph

#undef Text_DrawPrefixedNumberAtOffset
#undef Text_DrawWideStringInWindow
#undef Window_MapTextCanvasTiles

// ---- window/menus.c ----

/// The rest of the screens object (screens.c): the battle target selection,
/// the link timer, the party's battle commands, the command icon menus, the
/// debug menus and the pause menu, as The Broken Seal's screens.c ends. The ROM
/// shows one object; it is two here because BattleTarget_RunSelection keeps
/// Oam_AddSpriteAtDepth declared returning s32 (a fake match its tag measures),
/// which cannot share a translation unit with the routine's true declaration.
/// menus_internal.h holds the includes every part shares; the parts under
/// menus/ follow in ROM order.

#include "menus_internal.h"

// The parts, in ROM order.
#include "menus/battle_targets.c"
#include "menus/link_timer.c"
#include "menus/battle_party_commands.c"
#include "menus/icon_menu.c"
#include "menus/debug.c"
#include "menus/debug_flags.c"
#include "menus/pause_menu.c"

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