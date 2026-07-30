/*
 * resource_3c4 @ 0x02000cd0 (248 bytes: 244 code + one pool word).
 *
 * Tries to move the actor record to the caller's target position.  It builds a
 * three-word 12.20 probe on the stack from the record's own position — the
 * horizontal words are snapped to their whole-unit grid with the pool mask
 * 0xfff00000 and then lifted by half a unit (0x80 << 12 = 0x00080000) — asks
 * the collision service about it, and gives up when either the probe or the
 * target is rejected.
 *
 * When both pass it runs the move: a fixed sequence of service calls, three
 * speed words written at +48, +52 and +40 (0x00030000, 0x00020000 and
 * 0x00040000), the flag byte at +85 masked with 0x7e, and finally the target
 * converted from 12.20 into the coarse tile coordinates the placement service
 * wants — `asrs #20`, `lsls #4`, `+8`, i.e. the centre of a sixteen-unit cell.
 * The flag byte is restored from its saved value (held in sl across the whole
 * body) before returning.
 *
 * The return value is observable: `pop {r3, r5} ; ... ; pop {r1} ; bx r1`
 * preserves r0, so this returns 1 when the move was refused and 0 when it ran.
 *
 * Func_02003dec and Func_02003dca are each called twice with different
 * argument shapes, so they are declared without prototypes: only what the two
 * call sites jointly establish is asserted.
 */
typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

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

Actor_02000cd0 *Func_02003dec();
void Func_02003d6a(s32 kind, s32 mask, const s32 *probe);
s32 Func_02003dca();
s32 Func_02003dd6(Actor_02000cd0 *actor, const Target_02000cd0 *target);
void Func_02003e2e(void);
void Func_02003d96(Actor_02000cd0 *actor, s32 arg1);
void Func_02003d6c(s32 arg0);
void Func_02003efa(s32 arg0);
void Func_02003daa(Actor_02000cd0 *actor, s32 arg1);
void Func_02003e24(Actor_02000cd0 *actor, s32 arg1);
void Func_02003eb4(s32 arg0, s32 x, s32 z);
void Func_02003e4c(Actor_02000cd0 *actor, s32 arg1);
void Func_02003eaa(void);

s32 Func_02000cd0(const Target_02000cd0 *target)
{
    Actor_02000cd0 *actor = Func_02003dec(0);
    u8 savedFlags = actor->flags;
    s32 probe[3];

    probe[0] = (actor->x & (s32)0xfff00000) + 0x00080000;
    probe[1] = actor->y;
    probe[2] = (actor->z & (s32)0xfff00000) + 0x00080000;

    Func_02003d6a(0x00100000, (actor->tag + 0x2000) & 0xc000, probe);

    if (Func_02003dca(actor, probe) == 1) {
        return 1;
    }
    if (Func_02003dd6(actor, target) != 0) {
        return 1;
    }

    Func_02003e2e();
    Func_02003d96(actor, 6);
    Func_02003d6c(6);
    Func_02003efa(152);
    Func_02003daa(actor, 7);

    actor->speedX = 0x00030000;
    actor->speedY = 0x00020000;
    actor->speedZ = 0x00040000;
    actor->flags &= (u8)0x7e;   /* re-read from memory, not from the saved copy */

    Func_02003e24(actor, 0);
    Func_02003eb4(0, ((target->x >> 20) << 4) + 8, ((target->z >> 20) << 4) + 8);
    Func_02003dec(actor, 6);
    Func_02003e4c(actor, 1);
    Func_02003dca(6);

    actor->flags = savedFlags;
    Func_02003eaa();
    return 0;
}
