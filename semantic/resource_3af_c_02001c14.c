typedef unsigned char u8;
typedef int s32;

/*
 * resource_3af owner at 0x02001c14, 248 bytes: a two-actor scripted beat
 * wrapped in the established camera-transition bracket.
 *
 * It takes two actor ids: r0 (held in r5) is configured in detail, r1
 * (held in r6) is driven at the end. Both are pure pass-through
 * arguments -- nothing in the body constrains them, so they are spelled
 * as parameters rather than guessed constants.
 *
 * The bracket is the one first named in resource_37b_c_02000150.c and
 * resource_380_c_02001450.c: write the transition mode word to
 * workspace+448, then a pair of no-argument bracket helpers. Opening
 * uses mode 256 with Func_0808a360/Func_0808a370; closing uses mode
 * 0x202 (514) with Func_0808a368/Func_0808a370. The mode value is built
 * subtractively here -- `movs r3,#224 / lsls r3,#1` gives 448, which is
 * kept in sl as the DISPLACEMENT, and `subs r3,#192` turns the same
 * register into the VALUE 256. That is the additive/subtractive
 * workspace-write idiom (resource_377_c_02000a0c.c) with a subtractive
 * arm; do not assume the canonical 513. The closing value 0x202 comes
 * from a pool word, and the workspace pointer is re-read from
 * 0x03001ebc for the second write.
 *
 * Between the brackets: wait 20 frames, then a call to this overlay's
 * own Func_02000bb8 -- the once-only flag-0x271 reward grant already
 * drafted in resource_3af_c_02000bb8.c. That is the first caller found
 * for either of the 0xbb8/0xbf0 twins, and it fixes the twin's role:
 * the reward is granted from inside a scripted beat, not from a
 * dispatcher.
 *
 * Actor r5 is then placed (Func_0808a0f0 at 216<<16, 147<<18), given
 * the 0xcccc/0x6666 fixed-point pair through Func_0808a090 -- the same
 * pair actor 21 gets in this overlay's 0x0200185c scene entry -- and
 * given four Func_0808a0d0 triples: (216,600), (218,604), (234,604),
 * (236,618). Then Func_0808a1b8(id, 0x5000, 20) and
 * Func_0808a110(id, 3), and another 20-frame wait.
 *
 * Actor r6 gets Func_02003a00(id, 0x5000) -- a 12-byte owner in this
 * same overlay at 0x02003a00 that is already tracked byte-exact
 * (assets/code/resource_3af_c_02003a00.c), where it reads
 * `Func_02007dda(a, b, 10)` against the veneer address; resolved
 * through the +2 rule that veneer is Func_0808a1b8, so this call pins
 * that helper's third argument to 10. It is declared, not inlined,
 * because the source calls it -- then Func_0808a128(id, 4, 40),
 * Func_0808a130(id, 2), a dialogue line (Func_0808a170(0x1e39)), and
 * Func_0808a188(id, 0, 20). The beat closes with the transition bracket
 * and a 10-unit Func_0808a248.
 *
 * Complete owner: `push {r5, r6, lr}` plus the sl/r8 spill
 * (`mov r6,sl / mov r5,r8 / push {r5, r6}`) at 0x02001c14 through the
 * matching unwind `pop {r3, r5} / mov r8,r3 / mov sl,r5 / pop {r5, r6} /
 * pop {r0} / bx r0` at 0x02001ce8-0x02001cf2; six trailing pool words
 * (0x03001ebc, 0x0000cccc, 0x00006666, 0x0000026a, 0x00001e39,
 * 0x00000202) end at 0x02001d0b, and the next owner prologue
 * (`push {lr}`) begins at 0x02001d0c.
 *
 * That next prologue at 0x02001d0c is NOT a queue row, and it is NOT an
 * uncovered gap either: it is already tracked byte-exact as
 * assets/code/resource_3af_c_02001d0c.c. resource_3af carries 36
 * exact-C files, so in THIS overlay a prologue outside the queue is more
 * likely already owned than free -- check assets/code before spending a
 * boundary-gap read.
 *
 * Not found by the structural inventory walk: reached only by `bl`
 * (bun tools/overlay_call_targets.ts resource_3af 1c14 1d0c, the +2
 * rule against the raw image -- every import name below comes from that
 * resolution, never from overlay_show's pc-relative annotation).
 *
 * Uncertainty: the two parameters are actor ids by shape only; the
 * Func_0808a0d0 triples and the fixed-point pair are transcribed, their
 * meanings unknown.
 */

extern u8 *Data_03001ebc;       /* overlay workspace pointer, IWRAM cell */

extern void Func_0808a010(s32 frames);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a128(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a188(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a248(s32 arg0);
extern void Func_0808a360(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_02000bb8(void);
extern void Func_02003a00(s32 id, s32 arg1);

void Func_02001c14(s32 actor, s32 partner)
{
    u8 *workspace;

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 448 - 192;
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);

    Func_02000bb8();

    Func_0808a0f0(actor, 216 << 16, 147 << 18);
    Func_0808a090(actor, 0xcccc, 0x6666);
    Func_0808a0d0(actor, 216, 150 << 2);
    Func_0808a0d0(actor, 218, 151 << 2);
    Func_0808a0d0(actor, 234, 151 << 2);
    Func_0808a0d0(actor, 236, 0x26a);
    Func_0808a1b8(actor, 160 << 7, 20);
    Func_0808a110(actor, 3);
    Func_0808a010(20);

    Func_02003a00(partner, 160 << 7);
    Func_0808a128(partner, 4, 40);
    Func_0808a130(partner, 2);
    Func_0808a170(0x1e39);
    Func_0808a188(partner, 0, 20);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 0x202;
    Func_0808a368();
    Func_0808a370();
    Func_0808a248(10);
}
