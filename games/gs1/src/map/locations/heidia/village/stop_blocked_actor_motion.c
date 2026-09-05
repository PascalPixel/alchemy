#include "types.h"

typedef struct MovingActor {
    u8 pad0[6];
    u16 kind;       /* 0x06 */
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
    u8 pad14[0x10];
    s32 vx;         /* 0x24 */
    u8 pad28[4];
    s32 vy;         /* 0x2c */
    u8 pad30[8];
    s32 a;          /* 0x38 */
    u8 pad3c[4];
    s32 b;          /* 0x40 */
} MovingActor;

typedef struct BlockingActor {
    u8 pad0[0x50];
    s32 *info;      /* 0x50 */
} BlockingActor;

extern s32 Data_0200ded8[];
extern s32 Data_0200df18[];

BlockingActor *Func_02000342(s32 *position, MovingActor *actor);
s32 Func_02005902(MovingActor *actor, s32 *position);

#define StopBlockedActorMotion Func_020002a8
s32 StopBlockedActorMotion(MovingActor *actor)
{
    s32 destination[3];
    u32 direction_index = actor->kind >> 12;
    s32 step = Data_0200ded8[direction_index];
    BlockingActor *blocking_actor;

    destination[0] = actor->x + (step & 0xffff0000);
    destination[1] = actor->y;
    step <<= 16;
    destination[2] = actor->z + step;
    blocking_actor = Func_02000342(destination, actor);
    if (blocking_actor != 0) {
        u32 kind_index = 0;
        s32 blocking_kind = ((s16 *)*(s32 *)((u8 *)blocking_actor->info + 0x28))[kind_index];

        {
            s32 *p = Data_0200df18;

            do {
                if (blocking_kind == *p++) {
                    goto done;
                }
                kind_index++;
            } while (kind_index <= 5);
        }
        actor->vx = 0;
        actor->vy = 0;
        actor->a = 0x80000000;
        actor->b = 0x80000000;
    }
    step = Data_0200ded8[direction_index];
    destination[0] = actor->x + (step & 0xffff0000);
    destination[1] = actor->y;
    step <<= 16;
    destination[2] = actor->z + step;
    if (Func_02005902(actor, destination) > 0) {
        actor->vx = 0;
        actor->vy = 0;
        actor->a = 0x80000000;
        actor->b = 0x80000000;
    }
done:
    return 0;
}
