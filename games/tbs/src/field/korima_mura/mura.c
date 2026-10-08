#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "korima_mura.h"
#include "object_runtime.h"
#include "call.h"
#include "text/msg_ids.h"

void SceneData_InitRecordTable(u8 *o);

#define NULL ((void *)0)

s32 KorimaMura_PendingPose __attribute__((section(".bss")));
s32 KorimaMura_LayoutFlag __attribute__((section(".bss")));
s32 KorimaMura_EffectActive __attribute__((section(".bss")));

struct Obj {
    s32 f00, f04, f08, f0c, f10, f14, f18, f1c;
    s32 f20, f24, f28, f2c, f30, f34, f38, f3c;
    s32 f40, f44, f48, f4c, f50, f54, f58, f5c;
    s32 f60;
    u16 f64;
};

struct Ent {
    u8 pad00[6];
    u16 f06;
    s32 f08;
    u8 pad0c[4];
    s32 f10;
    u8 pad14[0x46];
    u8 f5a;
    u8 pad5b[13];
    struct Ent *f68;
};

struct Rec { u16 f00, f02, f04, f06; };

struct Ent_02000800 {
    s32 f00, f04, f08, f0c;
    u8 pad10[0x45];
    u8 f55;
};

struct Obj_020025d8 {
    s32 f00, f04, f08, f0c, f10, f14;
    s32 f18;
    s32 f1c, f20, f24, f28, f2c, f30, f34;
    s32 f38, f3c, f40;
};

struct Sub {
    u8 pad00[9];
    u8 f09;
    u8 pad0a[28];
    u8 f26;
};

s32 ArcTan2(s32, s32);
void BattleFx_RunPageEffectForSlot(s32, s32, s32);
void BattleEffect_CleanupSceneObjects(void);
void Object_RefreshSelectorById();
void FieldScene_RunPairedStepA();
void FieldScene_RunPairedStepB();
void Object_SetActionCallbackAndRefreshById();
void Audio_PlayCueFromEventWork();


void FieldScene_ConfigureActor0ThenRun();

void Engine_MapCopyCellAttributes();
void Object_SetTargetAndCallback();

void Event_CallWithLastActiveObjectId(u8 *script);

s32 KorimaMura_ApplyPendingPose(s32 arg0)
{
    if (KorimaMura_PendingPose != 0) {
        Object_SetMode(arg0, 2);
        KorimaMura_PendingPose = 0;
    }
    return 1;
}

s32 SceneEffect_AdvanceCounterAndSwitchMode(struct Obj *p)
{
    s32 v = Engine_RandomNext();
    s32 t = v * 100;
    s32 h = p->f64 + ((u32)t >> 16);
    p->f64 = h;
    if ((s16)h > 1000) {
        ObjectGroup_SetChildValue(p, 7);
    } else {
        ObjectGroup_SetChildValue(p, 10);
    }
    if ((s16)p->f64 > 1200) {
        p->f64 = 0;
    }
    return 1;
}

void SceneData_InitRecordTable(u8 *o)
{
    u8 *p = o + 72;
    u32 i;
    s32 normal;
    s32 special;

    i = 0;
    normal = 105;
    special = 110;
    for (; i <= 8; i++) {
        *(u16 *)p = normal;
        if ((u32)(i - 6) <= 1) {
            *(u16 *)p = special;
        }
        p[22] = 2;
        *(s32 *)(p + 4) = 1;
        p += 24;
    }
}

s32 SceneEffect_UpdateFallingObject(struct Obj *p)
{
    p->f08 += p->f24;
    p->f10 += p->f2c;
    p->f2c -= 2621;
    p->f18 += 0x600;
    p->f1c += 0x600;
    {
        s32 t = p->f64 - 1;
        p->f64 = t;
        if ((u16)t == 0) {
            Engine_ObjectDispatchRelease(p);
        }
    }
    return 1;
}

s32 SceneActor_TurnTowardTarget(struct Ent *p)
{
    struct Ent *q;
    u16 h;
    s32 t;
    s32 v;
    u8 *b;

    q = p->f68;
    if (q != 0) {
        b = &p->f5a;
        v = 0xfe;
        v &= *b;
        *b = v;
        h = ArcTan2(q->f10 - p->f10, q->f08 - p->f08);
        t = h;
        t -= p->f06;
        t <<= 16;
        t >>= 16;
        if (t != 0) {
            if (t > 0x1000) {
                t = 0x1000;
            }
            if (t < -0x1000) {
                t = -0x1000;
            }
            p->f06 = p->f06 + t;
        }
    }
    return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KorimaMura3) {
        return gKorimaMuraEntrances3;
    }
    if (scene == (s32)&SceneId_KorimaMura2) {
        return gKorimaMuraEntrances2;
    }
    return gKorimaMuraEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    if (gGameState.scene == (s32)&SceneId_KorimaMura2) {
        return gKorimaMuraRegions2;
    }
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return gKorimaMuraExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    s32 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KorimaMura1) {
        if (Engine_GameFlagIsSet(0x845) == 0) {
            SceneData_InitRecordTable(gKorimaMuraPlacements1);
        }
        return gKorimaMuraPlacements1;
    }
    if (scene == (s32)&SceneId_KorimaMura3) {
        return gKorimaMuraPlacements3;
    }
    return gKorimaMuraPlacements;
}

void FieldScene_RunActor16MessageBranch(void)
{
    struct Rec *q = Object_GetById(0);
    s32 v = q->f06;
    Engine_EventBegin();
    if (v >= 0xa001 && v <= 0xdfff) {
        Engine_SanctumOpen(16);
    } else {
        Engine_EventSetMessage((s32)MsgKorimaToldHolyTrees);
        Engine_EventAskYesNo(16, 0);
    }
    Engine_EventEnd();
}

void FieldScene_RunActor27Step(void)
{
    BattleFx_RunPageEffectForSlot(27, 0, 1);
}

const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_KorimaMura3) {
        return gKorimaMuraEvents3;
    }
    return gKorimaMuraEvents;
}

void FieldScene_ConfigureActor0ThenRun(s32 a0)
{
    u32 i;
    s32 record;

    Object_GetById(0)->motion_flags = 0;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
    gEventWork->transition_frames = 16;
    Engine_EventRequestExit(a0);
}

void KorimaMura_AnimateSwitchOrExit(void)
{
    u8 *work = (u8 *)gEventWork;
    s32 x = 0;
    s32 y = 0;

    Engine_EventBegin();
    Engine_AudioPlayCue(158);
    switch (*(s16 *)(work + 0x16c)) {
    case 5:
        x = 71;
        y = 9;
        break;
    case 6:
        x = 73;
        y = 17;
        break;
    case 7:
        x = 80;
        y = 21;
        break;
    case 8:
        x = 84;
        y = 12;
        break;
    case 9:
        Object_GetById(0)->motion_flags = 0;
        Actor_SetSpeed(0, 0x8000, 0x4000);
        Engine_ActorSetDestinationOffset(0, 0, 8);
        *(s32 *)((u8 *)gEventWork + 0x1c8) = 16;
        Engine_EventRequestExit(9);
        Engine_EventEnd();
        return;
    }
    Engine_MapAnimateCells(KorimaMura_SwitchCells, x, y);
    FieldScene_ConfigureActor0ThenRun(*(s16 *)(work + 0x16c));
    Engine_EventEnd();
}

void KorimaMura_RunObjectSpreadScene(void)
{
    u32 i;
    s32 v5;
    s32 v6;

    Engine_EventBegin();
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0xa80000, 0, 0xf60000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Actor_ShowEmote(8, 0x100, 40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventSetMessage((s32)MsgKorimaGoingCrossBridge);
    Engine_EventShowMessage(8, 0);
    Engine_CameraSetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0xa80000, 0, 0xea0000, 1);
    Actor_SetSpeed(0, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(0, 174, 0x116);
    Actor_FaceDirection(0, FACING_NORTHEAST, 20);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(8, 3);
    Engine_EventShowMessage(8, 0);
    Actor_FaceDirection(8, 0x9000, 20);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(20);
    Object_GetById(8)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_SetSpeed(8, 0x20000, 0x10000);
    Engine_ActorJump(8, 2, 0);
    Engine_ObjectMotionSetPositionAndCommit(8, 224, 197);
    Engine_AudioPlayCue(176);
    Engine_EventWait(10);
    Engine_ObjectMotionSetPositionAndCommit(8, 234, 200);
    Engine_EventWait(10);
    Engine_AudioPlayCue(198);
    v5 = 5;
    v6 = 4;
    Engine_EventWait(30);
    Engine_MapCopyCellsTo(91, 0, 72, 9, v5, v6);
    Engine_EventWait(12);
    Engine_MapCopyCellsTo(91, 4, 72, 9, v5, v6);
    Engine_EventWait(9);
    Actor_FaceDirection(0, FACING_NORTH, 0);
    Engine_MapCopyCellsTo(91, 8, 72, 9, v5, v5);
    Engine_EventWait(6);
    Engine_MapCopyCellsTo(91, 13, 72, 9, v5, 6);
    Engine_EventWait(3);
    Engine_AudioPlayCue(188);
    for (i = 0; i < 10; i++) {
        u8 *obj = (u8 *)Engine_ObjectCreate(222, (148 + i * 4) << 16, 0, 0x1020000);

        if (obj != 0) {
            obj[85] = 0;
            Engine_ActorSetSpriteFlags(obj, 0);
            ((struct Flags9 *)*(u8 **)(obj + 80))->mode = 0;
            {
                u16 delay = (((u32)(Engine_RandomNext() * 40)) >> 16) + 40;

                *(u16 *)(obj + 100) = delay;
            }
            *(s32 *)(obj + 36) = (s32)(((i & 3) << 16) + 0x10000) >> 1;
            *(s32 *)(obj + 44) = 0x10000;
            if (i & 1) {
                *(s32 *)(obj + 36) = -((s32)(((i & 3) << 16) + 0x10000) >> 1);
            }
            Object_SetMode(obj, 1);
            Engine_ObjectSetScript((s32)obj, (s32)KorimaMura_DebrisScript);
        }
    }
    Map_CopyCellsTo(91, 19, 72, 9, 5, 7);
    Map_CopyCellAttributes(23, 11, 5, 7, 8, 11);
    Work_SetValuesIfNonNegative(0, 0x40000, 0x10000);
    Engine_EventWait(10);
    Engine_ActorJump(0, 6, 0);
    Engine_EventWait(20);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Call3(Object_SetTargetAndCallback, 8, 0x10000, (s32)KorimaMura_Actor8Path);
    Engine_EventWait(60);
    Engine_ActorShowEmote(0, 0x102, 60);
    Engine_GameFlagSet(0x847);
    Engine_EventEnd();
}

s32 Scene_Initialize(void)
{
    s16 scene;
    u32 actor;
    u8 *tbl;
    s32 x;
    s32 y;

    scene = gGameState.scene;

    if (scene == (s32)&SceneId_KorimaMura3) {
        FieldScene_RunFlag845And847Branches();
        return 0;
    }

    if (scene == (s32)&SceneId_KorimaMura2) {
        *(s32 *)((u8 *)gEventWork + 0x1c0) = 0x204;
        return 0;
    }

    Engine_ActorSetSpriteFlags(Object_GetById(23), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(24), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(25), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(26), 0);

    tbl = KorimaMura_EntryActions;
    Engine_ActorEnableActionCallback(23, tbl);
    Engine_ActorEnableActionCallback(24, tbl);
    Engine_ActorEnableActionCallback(25, tbl);
    Engine_ActorEnableActionCallback(26, tbl);

    if (Engine_GameFlagIsSet(0x845) == 0) {
        for (actor = 8; actor <= 16; actor++) {
            Engine_ActorSetSpriteFlags(Object_GetById(actor), 0);
        }
        Engine_MapCopyCellAttributes(13, 9, 1, 1, 13, 8);
        Engine_MapCopyCellAttributes(13, 9, 1, 1, 15, 8);
        x = 14;
        y = 9;
        Engine_MapCopyCellAttributes(13, 9, 1, 1, x, y);
    }

    if (Engine_GameFlagIsSet(0x843) == 0) {
        if (gGameState.entrance == 1) {
            FieldScene_RunExtendedActorSequence();
        }
    }

    if (Engine_GameFlagIsSet(0x843) != 0) {
        Engine_ActorDestroy(GERALD);
        Engine_ActorDestroy(IWAN);
        Engine_ActorDestroy(MEARI);
        Engine_ActorDestroy(17);
        Engine_ActorDestroy(18);
        Engine_ActorDestroy(19);
        Engine_ActorDestroy(20);
        Engine_ActorDestroy(21);
        Engine_ActorDestroy(22);
        Event_CallWithLastActiveObjectId(KorimaMura_EntryScript);
    }

    return 0;
}

void SceneActor_SetActors19To22HeightByFrameParity(void)
{
    struct Ent_02000800 *p;

    p = Object_GetById(19);
    if (p != 0) {
        s32 m;
        p->f55 = 0;
        m = gFrameCount & 1;
        if (m == 0) {
            p->f0c = m;
        } else {
            p->f0c = 0x1f40000;
        }
    }
    p = Object_GetById(20);
    if (p != 0) {
        s32 z = 0;
        p->f55 = z;
        if (gFrameCount & 1) {
            p->f0c = z;
        } else {
            p->f0c = 0x1f40000;
        }
    }
    p = Object_GetById(21);
    if (p != 0) {
        s32 m;
        p->f55 = 0;
        m = gFrameCount & 1;
        if (m == 0) {
            p->f0c = m;
        } else {
            p->f0c = 0x1f40000;
        }
    }
    p = Object_GetById(22);
    if (p != 0) {
        s32 z = 0;
        p->f55 = z;
        if (gFrameCount & 1) {
            p->f0c = z;
        } else {
            p->f0c = 0x1f40000;
        }
    }
}

void FieldScene_StartEffect141Sequence(s32 arg0, s32 arg1)
{
    Engine_PsynergyBegin(141, 1);
    Engine_PsynergySetTarget(arg0, arg1);
    Engine_PsynergyRaiseHands();
    Engine_PsynergyPlayEffect(1);
    Engine_TaskWait(1);
}

void FieldScene_RunSequenceA(void)
{
    Engine_PsynergyPlayEffect(2);
    Engine_PsynergyLowerHands();
    BattleEffect_CleanupSceneObjects();
}

void FieldScene_RunExtendedActorSequence(void)
{
    struct FieldActor *actor;
    s32 *layout;
    s32 mask;
    s32 value;
    const u8 *actions_a;
    const u8 *actions_b;
    s32 *falling;
    s32 *work;
    const u8 *actions_c;
    const u8 *actions_d;
    s32 *again;

    Engine_EventBegin();
    layout = &KorimaMura_LayoutFlag;
    *layout = GameFlag_IsSet(3);
    actor = Object_GetById(19);
    Engine_ActorSetSpriteFlags(actor, 0);
    actor = Object_GetById(20);
    Engine_ActorSetSpriteFlags(actor, 0);
    actor = Object_GetById(21);
    Engine_ActorSetSpriteFlags(actor, 0);
    actor = Object_GetById(22);
    Engine_ActorSetSpriteFlags(actor, 0);
    Camera_MoveTo(0x680000, -1, 0x1000000, 0);
    Engine_MapRedraw();
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x170000, 0xf70000);
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 121, 238);
    Actor_SetSpeed(GERALD, 0x9999, 0x4ccc);
    Actor_SetSpeed(IWAN, 0x9999, 0x4ccc);
    actor = Object_GetById(0);
    if (actor != NULL) {
        Actor_SetPosition(GERALD, actor->x.fixed, actor->z.fixed);
    }
    actor = Object_GetById(0);
    if (actor != NULL) {
        Actor_SetPosition(IWAN, actor->x.fixed, actor->z.fixed);
    }
    Engine_ActorEnableActionCallback(GERALD, KorimaMura_ActionTable1);
    Engine_ActorEnableActionCallback(IWAN, KorimaMura_ActionTable2);
    if (*layout != 0) {
        Actor_SetSpeed(MEARI, 0x9999, 0x4ccc);
        actor = Object_GetById(0);
        if (actor != NULL) {
            Actor_SetPosition(MEARI, actor->x.fixed, actor->z.fixed);
        }
        Engine_ActorEnableActionCallback(MEARI, KorimaMura_ActionTable3);
    }
    Object_RefreshSelectorById(2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
    Actor_FaceDirection(IWAN, FACING_NORTHEAST, 20);
    Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 20);
    layout = &KorimaMura_LayoutFlag;
    if (*layout != 0) {
        Actor_FaceDirection(MEARI, FACING_SOUTHEAST, 0);
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
    Actor_FaceDirection(IWAN, FACING_SOUTHEAST, 20);
    Actor_FaceDirection(GERALD, FACING_NORTHEAST, 60);
    if (*layout != 0) {
        Actor_FaceDirection(MEARI, FACING_NORTHEAST, 0);
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceActor(IWAN, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(GERALD, ACTOR_PARTY_LEADER, 0);
    if (*layout != 0) {
        Actor_FaceActor(MEARI, ACTOR_PARTY_LEADER, 0);
    }
    Engine_EventWait(40);
    Engine_ActorStartRepeatedMotion(GERALD, 1);
    Engine_EventSetMessage((s32)MsgKorimaQuiet);
    FieldScene_RunPairedStepA(1, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, GERALD, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(30);
    Actor_WalkToAndWait(IWAN, 72, 0x11e);
    Actor_WalkToAndWait(IWAN, 72, 0x12e);
    Actor_WalkToAndWait(IWAN, 88, 0x136);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    Actor_FaceActor(IWAN, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
    Actor_FaceActor(GERALD, IWAN, 0);
    if (*layout != 0) {
        Actor_FaceActor(MEARI, IWAN, 0);
    }
    Engine_EventWait(30);
    Engine_ActorSetAnimation(GERALD, 3);
    if (*layout != 0) {
        Engine_ActorSetAnimation(MEARI, 3);
    }
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Engine_EventWait(30);
    Actor_FaceDirection(IWAN, FACING_EAST, 0);
    Engine_EventWait(20);
    FieldScene_StartEffect141Sequence(2, 9);
    Engine_EventWait(40);
    FieldScene_RunSequenceA();
    Actor_ShowEmote(IWAN, 0x100, 40);
    Actor_SetSpeed(IWAN, 0x10000, 0x8000);
    ((struct ObjectRuntime *)Object_GetById(2))->action_flags &= 254;
    Actor_WalkToAndWait(IWAN, 80, 0x136);
    mask = 1;
    Engine_EventWait(1);
    ((struct ObjectRuntime *)Object_GetById(2))->action_flags |= mask;
    Actor_ShowEmote(GERALD, 0x102, 40);
    FieldScene_RunPairedStepA(1, 20);
    Actor_ShowEmote(IWAN, 0x102, 40);
    FieldScene_RunPairedStepA(2, 20);
    if (*layout != 0) {
        Actor_FaceActor(MEARI, ACTOR_PARTY_LEADER, 0);
    }
    Engine_ActorFaceEachOther(IWAN, GERALD, 60);
    if (*layout != 0) {
        Actor_FaceDirection(MEARI, FACING_NORTH, 0);
    }
    Actor_FaceDirection(GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
    Engine_ActorStartRepeatedMotion(GERALD, 1);
    FieldScene_RunPairedStepA(1, 20);
    Actor_SetSpeed(IWAN, 0x8000, 0x4000);
    ((struct ObjectRuntime *)Object_GetById(2))->action_flags &= 254;
    Actor_WalkToAndWait(IWAN, 72, 0x11e);
    Engine_EventWait(1);
    ((struct ObjectRuntime *)Object_GetById(2))->action_flags |= mask;
    Engine_ActorEnableActionCallback(IWAN, KorimaMura_ActionTable2);
    if (*layout != 0) {
        Actor_ShowEmote(MEARI, 0x105, 0);
        Engine_EventWait(60);
        FieldScene_RunPairedStepA(3, 20);
        Engine_ActorSetAnimation(MEARI, 3);
    } else {
        gEventWork->message++;
    }
    Actor_FaceActor(IWAN, ACTOR_PARTY_LEADER, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Engine_EventWait(20);
    Actor_SetAttachedEffect(GERALD, 0x102);
    Engine_EventWait(10);
    FieldScene_RunPairedStepB(1, 0x2000, 10);
    FieldScene_RunPairedStepB(0, 0xa000, 10);
    Event_OpenMessage(GERALD, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_ActorSetAnimation(GERALD, 3);
    } else {
        Engine_ActorSetAnimationAndWait(GERALD, 4);
        gEventWork->message++;
    }
    FieldScene_RunPairedStepA(1, 40);
    FieldScene_RunPairedStepB(2, 0x2000, 40);
    FieldScene_RunPairedStepB(2, 0x8000, 20);
    FieldScene_RunPairedStepB(2, 0x4000, 40);
    Actor_ShowEmote(IWAN, 0x101, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
    FieldScene_RunPairedStepB(0, 0x6000, 60);
    value = 160;
    FieldScene_RunPairedStepB(3, 0x2000, 10);
    Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
    FieldScene_RunPairedStepB(0, (value << 8), 10);
    Actor_ShowEmote(GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Engine_EventWait(40);
    Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
    FieldScene_RunPairedStepB(0, 0x6000, 10);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Engine_EventSetMessage((s32)MsgKorimaWhatIsIt);
    FieldScene_RunPairedStepA(1, 10);
    FieldScene_RunPairedStepB(2, 0xc000, 20);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    FieldScene_RunPairedStepA(2, 20);
    FieldScene_RunPairedStepB(1, 0, 20);
    FieldScene_RunPairedStepB(0, (value << 8), 40);
    FieldScene_RunPairedStepB(1, 0x4000, 20);
    FieldScene_RunPairedStepB(0, 0x6000, 30);
    FieldScene_RunPairedStepB(1, 0x6000, 20);
    FieldScene_RunPairedStepB(0, 0xe000, 30);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Actor_ShowEmote(IWAN, 0x100, 0);
    Engine_EventWait(40);
    Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
    FieldScene_RunPairedStepB(0, 0x6000, 20);
    FieldScene_RunPairedStepB(2, 0xc000, 10);
    Audio_PlayCue(17);
    Audio_PlayCue(206);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    Engine_ColorBufferInterpolate(1);
    Engine_TaskWait(1);
    KorimaMura_EffectActive = 1;
    Engine_TaskAddCallback(SceneEffect_SpawnObject26EveryEightFrames, 0xc80);
    Engine_TaskWait(20);
    ColorBuffer_ApplyTarget(0x405210, 1);
    ColorBuffer_ApplyTarget(0x10000, 2);
    Engine_ColorBufferInterpolate(120);
    Engine_TaskWait(60);
    actions_a = KorimaMura_ActionTable4;
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, actions_a);
    Engine_ActorEnableActionCallback(GERALD, actions_a);
    Engine_ActorEnableActionCallback(IWAN, actions_a);
    Engine_ActorEnableActionCallback(MEARI, actions_a);
    Engine_EventWait(100);
    FieldScene_RunPairedStepA(1, 20);
    FieldScene_RunPairedStepA(2, 40);
    if (KorimaMura_LayoutFlag != 0) {
        Engine_EventWait(40);
        Actor_SetAttachedEffect(MEARI, 0x102);
        Engine_EventWait(40);
        FieldScene_RunPairedStepA(3, 40);
    } else {
        gEventWork->message++;
    }
    Engine_EventWait(20);
    if (KorimaMura_LayoutFlag != 0) {
        value = 128;
        actor = Object_GetById(3);
        actor->velocity_y = (value << 10);
        Engine_EventWait(10);
        Actor_SetSpeed(MEARI, (value << 10), (value << 10));
        Actor_SetDestinationOffset(MEARI, -2, 0);
        Engine_ActorEnableActionCallback(MEARI, KorimaMura_ActionTable5);
        actor = Object_GetById(3);
        Engine_ActorSetSpriteFlags(actor, 0);
        Engine_ActorSetAnimation(MEARI, 19);
        Engine_EventWait(10);
    }
    value = 128;
    actor = Object_GetById(0);
    actor->velocity_y = (value << 10);
    Engine_EventWait(10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, (value << 10), (value << 10));
    actions_b = KorimaMura_ActionTable5;
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, actions_b);
    actor = Object_GetById(0);
    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
    Engine_EventWait(20);
    actor = Object_GetById(1);
    actor->velocity_y = (value << 10);
    Engine_EventWait(10);
    Actor_SetSpeed(GERALD, (value << 10), (value << 10));
    Engine_ActorEnableActionCallback(GERALD, actions_b);
    actor = Object_GetById(1);
    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ActorSetAnimation(GERALD, 19);
    Engine_EventWait(40);
    actor = Object_GetById(2);
    actor->velocity_y = (value << 10);
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(IWAN, actions_b);
    actor = Object_GetById(2);
    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ActorSetAnimation(IWAN, 19);
    KorimaMura_EffectActive = 0;
    Engine_EventWait(160);
    Engine_TaskRemoveCallback(SceneEffect_SpawnObject26EveryEightFrames);
    Engine_EventWait(120);
    ColorBuffer_ApplyTarget(0x406218, 1);
    Engine_ColorBufferInterpolate(60);
    Engine_TaskWait(60);
    gFallingEffectWidth = 0;
    falling = &gFallingEffectState;
    gFallingEffectOffset = 0x800000;
    *falling = 1;
    Engine_TaskAddCallback(FieldScene_UpdateFallingEffect, 0xc80);
    Engine_EventWait(180);
    Audio_PlayCue(21);
    FieldScene_RunPairedStepA(1, 80);
    FieldScene_RunPairedStepA(2, 40);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(GERALD, 0x102);
    Actor_SetAttachedEffect(IWAN, 0x102);
    Actor_SetAttachedEffect(MEARI, 0x102);
    Engine_EventWait(60);
    FieldScene_RunPairedStepA(2, 20);
    *falling = 2;
    Engine_ActorStartRepeatedMotion(IWAN, 2);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(GERALD, 1);
    Engine_EventWait(40);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(MEARI, 1);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(IWAN, 3);
    Engine_EventWait(40);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(MEARI, 2);
    Actor_SetAttachedEffect(GERALD, 0x102);
    FieldScene_RunPairedStepA(1, 20);
    if (KorimaMura_LayoutFlag != 0) {
        Actor_SetAttachedEffect(MEARI, 0x102);
        FieldScene_RunPairedStepA(3, 10);
    } else {
        gEventWork->message++;
    }
    work = &gFallingEffectState;
    *work = 3;
    Object_GetById(0)->priority_flags &= 254;
    Object_GetById(1)->priority_flags &= 254;
    Object_GetById(2)->priority_flags &= 254;
    Object_GetById(3)->priority_flags &= 254;
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetSpritePriority(GERALD, 3);
    Engine_ActorSetSpritePriority(IWAN, 3);
    value = 0;
    Engine_ActorSetSpritePriority(MEARI, 3);
    KorimaMura_PendingPose = value;
    Engine_TaskAddCallback(SceneActor_SetActors19To22HeightByFrameParity, 0xc80);
    Audio_PlayCue(220);
    Object_GetById(19)->priority_flags &= 254;
    Engine_ActorSetSpritePriority(19, 2);
    Actor_SetPosition(19, 0x780000, 0xf80000);
    actions_c = KorimaMura_ActionTable6;
    Engine_ActorEnableActionCallback(19, actions_c);
    Object_GetById(20)->priority_flags &= 254;
    Engine_ActorSetSpritePriority(20, 2);
    Actor_SetPosition(20, 0x640000, 0x1120000);
    Engine_ActorEnableActionCallback(20, actions_c);
    if (KorimaMura_LayoutFlag != 0) {
        Object_GetById(21)->priority_flags &= 254;
        Engine_ActorSetSpritePriority(21, 2);
        Actor_SetPosition(21, 0x4a0000, 0xfe0000);
        Engine_ActorEnableActionCallback(21, actions_c);
    }
    Object_GetById(22)->priority_flags &= 254;
    Engine_ActorSetSpritePriority(22, 2);
    Actor_SetPosition(22, 0x5e0000, 0xe10000);
    Engine_ActorEnableActionCallback(22, actions_c);
    if (*work != 0) {
        do {
            Engine_TaskWait(1);
        } while (gFallingEffectState != 0);
    }
    Engine_EventWait(0x12c);
    Engine_TaskRemoveCallback(FieldScene_UpdateFallingEffect);
    Engine_EventWait(120);
    Audio_PlayCue(17);
    ColorBuffer_ApplyTarget(0x10000, 1);
    Engine_ColorBufferInterpolate(60);
    Engine_TaskWait(60);
    Engine_ActorStop(19);
    Engine_ActorStop(20);
    if (KorimaMura_LayoutFlag != 0) {
        Engine_ActorStop(21);
    }
    Engine_ActorStop(22);
    Engine_TaskWait(1);
    actions_d = KorimaMura_ActionTable7;
    Engine_ActorEnableActionCallback(19, actions_d);
    Engine_ActorEnableActionCallback(20, actions_d);
    if (KorimaMura_LayoutFlag != 0) {
        Engine_ActorEnableActionCallback(21, actions_d);
    }
    Object_SetActionCallbackAndRefreshById(22, actions_d);
    Engine_EventWait(80);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Engine_EventWait(40);
    Event_OpenMessage(GERALD, 0);
    Actor_SetPosition(17, 0x570000, 0x8b0000);
    Actor_SetPosition(18, 0x570000, 0x8b0000);
    Engine_TaskWait(1);
    if (Engine_EventChooseYesNo(17, 0) == 1) {
        gEventWork->message++;
    }
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    FieldScene_RunPairedStepA(2, 20);
    if (KorimaMura_LayoutFlag != 0) {
        Engine_ActorRunRepeatedMotion(MEARI, 2);
        Engine_EventWait(10);
        Engine_EventSetMessage((s32)MsgKorimaKnowThoseFieldsWere);
        FieldScene_RunPairedStepA(3, 40);
    }
    Engine_ActorStartRepeatedMotion(GERALD, 1);
    Actor_ShowEmote(GERALD, 0x101, 0);
    Engine_EventWait(80);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Engine_EventSetMessage((s32)MsgKorimaWasOurPsynergy);
    FieldScene_RunPairedStepA(2, 40);
    Engine_ActorRunRepeatedMotion(GERALD, 3);
    Engine_EventWait(40);
    Engine_ActorSetSpritePriority(GERALD, 2);
    Object_GetById(1)->priority_flags |= 1;
    actor = Object_GetById(1);
    Engine_ActorSetSpriteFlags(actor, 1);
    Engine_ActorJump(GERALD, 6, 0);
    Engine_ActorSetAnimation(GERALD, 1);
    FieldScene_RunPairedStepB(1, 0x4000, 60);
    FieldScene_RunPairedStepA(1, 20);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    FieldScene_RunPairedStepA(1, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
    FieldScene_RunPairedStepB(1, 0x2000, 20);
    Actor_ShowEmote(GERALD, 0x101, 0);
    Engine_EventWait(40);
    FieldScene_RunPairedStepB(1, 0x6000, 40);
    FieldScene_RunPairedStepB(1, 0x2000, 20);
    FieldScene_RunPairedStepB(1, 0x6000, 20);
    FieldScene_RunPairedStepB(1, 0x2000, 10);
    Engine_ActorJump(GERALD, 2, 0);
    Engine_EventWait(40);
    Engine_ActorJump(GERALD, 2, 0);
    Engine_EventWait(10);
    Engine_ActorJump(GERALD, 4, 0);
    Engine_EventWait(20);
    FieldScene_RunPairedStepA(1, 20);
    if (KorimaMura_LayoutFlag != 0) {
        Actor_ShowEmote(MEARI, 0x100, 0);
        Engine_EventWait(60);
        Engine_ActorRunRepeatedMotion(MEARI, 2);
        Engine_EventWait(80);
        Engine_ActorSetSpritePriority(MEARI, 2);
        Object_GetById(3)->priority_flags |= 1;
        actor = Object_GetById(3);
        Engine_ActorSetSpriteFlags(actor, 1);
        Engine_ActorJump(MEARI, 4, 0);
        Actor_SetDestinationOffset(MEARI, -2, 0);
        Engine_ActorSetAnimation(MEARI, 1);
        FieldScene_RunPairedStepB(3, 0xe000, 60);
        Engine_ActorRunRepeatedMotion(MEARI, 2);
        Engine_EventWait(20);
        FieldScene_RunPairedStepA(3, 20);
    } else {
        gEventWork->message++;
    }
    Engine_ActorJump(GERALD, 2, 0);
    FieldScene_RunPairedStepB(1, 0x4000, 20);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    FieldScene_RunPairedStepB(1, 0x2000, 10);
    FieldScene_RunPairedStepA(1, 20);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Engine_EventWait(20);
    value = 1;
    Engine_ActorSetSpritePriority(IWAN, 2);
    Object_GetById(2)->priority_flags |= value;
    actor = Object_GetById(2);
    Engine_ActorSetSpriteFlags(actor, 1);
    Engine_ActorJump(IWAN, 4, 0);
    Engine_ActorSetAnimation(IWAN, 1);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(10);
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
    Object_GetById(0)->priority_flags |= value;
    actor = Object_GetById(0);
    Engine_ActorSetSpriteFlags(actor, 1);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    FieldScene_RunPairedStepB(0, 0x6000, 60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
    Actor_ShowEmote(IWAN, 0x105, 0);
    Engine_EventWait(60);
    FieldScene_RunPairedStepB(0, 0xa000, 20);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    FieldScene_RunPairedStepB(0, 0x6000, 10);
    FieldScene_RunPairedStepB(1, 0x4000, 10);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    FieldScene_RunPairedStepA(2, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(MEARI, 3);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    Engine_EventWait(20);
    Event_OpenMessage(IWAN, 0);
    Engine_ActorSetAnimation(IWAN, 3);
    Actor_FaceDirection(IWAN, FACING_NORTHEAST, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorSetAnimation(IWAN, 3);
        Engine_ActorSetAnimationAndWait(GERALD, 3);
        gEventWork->message++;
    } else {
        Engine_ActorRunRepeatedMotion(GERALD, 2);
        FieldScene_RunPairedStepB(1, 0x2000, 10);
        Engine_ActorStartRepeatedMotion(GERALD, 2);
        Event_ShowMessage(GERALD, 0);
    }
    FieldScene_RunPairedStepB(1, 0x4000, 10);
    Engine_ActorSetAnimationAndWait(GERALD, 4);
    FieldScene_RunPairedStepA(1, 20);
    FieldScene_RunPairedStepB(2, 0xc000, 10);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    FieldScene_RunPairedStepA(2, 10);
    if (KorimaMura_LayoutFlag != 0) {
        Engine_ActorRunRepeatedMotion(MEARI, 2);
        FieldScene_RunPairedStepB(3, 0, 20);
        FieldScene_RunPairedStepB(3, 0x2000, 10);
        Engine_ActorSetAnimation(MEARI, 4);
        FieldScene_RunPairedStepA(3, 10);
    } else {
        gEventWork->message++;
    }
    Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
    FieldScene_RunPairedStepB(0, 0xa000, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    value = 128;
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    FieldScene_RunPairedStepB(1, (value << 7), 10);
    Engine_ActorSetAnimationAndWait(IWAN, 4);
    Engine_EventWait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(GERALD, 0x102, 80);
    FieldScene_RunPairedStepB(2, 0xe000, 10);
    Engine_ActorStartRepeatedMotion(IWAN, 2);
    FieldScene_RunPairedStepA(2, 20);
    Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
    FieldScene_RunPairedStepB(0, 0xa000, 40);
    Actor_FaceDirection(GERALD, (value << 7), 0);
    FieldScene_RunPairedStepB(0, 0x6000, 10);
    FieldScene_RunPairedStepB(2, 0xc000, 10);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    FieldScene_RunPairedStepA(2, 10);
    Actor_SetAttachedEffect(GERALD, 0x102);
    Engine_EventWait(40);
    FieldScene_RunPairedStepA(1, 20);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Engine_EventWait(20);
    Actor_SetAttachedEffect(GERALD, 0x102);
    Engine_EventWait(40);
    FieldScene_RunPairedStepA(1, 20);
    Engine_ActorSetAnimation(IWAN, 3);
    FieldScene_RunPairedStepA(2, 10);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    FieldScene_RunPairedStepB(1, 0x2000, 10);
    Event_OpenMessage(GERALD, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorSetAnimationAndWait(GERALD, 3);
    } else {
        Engine_EventWait(20);
        Engine_ActorRunRepeatedMotion(GERALD, 2);
        Engine_EventWait(40);
        gEventWork->message++;
    }
    Event_ShowMessage(GERALD, 0);
    Audio_PlayCue(21);
    ColorBuffer_ApplyTarget(0x406218, 1);
    Engine_ColorBufferInterpolate(60);
    Engine_TaskWait(60);
    gFallingEffectWidth = 0;
    gFallingEffectOffset = 0x800000;
    again = &gFallingEffectState;
    *again = 1;
    Engine_TaskAddCallback(FieldScene_UpdateFallingEffect, 0xc80);
    Engine_EventWait(80);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    Engine_ActorStartRepeatedMotion(MEARI, 2);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Engine_EventWait(60);
    FieldScene_RunPairedStepB(2, 0xc000, 10);
    Engine_EventSetMessage((s32)MsgKorimaWatchOutItsHappeningAgain);
    FieldScene_RunPairedStepA(2, 10);
    FieldScene_RunPairedStepB(1, 0xc000, 10);
    FieldScene_RunPairedStepB(0, 0xc000, 10);
    if (KorimaMura_LayoutFlag != 0) {
        FieldScene_RunPairedStepB(3, 0xc000, 10);
    }
    Object_GetById(0)->priority_flags &= 254;
    Object_GetById(1)->priority_flags &= 254;
    Object_GetById(2)->priority_flags &= 254;
    Object_GetById(3)->priority_flags &= 254;
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetSpritePriority(GERALD, 3);
    Engine_ActorSetSpritePriority(IWAN, 3);
    Engine_ActorSetSpritePriority(MEARI, 3);
    *again = 2;
    Audio_PlayCue(220);
    Actor_SetPosition(19, 0x780000, 0xf80000);
    actions_c = KorimaMura_ActionTable6;
    Engine_ActorEnableActionCallback(19, actions_c);
    Actor_SetPosition(20, 0x640000, 0x1120000);
    Engine_ActorEnableActionCallback(20, actions_c);
    if (KorimaMura_LayoutFlag != 0) {
        Actor_SetPosition(21, 0x4a0000, 0xfe0000);
        Engine_ActorEnableActionCallback(21, actions_c);
    }
    Actor_SetPosition(22, 0x5e0000, 0xe10000);
    Engine_ActorEnableActionCallback(22, actions_c);
    Engine_EventWait(120);
    *again = 3;
    do {
        Engine_TaskWait(1);
    } while (gFallingEffectState != 0);
    FieldScene_RunPairedStepA(17, 80);
    FieldScene_RunPairedStepA(18, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(GERALD, 0x101, 0);
    Actor_ShowEmote(IWAN, 0x101, 0);
    Actor_ShowEmote(MEARI, 0x101, 0);
    Engine_EventWait(60);
    FieldScene_RunPairedStepA(18, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(MEARI, 3);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    FieldScene_RunPairedStepA(18, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(MEARI, 3);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    FieldScene_RunPairedStepA(18, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(GERALD, 0x100, 0);
    Actor_ShowEmote(IWAN, 0x100, 0);
    Actor_ShowEmote(MEARI, 0x100, 0);
    Engine_EventWait(40);
    FieldScene_RunPairedStepA(17, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
    Actor_FaceDirection(MEARI, FACING_EAST, 0);
    FieldScene_RunPairedStepB(2, 0xc000, 40);
    FieldScene_RunPairedStepA(18, 10);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(GERALD, 0x102);
    Actor_SetAttachedEffect(IWAN, 0x102);
    Actor_SetAttachedEffect(MEARI, 0x102);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    FieldScene_RunPairedStepB(3, 0xc000, 80);
    FieldScene_RunPairedStepA(18, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    FieldScene_RunPairedStepB(3, 0, 40);
    FieldScene_RunPairedStepA(17, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    FieldScene_RunPairedStepB(3, 0xc000, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 4);
    Engine_ActorSetAnimation(GERALD, 4);
    Engine_ActorSetAnimation(MEARI, 4);
    Engine_ActorSetAnimationAndWait(IWAN, 4);
    Engine_EventWait(60);
    FieldScene_RunPairedStepA(18, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(MEARI, 3);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    FieldScene_RunPairedStepA(18, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    FieldScene_RunPairedStepB(3, 0, 20);
    FieldScene_RunPairedStepA(18, 10);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    Engine_ActorStartRepeatedMotion(MEARI, 2);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    Actor_FaceDirection(MEARI, FACING_NORTH, 0);
    FieldScene_RunPairedStepA(18, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    FieldScene_RunPairedStepB(3, 0, 20);
    FieldScene_RunPairedStepA(17, 20);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(GERALD, 0x102);
    Actor_SetAttachedEffect(MEARI, 0x102);
    Actor_SetAttachedEffect(IWAN, 0x102);
    Engine_EventWait(40);
    FieldScene_RunPairedStepA(18, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    FieldScene_RunPairedStepB(3, 0xc000, 10);
    FieldScene_RunPairedStepA(18, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(MEARI, 3);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Engine_EventWait(60);
    Event_ShowMessage(18, 0);
    Event_ShowMessage(17, 0);
    Engine_TaskRemoveCallback(FieldScene_UpdateFallingEffect);
    Engine_EventWait(80);
    ColorBuffer_ApplyTarget(0x10000, 1);
    Engine_ColorBufferInterpolate(60);
    Engine_TaskWait(80);
    Engine_ActorStop(19);
    Engine_ActorStop(20);
    work = &KorimaMura_LayoutFlag;
    Engine_ActorStop(21);
    Engine_ActorStop(22);
    Engine_TaskWait(1);
    actions_d = KorimaMura_ActionTable7;
    Engine_ActorEnableActionCallback(19, actions_d);
    Engine_ActorEnableActionCallback(20, actions_d);
    if (*work != 0) {
        Engine_ActorEnableActionCallback(21, actions_d);
    }
    Object_SetActionCallbackAndRefreshById(22, actions_d);
    Engine_EventWait(20);
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
    Engine_ActorSetSpritePriority(GERALD, 2);
    Engine_ActorSetSpritePriority(IWAN, 2);
    value = 1;
    Engine_ActorSetSpritePriority(MEARI, 2);
    Object_GetById(0)->priority_flags |= value;
    Object_GetById(1)->priority_flags |= value;
    Object_GetById(2)->priority_flags |= value;
    Object_GetById(3)->priority_flags |= value;
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    FieldScene_RunPairedStepB(2, 0xe000, 10);
    Event_OpenMessage(IWAN, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(MEARI, FACING_EAST, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorRunRepeatedMotion(GERALD, 2);
        Engine_EventWait(10);
        Event_OpenMessage(GERALD, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            FieldScene_RunPairedStepB(3, 0, 20);
            Actor_ShowEmote(GERALD, 0x101, 0);
            Actor_ShowEmote(IWAN, 0x101, 0);
            Actor_ShowEmote(MEARI, 0x101, 0);
            Engine_EventWait(40);
            FieldScene_RunPairedStepB(1, 0x4000, 20);
            FieldScene_RunPairedStepA(1, 10);
            FieldScene_RunPairedStepB(2, 0xc000, 20);
            FieldScene_RunPairedStepB(2, 0xe000, 20);
            Engine_ActorSetAnimationAndWait(IWAN, 3);
            FieldScene_RunPairedStepA(2, 20);
            FieldScene_RunPairedStepB(1, 0x2000, 20);
        } else {
            FieldScene_RunPairedStepB(3, 0, 20);
            Actor_ShowEmote(GERALD, 0x102, 0);
            Actor_ShowEmote(IWAN, 0x102, 0);
            Actor_ShowEmote(MEARI, 0x102, 40);
            FieldScene_RunPairedStepB(1, 0x4000, 20);
            Engine_EventSetMessage((s32)MsgKorimaHesAsStumpedAsWe);
            FieldScene_RunPairedStepA(1, 20);
            Engine_ActorSetAnimationAndWait(IWAN, 3);
            FieldScene_RunPairedStepA(2, 20);
        }
        Engine_ActorSetAnimation(MEARI, 3);
        Engine_ActorSetAnimationAndWait(GERALD, 3);
    } else {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(GERALD, 3);
        Engine_EventWait(10);
        Engine_EventSetMessage((s32)MsgKorimaYoureRobinThereIsntMuch);
        FieldScene_RunPairedStepA(1, 10);
        Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
        FieldScene_RunPairedStepB(0, 0x6000, 20);
        Engine_ActorSetAnimation(GERALD, 3);
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(IWAN, 4);
        Event_OpenMessage(IWAN, 0);
        if (Engine_EventChooseYesNo(0, 0) != 0) {
            goto relieved;
        }
        Engine_EventWait(20);
        Actor_ShowEmote(IWAN, 0x103, 0);
        Engine_EventWait(40);
        FieldScene_RunPairedStepB(2, 0xe000, 10);
        FieldScene_RunPairedStepA(2, 10);
        if (KorimaMura_LayoutFlag != 0) {
            FieldScene_RunPairedStepB(3, 0, 10);
            Engine_ActorStartRepeatedMotion(MEARI, 3);
            FieldScene_RunPairedStepA(3, 20);
        } else {
            gEventWork->message++;
        }
        Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
        Actor_SetAttachedEffect(GERALD, 0x102);
        Engine_EventWait(40);
        Engine_ActorRunRepeatedMotion(GERALD, 2);
        FieldScene_RunPairedStepA(1, 20);
        Actor_ShowEmote(GERALD, 0x105, 0);
        Engine_EventWait(120);
        FieldScene_RunPairedStepA(2, 40);
        if (KorimaMura_LayoutFlag != 0) {
            FieldScene_RunPairedStepB(3, 0x2000, 10);
            Engine_ActorSetAnimationAndWait(MEARI, 4);
            FieldScene_RunPairedStepA(3, 10);
        } else {
            gEventWork->message++;
        }
        Engine_EventWait(60);
        Engine_ActorRunRepeatedMotion(IWAN, 2);
        if (KorimaMura_LayoutFlag != 0) {
            FieldScene_RunPairedStepB(2, 0xa000, 40);
            FieldScene_RunPairedStepB(2, 0xe000, 20);
        }
        FieldScene_RunPairedStepA(2, 10);
        Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        Engine_ActorRunRepeatedMotion(GERALD, 2);
        Engine_EventWait(40);
        FieldScene_RunPairedStepA(2, 20);
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Engine_ActorSetAnimationAndWait(GERALD, 3);
        Engine_EventWait(20);
        Engine_ActorSetAnimation(MEARI, 3);
    }
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    goto leave;
relieved:
    Actor_ShowEmote(IWAN, 0x105, 0);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Engine_EventSetMessage((s32)MsgKorimaThatsReliefRobinThoughtYoud);
    FieldScene_RunPairedStepA(2, 20);
    if (KorimaMura_LayoutFlag != 0) {
        FieldScene_RunPairedStepB(3, 0, 10);
        Engine_ActorStartRepeatedMotion(MEARI, 1);
        FieldScene_RunPairedStepA(3, 20);
    } else {
        gEventWork->message++;
    }
    Actor_SetAttachedEffect(GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    FieldScene_RunPairedStepA(1, 20);
    Actor_ShowEmote(IWAN, 0x105, 0);
    Engine_EventWait(80);
    FieldScene_RunPairedStepA(2, 40);
    if (KorimaMura_LayoutFlag != 0) {
        FieldScene_RunPairedStepB(3, 0x2000, 20);
        Engine_ActorSetAnimation(MEARI, 4);
        FieldScene_RunPairedStepA(3, 40);
    } else {
        gEventWork->message++;
    }
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Engine_EventWait(20);
    FieldScene_RunPairedStepA(2, 20);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(40);
    FieldScene_RunPairedStepA(2, 20);
leave:
    Audio_PlayCue(17);
    Actor_SetSpeed(GERALD, 0x13333, 0x9999);
    Actor_SetSpeed(IWAN, 0x13333, 0x9999);
    Actor_SetSpeed(MEARI, 0x13333, 0x9999);
    Engine_ActorSetAnimation(GERALD, 2);
    actor = Object_GetById(0);
    if (actor != NULL) {
        Actor_SetDestination(GERALD, actor->x.part.pixel, actor->z.part.pixel);
    }
    Engine_ActorWaitForMove(GERALD);
    Actor_SetPosition(GERALD, 0, 0);
    Engine_ActorSetAnimation(IWAN, 2);
    actor = Object_GetById(0);
    if (actor != NULL) {
        Actor_SetDestination(IWAN, actor->x.part.pixel, actor->z.part.pixel);
    }
    Engine_ActorWaitForMove(IWAN);
    Actor_SetPosition(IWAN, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(18, 0, 0);
    if (KorimaMura_LayoutFlag != 0) {
        Engine_ActorSetAnimation(MEARI, 2);
        actor = Object_GetById(0);
        if (actor != NULL) {
            Actor_SetDestination(MEARI, actor->x.part.pixel, actor->z.part.pixel);
        }
        Engine_ActorWaitForMove(MEARI);
        Actor_SetPosition(MEARI, 0, 0);
    }
    GameFlag_Set(0x843);
    Audio_PlayCueFromEventWork();
    Engine_EventEnd();
}

void FieldScene_RunPairedStepA(s32 arg0, s32 arg1)
{
    Engine_EventShowMessage(arg0, 0);
    Engine_EventWait(arg1);
}

void FieldScene_RunPairedStepB(s32 arg0, s32 arg1, s32 arg2)
{
    Engine_ActorFaceDirection(arg0, arg1, 0);
    Engine_EventWait(arg2);
}

s32 SceneEffect_AdvanceAngleUntilIdle(struct Obj_020025d8 *p)
{
    p->f18 += 0x1eb8;
    if (p->f38 == 0x80000000 && p->f3c == p->f38 && p->f40 == p->f3c) {
        Engine_ObjectDispatchRelease(p);
    }
    return 1;
}

void SceneEffect_SpawnObject26EveryEightFrames(void)
{
    struct Obj_02002608 *p;
    struct Sub *q;
    s32 f;
    s32 v;
    s32 w;
    s32 c1 = 0x620000;
    s32 c2 = 0x690000;
    s32 c3 = 0x620000;
    s32 c4 = 0x010d0000;

    f = gFrameCount & 7;
    if (f != 0) {
        return;
    }
    if (KorimaMura_EffectActive != 0) {
        Engine_AudioPlayCue(200);
    }
    p = Engine_ObjectCreate(26, c1, 0, c2);
    if (p == 0) {
        return;
    }
    q = p->f50;
    q->f26 = f;
    v = 0xfe;
    v &= p->f23;
    p->f23 = v;
    w = ~12;
    w &= q->f09;
    w |= 4;
    q->f09 = w;
    p->f18 = 0x1999;
    p->f30 = 0x80000;
    p->f34 = 0x80000;
    p->f55 = f;
    Object_SetMode(p, 2);
    Engine_ObjectSetPosition(p, c3, 0, c4);
    Engine_ObjectSetScript(p, KorimaMura_Object26Script);
}

s32 SceneEffect_SetModeByFrameBit1(s32 arg0)
{
    if ((gFrameCount >> 1) & 1) {
        ObjectGroup_SetChildValue(arg0, 10);
    } else {
        ObjectGroup_SetChildValue(arg0, 7);
    }
    return 0;
}

s32 KorimaMura_TriggerRegionScript(struct FieldActor *actor)
{
    if (KorimaMura_LayoutFlag != 0) {
        if (actor->x.fixed > 0x3b0000 && actor->x.fixed < 0x8d0000
            && actor->z.fixed > 0xd30000 && actor->z.fixed < 0x1010000)
            goto hit;
        if (actor->x.fixed > 0x450000 && actor->x.fixed < 0x7a0000
            && actor->z.fixed > 0xc20000 && actor->z.fixed < 0x1150000)
            goto hit;
    } else {
        if (actor->x.fixed > 0x3b0000 && actor->x.fixed < 0x6f0000
            && actor->z.fixed > 0xc20000 && actor->z.fixed < 0xe60000)
            goto hit;
        if (actor->x.fixed > 0x6f0000 && actor->x.fixed < 0x8d0000
            && actor->z.fixed > 0xd80000 && actor->z.fixed < 0xfa0000)
            goto hit;
        if (actor->x.fixed > 0x4e0000 && actor->x.fixed < 0x7a0000
            && actor->z.fixed > 0xf10000 && actor->z.fixed < 0x1150000)
            goto hit;
    }
    return 0;
hit:
    Engine_AudioPlayCue(106);
    /* FAKEMATCH: the do/while loads the script address before the actor. */
    do {
        Engine_ObjectSetScript(actor, KorimaMura_RegionScript);
    } while (0);
    KorimaMura_PendingPose = 1;
    return 0;
}
