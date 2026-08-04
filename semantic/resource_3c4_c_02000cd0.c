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

void Func_080000c0();
void Func_08000128();
void Func_08009080();
s32 Func_080091d8();
void Func_080091e0();
void Func_0808a018();
void Func_0808a020();
Actor_02000cd0 *Func_0808a080();
void Func_0808a0c0();
void Func_080f9010();

s32 Func_02000cd0(const Target_02000cd0 *target)
{
    Actor_02000cd0 *actor = Func_0808a080(0);
    u8 savedFlags = actor->flags;
    s32 probe[3];

    probe[0] = (actor->x & (s32)0xfff00000) + 0x00080000;
    probe[1] = actor->y;
    probe[2] = (actor->z & (s32)0xfff00000) + 0x00080000;

    Func_08000128(0x00100000, (actor->tag + 0x2000) & 0xc000, probe);

    if (Func_080091d8(actor, probe) == 1) {
        return 1;
    }
    if (Func_080091d8(actor, target) != 0) {
        return 1;
    }

    Func_0808a018();
    Func_08009080(actor, 6);
    Func_080000c0(6);
    Func_080f9010(152);
    Func_08009080(actor, 7);

    actor->speedX = 0x00030000;
    actor->speedY = 0x00020000;
    actor->speedZ = 0x00040000;
    actor->flags &= (u8)0x7e;   /* re-read from memory, not from the saved copy */

    Func_080091e0(actor, 0);
    Func_0808a0c0(0, ((target->x >> 20) << 4) + 8, ((target->z >> 20) << 4) + 8);
    Func_08009080(actor, 6);
    Func_080091e0(actor, 1);
    Func_080000c0(6);

    actor->flags = savedFlags;
    Func_0808a020();
    return 0;
}
