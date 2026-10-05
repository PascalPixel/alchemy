#include "TYPES.H"
#include "EDITION.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "STAGED_ACTOR.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"
#include "CALL.H"
#include "IO_REG.H"
s32 SceneActor_FaceTowardActorZero();
void SceneActor_PublishMarkerBySlotZeroHeight(void);
void ActorPresentation_PlaceActorFourteenOnActorNine(void);
void TakaraAshiba_RaiseTriggerOnStand(void);
void SceneState_TriggerColumnTen(void);
void SceneState_TriggerColumnNineteen(void);

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

void TakaraAshiba_OpenPlatformPassage(s32 a0);

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

void TakaraAshiba_InitializeScene1(void);
void TakaraAshiba_InitializeScene2(void);
void TakaraAshiba_InitializeScene3(void);

void TakaraAshiba_InitializeScene1(void)
{
    u32 i;
    s32 record;

    Engine_TaskWait(1);
    Korosseo_ShowItemIcon(12, 243);
    Korosseo_ShowItemIcon(11, 244);
    Korosseo_ShowItemIcon(10, 244);
    Korosseo_ShowItemIcon(9, 244);
    Korosseo_ShowItemIcon(8, 244);
    if (GameFlag_IsSet(0xee7) == 0) {
        Actor_SetPosition(8, 0xe80000, 0x3680000);
    }
    if (GameFlag_IsSet(0xee8) == 0) {
        Actor_SetPosition(9, 0x1280000, 0x3380000);
    }
    if (GameFlag_IsSet(0xee9) == 0) {
        Actor_SetPosition(10, 0x1480000, 0x2f80000);
    }
    if (GameFlag_IsSet(0xeea) == 0) {
        Actor_SetPosition(11, 0x1680000, 0x3680000);
    }
    if (GameFlag_IsSet(0x9c0) != 0) {
        TakaraAshiba_OpenPassage(0);
    }
    if (GameFlag_IsSet(0x9c1) != 0) {
        TakaraAshiba_OpenPassage(1);
    }
    if (GameFlag_IsSet(0x9c2) != 0) {
        TakaraAshiba_OpenPassage(2);
    }
    if (GameFlag_IsSet(0x9c3) != 0) {
        TakaraAshiba_OpenPassage(3);
    }
    if (GameFlag_IsSet(0x9c4) != 0) {
        TakaraAshiba_OpenPlatformPassage(0);
    }
}

/* The second platform scene's start: actors 8 to 11 become solid, 8 to 12
 * are drawn at 0.7 of their width, the column and stand triggers are
 * scheduled, and the blend is set. */
void TakaraAshiba_InitializeScene2(void)
{
    Object_GetById(8)->collision_flags = 1;
    Object_GetById(9)->collision_flags = 1;
    Object_GetById(10)->collision_flags = 1;
    Object_GetById(11)->collision_flags = 1;
    Object_GetById(8)->scale_x = 0xb333;
    Object_GetById(9)->scale_x = 0xb333;
    Object_GetById(10)->scale_x = 0xb333;
    Object_GetById(11)->scale_x = 0xb333;
    Object_GetById(12)->scale_x = 0xb333;
    Call2(Scheduler_AddOrUpdateCallback, (s32)SceneState_TriggerColumnNineteen, 0xc80);
    Value2(Scheduler_AddOrUpdateCallback, (s32)SceneState_TriggerColumnTen, 0xc80);
    Scheduler_AddOrUpdateCallback((s32)TakaraAshiba_RaiseTriggerOnStand, 0xc80);
    {
        /* FAKEMATCH: each blend value is loaded before its register address. */
        u16 value;

        value = 0x3f42;
        REG_BLDCNT = value;
        value = 0x607;
        REG_BLDALPHA = value;
    }
}

void TakaraAshiba_InitializeScene3(void)
{
    s32 record;

    Object_GetById(14)->motion_flags = 0;
    Call2(Scheduler_AddOrUpdateCallback, (s32)ActorPresentation_PlaceActorFourteenOnActorNine, 0xc80);
    Scheduler_AddOrUpdateCallback((s32)SceneActor_PublishMarkerBySlotZeroHeight, 0xc80);
    MapObject_SetPosition(107, 0, 0);
    if (GameFlag_IsSet(0xed9) != 0) {
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
    Object_GetById(11)->motion_flags = 0;
    Object_GetById(12)->motion_flags = 0;
    ActorPresentation_RepaintCellsAtActorsElevenAndTwelve();
    if (GameFlag_IsSet(0x200) != 0) {
        SceneActor_MarkSlot13AndSetFlag200();
        Engine_ActorSetAnimation(13, 5);
    }
    if (GameFlag_IsSet(0x109) == 0) {
        if (GameFlag_IsSet(0x9ca) != 0) {
            Actor_SetPosition(15, 0x3580000, 0x3380000);
            record = Object_GetById(15);
            *(s32 *)(record + 108) = (s32)SceneActor_FaceTowardActorZero;
        } else if (GameFlag_IsSet(0x9c9) != 0) {
            Actor_SetPosition(15, 0x3780000, 0x2980000);
            record = Object_GetById(15);
            *(u16 *)(*(s32 *)(record + 80) + 30) = 0;
            ObjectDispatch_ApplyValueToChildren(record, 16);
        } else if (GameFlag_IsSet(0x9c8) != 0) {
            Actor_SetPosition(15, 0x2480000, 0x2a80000);
        } else {
            Actor_SetPosition(15, 0x2480000, 0x2980000);
        }
    }
}
