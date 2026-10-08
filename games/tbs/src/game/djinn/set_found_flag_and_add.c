#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene.h"

void Djinn_SetFoundFlagAndAdd(s32 owner, s32 element, s32 index)
{
    GameFlag_SetBitFar((element * 0x14) + index + 0x30);
    Djinn_AddToOwnerFar(owner, element, index);
}
