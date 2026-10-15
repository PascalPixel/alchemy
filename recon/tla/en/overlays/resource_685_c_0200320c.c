/* Draft of overlay 685's Func_0200320c, differ 4 of 68: allocates the 0x1078-byte work in heap
 * slot 55, starts Func_02002de4 and raises field106c in two steps until it reaches 0x14ccc.
 * The priority temporary is needed (the native finishes 0x480 before it loads the callback).
 * Left: in the second step the native finishes 0x14ccc (adds r3, #204) before it starts 999
 * (movs r5, #186), then stores; here 999 starts first. sched2 ranks the store one lower than
 * the native must. Tried: state as u32, s16 or u16, the value through a pointer, >= forms, an if
 * chain (19), state = 999 first (6), a volatile store, a register pin, an empty asm or a break
 * after it (4), a one-pass block round the store or round state = 999 (2, the store then goes
 * before 999); permute 3 minutes found only the one-pass block. */
#include "../../../../games/tla/src/scene/champa_cliff_caves/scene.h"

struct ChampaCliffCavesWork {
    u8 unk0000[0x1000];
    s32 field1000;
    s32 field1004;
    s32 field1008;
    u8 unk100c[0x58];
    s32 field1064;
    u8 unk1068[4];
    s32 field106c;
    s32 field1070;
    u16 field1074;
    u8 unk1076[2];
};

void *Runtime_AllocateHeapBlockFar(s32 slot, s32 size);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void WaitFrames(s32 frames);
void Func_02002d34(void);
void Func_02002de4(void);

void Func_0200320c(void)
{
    struct ChampaCliffCavesWork *work = Runtime_AllocateHeapBlockFar(220, sizeof(struct ChampaCliffCavesWork));
    s32 state;

    Func_02002d34();
    {
        s32 priority = 0x480;

        Scheduler_AddOrUpdateCallback(Func_02002de4, priority);
    }
    Func_0200362c(144);
    state = 0;
    do {
        switch (state) {
        case 0:
            work->field106c += 0x1000;
            if (work->field106c > 0x6665) {
                work->field106c = 0x6666;
                state = 1;
            }
            break;
        case 1:
            work->field106c += 0x3000;
            if (work->field106c > 0x14ccb) {
                work->field106c = 0x14ccc;
                state = 999;
            }
            break;
        }
        WaitFrames(1);
    } while (state != 999);
}
