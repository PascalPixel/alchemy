/// @file
/// The screens object: the screens built on the window system (window.c). It
/// holds creating window sprites and the portraits, the party status window,
/// the save files, the file list and the saving screen's progress bar, the
/// name entry screen, the joined-party and found-Djinni messages, and the
/// pictures and command icons loaded for the menus.
///
/// screens_internal.h holds what every part shares (its includes and the
/// declarations the parts disagreed on); the code is in the files under
/// screens/, included here in the order it has in the ROM. Camelot's own files
/// are not recoverable: the ROM shows one object, and these parts are ours,
/// cut where The Broken Seal's are. The object ends at menu_titles.c: the
/// command row's Func_080457d0 reads the routine work as bytes, a read whose
/// bytes change where the work's struct is in scope, so it and the functions
/// after it stay in objects of their own. Where to start:
///
///   Party status    party_status.c: PartyStatus_CreateWindow,
///                   PartyStatus_Draw
///   Saving          save.c: Save_WriteFile, Save_ChooseAndLoadFile
///   Menus           menu_titles.c: RenderResource_CreatePair,
///                   RenderResource_CreateCommandIcon

#include "screens_internal.h"

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
#include "screens/window_sprites.c"
#include "screens/party_status.c"
#include "screens/save.c"
#include "screens/name_entry.c"
#include "screens/joined.c"
#include "screens/menu_titles.c"
