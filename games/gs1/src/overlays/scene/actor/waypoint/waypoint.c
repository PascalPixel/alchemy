#include "types.h"
#include "spawn_configured_effect.h"
#include "spawn_configured_effect_body.inc"
#include "staged_actor_movement.h"
#include "run_staged_actor_movement_and_redraw_body.inc"
#include "select_overlay_data_by_runtime_selector.h"
#include "select_overlay_data_by_runtime_selector_body.inc"
#include "find_clear_actor_position.h"
#include "find_clear_actor_position_body.inc"
#include "staged_actor.h"

/* overlays/scene/actor/waypoint/actor_motion.c */
#define Actor_TryRunSlotZeroMoveStep Func_02000cd0
#define Actor_BobActorZeroWhenAheadClear Func_02001078
#define Actor_RunActorTwelveThreeWaypointMotion Func_02001328
#define Actor_RunActorFourteenFourWaypointMotion Func_02001880
#define Actor_BobActorZeroWhenTargetClear Func_02001ac8
s32 *Func_02003a98_a(s32 a);
void Func_02003a98_b(void);
s32 Func_02003a40(s32 *p, s16 *q);
void Func_02003a20(s32 *p, s32 b);
void Func_020039ee(s32 a);
void Func_02003bbc(s32 a);
void Func_02003a34(s32 *p, s32 b);
void Func_02003a8e(s32 *p, s32 b);
s32 Func_02003b14(s32 a, s32 b, s32 c);
void Func_02003a6c(s32 *p, s32 b);
void Func_02003aac(s32 *p, s32 b);
void Func_02003b04(void);
s32 *Func_02003e40(s32 a);
s32 Func_02001d82(s32 *actor);
void Func_02003e5e(void);
void Func_02003ec2_a(s32 a, s32 b);
void Func_02003ec2_b(void);
void Func_02003dc6(s32 a);
void Func_02003dd8(s32 a);
void Func_02003dee(s32 a);
u8 *Func_020040ec(s32);
void Func_020040dc(void);
void Func_020021e8(s32, s32);
void Func_020020e2(s32, s32, s32, s32);
void Func_02001e58(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_020041cc(s32, s32);
void Func_0200419e(s32, s32, s32);
void Func_02004124(s32);
void Func_02004194(s32, s32);
void Func_020041e6(s32, s32);
void Func_0200413e(s32);
void Func_02002142(s32, s32, s32, s32);
void Func_020041d0(s32, s32, s32);
void Func_0200415e(s32);
void Func_02002160(s32, s32, s32, s32);
void Func_020041ee(s32, s32, s32);
void Func_0200417c(s32);
void Func_0200217e(s32, s32, s32, s32);
void Func_0200420c(s32, s32, s32);
void Func_0200419a(s32);
void Func_02004198(s32);
void Func_02004202(s32, s32, s32);
void Func_020041be(void);
u8 *Func_02004644(s32);
void Func_02004632(void);
void Func_0200273e(s32, s32);
void Func_02002638(s32, s32, s32, s32);
void Func_0200464a(s32);
void Func_020023b4(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_02004728(s32, s32);
void Func_020046fa_a(s32, s32, s32);
void Func_02004680(s32);
void Func_020046f0(s32, s32);
void Func_02004742(s32, s32);
void Func_0200469c(s32);
void Func_020026a2(s32, s32, s32, s32);
void Func_02004730_a(s32, s32, s32);
void Func_020046be(s32);
void Func_020026c0(s32, s32, s32, s32);
void Func_0200474e(s32, s32, s32);
void Func_020046dc(s32);
void Func_020026de(s32, s32, s32, s32);
void Func_0200476c(s32, s32, s32);
void Func_020046fa_b(s32);
void Func_020026fc(s32, s32, s32, s32);
void Func_0200478a(s32, s32, s32);
void Func_02004718(s32);
void Func_020047d8(s32, s32);
void Func_02004782(s32, s32, s32);
void Func_02004730_b(s32);
void Func_0200472e(s32);
void Func_02004798(s32, s32, s32);
void Func_02004754(void);
u8 *Func_02004890(s32);
u8 *Func_020048a0(s32);
u8 *Func_020048b0(s32);
u8 *Func_020048ba(s32);
s32 Func_020027d8(s32 *);
void Func_020048b4(void);
void Func_02004918_a(s32, s32);
void Func_02004918_b(void);
void Func_0200481c(s32);
void Func_0200482e(s32);
void Func_02004844(s32);

/* Two veneers each serve two imports: 0x020046fa is the three-argument setter
 * at site 8 and the one-argument wait at site 21, and 0x02004730 is the
 * three-argument step at site 14 and the one-argument wait at site 27. */

/* Four sites of the record accessor, so four names. */

/* 0x02004918 serves two imports: the two-argument mode select and the
 * zero-argument bracket close. */

s32 Actor_TryRunSlotZeroMoveStep(s16 *arg)
{
    s32 *p = Func_02003a98_a(0);
    u8 *f = (u8 *)p + 0x55;
    s32 saved = *f;

    s32 r = Func_02003a40(p, arg);

    if (r == 0) {
        s32 m;

        Func_02003a98_b();
        Func_02003a20(p, 6);
        Func_020039ee(6);
        Func_02003bbc(152);
        Func_02003a34(p, 7);
        p[12] = 0x30000;
        p[13] = 0x20000;
        p[10] = 0x40000;
        m = 0x7e;
        m &= *f;
        *f = m;
        Func_02003a8e(p, 0);
        Func_02003b14(0, arg[1], arg[5]);
        Func_02003a6c(p, 6);
        Func_02003aac(p, 1);
        *f = saved;
        Func_02003b04();
        return 1;
    }
    return 0;
}

void Actor_BobActorZeroWhenAheadClear(void)
{
    s32 pos[3];
    s32 *actor = Func_02003e40(0);
    u8 *fp = (u8 *)actor + 0x55;
    s32 saved = *fp;

    pos[0] = (actor[2] & 0xfff00000) + 0x80000;
    pos[1] = actor[3];
    pos[2] = (actor[4] & 0xfff00000) + 0x280000;
    if (Func_02001d82(pos)!= 0) {
        Func_02003e5e();
        *fp = 0;
        Func_02003ec2_a(9, 7);
        actor[3] += -0x10000;
        actor[5] += -0x10000;
        Func_02003dc6(2);
        actor[3] += -0x10000;
        actor[5] += -0x10000;
        Func_02003dd8(10);
        actor[3] += 0x10000;
        actor[5] += 0x10000;
        Func_02003dee(4);
        actor[3] += 0x10000;
        actor[5] += 0x10000;
        *fp = saved;
        Func_02003ec2_b();
    }
}

void Actor_RunActorTwelveThreeWaypointMotion(void)
{
    u8 *slot;

    slot = Func_020040ec(12);

    /* r0 still holds the record returned above. */
    Func_020040dc();

    Func_020021e8(12, 1);
    Func_020020e2(12, 536, 344, 0x70000);       /* 134 << 2, 172 << 1, 224 << 11 */

    Func_02001e58(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x100000,   /* 128 << 13 */
                  0, 0, 0, 1, 0);

    Func_020041cc(12, 1);
    Func_0200419e(12, 0, 0);
    Func_02004124(20);
    Func_02004194(12, 2);
    Func_020041e6(12, 258);                     /* 129 << 1 */
    Func_0200413e(60);

    /* Three waypoints, each at height 0x30000 (192 << 10); the X literals are
     * 146 << 2, 158 << 2 and 170 << 2 and the Z is the same 172 << 1. */
    Func_02002142(12, 584, 344, 0x30000);
    Func_020041d0(0, 12, 0);
    Func_0200415e(6);

    Func_02002160(12, 632, 344, 0x30000);
    Func_020041ee(0, 12, 0);
    Func_0200417c(6);

    Func_0200217e(12, 680, 344, 0x30000);
    Func_0200420c(0, 12, 0);
    Func_0200419a(6);

    Func_02004198(0x302);
    Func_02004202(15, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_020041be();
}

void Actor_RunActorFourteenFourWaypointMotion(void)
{
    u8 *slot;

    slot = Func_02004644(14);

    /* r0 still holds the record returned above. */
    Func_02004632();

    Func_0200273e(14, 1);
    Func_02002638(14, 392, 504, 0x60000);       /* 196 << 1, 252 << 1, 192 << 11 */
    Func_0200464a(10);

    Func_020023b4(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_02004728(14, 1);
    Func_020046fa_a(14, 0, 0);
    Func_02004680(20);
    Func_020046f0(14, 2);
    Func_02004742(14, 258);                     /* 129 << 1 */
    Func_0200469c(60);

    /* Four waypoints; Z is 132 << 2 and the height 192 << 10 throughout. */
    Func_020026a2(14, 360, 528, 0x30000);
    Func_02004730_a(0, 14, 0);
    Func_020046be(6);

    Func_020026c0(14, 328, 528, 0x30000);
    Func_0200474e(0, 14, 0);
    Func_020046dc(6);

    Func_020026de(14, 288, 528, 0x30000);
    Func_0200476c(0, 14, 0);
    Func_020046fa_b(6);

    Func_020026fc(14, 256, 528, 0x30000);
    Func_0200478a(0, 14, 0);
    Func_02004718(6);

    Func_020047d8(0, 1);
    Func_02004782(14, 0, 0);
    Func_02004730_b(30);
    Func_0200472e(0x306);
    Func_02004798(17, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_02004754();
}

void Actor_BobActorZeroWhenTargetClear(void)
{
    u8 *record;
    u8 *mode;
    u8 saved;
    s32 target[3];

    record = Func_02004890(0);
    mode = record + 85;
    saved = *mode;

    target[0] = *(s32 *)(Func_020048a0(0) + 8) + (s32)0xffe00000;
    target[1] = *(s32 *)(Func_020048b0(0) + 12);
    target[2] = *(s32 *)(Func_020048ba(0) + 16);

    if (Func_020027d8(target)!= 0) {
        /* r0 still holds the nonzero result of the test above. */
        Func_020048b4();

        *mode = 0;
        Func_02004918_a(11, 7);

        *(s32 *)(record + 12) += (s32)0xffff0000;
        *(s32 *)(record + 20) += (s32)0xffff0000;
        Func_0200481c(2);

        *(s32 *)(record + 12) += (s32)0xffff0000;
        *(s32 *)(record + 20) += (s32)0xffff0000;
        Func_0200482e(10);

        *(s32 *)(record + 12) += 0x10000;
        *(s32 *)(record + 20) += 0x10000;
        Func_02004844(4);

        *(s32 *)(record + 12) += 0x10000;
        *(s32 *)(record + 20) += 0x10000;

        *mode = saved;
        Func_02004918_b();
    }
}
#undef Actor_TryRunSlotZeroMoveStep
#undef Actor_BobActorZeroWhenAheadClear
#undef Actor_RunActorTwelveThreeWaypointMotion
#undef Actor_RunActorFourteenFourWaypointMotion
#undef Actor_BobActorZeroWhenTargetClear

/* overlays/scene/actor/waypoint/actor_position.c */
#define Actor_PassOffsetPointOfActorZero Func_0200103c
#define Actor_MarkActorThirteenTileAndPark Func_02001b84
s32 *Func_02003e00(s32 a);
void Func_02001d38(s32 *p);
void Func_02004932();
s32 *Func_02004950();
s32 *Func_02004958();
s32 Func_02001dfa();
s32 Func_02001e0e();
s32 Func_02001e20();
s32 Func_02001e32();
s32 Func_02001e44();
s32 *Func_020049ca();
void Func_020049cc();

/* The five tile-painting calls take (layer, x, z, width, height, value) and
 * all reach the same routine, but each keeps its own call word: the encoding
 * is per site, so they must not be collapsed onto one alias. */

void Actor_PassOffsetPointOfActorZero(void)
{
    s32 v[3];
    s32 *p = Func_02003e00(0);

    v[0] = (p[2] & 0xfff00000) + 0x80000;
    v[1] = p[3];
    v[2] = (p[4] & 0xfff00000) + 0xffe80000;
    Func_02001d38(v);
}

/*
 * Parking step for actor slot 13 in resource_39f. It marks the actor's own
 * tile 0xff, clears the four orthogonally adjacent tiles, and once the actor
 * stands on tile (45, 6) clears the record's mode byte and writes -2.0 in
 * 16.16 into the words at +12 and +20.
 *
 * The 176-byte owner at 0x02001b84 runs past its code to include an alignment
 * halfword and the pool word 0xfffe0000 at 0x02001c30.
 */
void Actor_MarkActorThirteenTileAndPark(void)
{
    s32 x;
    s32 z;

    /* No argument register is written before this branch. */
    Func_02004932();

    x = Func_02004950(13)[2] >> 20;
    z = Func_02004958(13)[4] >> 20;

    Func_02001dfa(2, x, z, 1, 1, 0xff);
    Func_02001e0e(2, x + 1, z, 1, 1, 0);
    Func_02001e20(2, x - 1, z, 1, 1, 0);
    Func_02001e32(2, x, z + 1, 1, 1, 0);
    Func_02001e44(2, x, z - 1, 1, 1, 0);

    if (x == 45 && z == 6) {
        u8 *record = (u8 *)Func_020049ca(13);

        record[85] = 0;
        *(s32 *)(record + 20) = (s32)0xfffe0000;
        *(s32 *)(record + 12) = (s32)0xfffe0000;
    }

    /* Common exit; no argument registers are set. */
    Func_020049cc();
}
#undef Actor_PassOffsetPointOfActorZero
#undef Actor_MarkActorThirteenTileAndPark

/* overlays/scene/actor/waypoint/configurable_effect_spawn.c */
#define ConfigurableEffectDescriptors Data_0200b058
#define GetPrimaryEffectRecord Func_020038c6
#define SpawnConfiguredEffectObject Func_02003864
#define SetConfiguredEffectVariant Func_0200386e
#define ApplyConfiguredEffectDescriptor Func_02003888
#define SetConfiguredEffectLink Func_020039f2
#define ComputeConfiguredEffectRateFromDelta Func_0200390c
#define ComputeConfiguredEffectRateFromOffset Func_02003924
#define ComputeConfiguredEffectSecondRate Func_02003932
#define SetConfiguredEffectCallbackMode Func_02003988
#define ApplyConfiguredEffectCallbackArgument Func_02003998
#define Effect_SpawnConfigured Func_02000ae8


void Effect_SpawnConfigured(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct Options *options)
{
}
#undef ConfigurableEffectDescriptors
#undef GetPrimaryEffectRecord
#undef SpawnConfiguredEffectObject
#undef SetConfiguredEffectVariant
#undef ApplyConfiguredEffectDescriptor
#undef SetConfiguredEffectLink
#undef ComputeConfiguredEffectRateFromDelta
#undef ComputeConfiguredEffectRateFromOffset
#undef ComputeConfiguredEffectSecondRate
#undef SetConfiguredEffectCallbackMode
#undef ApplyConfiguredEffectCallbackArgument
#undef Effect_SpawnConfigured

/* overlays/scene/actor/waypoint/move_and_redraw.c */
#define Data_0200e1e8 Data_0200af50
#define Data_0200e190 Data_0200aef8
#define Func_02006610 Func_020033e8
#define Func_0200661c Func_020033f4
#define Func_020066b4 Func_0200348c
#define Func_02006714 Func_020034cc
#define Func_0200668a Func_02003472
#define Func_0200672c Func_020034e4_a
#define Func_020066ea Func_020034c2
#define Func_020066c4 Func_020034ac
#define RefreshStagedActor Func_020034e4_b
#define Func_02006614 Func_02003444
#define Func_0200661e Func_0200344e
#define Func_0200687c Func_020035e4
#define Func_02006658 Func_02003478
#define Func_02006776 Func_0200352e
#define Func_0200678e Func_02003546
#define Func_02006740 Func_02003518
#define Func_020067a4 Func_0200355c
#define Func_020067bc Func_02003574
#define Func_020067d4 Func_0200358c
#define Func_0200677a Func_02003552
#define Func_020068f4 Func_0200365c
#define Func_020068fa Func_02003662
#define Func_020066b8 Func_020034e8
#define Func_02006752 Func_02003562
#define Func_020067ae Func_020035be
#define Func_020069d0 Func_02003738

#define STAGED_ACTOR_MOVEMENT_UNTYPED_LOCATION_CALL


void Func_02000608(
    StagedActorMovementRequest request)
{
}
#undef Data_0200e1e8
#undef Data_0200e190
#undef Func_02006610
#undef Func_0200661c
#undef Func_020066b4
#undef Func_02006714
#undef Func_0200668a
#undef Func_0200672c
#undef Func_020066ea
#undef Func_020066c4
#undef RefreshStagedActor
#undef Func_02006614
#undef Func_0200661e
#undef Func_0200687c
#undef Func_02006658
#undef Func_02006776
#undef Func_0200678e
#undef Func_02006740
#undef Func_020067a4
#undef Func_020067bc
#undef Func_020067d4
#undef Func_0200677a
#undef Func_020068f4
#undef Func_020068fa
#undef Func_020066b8
#undef Func_02006752
#undef Func_020067ae
#undef Func_020069d0
#undef STAGED_ACTOR_MOVEMENT_UNTYPED_LOCATION_CALL

/* overlays/scene/actor/waypoint/orbiting_effect.c */
#define GetOrbitingSceneObject Func_020059d4
#define NormalizeOrbitingSceneObject Func_020059a6
#define IsGameFlagSet Func_020059ec
#define AllocateEffectTransfer Func_0200599c
#define LoadEffectResource Func_02005a1c
#define ConfigurePaletteTransfer Func_020059c4
#define ReleaseEffectTransfer Func_020059c2
#define UpdateOrbitingSceneObject Value_0200aba5
#define InitializeOrbitingEffect Func_02002c0c

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 palette;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;

extern u8 Value_0200aba5;

s32 Func_020058aa(s32);
s32 Func_020058c0(s32);
s32 Func_020058da(s32);
s32 Func_020058d8(void);
s32 Func_020058de(void);
OrbitingSceneObject *Func_020059d4(void);
void Func_020059a6(OrbitingSceneObject *, s32);
s32 Func_020059ec(s32);
u8 *Func_0200599c(s32, s32);
void Func_02005a1c(s32);
s32 Func_020059c4(u8, s32, u8 *);
void Func_020059c2(s32);

s32 Func_02002ba4(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Func_020058aa(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Func_020058c0(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Func_020058da(p[12] + 0x8000) / 8;
    a = Func_020058d8();
    b = Func_020058de();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}

void InitializeOrbitingEffect(void)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = GetOrbitingSceneObject();
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    NormalizeOrbitingSceneObject(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (IsGameFlagSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = AllocateEffectTransfer(17, 0x608);
    LoadEffectResource(181);
    transfer += 0x400;
    ConfigurePaletteTransfer(sprite->palette, 128, transfer);
    ReleaseEffectTransfer(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)&UpdateOrbitingSceneObject;
    actor->state = zero;
}
#undef GetOrbitingSceneObject
#undef NormalizeOrbitingSceneObject
#undef IsGameFlagSet
#undef AllocateEffectTransfer
#undef LoadEffectResource
#undef ConfigurePaletteTransfer
#undef ReleaseEffectTransfer
#undef UpdateOrbitingSceneObject
#undef InitializeOrbitingEffect

/* overlays/scene/actor/waypoint/overlay_object.c */
#define OvObj_PrepareSpawnedObject Func_020009f4
#define OvObj_SpawnConfiguredWithMode15 Func_02000a4c
#define OvObj_IntegrateVelocities Func_02000ab0
#define OvObj_ApplyField100 Func_02000d6c
#define OvObj_ApplyZero Func_02000d80
u8 *Func_0200373a(s32 kind, s32 x, s32 y, s32 z);
void Func_02003788(u8 *obj, s32 mode);
void Func_02003860(u8 *obj, s32 mode);
void Func_020037a0(u8 *obj, s32 mode);
u8 *Func_02003792(s32 kind, s32 x, s32 y, s32 z);
void Func_020037e4(u8 *obj, s32 mode);
void Func_020038bc(u8 *obj, s32 mode);
void Func_02003ba4(s32 a, s32 b);
void Func_02003ae2(s32 a, s32 b);

/*
 * Spawns an overlay object and puts it into its configured state --
 * resource_39f.
 */

/*
 * The four callees are declared at the in-overlay entry points that are
 * called directly, not at their veneer addresses.
 */

/*
 * Spawn an object, clear the low bits of its owner record, and leave it in
 * mode 15.  The three callees are declared at their in-overlay entry points,
 * not at veneers.  Each read-modify-write takes its mask from a local that is
 * initialised first and ANDed with the loaded byte afterwards, which is what
 * puts the mask in r3 and the byte in r2.  The +0x23 mask stays byte-wide as
 * 0xfe rather than ~1, which would widen it to a negated 2.
 */

u8 *OvObj_PrepareSpawnedObject(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Func_0200373a(kind, x, y, z);

    if (obj != 0) {
        u8 *rec = *(u8 **)(obj + 80);
        /*
         * The mask is a local initialised first and ANDed with the loaded
         * byte afterwards; folding the clear into one expression swaps the
         * two registers.
         */
        u32 mask = ~12;
        mask = mask & rec[9];
        rec[9] = (u8)mask;
        obj[85] = 0;
        obj[89] = 8;
        Func_02003788(obj, 0);
        Func_02003860(obj, 14);
        Func_020037a0(obj, 1);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
}

u8 *OvObj_SpawnConfiguredWithMode15(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Func_02003792(kind, x, y, z);

    if (obj != 0) {
        u8 *rec = *(u8 **)(obj + 80);
        u32 mask = ~12;
        u32 m;
        mask = mask & rec[9];
        rec[9] = (u8)(mask | 4);
        obj[85] = 0;
        obj[89] = 8;
        Func_020037e4(obj, 0);
        Func_020038bc(obj, 15);
        m = 0xfe;
        m = m & obj[35];
        obj[35] = (u8)(m | 2);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
}

void OvObj_IntegrateVelocities(void *arg0)
{
    u8 *a = arg0;

    *(volatile s32 *)(a + 0x08) += *(s32 *)(a + 0x44);
    *(volatile s32 *)(a + 0x0C) += *(s32 *)(a + 0x48);
    *(volatile s32 *)(a + 0x10) += *(s32 *)(a + 0x4C);
    *(volatile s32 *)(a + 0x18) += *(s32 *)(a + 0x30);
    *(volatile s32 *)(a + 0x1C) += *(s32 *)(a + 0x34);
    *(volatile u16 *)(*(u8 **)(a + 0x50) + 0x1E) += *(u16 *)(a + 0x64);
}

s32 OvObj_ApplyField100(s32 a)
{
    Func_02003ba4(a, *(s16 *)(a + 100));
    return 0;
}

s32 OvObj_ApplyZero(s32 a)
{
    Func_02003ae2(a, 0);
    return 0;
}
#undef OvObj_PrepareSpawnedObject
#undef OvObj_SpawnConfiguredWithMode15
#undef OvObj_IntegrateVelocities
#undef OvObj_ApplyField100
#undef OvObj_ApplyZero

/* overlays/scene/actor/waypoint/scene_data_head.c */
#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000044
#define SecondaryRuntimeSelector Value_00000045
#define TertiaryRuntimeSelector Value_00000046
#define PrimaryOverlayData Data_0200b0f4
#define SecondaryOverlayData Data_0200b1e4
#define TertiaryOverlayData Data_0200b334
#define DefaultOverlayData Data_0200b4b4
#define SceneData_SelectByRuntimeSelector Func_02000ee0
#define SceneData_ReturnZero Func_02000f34
#define SceneData_GetTableB5bc Func_02000f38


s32 SceneData_SelectByRuntimeSelector(void)
{
}

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* Complete eight-byte literal-address getter, including its sole pool word. */
u8 *SceneData_GetTableB5bc(void)
{
    return (u8 *)0x0200b5bc;
}
#undef RuntimeSelectorTable
#undef PrimaryRuntimeSelector
#undef SecondaryRuntimeSelector
#undef TertiaryRuntimeSelector
#undef PrimaryOverlayData
#undef SecondaryOverlayData
#undef TertiaryOverlayData
#undef DefaultOverlayData
#undef SceneData_SelectByRuntimeSelector
#undef SceneData_ReturnZero
#undef SceneData_GetTableB5bc

/* overlays/scene/actor/waypoint/scene_primary_script.c */
#define FieldScene_RunScene39f_02000d90 Func_02000d90
#define Scene_RunScriptedSteps0And17E6 Func_0200112c
#define Scene_RunActor10WaypointSequence Func_02001150
#define Scene_RunActorElevenPresentationBeat Func_02001244
#define Scene_RunStepFD4WithActor181 Func_02001420
#define Scene_RunProbedActorEightOrTenScene Func_02001454
#define Scene_RunActorThirteenPresentationBeat Func_020016f0
#define FieldScene_RunScene39f_02001818 Func_02001818
#define Scene_RunSupplementalSequenceOne Func_02001c34
#define Scene_RunScene39fSequenceA Func_02001d04
#define Scene_RunSlot16WaypointSequence Func_02001de0
#define Scene_RunActor17CameraSequence Func_02001ef0
#define FieldScene_RunScene39f_02002004 Func_02002004
#define Scene_RunActorEighteenEffectSequence Func_02002078
#define FieldScene_RunScene39f_020021b0 Func_020021b0
/* The probe record is passed on by value: four words in r0-r3 and the last two
 * copied to the outgoing stack area by an `ldmia`/`stmia` pair. */
struct Resource39fProbe {
    s32 word[6];
};

extern u8 Data_03001ebc[];

void Func_02003b36();
void Func_02003b5e();
s32 Func_02003b62();
void Func_02003b82();
void Func_02003bb8();
void Func_02003bec();
void Func_02003bee();
void Func_02003c78();
void Func_02003ed4(void);
void Func_02003f34(s32 a, s32 b);
void Func_02003ebc(s32 a, s32 b);
void Func_02003ef0(void);
u8 *Func_02003f14(s32);
void Func_02003f02(void);
void Func_02001c7a(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_02003fe6(void);
void Func_0200200e(s32, s32);
void Func_02001f04(s32, s32, s32, s32);
void Func_02003fee(s32, s32);
void Func_02003fc0(s32, s32, s32);
void Func_02003f46(s32);
void Func_02003fb6_b(s32, s32);
void Func_02003fb6_c(s32);
void Func_0200400a(s32, s32);
void Func_02003f62(s32);
void Func_02001f62(s32, s32, s32, s32);
void Func_02003ff0(s32, s32, s32);
void Func_02003f7e(s32);
void Func_02001f7e(s32, s32, s32, s32);
void Func_0200400c(s32, s32, s32);
void Func_02003f9a(s32);
void Func_02001f9a(s32, s32, s32, s32);
void Func_02004028(s32, s32, s32);
void Func_02003fb6_a(s32);
void Func_02004020(s32, s32, s32);
void Func_0200402a(s32, s32, s32);
u8 *Func_02004008();
void Func_02003ff6();
void Func_02002102();
void Func_02001ffc();
void Func_02001d72();
void Func_020040e6();
void Func_020040b8_a();
void Func_020040b8_b();
void Func_0200403e();
void Func_020040ae();
void Func_020040f8();
void Func_0200416e();
void Func_0200405c();
u8 *Func_02004082_a();
u8 *Func_0200408c();
void Func_02002070();
void Func_02004082_b();
void Func_02004080();
void Func_020040ea();
void Func_02004184();
void Func_020041c8_a(s32 a);
void Func_020041c8_b(s32 a);
s32 Func_02004222(s32 a, s32 b, s32 c);
void Func_020042e0(s32 a, s32 b);
s32 Func_020041f8(s32 a, s32 b);
void Func_020041f4(void);
void Func_020041fe(void);
s32 Func_020018d6(struct Resource39fProbe *);
void Func_02001a82(struct Resource39fProbe);
void Func_020041e8(s32, s32, s32, s32, s32, s32);
void Func_02004246_a(s32);
void Func_020042b6(s32, s32);
void Func_020042aa(s32, s32, s32);
void Func_02004268(s32);
void Func_020042d0(s32, s32);
void Func_0200438e(s32);
u8 *Func_0200429c(s32);
void Func_02004246_b(s32, s32, s32, s32, s32, s32);
s32 Func_02001752(s32, s32, s32, s32, s32, s32);
void Func_020042be(void);
u8 *Func_020044b4(s32);
void Func_020044a2(void);
void Func_020025ae(s32, s32);
void Func_020024a6(s32, s32, s32, s32);
void Func_020044b8(s32);
void Func_02002222(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_02004596(s32, s32);
void Func_02004568(s32, s32, s32);
void Func_020044ee(s32);
void Func_0200455e(s32, s32);
void Func_020045b0(s32, s32);
void Func_02004506(s32);
void Func_0200250a(s32, s32, s32, s32);
void Func_02004598(s32, s32, s32);
void Func_02004526(s32);
void Func_02002528(s32, s32, s32, s32);
void Func_020045b6(s32, s32, s32);
void Func_02004546(s32);
void Func_0200254a(s32, s32, s32, s32);
void Func_020045d8(s32, s32, s32);
void Func_02004566(s32);
void Func_02002568(s32, s32, s32, s32);
void Func_020045f6(s32, s32, s32);
void Func_02004584(s32);
void Func_020045e6(s32, s32, s32);
void Func_0200458e(s32);
void Func_020045f8(s32, s32, s32);
void Func_020045b4(void);
void Func_020025c4();
void Func_02002658();
void Func_020026cc();
void Func_020045b0_a();
void Func_020045c0();
void Func_020045d6();
void Func_020045f4();
void Func_020045f6_a();
s32 Func_0200460a();
void Func_0200461e();
void Func_02004662();
void Func_0200466c();
s32 Func_02001eb0();
void Func_02001ec2();
void Func_02001ed2();
s32 Func_02001ee2();
s32 Func_02001ef4();
void Func_02001f30();
void Func_02002834();
void Func_02002abc();
void Func_02002b32();
void Func_02002bc4();
void Func_020049e4();
s32 Func_02004a02();
s32 Func_02004a0a();
void Func_02004a6c();
s32 Func_02004a72();
s32 Func_02004a80();
void Func_02004a9c();
void Func_02004ab8();
s32 Func_02004ac8();
void Func_02004ace();
void Func_02004b00();
void Func_02004b1e();
void Func_02004b42();
u8 *Func_02004b44();
void Func_02004b70();
void Func_02004b7a();
void Func_02004ba8();
void Func_02004bba();
void Func_02004c30();
void Func_02004c3c();
s32 Func_02004b44_a();
s32 Func_02004b44_b();
void Func_02004b70_a();
void Func_02004b70_b();
u8 *Func_02004ba4(s32);
void Func_02004b92(void);
void Func_02002c9e(s32, s32);
void Func_02002b96(s32, s32, s32, s32);
void Func_02004ba8_a(s32);
void Func_02002912(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_02004c86(s32, s32);
void Func_02004c58(s32, s32, s32);
void Func_02004bde(s32);
void Func_02004c4e(s32, s32);
void Func_02004ca0(s32, s32);
void Func_02004bf8(s32);
void Func_02002bfc(s32, s32, s32, s32);
void Func_02004c8c(s32, s32, s32);
void Func_02004c1c(s32);
void Func_02002c1c(s32, s32, s32, s32);
void Func_02004caa(s32, s32, s32);
void Func_02004c38(s32);
void Func_02002c38(s32, s32, s32, s32);
void Func_02004cc6(s32, s32, s32);
void Func_02004c54(s32);
void Func_02004d14(s32, s32);
void Func_02004cbe(s32, s32, s32);
void Func_02004c6c_a(s32);
void Func_02004c6c_b(s32);
void Func_02004cd6(s32, s32, s32);
void Func_02004c92(void);
u8 *Func_02004cb4(s32);
void Func_02004ca4(void);
void Func_02002db0(s32, s32);
void Func_02002ca8(s32, s32, s32, s32);
void Func_02004cba(s32);
void Func_02002a24(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_02004d98_a(s32, s32);
void Func_02004d6a(s32, s32, s32);
void Func_02004cf0(s32);
void Func_02004d60(s32, s32);
void Func_02004db2(s32, s32);
void Func_02004d08(s32);
void Func_02002d0a(s32, s32, s32, s32);
void Func_02004d98_b(s32, s32, s32);
void Func_02004d28(s32);
void Func_02002d2c(s32, s32, s32, s32);
void Func_02004dba(s32, s32, s32);
void Func_02004d48(s32);
void Func_02002d4a(s32, s32, s32, s32);
void Func_02004dd8(s32, s32, s32);
void Func_02004d66(s32);
void Func_02004e26(s32, s32);
void Func_02004dd0(s32, s32, s32);
void Func_02004d7e(s32);
void Func_02004d7c(s32);
void Func_02004de6(s32, s32, s32);
void Func_02004da2(void);
void Func_02002dc4();
void Func_02002e52();
void Func_02002eb8();
void Func_02004daa();
void Func_02004dac();
void Func_02004dee();
void Func_02004df0();
s32 Func_02004e04();
void Func_02004e18();
void Func_02004e5c();
void Func_02004e66();
void Func_02004e86();
u8 *Func_02004e40(s32);
void Func_02004e30(void);
void Func_02002f3c(s32, s32);
void Func_02002e38(s32, s32, s32, s32);
void Func_02004e4a(s32);
void Func_02002bb4(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_02004f28_a(s32, s32);
void Func_02004efa(s32, s32, s32);
void Func_02004e80(s32);
void Func_02004ef0(s32, s32);
void Func_02004f42(s32, s32);
void Func_02004e98(s32);
void Func_02002e9a(s32, s32, s32, s32);
void Func_02004f28_b(s32, s32, s32);
void Func_02004eb8(s32);
void Func_02002ebc(s32, s32, s32, s32);
void Func_02004f4a(s32, s32, s32);
void Func_02004ed8(s32);
void Func_02002edc(s32, s32, s32, s32);
void Func_02004f6a(s32, s32, s32);
void Func_02004ef8(s32);
void Func_02002efa(s32, s32, s32, s32);
void Func_02004f88(s32, s32, s32);
void Func_02004f16(s32);
void Func_02004fd6(s32, s32);
void Func_02004f80(s32, s32, s32);
void Func_02004f2e(s32);
void Func_02004f2c(s32);
void Func_02004f48(void);
void Func_02002cf0();
void Func_02002f74();
void Func_02002fce();
void Func_02002ff0();
void Func_0200300e();
void Func_0200307c();
void Func_02004f62();
s32 Func_02004f74();
void Func_02004f86();
void Func_02004fc0();
void Func_02004fec();
void Func_0200500c();
void Func_0200502a();
void Func_02005032();
void Func_02005040();
void Func_02005042();
void Func_02005048();
void Func_0200505c_a();
void Func_0200505c_b();
void Func_0200506a();
void Func_0200507e();
void Func_02005082();
void Func_02005094();
void Func_0200509c();
void Func_020050ea();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* 0x02003fb6 serves the two-argument presentation select and, twice later,
 * one-argument imports; the `_b` alias carries the two-argument one. */

/* An EWRAM global, not overlay data: the module is linked above this address,
 * so it cannot be in-image. */

/* Returns a slot record.  Its fields are named by position and not verified:
 * +8/+12/+16 read as the position and +10/+18 as signed halfwords. */

/* Four register arguments plus four stack words. */

/* 0x02004246 serves two imports: the one-argument post at site 5 and the
 * six-argument message at site 12. */

/* 0x02004d98 serves two imports: the two-argument setter at site 7 and the
 * three-argument one at site 14. */

/* 0x02004f28 serves two imports: the two-argument setter at site 7 and the
 * three-argument step at site 14. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3_02001818(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_02000240[];
    u8 *Func_02004b4e();

    return f(a0);
}

static __inline__ void Call3_02001c34(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02000240[];
    u8 *Func_02004b4e();

    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_02000240[];
    u8 *Func_02004b4e();

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    extern u8 Data_02000240[];
    u8 *Func_02004b4e();

    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

static __inline__ void Call3_02001d04(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02000240[];
    s32 Func_02004b4e();

    f(a0, a1, a2);
}

static __inline__ void Call3_02002004(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3_020021b0(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FieldScene_RunScene39f_02000d90(s32 a0, s32 a1, s32 a2, s32 a3)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p10c;
    s32 p8;
    s32 p8b;
    s32 p9;
    s32 rec7;
    s32 record;

    p10 = a2;
    p9 = a3;
    p8 = a1;
    rec7 = Func_02003b62();
    Func_02003bec(a0, 1);
    Call3(Func_02003b82, a0, 0x30000, 0x18000);
    Func_02003c78(152);
    *(volatile s32 *)(rec7 + 40) = p9;
    *(volatile s32 *)(rec7 + 72) = 0x8000;
    *(volatile s32 *)(rec7 + 68) = 0;
    Func_02003b36(rec7, 0);
    Func_02003bb8(a0, p8, p10);
    p8b = ((s32)p8 << 16);
    p10b = ((s32)p10 << 16);
    Func_02003bee(a0, p8b, p10b);
    Func_02003b5e(rec7, 1);
    *(volatile s32 *)(rec7 + 72) = 0x10000;
    p10c = a0;
}

void Scene_RunScriptedSteps0And17E6(void)
{
    Func_02003ed4();
    Func_02003f34(0, 1);
    Func_02003ebc(6118, 1);
    Func_02003ef0();
}

void Scene_RunActor10WaypointSequence(void)
{
    u8 *slot;

    slot = Func_02003f14(10);

    /* r0 still holds the record returned above. */
    Func_02003f02();

    Func_0200200e(10, 1);
    Func_02001f04(10, 88, 120, 0x60000);        /* 192 << 11 */

    Func_02001c7a(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x180000,   /* 192 << 13 */
                  0, 0, 0, 1, 0);

    Func_02003fee(10, 1);
    Func_02003fc0(10, 0, 0);
    Func_02003f46(20);
    Func_02003fb6_b(10, 2);
    Func_0200400a(10, 258);                     /* 129 << 1 */
    Func_02003f62(60);

    /* Three waypoints, each at height 0x30000 (192 << 10). */
    Func_02001f62(10, 88, 152, 0x30000);
    Func_02003ff0(0, 10, 0);
    Func_02003f7e(10);

    Func_02001f7e(10, 120, 192, 0x30000);
    Func_0200400c(0, 10, 0);
    Func_02003f9a(10);

    Func_02001f9a(10, 120, 240, 0x30000);
    Func_02004028(0, 10, 0);
    Func_02003fb6_a(10);

    Func_02003fb6_c(768);                       /* 192 << 2 */
    Func_02004020(13, 0, 0);
    Func_0200402a(10, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_02003fe6();
}

/*
 * A full cutscene beat for slot 11: opens the slot, places it at (408, 456),
 * publishes an eight-argument piece, runs the presentation, then re-places the
 * slot on the party's current heading readings and sets the engine byte at
 * Data_02000240 + 0x22b to 3.  The 228-byte owner includes an alignment
 * halfword and its four pool words.
 */
void Scene_RunActorElevenPresentationBeat(void)
{
    extern s16 Data_02000240[];

    u8 *slot;
    s32 offset;

    slot = Func_02004008(11);

    /* Reads the record left in r0 by the call above; it must not be respelled
     * as a fresh fetch. */
    Func_02003ff6();

    Func_02002102(11, 0);
    Func_02001ffc(11, 408, 456, 0x60000);   /* 204 << 1, 228 << 1, 192 << 11 */

    Func_02001d72(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x180000,   /* 192 << 13 */
                  0, 0, 0, 1, 0);

    Func_020040e6(11, 1);
    Func_020040b8_a(11, 0, 0);
    Func_0200403e(30);
    Func_020040ae(11, 2);
    Func_020040f8(11, 0x103, 0);
    Func_0200416e(147);
    Func_0200405c(60);

    /* Two signed halfwords of slot 0, each read after its own fetch of the
     * record. */
    Func_02002070(11,
                  *(s16 *)(Func_02004082_a(0) + 10),
                  *(s16 *)(Func_0200408c(0) + 18),
                  0x40000);                          /* 128 << 11 */

    Func_02004082_b(10);
    Func_02004080(0x301);
    Func_020040ea(14, 0, 0);

    offset = 0x22b;
    ((u8 *)Data_02000240)[offset] = 3;

    Func_02004184(53, 0);

    /* Common exit; no argument registers are set. */
    Func_020040b8_b();
}

void Scene_RunStepFD4WithActor181(s32 a)
{
    Func_020041c8_a(a);
    Func_02004222(16, 0, 0);
    Func_020041c8_b(4052);
    Func_020042e0(181, 3);
    Func_020041f8(181, 0);
    Func_020041f4();
}

void Scene_RunProbedActorEightOrTenScene(void)
{
    struct Resource39fProbe probe;
    s32 fifth;
    s32 sixth;
    s32 height;
    s32 value;

    /* No argument register is written before this branch. */
    Func_020041fe();

    if (Func_020018d6(&probe) != 0) {
        Func_02001a82(probe);

        if (probe.word[1] == 8 && (probe.word[4] >> 20) == 23) {
            fifth = 35;
            sixth = 68;
            Func_020041e8(35, 67, 4, 1, fifth, sixth);
        } else if (probe.word[1] == 10 && (probe.word[2] >> 20) == 35) {
            /* Written here, not at the call: the reference keeps it in a
             * callee-saved register across the whole sequence. */
            value = 0;
            Func_02004246_a(0x311);
            Func_020042b6(10, 3);
            Func_020042aa(10, -16, 6);
            Func_02004268(30);
            Func_020042d0(10, 8);
            Func_0200438e(240);

            Func_0200429c(10)[35] = 2;

            fifth = 34;
            sixth = 30;
            Func_02004246_b(44, 30, 2, 4, fifth, sixth);
            height = 4;
            Func_02001752(2, 35, 30, 1, height, value);
        }
    }

    /* Common exit; no argument registers are set. */
    Func_020042be();
}

void Scene_RunActorThirteenPresentationBeat(void)
{
    u8 *slot;

    slot = Func_020044b4(13);

    /* r0 still holds the record returned above. */
    Func_020044a2();

    Func_020025ae(13, 1);
    Func_020024a6(13, 456, 104, 0x70000);       /* 228 << 1, 224 << 11 */
    Func_020044b8(10);

    Func_02002222(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_02004596(13, 1);
    Func_02004568(13, 0, 0);
    Func_020044ee(20);
    Func_0200455e(13, 2);
    Func_020045b0(13, 258);                     /* 129 << 1 */
    Func_02004506(60);

    Func_0200250a(13, 472, 136, 0x30000);       /* 236 << 1, 192 << 10 */
    Func_02004598(0, 13, 0);
    Func_02004526(6);

    Func_02002528(13, 504, 136, 0x33333);       /* 252 << 1, pooled height */
    Func_020045b6(0, 13, 0);
    Func_02004546(6);

    Func_0200254a(13, 552, 136, 0x38000);       /* 138 << 2, 224 << 10 */
    Func_020045d8(0, 13, 0);
    Func_02004566(6);

    Func_02002568(13, 584, 136, 0x38000);       /* 146 << 2 */
    Func_020045f6(0, 13, 0);
    Func_02004584(6);

    Func_020045e6(13, 0, 0);
    Func_0200458e(772);                         /* 193 << 2 */
    Func_020045f8(16, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_020045b4();
}

void FieldScene_RunScene39f_02001818(void)
{
    u32 i;
    s32 record;

    Func_020045c0();
    Func_020026cc(14, 1);
    Call4(Func_020025c4, 14, 0x1a8, 0x1e0, 0x79999);
    Func_020045d6(2);
    Func_02002658(14);
    Func_0200466c(14, 15);
    record = Func_0200460a(14);
    Func_020045b0_a(record, 0);
    Func_020045f6_a(30);
    Call1(Func_020045f4, 0x305);
    Call3_02001818(Func_02004662, 17, 0x1a80000, 0x1e00000);
    Func_0200461e();
}

void Scene_RunSupplementalSequenceOne(void)
{
    extern u8 Data_02000240[];
    u8 *Func_02004b4e();

    s32 one;
    s32 x;
    s32 y;
    u8 *record;

    Func_020049e4();
    record = Value1(Func_02004a02, 14);
    x = *(volatile s32 *)((s32)record + 8);
    record = Value1(Func_02004a0a, 14);
    y = *(volatile s32 *)((s32)record + 16);
    x >>= 20;
    y >>= 20;
    one = 1;
    Func_02001eb0(2, x, y, 1, one, 255);
    Call6(Func_02001ec2, 2, x + 1, y, 1, one, 0);
    Call6(Func_02001ed2, 2, x - 1, y, 1, one, 0);
    Func_02001ee2(2, x, y + 1, 1, one, 0);
    Func_02001ef4(2, x, y - 1, 1, one, 0);
    record = Value1(Func_02004a72, 14);
    if ((*(volatile s32 *)((s32)record + 16) >> 20) == 27) {
        record = Value1(Func_02004a80, 14);
        record[85] = 0;
        *(volatile s32 *)((s32)record + 20) = -0x20000;
        *(volatile s32 *)((s32)record + 12) = -0x20000;
        Call1(Func_02004a6c, 0x214);
        Call6(Func_02001f30, 2, 43, 23, 1, one, 255);
    }
    Func_02004a9c();
}

/*
 * Actor presentation beat for overlay resource_39f.  The twin at 0x02001d04
 * is the same beat for slot 15.
 */
void Scene_RunScene39fSequenceA(void)
{
    extern u8 Data_02000240[];
    s32 Func_02004b4e();

    s32 rec7;
    s32 big;
    s32 first;
    s32 shown;
    s32 second;
    s32 base3_2000240;

    rec7 = Func_02004ac8(15);
    big = 0x80000;
    Func_02004ab8();
    Func_02002bc4(15, 0);
    Call4(Func_02002abc, 15, 0x1d8, 104, big);
    Func_02004ace(10);
    Func_02002834(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), (*(s32 *)(rec7 + 16) + big), 0, 0, 0, 1, 0);
    Func_02004ba8(15, 1);
    Func_02004b7a(15, 0, 0);
    Func_02004b00(30);
    Func_02004b70_a(15, 2);
    Call3_02001d04(Func_02004bba, 15, 0x103, 0);
    Func_02004c30(147);
    Func_02004b1e(60);
    first = Func_02004b44_a(0);
    shown = *(s16 *)(first + 10);
    second = Func_02004b4e(0);
    Call4(Func_02002b32, 15, shown, *(s16 *)(second + 18), 0x60000);
    ((s64 (*)())Func_02004b44_b)(10);
    Call1(Func_02004b42, 0x307);
    base3_2000240 = (s32)Data_02000240;
    *(u8 *)((base3_2000240 + 0x22b)) = 3;
    Func_02004c3c(53, 0);
    Func_02004b70_b();
}

void Scene_RunSlot16WaypointSequence(void)
{
    u8 *slot;

    slot = Func_02004ba4(16);

    /* r0 still holds the record returned above. */
    Func_02004b92();

    Func_02002c9e(16, 1);
    Func_02002b96(16, 456, 152, 0x60000);       /* 228 << 1, 192 << 11 */
    Func_02004ba8_a(10);

    Func_02002912(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_02004c86(16, 1);
    Func_02004c58(16, 0, 0);
    Func_02004bde(20);
    Func_02004c4e(16, 2);
    Func_02004ca0(16, 258);                     /* 129 << 1 */
    Func_02004bf8(60);

    /* Three waypoints at height 0x30000 (192 << 10). */
    Func_02002bfc(16, 448, 192, 0x30000);       /* 224 << 1 */
    Func_02004c8c(0, 16, 0);
    Func_02004c1c(6);

    Func_02002c1c(16, 424, 208, 0x30000);       /* 212 << 1 */
    Func_02004caa(0, 16, 0);
    Func_02004c38(6);

    Func_02002c38(16, 424, 224, 0x30000);
    Func_02004cc6(0, 16, 0);
    Func_02004c54(6);

    Func_02004d14(0, 1);
    Func_02004cbe(16, 0, 0);
    Func_02004c6c_a(30);
    Func_02004c6c_b(776);                         /* 194 << 2 */
    Func_02004cd6(20, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_02004c92();
}

void Scene_RunActor17CameraSequence(void)
{
    u8 *slot;

    slot = Func_02004cb4(17);

    /* r0 still holds the record returned above. */
    Func_02004ca4();

    Func_02002db0(17, 1);
    Func_02002ca8(17, 392, 104, 0x60000);       /* 196 << 1, 192 << 11 */
    Func_02004cba(10);

    Func_02002a24(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_02004d98_a(17, 1);
    Func_02004d6a(17, 0, 0);
    Func_02004cf0(20);
    Func_02004d60(17, 2);
    Func_02004db2(17, 258);                     /* 129 << 1 */
    Func_02004d08(60);

    Func_02002d0a(17, 376, 152, 0x60000);       /* 188 << 1 */
    Func_02004d98_b(0, 17, 0);
    Func_02004d28(10);

    Func_02002d2c(17, 328, 160, 0x30000);       /* 164 << 1, 192 << 10 */
    Func_02004dba(0, 17, 0);
    Func_02004d48(6);

    Func_02002d4a(17, 296, 160, 0x30000);       /* 148 << 1 */
    Func_02004dd8(0, 17, 0);
    Func_02004d66(6);

    Func_02004e26(0, 1);
    Func_02004dd0(17, 0, 0);
    Func_02004d7e(30);
    Func_02004d7c(0x309);
    Func_02004de6(21, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_02004da2();
}

void FieldScene_RunScene39f_02002004(void)
{
    u32 i;
    s32 record;

    Func_02004dac();
    Func_02002eb8(18, 1);
    Call4(Func_02004e86, 0x2e80000, -1, 0x1f80000, 1);
    Call4(Func_02002dc4, 18, 0x2e8, 0x1f8, 0x90000);
    Func_02002e52(18);
    Func_02004e66(18, 15);
    record = Func_02004e04(18);
    Func_02004daa(record, 0);
    Func_02004df0(30);
    Call1(Func_02004dee, 0x30a);
    Call3_02002004(Func_02004e5c, 22, 0x2e80000, 0x1f80000);
    Func_02004e18();
}

void Scene_RunActorEighteenEffectSequence(void)
{
    u8 *slot;

    slot = Func_02004e40(18);

    /* r0 still holds the record returned above. */
    Func_02004e30();

    Func_02002f3c(18, 1);
    Func_02002e38(18, 712, 536, 0x60000);       /* 178 << 2, 134 << 2, 192 << 11 */
    Func_02004e4a(10);

    Func_02002bb4(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_02004f28_a(18, 1);
    Func_02004efa(18, 0, 0);
    Func_02004e80(20);
    Func_02004ef0(18, 2);
    Func_02004f42(18, 258);                     /* 129 << 1 */
    Func_02004e98(60);

    Func_02002e9a(18, 712, 568, 0x60000);       /* 142 << 2 */
    Func_02004f28_b(0, 18, 0);
    Func_02004eb8(10);

    Func_02002ebc(18, 712, 600, 0x30000);       /* 150 << 2, 192 << 10 */
    Func_02004f4a(0, 18, 0);
    Func_02004ed8(6);

    Func_02002edc(18, 736, 640, 0x30000);       /* X += 24, 160 << 2 */
    Func_02004f6a(0, 18, 0);
    Func_02004ef8(6);

    Func_02002efa(18, 736, 704, 0x30000);       /* 176 << 2 */
    Func_02004f88(0, 18, 0);
    Func_02004f16(6);

    Func_02004fd6(0, 1);
    Func_02004f80(18, 0, 0);
    Func_02004f2e(30);
    Func_02004f2c(0x30b);

    /* Common exit; no argument registers are set. */
    Func_02004f48();
}

void FieldScene_RunScene39f_020021b0(void)
{
    s32 rec7;

    rec7 = Value1(Func_02004f74, 18);
    Func_02004f62();
    Call3_020021b0(Func_02004fc0, 18, 0x880000, 0x1680000);
    Func_0200307c(18, 1);
    Func_02002f74(18, 136, 0x198, 0x80000);
    Func_02004f86(10);
    Func_02002cf0(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), (*(s32 *)(rec7 + 16) + 0x40000), 0, 0, 0, 1, 0);
    Call3_020021b0(Func_02005048, 18, 0xc000, 40);
    Call2(Func_0200506a, 18, 0x102);
    Func_02005032(18, 2);
    Func_02005082(18, 1);
    Func_02002fce(18, 136, 0x1b8, 0x60000);
    Func_0200505c_a(0, 18, 0);
    Func_02004fec(10);
    Func_02002ff0(18, 136, 0x1d8, 0x30000);
    Func_0200507e(0, 18, 0);
    Func_0200500c(6);
    Func_0200300e(18, 136, 0x1f8, 0x30000);
    Func_0200509c(0, 18, 0);
    Func_0200502a(6);
    Func_020050ea(0, 1);
    Func_02005094(18, 0, 0);
    Func_02005042(60);
    Call1(Func_02005040, 0x89d);
    Func_0200505c_b();
}
#undef FieldScene_RunScene39f_02000d90
#undef Scene_RunScriptedSteps0And17E6
#undef Scene_RunActor10WaypointSequence
#undef Scene_RunActorElevenPresentationBeat
#undef Scene_RunStepFD4WithActor181
#undef Scene_RunProbedActorEightOrTenScene
#undef Scene_RunActorThirteenPresentationBeat
#undef FieldScene_RunScene39f_02001818
#undef Scene_RunSupplementalSequenceOne
#undef Scene_RunScene39fSequenceA
#undef Scene_RunSlot16WaypointSequence
#undef Scene_RunActor17CameraSequence
#undef FieldScene_RunScene39f_02002004
#undef Scene_RunActorEighteenEffectSequence
#undef FieldScene_RunScene39f_020021b0

/* overlays/scene/actor/waypoint/scene_setup.c */
#define Scene_RunSixCallSetupSequence Func_02000ea8
void Func_02003d14(s32 no, s32 val);
s32 Func_02003d34(s32 no, s32 val);
void Func_02003d30(void);
void Func_02003c66(s32 no);
void Func_02001ce8(s32 no);
void Func_02003cfc(s32 no, s32 val);

void Scene_RunSixCallSetupSequence(s32 no, s32 val)
{
    s32 v0 = 0x20000;
    s32 v1 = 0x4000;

    Func_02003d14(v0, v1);
    Func_02003d34(no, 1);
    Func_02003d30();
    Func_02003c66(30);
    Func_02001ce8(no);
    Func_02003cfc(no, val);
}
#undef Scene_RunSixCallSetupSequence

/* overlays/scene/actor/waypoint/staged_actor.c */
#define TestActorPosition Func_020032b0
#define ActorProbeOffsets Data_0200af50
#define ActorSearchStep Data_0200aef8
#define StagedActor_FindClearPosition Func_02000474


s32 StagedActor_FindClearPosition(s32 *a)
{
}
#undef TestActorPosition
#undef ActorProbeOffsets
#undef ActorSearchStep
#undef StagedActor_FindClearPosition

/* overlays/scene/actor/waypoint/staged_actor_pair.c */
#define StagedActorStepTable Data_0200aef8
#define GetStagedActor Func_02002e90
#define FindNextStagedActor Func_02000176
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define CanStartStagedActorMove Func_02002ef4
#define SetStagedActorMode Func_02002edc_a
#define SelectStagedActorSlot Func_02002eac
#define StartStagedActorEffect Func_0200307a
#define StartNextStagedActorMove Func_02002f12
#define StartLeadStagedActorMove Func_02002f22
#define FinishStagedActorMove Func_02002f30
#define FinishStagedActorEffect Func_0200309c
#define SetStagedActorTransition Func_02002f48
#define StagedActor_AdvanceActorPair Func_020000c4


extern u32 Data_0200aef8[];

struct StagedActor *Func_02002e90(s32 arg0);
struct StagedActor *Func_02000176(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001a2(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001ce(s32 *arg0, struct StagedActor *arg1);
s32 Func_02002ef4(struct StagedActor *arg0, s32 *arg1);
void Func_02002edc_a(struct StagedActor *arg0, s32 arg1);
void Func_02002eac(s32 arg0);
void Func_0200307a(s32 arg0);
void Func_02002f12(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02002f22(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02002f30(struct StagedActor *arg0);
void Func_0200309c(void);
void Func_02002f48(struct StagedActor *arg0, s32 arg1);

void StagedActor_AdvanceActorPair(void)
{
    s32 dst[3];
    struct StagedActor *lead;
    struct StagedActor *next;
    struct StagedActor *blocker;
    s32 dir;
    u32 step;
    s32 rate;
    s32 zero;

    lead = GetStagedActor(0);
    dir = lead->direction_and_kind >> 12;
    step = StagedActorStepTable[dir];
    dst[0] = lead->x.value + (step & 0xffff0000);
    dst[1] = lead->y;
    step <<= 16;
    dst[2] = lead->z.value + step;
    next = FindNextStagedActor(dst, lead);
    if (next == 0) return;

    step = StagedActorStepTable[dir];
    dst[0] = next->x.value + (step & 0xffff0000);
    dst[1] = next->y;
    step <<= 16;
    dst[2] = next->z.value + step;
    blocker = FindBlockingStagedActor(dst, next);
    if (blocker != 0 && (blocker->collision_flags & 1) != 0) return;

    dst[0] = next->x.value;
    dst[1] = next->y + 0x100000;
    dst[2] = next->z.value;
    blocker = FindElevatedBlockingStagedActor(dst, next);
    if (blocker != 0 && (blocker->collision_flags & 1) != 0) return;

    next->transition_mode = 2;
    step = StagedActorStepTable[dir];
    dst[0] = next->x.value + (step & 0xffff0000);
    dst[1] = next->y;
    step <<= 16;
    dst[2] = next->z.value + step;
    if (CanStartStagedActorMove(next, dst) > 0) return;

    zero = next->transition_busy;
    if (zero != 0) return;

    SetStagedActorMode(lead, 8);
    rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next->move_rate_x = rate;
    next->move_rate_z = rate;
    StartNextStagedActorMove(next, dst[0], dst[1], dst[2]);
    lead->move_rate_x = rate;
    lead->move_rate_z = rate;
    StartLeadStagedActorMove(lead, dst[0], dst[1], dst[2]);
    FinishStagedActorMove(next);
    FinishStagedActorEffect();
    next->x.value = dst[0];
    next->z.value = dst[2];
    next->unknown_24 = zero;
    next->unknown_2c = zero;
    lead->unknown_38 = 0x80000000;
    lead->unknown_40 = 0x80000000;
    lead->unknown_24 = zero;
    lead->unknown_2c = zero;
    lead->x.value = lead->x.parts.cell << 16;
    lead->z.value = lead->z.parts.cell << 16;
    SetStagedActorTransition(lead, 1);
}
#undef StagedActorStepTable
#undef GetStagedActor
#undef FindNextStagedActor
#undef FindBlockingStagedActor
#undef FindElevatedBlockingStagedActor
#undef CanStartStagedActorMove
#undef SetStagedActorMode
#undef SelectStagedActorSlot
#undef StartStagedActorEffect
#undef StartNextStagedActorMove
#undef StartLeadStagedActorMove
#undef FinishStagedActorMove
#undef FinishStagedActorEffect
#undef SetStagedActorTransition
#undef StagedActor_AdvanceActorPair

/* overlays/scene/actor/waypoint/state_update.c */
#define State_SetValue18Mode2 Func_02000cc0
#define State_ApplyCrossRectsAroundActor11 Func_02001520
void Func_02003b4c(s32 a, s32 b);
void Func_020042ce(void);
s32 *Func_020042f4(s32);
s32 *Func_020042ec(s32);
s32 *Func_02004366(s32);
s32 Func_02001796(s32, s32, s32, s32, s32, s32);
s32 Func_020017aa(s32, s32, s32, s32, s32, s32);
s32 Func_020017bc(s32, s32, s32, s32, s32, s32);
s32 Func_020017ce(s32, s32, s32, s32, s32, s32);
s32 Func_020017e0(s32, s32, s32, s32, s32, s32);
void Func_02004368(void);

/* Three sites of the record accessor, so three names. */

/* Five sites of one import. */

void State_SetValue18Mode2(void)
{
    Func_02003b4c(18, 2);
}

void State_ApplyCrossRectsAroundActor11(void)
{
    s32 x;
    s32 z;

    /* No argument register is written before this branch. */
    Func_020042ce();

    /* Both coordinates are 16.16 fixed point reduced to whole tiles with
     * `asrs #20`, i.e. 16 fractional bits plus a 16-unit tile pitch. */
    x = Func_020042ec(11)[2] >> 20;
    z = Func_020042f4(11)[4] >> 20;

    Func_02001796(2, x, z, 1, 1, 0xff);
    Func_020017aa(2, x + 1, z, 1, 1, 0);
    Func_020017bc(2, x - 1, z, 1, 1, 0);
    Func_020017ce(2, x, z + 1, 1, 1, 0);
    Func_020017e0(2, x, z - 1, 1, 1, 0);

    if (x == 36 && z == 24) {
        u8 *p = (u8 *)Func_02004366(11);

        p[85] = 0;
        *(s32 *)(p + 20) = (s32)0xfffe0000;
        *(s32 *)(p + 12) = (s32)0xfffe0000;
    }

    /* Common exit; no argument registers are set. */
    Func_02004368();
}
#undef State_SetValue18Mode2
#undef State_ApplyCrossRectsAroundActor11
