#include "GLOBAL_CELLS.H"
#include "YAMA.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"
#include "CALL.H"
#include "IWRAM_CALL.H"
#include "DMA.H"
#include "IO_REG.H"

void ArutinYama_ApplyEntryState(void);
void ArutinYama_PlaceFlaggedActors(void);

extern u8 ArutinYama_ActorScript[];
void FieldScene_RunOpeningAuxiliarySequence();
void ArutinYama_StartPaletteAnim();
void Engine_MapCopyCellAttributes();
void Engine_ObjectSetTargetAndCallback();
void SceneActor_ClearCollisionFlagAndPlaceMarker();
void SceneState_ForwardByRuntimeSelector();
extern u8 Data_02000240[];

/* The way the wind blows the drifting leaves: 0 or 0x8000. */
extern s32 ArutinYama_LeafMode;

extern u8 MsgArutinGuardianStatuesWereCreatedLong[];
extern u8 MsgArutinWeDidRobinWeBeat[];

void ResourceMetadata_ClearRecord(s32 handle);
struct FieldActor *ObjectTable_Get(s32 actor);

void Vector_AddPolarOffset(s32 distance, s32 angle, union FieldCoordinate *pos);

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

/* Step lengths by height difference, centred on entry 16. */
extern s32 ArutinYama_RollRadii[];

s32 GetMapCellCollision(s32 layer, s32 x, s32 z);
void ResourceMetadata_Register(struct FieldSprite *sprite, s32 mode);
void SceneState_SetRecordWord102AndPlayCue288();
void ArutinYama_TurnRollingObjectA(struct FieldActor *object);
void ArutinYama_TurnRollingObjectB(struct FieldActor *object);
void ArutinYama_SettleAndMountLeader(struct FieldActor *object);
void ArutinYama_AdvanceRollingObject(struct FieldActor *object);

struct Byte {
    u8 v;
};

struct Half {
    u16 v;
};

/* The field palette buffer. */
extern u16 *Data_03001ed0;

/* Frames left before the next step of the animation. */
extern s16 ArutinYama_PaletteHold;

/* The position in the script below. */
extern s16 ArutinYama_PaletteStep;

/* Pairs of (first colour, frames to hold), ended by -1. */
extern s8 ArutinYama_PaletteScript[];

void ArutinYama_StepPaletteAnim(void);

/* Altin Peak: open the screen with the window transition and run the entry
 * scene of the area the party enters; the second and fourth areas have
 * none. */
s32 ArutinYama_RunAreaScene(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (gGameState.scene == (s32)&SceneId_ArutinYama1) {
        ArutinYama_ApplyEntryState();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama3) {
        FieldScene_RunScene3a4_02002310();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama4) {
        FieldScene_RunScene3a4_02002428();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama5) {
        FieldScene_RunScene3a4_02002490();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama6) {
        FieldScene_RunScene3a4_020025c0();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama7) {
        FieldScene_RunScene3a4_020026c0();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama9) {
        ArutinYama_PlaceFlaggedActors();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama10) {
        FieldScene_RunScene3a4_02002934();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama11) {
        FieldScene_RunScene3a4_020029dc();
    }
    return 0;
}

/* Altin Peak entry: run the opening at entrance 99 before flag 0x109, then
 * place the scene actors and restore the cells each story flag records. */
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
            Call6(Engine_MapCopyCellAttributes, 2, 0, 1, 1, 18, 13);
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
        Call6(Engine_MapCopyCellAttributes, 2, 0, 1, 1, 18, 13);
        Call3(Engine_ActorSetPosition, 10, 0x1040000, 0xd70000);
        {
            u8 *script = ArutinYama_ActorScript;

            Call3(Engine_ObjectSetTargetAndCallback, 10, 0x10000, (s32)script);
        }
    } else {
        map = gMapWork[0];
        Engine_ActorSetPosition(10, 0, 0);
        Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 2, 3, 14);
        *(u16 *)(map + 20) &= 0xfdff;
        SceneActor_ClearCollisionFlagAndPlaceMarker(8);
        if (Engine_GameFlagIsSet(0x200) != 0) {
            Engine_ActorSetAnimation(8, 5);
            Call6(Engine_MapCopyCellAttributes, 7, 13, 1, 1, 9, 13);
            actor = Object_GetById(8);
            actor->y.fixed = 0;
            actor->priority_flags |= 2;
        }
        SceneActor_ClearCollisionFlagAndPlaceMarker(9);
        if (Engine_GameFlagIsSet(0x201) != 0) {
            Engine_ActorSetAnimation(9, 5);
            Call6(Engine_MapCopyCellAttributes, 29, 1, 3, 1, 17, 13);
            actor = Object_GetById(9);
            actor->y.fixed = 0x200000;
            actor->priority_flags |= 2;
        }
    }
}

void FieldScene_RunScene3a4_02002310(void)
{

    if (GameFlag_IsSet(0x8fe) != 0) {
        *(u16 *)(((u8 *)gMapWork[0]) + 20) &= ~0x200;
        Actor_SetPosition(9, 0, 0);
    } else {
        ArutinYama_StartPaletteAnim();
        if (GameFlag_IsSet(0x109) == 0 && (*(struct GameState *)Data_02000240).entrance == 99) {
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

void FieldScene_RunScene3a4_02002428(void)
{
    extern u8 ArutinYama_RiseTimer[];

    extern u8 Data_02000240[];

    if (GameFlag_IsSet(0x8fe) != 0) {
        *(u16 *)(((u8 *)gMapWork[0]) + 20) &= ~0x200;
    } else {
        Map_CopyCellAttributes(52, 42, 1, 1, 53, 42);
    }
    {
        s32 index = 225;
        if ((u32)((((u16 *)Data_02000240)[index] - 6) << 16) <= 0x10000) {
            GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
        }
    }
}

void FieldScene_RunScene3a4_02002490(void)
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

void FieldScene_RunScene3a4_020025c0(void)
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

void FieldScene_RunScene3a4_020026c0(void)
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
        Call6((void (*)())Engine_MapCopyCellAttributes, 30, 82, 1, 1, 29, 81);
        Map_CopyCellsTo(46, 28, 29, 17, 1, 2);
    } else {
        Map_CopyCellAttributes(28, 82, 1, 1, 29, 81);
        Map_CopyCellsTo(47, 28, 29, 17, 1, 2);
    }
}

/* Parks actors 8 and 9, poses actors 10 and 12, and moves them further along when flag 0x908 is set. */
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
    if (Value1(Engine_GameFlagIsSet, 0x908)) {
        actor->x.fixed += 0xe0000;
        actor->y.fixed += -0x80000;
        actor->sprite->rotation = 0xc000;
    }
    if (Engine_GameFlagIsSet(0x908)) {
        Call6(Engine_MapCopyCellsTo, 25, 36, 43, 36, 11, 9);
        Call6(Engine_MapCopyCellAttributes, 25, 35, 10, 5, 43, 35);
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

void FieldScene_RunScene3a4_02002934(void)
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

void FieldScene_RunScene3a4_020029dc(void)
{
    extern u8 ArutinYama_RiseTimer[];

    struct FieldActor *record;

    record = Actor_Get(9);
    Engine_ActorSetSpriteFlags(record, 0);
    if (gGameState.entrance == 2) {
        Actor_SetPosition(9, 0xb80000, 0x1480000);
    }
}

/* A new leaf floats free of the ground at three tenths of full size. */
void ArutinYama_InitLeaf(struct FieldActor *leaf)
{
    leaf->motion_flags = 0;
    leaf->sprite->priority = 1;
    ObjectGroup_SetChildValue(leaf, 3);
    Engine_ActorSetSpriteFlags(leaf, 0);
    leaf->scale_x = 0x4ccc;
    leaf->scale_y = 0x4ccc;
}

/* A drifting leaf: blown along the wind with a random wobble while its
 * timer runs, shrinking and settling once it is low, flickering to palette
 * 7 at random; released when its lifetime ends. */
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

/* Every fourth frame, a leaf leaves the origin the gust started from. */
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

/* Starts a gust of leaves from a point; the wind carries them along x
   for facing 0 and back along it for facing 0x8000. */
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
    volatile u32 *frame = &gFrameCount;

    if (*frame % 3 == 0) {
        spark = (struct FieldActor *)Value4(Engine_ObjectCreate, 200, (((u32)(Engine_RandomNext() * 48) >> 16) << 16) + 0x2fd0000, -0x400000, 0x2600000);
        if (spark != NULL) {
            if (*frame % 9 == 0) {
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
    *(u8 *)(Battle_GetWorkObject1e0() + 85) = 0;
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

/*
 * Overlay resource_3a4. Per-frame tick that counts to sixty, fires one
 * audio cue and rewinds itself. The address of this routine is stored in
 * a record as a plain word, so it runs as a published callback.
 */

/* Count sixty updates before the sound cue. */
void SceneAudio_PlayCue183EverySixtyTicks(void)
{
    ArutinYama_CueTicks = ArutinYama_CueTicks + 1;
    if (ArutinYama_CueTicks == 60) {
        Engine_AudioPlayCue(183);
        ArutinYama_CueTicks = 0;
    }
}

/* The late scene on the peak: the camera pans up, the leader walks to the
 * ledge while cue 183 repeats, actors 8 and 9 follow, and the party is sent
 * on to the tenth area when it next returns. */
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

/* Bit 1 of the runtime status word selects mode 7 or 6 for actor 8. */
void SceneActor_SetActor8ModeByCounterBit(void)
{

    if (((gFrameCount >> 1) & 1) != 0) {
        Engine_ActorSetChildValue(8, 7);
    } else {
        Engine_ActorSetChildValue(8, 6);
    }
}

/* Record returned by Object_GetById/26/3a: a pair of s32 fields at +8 and
 * +16 that get forwarded straight into the matching setup call. */
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
        Actor_SetPosition(ACTOR_GERALD, record->x.fixed, record->z.fixed);
    }
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_IVAN, record->x.fixed, record->z.fixed);
    }
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_MIA, record->x.fixed, record->z.fixed);
    }
    Actor_SetSpeed(ACTOR_GERALD, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_IVAN, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_MIA, 0x9999, 0x4ccc);
    Engine_ActorEnableActionCallback(1, ArutinYama_GeraldScript);
    Engine_ActorEnableActionCallback(2, ArutinYama_IvanScript);
    Object_SetActionCallbackAndRefreshById(3, ArutinYama_MiaScript);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 10);
    Engine_EventSetMessage((s32)MsgArutinWeDidRobinWeBeat);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Event_OpenMessage(ACTOR_IVAN, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    } else {
        Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 4);
        Event_SkipMessages(1);
    }
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 2);
    Engine_EventSetMessage((s32)MsgArutinGuardianStatuesWereCreatedLong);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 60);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 40);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 40);
    Audio_PlayCue(190);
    Actor_SetChildValue(8, 7);
    Engine_EventWait(10);
    Audio_PlayCue(0x121);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 40);
    Audio_PlayCue(103);
    update = (s32)SceneActor_SetActor8ModeByCounterBit;
    Call2(Engine_TaskAddCallback, update, 0xc80);
    celebrate = (s32)ArutinYama_CelebrateScript;
    Engine_ActorEnableActionCallback(9, celebrate);
    Object_SetActionCallbackAndRefreshById(8, celebrate);
    Engine_TaskRemoveCallback(update);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 20);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_IVAN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 1);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 40);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_IVAN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(20);
    party = (s32)ArutinYama_PartyScript;
    Engine_ActorEnableActionCallback(ACTOR_GERALD, party);
    Engine_ActorEnableActionCallback(2, party);
    Object_SetActionCallbackAndRefreshById(3, party);
    Engine_EventWait(20);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    gEventWork->transition_frames = 16;
    GameFlag_Set(0x909);
    Engine_EventEnd();
}

/* Asks the engine for psynergy request 0x1018 on the next frame. */
void ArutinYama_RequestPsynergy(void)
{
    gEventWork->psynergy_request = 0x1018;
}

/* Set actor 12's sprite priority by actor 0's height; above 48.0, also set bit 1 of actor 11's field at +35. */
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

/* Settles the pushed object on the centre of its cell, then lifts the leader
 * onto it: the leader hops up a cell and a half and turns to face along the
 * row, drawn in front of the background while it jumps. */
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
    leader = ObjectTable_Get(gGameState.selected_actor);
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

/* Nothing calls this; it does nothing. */
void ArutinYama_ReservedNoOp(void)
{
}

/* Tile 97: swing the rolling object a quarter turn about the cell corner
 * beside it, sixteen steps of 0x400 from the grid-snapped pivot. */
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

/* Tile 98: the same swing the other way. */
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

/* Set the actor's field at +102, then play cue 288. */
void SceneState_SetRecordWord102AndPlayCue288(u16 *record)
{
    record = (u16 *)((char *)record + 102);
    {
        s32 value = 0x21;

        *record = value;
    }
    Engine_AudioPlayCue(288);
}

/* Rolls the object a step along its heading, sized by how far it sits
 * below its target height; counts down its rolling timer (cues at 20 and
 * 0) and then picks the animation from the slope ahead of it. */
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

/* The rolling-object driver: finds a heading whose step keeps the object
 * level, starts the roll, then dispatches on the tile under it each frame
 * until tile 99. */
void ArutinYama_RunRollingObject(s32 id, s32 heading)
{
    struct FieldActor *object = Object_GetById(id);
    struct FieldActor *leader = ObjectTable_Get(gGameState.selected_actor);
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
    Call2((void (*)())Engine_CameraSetSpeed, 0x100000, 0x20000);
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
            SceneState_SetRecordWord102AndPlayCue288(object);
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

/* Steps a palette animation: when the hold runs out, reads the next pair of
 * the script (restarting it at -1) and copies nine colours from that point of
 * the palette buffer to background palette colours 3-11. */
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
