/// @file
/// The menu object of the window bank: the screens built on the window system
/// (window.c). It holds the party status window, the file list, the name
/// entry screen, the joined-party messages, the command icon row, the Djinn
/// change preview and the Status screen, the battle menus (Djinn, summons,
/// items, Psynergy, targets and the party's commands), the command icon menus
/// of the field and title screens, and the debug menus.
///
/// screens_internal.h holds what every part shares (includes, edition
/// values, macros and the link turn state); the code is in the files under
/// screens/, included here in link order, one family to a part. README.md in
/// this folder gives the words, prefixes and spellings the parts use. Where to
/// start:
///
///   Party status    screens_party_status.c: PartyStatus_CreateWindow,
///                   PartyStatus_Draw
///   Saving          screens_save.c: FileList_SelectFile, Save_ToChosenFile
///   Battle menus    screens_battle_party_commands.c:
///   Battle_CollectPartyCommands, which
///                   runs screens_battle_command_row.c: BattleCommandRow_Run
///                   and the menus of screens_battle_djinn_menu.c,
///                   screens_battle_summon_menu.c, screens_battle_item_menu.c,
///                   screens_battle_psynergy_menu.c and
///                   screens_battle_targets.c
///   Icon menus      screens_icon_menu.c: FieldMenu_SelectCommand,
///                   FileList_SelectAction

#include "screens_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
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
#include "screens_battle_targets.c"
#include "screens_link_timer.c"
#include "screens_battle_party_commands.c"
#include "screens_icon_menu.c"
#include "screens_debug.c"
