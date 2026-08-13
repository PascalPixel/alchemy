#include "types.h"

/*
 * resource_39e owner at 0x020027ec, 740 bytes: a dialogue-heavy
 * scripted beat inside the Func_0808a018/Func_0808a020 bracket, built
 * on the shared 0x03001ebc IWRAM workspace. Three structural features:
 *
 * 1. Opening writes *(s32 *)(workspace + 448) = 256 -- the recurring
 *    workspace+448 idiom with yet ANOTHER value (256 = 448 - 192,
 *    computed subtractively where the 513/514 owners add); transcribed
 *    from the arithmetic per the 8b4 precedent.
 * 2. Two Func_0808a070(0, 0) gates in the skip-beat family, each
 *    bumping the u16 skip-beat counter at workspace + 472 -- but with
 *    UNEQUAL increments on the first gate (==0 arm +2, else arm +1;
 *    the second gate bumps +1 in both arms), unlike the
 *    equal-increment shape settled in the 3c9 drafts. Transcribed
 *    as-is.
 * 3. A repeat-prompt poll loop: its first physical body uses dialogue
 *    0x17c8, then a second test/body site repeats dialogue 0x17e0 while
 *    Func_0808a070(0, 0) remains 1; it falls out to dialogue 0x17c9.
 *
 * Closes with Func_0808a100(8, 5), story flag 0x893 set
 * (Func_080770c8), and the bracket close. Called 1x in this overlay.
 *
 * Complete owner: `push {r5, lr}` at 0x020027ec through `pop {r5} /
 * pop {r0} / bx r0` at 0x02002aae-0x02002ab2, seven pool words ending
 * 0x02002acf; the next owner prologue at 0x02002ad0 is the overlay's
 * remaining monster row. Not found by the structural inventory walk
 * (unindexed): reached only by bl (cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
 * resource_39e 27ec 2ab2, the +2 rule).
 *
 * Uncertainty: callees and argument roles by shape; dialogue ids and
 * constants transcribed.
 */

extern void Func_02006bcc();
extern void Func_02006c12();
extern void Func_02006c36();
extern void Func_02006d40();
extern void Func_02006d54();
extern void Func_02006d54_b();
extern void Func_02006c6a();
extern void Func_02006c00();
extern void Func_02006ca0();
extern void Func_02006d12();
extern void Func_02006c18();
extern u8 *Func_02006c4e();
extern void Func_02006dd2();
extern u8 *Func_02006c60();
extern void Func_02006cbe();
extern void Func_02006d0c();
extern void Func_02006d2e();
extern void Func_02006d38();
extern void Func_02006ce8();
extern void Func_02006c66();
extern void Func_02006cf6();
extern void Func_02006c74();
extern void Func_02006d5e();
extern void Func_02006d80();
extern void Func_02006d80_b();
extern void Func_02006d60();
extern s32 Func_02006cb8();
extern void Func_02006ca2();
extern void Func_02006d32();
extern void Func_02006cb0();
extern void Func_02006d9a();
extern void Func_02006cd0();
extern void Func_02006d78();
extern void Func_02006cde();
extern void Func_02006dd6();
extern void Func_02006d86();
extern void Func_02006d04();
extern void Func_02006dee();
extern void Func_02006df8();
extern void Func_02006dc0();
extern void Func_02006d26();
extern void Func_02006e10();
extern void Func_02006e32();
extern void Func_02006e12();
extern s32 Func_02006d6a();
extern void Func_02006e58();
extern void Func_02006e2e();
extern void Func_02006e3e();
extern s32 Func_02006d96();
extern void Func_02006e84();
extern void Func_02006e5a();
extern void Func_02006e6a();
extern void Func_02006e6a_b();
extern void Func_02006da8();
extern void Func_02006e38();
extern void Func_02006db6();
extern void Func_02006e8e();
extern s32 Func_02006de6();
extern void Func_02006dd0();
extern void Func_02006e60();
extern void Func_02006e60_b();
extern void Func_02006dde();
extern void Func_02006ec8();
extern void Func_02006e00();
extern void Func_02006ea8();
extern void Func_02006f02();
extern void Func_02006eb2();
extern void Func_02006e30();
extern void Func_02006f1a();
extern void Func_02006ee2();
extern void Func_02006e48();
extern void Func_02006f32();
extern void Func_02006ef0();
extern void Func_02006e6e();
extern void Func_02006ef6();
extern void Func_02006e6c();
extern void Func_02006e90();
extern u8 *Data_03001ebc;

void Func_020027ec(void)
{
    u8 *workspace;
    u8 *record;

    Func_02006bcc();
    Func_02006c12(0, 128 << 8, 128 << 7);
    Func_02006c36(0, 168, 252 << 1);
    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 448 - 192;
    Func_02006d40();
    Func_02006d54();
    Func_02006c6a(0);
    Func_02006c00(20);
    Func_02006ca0(8, 2);
    Func_02006d12(8, 0x102);
    Func_02006c18(60);
    record = Func_02006c4e(8);
    record[91] = 0;
    Func_02006dd2(152);
    *(s32 *)(record + 40) = 128 << 12;
    record = Func_02006c60(8);
    Func_02006cbe(8, 1);
    Func_02006d0c(0x17be);
    Func_02006d2e(8, 0, 20);
    Func_02006d38(8, 0, 20);
    Func_02006ce8(0, 3);
    Func_02006c66(20);
    Func_02006cf6(8, 3);
    Func_02006c74(20);
    Func_02006d5e(8, 0, 20);
    Func_02006d80(0, 0x101, 60);
    Func_02006d60(8, 0);
    if (Func_02006cb8(0, 0) == 0) {
        Func_02006ca2(10);
        *(u16 *)(workspace + 472) += 2;
        Func_02006d32(8, 3);
        Func_02006cb0(20);
        Func_02006d9a(8, 0, 20);
    } else {
        Func_02006cd0(10);
        Func_02006d78(8, 2);
        Func_02006cde(20);
        *(u16 *)(workspace + 472) += 1;
        Func_02006dd6(8, 0, 20);
        Func_02006d86(8, 3);
        Func_02006d04(20);
        Func_02006dee(8, 0, 20);
    }
    Func_02006df8(8, 0, 20);
    Func_02006dc0(8, 2);
    Func_02006d26(20);
    Func_02006e10(8, 0, 20);
    Func_02006e32(0, 0x101, 60);
    Func_02006e12(8, 0);
    if (Func_02006d6a(0, 0) == 1) {
        Func_02006d54_b(10);
        Func_02006e58(8, 0x102, 60);
        Func_02006e2e(0x17c8);
        Func_02006e3e(8, 0);
repeatPrompt:
        if (Func_02006d96(0, 0) == 1) {
            Func_02006d80_b(10);
            Func_02006e84(8, 0x102, 60);
            Func_02006e5a(0x17e0);
            goto repeatPrompt;
            Func_02006e6a(8, 0);
        }
    }
    Func_02006e6a_b(0x17c9);
    Func_02006da8(10);
    Func_02006e38(8, 3);
    Func_02006db6(20);
    Func_02006e8e(8, 0);
    if (Func_02006de6(0, 0) == 0) {
        Func_02006dd0(10);
        Func_02006e60(0, 3);
        Func_02006dde(20);
        Func_02006ec8(8, 0, 20);
        *(u16 *)(workspace + 472) += 1;
    } else {
        *(u16 *)(workspace + 472) += 1;
        Func_02006e00(10);
        Func_02006ea8(8, 2);
        Func_02006f02(8, 0, 20);
    }
    Func_02006eb2(8, 3);
    Func_02006e30(20);
    Func_02006f1a(8, 0, 20);
    Func_02006ee2(8, 2);
    Func_02006e48(20);
    Func_02006f32(8, 0, 20);
    Func_02006ee2(0, 3);
    Func_02006e60_b(20);
    Func_02006ef0(8, 3);
    Func_02006e6e(20);
    Func_02006ef6(8, 5);
    Func_02006e6c(0x893);
    Func_02006e90();
}
