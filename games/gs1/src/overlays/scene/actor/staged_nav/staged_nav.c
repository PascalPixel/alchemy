#include "types.h"
#include "staged_actor_movement.h"
#include "run_staged_actor_movement_and_redraw_body.inc"
#include "create_configured_overlay_object.h"
#include "create_configured_overlay_object_body.inc"
#include "configured_effect_spawn.h"
#include "staged_actor.h"
#include "configured_effect_spawn_body.inc"

/* overlays/scene/actor/staged_nav/actor_position.c */
#define Actor_UpdateBit1ByPositionToSlotZero Func_020020b8
#define StagedActor_PlaceAtObjectTenCell Func_02002498
u8 *Func_02004bdc(s32 slot);
u8 *Func_02004fbc();
void Func_02004fb2(void);
s32 Func_020029e6();
void Func_02004fd6(void);

/*
 * Place a staged actor at object ten's grid cell -- resource_3b3.
 */

/*
 * The Func_ aliases name the call words encoded in the overlay image. The
 * declarations are old-style because the call sites vary in arity.
 */

/*
 * resource_3b3 @ 0x020020b8 (56 bytes including trailing alignment).
 *
 * Compares an actor with slot zero.  When it is farther right, bit 1 at +35
 * is cleared and then restored only if the actor is also above slot zero.
 * The function always returns zero.
 */
s32 Actor_UpdateBit1ByPositionToSlotZero(u8 *actor)
{
    u8 *ref = Func_02004bdc(0);

    if (*(s32 *)(actor + 16) > *(s32 *)(ref + 16)) {
        actor[35] = (u8)(actor[35] & 0xfd);
        if (*(s32 *)(actor + 12) < *(s32 *)(ref + 12))
            actor[35] = (u8)(actor[35] | 2);
    }

    return 0;
}

/*
 * Fetch object ten, shift its +8 and +16 fixed-point fields down to grid
 * coordinates, and place there. The last two literal arguments go on the
 * stack.
 */
void StagedActor_PlaceAtObjectTenCell(void)
{
    u8 *obj = Func_02004fbc(10);
    s32 x;
    s32 z;

    Func_02004fb2();

    x = *(s32 *)(obj + 8) >> 20;
    z = *(s32 *)(obj + 16) >> 20;

    Func_020029e6(2, x, z, 1, 1, 0);
    Func_02004fd6();
}
#undef Actor_UpdateBit1ByPositionToSlotZero
#undef StagedActor_PlaceAtObjectTenCell

/* overlays/scene/actor/staged_nav/actor_presentation.c */
#define Scene_RunActor11Step Func_02002144
#define Scene_RunActor12Step Func_02002150
void Func_0200423a(s32 actor);
void Func_02004246(s32 actor);

/* Contiguous unnamed leaf-owner run for resource_3b3. */

/* Complete 12-byte actor-11 wrapper before 0x02002150. */
void Scene_RunActor11Step(void)
{
    Func_0200423a(11);
}

/* Complete 12-byte actor-12 wrapper before 0x0200215c. */
void Scene_RunActor12Step(void)
{
    Func_02004246(12);
}
#undef Scene_RunActor11Step
#undef Scene_RunActor12Step

/* overlays/scene/actor/staged_nav/actor_search.c */
#define Actor_CheckActors8To11NearSlotZero Func_020026b8
typedef struct {
    u8 pad[9];
    u8 low : 2;
    u8 mode : 2;
    u8 rest : 4;
} Handle;

u8 *Func_020051da();
u8 *Func_020051e4();
u8 *Func_02005240();
void Func_02005290();

void Actor_CheckActors8To11NearSlotZero(void)
{
    u8 *hero = Func_020051da(0);
    u32 selector = 8;
    u8 *actor;

loop:
    actor = Func_020051e4(selector);

    if (*(s32 *)(hero + 12) / 0x10000 != *(s32 *)(actor + 12) / 0x10000)
        goto mark_and_continue;

    if (*(s32 *)(hero + 16) > *(s32 *)(actor + 16) - 0x80000
        || *(s32 *)(hero + 16) <= *(s32 *)(actor + 16) - 0x180000)
        goto mark_and_continue;

    if (*(s32 *)(hero + 8) - 0x100000 > *(s32 *)(actor + 8)
        || *(s32 *)(actor + 8) >= *(s32 *)(hero + 8) + 0x100000)
        goto continue_loop;

    {
        Handle *handle = *(Handle **)(actor + 80);
        Func_02005290(0, handle->mode);
    }
    goto done;

mark_and_continue:
    {
        u8 *mark = Func_02005240(0) + 35;
        u8 bit = 1;
        bit |= *mark;
        *mark = bit;
    }

continue_loop:
    selector++;
    if (selector <= 11)
        goto loop;

done:
    return;
}
#undef Actor_CheckActors8To11NearSlotZero

/* overlays/scene/actor/staged_nav/configurable_effect_spawn.c */
#define Effect_SpawnRandomizedParticle Func_02002230
extern volatile s32 Data_03001e40;

void Func_02004e02(s32 soundId);
s32 Func_02004cb2(void);
s32 Func_02004cc0(void);
s32 Func_02004cd8(void);
void Func_020023f6(s32 x, s32 y, s32 z, s32 kind,
                          s32 arg4, s32 arg5, u32 flags, void *descriptor);

void Effect_SpawnRandomizedParticle(void)
{
    u8 descriptor[40];
    u8 *d;
    s32 spread;
    s32 secondary;
    u32 draw;
    u32 mask;

    if ((Data_03001e40 & 2) != 0)
        return;
    if ((Data_03001e40 & 7) == 0)
        Func_02004e02(136);

    d = descriptor;
    *(s32 *)(d + 4) = 10;
    *(s32 *)(d + 8) = 0x8000;
    *(s32 *)(d + 12) = 0x8000;
    *(s32 *)(d + 16) = 0x19999;
    *(s32 *)(d + 20) = 0x19999;
    draw = (u32)Func_02004cb2();
    mask = 0x0ffff000;
    mask &= draw;
    *(u16 *)(d + 32) = (u16)mask;
    *(s32 *)(d + 36) = 0x020093b1;

    draw = (u32)Func_02004cc0();
    spread = -((s32)((draw * 5) >> 16) * 0x10000 + 0x60000);
    spread /= 2;
    draw = (u32)Func_02004cd8();
    secondary = -((s32)((draw * 5) >> 16) * 0x10000 + 0x50000);

    Func_020023f6(0x01440000, 0x00300000, 0x00e40000, spread,
                  secondary, 0, 0x014d0000, d);
}
#undef Effect_SpawnRandomizedParticle

/* overlays/scene/actor/staged_nav/coordinate_preset.c */
#define CopyAndOffsetCoordinatePreset Func_020015cc
void CopyAndOffsetCoordinatePreset(void)
{
    u32 *destination;
    const u32 *source;
    u16 *coordinates;

    source = (const u32 *)0x03001ad4;
    destination = (u32 *)0x0200b72c;
    *destination++ = *source++;
    *destination++ = *source++;
    *destination = *source;
    coordinates = (u16 *)0x0200b72c;
    coordinates[1] += 0xc0;
    coordinates[3] += 0xc0;
    coordinates[5] += 0xc0;
}
#undef CopyAndOffsetCoordinatePreset

/* overlays/scene/actor/staged_nav/effect.c */
#define Effect_SpawnRandomEffectEveryEightFrames Func_020022d8
extern volatile s32 Data_03001e40;

s32 Func_02004d4c(void);
s32 Func_02004d60(void);
s32 Func_02004d70(void);
s32 Func_02004d82(void);
s32 Func_02004d78(s32 dividend, s32 divisor);
void Func_020024a0(s32 x, s32 y, s32 z, s32 kind,
                          s32 arg4, s32 arg5, u32 flags, void *desc);

s32 Effect_SpawnRandomEffectEveryEightFrames(u8 *actor)
{
    u8 desc[40];
    u8 *p;
    u32 phase = (u32)Data_03001e40 & 7;
    s32 x;
    s32 y;
    s32 z;
    s32 scale;

    if (phase != 0)
        return 0;

    p = desc;
    *(s32 *)(p + 4) = 7;
    *(s32 *)(p + 8) = 0xb333;
    *(s32 *)(p + 12) = 0xb333;

    x = *(s32 *)(actor + 8) + (((s32)(((u32)Func_02004d4c() * 17) >> 16) - 8) << 16);
    y = *(s32 *)(actor + 12) + ((s32)(((u32)Func_02004d60() * 17) >> 16) << 16);
    z = *(s32 *)(actor + 16) + (((s32)(((u32)Func_02004d70() * 17) >> 16) - 8) << 16);
    scale = Func_02004d78((s32)(((u32)Func_02004d82() * 5) >> 16) * 0x10000 + 0x30000, 10);

    Func_020024a0(x, y, z, 0, scale, (s32)phase, 0x00090001, p);
    return 0;
}
#undef Effect_SpawnRandomEffectEveryEightFrames

/* overlays/scene/actor/staged_nav/move_and_redraw.c */
#define Data_0200e1e8 Data_0200acf8
#define Data_0200e190 Data_0200aca0
#define Value_020082a9 Value_0200858d
#define Func_02000902 Func_02000eca
#define Func_02000a6e Func_02001036
#define Func_02000a80 Func_02001048
#define Func_02000acc Func_02001094
#define Func_02006610 Func_0200342c
#define Func_0200661c Func_02003438
#define Func_020066b4 Func_020034d0
#define Func_02006714 Func_02003508
#define Func_0200668a Func_020034be
#define Func_0200672c Func_02003520
#define Func_020066ea Func_02003506
#define Func_020066c4 Func_020034f8
#define RefreshStagedActor Func_02003518
#define Func_02006614 Func_02003468
#define Func_0200661e Func_02003472
#define Func_0200687c Func_020035c8
#define Func_02006658 Func_020034ac
#define Func_02006776 Func_0200356a
#define Func_0200678e Func_02003582
#define Func_02006740 Func_0200355c
#define Func_020067a4 Func_02003598
#define Func_020067bc Func_020035b0
#define Func_020067d4 Func_020035c8_a
#define Func_0200677a Func_02003596
#define Func_020068f4 Func_02003640
#define Func_020068fa Func_02003646
#define Func_020066b8 Func_0200350c
#define Func_02006752 Func_020035be
#define Func_020067ae Func_0200361a
#define Func_020069d0 Func_0200371c



void Func_020008ec(
    StagedActorMovementRequest request)
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

/* overlays/scene/actor/staged_nav/overlay_object.c */
#define NULL ((void *)0)
#define CreateOverlayObject Func_02002b26
#define SetOverlayObjectMode Func_02002bb0
#define SetOverlayObjectSlot Func_02002c50
#define OvObj_PrepareObject Func_02000048
#define OvObj_CreateConfiguredObject Func_020000a0
#define OvObj_WaitUntilSettledAndReset Func_02000d78
#define OvObj_SetCallbackAndMode2 Func_02002694


void *Func_02002ace(s32, s32, s32, s32);
void Func_02002b54(void *, s32);
void Func_02002b6c(void *, s32);
void Func_02002bf4(void *, s32);
void Func_020037be();
u8 *Func_020051b4(void);

/*
 * Poll an overlay object until it settles, then reset it -- resource_3b3.
 */

/* Declared without a prototype; the call site passes one argument. */

void *OvObj_PrepareObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj = Func_02002ace(arg3, arg0, arg1, arg2);

    if (obj != NULL) {
        u8 *rec = *(u8 **)(obj + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = rec[9];
        mask = -mask;
        mask &= flags;
        rec[9] = mask;
        obj[0x55] = 0;
        obj[0x59] = 8;
        Func_02002b54(obj, 0);
        Func_02002bf4(obj, 14);
        Func_02002b6c(obj, 1);
        return obj;
    }
    return NULL;
}

void *OvObj_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
}

/*
 * Wait at most sixty polls for the object's +12 to reach its +20, then clear
 * +0x28, set +0x3c, and mirror +20 back into +12. Func_020037be(1) is taken
 * to be a one-frame wait, and the field offsets are named by position and
 * not verified.
 */
void OvObj_WaitUntilSettledAndReset(u8 *obj)
{
    s32 cnt = 60;

    for (;;) {
        if (cnt == 0) {
            break;
        }
        Func_020037be(1);
        if (*(u32 *)(obj + 12) == *(u32 *)(obj + 20)) {
            break;
        }
        cnt--;
    }

    *(u32 *)(obj + 0x28) = 0;
    *(u32 *)(obj + 0x3c) = 0x80000000;
    *(u32 *)(obj + 12) = *(u32 *)(obj + 20);
}

void OvObj_SetCallbackAndMode2(void)
{
    u8 *obj = Func_020051b4();
    u8 *base = obj;
    u8 zero = 0;

    obj += 0x22;
    *obj = 2;
    base[0x55] = zero;
    *(u32 *)(base + 0x6c) = 0x02009aa9;
}
#undef NULL
#undef CreateOverlayObject
#undef SetOverlayObjectMode
#undef SetOverlayObjectSlot
#undef OvObj_PrepareObject
#undef OvObj_CreateConfiguredObject
#undef OvObj_WaitUntilSettledAndReset
#undef OvObj_SetCallbackAndMode2

/* overlays/scene/actor/staged_nav/scene_data.c */
#define SceneData_SelectTableBySceneId Func_02001440
#define SceneData_ReturnZero Func_020014b8
#define SceneData_GetTableb014 Func_020014bc
#define SceneData_SelectDataByRuntimeSelector Func_020014c4
#define SceneData_SelectSecondaryTableBySceneId Func_02002580
extern s16 Data_02000240[];
extern u8 Value_00000073;
extern u8 Value_00000074;
extern u8 Value_00000077;
extern u8 Value_00000079;
extern u8 Value_0000007a;
extern u8 Data_0200aeac[];
extern u8 Data_0200aef4[];
extern u8 Data_0200af3c[];
extern u8 Data_0200af84[];
extern u8 Data_0200afcc[];
extern u8 Data_0200ae7c[];
extern u8 Data_0200b06c[];
extern u8 Data_0200b0e4[];
extern u8 Data_0200b174[];
extern u8 Data_0200b2dc[];
extern u8 Data_0200b264[];
extern u8 Data_0200b3a8[];
extern u8 Data_0200b438[];
extern u8 Data_0200b498[];
extern u8 Data_0200b51c[];
extern u8 Data_0200b618[];
extern u8 Data_0200b39c[];

/* Contiguous unnamed leaf-owner run for resource_3b3. */

s32 SceneData_SelectTableBySceneId(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000073) {
        return (s32)Data_0200aeac;
    }
    if (v == (s32)&Value_00000074) {
        return (s32)Data_0200aef4;
    }
    if (v == (s32)&Value_00000077) {
        return (s32)Data_0200af3c;
    }
    if (v == (s32)&Value_00000079) {
        return (s32)Data_0200af84;
    }
    if (v == (s32)&Value_0000007a) {
        return (s32)Data_0200afcc;
    }
    return (s32)Data_0200ae7c;
}

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* Complete eight-byte literal-address getter, including its sole pool word. */
u8 *SceneData_GetTableb014(void)
{
    return (u8 *)0x0200b014;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000073) {
        return (s32)Data_0200b06c;
    }
    if (v == (s32)&Value_00000074) {
        return (s32)Data_0200b0e4;
    }
    if (v == (s32)&Value_00000077) {
        return (s32)Data_0200b174;
    }
    if (v == (s32)&Value_0000007a) {
        return (s32)Data_0200b2dc;
    }
    return (s32)Data_0200b264;
}

s32 SceneData_SelectSecondaryTableBySceneId(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000073) {
        return (s32)Data_0200b3a8;
    }
    if (v == (s32)&Value_00000074) {
        return (s32)Data_0200b438;
    }
    if (v == (s32)&Value_00000077) {
        return (s32)Data_0200b498;
    }
    if (v == (s32)&Value_00000079) {
        return (s32)Data_0200b51c;
    }
    if (v == (s32)&Value_0000007a) {
        return (s32)Data_0200b618;
    }
    return (s32)Data_0200b39c;
}
#undef SceneData_SelectTableBySceneId
#undef SceneData_ReturnZero
#undef SceneData_GetTableb014
#undef SceneData_SelectDataByRuntimeSelector
#undef SceneData_SelectSecondaryTableBySceneId

/* overlays/scene/actor/staged_nav/scene_primary_script.c */
#define Battle_Reset_1(args...) Func_0200426e(args)
#define Motion_SetSpeedLim_1(a0, a1) Call2(Func_020042ea, a0, a1)
#define Motion_CamBounds_1(a0, a1, a2, a3) Call4(Func_02004302, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_0200430e(args)
#define UiText_DrawMessage_1(a0, a1) Call2(Func_0200426e_a, a0, a1)
#define GameFlag_IsSet_1(a0) Value1(Func_0200427e, a0)
#define Audio_PlayCue_1(args...) Func_02004352(args)
#define Battle_WaitMode0_1(args...) Func_020042b2(args)
#define Audio_PlayCue_2(args...) Func_02004368(args)
#define Motion_SetActionVariant_1(args...) Func_02004330(args)
#define Scene_GetRecord_1(args...) Func_020042de(args)
#define Scene_GetRecord_2(args...) Func_020042e8(args)
#define Motion_SetHPosTerrain_1(a0, a1, a2) Call3(Func_0200432a, a0, a1, a2)
#define Object_SetModeById_1(args...) Func_0200433a(args)
#define GameFlag_Set_1(a0) Call1(Func_0200434c, a0)
#define Audio_PlayCue_3(args...) Func_02004414(args)
#define Battle_WaitMode0_2(args...) Func_02004374(args)
#define Audio_PlayCue_4(args...) Func_0200442a(args)
#define Scene_GetRecord_3(args...) Func_02004398(args)
#define Scene_GetRecord_4(args...) Func_020043a4(args)
#define Motion_SetHPosTerrain_2(a0, a1, a2) Call3(Func_020043e6, a0, a1, a2)
#define Object_SetModeById_2(args...) Func_020043f6(args)
#define GameFlag_Clear_1(a0) Call1(Func_02004406, a0)
#define Battle_SchedShoulder_1(args...) Func_02004422(args)
#define REC_ID 16
#define QUERY_FLAG 0x200
#define Scene_RunPrimarySequence Func_020011c4
#define Scene_RunScene3b3SequenceA Func_020015fc
#define Scene_RunFlaggedDisplayScene Func_0200175c
#define FieldScene_RunScene3b3_02001fd4 Func_02001fd4
#define FieldScene_RunScene3b3_020020f0 Func_020020f0
#define FieldScene_RunScene3b3_0200215c Func_0200215c
#define FieldScene_RunScene3b3_0200263c Func_0200263c

extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern u8 Data_02000240_t[][2];
extern u8 Data_0200b738[];

void Func_02001386();
s32 Func_02001630();
s32 Func_02001662();
s32 Func_02003c3e();
s32 Func_02003c52();
s32 Func_02003c5c();
s32 Func_02003c66();
s32 Func_02003cee();
void Func_02003d20();
void Func_02003d5e();
void Func_02003d68();
void Func_02003d92();
u8 *Func_02003d94();
s32 Func_02003da0();
void Func_02003da6();
void Func_02003de2();
s32 Func_02003dea();
s32 Func_02003e08();
s32 Func_02003e14();
s32 Func_02003e22();
void Func_02003e32();
void Func_02003e3c();
void Func_02003e5a();
void Func_02003ea4();
void Func_02003eaa();
void Func_02003eee();
void Func_020022e0();
void Func_020022f4();
void Func_02002308();
void Func_02002322();
void Func_02002336();
void Func_0200234a();
void Func_020040d4();
s32 Func_020040d6();
void Func_020040ec();
s32 Func_020040f2();
void Func_02004106();
void Func_02004154();
void Func_02004164();
void Func_02004174();
void Func_0200417a();
void Func_02004186();
void Func_0200418a();
void Func_0200419a();
void Func_020041ae();
s32 Func_020041bc();
void Func_020041be();
void Func_020041be_a();
void Func_02004264();
void Func_0200427c();
void Func_020026e0();
void Func_020026e6();
void Func_020026ec();
void Func_020026f2();
void Func_020026f8();
void Func_020026fe();
void Func_02002704();
void Func_02002822();
void Func_02002828();
void Func_0200282e();
void Func_02002834();
void Func_0200283a();
void Func_02002840();
void Func_02002846();
void Func_02002e0e();
void Func_02002ec0();
void Func_02004244();
void Func_0200426e();
void Func_0200426e_a();
s32 Func_0200427e();
void Func_0200428c();
void Func_02004296();
void Func_020042b2();
void Func_020042b2_a();
s32 Func_020042de();
u8 *Func_020042e8();
void Func_020042ea();
void Func_02004302();
void Func_02004306();
void Func_0200430e();
void Func_02004314();
void Func_0200432a();
void Func_02004330();
void Func_0200433a();
void Func_0200434c();
void Func_02004352();
void Func_0200435a();
void Func_0200435c();
void Func_02004368();
void Func_02004374();
s32 Func_02004398();
u8 *Func_020043a4();
void Func_020043c6();
void Func_020043e6();
void Func_020043f6();
void Func_02004406();
void Func_02004414();
void Func_02004422();
void Func_0200442a();
void Func_020023ac();
s32 Func_02003244();
void Func_02003d8c();
void Func_02004ae4();
s32 Func_02004b02();
s32 Func_02004b14();
s32 Func_02004b2a();
u8 *Func_02004b3a();
void Func_02004b3e();
void Func_02004bde();
void Func_02004c0a();
s32 Func_02004c14();
void Func_02004c16();
void Func_02004c42();
s32 Func_02004c56();
s32 Func_02004c7e();
void Func_02004c80();
s32 Func_02004c86();
void Func_02004c98();
void Func_020050be();
s32 Func_0200513a();
s32 Func_0200513a_a();
s32 Func_02005160();
void Func_020051aa();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Record id used for the single record configured below in either branch. */

/* Flag/id value passed to the query call and to the two closing calls, and
 * written into the flag byte at +85 when the query call returned zero. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    s32 Func_02001268();

    return f();
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    s32 Func_02001268();

    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    s32 Func_02001268();

    f(a0, a1, a2);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    s32 Func_02001268();

    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call7(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    f(a0, a1, a2, a3, a4, a5, a6);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_0200263c(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void Scene_RunPrimarySequence(void)
{
    s32 Func_02001268();

    s32 rec;
    s32 flag;
    s32 p6;
    s32 p5;
    s32 record;
    s32 v1;
    s32 v2;
    s32 v3;
    u8 *base;
    u8 slot16[40];

    rec = Func_02003cee(0);
    flag = *(volatile s32 *)0x03001e40 & 3;
    if (flag == 0) {
        base = slot16;
        *(s32 *)(base + 4) = 10;
        *(s32 *)(base + 8) = 0xb333;
        *(s32 *)(base + 12) = 0xb333;
        v1 = Func_02003c3e();
        p6 = *(volatile s32 *)(rec + 8) + ((((u32)((v1 << 4) + v1) >> 16) - 8) << 16);
        v2 = Func_02003c52();
        p5 = *(volatile s32 *)(rec + 16) + ((((u32)((v2 << 4) + v2) >> 16) - 8) << 16);
        v3 = Func_02003c66();
        record = Func_02003c5c((((u32)((v3 << 2) + v3) >> 16) << 16) + 0x30000, 10);
        Call8(Func_02001386, p6, *(volatile s32 *)(rec + 12), p5, 0, record, flag, 0x90001, (s32)base);
    }
}

s32 Func_02001268(void)
{
    s32 Func_02001268();

    u8 *rec;
    u8 *pflag;
    s32 saved;
    s32 mode;
    s32 *p;
    s32 buf[3];

    rec = Func_02003d94(0);
    pflag = rec + 85;
    saved = *pflag;
    mode = (*(volatile u16 *)(rec + 6) + 0x2000) & 0xc000;
    if (Data_02000240_t[249][0] != 0) {
        return 0;
    }
    p = buf;
    p[0] = (*(volatile s32 *)(rec + 8) & -0x100000) + 0x80000;
    p[1] = *(volatile s32 *)(rec + 12);
    p[2] = (*(volatile s32 *)(rec + 16) & -0x100000) + 0x80000;
    Call3(Func_02003d20, 0x100000, mode, (s32)p);
    if (Value2(Func_02003da0, (s32)rec, (s32)p) == 1) {
        goto reject;
    }
    if (Value2(Func_02001630, (s32)p, (s32)rec) != 0) {
        goto reject;
    }
    p[0] = (*(volatile s32 *)(rec + 8) & -0x100000) + 0x80000;
    p[1] = *(volatile s32 *)(rec + 12);
    p[2] = (*(volatile s32 *)(rec + 16) & -0x100000) + 0x80000;
    Call3(Func_02003d5e, 0x200000, mode, (s32)p);
    if (Value2(Func_02001662, (s32)p, (s32)rec) != 0) {
        goto reject;
    }
    if (Value2(Func_02003dea, (s32)rec, (s32)p) != 0) {
        goto reject;
    }
    Func_02003e32();
    Func_02003d92((s32)rec, 6);
    Func_02003d68(6);
    Func_02003eee(152);
    Func_02003da6((s32)rec, 7);
    *(volatile s32 *)(rec + 48) = 0x30000;
    *(volatile s32 *)(rec + 52) = 0x20000;
    *(volatile s32 *)(rec + 40) = 0x40000;
    *pflag &= 126;
    Func_02003e3c((s32)rec, 0);
    Func_02003eaa(0, *(s16 *)((u8 *)p + 2), *(s16 *)((u8 *)p + 10));
    Func_02003de2((s32)rec, 6);
    Func_02003e5a((s32)rec, 1);
    *pflag = saved;
    Func_02003ea4();
    return 1;
reject:
    return 0;
}

void Func_020013b0(s32 a0)
{
    s32 Func_02001268();

    s32 v68;
    s32 v72;
    s32 v76;
    s32 step;
    s32 t;

    v68 = *(s32 *)(a0 + 68);
    t = *(s32 *)(a0 + 8);
    v72 = *(s32 *)(a0 + 72);
    *(s32 *)(a0 + 8) = t + v68;
    t = *(s32 *)(a0 + 12);
    v76 = *(s32 *)(a0 + 76);
    *(s32 *)(a0 + 12) = t + v72;
    *(s32 *)(a0 + 16) += v76;
    step = Value2(Func_02003e08, v68, 10);
    *(s32 *)(a0 + 68) = v68 - step;
    step = Func_02003e14(v72, 3);
    *(s32 *)(a0 + 72) = v72 - step;
    step = Func_02003e22(v76, 10);
    *(volatile s32 *)(a0 + 76) = v76 - step;
    *(volatile s32 *)(a0 + 24) += *(volatile s32 *)(a0 + 48);
    *(volatile s32 *)(a0 + 28) += *(s32 *)(a0 + 52);
    *(u16 *)(*(volatile s32 *)(a0 + 80) + 30) += *(u16 *)(a0 + 100);
}

void Scene_RunScene3b3SequenceA(void)
{
    s32 record;
    s32 count;

    record = Value1(Func_020040f2, 0x200);
    if (record == 0) {
        Func_020022e0(10, 19, 16, 5, record, 10, 31);
        Func_020022f4(10, 51, 16, 5, 1, 10, 31);
        Func_02002308(42, 51, 16, 5, 2, 10, 31);
    } else {
        Func_02002322(10, 19, 16, 5, 0, 10, 31);
        Func_02002336(10, 83, 16, 5, 1, 10, 31);
        Func_0200234a(42, 83, 16, 5, 2, 10, 31);
    }
    *(volatile s32 *)Data_0200b738 = 0;
    Func_020040d6(0x20095cd, 0xc80);
    Func_020040d4(1);
    Func_02004106(1, 0, 0x2009579);
    Func_02004264(231);
    *(volatile s32 *)Data_0200b738 = 0;
    do {
        Func_020040ec(1);
        count = *(volatile s32 *)Data_0200b738;
        count = count + 1;
        *(volatile s32 *)Data_0200b738 = count;
    } while (count <= 100);
    Call1(Func_0200427c, 0x121);
    if (Value1(Func_020041bc, 0x200) == 0) {
        Call6(Func_0200418a, 0, 32, 32, 0, 32, 32);
        Call6(Func_0200419a, 32, 32, 64, 0, 32, 32);
    } else {
        Call6(Func_020041ae, 0, 64, 32, 0, 32, 32);
        Call6(Func_020041be, 32, 64, 64, 0, 32, 32);
    }
    Func_02004154(1);
    Func_02004186(1, 0, 0);
    Func_02004164(1);
    Call1(Func_0200417a, 0x20095cd);
    Func_020041be_a();
    Func_02004174(30);
}

/* Runs one of two near-identical setup sequences for record REC_ID and
 * records 9-15, chosen by the query call's return value; each sequence ends
 * with its own closing call carrying QUERY_FLAG. */
void Scene_RunFlaggedDisplayScene(void)
{
    u32 i;
    u8 *queried;
    u8 *record;

    Battle_Reset_1();
    Motion_SetSpeedLim_1(0x10000, 0x2000);
    Motion_CamBounds_1(0x1190000, -1, 0x1b00000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    UiText_DrawMessage_1(0x1528, 1);
    queried = GameFlag_IsSet_1(QUERY_FLAG);
    if (queried == 0) {
        Audio_PlayCue_1(232);
        Call3(Func_02004244, 0x200ada8, 84, 24);
        Battle_WaitMode0_1(30);
        Audio_PlayCue_2(240);
        Motion_SetActionVariant_1(REC_ID, 1);
        /* Flag byte at +85: cleared, since queried is zero here. */
        *(u8 *)(Scene_GetRecord_1(REC_ID) + 85) = queried;
        record = Scene_GetRecord_2(REC_ID);
        *(s32 *)(record + 12) = -0x200000;
        Motion_SetHPosTerrain_1(REC_ID, 0x1100000, 0x1a00000);
        Object_SetModeById_1(REC_ID, 1);
        Call3(Func_0200428c, 0x200adfc, 80, 24);
        Call3(Func_02004296, 0x200ae50, 80, 28);
        Call6(Func_020042b2_a, 65, 40, 16, 27, 2, 4);
        Func_02002e0e();
        Func_020026e0(9);
        Func_020026e6(10);
        Func_020026ec(11);
        Func_020026f2(12);
        Func_020026f8(13);
        Func_020026fe(14);
        Func_02002704(15);
        Call6(Func_02004314, 24, 3, 1, 1, 24, 8);
        GameFlag_Set_1(QUERY_FLAG);
    } else {
        Audio_PlayCue_3(232);
        Call3(Func_02004306, 0x200add2, 84, 24);
        Battle_WaitMode0_2(30);
        Audio_PlayCue_4(230);
        /* Flag byte at +85: cleared unconditionally in this branch. */
        *(u8 *)(Scene_GetRecord_3(REC_ID) + 85) = 0;
        record = Scene_GetRecord_4(REC_ID);
        *(s32 *)(record + 12) = -0x200000;
        Motion_SetHPosTerrain_2(REC_ID, 0x1100000, 0x1b40000);
        Object_SetModeById_2(REC_ID, 2);
        Call6(Func_0200435a, 65, 45, 16, 27, 2, 4);
        Call3(Func_0200435c, 0x200ae26, 80, 24);
        Func_02002ec0();
        Func_02002822(9);
        Func_02002828(10);
        Func_0200282e(11);
        Func_02002834(12);
        Func_0200283a(13);
        Func_02002840(14);
        Func_02002846(15);
        Call6(Func_020043c6, 24, 4, 1, 1, 24, 8);
        GameFlag_Clear_1(QUERY_FLAG);
    }
    Battle_SchedShoulder_1();
}

void FieldScene_RunScene3b3_02001fd4(void)
{
    u32 i;
    u8 *record;

    Func_02004ae4();
    if (Value0(Func_02003244) == 0) {
        *(u8 *)(Func_02004b02(0) + 85) &= 254;
        *(u8 *)(Func_02004b14(0) + 35) &= 254;
        Func_020023ac();
        Func_02003d8c();
        *(u8 *)(Func_02004b2a(0) + 85) |= 1;
        {
            u8 *record = Func_02004b3a(0);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 1);
        }
    }
    Func_02004b3e();
}

void FieldScene_RunScene3b3_020020f0(s32 a0)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value0(Func_02004c14);
    Func_02004c0a();
    *(s32 *)(rec7 + 108) = 0x200a0b9;
    Call6(Func_02004bde, 20, 14, 1, 1, (*(s32 *)(rec7 + 8) >> 20), (*(s32 *)(rec7 + 16) >> 20));
    Func_02004c16((a0 + 0x1f5));
    Value2(Func_02004c56, a0, 0x200ad64);
    Func_02004c42();
}

void FieldScene_RunScene3b3_0200215c(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    u8 *p6;

    rec7 = Value1(Func_02004c7e, 0);
    record = Func_02004c86(13);
    p6 = *(s32 *)0x03001f30;
    if ((*(s32 *)(record + 8) >> 20) == (*(s32 *)(rec7 + 8) >> 20)) {
        if ((*(s32 *)(record + 16) >> 20) != (*(s32 *)(rec7 + 16) >> 20)) {
            goto L_02002198;
        }
        Call1(Func_02004c80, 0x203);
        p6[53] = 1;
    } else {
        L_02002198:;
        Call1(Func_02004c98, 0x203);
    }
}

void FieldScene_RunScene3b3_0200263c(s32 a0)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Func_02005160();
    if (Value1_0200263c(Func_0200513a, (a0 + 0x1f5)) != 0) {
        Func_020050be(rec7, 5);
        *(s32 *)(rec7 + 108) = 0x200a0b9;
        Call6(Func_0200513a_a, 20, 14, 1, 1, (*(s32 *)(rec7 + 8) >> 20), (*(s32 *)(rec7 + 16) >> 20));
        Func_020051aa(a0, 0x200ad64);
    }
}
#undef Battle_Reset_1
#undef Motion_SetSpeedLim_1
#undef Motion_CamBounds_1
#undef Object_CommitPositionThenWaitIfModeZero_1
#undef UiText_DrawMessage_1
#undef GameFlag_IsSet_1
#undef Audio_PlayCue_1
#undef Battle_WaitMode0_1
#undef Audio_PlayCue_2
#undef Motion_SetActionVariant_1
#undef Scene_GetRecord_1
#undef Scene_GetRecord_2
#undef Motion_SetHPosTerrain_1
#undef Object_SetModeById_1
#undef GameFlag_Set_1
#undef Audio_PlayCue_3
#undef Battle_WaitMode0_2
#undef Audio_PlayCue_4
#undef Scene_GetRecord_3
#undef Scene_GetRecord_4
#undef Motion_SetHPosTerrain_2
#undef Object_SetModeById_2
#undef GameFlag_Clear_1
#undef Battle_SchedShoulder_1
#undef REC_ID
#undef QUERY_FLAG
#undef Scene_RunPrimarySequence
#undef Scene_RunScene3b3SequenceA
#undef Scene_RunFlaggedDisplayScene
#undef FieldScene_RunScene3b3_02001fd4
#undef FieldScene_RunScene3b3_020020f0
#undef FieldScene_RunScene3b3_0200215c
#undef FieldScene_RunScene3b3_0200263c

/* overlays/scene/actor/staged_nav/scene_state_interaction.c */
#define State_LinkActorZeroToWork24 Func_02002080
extern u8 *Data_03001ee0;

u8 *Func_02004ba2();

/* Complete scene/entity linker through return and its sole pool word. */
void State_LinkActorZeroToWork24(void)
{
    u8 *obj = Func_02004ba2(0);
    *(u8 **)(Data_03001ee0 + 24) = obj;
    obj[98] = 1;
}
#undef State_LinkActorZeroToWork24

/* overlays/scene/actor/staged_nav/staged_actor_navigation.c */
#define EffectDescriptorTable Data_0200ad58
#define GetPartyEffect Func_02002c7a
#define SpawnEffect Func_02002bf8
#define SetEffectVariant Func_02002c02
#define SetEffectDescriptor Func_02002c1c
#define SetEffectMode Func_02002d86
#define ScaleEffectDeltaFromAccumulated Func_02002ca8
#define ScaleEffectDeltaFromOrigin Func_02002cc0
#define ScaleEffectVerticalDelta Func_02002cce
#define SetEffectCallbackMode Func_02002d1c
#define SetEffectCallbackArgument Func_02002d2c
#define StagedActorStepTable Data_0200aca0
#define GetStagedActor Func_02002ed4
#define FindNextStagedActor Func_0200073e
#define FindBlockingStagedActor Func_0200076a
#define FindElevatedBlockingStagedActor Func_02000796
#define CanStartStagedActorMove Func_02002f50
#define SetStagedActorMode Func_02002f00
#define SelectStagedActorSlot Func_02002ed8
#define StartStagedActorEffect Func_0200305e
#define StartNextStagedActorMove Func_02002f46
#define StartLeadStagedActorMove Func_02002f56
#define FinishStagedActorMove Func_02002f64
#define FinishStagedActorEffect Func_02003080
#define SetStagedActorTransition Func_02002f6c
#define SpawnConfiguredEffect Func_0200013c
#define AdvanceStagedActorPair Func_020003a8
#define Actor_ApplyPlacementQueryAndTag Func_02000ec8
#define Actor_ApplyPlacementQuery Func_02000f58


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

extern s32 Data_0200ace0[];
extern s32 Data_0200acf8[];

struct StagedActor *Func_02002ed4(s32 arg0);
struct StagedActor *Func_0200073e(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_0200076a(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_02000796(s32 *arg0, struct StagedActor *arg1);
s32 Func_02002f50(struct StagedActor *arg0, s32 *arg1);
void Func_02002f00(struct StagedActor *arg0, s32 arg1);
void Func_02002ed8(s32 arg0);
void Func_0200305e(s32 arg0);
void Func_02002f46(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02002f56(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02002f64(struct StagedActor *arg0);
void Func_02003080(void);
void Func_02002f6c(struct StagedActor *arg0, s32 arg1);
Ent *Func_0200090a(Desc *, Ent *);
s32 Func_020030d2(Ent *, Desc *);
s32 Func_02000da6(s32 *, s32 *, s32 *);
s32 Func_0200330c(u8 *, s32 *);
u8 *Func_020039ec();
s32 Func_02001cc4();
void Func_020039c8();
void Func_02003970();
void Func_020039d8();
void Func_020039ee();
u8 *Func_02003a7c();
s32 Func_02001d54();
void Func_02003a5c();
void Func_020014d4();
void Func_02003a18();

/* The size test is unsigned, so out20 and out16 are u32. The two rec words
 * that travel on the stack are read into their own locals, which puts both
 * loads before both stores. The +0x23 flag set takes its mask from a local
 * assigned first, which fixes the register the mask lands in. */

/*
 * Apply a placement query and tag the object. Func_02001cc4 fills out20 and
 * out16, a 24-byte record of which only rec[2] and rec[4] are read back, and
 * two further out-params whose written values are never read -- only the
 * pointers matter. On success the object is tagged at +0x23 and one of two
 * near-identical finishing calls runs; on failure the object is untouched.
 */

/* The callees are declared at their in-overlay entry points. Declaring them at
 * their veneer addresses in ROM space would route every call through a
 * veneer. */

/*
 * Apply a placement query to an actor. Func_02001d54 is an out-param helper:
 * it fills out20 and out16, a 24-byte record of which only rec[2] and rec[4]
 * are read back, and out12 and out8 passed on the stack. Its field semantics
 * are not established. On success the values thread unchanged into the two
 * finishing calls in that shape; on failure the object is left untouched.
 */

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
}

void AdvanceStagedActorPair(void)
{
    extern u32 Data_0200aca0[];

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
    extern s32 Data_0200aca0[];

    Desc d;
    u32 idx;
    s32 m;
    Ent *r;

    idx = a->unk6 >> 12;
    m = Data_0200aca0[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    r = Func_0200090a(&d, a);
    if (r != 0) {
        u32 i = 0;
        s32 v = *r->unk50->unk28;
        s32 *p = Data_0200ace0;

        do {
            if (v == *p++) goto done;
            i++;
        } while (i <= 5);
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
    m = Data_0200aca0[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    if (Func_020030d2(a, &d) > 0) {
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
    extern s32 Data_0200aca0[];

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
    E = (u8 *)Data_0200acf8;
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
        d = Data_0200aca0[sel] & 0xffff0000;
        oc = obj;
        b[0] = *(s32 *)(oc + 8) + d;
        w = *(s32 *)(oc + 12);
        b[1] = w;
        b[2] = *(s32 *)(oc + 16) + (Data_0200aca0[sel] << 16);
        a[3] = w;
    }
    for (;;) {
        s32 j, k;
        {
            u8 *E2 = (u8 *)Data_0200acf8;
            s32 o2 = a[0] << 4;
            o2 += 4;
            a[4] = b[2] + (*(s32 *)(E2 + o2) << 16);
        }
        j = 0;
        while (j < ny) {
            {
                u8 *E3 = (u8 *)Data_0200acf8;
                a[2] = b[0] + (*(s32 *)(E3 + (a[0] << 4)) << 16);
            }
            k = 0;
            while (k < nx) {
                s32 *w2 = a + 2;
                if (Func_0200330c(obj, w2) == 2)
                    goto found;
                w2[0] = w2[0] + 0x100000;
                k++;
            }
            a[4] = a[4] + 0x100000;
            j++;
        }
        cnt++;
        buf[0] = buf[0] + (Data_0200aca0[sel] & 0xffff0000);
        buf[2] = buf[2] + (Data_0200aca0[sel] << 16);
    }
found:
    *p = 0;
    ret = 0;
    if (cnt != 0) {
        s32 d = Data_0200aca0[sel];
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

s32 Actor_ApplyPlacementQueryAndTag(u8 *no)
{
    u8 *obj = Func_020039ec(no);
    u32 out20, out16;
    s32 out12, out8;
    s32 rec[6];
    s32 r2, r4;
    u8 mask;

    if (Func_02001cc4(no, &out20, &out16, rec, &out12, &out8) == 0) {
        return 0;
    }

    r2 = rec[2];
    r4 = rec[4];
    Func_020039c8(2, 2, out20, out16, r2, r4);

    Func_02003970(obj, 4);
    mask = 2;
    obj[0x23] = obj[0x23] | mask;

    if (out20 > out16) {
        Func_020039d8(70, 40, rec[2] + 32, rec[4] + 2, out20, out16);
    } else {
        Func_020039ee(68, 40, rec[2] + 32, rec[4] + 2, out20, out16);
    }

    return 1;
}

s32 Actor_ApplyPlacementQuery(u8 *no)
{
    u8 *obj = Func_02003a7c(no);
    s32 out20, out16, out12, out8;
    s32 rec[6];

    if (Func_02001d54(no, &out20, &out16, rec, &out12, &out8) == 0) {
        return 0;
    }

    {
        s32 x = out12 + rec[2];
        s32 z = out8 + rec[4];

        Func_02003a5c(x, z, out20, out16, rec[2], rec[4]);
        Func_020014d4(0, rec[2], rec[4], out20, out16, 255);
    }

    Func_02003a18(obj, 1);
    obj[0x23] &= 0xfd;

    return 1;
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
#undef Actor_ApplyPlacementQueryAndTag
#undef Actor_ApplyPlacementQuery

/* overlays/scene/actor/staged_nav/state_update.c */
#define State_ApplyPlacementResult Func_02001544
#define State_ClearWord24AndObjectByte62 Func_0200209c
typedef struct {
    s32 v[6];
} PlacementResult;

extern u8 *Data_03001ee0;

void Func_02004056(void);
s32 Func_02001caa(PlacementResult *out);
void Func_02001e56(PlacementResult out);
void Func_02004082(void);
u8 *Func_02004bbe();

/*
 * Scene state reset for overlay resource_3b3. The callee name refers to its
 * own call word rather than to a shared runtime address.
 */

void State_ApplyPlacementResult(void)
{
    PlacementResult out;

    Func_02004056();
    if (Func_02001caa(&out) != 0)
        Func_02001e56(out);
    Func_02004082();
}

/*
 * Clears Data_03001ee0[+24] and one flag byte on the object returned by
 * Func_02004bbe. The 28-byte owner at 0x0200209c includes its one pool
 * word, the Data_03001ee0 pointer.
 */
void State_ClearWord24AndObjectByte62(void)
{
    u8 *obj = Func_02004bbe(0);

    *(s32 *)(Data_03001ee0 + 24) = 0;
    obj[0x62] = 0;
}
#undef State_ApplyPlacementResult
#undef State_ClearWord24AndObjectByte62
