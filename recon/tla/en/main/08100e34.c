#include "TYPES.H"
#include "SCENE.H"
void Audio_PlayCueReturnOne(s32);

/* ability/play_use_animation.c */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void *BattleAction_Get();

s32 Menu_SetFirstObjectRowCoordinates(s32 arg0)
{
    u8 *current;
    s32 value;
    s32 count;
    current = gMenuWork + 0x134;
    arg0 += 0x3D;
    value = 0x20;
    count = 3;
    do {
        count--;
        *(s16 *)current = value;
        *(s16 *)(current + 0x10) = arg0;
        value += 0x38;
        current += 2;
    } while (count >= 0);
    return arg0;
}
