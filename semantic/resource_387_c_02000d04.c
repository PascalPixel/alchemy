#include "types.h"








/* Run the actor-8 scene only while it occupies column eleven. */
extern void Func_02001e36(void);
extern u8 * Func_02001e54(s32 actorId);
extern void Func_02001af6(s32 actorId);
extern u8 * Func_02001e68(s32 actorId);
extern void Func_02001dfe(s32, s32, s32, s32, s32, s32);
extern void Func_02001e0e(s32, s32, s32, s32, s32, s32);
extern void Func_02001e66(s32 flag);
extern void Func_02001e92(void);
void Func_02000d04(void)
{
    u8 *actor;
    s32 column;

    Func_02001e36();
    actor = Func_02001e54(8);
    column = *(s32 *)(actor + 8) >> 20;
    if (column == 11) {
        Func_02001af6(8);
        actor = Func_02001e68(8);
        actor[35] |= 2;
        Func_02001dfe(39, 12, 3, 1, 8, 12);
        Func_02001e0e(43, 11, 3, 1, 12, column);
        Func_02001e66(0x860);
    }
    Func_02001e92();
}
