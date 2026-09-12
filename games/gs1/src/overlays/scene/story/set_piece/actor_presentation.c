#include "types.h"

#define ActorPresentation_SetCellAndLowerActorEight Func_0200047c
#define SceneActor_SetActor10Byte23To3 Func_02000c74
#define SceneActor_SetActor10Byte23To1 Func_02000c88
#define SceneActor_SetMode3AndRate4ccc Func_02002a10
#define SceneActor_SetActor8ModeByCounterBit Func_02002ffc
#define SceneActor_SetActor12ModeByActorZeroHeight Func_02003428
extern unsigned char Value_00004ccc;
extern u32 Data_03001e40;

s32 *Func_0200401c(s32);
void Func_02003fc2(s32, s32, s32, s32, s32, s32);
void Func_02003fd6(s32 *, s32);
void Func_02004016(s32);
u8 *Func_02004812(s32);
u8 *Func_02004826(s32);
void Func_0200664e();
void Func_02006566();
void Func_02006c2a();
void Func_02006c34();
u8 *Func_02006fc6(s32 id);
u8 *Func_02006fd6(s32 id);
void Func_020070a8(s32 id, s32 arg1);
void Func_020070b2(s32 id, s32 arg1);

/*
 * Presentation setup in resource_3a4: clear a record byte, adjust two
 * handle flag bits, run two presentation primitives, then stamp a fixed
 * rate into the record.
 */

/* Declared without prototypes -- call sites vary in argument shape. */

/*
 * resource_3a4: a published callback that sets the mode of actor record 8.
 */

/*
 * Presentation callback for resource_3a4, published rather than called
 * directly from this overlay.
 */

void ActorPresentation_SetCellAndLowerActorEight(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Func_0200401c(8);
    s0 = 9;
    s1 = 13;
    Func_02003fc2(7, 13, 1, 1, s0, s1);
    if (p != 0) {
        Func_02003fd6(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Func_02004016(0x200);
}

void SceneActor_SetActor10Byte23To3(void)
{
    Func_02004812(10)[0x23] = 3;
}

void SceneActor_SetActor10Byte23To1(void)
{
    Func_02004826(10)[0x23] = 1;
}

/*
 * Clear the record byte at +0x55, then rewrite the handle flags at +9 as
 * (flags & ~0x0c) | 0x04 -- the mask is built from the zero already in v,
 * not spelled as a constant. The rate address is held in a local and
 * stored to both +24 and +28. The 50-byte owner includes its one pool
 * word.
 */
void SceneActor_SetMode3AndRate4ccc(u8 *rec)
{
    u8 *p = rec + 0x55;
    s32 v = 0;
    u8 *h;

    *p = v;
    h = *(u8 **)(rec + 80);
    v -= 13;
    v &= h[9];
    v |= 4;
    h[9] = (u8)v;
    Func_0200664e(rec, 3);
    Func_02006566(rec, 0);
    {
        s32 rate = (s32)&Value_00004ccc;

        *(s32 *)(rec + 24) = rate;
        *(s32 *)(rec + 28) = rate;
    }
}

/*
 * Bit 1 of the runtime status word at 0x03001e40 selects mode 7 or mode 6
 * for record 8. That bit's meaning is unverified: other callbacks here mask
 * different bits of the same word. Both branches reach the same veneer, and
 * the declarations carry no parameter list because the arguments are set up
 * in registers at the call site. The owner spans 44 bytes -- the body, one
 * alignment halfword and one literal pool word.
 */
void SceneActor_SetActor8ModeByCounterBit(void)
{
    if (((Data_03001e40 >> 1) & 1) != 0) {
        Func_02006c2a(8, 7);
    } else {
        Func_02006c34(8, 6);
    }
}

/*
 * Compare record 0's field at +12 against a signed threshold and set record
 * 12's mode accordingly; the taller branch also sets bit 1 of record 11's
 * byte at +35.  Both offsets are named by position only and their roles are
 * unverified; +35 is read-modify-written as a flags byte.  The threshold is
 * kept as the value 0x00300000 the code builds, in no assumed fixed-point
 * unit.
 */
void SceneActor_SetActor12ModeByActorZeroHeight(void)
{
    if (*(s32 *)(Func_02006fc6(0) + 12) > 0x00300000) {
        {
            u8 *flag = Func_02006fd6(11) + 35;
            s32 bit = 2;

            bit |= *flag;
            *flag = bit;
        }
        Func_020070a8(12, 3);
    } else {
        Func_020070b2(12, 2);
    }
}
