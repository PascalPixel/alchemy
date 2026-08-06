#include "types.h"

/*
 * Resource 3c4 owner at 0x02001aba (370 bytes).
 *
 * The exported entry includes the leading `movs r0,r0` at 0x02001aba before
 * the saved-register prologue at 0x02001abc.  The matching high-register
 * unwind returns at 0x02001c1c, followed by alignment and the three referenced
 * pool words through 0x02001c2b.  The next independent owner starts at
 * 0x02001c2c.
 *
 * All 26 calls resolve through per-site veneers, so every callee is named at
 * its decoded per-site address.  Two veneers serve two sites each with
 * different argument shapes (Func_02004c92, Func_02004c9c); they are declared
 * without prototypes, which is what the call sites jointly establish.
 * Actor fields use the layout established by the adjacent owners: 12.20
 * x/y/z at +8/+12/+16 and a signed height-table index at +100.
 *
 * The height table is indexed through its symbol directly: the CSE'd pool
 * address is the base operand of every reg-offset load (a pointer local
 * would be swapped behind the shifted index by expand_binop), and the
 * per-slot lookup temporaries are block-scoped so their pseudos stay two-ref
 * and the target pointer sinks to sl above the delay.
 *
 * The two-stage negative-y test deliberately repeats the actor lookup.  The
 * first adjusted value decides whether more work is needed; the second is
 * shifted to a whole-unit threshold.  Keeping both reads preserves the four
 * lookup sites inside the loop rather than inventing a cached value.
 */

typedef struct Actor_02001aba {
    u8 pad0[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[80];
    s16 height_index;
} Actor_02001aba;

extern s32 Data_0200b350[];

void Func_02004bea();
void Func_02004bf8();
void Func_02004c9c();
Actor_02001aba *Func_02004c02();
Actor_02001aba *Func_02004c0a();
Actor_02001aba *Func_02004c16();
Actor_02001aba *Func_02004c22();
void Func_02004b9a();
Actor_02001aba *Func_02004c38();
Actor_02001aba *Func_02004c40();
Actor_02001aba *Func_02004c48();
Actor_02001aba *Func_02004c54();
void Func_02004bcc();
Actor_02001aba *Func_02004c92();
void Func_02004c98();
Actor_02001aba *Func_02004c76();
Actor_02001aba *Func_02004c7e();
Actor_02001aba *Func_02004c9a();
void Func_02004d54();
Actor_02001aba *Func_02004cc0();
Actor_02001aba *Func_02004cd4();
Actor_02001aba *Func_02004cee();
Actor_02001aba *Func_02004cf6();
void Func_02004cf8();

void Func_02001aba(s32 delay)
{
    Actor_02001aba *actor;
    s32 *table;
    s32 height;
    s32 x;
    s32 z;
    u32 i;

    Func_02004bea(10, 0x8000, 0x4000);
    Func_02004bf8(11, 0x8000, 0x4000);

    if (delay != 0) {
        Func_02004c9c(180);
    }

    {
        Actor_02001aba *target = Func_02004c02(10);
        Actor_02001aba *px = Func_02004c0a(10);
        Actor_02001aba *pz;
        s32 index;
        s32 offset;

        table = Data_0200b350;
        index = Func_02004c16(10)->height_index;
        pz = Func_02004c22(10);
        offset = index * 4;
        Func_02004b9a(target, px->x, *(s32 *)((u8 *)table + offset), pz->z);
    }

    {
        Actor_02001aba *target = Func_02004c38(11);
        Actor_02001aba *px = Func_02004c40(11);
        Actor_02001aba *pz;
        s32 index = Func_02004c48(11)->height_index;
        s32 offset;

        pz = Func_02004c54(11);
        offset = index * 4;
        Func_02004bcc(target, px->x, *(s32 *)((u8 *)table + offset), pz->z);
    }

    Func_02004c92(10);
    Func_02004c98(11);

    actor = Func_02004c76(10);
    {
        s32 offset = Func_02004c7e(10)->height_index * 4;
        actor->y = *(s32 *)((u8 *)table + offset);
    }

    actor = Func_02004c92(11);
    {
        s32 offset = Func_02004c9a(11)->height_index * 4;
        actor->y = *(s32 *)((u8 *)table + offset);
    }

    if (delay != 0) {
        Func_02004d54(0x121);
    }

    for (i = 0; i <= 4; i++) {
        s32 slot = i + 10;

        height = Func_02004cc0(slot)->y;
        if (height < 0) {
            height += 0xffff;
        }

        if (height < 0) {
            height = Func_02004cd4(slot)->y;
            if (height < 0) {
                height += 0xffff;
            }

            if ((height >> 16) > -30) {
                x = Func_02004cee(slot)->x;
                z = Func_02004cf6(slot)->z >> 20;
                Func_02004c9c(4, 9, 1, 1, x >> 20, z);
            }
        }
    }

    Func_02004cf8(delay);
}
