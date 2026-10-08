#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"


u32 GameFlag_SetByte(u32 flag, u8 value)
{
    u32 shifted = flag << 20;
    flag = shifted >> 23;
    GameFlagBytes[flag] = value;
    return flag;
}
