#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mapcopy.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "call.h"
#include "b5_context.h"
#include "edition.h"
#include "tbs_edition.h"
#include "fieldobj.h"
#include "game_state.h"
#include "field_service.h"
#include "text/msg_ids.h"

void GomaHashira_SpawnPillarEffect();

void BattleFx_SetPhaseRequest(s32, s32);

enum StagedPlacementMessage {
    MSG_GOT_WOW_THATS_PRETTY_IMPRESSIVE = 0x132f
};

struct ScriptTable {
    u8 *script[3];
};

struct PillarEffect {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x1c];
    s32 rate_first;
    s32 rate_second;
    s32 target_x;
    s32 target_y;
    s32 target_z;
    u8 unknown_44[0x22];
    s16 plane;
};

enum PillarEffectPlane {
    PLANE_XY = 0,
    PLANE_XZ = 1,
    PLANE_YZ = 2
};

struct Sprite389 {
    u8 pad[9];
    u8 lo : 2;
    u8 layer : 2;
    u8 hi : 4;
};

void ConfigureActorThirteenSceneParameters(void)
{
    Actor_ShowEmote(13, 256, 0);
    Engine_ActorJump(13, 2, 0);
    BattleFx_SetPhaseRequest(12, 40);
}

void SceneEffect_AdvancePositionByAxisMode(struct PillarEffect *effect)
{
    s16 plane = effect->plane;

    switch (plane) {
    case PLANE_XY:
        effect->x += effect->rate_first;
        effect->target_x = effect->x;
        effect->y += effect->rate_second;
        effect->target_y = effect->y;
        break;
    case PLANE_XZ:
        effect->x += effect->rate_first;
        effect->target_x = effect->x;
        effect->z += effect->rate_second;
        effect->target_z = effect->z;
        break;
    case PLANE_YZ:
        effect->y += effect->rate_first;
        effect->target_y = effect->y;
        effect->z += effect->rate_second;
        effect->target_z = effect->z;
        break;
    }
}

s32 OverlayObject_ApplyValue15(s32 actor)
{
    ObjectGroup_SetChildValue(actor, 15);
    return 0;
}

void GomaHashira_SpawnPillarEffect(s32 x, s32 y, s32 z, s32 a3, s32 a4, s32 flags, s32 layer)
{
    struct ScriptTable table;
    u8 *leader;
    u8 *obj;
    struct Sprite389 *spr;
    u32 sel;

    leader = (u8 *)Object_GetById(0);
    table = GomaHashira_PillarScripts;
    obj = (u8 *)Engine_ObjectCreate(222, x, y, z);
    if (obj == 0)
        return;
    spr = *(struct Sprite389 **)(obj + 80);
    Object_SetMode(obj, (flags + 1) & 15);
    Engine_ObjectSetScript(obj, table.script[flags & 15]);
    ObjectGroup_SetChildValue(obj, ((u32)flags >> 16) & 15);
    obj[85] = 0;
    ((u8 *)spr)[38] = 0;
    *(s32 *)(obj + 108) = (s32)SceneEffect_AdvancePositionByAxisMode;
    *(s32 *)(obj + 48) = a3;
    *(s32 *)(obj + 52) = a4;
    *(u16 *)(obj + 102) = *(u16 *)&layer;
    sel = (u32)layer >> 16;
    switch (sel) {
    case 0:
        spr->layer = (*(struct Sprite389 **)(leader + 80))->layer;
        break;
    case 1:
    case 2:
    case 3:
        obj[35] &= 0xfe;
        spr->layer = sel;
        break;
    }
}

u8 *SceneData_GetScriptTable(void) { return GomaHashira_Scripts; }

s32 SceneData_ReturnZero(void) { return 0; }

u8 *SceneData_GetMessageTable(void) { return GomaHashira_Messages; }

u8 *SceneData_GetActorTable(void) { return GomaHashira_Actors; }

void ConfigureSceneForActorEightColumn(void)
{
    struct FieldActor *actor;
    s32 x;
    s32 a4;
    s32 a5;

    actor = Object_GetById(8);
    x = actor->x.fixed;
    if (x < 0)
        x += 0xfffff;
    x >>= 20;
    Engine_EventBegin();
    if (x == 20) {
        a4 = 18;
        a5 = 6;
        Engine_MapCopyCellAttributes(18, 40, 6, 3, a4, a5);
        Engine_GameFlagClear(0x302);
    } else {
        a4 = 18;
        a5 = 6;
        Engine_MapCopyCellAttributes(24, 40, 6, 3, a4, a5);
        Engine_GameFlagSet(0x302);
    }
    Engine_EventEnd();
}

void FieldScene_RunPrimarySequence(void)
{
    s32 flags;
    s32 a0;
    s32 a2;
    struct FieldActor *rec4;
    s32 rec7a, reca, v1a, v2a, v3a, ta, ua, na;
    s32 rec7b, recb, v1b, v2b, v3b, tb, ub, nb;

    flags = gFrameCount & 7;
    if (flags == 0) {
        rec7a = Object_GetById(9);
        v1a = Engine_RandomNext();
        a0 = *(s32 *)(rec7a + 8);
        a0 = a0 + ((((u32)(((v1a << 1) + v1a) << 2)) >> 16) << 16);
        rec4 = Object_GetById(9);
        reca = (s32)Object_GetById(9);
        a2 = *(s32 *)(reca + 16);
        a2 = a2 + 0x60000;
        v2a = Engine_RandomNext();
        ta = ((u32)((v2a << 2) + v2a) >> 16);
        ua = (((ta << 1) + ta) << 2) + ta;
        na = ua << 6;
        na = na - ua;
        na = na << 3;
        na = na + ta;
        v3a = Engine_RandomNext();
        na = -na;
        Call7(GomaHashira_SpawnPillarEffect, a0, rec4->y.fixed, a2, 0, na, ((u32)(v3a << 1) >> 16), flags);
        flags = gFrameCount & 15;
        if (flags == 0) {
            rec7b = Object_GetById(9);
            v1b = Engine_RandomNext();
            a0 = *(s32 *)(rec7b + 8);
            a0 = a0 + ((((u32)(((v1b << 1) + v1b) << 2)) >> 16) << 16);
            rec4 = Object_GetById(9);
            recb = (s32)Object_GetById(9);
            a2 = *(s32 *)(recb + 16);
            a2 = a2 + 0x60000;
            v2b = Engine_RandomNext();
            tb = ((u32)((v2b << 2) + v2b) >> 16);
            ub = (((tb << 1) + tb) << 2) + tb;
            nb = ub << 6;
            nb = nb - ub;
            nb = nb << 3;
            nb = nb + tb;
            v3b = Engine_RandomNext();
            nb = -nb;
            Call7(GomaHashira_SpawnPillarEffect, a0, rec4->y.fixed, a2, 0, nb, ((u32)(v3b << 1) >> 16), flags);
        }
    }
}

#if !EDITION_INTERNATIONAL
#define FIELD_STAGED_ACTOR_IMPORTS
#endif
#if !EDITION_INTERNATIONAL
#include "fixed_point_position.h"

#define Field_TryJumpForward ((void (*)(void))Field_TryJumpForward)

s32 Object_CheckMovementCollision(struct FieldActor *object, struct FixedPointPosition *position);
void Engine_TaskWait(s32 frames);
#endif

void Engine_EventBegin(void);
void Engine_EventEnd(void);
void Engine_EventWait(s32 frames);
void Engine_ActorSetPosition(s32 actor, s32 fixed_x, s32 fixed_z);
void Engine_ActorSetDestinationOffset(s32 actor, s32 dx, s32 dz);
void Engine_ActorWaitForMove(s32 actor);
void Engine_ActorSetSpeed(s32 actor, s32 speed, s32 acceleration);
void Engine_ActorSetDestination(s32 actor, s32 x, s32 z);
s32 Engine_RandomNext(void);
void Engine_WorkSetValuesIfNonNegative(s32 first, s32 second, s32 third);
void Engine_MapRenderWaitForValues(void);
#if EDITION_INTERNATIONAL
s32 Engine_GameFlagSet(s32 flag);
#endif
void Engine_AudioPlayCue(s32 cue);
#if EDITION_INTERNATIONAL
#endif
void GomaHashira_SpawnPillarEffect();
void FieldScene_RunPrimarySequence(void);

s32 GomaHashira_Actor13Frames;

void FieldScene_RunPillarSequence(void)
{
    s32 kind;
    s32 zero;
    s32 base;
    s32 a0;
    s32 a2;
    struct FieldActor *rec4;
    s32 r1, r2, r3, r4, r5, r6, r7;
    s32 rec7, v1, v2, v3, t, u, n;
    struct FieldActor *p0;

    r1 = ((s32 (*)())Object_GetById)(9);
    kind = *(s32 *)(r1 + 8) / 0x100000;
    Engine_EventBegin();
    if (kind == 25) {
        p0 = Object_GetById(11);
        zero = 0;
        p0->terrain_id = 1;
        r2 = (s32)Object_GetById(11);
        Engine_ActorSetSpriteFlags(r2, 0);
        Engine_ActorSetChildValue(11, 14);
        r3 = (s32)Object_GetById(11);
        Engine_ObjectSetBlendMode(r3, 1);
        Engine_ActorSetPosition(11, 0x19e0000, 0xf00000);
        Engine_EventWait(10);
        base = (s32)FieldScene_RunPrimarySequence;
        Engine_TaskAddCallback(base, 0xc80);
        Engine_AudioPlayCue(141);
        Engine_ActorSetDestinationOffset(9, 1, 0);
        Engine_ActorWaitForMove(9);
        Engine_EventWait(10);
        Engine_ActorSetDestinationOffset(9, 2, 0);
        Engine_ActorWaitForMove(9);
        r4 = (s32)Object_GetById(9);
        *(s32 *)(r4 + 68) = zero;
        r5 = (s32)Object_GetById(9);
        *(s32 *)(r5 + 72) = 0x9999;
        Engine_EventWait(3);
        Engine_ActorSetSpeed(9, 0x28000, 0x4000);
        Engine_AudioPlayCue(0x120);
        Engine_ActorSetDestination(9, 0x1a0, 200);
        r6 = (s32)Object_GetById(9);
        Engine_ActorSetSpriteFlags(r6, 0);
        Engine_TaskRemoveCallback(base);
        Engine_EventWait(12);
        Engine_AudioPlayCue(189);
        rec7 = ((s32 (*)())Object_GetById)(9);
        v1 = Engine_RandomNext();
        a0 = *(s32 *)(rec7 + 8);
        a0 = a0 + ((((u32)(((v1 << 1) + v1) << 2)) >> 16) << 16);
        rec4 = Object_GetById(9);
        r7 = (s32)Object_GetById(9);
        a2 = *(s32 *)(r7 + 16);
        a2 = a2 + 0x60000;
        v2 = Engine_RandomNext();
        t = ((u32)((v2 << 2) + v2) >> 16);
        u = (((t << 1) + t) << 2) + t;
        n = u << 6;
        n = n - u;
        n = n << 3;
        n = n + t;
        v3 = Engine_RandomNext();
        n = -n;
        Call7(GomaHashira_SpawnPillarEffect, a0, rec4->y.fixed, a2, zero, n, ((u32)(v3 << 1) >> 16), zero);
        Engine_EventWait(20);
        Engine_AudioPlayCue(154);
        Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Engine_MapRenderWaitForValues();
        Engine_ActorSetPosition(9, 0, 0);
        Engine_ActorSetPosition(11, 0, 0);
        Engine_GameFlagSet(0x300);
        Map_CopyCellAttributes(21, 45, 4, 2, 21, 11);
    }
    Engine_EventEnd();
}

void FieldScene_RunPillarBurst(void)
{
    s32 x;
    s32 z;

    x = Object_GetById(10)->x.fixed / 0x100000;
    z = Object_GetById(10)->z.fixed / 0x100000;
    if (x == 38 && z == 14) {
        Object_GetById(10)->y.fixed = -0x20000;
        Object_GetById(10)->target_y = Object_GetById(10)->y.fixed;
        Engine_AudioPlayCue(188);
        GomaHashira_SpawnPillarEffect(Object_GetById(10)->x.fixed, Object_GetById(10)->y.fixed,
            Object_GetById(10)->z.fixed, 0x8000, 0, 0, 1);
        GomaHashira_SpawnPillarEffect(Object_GetById(10)->x.fixed, Object_GetById(10)->y.fixed,
            Object_GetById(10)->z.fixed, 0x6666, 0x6666, 0, 1);
        GomaHashira_SpawnPillarEffect(Object_GetById(10)->x.fixed, Object_GetById(10)->y.fixed,
            Object_GetById(10)->z.fixed, -0x6666, 0x6666, 0, 1);
        GomaHashira_SpawnPillarEffect(Object_GetById(10)->x.fixed, Object_GetById(10)->y.fixed,
            Object_GetById(10)->z.fixed, -0x8000, 0, 0, 1);
        GomaHashira_SpawnPillarEffect(Object_GetById(10)->x.fixed, Object_GetById(10)->y.fixed,
            Object_GetById(10)->z.fixed, 0x6666, -0x6666, 0, 1);
        GomaHashira_SpawnPillarEffect(Object_GetById(10)->x.fixed, Object_GetById(10)->y.fixed,
            Object_GetById(10)->z.fixed, -0x6666, -0x6666, 0, 1);
        Engine_GameFlagSet(0x301);
    }
}

void FieldScene_RunThreeCallSequence(void)
{
    Engine_EventBegin();
    FieldScene_RunPillarBurst();
    Engine_EventEnd();
}

void FieldScene_RunFourStepSequence(void)
{
    Engine_EventBegin();
    StagedActor_AdvancePair();
    FieldScene_RunPillarBurst();
    Engine_EventEnd();
}

void FieldScene_TryJumpForward(void)
{
#if EDITION_INTERNATIONAL
    Field_TryJumpForward();
#else
    struct FieldActor *obj;
    struct FixedPointPosition target;
    struct FixedPointPosition *pos;
    u8 *flags;
    u8 saved;

    obj = Object_GetById(0);
    flags = &obj->motion_flags;
    saved = *flags;
    target.x = Object_GetById(0)->x.fixed - 0x200000;
    pos = &target;
    pos->y = Object_GetById(0)->y.fixed;
    pos->z = Object_GetById(0)->z.fixed;
    if (Object_CheckMovementCollision(obj, pos) == 0) {
        Engine_EventBegin();
        Object_SetMode(obj, 6);
        Engine_TaskWait(6);
        Engine_AudioPlayCue(152);
        Object_SetMode(obj, 7);
        obj->speed = 0x30000;
        obj->acceleration = 0x20000;
        obj->velocity_y = 0x40000;
        *flags &= 0x7e;
        Engine_ActorSetSpriteFlags(obj, 0);
        Engine_ActorMoveToAndWait(0, *(s16 *)((u8 *)pos + 2), *(s16 *)((u8 *)pos + 10));
        Object_SetMode(obj, 6);
        Engine_ActorSetSpriteFlags(obj, 1);
        *flags = saved;
        Engine_EventEnd();
    }
#endif
}

u8 *SceneData_GetExtraTable(void)
{
    return GomaHashira_Extras;
}

void GomaHashira_DriveActor13Idle(void)
{
    struct EventWork *work;
    s32 t;

    work = gEventWork;
    t = ++GomaHashira_Actor13Frames;
    switch (t) {
    case 60:
        Actor_FaceDirection(13, 0x2000, 0);
        Engine_ActorShowEmote(13, 2, 0);
        break;
    case 180:
        Engine_ActorStartRepeatedMotion(13, 3);
        break;
    case 240:
    case 270:
        Engine_ActorJump(13, 4, 0);
        break;
    case 480:
        Engine_ActorSetAnimation(13, 4);
        break;
    }
    if (gGameState.pending_djinn_event == 0)
        work->raised_trigger = 99;
}

void FieldScene_RunActor13Departure(void)
{
    Engine_TaskRemoveCallback(GomaHashira_DriveActor13Idle);
    Engine_EventBegin();
    Actor_ShowEmote(13, 0x100, 30);
    Engine_ActorRunRepeatedMotion(13, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Engine_EventSetMessage((s32)MsgGomaGotWowThatsPrettyImpressive);
    Event_ShowMessage(13, 0);
    Engine_ActorSetAnimationAndWait(13, 3);
    Engine_EventWait(30);
    Object_GetById(10)->priority_flags &= ~ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetSpeed(13, 0x20000, 0x10000);
    Actor_WalkToAndWait(13, 0x258, 216);
    Actor_WalkToAndWait(13, 0x258, 248);
    Actor_WalkToAndWait(13, 0x238, 0x128);
    Actor_SetPosition(13, 0, 0);
    Object_GetById(10)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
    GameFlag_Set(0x869);
    Engine_EventEnd();
}

s32 FieldScene_SetupPillarsOnEntry(void)
{
    struct FieldActor *actor;
    struct EventWork *work;

    work = gEventWork;
    work->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    work->transition_frames = 24;
    Object_GetById(9)->collision_flags |= 16;
    if (GameFlag_IsSet(0x302) != 0) {
        Actor_SetPosition(8, 0x1580000, 0x680000);
        Map_CopyCellAttributes(24, 40, 6, 3, 18, 6);
    } else {
        Map_CopyCellAttributes(18, 40, 6, 3, 18, 6);
    }
    if (GameFlag_IsSet(0x300) != 0) {
        Actor_SetPosition(9, 0, 0);
        Map_CopyCellAttributes(21, 45, 4, 2, 21, 11);
    }
    if (GameFlag_IsSet(0x301) != 0) {
        Actor_SetPosition(10, 0x2680000, 0xe80000);
        if (gGameState.entrance == 2 || gGameState.entrance == 3) {
            Object_GetById(10)->terrain_id = 2;
            actor = Object_GetById(10);
            actor->y.fixed = actor->y.fixed - 1;
            Object_GetById(10)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
            Map_CopyCellAttributes(36, 48, 5, 1, 36, 14);
        }
    }
    if (gGameState.entrance == 99) {
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Actor_SetPosition(9, 0x1800000, 0xc00000);
        Engine_EventWait(60);
        Object_GetById(9)->terrain_id = 2;
        Actor_MoveToAndWait(9, 0x198, 192);
        Engine_EventWait(60);
        FieldScene_RunPillarSequence();
    }
    if (gGameState.pending_djinn_event != 0) {
        GomaHashira_Actor13Frames = 0;
        Task_AddCallback(GomaHashira_DriveActor13Idle, 0xc80);
    }
    return 0;
}
