#include "types.h"

/*
 * resource_39e owner at 0x02001494, 2236 bytes: the overlay's second
 * ensemble beat -- 233 calls, one inline literal pool skipped by the
 * forward branch at 0x0200188e (the a010(20) call at the skip target
 * completes the instruction pair split around the pool), and TWO
 * Func_0808a070(0, 0) gates near the end. The gates settle this
 * overlay's story-flag chain:
 *   - opening sets flag 0x89a (Func_080770c8);
 *   - gate 1 ==0 arm joins with id 16 and sets flag 0x898 -- the flag
 *     the dispatcher at 0x020012e0 tests and 0x02001dbc later clears;
 *   - gate 1 else arm bumps the workspace+472 skip-beat counter
 *     (0x03001ebc) and runs gate 2: its ==0 arm joins with id 18 and
 *     also sets 0x898, its else arm bumps the counter again and sets
 *     flag 0x899 instead.
 * Both join paths share the machine-level tail at 0x02001cb4
 * (a188(id, 0, 20) with id carried in r0 from either arm -- spelled
 * out per-arm here). Midway: record 16's byte +90 gets bit 0 cleared
 * then set around a Func_0808a0d0 reposition, sound 158/159 with
 * Func_08009178(0x0200c77a/0x0200c790, 78, 13) text calls, the
 * slot-19 sequence (a0f0(19/20, 232<<16, 168<<16), record +12 =
 * 0xc0000, +60 = 0x80000000, +24 = 0xcccc, BAM +30 of the +80
 * sub-object = 0x8000, sound 124) identical to the 0x1160/0x1dbc
 * closers, and a Func_0808a098(12, 0x0200c638) matching 0x1dbc's.
 * Called 1x within this overlay.
 *
 * Complete owner: `push {r5, lr}` at 0x02001494 through `pop {r5} /
 * pop {r0} / bx r0` at 0x02001d42-0x02001d46, two inline pools plus
 * two trailing pool words ending 0x02001d4f; the next owner
 * (0x02001d50, `push {lr}`) follows. Not found by the structural
 * inventory walk (unindexed): reached only by bl
 * (cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_39e 1494 1d46, +2 rule).
 *
 * Uncertainty: callees and argument roles by shape; ids, dialogue ids
 * and constants transcribed. 0x0200c77a/0x0200c790/0x0200c638 are
 * overlay-image data pointers, targets not identified.
 */

extern void Func_0808a010(s32 frames);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a088(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, s32 arg1);
extern void Func_0808a0c8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0e8(s32 id);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a148(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a170(s32 dialogue_id);
extern void Func_0808a178(s32 id, s32 arg1);
extern void Func_0808a188(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a190(s32 id, s32 arg1);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a200(s32 id, s32 arg1);
extern void Func_08009178(s32 arg0, s32 arg1, s32 arg2);
extern void Func_080770c8(s32 flagId);
extern void Func_080f9010(s32 sound_id);
/* Overlay-local relocation aliases witnessed from the reference BL stream.
 * Suffixes distinguish different logical prototypes sharing one address. */
extern void Func_0200585e();
extern void Func_02005874();
extern void Func_02005926();
extern void Func_02005930();
extern void Func_0200593a();
extern void Func_02005898();
extern void Func_0200599c();
extern void Func_020059a8();
extern void Func_020059b4();
extern void Func_020058c2();
extern void Func_02005990();
extern void Func_020059b2();
extern void Func_02005984();
extern void Func_02005984_b();
extern void Func_020058ea();
extern void Func_020059d4();
extern void Func_020059a6();
extern void Func_020059a6_b();
extern void Func_0200590c();
extern void Func_020059be();
extern void Func_02005a06();
extern void Func_02005924();
extern void Func_02005a34();
extern void Func_02005976();
extern void Func_020059a0();
extern void Func_020059ac();
extern void Func_02005a48();
extern void Func_02005a54();
extern void Func_02005b12();
extern void Func_02005934();
extern void Func_02005a1c();
extern void Func_02005982();
extern void Func_020059d0();
extern u8 *Func_020059c6();
extern void Func_02005a0e();
extern void Func_020059b4_b();
extern u8 *Func_020059ea();
extern void Func_02005aae();
extern void Func_02005a54_b();
extern void Func_02005a48_b();
extern void Func_02005a9a();
extern void Func_02005aa4();
extern void Func_02005aae_b();
extern void Func_02005ab8();
extern void Func_02005ac2();
extern void Func_02005acc();
extern void Func_02005ad6();
extern void Func_02005ae0();
extern void Func_02005aea();
extern void Func_02005a48_c();
extern void Func_02005ae8();
extern void Func_02005af0();
extern void Func_02005af8();
extern void Func_02005b00();
extern void Func_02005b08();
extern void Func_02005b10();
extern void Func_02005b18();
extern void Func_02005b28();
extern void Func_02005b8a();
extern void Func_02005b18_b();
extern void Func_02005b04();
extern void Func_02005b12_b();
extern void Func_02005b30();
extern void Func_02005bbc();
extern void Func_02005b42();
extern void Func_02005c80();
extern void Func_02005aa2();
extern void Func_02005bcc();
extern void Func_02005b9e();
extern void Func_02005ba8();
extern void Func_02005bb2();
extern void Func_02005bbc_b();
extern void Func_02005bc6();
extern void Func_02005bd0();
extern void Func_02005bda();
extern void Func_02005be4();
extern void Func_02005bee();
extern void Func_02005b4c();
extern void Func_02005bf4();
extern void Func_02005b5a();
extern void Func_02005bea();
extern void Func_02005b68();
extern void Func_02005c52();
extern void Func_02005c02();
extern void Func_02005b80();
extern void Func_02005c28();
extern void Func_02005b8e();
extern void Func_02005c78();
extern void Func_02005c94();
extern void Func_02005c8e();
extern void Func_02005cb2();
extern void Func_02005ca4();
extern void Func_02005c54();
extern void Func_02005bd2();
extern void Func_02005cbc();
extern void Func_02005c6c();
extern void Func_02005bea_b();
extern void Func_02005cd4();
extern void Func_02005c84();
extern void Func_02005c02_b();
extern void Func_02005c92();
extern void Func_02005c10();
extern void Func_02005cfa();
extern void Func_02005cc2();
extern void Func_02005c28_b();
extern void Func_02005d22();
extern void Func_02005d1c();
extern void Func_02005d38();
extern void Func_02005cd8();
extern void Func_02005c56();
extern void Func_02005ce6();
extern void Func_02005c82();
extern void Func_02005d6c();
extern void Func_02005d1c_b();
extern void Func_02005c9a();
extern void Func_02005d84();
extern void Func_02005da0();
extern void Func_02005d9a();
extern void Func_02005d4a();
extern void Func_02005cc8();
extern void Func_02005db2();
extern void Func_02005dce();
extern void Func_02005d86();
extern void Func_02005cec();
extern void Func_02005dd6();
extern void Func_02005dfa();
extern void Func_02005dec();
extern void Func_02005e0e();
extern void Func_02005e00();
extern void Func_02005db0();
extern void Func_02005d2e();
extern void Func_02005e18();
extern void Func_02005e3c();
extern void Func_02005e2e();
extern void Func_02005dde();
extern void Func_02005d5c();
extern void Func_02005e5e();
extern void Func_02005e50();
extern void Func_02005e74();
extern void Func_02005e66();
extern void Func_02005e16();
extern void Func_02005d94();
extern void Func_02005e7e();
extern void Func_02005e46();
extern void Func_02005dac();
extern void Func_02005e96();
extern void Func_02005e5e_b();
extern void Func_02005dc4();
extern void Func_02005e32();
extern void Func_02005ece();
extern void Func_02005e60();
extern void Func_02005e26();
extern void Func_02005df4();
extern void Func_02005e9c();
extern void Func_02005e02();
extern void Func_02005eec();
extern void Func_02005eb4();
extern void Func_02005e1a();
extern void Func_02005f04();
extern void Func_02005ed6();
extern void Func_02005e34();
extern void Func_02005edc();
extern void Func_02005e42();
extern void Func_02005f2c();
extern void Func_02005efe();
extern void Func_02005e5c();
extern void Func_02005f46();
extern void Func_02005ef6();
extern void Func_02005e74_b();
extern void Func_02005f5e();
extern void Func_02005f0e();
extern void Func_02005e8c();
extern void Func_02005f76();
extern void Func_02005ee0();
extern void Func_02005f0c();
extern void Func_02005fa8();
extern void Func_02005f58();
extern void Func_02005fc4();
extern void Func_02005f36();
extern void Func_02005f7e();
extern void Func_02005ee4();
extern void Func_02005f6a();
extern void Func_02005f78();
extern u8 *Func_02005f36_b();
extern u8 *Func_02005f42();
extern u8 *Func_02005f4e();
extern u8 *Func_02005f58_b();
extern void Func_020060de();
extern void Func_02006020();
extern void Func_0200603c();
extern void Func_02005fb8();
extern void Func_02006054();
extern void Func_0200600c();
extern void Func_02006056();
extern void Func_02006028();
extern void Func_02006032();
extern void Func_0200603c_b();
extern void Func_02006046();
extern void Func_02006050();
extern void Func_0200605a();
extern void Func_02006064();
extern void Func_0200606e();
extern void Func_0200606e_b();
extern void Func_02005fd4();
extern void Func_020060d0();
extern void Func_0200604a();
extern void Func_020060e6();
extern void Func_020060ce();
extern s32 Func_02006026();
extern void Func_020060b2();
extern void Func_02006018();
extern void Func_02006032_b();
extern void Func_02006134();
extern void Func_02006138();
extern void Func_020060f0();
extern void Func_02006056_b();
extern void Func_0200612e();
extern s32 Func_02006086();
extern void Func_020060fa();
extern void Func_02006078();
extern void Func_02006174();
extern void Func_0200616e();
extern void Func_02006084();
extern void Func_020060c6();
extern void Func_02006156();
extern void Func_020060d4();
extern void Func_020061be();
extern void Func_020060d4_b();
extern void Func_020061e0();
extern void Func_020061ec();
extern void Func_02006184();
extern void Func_0200618c();
extern void Func_0200615c();

extern u8 *Data_03001ebc;

void Func_02001494(void)
{
    s32 poseActor;
    u8 *workspace;

    Func_0200585e(0x89a);
    Func_02005874(30);
    Func_02005926(13, 0, 0);
    Func_02005930(15, 0, 0);
    Func_0200593a(16, 0, 0);
    Func_02005898(20);
    Func_0200599c(13, 128 << 1, 0);
    Func_020059a8(15, 256, 0);
    Func_020059b4(16, 0x80 * 2, 0);
    Func_020058c2(60);
    Func_02005990(0x183b);
    Func_020059b2(13, 0, 20);
    Func_02005984(0, 13, 0);
    Func_02005984_b(15, 1);
    Func_020058ea(20);
    Func_020059d4(15, 0, 20);
    Func_020059a6(0, 15, 0);
    Func_020059a6_b(16, 2);
    Func_0200590c(20);
    Func_020059be(0, 16, 0);
    Func_02005a06(16, 0);
    Func_02005924(50);
    Func_02005a34(16, 1);
    Func_02005976(16, 0xcccc, 0x6666);
    Func_020059a0(16, 176, 248);
    Func_020059ac(16, 154 << 1, 248);
    Func_02005a48(0, 128 << 6, 0);
    Func_02005a54(16, 192 << 8, 20);
    Func_02005b12(158);
    Func_02005934(0x0200c77a, 78, 13);
    Func_02005a1c(16, 2);
    Func_02005982(20);
    Func_020059d0(16, 192 << 9, 192 << 8);
    Func_020059c6(16)[90] &= 0xfe;
    Func_02005a0e(16, 154 << 1, 136 << 1);
    Func_020059b4_b(1);
    Func_020059ea(16)[90] |= 1;
    Func_02005aae(16, 0, 50);
    Func_02005a54_b(17, 152 << 17, 216 << 16);
    Func_02005a48_b(17, 152 << 1, 248);
    Func_02005a9a(9, 17, 0);
    Func_02005aa4(10, 17, 0);
    Func_02005aae_b(11, 17, 0);
    Func_02005ab8(12, 17, 0);
    Func_02005ac2(13, 17, 0);
    Func_02005acc(14, 17, 0);
    Func_02005ad6(15, 17, 0);
    Func_02005ae0(16, 17, 0);
    Func_02005aea(0, 17, 0);
    Func_02005a48_c(10);
    Func_02005ae8(9, 2);
    Func_02005af0(10, 2);
    Func_02005af8(11, 2);
    Func_02005b00(12, 2);
    Func_02005b08(13, 2);
    Func_02005b10(14, 2);
    Func_02005b18(15, 2);
    Func_02005b28(16, 2);
    Func_02005b8a(17, 0x103, 60);
    Func_02005b18_b(18, 152 << 17, 216 << 16);
    Func_02005b04(18, 152 << 1, 248);
    Func_02005b12_b(17, 140 << 1, 132 << 1);
    Func_02005b30(18);
    Func_02005bbc(18, 160 << 7, 0);
    Func_02005b42(17);
    Func_02005c80(159);
    Func_02005aa2(0x0200c790, 78, 13);
    Func_02005bcc(18, 0, 20);
    Func_02005b9e(9, 17, 0);
    Func_02005ba8(10, 17, 0);
    Func_02005bb2(11, 17, 0);
    Func_02005bbc_b(12, 17, 0);
    Func_02005bc6(13, 17, 0);
    Func_02005bd0(14, 17, 0);
    Func_02005bda(15, 17, 0);
    Func_02005be4(16, 17, 0);
    Func_02005bee(0, 17, 0);
    Func_02005b4c(10);
    Func_02005bf4(17, 2);
    Func_02005b5a(20);
    Func_02005bea(18, 4);
    Func_02005b68(20);
    Func_02005c52(18, 0, 20);
    Func_02005c02(17, 3);
    Func_02005b80(20);
    Func_02005c28(18, 1);
    Func_02005b8e(20);
    Func_02005c78(18, 0, 20);
    Func_02005c94(17, 208 << 8, 20);
    Func_02005c8e(17, 0, 20);
    Func_02005cb2(18, 0x102, 60);
    Func_02005ca4(18, 0, 20);
    Func_02005c54(17, 3);
    Func_02005bd2(20);
    Func_02005cbc(17, 0, 20);
    Func_02005c6c(18, 3);
    Func_02005bea_b(20);
    Func_02005cd4(18, 0, 20);
    Func_02005c84(17, 3);
    Func_02005c02_b(20);
    Func_02005c92(18, 4);
    Func_02005c10(20);
    Func_02005cfa(18, 0, 20);
    Func_02005cc2(17, 2);
    Func_02005c28_b(20);
    Func_02005d22(17, 0, 20);
    Func_02005d1c(17, 0, 20);
    Func_02005d38(16, 128 << 8, 20);
    Func_02005cd8(16, 3);
    Func_02005c56(20);
    Func_02005ce6(17, 3);
    Func_02005c82(20);
    Func_02005d6c(17, 0, 20);
    Func_02005d1c_b(16, 3);
    Func_02005c9a(20);
    Func_02005d84(16, 0, 20);
    Func_02005da0(17, 128 << 8, 20);
    Func_02005d9a(17, 0, 20);
    Func_02005d4a(9, 3);
    Func_02005cc8(20);
    Func_02005db2(9, 0, 20);
    Func_02005dce(17, 208 << 8, 20);
    Func_02005d86(17, 1);
    Func_02005cec(20);
    Func_02005dd6(17, 0, 20);
    Func_02005dfa(18, 0x102, 60);
    Func_02005dec(18, 0, 20);
    Func_02005e0e(17, 0x101, 60);
    Func_02005e00(17, 0, 20);
    Func_02005db0(18, 3);
    Func_02005d2e(20);
    Func_02005e18(18, 0, 20);
    Func_02005e3c(17, 0x100, 60);
    Func_02005e2e(17, 0, 20);
    Func_02005dde(18, 4);
    Func_02005d5c(20);
    Func_02005e5e(17, 0x103, 60);
    Func_02005e50(17, 0, 20);
    Func_02005e74(18, 0x100, 60);
    Func_02005e66(18, 0, 20);
    Func_02005e16(17, 4);
    Func_02005d94(20);
    Func_02005e7e(17, 0, 20);
    Func_02005e46(18, 2);
    Func_02005dac(20);
    Func_02005e96(18, 0, 20);
    Func_02005e5e_b(17, 2);
    Func_02005dc4(10);
    Func_02005e32(17, 128 << 1, 140 << 1);
    Func_02005ece(17, 128 << 7, 20);
    Func_02005e60(17, 0, 0);
    Func_02005e26(17);
    Func_02005df4(30);
    Func_02005e9c(9, 2);
    Func_02005e02(20);
    Func_02005eec(9, 0, 20);
    Func_02005eb4(15, 2);
    Func_02005e1a(20);
    Func_02005f04(15, 0, 20);
    Func_02005ed6(16, 18, 0);
    Func_02005e34(20);
    Func_02005edc(16, 2);
    Func_02005e42(20);
    Func_02005f2c(16, 0, 20);
    Func_02005efe(18, 16, 0);
    Func_02005e5c(20);
    Func_02005f46(18, 0, 20);
    Func_02005ef6(18, 4);
    Func_02005e74_b(20);
    Func_02005f5e(18, 0, 20);
    Func_02005f0e(18, 3);
    Func_02005e8c(20);
    Func_02005f76(18, 0, 20);
    Func_02005ee0(18, 0xcccc, 0x6666);
    Func_02005f0c(18, 128 << 1, 248);
    Func_02005fa8(18, 192 << 8, 20);
    Func_02005f58(18, 1);
    Func_02005fc4(18, 0x100, 60);
    Func_02005f36(18, 240, 184);
    Func_02005f7e(18, 2);
    Func_02005ee4(20);
    Func_02005f6a(19, 232 << 16, 168 << 16);
    Func_02005f78(20, 232 << 16, 168 << 16);
    *(s32 *)(Func_02005f36_b(19) + 12) = 192 << 12;
    *(u32 *)(Func_02005f42(19) + 60) = 0x80000000;
    *(s32 *)(Func_02005f4e(19) + 24) = 0xcccc;
    *(u16 *)(*(u8 **)(Func_02005f58_b(19) + 80) + 30) = 128 << 8;
    Func_020060de(124);
    Func_02006020(18, 0, 20);
    Func_0200603c(0, 192 << 8, 20);
    Func_02005fb8(16, 128 << 1, 240);
    Func_02006054(16, 176 << 8, 20);
    Func_0200600c(16, 1);
    Func_02006056(16, 0, 20);
    Func_02006028(9, 0, 0);
    Func_02006032(10, 0, 0);
    Func_0200603c_b(11, 0, 0);
    Func_02006046(12, 0, 0);
    Func_02006050(13, 0, 0);
    Func_0200605a(14, 0, 0);
    Func_02006064(15, 0, 0);
    Func_0200606e(16, 0, 0);
    Func_0200606e_b(18, 2);
    Func_02005fd4(20);
    Func_020060d0(18, 160 << 7, 20);
    Func_0200604a(18, 248, 208);
    Func_020060e6(18, 160 << 7, 20);
    Func_020060ce(18, 0);
    if (Func_02006026(0, 0) == 0) {
        Func_020060b2(16, 1);
        Func_02006018(20);
        poseActor = 16;
    } else {
        workspace = Data_03001ebc;
        Func_02006032_b(20);
        Func_02006134(18, 0x105, 60);
        Func_02006138(18, 128 << 7, 20);
        Func_020060f0(16, 2);
        Func_02006056_b(20);
        *(u16 *)(workspace + 472) += 1;
        Func_0200612e(16, 0);
        if (Func_02006086(0, 0) == 0) {
            Func_020060fa(16, 3);
            Func_02006078(20);
            Func_02006174(18, 176 << 8, 20);
            poseActor = 18;
        } else {
            goto secondSkip;
        }
    }

    Func_0200616e(poseActor, 0, 20);
    Func_02006084(0x898);
    goto skipComplete;

secondSkip:
    *(u16 *)(workspace + 472) += 1;
    Func_020060c6(20);
    Func_02006156(18, 4);
    Func_020060d4(20);
    Func_020061be(18, 0, 20);
    Func_020060d4_b(0x899);

skipComplete:
    Func_020061e0(10, 128 << 8, 0);
    Func_020061ec(11, 128 << 8, 20);
    Func_02006184(10, 5);
    Func_0200618c(11, 5);
    Func_0200615c(12, 0x0200c638);
}
