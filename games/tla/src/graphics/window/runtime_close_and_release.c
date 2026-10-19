//--------------------------------------------------------------------------
// runtime_close_and_release.c
// Windows: close the window in heap slot 16 and release its block.
//--------------------------------------------------------------------------

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"

//--------------------------------------------------------------------------
// Close the window in heap slot 16 and release its block.
//--------------------------------------------------------------------------
void UiWork_CloseAndRelease(void) { PartyStatus_Close(); }
