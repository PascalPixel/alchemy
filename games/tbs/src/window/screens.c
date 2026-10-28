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
/// screens/, included here in the order it has in the ROM. Camelot's
/// own files are not recoverable: the ROM shows one object, and these parts
/// are ours, cut where one family ends and the next begins. README.md in this
/// folder gives the words, prefixes and spellings the parts use. Where to
/// start:
///
///   Party status    party_status.c: PartyStatus_CreateWindow,
///                   PartyStatus_Draw
///   Saving          save.c: FileList_SelectFile, Save_ToChosenFile
///   Battle menus    battle_party_commands.c: Battle_CollectPartyCommands,
///   which
///                   runs battle_command_row.c: BattleCommandRow_Run and the
///                   menus of battle_djinn_menu.c, battle_summon_menu.c,
///                   battle_item_menu.c, battle_psynergy_menu.c and
///                   battle_targets.c
///   Icon menus      icon_menu.c: FieldMenu_SelectCommand,
///                   FileList_SelectAction

#include "screens_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
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
#include "screens/battle_targets.c"
#include "screens/link_timer.c"
#include "screens/battle_party_commands.c"
#include "screens/icon_menu.c"
#include "screens/debug.c"
