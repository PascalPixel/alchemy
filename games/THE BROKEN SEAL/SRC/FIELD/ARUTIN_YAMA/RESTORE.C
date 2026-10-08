#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "GLOBAL_CELLS.H"
#include "YAMA.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"
#include "CALL.H"
#include "FIELD_SERVICE.H"
#include "IWRAM_CALL.H"
#include "DMA.H"
#include "IO_REG.H"
#include "text/MSG_IDS.H"

#define Engine_ActorLookup ((struct FieldActor * (*)(s32))Engine_ActorLookup)
#define Data_03001ed0 (*(u16 * *)&Data_03001ed0)

void FieldScene_RunOpeningAuxiliarySequence();
void ArutinYama_StartPaletteAnim();
void Engine_MapCopyCellAttributes();
void Engine_ObjectSetTargetAndCallback();
void SceneActor_ClearCollisionFlagAndPlaceMarker();
void SceneState_ForwardByRuntimeSelector();

void Vector_AddPolarOffset(s32 distance, s32 angle, union FieldCoordinate *pos);

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

s32 GetMapCellCollision(s32 layer, s32 x, s32 z);
void ResourceMetadata_Register(struct FieldSprite *sprite, s32 mode);
void ArutinYama_SetRollingStateAndPlayCue();

struct Byte {
    u8 v;
};

struct Half {
    u16 v;
};

s32 ArutinYama_RunAreaScene(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (gGameState.scene == (s32)&SceneId_ArutinYama1) {
        ArutinYama_ApplyEntryState();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama3) {
        ArutinYama_RestoreAreaThree();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama4) {
        ArutinYama_RestoreAreaFour();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama5) {
        ArutinYama_RestoreAreaFive();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama6) {
        ArutinYama_RestoreAreaSix();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama7) {
        ArutinYama_RestoreAreaSeven();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama9) {
        ArutinYama_PlaceFlaggedActors();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama10) {
        ArutinYama_RestoreAreaTen();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama11) {
        ArutinYama_RestoreAreaEleven();
    }
    return 0;
}

void ArutinYama_ApplyEntryState(void)
{
    s32 opened;
    s32 flag;
    struct FieldActor *actor;
    u8 *map;

    if (Engine_GameFlagIsSet(0x109) == 0) {
        if (gGameState.entrance == 99) {
            FieldScene_RunOpeningAuxiliarySequence();
        }
    }
    ArutinYama_StartPaletteAnim();
    if (Engine_GameFlagIsSet(0x8fd) == 0) {
        Engine_ActorSetPosition(8, 0, 0);
    } else {
        actor = Object_GetById(8);
        if (actor != 0) {
            actor->priority_flags = 2;
            Engine_ActorSetSpriteFlags(actor, 0);
        }
    }
    actor = Object_GetById(9);
    if (actor != 0) {
        actor->priority_flags = 2;
        Engine_ActorSetSpriteFlags(actor, 0);
    }
    opened = Engine_GameFlagIsSet(0x8fd);
    if (opened == 0) {
        Engine_ActorSetChildValue(10, 2);
        flag = Engine_GameFlagIsSet(0x905);
        if (flag != 0) {
            Engine_ActorSetAnimation(9, 0);
            actor = Object_GetById(9);
            actor->update = SceneState_ForwardByRuntimeSelector;
            Object_GetById(9)->motion_flags = opened;
            actor = Object_GetById(9);
            actor->y.fixed = 0x200000;
            Map_CopyCellAttributes(2, 0, 1, 1, 18, 13);
            Engine_ActorSetPosition(10, 0x780000, 0xd70000);
            actor = Object_GetById(10);
            actor->facing = opened;
            Engine_ActorSetAnimation(10, 3);
            ArutinYama_StartLeafGust(0x820000, 0, 0xa80000, 0);
            return;
        }
        if (Engine_GameFlagIsSet(0x904) == 0) {
            return;
        }
        Engine_ActorSetAnimation(9, 0);
        actor = Object_GetById(9);
        actor->update = SceneState_ForwardByRuntimeSelector;
        Object_GetById(9)->motion_flags = flag;
        actor = Object_GetById(9);
        actor->y.fixed = 0x200000;
        Map_CopyCellAttributes(2, 0, 1, 1, 18, 13);
        Actor_SetPosition(10, 0x1040000, 0xd70000);
        {
            u8 *script = ArutinYama_ActorScript;

            Call3(Engine_ObjectSetTargetAndCallback, 10, 0x10000, (s32)script);
        }
    } else {
        map = gMapWork[0];
        Engine_ActorSetPosition(10, 0, 0);
        Map_CopyCellAttributes(0, 0, 1, 2, 3, 14);
        *(u16 *)(map + 20) &= 0xfdff;
        SceneActor_ClearCollisionFlagAndPlaceMarker(8);
        if (Engine_GameFlagIsSet(0x200) != 0) {
            Engine_ActorSetAnimation(8, 5);
            Map_CopyCellAttributes(7, 13, 1, 1, 9, 13);
            actor = Object_GetById(8);
            actor->y.fixed = 0;
            actor->priority_flags |= 2;
        }
        SceneActor_ClearCollisionFlagAndPlaceMarker(9);
        if (Engine_GameFlagIsSet(0x201) != 0) {
            Engine_ActorSetAnimation(9, 5);
            Map_CopyCellAttributes(29, 1, 3, 1, 17, 13);
            actor = Object_GetById(9);
            actor->y.fixed = 0x200000;
            actor->priority_flags |= 2;
        }
    }
}

void ArutinYama_RestoreAreaThree(void)
{

    if (GameFlag_IsSet(0x8fe) != 0) {
        *(u16 *)(((u8 *)gMapWork[0]) + 20) &= ~0x200;
        Actor_SetPosition(9, 0, 0);
    } else {
        ArutinYama_StartPaletteAnim();
        if (GameFlag_IsSet(0x109) == 0 && gGameState.entrance == 99) {
            FieldScene_RunScene3a4SequenceF();
        } else {
            Map_CopyCellAttributes(38, 24, 1, 2, 37, 24);
            Map_CopyCellAttributes(44, 23, 1, 2, 45, 23);
            if (GameFlag_IsSet(0x8fe) == 0) {
                Actor_SetChildValue(9, 2);
                Engine_ActorSetAnimation(9, 3);
                ArutinYama_StartLeafGust(0xee0000, 0, 0x1a20000, 0x8000);
            }
        }
    }
    if (GameFlag_IsSet(0x323) != 0) {
        Map_CopyCellAttributes(0, 0, 1, 1, 24, 80);
        Map_CopyCellsTo(0, 1, 24, 11, 1, 2);
    } else {
        Map_CopyCellAttributes(2, 0, 1, 1, 24, 80);
        Map_CopyCellsTo(2, 1, 24, 11, 1, 2);
    }
}

void ArutinYama_RestoreAreaFour(void)
{
    extern u8 ArutinYama_RiseTimer[];


    if (GameFlag_IsSet(0x8fe) != 0) {
        *(u16 *)(((u8 *)gMapWork[0]) + 20) &= ~0x200;
    } else {
        Map_CopyCellAttributes(52, 42, 1, 1, 53, 42);
    }
    {
        s32 index = 225;
        if ((u32)((((u16 *)&gGameState)[index] - 6) << 16) <= 0x10000) {
            GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
        }
    }
}

void ArutinYama_RestoreAreaFive(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    struct FieldActor *record;

    if (GameFlag_IsSet(0x907) != 0) {
        *(u16 *)(((u8 *)gMapWork[0]) + 20) &= ~0x200;
        Actor_SetPosition(10, 0, 0);
    } else {
        if (GameFlag_IsSet(0x109) == 0) {
            if (gGameState.entrance == 99) {
                FieldScene_RunScene3a4SequenceE();
            }
        }
        ArutinYama_StartPaletteAnim();
        if (GameFlag_IsSet(0x907) == 0) {
            Actor_SetChildValue(10, 2);
            Engine_ActorSetAnimation(10, 3);
            ArutinYama_StartLeafGust(0x2ec0000, 0x80000, 0x1180000, 0x8000);
        }
    }
    SceneActor_ClearCollisionFlagAndPlaceMarker(9);
    if (GameFlag_IsSet(0x200) != 0) {
        Engine_ActorSetAnimation(9, 5);
        Map_CopyCellAttributes(23, 13, 1, 1, 25, 13);
        {
            struct FieldActor *record = Actor_Get(9);
            record->priority_flags = ACTOR_PRIORITY_UNDERFOOT | record->priority_flags;
        }
    }
    if (GameFlag_IsSet(0x325) != 0) {
        Map_CopyCellAttributes(10, 72, 1, 1, 11, 73);
        Map_CopyCellsTo(49, 32, 11, 4, 1, 2);
    } else {
        Map_CopyCellAttributes(12, 72, 1, 1, 11, 73);
        Map_CopyCellsTo(48, 32, 11, 4, 1, 2);
    }
}

void ArutinYama_RestoreAreaSix(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    struct FieldActor *rec7;
    s32 record;

    if (gGameState.entrance == 2) {
        if (GameFlag_IsSet(0x109) == 0) {
            Actor_SetPosition(8, 0x1660000, 0x680000);
        }
    }
    SceneActor_ClearCollisionFlagAndPlaceMarker(9);
    if (GameFlag_IsSet(0x200) != 0) {
        rec7 = Actor_Get(9);
        Engine_ActorSetAnimation(9, 5);
        Map_CopyCellAttributes(45, 41, 1, 1, 43, 41);
        {
            rec7->priority_flags = ACTOR_PRIORITY_UNDERFOOT | rec7->priority_flags;
        }
    }
    if (GameFlag_IsSet(0x907) != 0) {
        *(u16 *)(((u8 *)gMapWork[0]) + 20) &= ~0x200;
    }
    if (GameFlag_IsSet(0x326) != 0) {
        Map_CopyCellAttributes(17, 93, 1, 1, 16, 92);
        Map_CopyCellsTo(46, 29, 16, 28, 1, 2);
    } else {
        Map_CopyCellAttributes(15, 93, 1, 1, 16, 92);
        Map_CopyCellsTo(47, 29, 16, 28, 1, 2);
    }
}

void ArutinYama_RestoreAreaSeven(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    struct FieldActor *record;

    record = Actor_Get(9);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    SceneActor_UpdateSlot10ByTileX();
    SceneActor_ClearCollisionFlagAndPlaceMarker(9);
    if (GameFlag_IsSet(0x200) != 0) {
        Engine_ActorSetAnimation(9, 5);
        Map_CopyCellAttributes(0, 0, 1, 1, 26, 26);
        {
            struct FieldActor *record = Actor_Get(9);
            record->priority_flags = ACTOR_PRIORITY_UNDERFOOT | record->priority_flags;
        }
    }
    SceneActor_ClearCollisionFlagAndPlaceMarker(11);
    if (GameFlag_IsSet(0x201) != 0) {
        Engine_ActorSetAnimation(11, 5);
        Map_CopyCellAttributes(1, 0, 1, 1, 17, 10);
        {
            struct FieldActor *record = Actor_Get(11);
            record->priority_flags = ACTOR_PRIORITY_UNDERFOOT | record->priority_flags;
        }
    }
    SceneActor_ClearCollisionFlagAndPlaceMarker(12);
    if (GameFlag_IsSet(0x204) != 0) {
        Engine_ActorSetAnimation(12, 5);
        Map_CopyCellAttributes(1, 0, 1, 1, 26, 15);
        {
            struct FieldActor *record = Actor_Get(12);
            record->priority_flags = ACTOR_PRIORITY_UNDERFOOT | record->priority_flags;
        }
    }
    Engine_TaskAddCallback(SceneActor_SetActor12ModeByActorZeroHeight, 0xc80);
    if (GameFlag_IsSet(0x327) != 0) {
        Map_CopyCellAttributes(30, 82, 1, 1, 29, 81);
        Map_CopyCellsTo(46, 28, 29, 17, 1, 2);
    } else {
        Map_CopyCellAttributes(28, 82, 1, 1, 29, 81);
        Map_CopyCellsTo(47, 28, 29, 17, 1, 2);
    }
}

void ArutinYama_PlaceFlaggedActors(void)
{
    struct FieldActor *actor;

    actor = Object_GetById(10);
    Engine_ActorSetPosition(8, 0, 0);
    Engine_ActorSetPosition(9, 0, 0);
    Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
    actor->motion_flags = 0;
    actor->scale_x = 0xe666;
    actor->scale_y = 0x9999;
    actor->sprite->rotation = 0x8000;
    Object_GetById(12)->motion_flags = 0;
    Object_GetById(12)->y.fixed = -0x1c0000;
    if (GameFlag_IsSet(0x908)) {
        actor->x.fixed += 0xe0000;
        actor->y.fixed += -0x80000;
        actor->sprite->rotation = 0xc000;
    }
    if (Engine_GameFlagIsSet(0x908)) {
        Map_CopyCellsTo(25, 36, 43, 36, 11, 9);
        Map_CopyCellAttributes(25, 35, 10, 5, 43, 35);
        Engine_MapRedraw();
        Engine_TaskWait(1);
    }
    if (gGameState.entrance == 6 && !Engine_GameFlagIsSet(0x109)) {
        Engine_EventBegin();
        Object_GetById(0)->y.fixed = -0x580000;
        Engine_CameraMoveTo(0x3180000, -0x580000, 0x2410000, 0);
        Engine_MapRedraw();
        Engine_TaskWait(1);
        Engine_EventEnd();
    }
}

void ArutinYama_RestoreAreaTen(void)
{
    extern u8 ArutinYama_RiseTimer[];

    s32 rec7;
    struct FieldActor *record;
    s16 flag;

    rec7 = Engine_GameFlagIsSet(0x909);
    if (rec7 != 0) {
        Engine_ActorSetPosition(8, 0, 0);
        Engine_ActorSetPosition(9, 0, 0);
    } else {
        record = Object_GetById(8);
        Engine_ActorSetSpriteFlags(record, 0);
        Engine_ActorSetSpritePriority(9, 3);
        record = Object_GetById(9);
        Engine_ActorSetSpriteFlags(record, 0);
        Object_GetById(9)->collision_flags = rec7;
    }
    flag = gGameState.entrance;
    if (flag == 1 || flag == 98) {
        if (Engine_GameFlagIsSet(0x109) == 0) {
            rec7 = Object_GetById(ACTOR_PARTY_LEADER);
            Engine_EventBegin();
            *(s32 *)(rec7 + 12) = 0x100000;
            Engine_EventEnd();
        }
    } else if (flag == 99) {
        if (Engine_GameFlagIsSet(0x109) == 0) {
            RunEventScript01();
        }
    }

}

void ArutinYama_RestoreAreaEleven(void)
{
    extern u8 ArutinYama_RiseTimer[];

    struct FieldActor *record;

    record = Actor_Get(9);
    Engine_ActorSetSpriteFlags(record, 0);
    if (gGameState.entrance == 2) {
        Actor_SetPosition(9, 0xb80000, 0x1480000);
    }
}

void ArutinYama_InitLeaf(struct FieldActor *leaf)
{
    leaf->motion_flags = 0;
    leaf->sprite->priority = 1;
    ObjectGroup_SetChildValue(leaf, 3);
    Engine_ActorSetSpriteFlags(leaf, 0);
    leaf->scale_x = 0x4ccc;
    leaf->scale_y = 0x4ccc;
}

void ArutinYama_UpdateDriftingLeaf(union FieldObject *object)
{
    struct FieldActor *leaf = &object->actor;

    switch (ArutinYama_LeafMode) {
    case 0:
        leaf->x.fixed += ((s16)leaf->timer << 12) + ((s32)((((u32)(Engine_RandomNext() << 1) >> 16) - 1) << 16) >> 1);
        break;
    case 0x8000:
        leaf->x.fixed -= ((s16)leaf->timer << 12) + ((s32)((((u32)(Engine_RandomNext() << 1) >> 16) - 1) << 16) >> 1);
        break;
    }
    if ((s16)leaf->timer <= 3) {
        switch (ArutinYama_LeafMode) {
        case 0:
            leaf->x.fixed += 0x8000;
            break;
        case 0x8000:
            leaf->x.fixed -= 0x8000;
            break;
        }
        leaf->scale_x += 0x1999;
        leaf->scale_y -= 0xccc;
    } else {
        leaf->z.fixed += 0x13333;
        leaf->scale_x += 0x7ae;
        leaf->scale_y += 0x7ae;
    }
    if ((u32)((s16)leaf->timer * Engine_RandomNext()) >> 16 == 0)
        ObjectGroup_SetChildValue(leaf, 7);
    if ((s16)leaf->timer != 0)
        *(s16 *)&leaf->timer -= 2;
    else
        leaf->timer = ((u32)(Engine_RandomNext() * 5) >> 16) * 2 + 2;
    if (--object->effect.countdown == 0)
        Engine_ObjectDispatchRelease(leaf);
}

void ArutinYama_SpawnLeaf(void)
{
    union FieldObject *leaf;

    if ((gFrameCount & 3) != 0)
        return;
    leaf = (union FieldObject *)Engine_ObjectCreate(222, ArutinYama_LeafOrigin[0], ArutinYama_LeafOrigin[1], ArutinYama_LeafOrigin[2]);
    if (leaf == NULL)
        return;
    leaf->actor.timer = 30;
    leaf->actor.work = 1;
    leaf->effect.countdown = 20;
    ArutinYama_InitLeaf(&leaf->actor);
    leaf->effect.update = ArutinYama_UpdateDriftingLeaf;
    Object_SetMode(&leaf->actor, 1);
}

void ArutinYama_StartLeafGust(s32 x, s32 y, s32 z, s32 facing)
{
    ArutinYama_LeafOrigin[0] = x;
    ArutinYama_LeafOrigin[1] = y;
    ArutinYama_LeafOrigin[2] = z;
    ArutinYama_LeafMode = facing;

    BattleFx_SetQueuedSoundAndPlay(170);
    Engine_TaskAddCallback(ArutinYama_SpawnLeaf, 0xc80);
}

void FieldScene_RunScene3a4SequenceG(void)
{
    struct FieldActor *spark;
    s32 scale;

    if (gFrameCount % 3 == 0) {
        spark = (struct FieldActor *)Object_Create(200, (((u32)(Engine_RandomNext() * 48) >> 16) << 16) + 0x2fd0000, -0x400000, 0x2600000);
        if (spark != NULL) {
            if (gFrameCount % 9 == 0) {
                if ((u32)(Engine_RandomNext() << 1) >> 16 != 0)
                    Engine_AudioPlayCue(145);
                else
                    Engine_AudioPlayCue(144);
            }
            spark->motion_flags = 0;
            scale = ((u32)(Engine_RandomNext() << 15) >> 16) + 0x4ccc;
            spark->gravity = 0x6666;
            spark->scale_y = scale;
            spark->scale_x = scale;
            spark->unknown_5d[4] = 1;
            Engine_ActorSetSpriteFlags(spark, 0);
            spark->priority_flags &= ~ACTOR_PRIORITY_AUTOMATIC;
            spark->sprite->priority = 1;
            Object_SetMode(spark, 1);
            Engine_ObjectSetScript(spark, (const s32 *)ArutinYama_SparkScript);
            spark->velocity_x = (((u32)(Engine_RandomNext() * 6) >> 16) - 3) << 16;
            spark->velocity_y = 0x80000;
            spark->velocity_z = ((u32)(Engine_RandomNext() * 0x600) >> 16) - 0x300;
        }
    }
}

void SceneState_ForwardByRuntimeSelector(s32 arg)
{

    s32 sel = gFrameCount & 7;

    if (sel == 0) {
        ObjectGroup_SetChildValue(arg, 2);
    } else if (sel == 2) {
        ObjectGroup_SetChildValue(arg, 0);
    }
}

void FieldScene_RunActorTenFourStepSequence(void)
{
    Engine_PsynergyBegin(24, 1);
    Engine_PsynergySetTarget(10, 9);
    Engine_PsynergyRaiseHands();
    Engine_ActorSetChildValue(10, 2);
    Engine_PsynergyPlayEffect(1);
    Engine_ActorSetChildValue(10, 2);
    Engine_PsynergyLowerHands();
    Engine_ActorSetChildValue(10, 2);
    BattleEffect_CleanupSceneObjects();
    Engine_AudioPlayCue(288);
    Engine_ActorSetChildValue(10, 2);
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    struct FieldActor *rec7;
    s32 record;

    rec7 = Object_GetById(ACTOR_PARTY_LEADER);
    Engine_EventBegin();
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(8, 0, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Engine_ItemShowFound(ITEM_FROST_JEWEL, 3);
    Engine_PartyGiveItem(ITEM_FROST_JEWEL, 0);
    ((struct FieldActor *)Battle_GetWorkObject1e0())->motion_flags = 0;
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x640000, 0, 0xf90000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    FieldScene_RunEarlySequence();
    Camera_MoveTo(rec7->x.fixed, rec7->y.fixed, rec7->z.fixed, 1);
    Engine_CameraWaitForMove();
    Event_LoadAreaScript(ArutinYama_OpenedAreaScript);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    Engine_EventEnd();
}

void FieldScene_RunScene3a4SequenceF(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_ActorSetPosition(9, 0, 0);
    Engine_ActorSetPosition(8, 0x1480000, 0x1a80000);
    Engine_GameFlagSet(0x323);
    FieldScene_RunScene3a4SequenceB();
    Engine_MapRedraw();
    Engine_TaskWait(1);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    FieldScene_RunScene3a4SequenceH();
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    Engine_EventEnd();
}

void FieldScene_RunScene3a4SequenceE(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_ActorSetPosition(10, 0, 0);
    Engine_ActorSetPosition(8, 0x1e80000, 0x8a0000);
    Engine_GameFlagSet(0x325);
    FieldScene_RunScene3a4SequenceA();
    Engine_MapRedraw();
    Engine_TaskWait(1);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    FieldScene_RunScene3a4SequenceI();
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    Engine_EventEnd();
}

void SceneAudio_PlayCue183EverySixtyTicks(void)
{
    ArutinYama_CueTicks = ArutinYama_CueTicks + 1;
    if (ArutinYama_CueTicks == 60) {
        Engine_AudioPlayCue(183);
        ArutinYama_CueTicks = 0;
    }
}

void FieldScene_RunLateAuxiliarySequence(void)
{
    Engine_EventBegin();
    Camera_SetSpeed(0x9999, 0x1333);
    Camera_MoveTo(0x1480000, -1, 0x570000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 328, 116);
    Audio_PlayCue(148);
    Engine_TaskAddCallback(SceneAudio_PlayCue183EverySixtyTicks, 3200);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Actor_SetSpeed(8, 0x1999, 0xccc);
    Actor_SetSpeed(9, 0x1999, 0xccc);
    Engine_ActorSetAnimation(8, 2);
    Actor_SetDestination(8, 328, 104);
    Actor_SetDestination(9, 328, 108);
    Engine_EventWait(60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 256, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorWaitForMove(8);
    gGameState.battle_start = 3;
    Party_SetFields1ceAnd1d0((s32)&SceneId_ArutinYama10, 99);
    BattleFx_SetWeightedResult(53, 3);
}

void SceneActor_SetActor8ModeByCounterBit(void)
{

    if (((gFrameCount >> 1) & 1) != 0) {
        Engine_ActorSetChildValue(8, 7);
    } else {
        Engine_ActorSetChildValue(8, 6);
    }
}

void RunEventScript01(void)
{
    u32 i;
    struct FieldActor *record;
    struct EventWork *work;
    s32 update;
    s32 celebrate;
    s32 party;

    Engine_EventBegin();
    Actor_SetPosition(8, 0x1480000, 0x580000);
    Actor_SetPosition(9, 0x1480000, 0x580000);
    Engine_ActorSetAnimation(8, 0);
    work = gEventWork;
    work->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    work->transition_frames = 40;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(GERALD, record->x.fixed, record->z.fixed);
    }
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(IWAN, record->x.fixed, record->z.fixed);
    }
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(MEARI, record->x.fixed, record->z.fixed);
    }
    Actor_SetSpeed(GERALD, 0x9999, 0x4ccc);
    Actor_SetSpeed(IWAN, 0x9999, 0x4ccc);
    Actor_SetSpeed(MEARI, 0x9999, 0x4ccc);
    Engine_ActorEnableActionCallback(1, ArutinYama_GeraldScript);
    Engine_ActorEnableActionCallback(2, ArutinYama_IvanScript);
    Object_SetActionCallbackAndRefreshById(3, ArutinYama_MiaScript);
    Actor_FaceDirection(GERALD, 0xc000, 0);
    Actor_FaceDirection(IWAN, 0xc000, 0);
    Actor_FaceDirection(MEARI, 0xc000, 40);
    Engine_ActorRunRepeatedMotion(GERALD, 1);
    Actor_FaceDirection(GERALD, 0xe000, 10);
    Engine_EventSetMessage((s32)MsgArutinWeDidRobinWeBeat);
    Event_ShowMessageAndWait(GERALD, 0, 10);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    Actor_FaceDirection(IWAN, 0xa000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Event_OpenMessage(IWAN, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorSetAnimationAndWait(IWAN, 3);
    } else {
        Engine_ActorSetAnimationAndWait(IWAN, 4);
        Event_SkipMessages(1);
    }
    Event_ShowMessageAndWait(IWAN, 0, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(MEARI, 2);
    Engine_EventSetMessage((s32)MsgArutinGuardianStatuesWereCreatedLong);
    Event_ShowMessage(MEARI, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
    Actor_ShowEmote(GERALD, 0x102, 60);
    Actor_FaceDirection(GERALD, 0x2000, 40);
    Event_ShowMessage(GERALD, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(GERALD, 0x101, 0);
    Actor_ShowEmote(IWAN, 0x101, 0);
    Actor_ShowEmote(MEARI, 0x101, 40);
    Audio_PlayCue(190);
    Actor_SetChildValue(8, 7);
    Engine_EventWait(10);
    Audio_PlayCue(0x121);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(GERALD, 0xc000, 0);
    Actor_FaceDirection(IWAN, 0xc000, 0);
    Actor_FaceDirection(MEARI, 0xc000, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(GERALD, 0x100, 0);
    Actor_ShowEmote(IWAN, 0x100, 0);
    Actor_ShowEmote(MEARI, 0x100, 40);
    Audio_PlayCue(103);
    update = (s32)SceneActor_SetActor8ModeByCounterBit;
    Task_AddCallback((void (*)(void))update, 0xc80);
    celebrate = (s32)ArutinYama_CelebrateScript;
    Engine_ActorEnableActionCallback(9, celebrate);
    Object_SetActionCallbackAndRefreshById(8, celebrate);
    Engine_TaskRemoveCallback(update);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Engine_EventWait(20);
    Event_ShowMessage(IWAN, 0);
    Actor_FaceDirection(GERALD, 0xe000, 0);
    Actor_FaceDirection(IWAN, 0xa000, 20);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(IWAN, 3);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(MEARI, 1);
    Event_ShowMessageAndWait(MEARI, 0, 20);
    Actor_FaceDirection(GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 40);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Event_ShowMessageAndWait(GERALD, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(IWAN, 3);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(20);
    party = (s32)ArutinYama_PartyScript;
    Engine_ActorEnableActionCallback(GERALD, party);
    Engine_ActorEnableActionCallback(2, party);
    Object_SetActionCallbackAndRefreshById(3, party);
    Engine_EventWait(20);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    gEventWork->transition_frames = 16;
    GameFlag_Set(0x909);
    Engine_EventEnd();
}

void ArutinYama_RequestPsynergy(void)
{
    gEventWork->psynergy_request = 0x1018;
}

void SceneActor_SetActor12ModeByActorZeroHeight(void)
{

    if (Object_GetById(0)->y.fixed > 0x00300000) {
        {
            u8 *flag = (u8 *)Object_GetById(11) + 35;
            s32 bit = 2;

            bit |= *flag;
            *flag = bit;
        }
        Engine_ActorSetSpritePriority(12, 3);
    } else {
        Engine_ActorSetSpritePriority(12, 2);
    }
}

void SceneActor_ClearCollisionFlagAndPlaceMarker(s32 no)
{
    struct FieldActor *record;

    record = Object_GetById(no);
    record->collision_flags &= 0xfe;

    SetMapCellCollision(0, record->x.fixed, record->z.fixed, 255);
}

void ArutinYama_SettleAndMountLeader(struct FieldActor *object)
{
    s32 x;
    s32 z;
    s32 center_z;
    struct FieldSprite *sprite;
    struct FieldActor *leader;

    Engine_AudioPlayCue(288);
    Engine_AudioPlayCue(232);
    x = object->x.fixed & 0xfff00000;
    center_z = 0x80000;
    z = object->z.fixed & 0xfff00000;
    object->acceleration = 0x20000;
    x += center_z;
    center_z += z;
    Engine_ObjectSetPosition(object, x, object->y.fixed, center_z);
    Engine_ObjectCommitPosition(object);
    object->terrain_id = 0;
    object->x.fixed = x;
    object->z.fixed = center_z;
    object->velocity_x = 0;
    object->velocity_z = 0;
    Object_SetMode(object, 2);
    Engine_TaskWait(15);
    Object_SetMode(object, 1);
    Engine_TaskWait(30);
    sprite = object->sprite;
    sprite->part_count = 1;
    ResourceMetadata_ClearRecord(*(s32 *)((u8 *)sprite + 44));
    *(s32 *)((u8 *)sprite + 44) = 0;
    ((u8 *)sprite)[37] = 1;
    leader = Engine_ActorLookup(gGameState.selected_actor);
    Engine_AudioPlayCue(152);
    leader->x.fixed = x;
    leader->velocity_y = 0x60000;
    leader->gravity = 0x10000;
    leader->z.fixed = center_z;
    leader->sprite->priority = 0;
    Object_SetMode(leader, 7);
    Engine_ObjectSetPosition(leader, x, leader->y.fixed, z + 0x180000);
    leader->facing = 0x4000;
    Engine_TaskWait(20);
    leader->sprite->priority = 2;
    Engine_AudioPlayCue(159);
}

void ArutinYama_ReservedNoOp(void)
{
}

void ArutinYama_TurnRollingObjectA(struct FieldActor *object)
{
    union FieldCoordinate *p;
    union FieldCoordinate pos[3];
    s32 angle;
    s32 x;
    s32 z;
    s32 n;

    angle = (object->facing + 0x4000) & 0xc000;
    p = pos;
    p[0].fixed = object->x.fixed;
    p[1].fixed = object->y.fixed;
    p[2].fixed = object->z.fixed;
    Vector_AddPolarOffset(0x180000, angle, p);
    x = (p[0].fixed + 0x80000) & 0xfff00000;
    z = (p[2].fixed + 0x80000) & 0xfff00000;
    angle += 0x8000;
    Object_SetMode(object, 5);
    n = 0;
    Engine_AudioPlayCue(184);
    while (n < 16) {
        angle += 0x400;
        p[0].fixed = x;
        p[2].fixed = z;
        Vector_AddPolarOffset(0x180000, angle, p);
        object->x.fixed = p[0].fixed;
        object->z.fixed = p[2].fixed;
        object->facing = angle + 0x4000;
        Engine_TaskWait(1);
        n++;
    }
    Engine_AudioPlayCue(233);
}

void ArutinYama_TurnRollingObjectB(struct FieldActor *object)
{
    union FieldCoordinate *p;
    union FieldCoordinate pos[3];
    s32 angle;
    s32 x;
    s32 z;
    s32 n;

    angle = (object->facing - 0x4000) & 0xc000;
    p = pos;
    p[0].fixed = object->x.fixed;
    p[1].fixed = object->y.fixed;
    p[2].fixed = object->z.fixed;
    Vector_AddPolarOffset(0x180000, angle, p);
    x = (p[0].fixed + 0x80000) & 0xfff00000;
    z = (p[2].fixed + 0x80000) & 0xfff00000;
    angle += 0x8000;
    Object_SetMode(object, 6);
    n = 0;
    Engine_AudioPlayCue(184);
    while (n < 16) {
        angle -= 0x400;
        p[0].fixed = x;
        p[2].fixed = z;
        Vector_AddPolarOffset(0x180000, angle, p);
        object->x.fixed = p[0].fixed;
        object->z.fixed = p[2].fixed;
        object->facing = angle - 0x4000;
        Engine_TaskWait(1);
        n++;
    }
    Engine_AudioPlayCue(233);
}

void ArutinYama_SetRollingStateAndPlayCue(u16 *record)
{
    record = (u16 *)((char *)record + 102);
    {
        s32 value = 0x21;

        *record = value;
    }
    Engine_AudioPlayCue(288);
}

void ArutinYama_AdvanceRollingObject(struct FieldActor *object)
{
    s32 angle = 0xc000;
    s32 radius;
    s16 *timer;
    union FieldCoordinate pos[3];
    union FieldCoordinate *p;
    s32 ahead;
    s32 behind;

    angle &= object->facing;
    radius = object->y.fixed / 0x10000;
    radius = ArutinYama_RollRadii[(s16)object->timer - radius + 16];
    timer = (s16 *)&object->work;
    if (*timer != 0) {
        if ((s16)--*timer == 20) {
            Engine_AudioPlayCue(184);
        }
        if (*timer == 0) {
            Engine_AudioPlayCue(233);
        }
    }
    p = pos;
    p[0].fixed = object->x.fixed;
    p[1].fixed = object->y.fixed;
    p[2].fixed = object->z.fixed;
    Vector_AddPolarOffset(Iwram_MulQ16(radius, 0xc000), angle, p);
    object->x.fixed = p[0].fixed;
    object->z.fixed = p[2].fixed;
    ahead = Map_GetTerrainHeight(2, p[0].fixed, p[2].fixed);
    Vector_AddPolarOffset(-Iwram_MulQ16(radius, 0x18000), angle, p);
    behind = Map_GetTerrainHeight(2, p[0].fixed, p[2].fixed);
    if (*timer <= 20) {
        if (ahead == behind) {
            Object_SetMode(object, 2);
        } else if (ahead > behind) {
            Object_SetMode(object, 3);
        } else {
            Object_SetMode(object, 4);
        }
    }
}

void ArutinYama_RunRollingObject(s32 id, s32 heading)
{
    struct FieldActor *object = Object_GetById(id);
    struct FieldActor *leader = Engine_ActorLookup(gGameState.selected_actor);
    union FieldCoordinate pos[3];
    union FieldCoordinate *p;
    struct Byte zero;
    s32 n;

    Engine_EventBegin();
    if (heading == -1) {
        heading = object->facing;
    }
    for (n = 0; n <= 3; n++) {
        pos[0].fixed = object->x.fixed;
        pos[1].fixed = object->y.fixed;
        pos[2].fixed = object->z.fixed;
        Vector_AddPolarOffset(0x100000, heading, pos);
        if (Map_GetTerrainHeight(2, pos[0].fixed, pos[2].fixed) == object->y.fixed) {
            break;
        }
        heading += 0x4000;
    }
    if (n == 4) {
        return;
    }
    object->terrain_id = 2;
    leader->x.fixed = 0;
    leader->z.fixed = 0;
    ResourceMetadata_Register(object->sprite, 16);
    Engine_CameraFollowActor(id, 1);
    Engine_CameraWaitForMove();
    Camera_SetSpeed(0x100000, 0x20000);
    object->facing = heading;
    object->speed = 0x20000;
    object->acceleration = 0xccc;
    zero.v = 0;
    object->paused = zero.v;
    object->timer = object->y.fixed / 0x10000;
    object->work = 0;
    pos[0].fixed = object->x.fixed;
    pos[1].fixed = object->y.fixed;
    pos[2].fixed = object->z.fixed;
    Vector_AddPolarOffset(0x180000, heading, pos);
    Engine_ObjectSetPosition(object, pos[0].fixed, object->y.fixed, pos[2].fixed);
    Engine_ObjectCommitPosition(object);
    Engine_AudioPlayCue(233);
    for (;;) {
        switch (GetMapCellCollision(2, object->x.fixed, object->z.fixed)) {
        case 98:
            ArutinYama_TurnRollingObjectA(object);
            break;
        case 97:
            ArutinYama_TurnRollingObjectB(object);
            break;
        case 96:
            ArutinYama_SetRollingStateAndPlayCue(object);
            break;
        case 99:
            goto arrived;
        }
        ArutinYama_AdvanceRollingObject(object);
        Engine_TaskWait(1);
    }
arrived:
    ArutinYama_SettleAndMountLeader(object);
    Engine_EventEnd();
}

void ArutinYama_StepPaletteAnim(void)
{
    u16 *palette = Data_03001ed0;

    if (ArutinYama_PaletteHold <= 0) {
        s32 frame;
        struct Half zero;

    next:
        frame = ArutinYama_PaletteScript[ArutinYama_PaletteStep++];
        if (frame == -1) {
            zero.v = 0;
            ArutinYama_PaletteStep = zero.v;
            goto next;
        }
        ArutinYama_PaletteHold = ArutinYama_PaletteScript[ArutinYama_PaletteStep++];
        palette += frame;
        Dma_Set(palette, (void *)0x05000006, DMA_ENABLE32 | 9, REG_DMA3);
    }
    ArutinYama_PaletteHold--;
}

void ArutinYama_StartPaletteAnim(void)
{
    struct Half zero;

    zero.v = 0;
    ArutinYama_PaletteStep = zero.v;
    ArutinYama_PaletteHold = zero.v;
    Engine_TaskAddCallback((s32)&ArutinYama_StepPaletteAnim, 0xc80);
}
