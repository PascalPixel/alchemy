#include "types.h"

/*
 * Resource 3c8 at 0x02003068: the package's per-scene "step" entry point.
 *
 * ---------------------------------------------------------------------------
 * Boundary
 * ---------------------------------------------------------------------------
 * One owner, 3,922 bytes, 0x02003068..0x02003fe3 inclusive of its trailing
 * literal pool.  Prologue at 0x02003068:
 *
 *     push {r5, r6, r7, lr} ; mov r7,fp ; mov r6,sl ; mov r5,r9
 *     push {r5, r6, r7}     ; mov r7,r8 ; push {r7} ; sub sp, #12
 *
 * and the sole matching unwind at 0x02003fa8..0x02003fb8
 *
 *     add sp,#12 ; pop {r3,r5,r6,r7} ; mov r8,r3 ; mov r9,r5 ; mov sl,r6
 *     mov fp,r7  ; pop {r5,r6,r7} ; pop {r1} ; bx r1
 *
 * `pop {r1} ; bx r1` leaves r0 alive, and `movs r0, #0` at 0x02003fa6 is
 * executed on every path into the unwind, so the owner is `s32` and always
 * returns 0.  Twenty-four `bl` sites in the body target 0x02003fa6 - they are
 * long branches to that shared `return 0`, not calls, and are excluded from the
 * call accounting below.
 *
 * The overlay's 18 inventory rows across this span are `call:` seeds (import
 * identities), not entry points.  The only genuine internal structure is five
 * `mov pc, r3` jump tables, all of which are data inside the span:
 *
 *     0x02003138  26 entries  scene 0xb6, state 1..26
 *     0x020033cc  21 entries  scene 0xb7, state 1..21
 *     0x020035d8  11 entries  scene 0xb8, state 1..11
 *     0x020037e0  17 entries  scene 0xb9, state 4..20
 *     0x02003a9c  20 entries  scene 0xba, state 1..20
 *
 * The table base pool words read 0x0200b138 / 0x0200b3cc / 0x0200b5d8 /
 * 0x0200b7e0 / 0x0200ba9c; resource_3c8 is linked at 0x02008000, so each base
 * is its own physical file offset + 0x8000, which is the sixth witness for that
 * base.  Entries carry no Thumb bit because ARMv4T `mov pc, rN` does not
 * interwork - it keeps the current instruction set and ignores bit 0.
 *
 * Literal pools inside the span, established by a control-flow walk from the
 * prologue rather than from a referenced-words listing:
 *   0x0200344c-0x0200348b, 0x0200382a-0x02003863, 0x02003b3e-0x02003b6f,
 *   0x02003be4-0x02003beb (branched over mid-block by `b.n` at 0x02003be2,
 *   with r2/r3/r5/sl/r7 live across it - see the slot-9 reset in scene 0xba),
 *   0x02003fbc-0x02003fe3.
 *
 * ---------------------------------------------------------------------------
 * Call accounting
 * ---------------------------------------------------------------------------
 * `overlay_call_targets.ts resource_3c8 3068` resolves 248 `bl` sites to 37
 * distinct targets: 195 veneer, 29 prologue, 24 "unknown" (all of them the
 * 0x02003fa6 return, above).  So 224 sites are real calls, and this file
 * contains exactly 224 call expressions.
 *
 * Two `bl` sites are reached from two control paths each and would otherwise
 * inflate the C count; both are expressed so that exactly one C call remains:
 *   - 0x0200331a (scene 0xb6, states 20/21) is the head of a three-call tail
 *     shared by the 0x982 and 0x983 arms; the four differing arguments are
 *     hoisted into locals and the tail is written once.
 *   - 0x02003fa2 (`Func_02002b14`, scene 0xba states 15/16) sits after the
 *     0x308 flag block on both paths, so a plain `if` reproduces it.
 *
 * ---------------------------------------------------------------------------
 * Evidence for the environment
 * ---------------------------------------------------------------------------
 * `Data_02000240[224]` is the scene id and `[225]` the scene step; both are
 * read with `ldrsh`.  The byte-exact siblings 0x02000e04 and 0x02000e88 use the
 * same pair, and semantic/overlays/resource_3c8_c_02004bd8.c compares
 * `Data_02000240[224]` against 0xb9.  0x03001ebc is the package workspace
 * pointer (byte-exact 0x02000754 / 0x02001780); the s32 at +448 is the
 * package's own request word, written here as 516, 256, 514 and 516 - the same
 * spellings as 0x02001af0, 0x020026f8 and 0x0200290c.
 *
 * `Func_0808a080(slot)` returns the sprite record for a numbered slot; the
 * fields touched here are +8/+16 (16.16 world x/y, compared as `>> 20` tile
 * units), +12 (16.16 z), +0x23 (a flag byte), +0x3c, +0x55 and +0x59 (state
 * bytes), +0x64 (a halfword counter) and +0x6c (a per-frame callback).  The
 * callbacks installed are 0x02008b99 = Func_02000b98 + Thumb and 0x020088c9 =
 * Func_020008c8 + Thumb, both byte-exact in assets/code - a further witness
 * for the 0x02008000 link base.
 *
 * ---------------------------------------------------------------------------
 * Uncertainties
 * ---------------------------------------------------------------------------
 *  - Import names are the resolved main-image addresses reported by
 *    overlay_call_targets.ts.  Their real interfaces are unknown, so all
 *    declarations are old-style: two names can be one import and one import can
 *    take different argument counts at different sites.
 *  - The struct field names below describe how this owner uses each offset,
 *    not the original layout; the padding is only what the offsets require.
 *  - Func_020045f0 and Func_02004600 are in-image callbacks installed through
 *    Func_080000d0 and have no converted source yet, so only their addresses
 *    are asserted.
 *  - The stack slot at sp+8 is used once, purely to preserve r4 across a call
 *    inside the scene-0xba proximity loop; it carries no argument.
 */

struct Sprite {
    u8  pad00[8];
    s32 x;              /* +0x08 */
    s32 z;              /* +0x0c */
    s32 y;              /* +0x10 */
    u8  pad14[0x23 - 0x14];
    u8  flags;          /* +0x23 */
    u8  pad24[0x3c - 0x24];
    s32 field3c;        /* +0x3c */
    u8  pad40[0x55 - 0x40];
    u8  state55;        /* +0x55 */
    u8  pad56[0x59 - 0x56];
    u8  state59;        /* +0x59 */
    u8  pad5a[0x64 - 0x5a];
    u16 counter64;      /* +0x64 */
    u8  pad66[0x6c - 0x66];
    void (*handler)();  /* +0x6c */
};

extern s16 Data_02000240[];

/* Old-style declarations: the imports' real interfaces are not known here. */
void Func_02007f08();
s32 Func_02007ef6();
void Func_020035e2();
void Func_02007f0e();
s32 Func_02007f2c();
void Func_020060ce();
s32 Func_02007f0c();
void Func_020060ea();
void Func_020060f0();
void Func_020060f6();
struct Sprite *Func_02008056();
void Func_02006104();
struct Sprite *Func_02008064();
void Func_02006112();
void Func_02006118();
void Func_020061b0();
s32 Func_02008034();
void Func_020061c2();
s32 Func_02007ff4();
s32 Func_02008008();
s32 Func_02008018();
s32 Func_02008028();
s32 Func_02008038();
s32 Func_02008048();
s32 Func_02008058();
s32 Func_02008068();
s32 Func_02008078();
s32 Func_020080e0();
s32 Func_020080a4();
s32 Func_020080b8();
s32 Func_020080c8();
s32 Func_020080d8();
s32 Func_020080e8();
s32 Func_020080f8();
s32 Func_02008108();
s32 Func_02008118();
s32 Func_02008128();
s32 Func_02008138();
s32 Func_02008148();
s32 Func_02008158();
s32 Func_020081ca();
s32 Func_020081d0();
s32 Func_020081c8();
s32 Func_0200818c();
s32 Func_020081ee();
struct Sprite *Func_02008284();
struct Sprite *Func_02008242();
s32 Func_020082a8();
void Func_020076e8();
struct Sprite *Func_02008422();
s32 Func_020082b0();
struct Sprite *Func_02003e10();
struct Sprite *Func_0200833c();
struct Sprite *Func_02004e56();
struct Sprite *Func_02004eb8();
struct Sprite *Func_02008356();
struct Sprite *Func_02004ecc();
void Func_02003520();
void Func_0200352e();
void Func_020084bc();
void Func_0200828a();
struct Sprite *Func_02008398();
struct Sprite *Func_020083ac();
struct Sprite *Func_020083bc();
void Func_02004e16();
struct Sprite *Func_02008514();
void Func_0200850a();
void Func_020082d8();
s32 Func_020083b0();
s32 Func_02008374();
s32 Func_02008384();
s32 Func_02008394();
s32 Func_020083a4();
void Func_020083c8();
void Func_020083da();
struct Sprite *Func_02003dde();
void Func_020085ea();
struct Sprite *Func_020084c4();
struct Sprite *Func_020084da();
s32 Func_020084ba();
void Func_020083f4();
s32 Func_02008480();
s32 Func_020084ac();
struct Sprite *Func_0200857a();
struct Sprite *Func_020085fc();
s32 Func_020084e4();
void Func_020085b4();
struct Sprite *Func_02008624();
struct Sprite *Func_02008572();
struct Sprite *Func_020085d4();
void Func_02005430();
struct Sprite *Func_020085ac();
struct Sprite *Func_020086f6();
void Func_02008532();
s32 Func_02008590();
s32 Func_0200854e();
s32 Func_0200855e();
s32 Func_02008570();
s32 Func_02008580();
s32 Func_020085ac_b();
s32 Func_020085be();
void Func_02008766();
void Func_0200877a();
void Func_02007e36();
struct Sprite *Func_02008854();
s32 Func_020086e2();
void Func_020086b2();
s32 Func_020086b6();
void Func_02008644();
struct Sprite *Func_02008656();
s32 Func_02008724();
void Func_020086f4();
s32 Func_020086f8();
void Func_02008686();
struct Sprite *Func_02008698();
struct Sprite *Func_020040d8();
void Func_020080cc();
struct Sprite *Func_020040ea();
struct Sprite *Func_020087c6();
struct Sprite *Func_020087d8();
struct Sprite *Func_020087e2();
struct Sprite *Func_020087ec();
s32 Func_020087c0();
void Func_020086fc();
s32 Func_02008788();
s32 Func_020087b4();
s32 Func_020087fa();
struct Sprite *Func_0200888c();
struct Sprite *Func_0200889a();
struct Sprite *Func_020088a8();
struct Sprite *Func_020088b8();
struct Sprite *Func_020088c6();
struct Sprite *Func_020088d4();
struct Sprite *Func_0200894c();
struct Sprite *Func_0200896c();
struct Sprite *Func_020088ba();
struct Sprite *Func_02008984();
struct Sprite *Func_020088d2();
void Func_02008934();
struct Sprite *Func_020088ea();
void Func_02008888();
struct Sprite *Func_020089b0();
struct Sprite *Func_020088fe();
struct Sprite *Func_02008960();
void Func_020059d4();
s32 Func_02008964();
void Func_02005dc2();
struct Sprite *Func_020089ac();
struct Sprite *Func_02004450();
struct Sprite *Func_020089ba();
s32 Func_020089a0();
struct Sprite *Func_02008a20();
struct Sprite *Func_02008b1e();
struct Sprite *Func_02008a6c();
struct Sprite *Func_02008b5e();
struct Sprite *Func_02008aac();
struct Sprite *Func_02008ac2();
void Func_02008a60();
struct Sprite *Func_02008ace();
struct Sprite *Func_02008ae4();
void Func_02008a82();
struct Sprite *Func_02008af0();
struct Sprite *Func_02008b06();
void Func_02008aa4();
struct Sprite *Func_02008b12();
struct Sprite *Func_02008b28();
void Func_02008ac6();
struct Sprite *Func_02008c76();
struct Sprite *Func_02008b4c();
struct Sprite *Func_02008b56();
struct Sprite *Func_02008b62();
struct Sprite *Func_02008b72();
void Func_02008af2();
s32 Func_02008b4e();
struct Sprite *Func_02008cd0();
s32 Func_02008b14();
s32 Func_02008b24();
s32 Func_02008b50();
s32 Func_02008b62_b();
s32 Func_02008b5c();
s32 Func_02008b6c();
s32 Func_02008bca();
s32 Func_02008b8a();
s32 Func_02008b9a();
s32 Func_02008bc6();
s32 Func_02008bd8();
s32 Func_02008bd2();
s32 Func_02008be2();
void Func_02008da6();
void Func_02008dba();
void Func_02008832();
s32 Func_02008c64();
void Func_02008bdc();
struct Sprite *Func_02008bee();
struct Sprite *Func_02008cf4();
struct Sprite *Func_02008d0a();
struct Sprite *Func_02008d16();
struct Sprite *Func_02008d22();
struct Sprite *Func_02008d2e();
s32 Func_02008d04();
void Func_02008c40();
s32 Func_02008ccc();
s32 Func_02008cf8();
struct Sprite *Func_02008dc6();
struct Sprite *Func_02008d84();
struct Sprite *Func_02008e50();
struct Sprite *Func_02008dee();
struct Sprite *Func_02008dac();
struct Sprite *Func_02008e76();
struct Sprite *Func_02008e14();
struct Sprite *Func_02008dd2();
struct Sprite *Func_02008e9c();
struct Sprite *Func_02008e3a();
struct Sprite *Func_02008df8();
struct Sprite *Func_02008ec2();
void Func_02008e62();
struct Sprite *Func_02008e18();
void Func_02008db6();
struct Sprite *Func_02008ede();
struct Sprite *Func_02008e2c();
struct Sprite *Func_02008e90();
void Func_02006ab8();

                               

                     

                     

                     

                     

                     

                     

                     

                    

                               

                     

                               

/* In-image callees of this overlay. */

/* Installed through Func_080000d0 / the sprite +0x6c callback slot. */
void Func_020008c8();
void Func_02000b98();
void Func_02003050();
void Func_020045f0();
void Func_02004600();

#define WORKSPACE   (*(u8 **)0x03001ebc)
#define REQUEST     (*(s32 *)(WORKSPACE + 448))
#define BLEND_CTRL  (*(u16 *)0x04000050)

s32 Func_02003068(void)
{
    struct Sprite *sprite;
    s32 scene;
    s32 step;
    u32 i;

    Func_02007f08(1);
    if (Func_02007ef6(0x109) != 0) {
        Func_020035e2();
    }
    Func_02007f0e(272);

    REQUEST = 516;
    scene = Data_02000240[224];

    /* ------------------------------------------------------------ 0xb5 */
    if (scene == 0xb5) {
        REQUEST = 256;
        if (Func_02007f2c(0x981) == 0) {
            Func_020060ce(8);
        } else {
            Func_02007f0c(7, 17, 2, 1, 7, 16);
        }
        Func_020060ea(9);
        Func_020060f0(10);
        Func_020060f6(11);
        Func_02008056(11, 2);
        Func_02006104(12);
        Func_02008064(12, 2);
        Func_02006112(13);
        Func_02006118(14);
        return 0;
    }

    /* ------------------------------------------------------------ 0xb6 */
    if (scene == 0xb6) {
        step = Data_02000240[225];
        switch (step) {
        case 1:
        case 2:
            Func_020061b0(8);
            return 0;

        case 5:
            Func_02008034(288);
            /* fall through */
        case 3:
        case 4:
        case 6:
            Func_020061c2(9);
            return 0;

        case 20:
        case 21: {
            s32 a0, a1, a2, a3;

            if (Func_02008034(0x982) != 0) {
                Func_02007ff4(103, 27, 89, 27, 7, 8);
                Func_02008008(41, 90, 27, 92, 3, 2);
                Func_02008018(41, 90, 29, 93, 3, 2);
                Func_02008028(41, 90, 27, 94, 3, 2);
                Func_02008038(41, 90, 27, 96, 3, 2);
                Func_02008048(41, 90, 29, 97, 3, 2);
                Func_02008058(41, 96, 25, 91, 3, 2);
                Func_02008068(41, 92, 25, 93, 3, 2);
                Func_02008078(41, 96, 25, 95, 3, 2);
                a0 = 41; a1 = 96; a2 = 25; a3 = 97;
            } else {
                if (Func_020080e0(0x983) == 0) {
                    return 0;
                }
                Func_020080a4(111, 27, 89, 27, 7, 8);
                Func_020080b8(41, 90, 25, 91, 3, 2);
                Func_020080c8(41, 90, 25, 93, 3, 2);
                Func_020080d8(41, 90, 25, 95, 3, 2);
                Func_020080e8(41, 90, 25, 97, 3, 2);
                Func_020080f8(41, 90, 27, 96, 3, 2);
                Func_02008108(41, 90, 29, 97, 3, 2);
                Func_02008118(41, 94, 27, 92, 3, 2);
                Func_02008128(41, 96, 29, 93, 3, 2);
                a0 = 41; a1 = 94; a2 = 27; a3 = 94;
            }
            /* 0x0200331a: one site, reached from both arms above. */
            Func_02008138(a0, a1, a2, a3, 3, 2);
            Func_02008148(41, 96, 27, 96, 3, 2);
            Func_02008158(41, 96, 29, 97, 3, 2);
            return 0;
        }

        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            if (Func_020081ca(0x987) == 0) {
                return 0;
            }
            Func_020081d0(12, 218 << 18, 176 << 15);
            sprite = Func_020081c8(12);
            sprite->z = (s32)0xffe80000;
            sprite->field3c = (s32)0x80000000;
            return 0;

        case 26:
            Func_0200818c(0x121);
            /* fall through */
        case 22:
        case 23:
            Func_020081ee(0x12f);
            if (Func_02008284(512) == 0) {
                return 0;
            }
            Func_02008242(44, 117, 41, 117, 3, 5);
            return 0;

        default:
            return 0;
        }
    }

    /* ------------------------------------------------------------ 0xb7 */
    if (scene == 0xb7) {
        step = Data_02000240[225];
        switch (step) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 19:
            goto scene_fade;

        case 7:
        case 8:
            Func_020082a8(0);
            Func_020076e8(2);
            sprite = Func_02008422(8);
            sprite->state55 = 0;
            sprite->handler = Func_020008c8;
            sprite = Func_020082b0(9);
            sprite->state55 = 0;
            sprite->handler = Func_020008c8;
            sprite = Func_02003e10(10);
            sprite->state55 = 0;
            sprite->handler = Func_020008c8;
            Func_0200833c();
            return 0;

        case 9:
        case 10:
            sprite = Func_02004e56(11);
            Func_02004eb8();
            if ((sprite->x >> 20) == 8) {
                Func_02008356(sprite);
            }
            sprite = Func_02004ecc(12);
            if ((sprite->x >> 20) == 7) {
                Func_02003520(sprite);
            }
            Func_0200352e(206 << 16, 0, 0x01c10000, 223);
            Func_020084bc(210 << 16, 0, 0x01c10000, 223);
            return 0;

        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            Func_0200828a(170);
            Func_02008398(0);
            Func_020083ac(2);
            if (Func_020083bc(768) == 0) {
                return 0;
            }
            Func_02004e16(111, 5, 117, 5, 5, 2);
            Func_02008514(111, 10, 117, 10, 5, 2);
            Func_0200850a(111, 7, 111, 5, 5, 2);
            Func_020082d8(111, 7, 111, 10, 5, 2);
            Func_020083b0(48, 3, 3, 10, 54, 3);
            Func_02008374(55, 26, 3, 10, 48, 3);
            return 0;

        case 16:
            Func_02008384(0x12f);
            return 0;

        case 20:
            Func_02008394(170);
            if (Func_020083a4(0x109) != 0) {
                return 0;
            }
            Func_020083c8();
            return 0;

        case 21:
            Func_020083da();
            return 0;

        default:
            return 0;
        }
    }

    /* ------------------------------------------------------------ 0xb8 */
    if (scene == 0xb8) {
        step = Data_02000240[225];
        switch (step) {
        case 2:
            Func_02003dde();
            goto scene_fade;
        case 1:
            goto scene_fade;

        case 4:
        case 6:
            Func_020085ea(0);
            return 0;

        case 9:
        case 10:
            sprite = Func_020084c4(8);
            sprite->state55 = 0;
            sprite->z = 0;
            sprite = Func_020084da(9);
            sprite->state55 = 0;
            sprite->state59 = 0;
            if (Func_020084ba(0x301) != 0) {
                struct Sprite *extra;

                Func_020083f4(1);
                Func_02008480(124, 41, 110, 41, 1, 2);
                Func_020084ac(46, 41, 1, 1, 46, 42);
                Func_0200857a(9, 186 << 18, 182 << 18);
                sprite->state55 = 0;
                sprite->z = (s32)0xfff00000;
                Func_020085fc(9, 3);
                sprite->flags = 2;
                Func_020084e4(45, 45, 1, 1, 46, 45);
                Func_020085b4(10, 7);
                Func_02008624(10, 1);
                extra = Func_02008572(10);
                extra->state59 = 0;
                extra->flags = 2;
                Func_020085d4(10, 0x02e70000, 174 << 18);
                extra->handler = Func_02000b98;
            }
            Func_02005430();
            return 0;

        case 11:
            REQUEST = 514;
            sprite = Func_020085ac(0);
            sprite->z = (s32)0xfffe0000;
            /* fall through to the shared 0x02003706 tail */
        case 7:
        case 8:
            Func_020086f6(170);
            Func_02008532();
            BLEND_CTRL = 0;
            if (Func_02008590(768) != 0) {
                Func_0200854e(15, 96, 9, 96, 3, 3);
                Func_0200855e(12, 96, 15, 96, 3, 3);
                Func_02008570(5, 50, 15, 32, 3, 4);
                Func_02008580(25, 45, 9, 32, 3, 4);
                Func_020085ac_b(15, 32, 3, 1, 9, 32);
                Func_020085be(12, 32, 3, 1, 15, 32);
            }
            if (Data_02000240[225] != 11) {
                return 0;
            }
            Func_02008766();
            Func_0200877a();
            REQUEST = 516;
            return 0;

        default:
            return 0;
        }
    }

    /* ------------------------------------------------------------ 0xb9 */
    if (scene == 0xb9) {
        step = Data_02000240[225];
        switch (step) {
        case 4:
        case 5:
            Func_02007e36();
            goto scene_fade;
        case 6:
            goto scene_fade;

        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 20:
            Func_02008854(170);
            if (Func_020086e2(0x306) != 0) {
                Func_020086b2(53, 12, 3, 13, 26, 12);
                Func_020086b6(81, 41, 89, 14, 9, 2);
                Func_02008644(1);
                Func_02008656(Func_020045f0, 3200);
            }
            if (Func_02008724(0x307) != 0) {
                Func_020086f4(58, 12, 3, 13, 34, 12);
                Func_020086f8(81, 41, 97, 14, 5, 2);
                Func_02008686(1);
                Func_02008698(Func_02004600, 3200);
            }
            step = Data_02000240[225];
            if (step == 11) {
                Func_020040d8();
                return 0;
            }
            if (step != 20) {
                return 0;
            }
            Func_020080cc();
            return 0;

        case 15:
        case 16:
            sprite = Func_020040ea(8);
            sprite->state55 = 0;
            sprite->z = 0;
            Func_020087c6(9)->state55 = 0;
            Func_020087d8(10)->state55 = 0;
            Func_020087e2(11)->state55 = 0;
            if (Func_020087ec(772) != 0) {
                Func_020087c0(1);
                Func_020086fc(111, 59, 109, 37, 1, 2);
                Func_02008788(45, 37, 1, 1, 45, 38);
                if (Func_020087b4(0x302) != 0) {
                    Func_020087fa(9, 194 << 18, 166 << 18);
                    Func_0200888c(10, 210 << 18, 166 << 18);
                    Func_0200889a(11, 194 << 18, 174 << 18);
                } else {
                    Func_020088a8(9, 210 << 18, 166 << 18);
                    Func_020088b8(10, 194 << 18, 174 << 18);
                    Func_020088c6(11, 210 << 18, 174 << 18);
                }
                Func_020088d4(9, 3);
                sprite = Func_0200894c(9);
                sprite->z = (s32)0xfff00000;
                sprite->flags = 2;
                Func_0200889a(10, 3);
                sprite = Func_0200896c(10);
                sprite->z = (s32)0xfff00000;
                sprite->flags = 2;
                Func_020088ba(11, 3);
                sprite = Func_02008984(11);
                sprite->z = (s32)0xfff00000;
                sprite->flags = 2;
                Func_020088d2(12, 7);
                Func_02008934(12, 7);
                sprite = Func_020088ea(12);
                Func_02008888(sprite, 0);
                sprite = Func_020089b0(12);
                sprite->state59 = 0;
                sprite->flags = 2;
                Func_020088fe(12, 0x02d70000, 158 << 18);
                sprite->handler = Func_02000b98;
            }
            Func_02008960();
            return 0;

        case 19:
            Func_020059d4();
            return 0;

        default:
            return 0;
        }
    }

    /* ------------------------------------------------------------ 0xba */
    if (scene != 0xba) {
        return 0;
    }
    step = Data_02000240[225];
    switch (step) {
    case 1:
    case 2:
        if (Func_02008964(0x109) != 0) {
            Func_02005dc2(0);
            Func_02004450(Func_020089ac(0));
            for (i = 0; i <= 3; i++) {
                sprite = Func_020089ba(i + 10);
                if ((sprite->x >> 20) == 13 && (sprite->y >> 20) == 7 &&
                    Func_020089a0(512 + i) != 0) {
                    sprite->flags |= 2;
                    sprite->state59 = 0;
                    sprite->state55 = 0;
                    Func_02008a20(4, 19, 1, 1, 13, 7);
                    return 0;
                }
            }
            return 0;
        }
        sprite = Func_02008b1e(8);
        sprite->state55 = 0;
        sprite->z = (s32)0xffd00000;
        sprite->flags |= 2;
        sprite->state59 &= 0xfe;
        sprite->counter64 = 3;
        Func_02008a6c(8, 1);
        sprite = Func_02008b5e(9);
        sprite->state55 = 0;
        sprite->z = (s32)0xffd00000;
        sprite->flags |= 2;
        sprite->state59 &= 0xfe;
        sprite->counter64 = 3;
        Func_02008aac(9, 1);
        sprite = Func_02008ac2(10);
        sprite->state55 = 0;
        sprite->counter64 = 0;
        Func_02008a60(sprite, 0);
        sprite = Func_02008ace(11);
        sprite->state55 = 0;
        sprite->counter64 = 0;
        Func_02008a82(Func_02008ae4(11), 0);
        sprite = Func_02008af0(12);
        sprite->state55 = 0;
        sprite->counter64 = 0;
        Func_02008aa4(Func_02008b06(12), 0);
        sprite = Func_02008b12(13);
        sprite->state55 = 0;
        sprite->counter64 = 0;
        Func_02008ac6(Func_02008b28(13), 0);
        return 0;

    case 13:
    case 14:
        goto scene_fade;

    /* Four jump tables enter this one physical fade tail.  Its placement
     * here mirrors the machine's address order between switch arms. */
    scene_fade:
        Func_02008c76(170);
        return 0;

    case 18:
    case 19:
        REQUEST = 514;
        sprite = Func_02008b4c(0);
        sprite->z = (s32)0xfffe0000;
        /* fall through to the shared 0x02003ca6 tail */
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 20:
        Func_02008b56(20)->state55 = 4;
        Func_02008b62(20)->flags |= 2;
        Func_02008b72(20)->z = (s32)0xffef8000;
        Func_02008af2();
        BLEND_CTRL = 0;
        if (Func_02008b4e(0x306) != 0) {
            Func_02008cd0(170);
            Func_02008b14(36, 81, 32, 81, 3, 2);
            Func_02008b24(36, 83, 36, 81, 3, 2);
            Func_02008b50(36, 17, 3, 1, 32, 17);
            Func_02008b62_b(36, 18, 3, 1, 36, 17);
            Func_02008b5c(63, 29, 33, 20, 1, 1);
            Func_02008b6c(20, 56, 36, 17, 3, 4);
        }
        if (Func_02008bca(0x307) != 0) {
            Func_02008b8a(44, 81, 48, 81, 3, 2);
            Func_02008b9a(44, 83, 44, 81, 3, 2);
            Func_02008bc6(44, 17, 3, 1, 48, 17);
            Func_02008bd8(44, 18, 3, 1, 44, 17);
            Func_02008bd2(63, 29, 49, 20, 1, 1);
            Func_02008be2(41, 56, 44, 17, 3, 4);
        }
        step = (u16)Data_02000240[225];
        if ((u16)(step - 18) <= 1) {
            Func_02008da6();
            Func_02008dba();
            REQUEST = 516;
            step = (u16)Data_02000240[225];
        }
        if (step != 20) {
            return 0;
        }
        Func_02008832();
        return 0;

    case 15:
    case 16:
        Func_02008c64(1);
        Func_02008bdc(Func_02003050, 3200);
        sprite = Func_02008bee(14);
        sprite->state55 = 0;
        sprite->z = 0;
        Func_02008cf4(15)->state55 = 0;
        Func_02008d0a(16)->state55 = 0;
        Func_02008d16(17)->state55 = 0;
        Func_02008d22(18)->state55 = 0;
        if (Func_02008d2e(776) != 0) {
            Func_02008d04(1);
            Func_02008c40(95, 56, 77, 35, 1, 2);
            Func_02008ccc(13, 35, 1, 1, 13, 36);
            Func_02008cf8(15, 132 << 17, 186 << 18);
            sprite = Func_02008dc6(15);
            sprite->z = (s32)0xfff00000;
            sprite->flags = 2;
            Func_02008d84(15, 3);
            Func_02008e50(16, 184 << 16, 158 << 18);
            sprite = Func_02008dee(16);
            sprite->z = (s32)0xfff00000;
            sprite->flags = 2;
            Func_02008dac(16, 3);
            Func_02008e76(17, 232 << 16, 174 << 18);
            sprite = Func_02008e14(17);
            sprite->z = (s32)0xfff00000;
            sprite->flags = 2;
            Func_02008dd2(17, 3);
            Func_02008e9c(18, 184 << 16, 166 << 18);
            sprite = Func_02008e3a(18);
            sprite->z = (s32)0xfff00000;
            sprite->flags = 2;
            Func_02008df8(18, 3);
            Func_02008ec2(19, 7);
            Func_02008e62(19, 7);
            sprite = Func_02008e18(19);
            Func_02008db6(sprite, 0);
            sprite = Func_02008ede(19);
            sprite->state59 = 0;
            sprite->flags = 2;
            Func_02008e2c(19, 215 << 16, 150 << 18);
            sprite->handler = Func_02000b98;
        }
        Func_02008e90();
        Func_02006ab8();
        return 0;

    default:
        return 0;
    }

}
