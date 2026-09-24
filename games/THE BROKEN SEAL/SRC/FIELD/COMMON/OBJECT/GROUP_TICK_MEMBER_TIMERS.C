#include "B5_CONTEXT.H"

/* object/group/tick_member_timers.c */
extern u8 *gBattleFxWork;

void ObjectGroup_TickMemberTimers(void)
{
    u8 *base;
    s32 i;
    s32 index;

    base = gBattleFxWork;
    i = 0;
    do {
        if (base[0x7818 + i] != 0) {
            if ((base[0x7818 + i] = base[0x7818 + i] - 1) == 0) {
                index = i * 2 + 36;
                ObjectGroup_UpdateMembers(
                    *(s16 *)(*(u8 **)(base + 0x7828) + index),
                    0, -1, -1, 0);
            }
        }
        i += 1;
    } while (i != 8);
}
