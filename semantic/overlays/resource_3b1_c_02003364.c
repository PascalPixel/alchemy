typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 3b1 unindexed helper at 0x02003364 (440 bytes incl. pool,
 * 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,r7,lr}` + `r8` spill at 0x02003364, epilogue `pop {r3} / mov
 * r8,r3 / pop {r5,r6,r7} / pop {r0} / bx r0` at
 * 0x020034f2-0x020034fa. The trailing pool at 0x020034fc-0x02003518
 * (0x00000928, 0x00000266, 0x00000925, 0x00000921, 0x000001db,
 * 0x00000256, 0x0000026a, 0x00000293) ends exactly where the next
 * owner's `push {r5,r6,lr}` begins (0x0200351c, already this overlay's
 * row `0x0200351c | 1 call`), so the span is 0x02003364-0x0200351c,
 * 440 bytes.
 *
 * A `Value_XXXXXXXX`-gated cascade (same low-address family used
 * throughout this overlay): an early guard call bails out entirely if
 * unset, then an installed-object setup block (writing two handler
 * pointers, `0x0200812d`/`0x02008285`, into `+0x6c` fields — the same
 * "veneer address + 1" idiom documented for `Func_0200012c` in
 * assets/code/resource_3b1_c_0200012c.c) either runs and returns, or
 * falls through to a three-way else-if chain of simpler setter pairs,
 * the last of which is unconditional-once-reached (no further gate).
 *
 * Raw callee naming.
 */

extern u8 Value_00000911;
extern u8 Value_00000928;
extern u8 Value_0000093e;
extern u8 Value_00000109;
extern u8 Value_00000925;
extern u8 Value_00000921;

s32 Func_0200980e();
void Func_02007946(void);
s32 Func_02009824();
void Func_0200795c(void);
s32 Func_02009832();
s32 Func_02009842();
u8 *Func_02009894();
void Func_02007ca0();
void Func_02007c58();
void Func_02007c6e();
void Func_020098e0();
u8 *Func_020098f2();
s32 Func_020098ba();
void Func_02007ce2();
s32 Func_020098fa();
void Func_02007d16();
void Func_02007d78();
s32 Func_02009912();
s32 Func_02009938();
void Func_02007d3c();
void Func_02007d4c();
u8 *Func_020099aa();
u8 *Func_020099b6();
void Func_02007d70();
void Func_02007d80();

void Func_02003364(void)
{
    if (Func_0200980e((s32)&Value_00000911) == 0) {
        Func_02007946();
        return;
    }

    if (Func_02009824((s32)&Value_00000928) != 0) {
        Func_0200795c();
    }

    {
        s32 guard = Func_02009832((s32)&Value_0000093e);
        if (guard != 0) {
            return;
        }

        if (Func_02009842(0x8a0) != 0) {
            u8 *obj = Func_02009894(9);
            u8 *obj2;

            Func_02007ca0(13, 0, 0);
            Func_02007c58(8, 0x1c8, 0x28c, 0);
            Func_02007c6e(9, 0x1e0, 0x258, 0xb000);
            Func_020098e0(9, 0xcccc, 0x6666);
            *(u16 *)(obj + 0x66) = (u16)guard;
            obj[0x63] = 0;
            obj[0x59] |= 0x80;
            *(u32 *)(obj + 0x6c) = 0x0200812d;

            obj2 = Func_020098f2(8);
            obj2[0x62] = 0;
            *(u32 *)(obj2 + 0x6c) = 0x02008285;

            if (Func_020098ba((s32)&Value_00000109) != 0) {
                Func_02007ce2(0, 0x1e0, 0x29a, 0xa000);
            }
            return;
        }
    }

    if (Func_020098fa((s32)&Value_00000928) != 0) {
        Func_02007d16(8, 0x1bc, 0x266, 0xd000);
        Func_02007d78(13, 0, 0);
        return;
    }

    if (Func_02009912((s32)&Value_00000925) != 0) {
        Func_02007d16(8, 0x1c8, 0x288, 0);
        Func_02007d78(13, 0, 0);
        return;
    }

    if (Func_02009938((s32)&Value_00000921) != 0) {
        u8 *obj3;
        u8 *obj4;

        Func_02007d3c(8, 0x1db, 0x256, 0x8000);
        Func_02007d4c(9, 0x1ce, 0x26a, 0xb000);

        obj3 = Func_020099aa(12);
        *(u16 *)(obj3 + 6) = 0x3000;

        obj4 = Func_020099b6(11);
        *(u16 *)(obj4 + 6) = 0xb000;

        Func_02007d70(13, 0x1b6, 0x293, 0xd000);
        Func_02007d80(10, 0x1e8, 0x2b0, 0xb000);
    }
}
