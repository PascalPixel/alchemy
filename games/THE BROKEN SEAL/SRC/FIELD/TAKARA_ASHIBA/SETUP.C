#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "EDITION.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIELD_EVENT.H"
#include "FIELD_SERVICE.H"
#include "FIELD_SCENE.H"

#include "STAGED_ACTOR.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"
#include "CALL.H"
#include "IO_REG.H"
s32 SceneActor_FaceTowardActorZero();

enum {
    ITEM_RED_KEY = 243,
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

void TakaraAshiba_OpenPassage();
void Korosseo_ShowItemIcon();
void TakaraAshiba_DispatchByActorEightColumn();
void TakaraAshiba_UpdateBlockRects();
void ObjectDispatch_ApplyValueToChildren();

static __inline__ void SceneState_StoreStep(s16 *field, s32 step)
{
    *field = step;
}
void FieldScene_RunSingleStep(void);

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
    FieldScheduler_AddOrUpdateCallback((s32)SceneState_TriggerColumnNineteen, 0xc80);
    FieldScheduler_AddOrUpdateCallback((s32)SceneState_TriggerColumnTen, 0xc80);
    Engine_TaskAddCallback((s32)TakaraAshiba_RaiseTriggerOnStand, 0xc80);
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
    FieldScheduler_AddOrUpdateCallback((s32)ActorPresentation_PlaceActorFourteenOnActorNine, 0xc80);
    Engine_TaskAddCallback((s32)SceneActor_PublishMarkerBySlotZeroHeight, 0xc80);
    MapObject_SetPosition(107, 0, 0);
    if (GameFlag_IsSet(0xed9) != 0) {
        Engine_ActorSetAnimation(14, 2);
    }
    TakaraAshiba_DispatchByActorEightColumn();
    SceneActor_RunWhenActor9AtTile45x43();
    TakaraAshiba_UpdateBlockRects();
    FieldScene_RunSingleStep();
    TakaraAshiba_RepaintActorCells();
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
