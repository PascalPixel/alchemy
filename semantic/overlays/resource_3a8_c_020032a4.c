typedef unsigned char u8;
typedef signed int s32;

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
 * `bun tools/overlay_call_targets.ts resource_3a8 32a4` - 11 distinct targets,
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
struct SceneEntity *Func_0808a080();    /* scene entity by selector */
void Func_0808a098();                   /* start a scripted motion */
void Func_0808a0f0();                   /* place an entity at (x, z) */
void Func_0808a010();                   /* wait n frames */
void Func_0808a018();                   /* enter scripted-scene mode */
void Func_0808a020();                   /* leave scripted-scene mode */
s32 Func_080770c0();                    /* read a story flag */
void Func_080770c8();                   /* set a story flag */
void Func_08009180();                   /* six-argument scripted-actor call */
void Func_080091c0();                   /* six-argument scripted-actor call */

/* Intra-overlay callee, byte-exact in assets/code. */
void Func_020015b4(void);

/* In-image motion script at file offset 0x4268. */
extern u8 Data_0200c268[];

void Func_020032a4(void)
{
    struct SceneEntity *follower8;
    struct SceneEntity *follower9;
    struct SceneEntity *prop11;
    s32 flag;
    s32 i;

    follower8 = Func_0808a080(8);
    follower9 = Func_0808a080(9);

    flag = Func_080770c0(0x302);
    if (flag == 0 && (follower8->x >> 19) <= 29) {
        prop11 = Func_0808a080(11);

        Func_0808a018();
        Func_080091c0(7, 44, 1, 1, flag, 1);    /* flag is 0 here */

        i = 67;
        do {
            Func_08009180(i, 58, 78, 41, 1, 5);
            Func_0808a010(4);
            if (i == 70) {
                Func_080770c8(0x302);
            }
            i++;
        } while ((unsigned int)i <= 74);

        Func_08009180(16, 109, 13, 109, 3, 2);
        Func_0808a010(40);

        prop11->field18 = 0x1999;
        prop11->field1c = 0x1999;
        Func_0808a0f0(11, 0x00960000, 0x02d80000);
        Func_0808a098(11, Data_0200c268);

        Func_08009180(67, 64, 71, 44, 1, 2);
        Func_08009180(67, 64, 72, 44, 1, 2);
        Func_08009180(67, 68, 73, 43, 1, 2);
        Func_08009180(67, 68, 74, 43, 1, 2);
        Func_08009180(67, 64, 75, 44, 1, 2);
        Func_08009180(67, 66, 76, 44, 1, 2);
        Func_08009180(67, 64, 77, 44, 1, 2);
        Func_08009180(67, 64, 78, 44, 1, 2);
        Func_08009180(67, 64, 79, 44, 1, 2);
        Func_08009180(67, 66, 80, 44, 1, 2);
        Func_08009180(2, 0, 9, 42, 2, 2);
        Func_0808a010(40);

        Func_08009180(68, 64, 71, 44, 1, 2);
        Func_08009180(68, 64, 72, 44, 1, 2);
        Func_08009180(68, 68, 73, 43, 1, 2);
        Func_08009180(68, 68, 74, 43, 1, 2);
        Func_08009180(68, 64, 75, 44, 1, 2);
        Func_08009180(68, 66, 76, 44, 1, 2);
        Func_08009180(68, 64, 77, 44, 1, 2);
        Func_08009180(68, 64, 78, 44, 1, 2);
        Func_08009180(68, 64, 79, 44, 1, 2);
        Func_08009180(68, 66, 80, 44, 1, 2);
        Func_08009180(4, 0, 9, 42, 2, 2);
        Func_0808a010(40);

        Func_08009180(7, 11, 7, 42, 10, 8);
        Func_08009180(71, 12, 71, 43, 10, 13);
        Func_080091c0(6, 13, 12, 12, 6, 44);
        Func_0808a010(40);
        Func_020015b4();
        Func_080091c0(0, 1, 1, 1, 7, 44);
        Func_0808a020();
    }

    if (Func_080770c0(0x303) != 0 || (follower9->x >> 19) > 87) {
        return;
    }

    Func_0808a018();

    i = 67;
    do {
        Func_08009180(i, 58, 107, 41, 1, 5);
        Func_0808a010(4);
        if (i == 70) {
            Func_080770c8(0x303);
        }
        i++;
    } while ((unsigned int)i <= 74);

    Func_08009180(45, 109, 42, 109, 3, 2);
    Func_0808a010(40);

    Func_08009180(67, 64, 102, 44, 1, 2);
    Func_08009180(67, 64, 103, 44, 1, 2);
    Func_08009180(67, 64, 104, 44, 1, 2);
    Func_08009180(67, 66, 105, 44, 1, 2);
    Func_08009180(67, 64, 106, 44, 1, 2);
    Func_08009180(67, 64, 107, 44, 1, 2);
    Func_08009180(67, 64, 108, 44, 1, 2);
    Func_08009180(67, 66, 109, 44, 1, 2);
    Func_0808a010(40);

    Func_08009180(68, 64, 102, 44, 1, 2);
    Func_08009180(68, 64, 103, 44, 1, 2);
    Func_08009180(68, 64, 104, 44, 1, 2);
    Func_08009180(68, 66, 105, 44, 1, 2);
    Func_08009180(68, 64, 106, 44, 1, 2);
    Func_08009180(68, 64, 107, 44, 1, 2);
    Func_08009180(68, 64, 108, 44, 1, 2);
    Func_08009180(68, 66, 109, 44, 1, 2);
    Func_0808a010(40);

    Func_08009180(38, 14, 38, 44, 8, 4);
    Func_08009180(102, 14, 102, 44, 8, 12);
    Func_080091c0(37, 13, 10, 12, 37, 43);
    Func_0808a010(40);
    Func_020015b4();
    Func_0808a020();
}
