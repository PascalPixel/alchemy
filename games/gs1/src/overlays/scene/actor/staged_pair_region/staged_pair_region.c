#include "types.h"
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
#define Actor_FaceActorZero Func_02000cf8
u8 *Func_020029bc(s32 no);
s32 Func_0200293e(s32 dz, s32 dx);

/* Point an object toward actor zero using their fixed-point X/Z delta. */

s32 Actor_FaceActorZero(u8 *obj)
{
    u8 *target = Func_020029bc(0);
    s32 dz = *(s32 *)(target + 16) - *(s32 *)(obj + 16);
    s32 dx = *(s32 *)(target + 8) - *(s32 *)(obj + 8);

    *(s16 *)(obj + 6) = (s16)Func_0200293e(dz, dx);
    return 0;
}
#undef Actor_FaceActorZero

/* overlays/scene/actor/staged_pair_region/actor_motion.c */
#define Actor_WaitActorDescent Func_02000cd0
void Func_020028fa(s32 cnt);

/* Give the actor at most sixty frames to descend to its target height, then
 * clamp the live height to the target so the following scene starts exact. */

void Actor_WaitActorDescent(u8 *obj)
{
    s32 cnt = 60;

    while (cnt != 0) {
        Func_020028fa(1);
        cnt--;
        if (*(s32 *)(obj + 12) <= *(s32 *)(obj + 20))
            break;
    }
    *(s32 *)(obj + 12) = *(s32 *)(obj + 20);
}
#undef Actor_WaitActorDescent

/* overlays/scene/actor/staged_pair_region/actor_position.c */
#define Actor_PlaceAtTile Func_02001938
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

struct Rec_3a6 *Func_02003602();
void Func_02003670();

void Actor_PlaceAtTile(s32 id, s32 x, s32 y)
{
    struct Rec_3a6 *rec = Func_02003602(id);

    if (rec != 0) {
        Func_02003670(id, 3);
        rec->f34 = 2;
        rec->f35 |= 2;
        rec->f8 = (x << 20) + 0x80000;
        rec->f16 = (y << 20) + 0x80000;
    }
}
#undef Actor_PlaceAtTile

/* overlays/scene/actor/staged_pair_region/actor_presentation.c */
#define ActorDraw_AdvanceActorElevenStates Func_02001bfc
void Func_02003900(s32 actor, s32 state);
void Func_02003908(s32 actor, s32 state);

/* Advance actor eleven through the two presentation states used at scene end. */

void ActorDraw_AdvanceActorElevenStates(void)
{
    Func_02003900(11, 1);
    Func_02003908(11, 2);
}
#undef ActorDraw_AdvanceActorElevenStates

/* overlays/scene/actor/staged_pair_region/dialogue_layout.c */
#define DialogueLayout_ConfigureRowsByFlag301 Func_02001770
void Func_020033f2(s32 left, s32 top, s32 width, s32 height,
                          s32 right, s32 bottom);
s32 Func_02003418(s32 flagId);
void Func_020030d2(s32 width, s32 top, s32 bottom);
void Func_02003416(s32 left, s32 top, s32 width, s32 height,
                          s32 right, s32 bottom);
void Func_020030ee(s32 width, s32 top, s32 bottom);
void Func_02003434(s32 left, s32 top, s32 width, s32 height,
                          s32 right, s32 bottom);

/*
 * resource_3a6 owner at 0x02001770, complete 104-byte span through its
 * one-word pool. It installs the common window rectangle, then selects one of
 * two row layouts from story flag 0x301.
 */

void DialogueLayout_ConfigureRowsByFlag301(void)
{
    Func_020033f2(0, 34, 13, 3, 23, 34);

    if (Func_02003418(0x301) != 0) {
        Func_020030d2(11, 35, 35);
        Func_02003416(24, 34, 1, 3, 23, 34);
    } else {
        Func_020030ee(11, 23, 35);
        Func_02003434(24, 34, 1, 3, 35, 34);
    }
}
#undef DialogueLayout_ConfigureRowsByFlag301

/* overlays/scene/actor/staged_pair_region/move_and_redraw.c */
#define Data_0200e1e8 Data_02009ea0
#define Data_0200e190 Data_02009e48
#define Value_020082a9 Value_0200858d
#define Func_02000902 Func_02000eca
#define Func_02000a6e Func_02001036
#define Func_02000a80 Func_02001048
#define Func_02000acc Func_02001094
#define Func_02006610 Func_020025cc
#define Func_0200661c Func_020025d8
#define Func_020066b4 Func_02002670
#define Func_02006714 Func_020026b0
#define Func_0200668a Func_0200265e
#define Func_0200672c Func_020026c8
#define Func_020066ea Func_020026a6
#define Func_020066c4 Func_02002698
#define RefreshStagedActor Func_020026e0
#define Func_02006614 Func_02002638
#define Func_0200661e Func_02002642
#define Func_0200687c Func_02002770_a
#define Func_02006658 Func_02002674
#define Func_02006776 Func_02002712
#define Func_0200678e Func_0200272a
#define Func_02006740 Func_020026fc
#define Func_020067a4 Func_02002740
#define Func_020067bc Func_02002758
#define Func_020067d4 Func_02002770_b
#define Func_0200677a Func_02002736
#define Func_020068f4 Func_020027e8
#define Func_020068fa Func_020027ee
#define Func_020066b8 Func_020026dc
#define Func_02006752 Func_02002766
#define Func_020067ae Func_020027c2
#define Func_020069d0 Func_020028c4



void Func_020008ec(StagedActorMovementRequest request)
{
}
#undef Data_0200e1e8
#undef Data_0200e190
#undef Value_020082a9
#undef Func_02000902
#undef Func_02000a6e
#undef Func_02000a80
#undef Func_02000acc
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

/* overlays/scene/actor/staged_pair_region/overlay_object.c */
#define NULL ((void *)0)
#define CreateOverlayObject Func_02001cf6
#define SetOverlayObjectMode Func_02001d58
#define SetOverlayObjectSlot Func_02001df0
#define OvObj_CreateConfigured Func_02000048
#define OvObj_CreateConfiguredB Func_020000a0


void *Func_02001c9e(s32, s32, s32, s32);
void Func_02001cfc(void *, s32);
void Func_02001d14(void *, s32);
void Func_02001d94(void *, s32);

void *OvObj_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj = Func_02001c9e(arg3, arg0, arg1, arg2);

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
        Func_02001cfc(obj, 0);
        Func_02001d94(obj, 14);
        Func_02001d14(obj, 1);
        return obj;
    }
    return NULL;
}

void *OvObj_CreateConfiguredB(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
}
#undef NULL
#undef CreateOverlayObject
#undef SetOverlayObjectMode
#undef SetOverlayObjectSlot
#undef OvObj_CreateConfigured
#undef OvObj_CreateConfiguredB

/* overlays/scene/actor/staged_pair_region/scene_audio.c */
#define SceneAudio_PlayCue123AndDispatchWork364 Func_02001748
extern u8 *Data_03001ebc;

void Func_020034b8(s32 cue);
void Func_020034a6(s32 val);

/*
 * resource_3a6 owner at 0x02001748, complete 40-byte span through its one-word
 * pool: play cue 123, then dispatch the signed scene value at workspace +364.
 */

void SceneAudio_PlayCue123AndDispatchWork364(void)
{
    s32 val = *(s16 *)(Data_03001ebc + 364);

    Func_020034b8(123);
    Func_020034a6(val);
}
#undef SceneAudio_PlayCue123AndDispatchWork364

/* overlays/scene/actor/staged_pair_region/scene_data_head.c */
#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_0000005d
#define SecondaryRuntimeSelector Value_0000005e
#define TertiaryRuntimeSelector Value_0000005f
#define PrimaryOverlayData Data_02009f9c
#define SecondaryOverlayData Data_0200a014
#define TertiaryOverlayData Data_0200a134
#define DefaultOverlayData Data_02009f6c
#define SceneData_SelectByRuntimeSelector Func_02000d20
#define SceneData_ReturnZero Func_02000d74
#define SceneData_GetTablea1dc Func_02000d78


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
#undef SceneData_GetTablea1dc

/* overlays/scene/actor/staged_pair_region/scene_data_tail.c */
#define SceneState5D Value_0000005d
#define SceneState5E Value_0000005e
#define SceneState5F Value_0000005f
#define SceneDataA234 Data_0200a234
#define SceneDataA2C4 Data_0200a2c4
#define SceneDataA39C Data_0200a39c
#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_0000005d
#define SecondaryRuntimeSelector Value_0000005e
#define TertiaryRuntimeSelector Value_0000005f
#define PrimaryOverlayData Data_0200a420
#define SecondaryOverlayData Data_0200a450
#define TertiaryOverlayData Data_0200a624
#define DefaultOverlayData Data_0200a414
#define SelectSceneDataByState Func_02000d80
#define SceneData_SelectSecondaryByRuntimeSelector Func_0200169c


extern s16 Data_02000240[];
extern u8 Value_0000005d;
extern u8 Value_0000005e;
extern u8 Value_0000005f;
extern u8 Data_0200a234[];
extern u8 Data_0200a2c4[];
extern u8 Data_0200a39c[];

s32 SelectSceneDataByState(void)
{
    s16 state = Data_02000240[224];

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
#undef SceneState5D
#undef SceneState5E
#undef SceneState5F
#undef SceneDataA234
#undef SceneDataA2C4
#undef SceneDataA39C
#undef RuntimeSelectorTable
#undef PrimaryRuntimeSelector
#undef SecondaryRuntimeSelector
#undef TertiaryRuntimeSelector
#undef PrimaryOverlayData
#undef SecondaryOverlayData
#undef TertiaryOverlayData
#undef DefaultOverlayData
#undef SelectSceneDataByState
#undef SceneData_SelectSecondaryByRuntimeSelector

/* overlays/scene/actor/staged_pair_region/scene_primary_script.c */
#define Scene_RunScene3a6SequenceA Func_020011a0
#define Scene_RunScene3a6SequenceB Func_02001258
#define Scene_RunScene3a6SequenceC Func_02001300
#define FieldScene_RunScene3a6_020014ac Func_020014ac
#define Scene_RunShiftAndSetFlag301 Func_020018b4
#define Scene_RunActor11Transition301 Func_020018f4
extern u8 Data_03001ebc[];
extern u8 Data_02000240[];

void Func_02002d4e();
void Func_02002d60();
void Func_02002d74();
s32 Func_02002d90();
s32 Func_02002da8();
void Func_02002dac();
void Func_02002dd0();
void Func_02002dde();
void Func_02002dfc();
s32 Func_02002e3c_a();
s32 Func_02002e3c_b();
void Func_02002e94();
s32 Func_02002e34();
void Func_02002e48();
void Func_02002e64();
void Func_02002e7a();
void Func_02002e9e();
void Func_02002ecc();
s32 Func_02002edc();
void Func_02002ede();
void Func_02002ef0_a();
void Func_02002ef0_b();
void Func_02002ef6();
void Func_02002ef8_a();
void Func_02002ef8_b();
void Func_02002ef8_c();
void Func_02002f04();
void Func_02002f0c();
void Func_02002f10_a();
void Func_02002f10_b();
void Func_02002f5a();
void Func_02002f76();
s32 Func_02002eec();
s32 Func_02002ef6_a();
void Func_02002f08();
void Func_02002f0e();
void Func_02002f2a();
void Func_02002f3a();
void Func_02002f68();
void Func_02002f78();
void Func_02002f8e();
void Func_02002f94();
void Func_02002f9c();
s32 Func_02002fa2_a();
s32 Func_02002fa2_b();
void Func_02002faa();
void Func_02002fac();
void Func_02002ff4();
s32 Func_02002f96();
s32 Func_02002fe8();
s32 Func_02002ff0();
s32 Func_02002ffa();
s32 Func_02003002();
void Func_02003030();
s32 Func_02001ac2();
void Func_02001ad2();
void Func_0200223c();
void Func_0200315c();
void Func_0200315e();
void Func_02003176();
void Func_02003186();
void Func_02003188();
void Func_020031c0_a();
void Func_020031c0_b();
s32 Func_020031c6();
void Func_020031c8();
s32 Func_020031d2();
void Func_02003200();
void Func_02003206();
s32 Func_0200320e();
void Func_02003214();
void Func_02003222();
void Func_02003224();
void Func_0200322c();
s32 Func_0200323e();
void Func_02003244();
void Func_02003246_a();
void Func_02003246_b();
void Func_02003248();
void Func_0200324c();
void Func_0200326e();
void Func_02003270();
void Func_02003298();
void Func_0200329a();
void Func_020032a0();
void Func_02003312();
void Func_02003616();
void Func_0200309c();
void Func_020030a6();
void Func_02003568();
void Func_020034f6();
void Func_0200304e();
void Func_02003640();
void Func_02003656(s32 soundId);
void Func_020030de(s32 actorId, s32 cellX, s32 cellZ);
void Func_020030ea(s32 actorId, s32 cellX, s32 cellZ);
void Func_020035b4(s32 flagId);
void Func_0200353a(s32 frames);
void Func_02003092(void);
void Func_02003684(s32 soundId);

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/*
 * Camera-shift beat for overlay resource_3a6.  Each Func_ name spells the
 * overlay's own relocated call word, not a runtime address.
 */

/* Mirror the preceding transition on the negative grid axis, latch its event
 * flag, wait for the map update, and close on the paired sound cue. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1_020011a0(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

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
static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1_02001258(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1_02001258(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return f(a0, a1, a2, a3, a4, a5);
}

s32 Func_020010e0(s32 a0)
{
    s32 box[3];
    u8 *rec;
    u8 *flag;
    u8 *slot;
    s32 saved;

    rec = (u8 *)Value1(Func_02002da8, 0);
    flag = rec + 85;
    saved = *flag;
    slot = (u8 *)box;
    *(s32 *)(slot + 0) = (*(volatile s32 *)(rec + 8) & -0x100000) + 0x80000;
    *(s32 *)(slot + 4) = *(volatile s32 *)(rec + 12);
    *(s32 *)(slot + 8) = (*(volatile s32 *)(rec + 16) & -0x100000) + 0x280000;
    if (Value2(Func_02002d90, (s32)rec, (s32)slot) == 0) {
        Func_02002dd0();
        Func_02002d60((s32)rec, 6);
        Func_02002d4e(6);
        Func_02002e94(152);
        Func_02002d74((s32)rec, 7);
        *(volatile s32 *)(rec + 48) = 0x30000;
        *(volatile s32 *)(rec + 52) = 0x20000;
        *(volatile s32 *)(rec + 40) = 0x40000;
        *flag = *flag & 126;
        Func_02002dde((s32)rec, 0);
        Value3(Func_02002e3c_a, 0, *(s16 *)(slot + 2), *(s16 *)(slot + 10));
        Func_02002dac((s32)rec, 6);
        Func_02002dfc((s32)rec, 1);
        *flag = (u8)saved;
        ((void (*)())Func_02002e3c_b)();
        return 1;
    }
    return 0;
}

void Scene_RunScene3a6SequenceA(void)
{
    u32 i;
    s32 record;

    if (Value1_020011a0(Func_02002e34, 0x200) == 0) {
        Call1(Func_02002e48, 0x200);
        Func_02002e64();
        Call2(Func_02002ef8_a, 0x10000, 0x2000);
        Func_02002ef8_b(8, 1);
        Func_02002f0c();
        Func_02002e7a(60);
        Call3(Func_02002ef6, 8, 0xc000, 20);
        Call2(Func_02002f10_a, 8, 0x102);
        Func_02002ef8_c(8, 2);
        Func_02002e9e(20);
        Call3(Func_02002ecc, 8, 0x10000, 0x8000);
        Call3(Func_02002ef0_a, 8, 0x318, 248);
        Func_02002f76(152);
        record = Func_02002edc(8);
        *(volatile s32 *)(record + 40) = 0x80000;
        Call3(Func_02002f10_b, 8, 0x318, 0x118);
        Func_02002ede(20);
        Call3(Func_02002f5a, 8, 0xc000, 20);
        Func_02002ef0_b(30);
        Func_02002f04();
    }
}

void Scene_RunScene3a6SequenceB(void)
{
    u32 i;
    s32 record;

    if (Value1_02001258(Func_02002eec, 0x200) != 0) {
        if (Value1_02001258(Func_02002ef6_a, 0x201) == 0) {
            Call1_02001258(Func_02002f08, 0x201);
            Call1_02001258(Func_02002f0e, 0x302);
            Func_02002f2a();
            Call2(Func_02002fac, 8, 0x102);
            Func_02002f94(8, 2);
            Func_02002f3a(20);
            Call3(Func_02002f68, 8, 0x20000, 0x10000);
            Call3(Func_02002f8e, 8, 0x2f8, 0x118);
            Call3(Func_02002f9c, 8, 0x2f8, 0x138);
            Call3(Func_02002faa, 8, 0x318, 0x138);
            Func_02002f78(10);
            Call3(Func_02002ff4, 8, 0xc000, 20);
            record = Func_02002fa2_a(8);
            *(volatile s32 *)(record + 108) = 0x2008cf9;
            Call0((void (*)())Func_02002fa2_b);
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

    p5 = *(volatile s32 *)Data_03001ebc;
    if (Value1(Func_02002f96, 0x302) != 0) {
        off24a = 0x24a;
        if (*(s16 *)((s32)Data_02000240 + off24a) != 8) {
            idx = *(s16 *)((s32)p5 + 0x16c);
            rec8 = Value1(Func_02002fe8, 8);
            record = Value1(Func_02002ff0, 0);
            *(volatile s32 *)(rec8 + 48) = *(volatile s32 *)(record + 48);
            rec8 = Value1(Func_02002ffa, 8);
            record = Value1(Func_02003002, 0);
            *(volatile s32 *)(rec8 + 52) = *(volatile s32 *)(record + 52);
            idx -= 45;
            tbl = 0x02009f00;
            idx <<= 3;
            idx4 = idx + 4;
            Func_02003030(8, *(volatile s32 *)(tbl + idx), *(volatile s32 *)(tbl + idx4));
        }
    }
}

void FieldScene_RunScene3a6_020014ac(void)
{
    u32 i;
    s32 record;
    s32 zero;

    Func_0200315e();
    Func_0200315c(10);
    Call3(Func_02003188, 0, 0x8000, 0x1999);
    Func_020031c8(0, 8);
    Func_02003176(15);
    Func_020031c0_a(0, 8, 0);
    Func_02003186(4);
    Call1(Func_02003246_a, 0x120);
    Func_0200324c(239);
    Call3(Func_020031c0_b, 9, 0x8000, 0x1999);
    Func_02003200(9, 2);
    zero = 0;
    *(u8 *)(Func_020031c6(9) + 85) = zero;
    record = Func_020031d2(9);
    *(volatile s32 *)(record + 68) = zero;
    Func_02003206(9, 12, 0);
    Func_02003214(0);
    Func_0200322c(0, 1);
    Func_02003222(9);
    Call1(Func_0200329a, 0x120);
    Func_020032a0(213);
    Func_02003248(9, 3);
    *(u8 *)(Func_0200320e(9) + 85) = 3;
    Func_02003246_b(9, 6, 0);
    Func_02003224(9);
    Func_0200223c();
    Func_02003270(9, 8);
    Func_02003298(9, 3);
    *(u8 *)(Func_0200323e(9) + 35) = 2;
    Value6(Func_02001ac2, 0, 12, 16, 1, 4, 0);
    Call6(Func_02001ad2, 0, 13, 16, 1, 4, 0);
    Call1(Func_02003244, 0x202);
    Func_02003312(240);
    Func_0200326e();
}

/*
 * The 54-byte owner at 0x020018b4 includes its two pool words: 0x301 and
 * 0x121 are identifiers passed as arguments, never dereferenced as
 * addresses.  0x301 is this scene's event flag.  The two shift calls carry
 * a displacement and its opposite, not two unrelated magnitudes.
 */
void Scene_RunShiftAndSetFlag301(void)
{
    Func_02003616(241);
    Func_0200309c(11, 112, 0);
    Func_020030a6(11, 80, 0);
    Func_02003568(0x301);
    Func_020034f6(2);
    Func_0200304e();
    Func_02003640(0x121);
}

void Scene_RunActor11Transition301(void)
{
    Func_02003656(241);
    Func_020030de(11, -112, 0);
    Func_020030ea(11, -80, 0);
    Func_020035b4(0x301);
    Func_0200353a(2);
    Func_02003092();
    Func_02003684(0x121);
}
#undef Scene_RunScene3a6SequenceA
#undef Scene_RunScene3a6SequenceB
#undef Scene_RunScene3a6SequenceC
#undef FieldScene_RunScene3a6_020014ac
#undef Scene_RunShiftAndSetFlag301
#undef Scene_RunActor11Transition301

/* overlays/scene/actor/staged_pair_region/scene_setup.c */
#define Scene_ConfigureRegionAtRow15 Func_02000dcc
#define Scene_ConfigureRegionAtRow17 Func_02000de8
#define Scene_RunInitBracketThenSequence Func_020010c8
#define Scene_RunActor8ZeroStep Func_02001378
void Func_02002a44();
void Func_02002a60();
void Func_02002d78();
void Func_02001478();
void Func_02002d88();
void Func_02002050();
void Func_02003028(void);
void Func_02003080(s32 actor, s32 mode);
void Func_0200303c(void);

/* Contiguous unnamed leaf-owner run for resource_3a6. */

/* Configure the 16x15 scene rectangle at row 15. */
void Scene_ConfigureRegionAtRow15(void)
{
    Func_02002a44(16, 15, 1, 1, 15, 15);
}

/* Configure the matching 16x15 scene rectangle at row 17. */
void Scene_ConfigureRegionAtRow17(void)
{
    Func_02002a60(16, 17, 1, 1, 15, 15);
}

/*
 * resource_3a6 owner at 0x020010c8, 24 bytes: open the scene scheduler,
 * initialize it, close it, then run the preceding tile-27 rain sequence.
 */
void Scene_RunInitBracketThenSequence(void)
{
    Func_02002d78();
    Func_02001478();
    Func_02002d88();
    Func_02002050();
}

/* Actor-8 presentation reset at 0x02001378, including alignment to 0x1390. */
void Scene_RunActor8ZeroStep(void)
{
    Func_02003028();
    Func_02003080(8, 0);
    Func_0200303c();
}
#undef Scene_ConfigureRegionAtRow15
#undef Scene_ConfigureRegionAtRow17
#undef Scene_RunInitBracketThenSequence
#undef Scene_RunActor8ZeroStep

/* overlays/scene/actor/staged_pair_region/staged_actor_pair.c */
#define EffectDescriptorTable Data_02009f60
#define GetPartyEffect Func_02001e1a
#define SpawnEffect Func_02001dc8
#define SetEffectVariant Func_02001dd2
#define SetEffectDescriptor Func_02001dec
#define SetEffectMode Func_02001f26
#define ScaleEffectDeltaFromAccumulated Func_02001e90
#define ScaleEffectDeltaFromOrigin Func_02001ea8
#define ScaleEffectVerticalDelta Func_02001eb6
#define SetEffectCallbackMode Func_02001eec
#define SetEffectCallbackArgument Func_02001efc
#define StagedActorStepTable Data_02009e48
#define GetStagedActor Func_02002074
#define FindNextStagedActor Func_0200073e
#define FindBlockingStagedActor Func_0200076a
#define FindElevatedBlockingStagedActor Func_02000796
#define CanStartStagedActorMove Func_020020f8
#define SetStagedActorMode Func_020020d0
#define SelectStagedActorSlot Func_020020c0
#define StartStagedActorEffect Func_02002206
#define StartNextStagedActorMove Func_0200210e
#define StartLeadStagedActorMove Func_0200211e
#define FinishStagedActorMove Func_0200212c
#define FinishStagedActorEffect Func_02002228
#define SetStagedActorTransition Func_0200213c
#define SpawnConfiguredEffect Func_0200013c
#define AdvanceStagedActorPair Func_020003a8


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

extern s32 Data_02009e88[];
extern s32 Data_02009ea0[];
extern s32 Data_02000240[];

struct StagedActor *Func_02002074(s32 arg0);
struct StagedActor *Func_0200073e(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_0200076a(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_02000796(s32 *arg0, struct StagedActor *arg1);
s32 Func_020020f8(struct StagedActor *arg0, s32 *arg1);
void Func_020020d0(struct StagedActor *arg0, s32 arg1);
void Func_020020c0(s32 arg0);
void Func_02002206(s32 arg0);
void Func_0200210e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_0200211e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_0200212c(struct StagedActor *arg0);
void Func_02002228(void);
void Func_0200213c(struct StagedActor *arg0, s32 arg1);
Ent *Func_0200090a(Desc *, Ent *);
s32 Func_0200227a(Ent *, Desc *);
s32 Func_02000da6(s32 *, s32 *, s32 *);
s32 Func_020024b4(u8 *, s32 *);
Obj *Func_020034b6_a();
Obj *Func_020034b6_b();
Obj *Func_020034be(s32);
void Func_020034b4(void);
void Func_02003496(Obj *, s32, s32, s32);
void Func_0200347e(Obj *, s32);
void Func_020034c2(Obj *, s32, s32, s32);
void Func_020034c0(Obj *, s32);
void Func_020034ee(Obj *);
void Func_0200354a(void);

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
}

void AdvanceStagedActorPair(void)
{
    extern u32 Data_02009e48[];

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

s32 Func_0200058c(Ent *a)
{
    extern s32 Data_02009e48[];

    Desc d;
    u32 idx;
    s32 m;
    Ent *r;

    idx = a->unk6 >> 12;
    m = Data_02009e48[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    r = Func_0200090a(&d, a);
    if (r != 0) {
        u32 i = 0;
        s32 v = *r->unk50->unk28;
        s32 *p = Data_02009e88;

        do {
            if (v == *p++) goto done;
            i++;
        } while (i <= 5);
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
    m = Data_02009e48[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    if (Func_0200227a(a, &d) > 0) {
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
done:
    return 0;
}

s32 Func_02000758(s32 *a)
{
    extern s32 Data_02009e48[];

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
    obj = (u8 *)Func_02000da6(&sel, a + 1, a);
    if (obj == 0)
        return 0;
    p = obj + 0x22;
    *p = 2;
    cnt = 0;
    E = (u8 *)Data_02009ea0;
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
        d = Data_02009e48[sel] & 0xffff0000;
        oc = obj;
        b[0] = *(s32 *)(oc + 8) + d;
        w = *(s32 *)(oc + 12);
        b[1] = w;
        b[2] = *(s32 *)(oc + 16) + (Data_02009e48[sel] << 16);
        a[3] = w;
    }
    for (;;) {
        s32 j, k;
        {
            u8 *E2 = (u8 *)Data_02009ea0;
            s32 o2 = a[0] << 4;
            o2 += 4;
            a[4] = b[2] + (*(s32 *)(E2 + o2) << 16);
        }
        j = 0;
        while (j < ny) {
            {
                u8 *E3 = (u8 *)Data_02009ea0;
                a[2] = b[0] + (*(s32 *)(E3 + (a[0] << 4)) << 16);
            }
            k = 0;
            while (k < nx) {
                s32 *w2 = a + 2;
                if (Func_020024b4(obj, w2) == 2)
                    goto found;
                w2[0] = w2[0] + 0x100000;
                k++;
            }
            a[4] = a[4] + 0x100000;
            j++;
        }
        cnt++;
        buf[0] = buf[0] + (Data_02009e48[sel] & 0xffff0000);
        buf[2] = buf[2] + (Data_02009e48[sel] << 16);
    }
found:
    *p = 0;
    ret = 0;
    if (cnt != 0) {
        s32 d = Data_02009e48[sel];
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

void Func_020017d8(s32 a0, s32 a1, s32 a2)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Func_020034b6_a(Data_02000240[125]);
    q = Func_020034be(a0);
    Func_020034b4();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Func_02003496(p, x, p->f0c, y);
    }
    Func_0200347e(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Func_020034c2(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Func_020034b6_b(q, 4);
    } else {
        Func_020034c0(q, 3);
    }
    Func_020034ee(p);
    Func_0200354a();
}
#undef EffectDescriptorTable
#undef GetPartyEffect
#undef SpawnEffect
#undef SetEffectVariant
#undef SetEffectDescriptor
#undef SetEffectMode
#undef ScaleEffectDeltaFromAccumulated
#undef ScaleEffectDeltaFromOrigin
#undef ScaleEffectVerticalDelta
#undef SetEffectCallbackMode
#undef SetEffectCallbackArgument
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
#undef SpawnConfiguredEffect
#undef AdvanceStagedActorPair

/* overlays/scene/actor/staged_pair_region/state_update.c */
#define State_SetValues8_3_4 Func_02000cc0
void Func_02002a16(s32, s32, s32);

/* Open the three-by-four presentation block used by this resource. */

void State_SetValues8_3_4(void)
{
    Func_02002a16(8, 3, 4);
}
#undef State_SetValues8_3_4
