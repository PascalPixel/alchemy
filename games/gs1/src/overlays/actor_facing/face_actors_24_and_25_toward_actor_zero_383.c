#include "types.h"

/*
 * resource_383 owner at 0x02002ba0, 80 bytes.
 * Points two records at a third: each gets the angle from its own offset to the
 * reference record, stored as a halfword at +6.
 */
struct Rec_383 {
    u8 pad00[6];
    u16 f6;                     /* +6  */
    s32 f8;                     /* +8  */
    u8 pad0c[4];
    s32 f16;                    /* +16 */
};
struct Rec_383 *Func_02007926();
struct Rec_383 *Func_0200792e();
struct Rec_383 *Func_02007936();
s32 Func_02007858();
s32 Func_0200786c();
void SceneActor_FaceActors24And25TowardActorZero(void)
{
    struct Rec_383 *origin = Func_02007926(0);
    struct Rec_383 *first = Func_0200792e(24);
    struct Rec_383 *second = Func_02007936(25);

    first->f6 = Func_02007858(origin->f16 - first->f16, origin->f8 - first->f8);
    second->f6 = Func_0200786c(origin->f16 - second->f16, origin->f8 - second->f8);
}
