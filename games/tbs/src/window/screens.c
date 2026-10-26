/// @file
/// The menu object of the window bank: the screens built on the window system
/// (window.c). It holds the party status window, the file menu, the name
/// entry screen, the joined-party messages, the command icon row, the Djinn
/// change preview and the Status screen, the battle menus (Djinn, summons,
/// items, Psynergy, targets and the party's commands), the command icon menus
/// of the field and title screens, and the debug menus.
///
/// screens_internal.h holds what every part shares (includes, edition
/// values, macros and the link round state); the code is in the files under
/// screens/, included here in the order it has in the ROM. Camelot's
/// own files are not recoverable: the ROM shows one object, and these parts
/// are ours, cut where one family ends and the next begins. README.md in this
/// folder gives the words, prefixes and spellings the parts use. Where to
/// start:
///
///   Party status    party_status.c: PartyStatus_CreateWindow,
///                   PartyStatus_Draw
///   Saving          save.c: FileMenu_SelectFile, Save_ToChosenFile
///   Battle menus    party_commands.c: Battle_CollectPartyCommands, which
///                   runs command_row.c: BattleCommandRow_Run and the menus
///                   of djinn_menu.c, summon_menu.c, item_menu.c,
///                   psynergy_menu.c and targets.c
///   Icon menus      icon_menu.c: FieldMenu_SelectCommand,
///                   FileMenu_SelectAction

#include "screens_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
#include "screens/outputs.c"
#include "screens/party_status.c"
#include "screens/save.c"
#include "screens/name_entry.c"
#include "screens/joined.c"
#include "screens/pictures.c"
#include "screens/command_row.c"
#include "screens/djinn_preview.c"
#include "screens/status.c"
#include "screens/djinn_menu.c"
#include "screens/summon_menu.c"
#include "screens/item_menu.c"
#include "screens/psynergy_menu.c"
#include "screens/targets.c"
#include "screens/link_timer.c"
#include "screens/party_commands.c"
#include "screens/icon_menu.c"
#include "screens/debug.c"
