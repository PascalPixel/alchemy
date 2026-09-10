#include "types.h"

#define StopBlockedActorMotion Func_020002a8
#define ActivateSceneActor17 Func_020014bc
#define ActivateSceneActor18 Func_02001650
#define FinishActor20SceneSequence Func_02004d0c
#define ActivateFiveActorGroupFromFlags Func_02005520
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
extern u8 *Data_03001ebc;

BlockingActor *Func_02000342(s32 *position, MovingActor *actor);
s32 Func_02005902(MovingActor *actor, s32 *position);
void Func_02002fe6(s32);
void Func_02006afe(void);
void Func_0200317a(s32);
void Func_02006c92(void);
s32 Func_0200a322(s32);
void Func_0200a404(s32);
void Func_0200a41c(s32, s32);
void Func_0200a4c6(void);
void Func_02009936(void);
s32 Func_0200ab38(s32);
void Func_020061c2(s32);
s32 Func_0200ab48(s32);
void Func_020061d2(s32);
s32 Func_0200ab58(s32);
void Func_020061e2(s32);
s32 Func_0200ab68(s32);
void Func_020061f2(s32);
s32 Func_0200ab7a(s32);
void Func_02006204(s32);

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

void ActivateSceneActor17(void)
{
    Func_02002fe6(17);
    Func_02006afe();
}

void ActivateSceneActor18(void)
{
    Func_0200317a(18);
    Func_02006c92();
}

void FinishActor20SceneSequence(void)
{
    if (Func_0200a322(0x226)) {
        Func_0200a404(0x2435);
        Func_0200a41c(20, 0);
    } else {
        s16 *q = (s16 *)(Data_03001ebc + 382);

        *q = 0;
        Func_0200a4c6();
        Func_02009936();
    }
}

void ActivateFiveActorGroupFromFlags(void)
{
    if (Func_0200ab38(0x350)) {
        Func_020061c2(0);
    }
    if (Func_0200ab48(0x351)) {
        Func_020061d2(1);
    }
    if (Func_0200ab58(0x352)) {
        Func_020061e2(2);
    }
    if (Func_0200ab68(0x353)) {
        Func_020061f2(3);
    }
    if (Func_0200ab7a(0x354)) {
        Func_02006204(4);
    }
}
