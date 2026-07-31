typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;

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
void Func_02000058();
void Func_02000558();
struct Sprite *Func_020007d8();
struct Sprite *Func_0200094c();
struct Sprite *Func_020009c8();
void Func_020018f8();
struct Sprite *Func_020019c0();
struct Sprite *Func_02001a14();
void Func_02001d48();
void Func_02001f60();
void Func_020022c8();
void Func_02002b14();
void Func_0200300c();
void Func_020042bc();
void Func_02004610();
void Func_020047c0();
void Func_02004a2c();
void Func_080000c0();
struct Sprite *Func_080000d0();
s32 Func_08009180();
void Func_080091a0();
void Func_080091b8();
s32 Func_080091c0();
void Func_080091e0();
s32 Func_080770c0();
void Func_080770c8();
s32 Func_080770d0();
void Func_0808a010();
struct Sprite *Func_0808a080();
struct Sprite *Func_0808a0f0();
void Func_0808a100();
struct Sprite *Func_0808a1e0();
void Func_0808a360();
void Func_0808a370();
void Func_0808a408();
struct Sprite *Func_0808a5e0();

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
    s32 i;

    Func_0808a010(1);
    if (Func_080770c0(0x109) != 0) {
        Func_02000558();
    }
    Func_080770c8(272);

    REQUEST = 516;
    scene = Data_02000240[224];

    /* ------------------------------------------------------------ 0xb5 */
    if (scene == 0xb5) {
        REQUEST = 256;
        if (Func_080770c0(0x981) == 0) {
            Func_0200300c(8);
        } else {
            Func_080091c0(7, 17, 2, 1, 7, 16);
        }
        Func_0200300c(9);
        Func_0200300c(10);
        Func_0200300c(11);
        Func_0808a1e0(11, 2);
        Func_0200300c(12);
        Func_0808a1e0(12, 2);
        Func_0200300c(13);
        Func_0200300c(14);
        return 0;
    }

    /* ------------------------------------------------------------ 0xb6 */
    if (scene == 0xb6) {
        step = Data_02000240[225];
        switch (step) {
        case 1:
        case 2:
            Func_0200300c(8);
            return 0;

        case 5:
            Func_080770d0(288);
            /* fall through */
        case 3:
        case 4:
        case 6:
            Func_0200300c(9);
            return 0;

        case 20:
        case 21: {
            s32 a0, a1, a2, a3;

            if (Func_080770c0(0x982) != 0) {
                Func_08009180(103, 27, 89, 27, 7, 8);
                Func_08009180(41, 90, 27, 92, 3, 2);
                Func_08009180(41, 90, 29, 93, 3, 2);
                Func_08009180(41, 90, 27, 94, 3, 2);
                Func_08009180(41, 90, 27, 96, 3, 2);
                Func_08009180(41, 90, 29, 97, 3, 2);
                Func_08009180(41, 96, 25, 91, 3, 2);
                Func_08009180(41, 92, 25, 93, 3, 2);
                Func_08009180(41, 96, 25, 95, 3, 2);
                a0 = 41; a1 = 96; a2 = 25; a3 = 97;
            } else {
                if (Func_080770c0(0x983) == 0) {
                    return 0;
                }
                Func_08009180(111, 27, 89, 27, 7, 8);
                Func_08009180(41, 90, 25, 91, 3, 2);
                Func_08009180(41, 90, 25, 93, 3, 2);
                Func_08009180(41, 90, 25, 95, 3, 2);
                Func_08009180(41, 90, 25, 97, 3, 2);
                Func_08009180(41, 90, 27, 96, 3, 2);
                Func_08009180(41, 90, 29, 97, 3, 2);
                Func_08009180(41, 94, 27, 92, 3, 2);
                Func_08009180(41, 96, 29, 93, 3, 2);
                a0 = 41; a1 = 94; a2 = 27; a3 = 94;
            }
            /* 0x0200331a: one site, reached from both arms above. */
            Func_08009180(a0, a1, a2, a3, 3, 2);
            Func_08009180(41, 96, 27, 96, 3, 2);
            Func_08009180(41, 96, 29, 97, 3, 2);
            return 0;
        }

        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            if (Func_080770d0(0x987) == 0) {
                return 0;
            }
            Func_080770d0(12, 218 << 18, 176 << 15);
            sprite = Func_080770c0(12);
            sprite->z = (s32)0xffe80000;
            sprite->field3c = (s32)0x80000000;
            return 0;

        case 26:
            Func_08009180(0x121);
            /* fall through */
        case 22:
        case 23:
            Func_080770c0(0x12f);
            if (Func_0808a0f0(512) == 0) {
                return 0;
            }
            Func_0808a080(44, 117, 41, 117, 3, 5);
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
            Func_080770d0(0);
            Func_020042bc(2);
            sprite = Func_0808a5e0(8);
            sprite->state55 = 0;
            sprite->handler = Func_020008c8;
            sprite = Func_080770c0(9);
            sprite->state55 = 0;
            sprite->handler = Func_020008c8;
            sprite = Func_020009c8(10);
            sprite->state55 = 0;
            sprite->handler = Func_020008c8;
            Func_0808a080();
            return 0;

        case 9:
        case 10:
            sprite = Func_020019c0(11);
            Func_02001a14();
            if ((sprite->x >> 20) == 8) {
                Func_0808a080(sprite);
            }
            sprite = Func_02001a14(12);
            if ((sprite->x >> 20) == 7) {
                Func_02000058(sprite);
            }
            Func_02000058(206 << 16, 0, 0x01c10000, 223);
            Func_0808a408(210 << 16, 0, 0x01c10000, 223);
            return 0;

        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            Func_080000c0(170);
            Func_0808a080(0);
            Func_0808a080(2);
            if (Func_0808a080(768) == 0) {
                return 0;
            }
            Func_020018f8(111, 5, 117, 5, 5, 2);
            Func_0808a5e0(111, 10, 117, 10, 5, 2);
            Func_0808a408(111, 7, 111, 5, 5, 2);
            Func_080000c0(111, 7, 111, 10, 5, 2);
            Func_080770c0(48, 3, 3, 10, 54, 3);
            Func_08009180(55, 26, 3, 10, 48, 3);
            return 0;

        case 16:
            Func_08009180(0x12f);
            return 0;

        case 20:
            Func_08009180(170);
            if (Func_08009180(0x109) != 0) {
                return 0;
            }
            Func_080091b8();
            return 0;

        case 21:
            Func_080091b8();
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
            Func_020007d8();
            goto scene_fade;
        case 1:
            goto scene_fade;

        case 4:
        case 6:
            Func_0808a408(0);
            return 0;

        case 9:
        case 10:
            sprite = Func_0808a080(8);
            sprite->state55 = 0;
            sprite->z = 0;
            sprite = Func_0808a080(9);
            sprite->state55 = 0;
            sprite->state59 = 0;
            if (Func_080770c0(0x301) != 0) {
                struct Sprite *extra;

                Func_080000c0(1);
                Func_08009180(124, 41, 110, 41, 1, 2);
                Func_080091c0(46, 41, 1, 1, 46, 42);
                Func_0808a0f0(9, 186 << 18, 182 << 18);
                sprite->state55 = 0;
                sprite->z = (s32)0xfff00000;
                Func_0808a1e0(9, 3);
                sprite->flags = 2;
                Func_080091c0(45, 45, 1, 1, 46, 45);
                Func_0808a100(10, 7);
                Func_0808a1e0(10, 1);
                extra = Func_0808a080(10);
                extra->state59 = 0;
                extra->flags = 2;
                Func_0808a0f0(10, 0x02e70000, 174 << 18);
                extra->handler = Func_02000b98;
            }
            Func_02001d48();
            return 0;

        case 11:
            REQUEST = 514;
            sprite = Func_0808a080(0);
            sprite->z = (s32)0xfffe0000;
            /* fall through to the shared 0x02003706 tail */
        case 7:
        case 8:
            Func_0808a5e0(170);
            Func_080091a0();
            BLEND_CTRL = 0;
            if (Func_080770c0(768) != 0) {
                Func_08009180(15, 96, 9, 96, 3, 3);
                Func_08009180(12, 96, 15, 96, 3, 3);
                Func_08009180(5, 50, 15, 32, 3, 4);
                Func_08009180(25, 45, 9, 32, 3, 4);
                Func_080091c0(15, 32, 3, 1, 9, 32);
                Func_080091c0(12, 32, 3, 1, 15, 32);
            }
            if (Data_02000240[225] != 11) {
                return 0;
            }
            Func_0808a360();
            Func_0808a370();
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
            Func_02004610();
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
            Func_0808a5e0(170);
            if (Func_080770c0(0x306) != 0) {
                Func_080091b8(53, 12, 3, 13, 26, 12);
                Func_08009180(81, 41, 89, 14, 9, 2);
                Func_080000c0(1);
                Func_080000d0(Func_020045f0, 3200);
            }
            if (Func_080770c0(0x307) != 0) {
                Func_080091b8(58, 12, 3, 13, 34, 12);
                Func_08009180(81, 41, 97, 14, 5, 2);
                Func_080000c0(1);
                Func_080000d0(Func_02004600, 3200);
            }
            step = Data_02000240[225];
            if (step == 11) {
                Func_020007d8();
                return 0;
            }
            if (step != 20) {
                return 0;
            }
            Func_020047c0();
            return 0;

        case 15:
        case 16:
            sprite = Func_020007d8(8);
            sprite->state55 = 0;
            sprite->z = 0;
            Func_0808a080(9)->state55 = 0;
            Func_0808a080(10)->state55 = 0;
            Func_0808a080(11)->state55 = 0;
            if (Func_0808a080(772) != 0) {
                Func_080770c0(1);
                Func_080000c0(111, 59, 109, 37, 1, 2);
                Func_08009180(45, 37, 1, 1, 45, 38);
                if (Func_080091c0(0x302) != 0) {
                    Func_080770c0(9, 194 << 18, 166 << 18);
                    Func_0808a0f0(10, 210 << 18, 166 << 18);
                    Func_0808a0f0(11, 194 << 18, 174 << 18);
                } else {
                    Func_0808a0f0(9, 210 << 18, 166 << 18);
                    Func_0808a0f0(10, 194 << 18, 174 << 18);
                    Func_0808a0f0(11, 210 << 18, 174 << 18);
                }
                Func_0808a0f0(9, 3);
                sprite = Func_0808a1e0(9);
                sprite->z = (s32)0xfff00000;
                sprite->flags = 2;
                Func_0808a080(10, 3);
                sprite = Func_0808a1e0(10);
                sprite->z = (s32)0xfff00000;
                sprite->flags = 2;
                Func_0808a080(11, 3);
                sprite = Func_0808a1e0(11);
                sprite->z = (s32)0xfff00000;
                sprite->flags = 2;
                Func_0808a080(12, 7);
                Func_0808a100(Func_0808a080(12), 0);
                Func_080091e0(12, 1);
                sprite = Func_0808a1e0(12);
                sprite->state59 = 0;
                sprite->flags = 2;
                Func_0808a080(12, 0x02d70000, 158 << 18);
                sprite->handler = Func_02000b98;
            }
            Func_0808a0f0();
            return 0;

        case 19:
            Func_02001f60();
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
        if (Func_080770c0(0x109) != 0) {
            Func_020022c8(0);
            Func_0808a080(Func_0200094c(0));
            for (i = 0; i <= 3; i++) {
                sprite = Func_0808a080(i + 10);
                if ((sprite->x >> 20) == 13 && (sprite->y >> 20) == 7 &&
                    Func_080770c0(512 + i) != 0) {
                    sprite->flags |= 2;
                    sprite->state59 = 0;
                    sprite->state55 = 0;
                    Func_0808a080(4, 19, 1, 1, 13, 7);
                    return 0;
                }
            }
            return 0;
        }
        sprite = Func_0808a1e0(8);
        sprite->state55 = 0;
        sprite->z = (s32)0xffd00000;
        sprite->flags |= 2;
        sprite->state59 &= 0xfe;
        sprite->counter64 = 3;
        Func_0808a080(8, 1);
        sprite = Func_0808a1e0(9);
        sprite->state55 = 0;
        sprite->z = (s32)0xffd00000;
        sprite->flags |= 2;
        sprite->state59 &= 0xfe;
        sprite->counter64 = 3;
        Func_0808a080(9, 1);
        sprite = Func_0808a080(10);
        sprite->state55 = 0;
        sprite->counter64 = 0;
        Func_080091e0(Func_0808a080(10), 0);
        sprite = Func_0808a080(11);
        sprite->state55 = 0;
        sprite->counter64 = 0;
        Func_080091e0(Func_0808a080(11), 0);
        sprite = Func_0808a080(12);
        sprite->state55 = 0;
        sprite->counter64 = 0;
        Func_080091e0(Func_0808a080(12), 0);
        sprite = Func_0808a080(13);
        sprite->state55 = 0;
        sprite->counter64 = 0;
        Func_080091e0(Func_0808a5e0(13), 0);
        return 0;

    case 13:
    case 14:
        goto scene_fade;

    case 18:
    case 19:
        REQUEST = 514;
        sprite = Func_0808a080(0);
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
        Func_0808a080(20)->state55 = 4;
        Func_0808a080(20)->flags |= 2;
        Func_0808a080(20)->z = (s32)0xffef8000;
        Func_080091a0();
        BLEND_CTRL = 0;
        if (Func_080770c0(0x306) != 0) {
            Func_0808a5e0(170);
            Func_08009180(36, 81, 32, 81, 3, 2);
            Func_08009180(36, 83, 36, 81, 3, 2);
            Func_080091c0(36, 17, 3, 1, 32, 17);
            Func_080091c0(36, 18, 3, 1, 36, 17);
            Func_08009180(63, 29, 33, 20, 1, 1);
            Func_08009180(20, 56, 36, 17, 3, 4);
        }
        if (Func_080770c0(0x307) != 0) {
            Func_08009180(44, 81, 48, 81, 3, 2);
            Func_08009180(44, 83, 44, 81, 3, 2);
            Func_080091c0(44, 17, 3, 1, 48, 17);
            Func_080091c0(44, 18, 3, 1, 44, 17);
            Func_08009180(63, 29, 49, 20, 1, 1);
            Func_08009180(41, 56, 44, 17, 3, 4);
        }
        step = (u16)Data_02000240[225];
        if ((u16)(step - 18) <= 1) {
            Func_0808a360();
            Func_0808a370();
            REQUEST = 516;
            step = (u16)Data_02000240[225];
        }
        if (step != 20) {
            return 0;
        }
        Func_02004a2c();
        return 0;

    case 15:
    case 16:
        Func_080091c0(1);
        Func_080000c0(Func_02003050, 3200);
        sprite = Func_080000d0(14);
        sprite->state55 = 0;
        sprite->z = 0;
        Func_0808a080(15)->state55 = 0;
        Func_0808a080(16)->state55 = 0;
        Func_0808a080(17)->state55 = 0;
        Func_0808a080(18)->state55 = 0;
        if (Func_0808a080(776) != 0) {
            Func_080770c0(1);
            Func_080000c0(95, 56, 77, 35, 1, 2);
            Func_08009180(13, 35, 1, 1, 13, 36);
            Func_080091c0(15, 132 << 17, 186 << 18);
            sprite = Func_0808a0f0(15);
            sprite->z = (s32)0xfff00000;
            sprite->flags = 2;
            Func_0808a080(15, 3);
            Func_0808a1e0(16, 184 << 16, 158 << 18);
            sprite = Func_0808a0f0(16);
            sprite->z = (s32)0xfff00000;
            sprite->flags = 2;
            Func_0808a080(16, 3);
            Func_0808a1e0(17, 232 << 16, 174 << 18);
            sprite = Func_0808a0f0(17);
            sprite->z = (s32)0xfff00000;
            sprite->flags = 2;
            Func_0808a080(17, 3);
            Func_0808a1e0(18, 184 << 16, 166 << 18);
            sprite = Func_0808a0f0(18);
            sprite->z = (s32)0xfff00000;
            sprite->flags = 2;
            Func_0808a080(18, 3);
            Func_0808a1e0(19, 7);
            Func_0808a100(Func_0808a080(19), 0);
            Func_080091e0(19, 1);
            sprite = Func_0808a1e0(19);
            sprite->state59 = 0;
            sprite->flags = 2;
            Func_0808a080(19, 215 << 16, 150 << 18);
            sprite->handler = Func_02000b98;
        }
        Func_0808a0f0();
        return 0;

    default:
        return 0;
    }

    /*
     * 0x02003c86: a two-instruction tail that four of the five jump tables
     * enter directly (scene 0xb7 states 1-4/19, 0xb8 states 1/2, 0xb9 states
     * 4-6, 0xba states 13/14).  Written once, reached by `goto`, so the file's
     * call count stays equal to the assembly's site count.
     */
scene_fade:
    Func_02002b14(170);
    return 0;
}
