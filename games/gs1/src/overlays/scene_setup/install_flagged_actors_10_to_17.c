#include "types.h"

/* Scene setup for resource_3b1: installs actors 10 through 17. */

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

/*
 * Four "install or fallback" blocks, each gated by a flag check, then four
 * unconditional installs.  src is threaded through every block unmodified,
 * and blocks two and four store the 0xffff0000 sentinel into the installed
 * object's +24 field.  The owner includes the trailing pool words that hold
 * the flag addresses and the sentinel.  Callees are named by the address
 * their call site computes, not by a runtime address.
 */
void FieldScene_InstallFlaggedActors10To17(u8 *src)
{
    if (Func_0200ab18((s32)&Value_00000928) != 0) {
        u8 *obj = Func_0200962c(0, 0);
        Func_0200abda(obj, 0xcd << 17, 0xac << 16);
        Func_02008f84(7, obj, src);
        Func_0200abee(10, 0, 0);
    } else {
        Func_02008f9a(5, 10, src);
    }

    if (Func_0200ab58((s32)&Value_00000929) != 0) {
        u8 *obj = Func_0200966c(1, 0);
        Func_0200ac1a(obj, 0xeb << 17, 0xac << 16);
        *(u32 *)(Func_0200abc0(obj) + 24) = 0xffff0000;
        Func_02008fce(7, obj, src);
        Func_0200ac38(11, 0, 0);
    } else {
        Func_02008fe4(6, 11, src);
    }

    if (Func_0200aba2((s32)&Value_0000092a) != 0) {
        u8 *obj = Func_020096b6(2, 0);
        Func_0200ac64(obj, 0xcd << 17, 0xcc << 16);
        Func_0200900e(7, obj, src);
        Func_0200ac78(12, 0, 0);
    } else {
        Func_02009024(5, 12, src);
    }

    if (Func_0200abe2((s32)&Value_0000092b) != 0) {
        u8 *obj = Func_020096f6(3, 0);
        Func_0200aca4(obj, 0xeb << 17, 0xcc << 16);
        *(u32 *)(Func_0200ac4a(obj) + 24) = 0xffff0000;
        Func_02009058(7, obj, src);
        Func_0200acc2(13, 0, 0);
    } else {
        Func_0200906e(6, 13, src);
    }

    Func_02009078(5, 14, src);
    Func_02009082(6, 15, src);
    Func_0200908c(5, 16, src);
    Func_02009096(6, 17, src);
}
