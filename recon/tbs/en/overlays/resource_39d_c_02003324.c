/* NONMATCHING: 276 of 276 bytes, 14 differing halfwords (2026-09-24); the
 * descending twin of the adopted 39d:02003208. Remaining: the high registers
 * for work, speed and the count come out (sl, r9, fp) instead of (fp, sl, r9). */
#include "TYPES.H"

void Local_020031c0();
u8 * Engine_ActorGet();
void Engine_AudioPlayCue();
void Engine_TaskWait();
void Engine_MapRedraw();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

struct Flags85 {
    u8 pad[85];
    u8 flags;
};

void MakyuriChojo_Func02003324(void)
{
    s32 ids[5];
    u8 *work;
    u32 i;
    u32 j;
    u32 n;
    s32 speed;
    u8 *actor;

    work = *(u8 **)0x03001e70 + 0x164;
    speed = 0x18000;
    n = 0;
    for (i = 0; i <= 4; i++)
        ids[i] = 66;
    Local_020031c0(ids, *(s32 *)(work + 12), work);
    for (i = 0; i <= 4; i++) {
        if (ids[i] == 66)
            break;
        ((struct Flags85 *)Engine_ActorGet(ids[i]))->flags = 0;
        n++;
    }
    Engine_AudioPlayCue(223);
    for (i = 0; i <= 85; i++) {
        *(s32 *)(work + 12) += speed;
        for (j = 0; j < n; j++) {
            *(s32 *)(Engine_ActorGet(ids[j]) + 16) -= speed;
            actor = Engine_ActorGet(ids[j]);
            *(s32 *)(actor + 64) = *(s32 *)(Engine_ActorGet(ids[j]) + 16);
        }
        if ((i & 3) == 3 && i > 75)
            speed += -0x3333;
        if (speed < 0xccc)
            speed = 0xccc;
        Engine_TaskWait(1);
    }
    *(s32 *)(work + 12) = 0x4000000;
    Engine_MapRedraw();
    Engine_TaskWait(2);
}
