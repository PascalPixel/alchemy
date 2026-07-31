typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3b1 unindexed helper at 0x0200351c (336 bytes incl. pool,
 * 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,lr}` + `sl`/`r8` spill at 0x0200351c, epilogue `pop {r3,r5} /
 * mov r8,r3 / mov sl,r5 / pop {r5,r6} / pop {r0} / bx r0` at
 * 0x0200363e-0x02003648. The trailing pool at 0x0200364c-0x02003668
 * (0x0000093e, 0x0200e958, 0x0000020e, 0x0200e840, 0x00000928,
 * 0x00000925, 0x00000911, 0x00000922) ends exactly where the next
 * owner's `push {lr}` begins (0x0200366c, already this overlay's row
 * `0x0200366c | 1 call`), so the span is 0x0200351c-0x0200366c,
 * 336 bytes.
 *
 * A four-way cascade of `Value_XXXXXXXX`-gated blocks (same low-address
 * family as 0x02004670's blocks earlier in this overlay), each either
 * running its own setter sequence and returning, or falling through to
 * the next gate. All four share one exit.
 *
 * Raw callee naming.
 */

extern u8 Value_0000093e;
extern u8 Value_00000928;
extern u8 Value_00000925;
extern u8 Value_00000911;
extern u8 Value_00000922;

s32 Func_020099c8();
void Func_02009a7e();
void Func_02009a88();
void Func_02009a92();
void Func_02009a9c();
void Func_02009aa6();
void Func_02007efa();

s32 Func_02009a0a();
void Func_02007e0e();
void Func_02009a86();
void Func_02007e32();
void Func_02007e40();
void Func_02007e4c();
void Func_02007e58();
void Func_02007e64();
void Func_02007e74();
void Func_02009aec();

s32 Func_02009a94();
void Func_02008600();
s32 Func_02009aa4();

s32 Func_02009aba();
s32 Func_02009ac4();
void Func_02007f1a();
void Func_02009b84();

void Func_0200351c(void)
{
    if (Func_020099c8((s32)&Value_0000093e) != 0) {
        Func_02009a7e(8, 0, 0);
        Func_02009a88(9, 0, 0);
        Func_02009a92(10, 0, 0);
        Func_02009a9c(11, 0, 0);
        Func_02009aa6(12, 0, 0);
        Func_02007efa(14, 0, 0);
        return;
    }

    if (Func_02009a0a(0x8a0) != 0) {
        Func_02007e0e(8, 0x98, 0x1bc, 0x3000);
        Func_02009a86(8, 0x0200e958);
        Func_02007e32(10, 0xb8, 0x1e0, 0xb000);
        Func_02007e40(12, 0xaa, 0x1e8, 0xb000);
        Func_02007e4c(13, 0x88, 0x1e8, 0xd000);
        Func_02007e58(15, 0x78, 0x1e0, 0xd000);
        Func_02007e64(14, 0xb8, 0x20e, 0xb000);
        Func_02007e74(11, 0x88, 0x248, 0x8000);
        Func_02009aec(11, 0x0200e840);
        return;
    }

    {
        s32 t = Func_02009a94((s32)&Value_00000928);
        if (t != 0) {
            Func_02008600(t);
            return;
        }
    }

    if (Func_02009aa4((s32)&Value_00000925) != 0) {
        Func_02007efa(18, 0, 0);
        return;
    }

    if (Func_02009aba((s32)&Value_00000911) != 0 &&
        Func_02009ac4((s32)&Value_00000922) != 0) {
        Func_02007f1a(14, 0, 0);
        Func_02009b84(12, 0, 0);
    }
}
