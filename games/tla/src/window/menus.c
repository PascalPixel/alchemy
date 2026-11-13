/// @file
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
