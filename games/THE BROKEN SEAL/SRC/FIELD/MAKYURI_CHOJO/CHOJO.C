#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "OVERLAY_OBJECT.H"
#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
#include "AERIE.H"

s32 FindNearestF2Actor(void);
void FieldScene_RunFourActorPresentation(void);
void RunScene59Sequence(void);
void MakyuriChojo_FlickerActorEight(void);

struct MapLayer {
    u8 unknown_00[12];
    s32 y;
    u8 unknown_10[32];
};

struct MapWork {
    u8 unknown_00[20];
    struct MapLayer layers[8];
};

/* The map work, read here as its layers. */
extern void *gMapWork;

void *SceneData_GetTableB938(void)
{
    return MakyuriChojo_ScriptTable;
}

s32 get_runtime_default_result(void)
{
    return 0;
}

void *SceneData_GetTableb9c8(void)
{
    return MakyuriChojo_MessageTable;
}

void *SceneData_GetTableB9d4AfterStateCheck(void)
{
    if (gGameState.entrance != 1) {
        GameFlag_Set(0x253);
    }
    return MakyuriChojo_ActorTable;
}

void *SceneData_GetTablebbe4(void)
{
    return MakyuriChojo_EventTable;
}

/* Mercury Lighthouse aerie entry: set the entrance selector and, in the aerie's first scene, raise the sprite priorities, lift actors 14-19 and set up the scene for the entrance. */
s32 MakyuriChojo_ApplyEntryState(void)
{
    u32 i;
    s32 actor;
    struct FieldActor *obj;

    Engine_GameFlagSet(0x111);
    gEventWork->start_transition = 0x204;
    if (gGameState.scene == (s32)&SceneId_MakyuriChojo1) {
    Engine_GameFlagSet(0x144);
    Engine_TaskAddCallback(MakyuriChojo_FlickerActorEight, 0xc80);
    Engine_ActorSetSpritePriority(0, 1);
    Engine_ActorSetSpritePriority(1, 1);
    Engine_ActorSetSpritePriority(2, 1);
    Engine_ActorSetSpritePriority(3, 1);
    Engine_ActorSetSpritePriority(5, 1);
    Engine_ActorSetSpritePriority(20, 1);
    Engine_ActorSetSpritePriority(21, 1);
    Engine_ActorSetSpritePriority(22, 1);
    Engine_ActorSetSpritePriority(23, 1);
    Engine_ActorSetSpritePriority(24, 1);
    Engine_ActorSetSpritePriority(8, 1);
    Engine_ActorSetSpritePriority(9, 1);
    Engine_ActorSetSpritePriority(10, 1);
    Engine_ActorSetSpritePriority(11, 1);
    Engine_ActorSetSpritePriority(12, 1);
    Engine_ActorSetSpritePriority(13, 1);
    for (i = 14; i < 20; i++) {
        Engine_ActorSetSpritePriority(i, 1);
        Object_GetById(i)->motion_flags = 4;
        Object_GetById(i)->priority_flags |= 2;
        Object_GetById(i)->y.fixed = -0x328000;
    }
    if (Engine_GameFlagIsSet(0x109) && (actor = FindNearestF2Actor()) != 0 && (obj = Object_GetById(actor)) != NULL) {
        obj->motion_flags = 0;
    }
    SetOverlayObjectMode(Object_GetById(9), 0);
    SetOverlayObjectMode(Object_GetById(10), 0);
    SetOverlayObjectMode(Object_GetById(11), 0);
    SetOverlayObjectMode(Object_GetById(12), 0);
    SetOverlayObjectMode(Object_GetById(13), 0);
    Object_GetById(12)->scale_x = -0x10000;
    Object_GetById(13)->scale_x = -0x10000;
    if (gGameState.entrance == 1) {
        if (!Engine_GameFlagIsSet(0x109)) {
            FieldScene_RunFourActorPresentation();
        }
    } else if (gGameState.entrance == 2) {
        if (!Engine_GameFlagIsSet(0x251)) {
            {
                struct MapLayer *layer = &((struct MapWork *)gMapWork)->layers[7];

                layer->y = 0x4000000;
            }
            Engine_MapRedraw();
            Engine_TaskWait(1);
            Map_CopyCellsTo(4, 70, 4, 74, 5, 4);
            Engine_ActorSetPosition(9, 0, 0);
            if (!Engine_GameFlagIsSet(0x109)) {
                RunScene59Sequence();
            }
        }
    } else if (gGameState.entrance == 5) {
        Engine_GameFlagSet(0x251);
    }
    }
    return 0;
}

void FieldScene_RunFourActorPresentation(void)
{
    u32 i;
    u8 *record;

    Event_Begin();
    Owner_RefreshRatiosOnFlagFar(); /* main:08077268 */
    record = ((u8 *)Object_GetById(0)); /* main:0808a080 */
    SetOverlayObjectMode(record, 0); /* main:080091e0 */
    record = ((u8 *)Object_GetById(1)); /* main:0808a080 */
    SetOverlayObjectMode(record, 0); /* main:080091e0 */
    record = ((u8 *)Object_GetById(2)); /* main:0808a080 */
    SetOverlayObjectMode(record, 0); /* main:080091e0 */
    record = ((u8 *)Object_GetById(3)); /* main:0808a080 */
    SetOverlayObjectMode(record, 0); /* main:080091e0 */
    Camera_MoveTo(0x1300000, -1, 0x780000, 0);
    Task_Wait(1); /* main:080000c0 */
    Map_Redraw(); /* main:08009128 */
    Task_Wait(1); /* main:080000c0 */
    Audio_PlayCue(141);
    Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000); /* main:080091f0 */
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666); /* main:080091f0 */
    SCENE_PHASE = 0x100;
    Event_OpenScreen(); /* main:0808a360 */
    Event_WaitForScreen(); /* main:0808a370 */
    MapRender_WaitForValues(); /* main:080091f8 */
    Event_Wait(30);
    /* Clear the byte at offset 85 of the record Battle_GetWorkObject1e0Far() returns. */
    *(u8 *)(Battle_GetWorkObject1e0Far() + 85) = 0;
    Camera_SetSpeed(0xcccc, 0x1999); /* speed_limit, acceleration */
    Camera_MoveTo(0x2000000, -0x180000, 0xa00000, 1);
    Camera_WaitForMove();
    ColorBuffer_ApplySource(0x10000, 0);
    ColorBuffer_ApplyTarget(0x10005, 0); /* main:0808a330 */
    ColorBuffer_Interpolate(50); /* main:0808a348 */
    Event_Wait(50);
    ColorBuffer_ApplyTarget(0x7fff, 0); /* main:0808a330 */
    ColorBuffer_Interpolate(30); /* main:0808a348 */
    Event_Wait(30);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x1f80000, 0xa80000);
    Actor_SetPosition(ACTOR_GERALD, 0x2100000, 0x900000);
    Actor_SetPosition(ACTOR_IVAN, 0x1e80000, 0x900000);
    Actor_SetPosition(ACTOR_MIA, 0x2000000, 0x980000);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 19); /* object 0, action 19 */
    Actor_SetAnimation(ACTOR_GERALD, 19);
    Actor_SetAnimation(ACTOR_IVAN, 19);
    Actor_SetAnimation(ACTOR_MIA, 19);
    Event_Wait(10);
    ColorBuffer_ApplyTarget(0x10000, 0); /* main:0808a330 */
    ColorBuffer_Interpolate(30); /* main:0808a348 */
    Event_Wait(30);
    Event_Wait(80);
    record = ((u8 *)Object_GetById(0));
    SetOverlayObjectMode(record, 1); /* main:080091e0 */
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(60);
    record = ((u8 *)Object_GetById(1)); /* main:0808a080 */
    SetOverlayObjectMode(record, 1); /* main:080091e0 */
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    record = ((u8 *)Object_GetById(2));
    SetOverlayObjectMode(record, 1); /* main:080091e0 */
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Event_Wait(40);
    record = ((u8 *)Object_GetById(3)); /* main:0808a080 */
    SetOverlayObjectMode(record, 1); /* main:080091e0 */
    Actor_SetAnimation(ACTOR_MIA, 1);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_MIA, 0xcccc, 0x6666);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    Actor_SetAnimation(ACTOR_IVAN, 2);
    Actor_SetAnimation(ACTOR_MIA, 2);
    record = ((u8 *)Object_GetById(0)); /* main:0808a080 */
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, ACTOR_FIELD_0XA(record), ACTOR_FIELD_0X12(record));
    }
    record = ((u8 *)Object_GetById(0)); /* main:0808a080 */
    if (record != 0) {
        Actor_SetDestination(ACTOR_IVAN, ACTOR_FIELD_0XA(record), ACTOR_FIELD_0X12(record));
    }
    record = ((u8 *)Object_GetById(0)); /* main:0808a080 */
    if (record != 0) {
        Actor_SetDestination(ACTOR_MIA, ACTOR_FIELD_0XA(record), ACTOR_FIELD_0X12(record));
    }
    Actor_WaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Event_End();
}
