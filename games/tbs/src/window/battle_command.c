/// @file
/// The menu object of the window bank: the screens built on the window system
/// (window.c). It holds the party status window, the save file menu, the name
/// entry screen, the joined-party messages, the command icon row, the Djinn
/// change preview and the status screen, the battle menus (Djinn, summons,
/// items, Psynergy, targets and the party's commands), the command icon menus
/// of the field and title screens, and the debug menus.
///
/// battle_command_internal.h holds what every part shares (includes, edition
/// values, macros and the link round state); the code is in the files under
/// battle_command/, included here in the order it has in the ROM. Camelot's
/// own files are not recoverable: the ROM shows one object, and these parts
/// are ours, cut where one family ends and the next begins. README.md in this
/// folder gives the words, prefixes and spellings the parts use. Where to
/// start:
///
///   Party status    party_status.c: UiWindow_CreateWithLayoutBounds,
///                   PartyStatus_Draw
///   Saving          save.c: SaveMenu_SelectSlot, SaveMenu_WriteSelectedSlot
///   Battle menus    party_commands.c: Battle_CollectPartyCommands, which
///                   runs command_row.c: Menu_RunCommandRow and the menus
///                   of djinn_menu.c, summon_menu.c, item_menu.c,
///                   psynergy_menu.c and targets.c
///   Icon menus      icon_menu.c: Menu_SelectTopEntry,
///                   Menu_SelectSaveSlotAction

#include "battle_command_internal.h"

// The menu object's code, in ROM order. Each part is compiled here, as part
// of this one object, never on its own: the order of these includes is the
// order of the code in the ROM.
#include "battle_command/outputs.c"
#include "battle_command/party_status.c"
#include "battle_command/save.c"
#include "battle_command/name_entry.c"
#include "battle_command/joined.c"
#include "battle_command/pictures.c"
#include "battle_command/command_row.c"
#include "battle_command/djinn_preview.c"
#include "battle_command/status.c"
#include "battle_command/djinn_menu.c"
#include "battle_command/summon_menu.c"
#include "battle_command/item_menu.c"
#include "battle_command/psynergy_menu.c"
#include "battle_command/targets.c"
#include "battle_command/link_timer.c"
#include "battle_command/party_commands.c"
#include "battle_command/icon_menu.c"
#include "battle_command/debug.c"
