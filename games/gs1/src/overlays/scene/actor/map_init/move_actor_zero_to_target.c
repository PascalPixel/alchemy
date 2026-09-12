/*
 * Actor movement for overlay resource_3c4.  Declarations are left without
 * prototypes because Func_02003dec and Func_02003dca are each called twice
 * with different argument shapes.
 */
#include "types.h"

typedef struct Actor_02000cd0 {
    u8 pad0[6];
    u16 tag;               /* +6  */
    s32 x;                 /* +8,  12.20 fixed point */
    s32 y;                 /* +12 */
    s32 z;                 /* +16, 12.20 fixed point */
    u8 pad20[20];
    s32 speedZ;            /* +40 */
    s32 pad44;
    s32 speedX;            /* +48 */
    s32 speedY;            /* +52 */
    u8 pad56[29];
    u8 flags;              /* +85 */
} Actor_02000cd0;

typedef struct Target_02000cd0 {
    s32 x;                 /* +0, 12.20 fixed point */
    s32 pad4;
    s32 z;                 /* +8, 12.20 fixed point */
} Target_02000cd0;

Actor_02000cd0 *Func_02003dec_a();
s32 Func_02003d6a();
s32 Func_02003dca_a();
s32 Func_02003dca_b();
s32 Func_02003dd6();
void Func_02003e2e();
void Func_02003d96();
void Func_02003d6c();
void Func_02003efa();
void Func_02003daa();
void Func_02003e24();
void Func_02003eb4();
void Func_02003dec_b();
void Func_02003e4c();
void Func_02003eaa();

/*
 * Tries to move actor 0 onto the caller's target.  It builds a three-word
 * 12.20 probe from the record's own position -- the horizontal words snapped
 * to their whole-unit grid and lifted by half a unit -- asks the collision
 * service about it, and refuses when either the probe or the target is
 * rejected.  Returns 1 when refused and 0 when the move ran.  The 248-byte
 * owner includes its one pool word.
 */
s32 SceneActor_MoveActorZeroToTarget(const Target_02000cd0 *target)
{
    Actor_02000cd0 *actor = Func_02003dec_a(0);
    u8 saved = actor->flags;
    s32 probe[3];

    probe[0] = (actor->x & (s32)0xfff00000) + 0x00080000;
    probe[1] = actor->y;
    probe[2] = (actor->z & (s32)0xfff00000) + 0x00080000;

    Func_02003d6a(0x00100000, (actor->tag + 0x2000) & 0xc000, probe);

    /* Both guards branch to one shared exit placed after the body.  Writing
     * `return 1` twice would put an inline copy near the top instead. */
    if (Func_02003dca_a(actor, probe) == 1) {
        goto refuse;
    }
    if (Func_02003dd6(actor, target) != 0) {
        goto refuse;
    }

    Func_02003e2e();
    Func_02003d96(actor, 6);
    Func_02003d6c(6);
    Func_02003efa(152);
    Func_02003daa(actor, 7);

    actor->speedX = 0x00030000;
    actor->speedY = 0x00020000;
    actor->speedZ = 0x00040000;
    actor->flags &= (u8)0x7e;   /* masks the byte re-read here, not `saved` */

    Func_02003e24(actor, 0);
    Func_02003eb4(0, ((target->x >> 20) << 4) + 8, ((target->z >> 20) << 4) + 8);
    Func_02003dec_b(actor, 6);
    Func_02003e4c(actor, 1);
    Func_02003dca_b(6);

    actor->flags = saved;
    Func_02003eaa();
    return 0;

refuse:
    return 1;
}
