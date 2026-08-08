#include "types.h"

/*
 * resource_3a8 owner at 0x020032a4, 1,108 bytes: the two scripted cutscenes
 * that fire when follower 8 or follower 9 has been pushed far enough.
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7, sl / mov r6, r8 /
 * push {r6, r7} / sub sp, #8` at 0x020032a4 through the single interworking
 * epilogue at 0x020036ea (`add sp,#8 / pop {r3,r5} / mov r8,r3 / mov sl,r5 /
 * pop {r5,r6,r7} / pop {r0} / bx r0`).  One interior literal pool at
 * 0x02003562-0x02003573 (an alignment halfword plus four words) sits between
 * the second arm's loop tail and its `adds r5,#1`, is branched over by both
 * `bne 0x2003574` and `b 0x2003574`, and is data rather than code; it is
 * excluded.  The 8-byte frame carries the fifth and sixth arguments of the
 * six-argument calls and nothing else.  Nothing is live past the return.
 *
 * Signature.  The epilogue is `pop {r0} ; bx r0`, so r0 holds the popped
 * return address and the owner returns nothing: `void`.  r0 is written with 8
 * before any read and r1-r3 are never read before being written, so it takes
 * no arguments.  It is called from Func_020031b8, which passes nothing.
 *
 * Call accounting: 75 `bl` sites, all resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3a8 32a4` - 11 distinct targets,
 * 73 import veneers plus 2 calls to the intra-overlay prologue 0x020015b4
 * (which has a byte-exact source in assets/code).  Every per-target count
 * below was reproduced by hand from `printed_target - site - 2` and matches
 * the tool exactly:
 *   0x02003b9c -> Func_08009180  x46      0x02003ba4 -> Func_080091c0   x4
 *   0x02003bec -> Func_0808a010  x10      0x02003bf4 -> Func_0808a018   x2
 *   0x02003bfc -> Func_0808a020   x2      0x02003c1c -> Func_0808a080   x3
 *   0x02003c34 -> Func_0808a098   x1      0x02003c74 -> Func_0808a0f0   x1
 *   0x02003bd4 -> Func_080770c0   x2      0x02003bdc -> Func_080770c8   x2
 *   0x020015b4 -> prologue        x2
 * The inventory row advertises `calls=71`; the difference is the four sites
 * inside the two do/while loops, which the row folds.
 *
 * LINK BASE 0x02008000, confirmed again: the only in-image-looking pool word,
 * 0x0200c268, is handed to Func_0808a098 in the argument position that every
 * other source in the tree fills with a `Data_0200a*`/`Data_0200b*` script
 * pointer, and 0x0200c268 - 0x8000 = 0x4268 is inside this image.  The other
 * three pool words are the story-flag ids 0x302 and 0x303 and the constant
 * 0x1999.
 *
 * Func_08009180 and Func_080091c0 are the established six-argument scripted
 * -actor calls (four register arguments plus two stack words); the tree
 * already spells both with six arguments at dozens of sites.  Func_0808a018 /
 * Func_0808a020 bracket a cutscene, Func_0808a010(frames) waits, and
 * Func_0808a0f0(selector, x, z) places an entity.
 *
 * Structure: two independent, near-identical arms.  Each is gated on its own
 * story flag being unset AND on its follower's x (an `asr #19` of the 16.16
 * word at +8) still being at or below a threshold, then plays a bracketed
 * scripted sequence and latches the flag partway through - inside the opening
 * loop, on the single iteration where the counter equals 70.
 *
 * UNCERTAINTIES.
 *  - Func_080091c0's first call takes r5 as its fifth argument, and r5 there
 *    is the Func_080770c0(0x302) result, which is provably 0 on that path.  It
 *    is written as 0 below.
 *  - Neither six-argument import's parameter meanings are established; the
 *    literals are reproduced positionally.
 *  - The two `>> 19` gates are arithmetic shifts of a 16.16 coordinate, i.e.
 *    eighths of an integer unit; the thresholds 29 and 87 are kept raw rather
 *    than rescaled.
 */

struct SceneEntity {
    u8 unknown_00[8];
    s32 x;                          /* 0x08, 16.16 */
    s32 y;                          /* 0x0c */
    s32 z;                          /* 0x10 */
    u8 unknown_14[4];
    s32 field18;                    /* 0x18 */
    s32 field1c;                    /* 0x1c */
};

/* Old-style declarations: overlay imports vary in arity between call sites. */
struct SceneEntity *Func_02006ece();
struct SceneEntity *Func_02006ed6();
s32 Func_02006e96();
struct SceneEntity *Func_02006ef6();
void Func_02006ed4();
void Func_02006e94();
void Func_02006ea2();
void Func_02006ef8();
void Func_02006ef2();
void Func_02006ecc();
void Func_02006f22();
void Func_02006fc0();
void Func_02006f88();
void Func_02006f02();
void Func_02006f12();
void Func_02006f32();
void Func_02006f42();
void Func_02006f52();
void Func_02006f62();
void Func_02006f72();
void Func_02006f82();
void Func_02006f92();
void Func_02006fa2();
void Func_02006ff8();
void Func_02006fb8();
void Func_02006fc8();
void Func_02006fd8();
void Func_02006fe8();
void Func_02007008();
void Func_02007018();
void Func_02007028();
void Func_02007038();
void Func_02007048();
void Func_02007058();
void Func_020070ae();
void Func_02007072();
void Func_02007084();
void Func_020070a0();
void Func_020070ee();
void Func_02004aba(void);
void Func_020070bc();
void Func_02007118();
s32 Func_020070f6();
void Func_0200712c();
void Func_020070ea();
void Func_02007140();
void Func_0200713a();
void Func_02007128();
void Func_0200717e();
void Func_02007150();
void Func_02007160();
void Func_02007170();
void Func_02007180();
void Func_02007190();
void Func_020071a0();
void Func_020071b0();
void Func_02007206();
void Func_020071c6();
void Func_020071d6();
void Func_020071e6();
void Func_020071f6();
void Func_02007216();
void Func_02007226();
void Func_02007236();
void Func_0200728c();
void Func_02007250();
void Func_02007262();
void Func_0200727e();
void Func_020072cc();
void Func_02004c98(void);
void Func_020072e4();
                                        /* scene entity by selector */
                                        /* start a scripted motion */
                                        /* place an entity at (x, z) */
                                        /* wait n frames */
                                        /* enter scripted-scene mode */
                                        /* leave scripted-scene mode */
                                        /* read a story flag */
                                        /* set a story flag */
                                        /* six-argument scripted-actor call */
                                        /* six-argument scripted-actor call */

/* Intra-overlay callee, byte-exact in assets/code. */

/* In-image motion script at file offset 0x4268. */
extern u8 Data_0200c268[];

void Func_020032a4(void)
{
    struct SceneEntity *follower8;
    struct SceneEntity *follower9;
    struct SceneEntity *prop11;
    s32 flag;
    s32 i;

    follower8 = Func_02006ece(8);
    follower9 = Func_02006ed6(9);

    flag = Func_02006e96(0x302);
    if (flag == 0 && (follower8->x >> 19) <= 29) {
        prop11 = Func_02006ef6(11);

        Func_02006ed4();
        Func_02006e94(7, 44, 1, 1, flag, 1);    /* flag is 0 here */

        i = 67;
        do {
            Func_02006ea2(i, 58, 78, 41, 1, 5);
            Func_02006ef8(4);
            if (i == 70) {
                Func_02006ef2(0x302);
            }
            i++;
        } while ((unsigned int)i <= 74);

        Func_02006ecc(16, 109, 13, 109, 3, 2);
        Func_02006f22(40);

        prop11->field1c = 0x1999;
        Func_02006fc0(11, 0x00960000, 0x02d80000);
        prop11->field18 = 0x1999;
        Func_02006f88(11, Data_0200c268);

        Func_02006f02(67, 64, 71, 44, 1, 2);
        Func_02006f12(67, 64, 72, 44, 1, 2);
        Func_02006f22(67, 68, 73, 43, 1, 2);
        Func_02006f32(67, 68, 74, 43, 1, 2);
        Func_02006f42(67, 64, 75, 44, 1, 2);
        Func_02006f52(67, 66, 76, 44, 1, 2);
        Func_02006f62(67, 64, 77, 44, 1, 2);
        Func_02006f72(67, 64, 78, 44, 1, 2);
        Func_02006f82(67, 64, 79, 44, 1, 2);
        Func_02006f92(67, 66, 80, 44, 1, 2);
        Func_02006fa2(2, 0, 9, 42, 2, 2);
        Func_02006ff8(40);

        Func_02006fb8(68, 64, 71, 44, 1, 2);
        Func_02006fc8(68, 64, 72, 44, 1, 2);
        Func_02006fd8(68, 68, 73, 43, 1, 2);
        Func_02006fe8(68, 68, 74, 43, 1, 2);
        Func_02006ff8(68, 64, 75, 44, 1, 2);
        Func_02007008(68, 66, 76, 44, 1, 2);
        Func_02007018(68, 64, 77, 44, 1, 2);
        Func_02007028(68, 64, 78, 44, 1, 2);
        Func_02007038(68, 64, 79, 44, 1, 2);
        Func_02007048(68, 66, 80, 44, 1, 2);
        Func_02007058(4, 0, 9, 42, 2, 2);
        Func_020070ae(40);

        Func_02007072(7, 11, 7, 42, 10, 8);
        Func_02007084(71, 12, 71, 43, 10, 13);
        Func_020070a0(6, 13, 12, 12, 6, 44);
        Func_020070ee(40);
        Func_02004aba();
        Func_020070bc(0, 1, 1, 1, 7, 44);
        Func_02007118();
    }

    if (Func_020070f6(0x303) != 0 || (follower9->x >> 19) > 87) {
        return;
    }

    Func_0200712c();

    i = 67;
    do {
        Func_020070ea(i, 58, 107, 41, 1, 5);
        Func_02007140(4);
        if (i == 70) {
            Func_0200713a(0x303);
        }
        i++;
    } while ((unsigned int)i <= 74);

    Func_02007128(45, 109, 42, 109, 3, 2);
    Func_0200717e(40);

    Func_02007140(67, 64, 102, 44, 1, 2);
    Func_02007150(67, 64, 103, 44, 1, 2);
    Func_02007160(67, 64, 104, 44, 1, 2);
    Func_02007170(67, 66, 105, 44, 1, 2);
    Func_02007180(67, 64, 106, 44, 1, 2);
    Func_02007190(67, 64, 107, 44, 1, 2);
    Func_020071a0(67, 64, 108, 44, 1, 2);
    Func_020071b0(67, 66, 109, 44, 1, 2);
    Func_02007206(40);

    Func_020071c6(68, 64, 102, 44, 1, 2);
    Func_020071d6(68, 64, 103, 44, 1, 2);
    Func_020071e6(68, 64, 104, 44, 1, 2);
    Func_020071f6(68, 66, 105, 44, 1, 2);
    Func_02007206(68, 64, 106, 44, 1, 2);
    Func_02007216(68, 64, 107, 44, 1, 2);
    Func_02007226(68, 64, 108, 44, 1, 2);
    Func_02007236(68, 66, 109, 44, 1, 2);
    Func_0200728c(40);

    Func_02007250(38, 14, 38, 44, 8, 4);
    Func_02007262(102, 14, 102, 44, 8, 12);
    Func_0200727e(37, 13, 10, 12, 37, 43);
    Func_020072cc(40);
    Func_02004c98();
    Func_020072e4();
}
