#include "types.h"
#include "scene.h"
#include "staged_actor_movement.h"
#include "run_staged_actor_movement_and_redraw_body.inc"
#include "create_configured_overlay_object.h"
#include "create_configured_overlay_object_body.inc"
#include "select_overlay_data_by_runtime_selector.h"
#include "select_overlay_data_by_runtime_selector_body.inc"
#include "configured_effect_spawn.h"
#include "staged_actor.h"
#include "configured_effect_spawn_body.inc"

/* overlays/scene/actor/staged_pair_region/actor_facing.c */
u8 *Actor_unk5_4(s32 no);

/* Point an object toward actor zero using their fixed-point X/Z delta. */

s32 Actor_FaceActorZero(u8 *obj)
{
    u8 *target = Actor_unk5_4(0);
    s32 dz = *(s32 *)(target + 16) - *(s32 *)(obj + 16);
    s32 dx = *(s32 *)(target + 8) - *(s32 *)(obj + 8);

    *(s16 *)(obj + 6) = (s16)Actor_Apply(dz, dx);
    return 0;
}

/* overlays/scene/actor/staged_pair_region/actor_motion.c */

/* Give the actor at most sixty frames to descend to its target height, then
 * clamp the live height to the target so the following scene starts exact. */

void Actor_WaitActorDescent(u8 *obj)
{
    s32 cnt = 60;

    while (cnt != 0) {
        Actor_unk7_2(1);
        cnt--;
        if (*(s32 *)(obj + 12) <= *(s32 *)(obj + 20))
            break;
    }
    *(s32 *)(obj + 12) = *(s32 *)(obj + 20);
}

/* overlays/scene/actor/staged_pair_region/actor_position.c */
/* resource_3a6 owner at 0x02001938, 76 bytes.
 * Fetches a record for the caller's id and, when it exists, marks it and
 * writes two 20.12 fixed-point coordinates biased by half a unit. */
struct Rec_3a6 {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    u8 pad0c[4];
    s32 f16;                    /* +16 */
    u8 pad14[14];
    u8 f34;                     /* +34 */
    u8 f35;                     /* +35 */
};

struct Rec_3a6 *Actor_unk6_4();

void Actor_PlaceAtTile(s32 id, s32 x, s32 y)
{
    struct Rec_3a6 *rec = Actor_unk6_4(id);

    if (rec != 0) {
        Actor_unk7_4(id, 3);
        rec->f34 = 2;
        rec->f35 |= 2;
        rec->f8 = (x << 20) + 0x80000;
        rec->f16 = (y << 20) + 0x80000;
    }
}

/* overlays/scene/actor/staged_pair_region/actor_presentation.c */

/* Advance actor eleven through the two presentation states used at scene end. */

void ActorDraw_AdvanceActorElevenStates(void)
{
    Actor_Apply2(11, 1);
    Actor_Apply3(11, 2);
}

/* overlays/scene/actor/staged_pair_region/dialogue_layout.c */

/*
 * resource_3a6 owner at 0x02001770, complete 104-byte span through its
 * one-word pool. It installs the common window rectangle, then selects one of
 * two row layouts from story flag 0x301.
 */

void DialogueLayout_ConfigureRowsByFlag301(void)
{
    Actor_unk3_5(0, 34, 13, 3, 23, 34);

    if (Actor_unk11(0x301) != 0) {
        Actor_unk14_3(11, 35, 35);
        Actor_unk4_5(24, 34, 1, 3, 23, 34);
    } else {
        Actor_unk15_3(11, 23, 35);
        Actor_unk5_5(24, 34, 1, 3, 35, 34);
    }
}

/* overlays/scene/actor/staged_pair_region/move_and_redraw.c */
#define Value Value4

void *OvObj_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj = Actor_unk8_4(arg3, arg0, arg1, arg2);

    if (obj != NULL) {
        u8 *object = *(u8 **)(obj + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = object[9];
        mask = -mask;
        mask &= flags;
        object[9] = mask;
        obj[0x55] = 0;
        obj[0x59] = 8;
        Actor_Apply4(obj, 0);
        Actor_Apply5(obj, 14);
        Actor_Apply6(obj, 1);
        return obj;
    }
    return NULL;
}

void *OvObj_CreateConfiguredB(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
}

/* overlays/scene/actor/staged_pair_region/scene_audio.c */
extern u8 gWork[];

/*
 * resource_3a6 owner at 0x02001748, complete 40-byte span through its one-word
 * pool: play cue 123, then dispatch the signed scene value at workspace +364.
 */

void SceneAudio_PlayCue123AndDispatchWork364(void)
{
    s32 val = *(s16 *)(gWork + 364);

    Actor_unk8_2(123);
    Actor_unk9_2(val);
}

/* overlays/scene/actor/staged_pair_region/scene_data_head.c */
#define PrimaryRuntimeSelector Value_0000005d
#define SecondaryRuntimeSelector Value_0000005e
#define TertiaryRuntimeSelector Value_0000005f

s32 SceneData_SelectByRuntimeSelector(void)
{
}

/* Contiguous unnamed leaf-owner run for resource_3a6. */
signed int SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTablea1dc(void)
{
    return (u8 *)0x0200a1dc;
}

/* overlays/scene/actor/staged_pair_region/scene_data_tail.c */
#define SceneState5D Value_0000005d
#define SceneState5E Value_0000005e
#define SceneState5F Value_0000005f
#define PrimaryRuntimeSelector Value_0000005d
#define SecondaryRuntimeSelector Value_0000005e
#define TertiaryRuntimeSelector Value_0000005f

extern s16 RuntimeSelectorTable[];
extern u8 Value_0000005d;
extern u8 Value_0000005e;
extern u8 Value_0000005f;
extern u8 SceneDataA234[];
extern u8 SceneDataA2C4[];
extern u8 SceneDataA39C[];

s32 SelectSceneDataByState(void)
{
    s16 state = RuntimeSelectorTable[224];

    if (state == (s32)&SceneState5D) {
        return (s32)SceneDataA234;
    }
    if (state == (s32)&SceneState5E) {
        return (s32)SceneDataA2C4;
    }
    if (state == (s32)&SceneState5F) {
        return (s32)SceneDataA39C;
    }
    return (s32)SceneDataA234;
}

s32 SceneData_SelectSecondaryByRuntimeSelector(void)
{
}

/* overlays/scene/actor/staged_pair_region/scene_primary_script.c */

/*
 * Camera-shift beat for overlay resource_3a6.  Each Func_ name spells the
 * overlay's own relocated call word, not a runtime address.
 */

/* Mirror the preceding transition on the negative grid axis, latch its event
 * flag, wait for the map update, and close on the paired sound cue. */

s32 Scene_RunPrimarySequence(s32 a0)
{
    s32 box[3];
    u8 *rec;
    u8 *flag;
    u8 *slot;
    s32 saved;

    rec = (u8 *)Actor_Check(0);
    flag = rec + 85;
    saved = *flag;
    slot = (u8 *)box;
    *(s32 *)(slot + 0) = (*(volatile s32 *)(rec + 8) & -0x100000) + 0x80000;
    *(s32 *)(slot + 4) = *(volatile s32 *)(rec + 12);
    *(s32 *)(slot + 8) = (*(volatile s32 *)(rec + 16) & -0x100000) + 0x280000;
    if (Actor_unk2((s32)rec, (s32)slot) == 0) {
        Actor_unk10_4();
        Actor_unk11_4((s32)rec, 6);
        Actor_unk12_4(6);
        Actor_unk13_4(152);
        Actor_unk14_4((s32)rec, 7);
        *(volatile s32 *)(rec + 48) = 0x30000;
        *(volatile s32 *)(rec + 52) = 0x20000;
        *(volatile s32 *)(rec + 40) = 0x40000;
        *flag = *flag & 126;
        Actor_unk15_4((s32)rec, 0);
        Actor_Place(0, *(s16 *)(slot + 2), *(s16 *)(slot + 10));
        Actor_unk16_4((s32)rec, 6);
        Actor_unk17_4((s32)rec, 1);
        *flag = (u8)saved;
        ((void (*)())Actor_unk12)();
        return 1;
    }
    return 0;
}

void Scene_RunScene3a6SequenceA(void)
{
    u32 i;
    s32 record;

    if (Actor_unk3(0x200) == 0) {
        Actor_Do(0x200);
        Actor_unk18_4();
        Actor_Run(0x10000, 0x2000);
        Actor_unk19_4(8, 1);
        Actor_unk20_4();
        Actor_unk21_4(60);
        Actor_unk2_3(8, 0xc000, 20);
        Actor_unk2_4(8, 0x102);
        Actor_unk22_4(8, 2);
        Actor_unk23_4(20);
        Actor_unk3_3(8, 0x10000, 0x8000);
        Actor_unk4_3(8, 0x318, 248);
        Actor_unk24_4(152);
        record = Actor_unk13(8);
        *(volatile s32 *)(record + 40) = 0x80000;
        Actor_unk5_3(8, 0x318, 0x118);
        Actor_unk25_4(20);
        Actor_unk6_3(8, 0xc000, 20);
        Actor_unk26_4(30);
        Actor_unk27_4();
    }
}

void Scene_RunScene3a6SequenceB(void)
{
    u32 i;
    s32 record;

    if (Actor_unk4(0x200) != 0) {
        if (Actor_unk5(0x201) == 0) {
            Actor_unk2_2(0x201);
            Actor_unk3_2(0x302);
            Actor_unk28_4();
            Actor_unk3_4(8, 0x102);
            Actor_unk29_4(8, 2);
            Actor_unk30_4(20);
            Actor_unk7_3(8, 0x20000, 0x10000);
            Actor_unk8_3(8, 0x2f8, 0x118);
            Actor_unk9_3(8, 0x2f8, 0x138);
            Actor_unk10_3(8, 0x318, 0x138);
            Actor_unk31_4(10);
            Actor_unk11_3(8, 0xc000, 20);
            record = Actor_unk14(8);
            *(volatile s32 *)(record + 108) = 0x2008cf9;
            Actor_unk4_4();
        }
    }
}

void Scene_RunScene3a6SequenceC(void)
{
    s32 rec8;
    s32 record;
    s32 idx;
    s32 tbl;
    s32 idx4;
    s32 off24a;
    u8 *p5;

    p5 = *(volatile s32 *)gWork;
    if (Actor_unk6(0x302) != 0) {
        off24a = 0x24a;
        if (*(s16 *)((s32)RuntimeSelectorTable + off24a) != 8) {
            idx = *(s16 *)((s32)p5 + 0x16c);
            rec8 = Actor_unk7(8);
            record = Actor_unk8(0);
            *(volatile s32 *)(rec8 + 48) = *(volatile s32 *)(record + 48);
            rec8 = Actor_unk9(8);
            record = Actor_unk10(0);
            *(volatile s32 *)(rec8 + 52) = *(volatile s32 *)(record + 52);
            idx -= 45;
            tbl = 0x02009f00;
            idx <<= 3;
            idx4 = idx + 4;
            Actor_unk32_4(8, *(volatile s32 *)(tbl + idx), *(volatile s32 *)(tbl + idx4));
        }
    }
}

void Scene_RunScene3a6(void)
{
    u32 i;
    s32 record;
    s32 zero;

    Actor_unk33_4();
    Actor_unk34_4(10);
    Actor_unk12_3(0, 0x8000, 0x1999);
    Actor_unk35_4(0, 8);
    Actor_unk36_4(15);
    Actor_unk37_4(0, 8, 0);
    Actor_unk38_4(4);
    Actor_unk4_2(0x120);
    Actor_unk39_4(239);
    Actor_unk13_3(9, 0x8000, 0x1999);
    Actor_unk40_4(9, 2);
    zero = 0;
    *(u8 *)(Actor_unk15(9) + 85) = zero;
    record = Actor_unk16(9);
    *(volatile s32 *)(record + 68) = zero;
    Actor_unk41_4(9, 12, 0);
    Actor_unk42_4(0);
    Actor_unk43_4(0, 1);
    Actor_unk44_4(9);
    Actor_unk5_2(0x120);
    Actor_unk45_4(213);
    Actor_unk46_4(9, 3);
    *(u8 *)(Actor_unk17(9) + 85) = 3;
    Actor_unk47_4(9, 6, 0);
    Actor_unk48_4(9);
    Actor_unk49_4();
    Actor_unk50_4(9, 8);
    Actor_unk51_4(9, 3);
    *(u8 *)(Actor_unk18(9) + 35) = 2;
    Actor_SetRect(0, 12, 16, 1, 4, 0);
    Actor_unk2_5(0, 13, 16, 1, 4, 0);
    Actor_unk6_2(0x202);
    Actor_unk52_4(240);
    Actor_unk53_4();
}

/*
 * The 54-byte owner at 0x020018b4 includes its two pool words: 0x301 and
 * 0x121 are identifiers passed as arguments, never dereferenced as
 * addresses.  0x301 is this scene's event flag.  The two shift calls carry
 * a displacement and its opposite, not two unrelated magnitudes.
 */
void Scene_RunShiftAndSetFlag301(void)
{
    Actor_unk54_4(241);
    Actor_unk55_4(11, 112, 0);
    Actor_unk56_4(11, 80, 0);
    Actor_unk57_4(0x301);
    Actor_unk58_4(2);
    Actor_unk59_4();
    Actor_unk60_4(0x121);
}

void Scene_RunActor11Transition301(void)
{
    Actor_unk10_2(241);
    Actor_unk16_3(11, -112, 0);
    Actor_unk17_3(11, -80, 0);
    Actor_unk11_2(0x301);
    Actor_unk12_2(2);
    Actor_unk61_4();
    Actor_unk13_2(0x121);
}

/* overlays/scene/actor/staged_pair_region/scene_setup.c */

/* Contiguous unnamed leaf-owner run for resource_3a6. */

/* Configure the 16x15 scene rectangle at row 15. */
void Scene_ConfigureRegionAtRow15(void)
{
    Actor_unk62_4(16, 15, 1, 1, 15, 15);
}

/* Configure the matching 16x15 scene rectangle at row 17. */
void Scene_ConfigureRegionAtRow17(void)
{
    Actor_unk63_4(16, 17, 1, 1, 15, 15);
}

/*
 * resource_3a6 owner at 0x020010c8, 24 bytes: open the scene scheduler,
 * initialize it, close it, then run the preceding tile-27 rain sequence.
 */
void Scene_RunInitBracketThenSequence(void)
{
    Actor_unk64_4();
    Actor_unk65_4();
    Actor_unk66_4();
    Actor_unk67_4();
}

/* Actor-8 presentation reset at 0x02001378, including alignment to 0x1390. */
void Scene_RunActor8ZeroStep(void)
{
    Actor_unk68_4();
    Actor_Apply7(8, 0);
    Actor_unk69_4();
}

/* overlays/scene/actor/staged_pair_region/staged_actor_pair.c */

typedef struct { s32 unk0; s32 unk4; s32 unk8; } Desc;

typedef struct { u8 filler0[0x28]; s16 *unk28; } Sub;

typedef struct {
    u8 filler0[6];
    u16 unk6;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 filler14[0x10];
    s32 unk24;
    u8 filler28[4];
    s32 unk2C;
    u8 filler30[8];
    s32 unk38;
    u8 filler3C[4];
    s32 unk40;
    u8 filler44[0xC];
    Sub *unk50;
} Ent;

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} Obj;

extern s32 gOv[];
extern s32 Actor_Far[];

struct StagedActor *GetStagedActor(s32 arg0);
struct StagedActor *FindNextStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindElevatedBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);

Ent *Actor_unk70_4(Desc *, Ent *);

Obj *Actor_unk71_4();
Obj *Actor_unk72_4();
Obj *Actor_unk73_4(s32);

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
}

void AdvanceStagedActorPair(void)
{
    extern u32 Actor_Far2[];

    s32 destination[3];
    struct StagedActor *lead_actor;
    struct StagedActor *next_actor;
    struct StagedActor *blocking_actor;
    s32 facing_index;
    u32 step;
    s32 move_rate;
    s32 zero;

    lead_actor = GetStagedActor(0);
    facing_index = lead_actor->direction_and_kind >> 12;
    step = StagedActorStepTable[facing_index];
    destination[0] = lead_actor->x.value + (step & 0xffff0000);
    destination[1] = lead_actor->y;
    step <<= 16;
    destination[2] = lead_actor->z.value + step;
    next_actor = FindNextStagedActor(destination, lead_actor);
    if (next_actor == 0) return;

    step = StagedActorStepTable[facing_index];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    blocking_actor = FindBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    destination[0] = next_actor->x.value;
    destination[1] = next_actor->y + 0x100000;
    destination[2] = next_actor->z.value;
    blocking_actor = FindElevatedBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    next_actor->transition_mode = 2;
    step = StagedActorStepTable[facing_index];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    if (CanStartStagedActorMove(next_actor, destination) > 0) return;

    zero = next_actor->transition_busy;
    if (zero != 0) return;

    SetStagedActorMode(lead_actor, 8);
    move_rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next_actor->move_rate_x = move_rate;
    next_actor->move_rate_z = move_rate;
    StartNextStagedActorMove(next_actor, destination[0], destination[1], destination[2]);
    lead_actor->move_rate_x = move_rate;
    lead_actor->move_rate_z = move_rate;
    StartLeadStagedActorMove(lead_actor, destination[0], destination[1], destination[2]);
    FinishStagedActorMove(next_actor);
    FinishStagedActorEffect();
    next_actor->x.value = destination[0];
    next_actor->z.value = destination[2];
    next_actor->unknown_24 = zero;
    next_actor->unknown_2c = zero;
    lead_actor->unknown_38 = 0x80000000;
    lead_actor->unknown_40 = 0x80000000;
    lead_actor->unknown_24 = zero;
    lead_actor->unknown_2c = zero;
    lead_actor->x.value = lead_actor->x.parts.cell << 16;
    lead_actor->z.value = lead_actor->z.parts.cell << 16;
    SetStagedActorTransition(lead_actor, 1);
}

s32 stop_blocked_actor_motion(Ent *a)
{
    extern s32 Actor_Far2[];

    Desc d;
    u32 idx;
    s32 m;
    Ent *r;

    idx = a->unk6 >> 12;
    m = Actor_Far2[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    r = Actor_unk70_4(&d, a);
    if (r != 0) {
        u32 i = 0;
        s32 v = *r->unk50->unk28;
        s32 *p = gOv;

        do {
            if (v == *p++) goto done;
            i++;
        } while (i <= 5);
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
    m = Actor_Far2[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    if (Actor_EntOp(a, &d) > 0) {
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
done:
    return 0;
}

s32 find_clear_actor_position(s32 *a)
{
    extern s32 Actor_Far2[];

    s32 sel;
    s32 buf[3];
    u8 *obj;
    s32 cnt;
    s32 ny, nx;
    u8 *p;
    s32 off;
    s32 t, u;
    u8 *E;
    s32 *b;
    s32 ret;
    a[5] = 0;
    obj = (u8 *)Actor_unk18_3(&sel, a + 1, a);
    if (obj == 0)
        return 0;
    p = obj + 0x22;
    *p = 2;
    cnt = 0;
    E = (u8 *)Actor_Far;
    off = a[0] << 4;
    {
        s32 o = off + 4;
        t = *(s32 *)(E + o);
        if (t < 0)
            t = -t;
        o = off;
        o += 12;
        u = *(s32 *)(E + o);
        if (u < 0)
            u = -u;
        ny = (t + u) >> 4;
        t = *(s32 *)(E + off);
        if (t < 0)
            t = -t;
        o = off;
        o += 8;
        u = *(s32 *)(E + o);
        if (u < 0)
            u = -u;
        nx = (t + u) >> 4;
    }
    {
        u8 *oc;
        s32 w;
        s32 d;
        b = buf;
        d = Actor_Far2[sel] & 0xffff0000;
        oc = obj;
        b[0] = *(s32 *)(oc + 8) + d;
        w = *(s32 *)(oc + 12);
        b[1] = w;
        b[2] = *(s32 *)(oc + 16) + (Actor_Far2[sel] << 16);
        a[3] = w;
    }
    for (;;) {
        s32 j, k;
        {
            u8 *E2 = (u8 *)Actor_Far;
            s32 o2 = a[0] << 4;
            o2 += 4;
            a[4] = b[2] + (*(s32 *)(E2 + o2) << 16);
        }
        j = 0;
        while (j < ny) {
            {
                u8 *E3 = (u8 *)Actor_Far;
                a[2] = b[0] + (*(s32 *)(E3 + (a[0] << 4)) << 16);
            }
            k = 0;
            while (k < nx) {
                s32 *w2 = a + 2;
                if (Actor_Apply8(obj, w2) == 2)
                    goto found;
                w2[0] = w2[0] + 0x100000;
                k++;
            }
            a[4] = a[4] + 0x100000;
            j++;
        }
        cnt++;
        buf[0] = buf[0] + (Actor_Far2[sel] & 0xffff0000);
        buf[2] = buf[2] + (Actor_Far2[sel] << 16);
    }
found:
    *p = 0;
    ret = 0;
    if (cnt != 0) {
        s32 d = Actor_Far2[sel];
        s32 m = d & 0xffff0000;
        s32 v1 = m *cnt;
        s32 v2 = (d << 16) * cnt;
        u8 *oc = obj;
        a[2] = *(s32 *)(oc + 8) + v1;
        a[3] = *(s32 *)(oc + 12);
        a[4] = *(s32 *)(oc + 16) + v2;
        ret = 1;
    }
    return ret;
}

void position_actor_pair(s32 a0, s32 a1, s32 a2)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Actor_unk71_4(RuntimeSelectorTable[125]);
    q = Actor_unk73_4(a0);
    Actor_unk77_4();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Actor_SetMode(p, x, p->f0c, y);
    }
    Actor_Apply9(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Actor_SetMode2(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Actor_unk72_4(q, 4);
    } else {
        Actor_Apply10(q, 3);
    }
    Actor_unk14_2(p);
    Actor_unk78_4();
}

/* overlays/scene/actor/staged_pair_region/state_update.c */

/* Open the three-by-four presentation block used by this resource. */

void State_SetValues8_3_4(void)
{
    Actor_unk19_3(8, 3, 4);
}
