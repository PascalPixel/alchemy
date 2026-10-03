#include "TYPES.H"
#include "EDITION.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "STAGED_ACTOR.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"
#include "CALL.H"
s32 SceneActor_FaceTowardActorZero();
void SceneActor_PublishMarkerBySlotZeroHeight(void);
void ActorPresentation_PlaceActorFourteenOnActorNine(void);
void TakaraAshiba_RaiseTriggerOnStand(void);
void SceneState_TriggerColumnTen(void);
void SceneState_TriggerColumnNineteen(void);
/* CALL.H keeps each scheduled callback's original argument order. */

enum {
    /* Message 0x182 + 243. */
    ITEM_RED_KEY = 243,
    /* Message 0x182 + 244. */
    ITEM_BLUE_KEY = 244
};

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct Frame {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct Slot {
    u16 f00;
    u16 f02;
    u16 f04;
    u16 f06;
};

void TakaraAshiba_OpenPassage();
void Korosseo_ShowItemIcon();
void TakaraAshiba_DispatchByActorEightColumn();
void TakaraAshiba_UpdateBlockRects();
void ObjectDispatch_ApplyValueToChildren();

/*
 * Distance between two three-component 16.16 fixed-point positions.
 *
 * Each argument walks three consecutive 16.16 words in x, y, z order. The
 * per-axis deltas are taken in fixed point, shifted down to integers, squared,
 * and summed; the total is passed to the resident IWRAM integer square root.
 *
 * Expressions are preserved exactly as reconstructed: the walking-pointer form
 * is load-bearing for byte-identity and must not become struct field access.
 */

void FieldScene_RunScene3b4_02000fdc(s32 a0);

s32 *Engine_GetTriggerActor(s32 slot);
s32 Engine_TestTriggerFlag(s32 flag);
void Engine_SetTriggerFlag(s32 flag);

static __inline__ void SceneState_StoreStep(s16 *field, s32 step)
{
    *field = step;
}
void SceneActor_MarkSlot13AndSetFlag200(void);
void SceneActor_RunWhenActor9AtTile45x43(void);
void ActorPresentation_RepaintCellsAtActorsElevenAndTwelve(void);
void FieldScene_RunSingleStep(void);
void FieldScene_CallHelper3c70(void);

void FieldScene_RunScene3b4_02002188(void);
void FieldScene_RunScene3b4_02002290(void);
void FieldScene_RunScene3b4_02002334(void);

void FieldScene_RunScene3b4_02002188(void)
{
    u32 i;
    s32 record;

    Engine_TaskWait(1);
    Korosseo_ShowItemIcon(12, 243);
    Korosseo_ShowItemIcon(11, 244);
    Korosseo_ShowItemIcon(10, 244);
    Korosseo_ShowItemIcon(9, 244);
    Korosseo_ShowItemIcon(8, 244);
    if (Engine_GameFlagIsSet(0xee7) == 0) {
        Engine_ActorSetPosition(8, 0xe80000, 0x3680000);
    }
    if (Engine_GameFlagIsSet(0xee8) == 0) {
        Engine_ActorSetPosition(9, 0x1280000, 0x3380000);
    }
    if (Engine_GameFlagIsSet(0xee9) == 0) {
        Engine_ActorSetPosition(10, 0x1480000, 0x2f80000);
    }
    if (Engine_GameFlagIsSet(0xeea) == 0) {
        Engine_ActorSetPosition(11, 0x1680000, 0x3680000);
    }
    if (Engine_GameFlagIsSet(0x9c0) != 0) {
        TakaraAshiba_OpenPassage(0);
    }
    if (Engine_GameFlagIsSet(0x9c1) != 0) {
        TakaraAshiba_OpenPassage(1);
    }
    if (Engine_GameFlagIsSet(0x9c2) != 0) {
        TakaraAshiba_OpenPassage(2);
    }
    if (Engine_GameFlagIsSet(0x9c3) != 0) {
        TakaraAshiba_OpenPassage(3);
    }
    if (Engine_GameFlagIsSet(0x9c4) != 0) {
        FieldScene_RunScene3b4_02000fdc(0);
    }
}

void FieldScene_RunScene3b4_02002290(void)
{
    s32 record;

    *(u8 *)((s32)Object_GetById(8) + 89) = 1;
    *(u8 *)((s32)Object_GetById(9) + 89) = 1;
    *(u8 *)((s32)Object_GetById(10) + 89) = 1;
    *(u8 *)((s32)Object_GetById(11) + 89) = 1;
    record = Engine_GetTriggerActor(8);
    *(s32 *)(record + 24) = 0xb333;
    record = Engine_GetTriggerActor(9);
    *(s32 *)(record + 24) = 0xb333;
    record = Engine_GetTriggerActor(10);
    *(s32 *)(record + 24) = 0xb333;
    record = Engine_GetTriggerActor(11);
    *(s32 *)(record + 24) = 0xb333;
    record = Engine_GetTriggerActor(12);
    *(s32 *)(record + 24) = 0xb333;
    Call2(Scheduler_AddOrUpdateCallback, (s32)SceneState_TriggerColumnNineteen, 0xc80);
    Value2(Scheduler_AddOrUpdateCallback, (s32)SceneState_TriggerColumnTen, 0xc80);
    Scheduler_AddOrUpdateCallback((s32)TakaraAshiba_RaiseTriggerOnStand, 0xc80);
    {
        u16 t;
        t = 0x3f42;
        *(volatile u16 *)0x04000050 = t;
        t = 0x607;
        *(volatile u16 *)0x04000052 = t;
    }
}

void FieldScene_RunScene3b4_02002334(void)
{
    s32 record;

    *(u8 *)((s32)Object_GetById(14) + 85) = 0;
    Call2(Scheduler_AddOrUpdateCallback, (s32)ActorPresentation_PlaceActorFourteenOnActorNine, 0xc80);
    Scheduler_AddOrUpdateCallback((s32)SceneActor_PublishMarkerBySlotZeroHeight, 0xc80);
    Engine_MapObjectSetPosition(107, 0, 0);
    if (Engine_GameFlagIsSet(0xed9) != 0) {
        Engine_ActorSetAnimation(14, 2);
    }
    TakaraAshiba_DispatchByActorEightColumn();
    SceneActor_RunWhenActor9AtTile45x43();
    TakaraAshiba_UpdateBlockRects();
    FieldScene_RunSingleStep();
    FieldScene_CallHelper3c70();
#if EDITION_INTERNATIONAL
    Engine_ActorSetSpritePriority(8, 3);
#endif
    *(u8 *)((s32)Object_GetById(11) + 85) = 0;
    *(u8 *)((s32)Object_GetById(12) + 85) = 0;
    ActorPresentation_RepaintCellsAtActorsElevenAndTwelve();
    if (Engine_GameFlagIsSet(0x200) != 0) {
        SceneActor_MarkSlot13AndSetFlag200();
        Engine_ActorSetAnimation(13, 5);
    }
    if (Engine_GameFlagIsSet(0x109) == 0) {
        if (Engine_GameFlagIsSet(0x9ca) != 0) {
            Engine_ActorSetPosition(15, 0x3580000, 0x3380000);
            record = Engine_GetTriggerActor(15);
            *(s32 *)(record + 108) = (s32)SceneActor_FaceTowardActorZero;
        } else if (Engine_GameFlagIsSet(0x9c9) != 0) {
            Engine_ActorSetPosition(15, 0x3780000, 0x2980000);
            record = Engine_GetTriggerActor(15);
            *(u16 *)(*(s32 *)(record + 80) + 30) = 0;
            ObjectDispatch_ApplyValueToChildren(record, 16);
        } else if (Engine_GameFlagIsSet(0x9c8) != 0) {
            Engine_ActorSetPosition(15, 0x2480000, 0x2a80000);
        } else {
            Engine_ActorSetPosition(15, 0x2480000, 0x2980000);
        }
    }
}
