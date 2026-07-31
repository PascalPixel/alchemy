typedef signed int s32;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * Resource 3c8 at 0x02000374: "push the object in front of the player" step -
 * probe the tile the player faces, refuse if anything there is flagged, then
 * slide both the pushed object and the player one step along the facing
 * direction.
 *
 * The owner is complete: `push {r5, r6, r7, lr}` plus the high-register save
 * `mov r7, sl ; mov r6, r9 ; mov r5, r8 ; push {r5, r6, r7}` at 0x02000374,
 * a 12-byte frame, and the matching unwind at 0x020004d6..0x020004e4,
 * followed by an alignment halfword and a three-word literal pool at
 * 0x020004e6-0x020004f3.  No live frame or register state escapes the row;
 * the next prologue is at 0x020004f4.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.  Every failed guard branches to that same epilogue.
 *
 * PACKED DIRECTION WORD.  `Data_0200d0e8` is a table indexed by
 * `heading >> 12` - the top nibble of the halfword at +6 - with 4-byte
 * entries.  The high half of the entry is the X step and the low half is the
 * Z step, promoted to 16.16 by `& 0xffff0000` and `<< 16` respectively rather
 * than by a multiply.  This is the same family HANDOVER records for
 * resource_373's 0x0200e190 and for the resource_3bd conversion.  The table
 * is reloaded from memory at each of the three probe sites; the reloads are
 * preserved.
 *
 * The three probe calls at 0x020006e6, 0x02000712 and 0x0200073e each take a
 * pointer to the 12-byte candidate position on the stack plus the record
 * being moved.  Two of them return a record whose byte at +0x59 is tested for
 * bit 0 - `state59` in
 * semantic/overlays/resource_3c8_c_02001d48.c - and abort the step when it is
 * set.  0x0200528c returns a signed value that must not be positive.
 *
 * Field offsets are the ones proven for this package's actor record: 8/12/16
 * the 16.16 x/y/z triple, 0x22 `flags22`, 0x59 `state59`, and the signed
 * halfwords at 0x0a/0x12 which are the integer parts of x and z (the two
 * struct views of the byte-exact sibling 0x020014f4).  The final block
 * rewrites the player's x and z from those halfwords, i.e. snaps the player
 * back onto the tile grid.
 *
 * 0x80000000 is built as `movs r3, #128 ; lsls r3, r3, #24`; 0x00100000 as
 * `movs r0, #128 ; lsls r0, r0, #13`; 0x3333 and 0xffff0000 are pooled.
 *
 * Thirteen `bl` sites reach twelve distinct targets - 0x020052a8 is called
 * twice, with one and with two arguments, which is why every import is
 * declared old-style.  The full list: 0x02005230, 0x020006e6, 0x02000712,
 * 0x0200073e, 0x0200528c, 0x0200523c, 0x02005214, 0x02005472, 0x0200528a,
 * 0x0200529a, 0x020052a8, 0x02005494.  Import naming follows the note in
 * resource_3c8_c_020002f0.c; the three probe addresses fall numerically
 * inside this overlay's own code region but are import identities, not
 * locations.
 */

struct Actor_02000374 {
    u8 unknown_00[6];
    u16 heading;                /* 0x06 */
    s32 x;                      /* 0x08 - 16.16 */
    s32 y;                      /* 0x0c */
    s32 z;                      /* 0x10 - 16.16 */
    u8 unknown_14[0x0e];
    u8 flags22;                 /* 0x22 */
    u8 unknown_23[1];
    s32 state24;                /* 0x24 */
    u8 unknown_28[4];
    s32 state2c;                /* 0x2c */
    s32 rate30;                 /* 0x30 */
    s32 rate34;                 /* 0x34 */
    s32 limit38;                /* 0x38 */
    u8 unknown_3c[4];
    s32 limit40;                /* 0x40 */
    u8 unknown_44[0x15];
    u8 state59;                 /* 0x59 */
    u8 unknown_5a[8];
    u8 lock62;                  /* 0x62 */
};

struct Position_02000374 {
    s32 x;
    s32 y;
    s32 z;
};

/* 16-entry packed direction table, indexed by `heading >> 12`. */
extern s32 Data_0200d0e8[];

/* Used for their return values. */
struct Actor_02000374 *Func_0200032c();
void Func_080000c0();
void Func_08009080();
void Func_08009150();
void Func_08009158();
s32 Func_080091d8();
struct Actor_02000374 *Func_0808a080();
void Func_0808a5e8();
void Func_080f9010();

/* Old-style declarations: the imports' real interfaces are not known here. */

void Func_02000374(void)
{
    struct Actor_02000374 *player;
    struct Actor_02000374 *pushed;
    struct Actor_02000374 *blocker;
    struct Position_02000374 probe;
    s32 heading;
    s32 step;

    player = Func_0808a080(0);
    heading = player->heading >> 12;

    step = Data_0200d0e8[heading];
    probe.x = player->x + (s32)(step & 0xffff0000);
    probe.y = player->y;
    probe.z = player->z + (step << 16);
    pushed = Func_0200032c(&probe, player);
    if (pushed == 0) {
        return;
    }

    step = Data_0200d0e8[heading];
    probe.x = pushed->x + (s32)(step & 0xffff0000);
    probe.y = pushed->y;
    probe.z = pushed->z + (step << 16);
    blocker = Func_0200032c(&probe, pushed);
    if (blocker != 0 && (blocker->state59 & 1) != 0) {
        return;
    }

    probe.x = pushed->x;
    probe.y = pushed->y + 0x00100000;
    probe.z = pushed->z;
    blocker = Func_0200032c(&probe, pushed);
    if (blocker != 0 && (blocker->state59 & 1) != 0) {
        return;
    }

    pushed->flags22 = 2;

    step = Data_0200d0e8[heading];
    probe.x = pushed->x + (s32)(step & 0xffff0000);
    probe.y = pushed->y;
    probe.z = pushed->z + (step << 16);
    if (Func_080091d8(pushed, &probe) > 0) {
        return;
    }

    if (pushed->lock62 != 0) {
        return;
    }

    Func_08009080(player, 8);
    Func_080000c0(15);
    Func_080f9010(185);

    pushed->rate30 = 0x3333;
    pushed->rate34 = 0x3333;
    Func_08009150(pushed, probe.x, probe.y, probe.z);

    player->rate30 = 0x3333;
    player->rate34 = 0x3333;
    Func_08009150(player, probe.x, probe.y, probe.z);

    Func_08009158(pushed);
    Func_0808a5e8();

    pushed->x = probe.x;
    pushed->z = probe.z;
    pushed->state24 = 0;
    pushed->state2c = 0;

    player->limit38 = 0x80000000;
    player->limit40 = 0x80000000;
    player->state24 = 0;
    player->state2c = 0;
    /* Snap the player onto the tile grid: +0x0a and +0x12 are the integer
     * parts of the 16.16 x and z words above. */
    player->x = *(s16 *)((u8 *)player + 10) << 16;
    player->z = *(s16 *)((u8 *)player + 18) << 16;
    Func_08009080(player, 1);
}
