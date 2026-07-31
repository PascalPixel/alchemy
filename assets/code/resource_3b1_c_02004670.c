typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

/*
 * Resource 3b1 unindexed helper at 0x02004670 (348 bytes incl. pool,
 * 7 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,lr}` at 0x02004670, epilogue `pop {r5,r6} / pop {r0} / bx r0` at
 * 0x020047b0-0x020047b4. The trailing four-word pool at
 * 0x020047b6-0x020047c8 (0x00000928, 0x00000929, 0xffff0000, 0x0000092a,
 * 0x0000092b — five words, one is a shared padding read) ends exactly
 * where the next owner's `push {r5,lr}` begins (0x020047cc, already this
 * overlay's row `0x020047cc | 1 call`), so the span is
 * 0x02004670-0x020047cc, 348 bytes.
 *
 * Four near-identical "install or fallback" blocks, gated by
 * `Func_0200abNN((s32)&Value_XXXXXXXX)` checks against the same
 * `Value_XXXXXXXX` low-address family used throughout this overlay (see
 * assets/code/resource_3b1_c_02000b84.c). The second and fourth blocks
 * additionally stash the sentinel `0xffff0000` into the installed
 * object's `+24` field before the shared per-block finisher call.
 * `subject` (the incoming argument) is threaded through every block
 * unmodified.
 *
 * Raw callee naming.
 */

extern u8 Value_00000928;
extern u8 Value_00000929;
extern u8 Value_0000092a;
extern u8 Value_0000092b;

s32 Func_0200ab18();
s32 Func_0200ab58();
s32 Func_0200aba2();
s32 Func_0200abe2();

u8 *Func_0200962c();
void Func_0200abda();
void Func_02008f84();
void Func_0200abee();
void Func_02008f9a();

u8 *Func_0200966c();
void Func_0200ac1a();
u8 *Func_0200abc0();
void Func_02008fce();
void Func_0200ac38();
void Func_02008fe4();

u8 *Func_020096b6();
void Func_0200ac64();
void Func_0200900e();
void Func_0200ac78();
void Func_02009024();

u8 *Func_020096f6();
void Func_0200aca4();
u8 *Func_0200ac4a();
void Func_02009058();
void Func_0200acc2();
void Func_0200906e();

void Func_02009078();
void Func_02009082();
void Func_0200908c();
void Func_02009096();

void Func_02004670(u8 *subject)
{
    if (Func_0200ab18((s32)&Value_00000928) != 0) {
        u8 *obj = Func_0200962c(0, 0);
        Func_0200abda(obj, 0xcd << 17, 0xac << 16);
        Func_02008f84(7, obj, subject);
        Func_0200abee(10, 0, 0);
    } else {
        Func_02008f9a(5, 10, subject);
    }

    if (Func_0200ab58((s32)&Value_00000929) != 0) {
        u8 *obj = Func_0200966c(1, 0);
        Func_0200ac1a(obj, 0xeb << 17, 0xac << 16);
        *(u32 *)(Func_0200abc0(obj) + 24) = 0xffff0000;
        Func_02008fce(7, obj, subject);
        Func_0200ac38(11, 0, 0);
    } else {
        Func_02008fe4(6, 11, subject);
    }

    if (Func_0200aba2((s32)&Value_0000092a) != 0) {
        u8 *obj = Func_020096b6(2, 0);
        Func_0200ac64(obj, 0xcd << 17, 0xcc << 16);
        Func_0200900e(7, obj, subject);
        Func_0200ac78(12, 0, 0);
    } else {
        Func_02009024(5, 12, subject);
    }

    if (Func_0200abe2((s32)&Value_0000092b) != 0) {
        u8 *obj = Func_020096f6(3, 0);
        Func_0200aca4(obj, 0xeb << 17, 0xcc << 16);
        *(u32 *)(Func_0200ac4a(obj) + 24) = 0xffff0000;
        Func_02009058(7, obj, subject);
        Func_0200acc2(13, 0, 0);
    } else {
        Func_0200906e(6, 13, subject);
    }

    Func_02009078(5, 14, subject);
    Func_02009082(6, 15, subject);
    Func_0200908c(5, 16, subject);
    Func_02009096(6, 17, subject);
}
