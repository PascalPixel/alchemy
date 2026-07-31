typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;

/*
 * resource_3bf owner at 0x020000c4, 384 bytes: "push the object in front of
 * the player one square".
 *
 * TRANSPOSED from semantic/overlays/resource_373_c_020000c4.c.  The two owners
 * are the same routine shared verbatim between the overlays: over all 192
 * halfwords they differ in exactly 21 places, 20 of which are the two halves of
 * the ten BL pairs (each overlay's veneer table sits at a different offset) and
 * one of which is the low half of the pool word at +0x174 that names the
 * direction-step table.  Nothing structural differs.
 *
 * What was changed from the source:
 *  - the direction table pool word 0x0200e190 becomes 0x0200ded8 here
 *    (file offset 0x5ed8 under the proven 0x02008000 link base);
 *  - every call was re-resolved with 'bun tools/overlay_call_targets.ts
 *    resource_3bf 00c4', which reports the same 13 sites in the same order:
 *    9 distinct targets, 10 veneer sites and 3 prologue sites.  The imports are
 *    therefore named by the main-image address their veneer publishes, which is
 *    identical in both overlays; only the veneer offsets differ, and those do
 *    not appear in C.
 *
 * The three "is this position occupied" probes all resolve to 0x0200006c, the
 * overlay's own slot lookup, whose byte-exact source (assets/code/) returns the
 * occupying record or 0 — exactly how each result is used here.  (The 373
 * source spelled them as three separate encoded addresses; the corrected 'bl'
 * rule collapses them into one callee.)
 *
 * The epilogue is a single `add sp, #12` tail popping the return address, so
 * the owner is void.
 */

struct PushMover {
    u8 unknown_00[6];
    u16 heading;                    /* top four bits select the direction */
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c */
    s32 z;                          /* 0x10 */
    u8 unknown_14[0x0e];
    u8 state22;                     /* 0x22 */
    u8 unknown_23[1];
    s32 velocityX;                  /* 0x24 */
    u8 unknown_28[4];
    s32 velocityZ;                  /* 0x2c */
    s32 field30;
    s32 field34;
    s32 accelerationX;              /* 0x38 */
    u8 unknown_3c[4];
    s32 accelerationZ;              /* 0x40 */
    u8 unknown_44[0x15];
    u8 flags59;                     /* 0x59 */
    u8 unknown_5a[8];
    u8 blocked62;                   /* 0x62 */
};

struct PushProbe {
    s32 x;
    s32 y;
    s32 z;
};

/*
 * The reference rewrites x and z from their own high halfwords, i.e. it snaps
 * the 16.16 coordinate to a whole square by discarding the fraction.
 */
#define PUSH_WHOLE(field) (((s16 *)&(field))[1])

/* Packed direction steps: X step in the high halfword, Z step in the low. */
#define PUSH_DIRECTION_STEPS ((const s32 *)0x0200ded8)

/* Old-style declarations are mandatory here: one overlay import is reached
 * with different argument counts at different sites. */
s32 Func_0200006c();
void Func_080000c0();
void Func_08009080();
void Func_08009150();
void Func_08009158();
s32 Func_080091d8();
s32 Func_0808a080();
void Func_0808a5e8();
void Func_080f9010();

static void step_from(const struct PushMover *mover, s32 step,
                      struct PushProbe *probe)
{
    probe->x = mover->x + (step & (s32)0xffff0000);
    probe->y = mover->y;
    probe->z = mover->z + (step << 16);
}

void Func_020000c4(void)
{
    struct PushProbe probe;
    struct PushMover *player = Func_0808a080(0);
    s32 direction = player->heading >> 12;
    struct PushMover *pushed;
    struct PushMover *obstacle;

    step_from(player, PUSH_DIRECTION_STEPS[direction], &probe);
    pushed = Func_0200006c(&probe, player);
    if (pushed == 0) {
        return;
    }

    step_from(pushed, PUSH_DIRECTION_STEPS[direction], &probe);
    obstacle = Func_0200006c(&probe, pushed);
    if (obstacle != 0 && (obstacle->flags59 & 1) != 0) {
        return;
    }

    /* The third probe drops one square in Y instead of stepping sideways. */
    probe.x = pushed->x;
    probe.y = pushed->y + 0x00100000;   /* 0x80 << 13 */
    probe.z = pushed->z;
    obstacle = Func_0200006c(&probe, pushed);
    if (obstacle != 0 && (obstacle->flags59 & 1) != 0) {
        return;
    }

    pushed->state22 = 2;

    step_from(pushed, PUSH_DIRECTION_STEPS[direction], &probe);
    if (Func_080091d8(pushed, &probe) > 0) {
        return;
    }
    if (pushed->blocked62 != 0) {
        return;
    }

    Func_08009080(player, 8);
    Func_080000c0(15);
    Func_080f9010(185);

    pushed->field30 = 0x3333;
    pushed->field34 = 0x3333;
    Func_08009150(pushed, probe.x, probe.y, probe.z);

    player->field30 = 0x3333;
    player->field34 = 0x3333;
    Func_08009150(player, probe.x, probe.y, probe.z);

    Func_08009158(pushed);
    Func_0808a5e8();

    pushed->x = probe.x;
    pushed->z = probe.z;
    pushed->velocityX = 0;
    pushed->velocityZ = 0;

    player->accelerationX = (s32)0x80000000;
    player->accelerationZ = (s32)0x80000000;
    player->velocityX = 0;
    player->velocityZ = 0;
    player->x = (s32)PUSH_WHOLE(player->x) << 16;
    player->z = (s32)PUSH_WHOLE(player->z) << 16;

    Func_08009080(player, 1);
}
