typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;

/*
 * Resource 373: push the object in front of the player one square.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/r9/sl saves and the
 * 12-byte local frame at 0x020000c4, through the single `add sp, #12`
 * epilogue at 0x02000226..0x02000234.  Every abort path branches to that
 * epilogue; nothing is live past it.
 *
 * All thirteen calls are placed.  None is an interworking `call_via rN`
 * site: r3 is only ever a scratch value here.
 *
 * IMPORTANT UNCERTAINTY — call targets.  Three of the calls encode targets
 * (0x02000176, 0x020001a2, 0x020001ce) that land *inside this very function*,
 * at addresses that are plain join points in its own control flow.  A call
 * cannot mean that, so the overlay's `bl` displacements must be fixed up when
 * the module is loaded; the encoded value is a stable identity for an
 * imported service, not a location.  The three sites are structurally the
 * same "is this position blocked" probe and are spaced identically to their
 * encoded targets, so they are very likely one service; they are nevertheless
 * spelled with their distinct encoded addresses, because that is the evidence
 * the tree holds and it is the convention already used for resource 394 and
 * resource 3c8.  Anyone resolving the overlay relocation table should expect
 * to collapse them.
 *
 * The direction table at 0x0200e190 packs the X step in the high half of its
 * word and the Z step in the low half, both already 16.16 fixed; this is the
 * same idiom Func_020002a8 and Func_0200034c use.
 */

struct Resource373Mover {
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

struct Resource373Probe {
    s32 x;
    s32 y;
    s32 z;
};

/*
 * The reference rewrites x and z from their own high halfwords, i.e. it
 * snaps the 16.16 coordinate to a whole square by discarding the fraction.
 */
#define RESOURCE_373_WHOLE(field) (((s16 *)&(field))[1])

#define RESOURCE_373_DIRECTION_STEPS ((const s32 *)0x0200e190)

struct Resource373Mover *Func_020060b8(s32 index);
struct Resource373Mover *Func_02000176(const struct Resource373Probe *probe,
                                       struct Resource373Mover *mover);
struct Resource373Mover *Func_020001a2(const struct Resource373Probe *probe,
                                       struct Resource373Mover *mover);
struct Resource373Mover *Func_020001ce(const struct Resource373Probe *probe,
                                       struct Resource373Mover *mover);
s32 Func_020060e4(struct Resource373Mover *mover,
                  const struct Resource373Probe *probe);
void Func_0200604c(s32 channel);
void Func_020060ac(struct Resource373Mover *mover, s32 mode);
void Func_020060f2(struct Resource373Mover *mover, s32 x, s32 y, s32 z);
void Func_02006102(struct Resource373Mover *mover, s32 x, s32 y, s32 z);
void Func_02006110(struct Resource373Mover *mover);
void Func_02006118(struct Resource373Mover *mover, s32 mode);
void Func_02006312(s32 sound);
void Func_02006334(void);

static void step_from(const struct Resource373Mover *mover, s32 step,
                      struct Resource373Probe *probe)
{
    probe->x = mover->x + (step & (s32)0xffff0000);
    probe->y = mover->y;
    probe->z = mover->z + (step << 16);
}

void Func_020000c4(void)
{
    struct Resource373Probe probe;
    struct Resource373Mover *player = Func_020060b8(0);
    s32 direction = player->heading >> 12;
    struct Resource373Mover *pushed;
    struct Resource373Mover *obstacle;

    step_from(player, RESOURCE_373_DIRECTION_STEPS[direction], &probe);
    pushed = Func_02000176(&probe, player);
    if (pushed == 0) {
        return;
    }

    step_from(pushed, RESOURCE_373_DIRECTION_STEPS[direction], &probe);
    obstacle = Func_020001a2(&probe, pushed);
    if (obstacle != 0 && (obstacle->flags59 & 1) != 0) {
        return;
    }

    /* The third probe drops one square in Y instead of stepping sideways. */
    probe.x = pushed->x;
    probe.y = pushed->y + 0x00100000;   /* 0x80 << 13 */
    probe.z = pushed->z;
    obstacle = Func_020001ce(&probe, pushed);
    if (obstacle != 0 && (obstacle->flags59 & 1) != 0) {
        return;
    }

    pushed->state22 = 2;

    step_from(pushed, RESOURCE_373_DIRECTION_STEPS[direction], &probe);
    if (Func_020060e4(pushed, &probe) > 0) {
        return;
    }
    if (pushed->blocked62 != 0) {
        return;
    }

    Func_020060ac(player, 8);
    Func_0200604c(15);
    Func_02006312(185);

    pushed->field30 = 0x3333;
    pushed->field34 = 0x3333;
    Func_020060f2(pushed, probe.x, probe.y, probe.z);

    player->field30 = 0x3333;
    player->field34 = 0x3333;
    Func_02006102(player, probe.x, probe.y, probe.z);

    Func_02006110(pushed);
    Func_02006334();

    pushed->x = probe.x;
    pushed->z = probe.z;
    pushed->velocityX = 0;
    pushed->velocityZ = 0;

    player->accelerationX = (s32)0x80000000;
    player->accelerationZ = (s32)0x80000000;
    player->velocityX = 0;
    player->velocityZ = 0;
    player->x = (s32)RESOURCE_373_WHOLE(player->x) << 16;
    player->z = (s32)RESOURCE_373_WHOLE(player->z) << 16;

    Func_02006118(player, 1);
}
