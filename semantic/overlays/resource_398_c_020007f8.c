typedef signed short s16;
typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * Resource 398 push-the-block interaction at 0x020007f8.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the high-register save
 * `mov r7,sl / mov r6,r9 / mov r5,r8 / push {r5, r6, r7}` at 0x020007f8, and
 * the matching unwind `add sp,#12 / pop {r3, r5, r6} / mov r8,r3 / mov r9,r5 /
 * mov sl,r6 / pop {r5, r6, r7} / pop {r0} / bx r0` at 0x020008e6.  268-byte
 * row: 254 bytes of code, an alignment halfword at 0x020008f6, and the three
 * pool words 0x02009064, 0xffff0000 and 0x00003333 filling
 * 0x020008f8-0x02000903.  Control-flow walk: every branch target is
 * 0x020008e6 or below and the bare `bx lr` leaf at 0x02000904 follows the
 * pool.  The return address is popped into r0, so the owner is `void`.
 *
 * Role.  Two interaction records name this address (pool word 0x020087f9 =
 * 0x0200_07f8 + the Thumb bit under the proven 0x02008000 link base), keyed
 * 0x00000202/0xffff000a and 0x00008602/0xffff000b.
 *
 * Direction table.  0x02009064 is in-image data at file offset 0x1064 (even
 * pool word, so data rather than a Thumb entry).  It is the packed-direction
 * family: indexed by `heading >> 12`, X step in the high halfword and Z step
 * in the low halfword, promoted back to 16.16 by `& 0xffff0000` and `<< 16`
 * rather than by multiply.
 *
 * Coordinate views.  The s16 at +0x0a and +0x12 are the integer parts of the
 * 16.16 words at +0x08 and +0x10, so `(integer + (step >> 16)) >> 4` is the
 * neighbouring tile.  That is exactly the pair the tracked byte-exact
 * assets/code/resource_398_c_020007c4.c compares as `p[2] >> 20` and
 * `p[4] >> 20` — the lookup takes tile coordinates.
 *
 * Behaviour: find the actor one step ahead of the player; require the tile
 * beyond it to be empty; stage the pushed position in the three-word frame
 * slot, ask Func_080091d8 whether the move collides, and if it does not,
 * commit it and hand off to the paired-actor gate scene at 0x02000304.
 *
 * Imports resolved with tools/overlay_call_targets.ts (an overlay `bl` stores
 * `target_offset - 2`).  Twelve call sites against the row's advertised 11;
 * the manifest's `calls` field is a floor.  Per-target: Func_0808a080 1,
 * Func_020007c4 2, Func_080091d8 1, Func_08009080 2, Func_080000c0 1,
 * Func_080f9010 1, Func_08009150 2, Func_08009158 1, Func_02000304 1.
 *
 * `blocker` is provably zero where it is stored at +0x24 and +0x2c — the guard
 * above returns early otherwise — but it is the register the assembly stores,
 * so it is spelled as itself rather than folded to a literal.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */
u8 *Func_0808a080();   /* actor record by slot id */
s32 *Func_020007c4();  /* overlay-local: actor occupying tile (x, z), or 0 */
s32 Func_080091d8();   /* collision probe: >0 means the move is blocked */
void Func_08009080();  /* set actor motion state */
void Func_080000c0();  /* audio cue */
void Func_080f9010();  /* present message by id */
void Func_08009150();  /* start a slide to (x, y, z) */
void Func_08009158();  /* commit the slide */
void Func_02000304(void);

extern s32 Data_02009064[]; /* packed direction steps, 16 entries */

void Func_020007f8(void)
{
    u8 *player;
    u8 *target;
    u8 *blocker;
    s32 heading;
    s32 step;
    s32 place[3];

    player = Func_0808a080(0);
    heading = *(u16 *)(player + 6) >> 12;
    step = Data_02009064[heading];

    target = (u8 *)Func_020007c4(
        (*(s16 *)(player + 10) + (step >> 16)) >> 4,
        (*(s16 *)(player + 18) + ((step << 16) >> 16)) >> 4);
    if (target == 0) return;

    step = Data_02009064[heading];
    blocker = (u8 *)Func_020007c4(
        (*(s16 *)(target + 10) + (step >> 16)) >> 4,
        (*(s16 *)(target + 18) + ((step << 16) >> 16)) >> 4);
    if (blocker != 0) return;

    target[0x22] = 2;

    step = Data_02009064[heading];
    place[0] = *(s32 *)(target + 8) + (step & (s32)0xffff0000);
    place[1] = *(s32 *)(target + 12);
    place[2] = *(s32 *)(target + 16) + (step << 16);

    if (Func_080091d8(target, place) > 0) return;

    Func_08009080(player, 8);
    Func_080000c0(15);
    Func_080f9010(185);

    *(s32 *)(target + 48) = 0x3333;
    *(s32 *)(target + 52) = 0x3333;
    Func_08009150(target, place[0], place[1], place[2]);

    *(s32 *)(player + 48) = 0x3333;
    *(s32 *)(player + 52) = 0x3333;
    Func_08009150(player, place[0], place[1], place[2]);

    Func_08009158(target);

    *(s32 *)(target + 8) = place[0];
    *(s32 *)(target + 16) = place[2];
    *(s32 *)(target + 36) = (s32)blocker;
    *(s32 *)(target + 44) = (s32)blocker;

    Func_08009080(player, 1);
    Func_02000304();
}
