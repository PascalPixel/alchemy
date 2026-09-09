#include "types.h"

/*
 * resource_3c4: land an actor on the highest platform sharing its tile.
 */

typedef struct Slot_02001c2c {
    u8 pad0[8];
    s32 x;                 /* +8,  12.20 fixed point */
    s32 y;                 /* +12 */
    s32 z;                 /* +16, 12.20 fixed point */
} Slot_02001c2c;

/*
 * Each site names the call word it holds, so the same slot accessor appears
 * here under several symbols. The declarations are old-style because the call
 * sites vary in arity.
 */
Slot_02001c2c *Func_02004d56();
Slot_02001c2c *Func_02004d5e();
Slot_02001c2c *Func_02004d6e();
Slot_02001c2c *Func_02004d76();
Slot_02001c2c *Func_02004d86();
Slot_02001c2c *Func_02004d98();
Slot_02001c2c *Func_02004da8();
void Func_02004dcc();
Slot_02001c2c *Func_02004dca();
Slot_02001c2c *Func_02004dd2();
Slot_02001c2c *Func_02004dda();
void Func_02004d4e();
void Func_02004e14();
void Func_02004e92();
void Func_02002ecc();
void Func_02004dde();

/*
 * Scan slots 10 to 14, skipping the subject, keep those whose whole-tile x
 * and z match the subject's, and take the greatest height among them; the
 * winning slot index goes into the subject's tag at +100. The starting best
 * height is -5.0 in 12.20, held in the owner's one pool word, and it is what
 * the move receives when no slot matches -- the tag is then left untouched.
 * Every field read re-fetches its record, which is the shape to keep.
 */
void SceneActor_LandOnHighestPlatform(s32 subject)
{
    s32 best = (s32)0xffb00000;
    u32 i;

    for (i = 0; i <= 4; i++) {
        s32 slot = i + 10;

        if (slot == subject) continue;

        if ((Func_02004d56(slot)->x >> 20) != (Func_02004d5e(subject)->x >> 20)) continue;
        if ((Func_02004d6e(slot)->z >> 20) != (Func_02004d76(subject)->z >> 20)) continue;

        /*
         * 0x00100000 is one whole unit above the candidate's own height. The
         * comparison is signed, and a tie updates the best.
         */
        if (best > Func_02004d86(slot)->y + 0x100000) continue;

        best = Func_02004d98(slot)->y + 0x100000;
        *(u16 *)((u8 *)Func_02004da8(subject) + 100) = (u16)slot;
    }

    Func_02004dcc(subject, 0x40000, 0x20000);   /* 128 << 11, 128 << 10 */

    /*
     * Three separate lookups of the same record, in this order. The locals
     * fix the sequence, which argument evaluation order would not.
     */
    {
        Slot_02001c2c *target = Func_02004dca(subject);
        Slot_02001c2c *from = Func_02004dd2(subject);
        s32 z = Func_02004dda(subject)->z;

        Func_02004d4e(target, from->x, best, z);
    }

    Func_02004e14(subject);
    Func_02004e92(188);
    Func_02002ecc(subject);
    Func_02004dde(30);
}
