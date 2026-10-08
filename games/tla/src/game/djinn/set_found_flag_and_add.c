#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"


s32 GameFlag_SetBit(s32);

void Djinn_SetFoundFlagAndAdd(s32 arg0, s32 arg1, s32 arg2)
{
    GameFlag_SetBit((arg1 * 0x14) + arg2 + 0x30);
    Djinn_AddToOwnerFar(arg0, arg1, arg2);
}
