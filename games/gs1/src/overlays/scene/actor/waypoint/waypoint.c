#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/waypoint/waypoint.h"
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
s32 *Actor_Run7(s32 a);

s32 *Actor_Run8(s32 a);

u8 *Actor_Run9(s32);

u8 *Actor_Run10(s32);

u8 *Actor_Run11(s32);
u8 *Actor_Run12(s32);
u8 *Actor_Run13(s32);
u8 *Actor_Run14(s32);

/* Two veneers each serve two imports: 0x020046fa is the three-argument setter
 * at site 8 and the one-argument wait at site 21, and 0x02004730 is the
 * three-argument step at site 14 and the one-argument wait at site 27. */

/* Four sites of the record accessor, so four names. */

/* 0x02004918 serves two imports: the two-argument mode select and the
 * zero-argument bracket close. */

s32 Actor_TryRunSlotZeroMoveStep(s16 *arg)
{
    s32 *p = Actor_Run7(0);
    u8 *f = (u8 *)p + 0x55;
    s32 saved = *f;

    s32 r = Actor_Apply(p, arg);

    if (r == 0) {
        s32 m;

        Actor_Run15();
        Actor_Apply2(p, 6);
        Actor_Do6(6);
        Actor_Do7(152);
        Actor_Apply3(p, 7);
        p[12] = 0x30000;
        p[13] = 0x20000;
        p[10] = 0x40000;
        m = 0x7e;
        m &= *f;
        *f = m;
        Actor_Apply4(p, 0);
        Actor_Place7(0, arg[1], arg[5]);
        Actor_Apply5(p, 6);
        Actor_Apply6(p, 1);
        *f = saved;
        Actor_Run16();
        return 1;
    }
    return 0;
}

void Actor_BobActorZeroWhenAheadClear(void)
{
    s32 pos[3];
    s32 *actor = Actor_Run8(0);
    u8 *fp = (u8 *)actor + 0x55;
    s32 saved = *fp;

    pos[0] = (actor[2] & 0xfff00000) + 0x80000;
    pos[1] = actor[3];
    pos[2] = (actor[4] & 0xfff00000) + 0x280000;
    if (Actor_Check6(pos)!= 0) {
        Actor_Run17();
        *fp = 0;
        Actor_Apply7(9, 7);
        actor[3] += -0x10000;
        actor[5] += -0x10000;
        Actor_Do8(2);
        actor[3] += -0x10000;
        actor[5] += -0x10000;
        Actor_Do9(10);
        actor[3] += 0x10000;
        actor[5] += 0x10000;
        Actor_Do10(4);
        actor[3] += 0x10000;
        actor[5] += 0x10000;
        *fp = saved;
        Actor_Run18();
    }
}

void Actor_RunActorTwelveThreeWaypointMotion(void)
{
    u8 *slot;

    slot = Actor_Run9(12);

    /* r0 still holds the record returned above. */
    Actor_Run19();

    Actor_Apply8(12, 1);
    Actor_SetMode(12, 536, 344, 0x70000);       /* 134 << 2, 172 << 1, 224 << 11 */

    Actor_SetRect4(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x100000,   /* 128 << 13 */
                  0, 0, 0, 1, 0);

    Actor_Apply9(12, 1);
    Actor_Place8(12, 0, 0);
    Actor_Do11(20);
    Actor_Apply10(12, 2);
    Actor_Apply11(12, 258);                     /* 129 << 1 */
    Actor_Do12(60);

    /* Three waypoints, each at height 0x30000 (192 << 10); the X literals are
     * 146 << 2, 158 << 2 and 170 << 2 and the Z is the same 172 << 1. */
    Actor_SetMode2(12, 584, 344, 0x30000);
    Actor_Place9(0, 12, 0);
    Actor_Do13(6);

    Actor_SetMode3(12, 632, 344, 0x30000);
    Actor_Place10(0, 12, 0);
    Actor_Do14(6);

    Actor_SetMode4(12, 680, 344, 0x30000);
    Actor_Place11(0, 12, 0);
    Actor_Do15(6);

    Actor_Do16(0x302);
    Actor_Place12(15, 0, 0);

    /* Common exit; no argument registers are set. */
    Actor_Run20();
}

void Actor_RunActorFourteenFourWaypointMotion(void)
{
    u8 *slot;

    slot = Actor_Run10(14);

    /* r0 still holds the record returned above. */
    Actor_Run21();

    Actor_Apply12(14, 1);
    Actor_SetMode5(14, 392, 504, 0x60000);       /* 196 << 1, 252 << 1, 192 << 11 */
    Actor_Do17(10);

    Actor_SetRect5(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Actor_Apply13(14, 1);
    Actor_Place13(14, 0, 0);
    Actor_Do18(20);
    Actor_Apply14(14, 2);
    Actor_Apply15(14, 258);                     /* 129 << 1 */
    Actor_Do19(60);

    /* Four waypoints; Z is 132 << 2 and the height 192 << 10 throughout. */
    Actor_SetMode6(14, 360, 528, 0x30000);
    Actor_Place14(0, 14, 0);
    Actor_Do20(6);

    Actor_SetMode7(14, 328, 528, 0x30000);
    Actor_Place15(0, 14, 0);
    Actor_Do21(6);

    Actor_SetMode8(14, 288, 528, 0x30000);
    Actor_Place16(0, 14, 0);
    Actor_Do22(6);

    Actor_SetMode9(14, 256, 528, 0x30000);
    Actor_Place17(0, 14, 0);
    Actor_Do23(6);

    Actor_Apply16(0, 1);
    Actor_Place18(14, 0, 0);
    Actor_Do24(30);
    Actor_Do25(0x306);
    Actor_Place19(17, 0, 0);

    /* Common exit; no argument registers are set. */
    Actor_Run22();
}

void Actor_BobActorZeroWhenTargetClear(void)
{
    u8 *record;
    u8 *mode;
    u8 saved;
    s32 target[3];

    record = Actor_Run11(0);
    mode = record + 85;
    saved = *mode;

    target[0] = *(s32 *)(Actor_Run12(0) + 8) + (s32)0xffe00000;
    target[1] = *(s32 *)(Actor_Run13(0) + 12);
    target[2] = *(s32 *)(Actor_Run14(0) + 16);

    if (Actor_Check7(target)!= 0) {
        /* r0 still holds the nonzero result of the test above. */
        Actor_Run23();

        *mode = 0;
        Actor_Apply17(11, 7);

        *(s32 *)(record + 12) += (s32)0xffff0000;
        *(s32 *)(record + 20) += (s32)0xffff0000;
        Actor_Do26(2);

        *(s32 *)(record + 12) += (s32)0xffff0000;
        *(s32 *)(record + 20) += (s32)0xffff0000;
        Actor_Do27(10);

        *(s32 *)(record + 12) += 0x10000;
        *(s32 *)(record + 20) += 0x10000;
        Actor_Do28(4);

        *(s32 *)(record + 12) += 0x10000;
        *(s32 *)(record + 20) += 0x10000;

        *mode = saved;
        Actor_Run24();
    }
}

/* overlays/scene/actor/waypoint/actor_position.c */
s32 *Actor_Run25(s32 a);

s32 *Actor_Run26();
s32 *Actor_Run27();

s32 *Actor_Run28();

/* The five tile-painting calls take (layer, x, z, width, height, value) and
 * all reach the same routine, but each keeps its own call word: the encoding
 * is per site, so they must not be collapsed onto one alias. */

void Actor_PassOffsetPointOfActorZero(void)
{
    s32 v[3];
    s32 *p = Actor_Run25(0);

    v[0] = (p[2] & 0xfff00000) + 0x80000;
    v[1] = p[3];
    v[2] = (p[4] & 0xfff00000) + 0xffe80000;
    Actor_Do29(v);
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
    Actor_Run29();

    x = Actor_Run26(13)[2] >> 20;
    z = Actor_Run27(13)[4] >> 20;

    Actor_Check8(2, x, z, 1, 1, 0xff);
    Actor_Check9(2, x + 1, z, 1, 1, 0);
    Actor_Check10(2, x - 1, z, 1, 1, 0);
    Actor_Check11(2, x, z + 1, 1, 1, 0);
    Actor_Check12(2, x, z - 1, 1, 1, 0);

    if (x == 45 && z == 6) {
        u8 *record = (u8 *)Actor_Run28(13);

        record[85] = 0;
        *(s32 *)(record + 20) = (s32)0xfffe0000;
        *(s32 *)(record + 12) = (s32)0xfffe0000;
    }

    /* Common exit; no argument registers are set. */
    Actor_Run30();
}

/* overlays/scene/actor/waypoint/configurable_effect_spawn.c */

void Effect_SpawnConfigured(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct Options *options)
{
}

/* overlays/scene/actor/waypoint/move_and_redraw.c */

#define STAGED_ACTOR_MOVEMENT_UNTYPED_LOCATION_CALL

void Actor_Run31(
    StagedActorMovementRequest request)
{
}

/* overlays/scene/actor/waypoint/orbiting_effect.c */
#define UpdateOrbitingSceneObject Value

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

extern u8 Value;

OrbitingSceneObject *GetOrbitingSceneObject(void);

u8 *AllocateEffectTransfer(s32, s32);

s32 Actor_Run32(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Actor_Check13(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Actor_Check14(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Actor_Check15(p[12] + 0x8000) / 8;
    a = Actor_Check16();
    b = Actor_Check17();
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

/* overlays/scene/actor/waypoint/overlay_object.c */
u8 *Actor_Run33(s32 kind, s32 x, s32 y, s32 z);

u8 *Actor_Run34(s32 kind, s32 x, s32 y, s32 z);

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
    u8 *obj = Actor_Run33(kind, x, y, z);

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
        Actor_Apply18(obj, 0);
        Actor_Apply19(obj, 14);
        Actor_Apply20(obj, 1);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
}

u8 *OvObj_SpawnConfiguredWithMode15(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Actor_Run34(kind, x, y, z);

    if (obj != 0) {
        u8 *rec = *(u8 **)(obj + 80);
        u32 mask = ~12;
        u32 m;
        mask = mask & rec[9];
        rec[9] = (u8)(mask | 4);
        obj[85] = 0;
        obj[89] = 8;
        Actor_Apply21(obj, 0);
        Actor_Apply22(obj, 15);
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
    Actor_Apply23(a, *(s16 *)(a + 100));
    return 0;
}

s32 OvObj_ApplyZero(s32 a)
{
    Actor_Apply24(a, 0);
    return 0;
}

/* overlays/scene/actor/waypoint/scene_data_head.c */
#define PrimaryRuntimeSelector Value_00000044
#define SecondaryRuntimeSelector Value_00000045
#define TertiaryRuntimeSelector Value_00000046

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

/* overlays/scene/actor/waypoint/scene_primary_script.c */
/* The probe record is passed on by value: four words in r0-r3 and the last two
 * copied to the outgoing stack area by an `ldmia`/`stmia` pair. */
struct Resource39fProbe {
    s32 word[6];
};

extern u8 gWork[];

u8 *Actor_Run35(s32);

u8 *Actor_Run36();

u8 *Actor_Run37();
u8 *Actor_Run38();

u8 *Actor_Run39(s32);

u8 *Actor_Run40(s32);

u8 *Actor_Run41();

u8 *Actor_Run42(s32);

u8 *Actor_Run43(s32);

u8 *Actor_Run44(s32);

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

void Scene_RunScene39f(s32 a0, s32 a1, s32 a2, s32 a3)
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
    rec7 = Actor_Check18();
    Actor_Run45(a0, 1);
    Actor_Place(a0, 0x30000, 0x18000);
    Actor_Run46(152);
    *(volatile s32 *)(rec7 + 40) = p9;
    *(volatile s32 *)(rec7 + 72) = 0x8000;
    *(volatile s32 *)(rec7 + 68) = 0;
    Actor_Run47(rec7, 0);
    Actor_Run48(a0, p8, p10);
    p8b = ((s32)p8 << 16);
    p10b = ((s32)p10 << 16);
    Actor_Run49(a0, p8b, p10b);
    Actor_Run50(rec7, 1);
    *(volatile s32 *)(rec7 + 72) = 0x10000;
    p10c = a0;
}

void Scene_RunScriptedSteps0And17E6(void)
{
    Actor_Run51();
    Actor_Apply25(0, 1);
    Actor_Apply26(6118, 1);
    Actor_Run52();
}

void Scene_RunActor10WaypointSequence(void)
{
    u8 *slot;

    slot = Actor_Run35(10);

    /* r0 still holds the record returned above. */
    Actor_Run53();

    Actor_Apply27(10, 1);
    Actor_SetMode10(10, 88, 120, 0x60000);        /* 192 << 11 */

    Actor_SetRect6(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x180000,   /* 192 << 13 */
                  0, 0, 0, 1, 0);

    Actor_Apply28(10, 1);
    Actor_Place20(10, 0, 0);
    Actor_Do30(20);
    Actor_Apply29(10, 2);
    Actor_Apply30(10, 258);                     /* 129 << 1 */
    Actor_Do31(60);

    /* Three waypoints, each at height 0x30000 (192 << 10). */
    Actor_SetMode11(10, 88, 152, 0x30000);
    Actor_Place21(0, 10, 0);
    Actor_Do32(10);

    Actor_SetMode12(10, 120, 192, 0x30000);
    Actor_Place22(0, 10, 0);
    Actor_Do33(10);

    Actor_SetMode13(10, 120, 240, 0x30000);
    Actor_Place23(0, 10, 0);
    Actor_Do34(10);

    Actor_Do35(768);                       /* 192 << 2 */
    Actor_Place24(13, 0, 0);
    Actor_Place25(10, 0, 0);

    /* Common exit; no argument registers are set. */
    Actor_Run54();
}

/*
 * A full cutscene beat for slot 11: opens the slot, places it at (408, 456),
 * publishes an eight-argument piece, runs the presentation, then re-places the
 * slot on the party's current heading readings and sets the engine byte at
 * RuntimeSelectorTable + 0x22b to 3.  The 228-byte owner includes an alignment
 * halfword and its four pool words.
 */
void Scene_RunActorElevenPresentationBeat(void)
{
    extern s16 RuntimeSelectorTable[];

    u8 *slot;
    s32 offset;

    slot = Actor_Run36(11);

    /* Reads the record left in r0 by the call above; it must not be respelled
     * as a fresh fetch. */
    Actor_Run55();

    Actor_Run56(11, 0);
    Actor_Run57(11, 408, 456, 0x60000);   /* 204 << 1, 228 << 1, 192 << 11 */

    Actor_Run58(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x180000,   /* 192 << 13 */
                  0, 0, 0, 1, 0);

    Actor_Run59(11, 1);
    Actor_Run60(11, 0, 0);
    Actor_Run61(30);
    Actor_Run62(11, 2);
    Actor_Run63(11, 0x103, 0);
    Actor_Run64(147);
    Actor_Run65(60);

    /* Two signed halfwords of slot 0, each read after its own fetch of the
     * record. */
    Actor_Run66(11,
                  *(s16 *)(Actor_Run37(0) + 10),
                  *(s16 *)(Actor_Run38(0) + 18),
                  0x40000);                          /* 128 << 11 */

    Actor_Run67(10);
    Actor_Run68(0x301);
    Actor_Run69(14, 0, 0);

    offset = 0x22b;
    ((u8 *)RuntimeSelectorTable)[offset] = 3;

    Actor_Run70(53, 0);

    /* Common exit; no argument registers are set. */
    Actor_Run71();
}

void Scene_RunStepFD4WithActor181(s32 a)
{
    Actor_Do36(a);
    Actor_Place26(16, 0, 0);
    Actor_Do37(4052);
    Actor_Apply31(181, 3);
    Actor_Apply32(181, 0);
    Actor_Run72();
}

void Scene_RunProbedActorEightOrTenScene(void)
{
    struct Resource39fProbe probe;
    s32 fifth;
    s32 sixth;
    s32 height;
    s32 value;

    /* No argument register is written before this branch. */
    Actor_Run73();

    if (Actor_Check19(&probe) != 0) {
        Actor_Do38(probe);

        if (probe.word[1] == 8 && (probe.word[4] >> 20) == 23) {
            fifth = 35;
            sixth = 68;
            Actor_SetRect7(35, 67, 4, 1, fifth, sixth);
        } else if (probe.word[1] == 10 && (probe.word[2] >> 20) == 35) {
            /* Written here, not at the call: the reference keeps it in a
             * callee-saved register across the whole sequence. */
            value = 0;
            Actor_Do39(0x311);
            Actor_Apply33(10, 3);
            Actor_Place27(10, -16, 6);
            Actor_Do40(30);
            Actor_Apply34(10, 8);
            Actor_Do41(240);

            Actor_Run39(10)[35] = 2;

            fifth = 34;
            sixth = 30;
            Actor_SetRect8(44, 30, 2, 4, fifth, sixth);
            height = 4;
            Actor_SetRect9(2, 35, 30, 1, height, value);
        }
    }

    /* Common exit; no argument registers are set. */
    Actor_Run74();
}

void Scene_RunActorThirteenPresentationBeat(void)
{
    u8 *slot;

    slot = Actor_Run40(13);

    /* r0 still holds the record returned above. */
    Actor_Run75();

    Actor_Apply35(13, 1);
    Actor_SetMode14(13, 456, 104, 0x70000);       /* 228 << 1, 224 << 11 */
    Actor_Do42(10);

    Actor_SetRect10(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Actor_Apply36(13, 1);
    Actor_Place28(13, 0, 0);
    Actor_Do43(20);
    Actor_Apply37(13, 2);
    Actor_Apply38(13, 258);                     /* 129 << 1 */
    Actor_Do44(60);

    Actor_SetMode15(13, 472, 136, 0x30000);       /* 236 << 1, 192 << 10 */
    Actor_Place29(0, 13, 0);
    Actor_Do45(6);

    Actor_SetMode16(13, 504, 136, 0x33333);       /* 252 << 1, pooled height */
    Actor_Place30(0, 13, 0);
    Actor_Do46(6);

    Actor_SetMode17(13, 552, 136, 0x38000);       /* 138 << 2, 224 << 10 */
    Actor_Place31(0, 13, 0);
    Actor_Do47(6);

    Actor_SetMode18(13, 584, 136, 0x38000);       /* 146 << 2 */
    Actor_Place32(0, 13, 0);
    Actor_Do48(6);

    Actor_Place33(13, 0, 0);
    Actor_Do49(772);                         /* 193 << 2 */
    Actor_Place34(16, 0, 0);

    /* Common exit; no argument registers are set. */
    Actor_Run76();
}

void Scene_RunScene39f(void)
{
    u32 i;
    s32 record;

    Actor_Run77();
    Actor_Run78(14, 1);
    Actor_Run(14, 0x1a8, 0x1e0, 0x79999);
    Actor_Run79(2);
    Actor_Run80(14);
    Actor_Run81(14, 15);
    record = Actor_Check20(14);
    Actor_Run82(record, 0);
    Actor_Run83(30);
    Actor_Do(0x305);
    Actor_Place2(17, 0x1a80000, 0x1e00000);
    Actor_Run84();
}

void Scene_RunSupplementalSequenceOne(void)
{
    extern u8 RuntimeSelectorTable[];
    u8 *Actor_Run85();

    s32 one;
    s32 x;
    s32 y;
    u8 *record;

    Actor_Run86();
    record = Actor_Check(14);
    x = *(volatile s32 *)((s32)record + 8);
    record = Actor_Check2(14);
    y = *(volatile s32 *)((s32)record + 16);
    x >>= 20;
    y >>= 20;
    one = 1;
    Actor_Check21(2, x, y, 1, one, 255);
    Actor_SetRect(2, x + 1, y, 1, one, 0);
    Actor_SetRect2(2, x - 1, y, 1, one, 0);
    Actor_Check22(2, x, y + 1, 1, one, 0);
    Actor_Check23(2, x, y - 1, 1, one, 0);
    record = Actor_Check3(14);
    if ((*(volatile s32 *)((s32)record + 16) >> 20) == 27) {
        record = Actor_Check4(14);
        record[85] = 0;
        *(volatile s32 *)((s32)record + 20) = -0x20000;
        *(volatile s32 *)((s32)record + 12) = -0x20000;
        Actor_Do2(0x214);
        Actor_SetRect3(2, 43, 23, 1, one, 255);
    }
    Actor_Run87();
}

/*
 * Actor presentation beat for overlay resource_39f.  The twin at 0x02001d04
 * is the same beat for slot 15.
 */
void Scene_RunScene39fSequenceA(void)
{
    extern u8 RuntimeSelectorTable[];
    s32 Actor_Run85();

    s32 rec7;
    s32 big;
    s32 first;
    s32 shown;
    s32 second;
    s32 base3_2000240;

    rec7 = Actor_Check24(15);
    big = 0x80000;
    Actor_Run88();
    Actor_Run89(15, 0);
    Actor_Run2(15, 0x1d8, 104, big);
    Actor_Run90(10);
    Actor_Run91(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), (*(s32 *)(rec7 + 16) + big), 0, 0, 0, 1, 0);
    Actor_Run92(15, 1);
    Actor_Run93(15, 0, 0);
    Actor_Run94(30);
    Actor_Run95(15, 2);
    Actor_Place3(15, 0x103, 0);
    Actor_Run96(147);
    Actor_Run97(60);
    first = Actor_Check25(0);
    shown = *(s16 *)(first + 10);
    second = Actor_Run85(0);
    Actor_Run3(15, shown, *(s16 *)(second + 18), 0x60000);
    ((s64 (*)())Actor_Check26)(10);
    Actor_Do3(0x307);
    base3_2000240 = (s32)RuntimeSelectorTable;
    *(u8 *)((base3_2000240 + 0x22b)) = 3;
    Actor_Run98(53, 0);
    Actor_Run99();
}

void Scene_RunSlot16WaypointSequence(void)
{
    u8 *slot;

    slot = Actor_Run42(16);

    /* r0 still holds the record returned above. */
    Actor_Run100();

    Actor_Apply39(16, 1);
    Actor_SetMode19(16, 456, 152, 0x60000);       /* 228 << 1, 192 << 11 */
    Actor_Do50(10);

    Actor_SetRect11(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Actor_Apply40(16, 1);
    Actor_Place35(16, 0, 0);
    Actor_Do51(20);
    Actor_Apply41(16, 2);
    Actor_Apply42(16, 258);                     /* 129 << 1 */
    Actor_Do52(60);

    /* Three waypoints at height 0x30000 (192 << 10). */
    Actor_SetMode20(16, 448, 192, 0x30000);       /* 224 << 1 */
    Actor_Place36(0, 16, 0);
    Actor_Do53(6);

    Actor_SetMode21(16, 424, 208, 0x30000);       /* 212 << 1 */
    Actor_Place37(0, 16, 0);
    Actor_Do54(6);

    Actor_SetMode22(16, 424, 224, 0x30000);
    Actor_Place38(0, 16, 0);
    Actor_Do55(6);

    Actor_Apply43(0, 1);
    Actor_Place39(16, 0, 0);
    Actor_Do56(30);
    Actor_Do57(776);                         /* 194 << 2 */
    Actor_Place40(20, 0, 0);

    /* Common exit; no argument registers are set. */
    Actor_Run101();
}

void Scene_RunActor17CameraSequence(void)
{
    u8 *slot;

    slot = Actor_Run43(17);

    /* r0 still holds the record returned above. */
    Actor_Run102();

    Actor_Apply44(17, 1);
    Actor_SetMode23(17, 392, 104, 0x60000);       /* 196 << 1, 192 << 11 */
    Actor_Do58(10);

    Actor_SetRect12(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Actor_Apply45(17, 1);
    Actor_Place41(17, 0, 0);
    Actor_Do59(20);
    Actor_Apply46(17, 2);
    Actor_Apply47(17, 258);                     /* 129 << 1 */
    Actor_Do60(60);

    Actor_SetMode24(17, 376, 152, 0x60000);       /* 188 << 1 */
    Actor_Place42(0, 17, 0);
    Actor_Do61(10);

    Actor_SetMode25(17, 328, 160, 0x30000);       /* 164 << 1, 192 << 10 */
    Actor_Place43(0, 17, 0);
    Actor_Do62(6);

    Actor_SetMode26(17, 296, 160, 0x30000);       /* 148 << 1 */
    Actor_Place44(0, 17, 0);
    Actor_Do63(6);

    Actor_Apply48(0, 1);
    Actor_Place45(17, 0, 0);
    Actor_Do64(30);
    Actor_Do65(0x309);
    Actor_Place46(21, 0, 0);

    /* Common exit; no argument registers are set. */
    Actor_Run103();
}

void Scene_RunScene39f(void)
{
    u32 i;
    s32 record;

    Actor_Run104();
    Actor_Run105(18, 1);
    Actor_Run4(0x2e80000, -1, 0x1f80000, 1);
    Actor_Run5(18, 0x2e8, 0x1f8, 0x90000);
    Actor_Run106(18);
    Actor_Run107(18, 15);
    record = Actor_Check27(18);
    Actor_Run108(record, 0);
    Actor_Run109(30);
    Actor_Do4(0x30a);
    Actor_Place4(22, 0x2e80000, 0x1f80000);
    Actor_Run110();
}

void Scene_RunActorEighteenEffectSequence(void)
{
    u8 *slot;

    slot = Actor_Run44(18);

    /* r0 still holds the record returned above. */
    Actor_Run111();

    Actor_Apply49(18, 1);
    Actor_SetMode27(18, 712, 536, 0x60000);       /* 178 << 2, 134 << 2, 192 << 11 */
    Actor_Do66(10);

    Actor_SetRect13(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Actor_Apply50(18, 1);
    Actor_Place47(18, 0, 0);
    Actor_Do67(20);
    Actor_Apply51(18, 2);
    Actor_Apply52(18, 258);                     /* 129 << 1 */
    Actor_Do68(60);

    Actor_SetMode28(18, 712, 568, 0x60000);       /* 142 << 2 */
    Actor_Place48(0, 18, 0);
    Actor_Do69(10);

    Actor_SetMode29(18, 712, 600, 0x30000);       /* 150 << 2, 192 << 10 */
    Actor_Place49(0, 18, 0);
    Actor_Do70(6);

    Actor_SetMode30(18, 736, 640, 0x30000);       /* X += 24, 160 << 2 */
    Actor_Place50(0, 18, 0);
    Actor_Do71(6);

    Actor_SetMode31(18, 736, 704, 0x30000);       /* 176 << 2 */
    Actor_Place51(0, 18, 0);
    Actor_Do72(6);

    Actor_Apply53(0, 1);
    Actor_Place52(18, 0, 0);
    Actor_Do73(30);
    Actor_Do74(0x30b);

    /* Common exit; no argument registers are set. */
    Actor_Run112();
}

void Scene_RunScene39f(void)
{
    s32 rec7;

    rec7 = Actor_Check5(18);
    Actor_Run113();
    Actor_Place5(18, 0x880000, 0x1680000);
    Actor_Run114(18, 1);
    Actor_Run115(18, 136, 0x198, 0x80000);
    Actor_Run116(10);
    Actor_Run117(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), (*(s32 *)(rec7 + 16) + 0x40000), 0, 0, 0, 1, 0);
    Actor_Place6(18, 0xc000, 40);
    Actor_Run6(18, 0x102);
    Actor_Run118(18, 2);
    Actor_Run119(18, 1);
    Actor_Run120(18, 136, 0x1b8, 0x60000);
    Actor_Run121(0, 18, 0);
    Actor_Run122(10);
    Actor_Run123(18, 136, 0x1d8, 0x30000);
    Actor_Run124(0, 18, 0);
    Actor_Run125(6);
    Actor_Run126(18, 136, 0x1f8, 0x30000);
    Actor_Run127(0, 18, 0);
    Actor_Run128(6);
    Actor_Run129(0, 1);
    Actor_Run130(18, 0, 0);
    Actor_Run131(60);
    Actor_Do5(0x89d);
    Actor_Run132();
}

/* overlays/scene/actor/waypoint/scene_setup.c */

void Scene_RunSixCallSetupSequence(s32 no, s32 val)
{
    s32 v0 = 0x20000;
    s32 v1 = 0x4000;

    Actor_Apply54(v0, v1);
    Actor_Apply55(no, 1);
    Actor_Run133();
    Actor_Do75(30);
    Actor_Do76(no);
    Actor_Apply56(no, val);
}

/* overlays/scene/actor/waypoint/staged_actor.c */

s32 StagedActor_FindClearPosition(s32 *a)
{
}

/* overlays/scene/actor/waypoint/staged_actor_pair.c */

extern u32 Actor_Far2[];

struct StagedActor *GetStagedActor(s32 arg0);
struct StagedActor *FindNextStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindElevatedBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);

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

/* overlays/scene/actor/waypoint/state_update.c */

s32 *Actor_Run134(s32);
s32 *Actor_Run135(s32);
s32 *Actor_Run136(s32);

/* Three sites of the record accessor, so three names. */

/* Five sites of one import. */

void State_SetValue18Mode2(void)
{
    Actor_Apply57(18, 2);
}

void State_ApplyCrossRectsAroundActor11(void)
{
    s32 x;
    s32 z;

    /* No argument register is written before this branch. */
    Actor_Run137();

    /* Both coordinates are 16.16 fixed point reduced to whole tiles with
     * `asrs #20`, i.e. 16 fractional bits plus a 16-unit tile pitch. */
    x = Actor_Run135(11)[2] >> 20;
    z = Actor_Run134(11)[4] >> 20;

    Actor_SetRect14(2, x, z, 1, 1, 0xff);
    Actor_SetRect15(2, x + 1, z, 1, 1, 0);
    Actor_SetRect16(2, x - 1, z, 1, 1, 0);
    Actor_SetRect17(2, x, z + 1, 1, 1, 0);
    Actor_SetRect18(2, x, z - 1, 1, 1, 0);

    if (x == 36 && z == 24) {
        u8 *p = (u8 *)Actor_Run136(11);

        p[85] = 0;
        *(s32 *)(p + 20) = (s32)0xfffe0000;
        *(s32 *)(p + 12) = (s32)0xfffe0000;
    }

    /* Common exit; no argument registers are set. */
    Actor_Run138();
}
