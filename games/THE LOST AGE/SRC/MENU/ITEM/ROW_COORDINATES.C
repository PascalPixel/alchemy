#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "text/MSG_IDS.H"

s32 Menu_GetModuloOfSum(s32 arg0, s32 arg1)
{
    return (arg0 + arg1) % arg1;
}

s32 Menu_SetFirstObjectRowCoordinates(s32 arg0)
{
    u8 *current;
    s32 value;
    s32 count;
    current = (u8 *)Ram_HeapSlots->menu_runtime + 0x118;
    arg0 += 61;
    value = 0x20;
    count = 3;
    do {
        count--;
        *(s16 *)current = value;
        *(s16 *)(current + 0x10) = arg0;
        value += 56;
        current += 2;
    } while (count >= 0);
    return arg0;
}

s32 GameFlag_Test(s32 flag);

s32 Menu_GetDjinnGiveMessage(void)
{
    if (GameFlag_Test(0x175))
        return (s32)MsgDjinnGiveTutorialB;
    return (s32)MsgDjinnGiveTutorialA;
}
