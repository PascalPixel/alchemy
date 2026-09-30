#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "OVERLAY_OBJECT.H"
#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
#include "AERIE.H"
#include "IWRAM_CALL.H"
#include "CALL.H"

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

struct SceneActor {
    u8 unk_00[6]; u16 angle; s32 x,y,z; u8 unk_14[20];
    s32 motion28; u8 unk_2c[4]; s32 motion30,motion34;
    u8 unk_38[29]; u8 flags55;
};

extern void Vector_AddPolarOffsetFar(s32,s32,s32 *);
extern s32 Object_CheckMovementCollision(struct SceneActor *,s32 *);
extern void GameFlag_ClearBitFar(s32);
extern void SceneActor_SetMode55OnSevenRecords(void);
extern void Engine_TaskWait(s32);
extern void Engine_AudioPlayCue(s32);
extern void ObjectMotion_SetPositionAndCommitFar(s32,s32,s32);
extern u8 MsgMakyuriLighthouseAlreadyLit[];
void Party_SetFields1ceAnd1d0();
void BattleFx_SetWeightedResult();
extern u8 MsgMakyuriGot[];
void Engine_AudioPlayCue(s32 cue);
void Engine_ActorSetChildValue();
void Engine_EventWait(s32 frames);
void Effect_Spawn(s32 x, s32 y, s32 z, s32 dx, s32 dy, s32 dz, s32 lift, void *params);
void Engine_ActorSetPosition(s32 actor, s32 x, s32 y);

struct Actor {
    u8 pad[8];
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams {
    s32 count;
    s32 kind;
    s32 spread;
    s32 rise;
    u8 pad10[8];
    u16 tile;
    u16 pad1a;
    u8 pad1c[12];
};

struct Rec_39d {
    u8 pad00[8];
    s32 x;
    u8 pad0c[4];
    s32 y;
};

/* The scene's record list: 66 slots from event work +20. */
struct RecList_39d {
    u8 pad[20];
    struct Rec_39d *recs[66];
};

void MakyuriChojo_CollectBandSlots();
void Engine_AudioPlayCue();
void Engine_TaskWait();

struct Flags85 {
    u8 pad[85];
    u8 flags;
};

void Engine_MapRedraw();

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

/* The whole-pixel distance between two fixed-point positions, through the
 * resident square root. Linked into the field overlays that measure it on
 * its own. */
s32 FixedPoint_Distance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x * delta_x;
    s32 delta_y_squared = delta_y * delta_y;
    s32 delta_z_squared = delta_z * delta_z;

    return Iwram_Sqrt(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 FindNearestF2Actor(void)
{
    u8 *work = *(u8 **)&gEventWork;
    Actor **actor_slot;
    Actor *origin;
    s32 nearest_actor = 0;
    s32 min_dist;
    u32 actor_id;

    min_dist = 640;
    origin = (Actor *)Object_GetById(0);
    actor_id = 8;
    actor_slot = (Actor **)(work + 0x34);
    do {
        Actor *actor = *actor_slot++;
        if (actor != 0) {
            if (*actor->data->kind == 0xf2) {
                s32 dist = FixedPoint_Distance(
                    (u8 *)origin + 8, (u8 *)actor + 8);
                if (dist < min_dist) {
                    min_dist = dist;
                    nearest_actor = actor_id;
                }
            }
        }
        actor_id++;
    } while (actor_id <= 65);
    return nearest_actor;
}

/*
 * Makyuri aerie: initialize the actor's motion. It computes the actor's
 * tile-aligned position from its angle, and if the motion check passes it
 * arms the motion fields, clears the active flag, and queues the start-up
 * cues before restoring the saved flags.
 */
void SceneActor_InitializeMotion(void)
{
    struct SceneActor *actor;
    s32 position[3];
    s32 angle;
    u8 flags;
    actor = (struct SceneActor *)Object_GetById(0);
    angle = (actor->angle + 0x1000) & 0xe000;
    flags = actor->flags55;
    position[0] = (actor->x & 0xfff00000) + 0x80000;
    position[1] = actor->y;
    position[2] = (actor->z & 0xfff00000) + 0x80000;
    Vector_AddPolarOffsetFar(0x200000,angle,position);
    if (Object_CheckMovementCollision(actor,position) == 0) {
        GameFlag_ClearBitFar(592);
        SceneActor_SetMode55OnSevenRecords();
        Object_SetMode(actor,6);
        Engine_TaskWait(6);
        Engine_AudioPlayCue(152);
        Object_SetMode(actor,7);
        actor->motion30 = 0x30000;
        actor->motion34 = 0x20000;
        actor->motion28 = 0x40000;
        actor->flags55 &= 0x7e;
        SetOverlayObjectMode(actor,0);
        ObjectMotion_SetPositionAndCommitFar(0,(s16)(position[0] >> 16),(s16)(position[2] >> 16));
        Object_SetMode(actor,6);
        SetOverlayObjectMode(actor,1);
        actor->flags55 = flags;
    }
}

void FieldScene_RunScene39d_020009fc(void)
{
    u32 i;
    u8 *rec;
    u8 *rec8;
    s32 record;
    s32 nearest;

    rec = Actor_Get(ACTOR_PARTY_LEADER);
    Event_Begin();
    record = FindNearestF2Actor();
    nearest = (s32)MakyuriChojo_NearestActor;
    *(s32 *)nearest = record;
    if (record != 0) {
        GameFlag_Set(0x250);
        rec8 = Actor_Get(*(s32 *)nearest);
        rec8[85] = 0;
        rec[85] &= 254;
        *(s32 *)((s32)rec8 + 12) += -0x30000;
        *(s32 *)((s32)rec + 12) += -0x30000;
        *(s32 *)((s32)rec + 20) += -0x30000;
        Task_Wait(2);
        *(s32 *)((s32)rec8 + 12) += -0x20000;
        *(s32 *)((s32)rec + 12) += -0x20000;
        *(s32 *)((s32)rec + 20) += -0x20000;
        Task_Wait(10);
        *(s32 *)((s32)rec8 + 12) += 0x20000;
        *(s32 *)((s32)rec + 12) += 0x20000;
        *(s32 *)((s32)rec + 20) += 0x20000;
        Task_Wait(4);
        *(s32 *)((s32)rec8 + 12) += 0x20000;
        *(s32 *)((s32)rec + 12) += 0x20000;
        *(s32 *)((s32)rec + 20) += 0x20000;
        Task_Wait(4);
        *(s32 *)((s32)rec8 + 12) += 0x10000;
        *(s32 *)((s32)rec + 12) += 0x10000;
        *(s32 *)((s32)rec + 20) += 0x10000;
    }
    Event_End();
}

void SceneActor_SetMode55OnSevenRecords(void)
{
    ((struct Record *)Object_GetById(0))->mode55 = 3;
    ((struct Record *)Object_GetById(14))->mode55 = 4;
    ((struct Record *)Object_GetById(15))->mode55 = 4;
    ((struct Record *)Object_GetById(16))->mode55 = 4;
    ((struct Record *)Object_GetById(17))->mode55 = 4;
    ((struct Record *)Object_GetById(18))->mode55 = 4;
    ((struct Record *)Object_GetById(19))->mode55 = 4;
}

void RunScene58Sequence(void)
{
    void *temp_r0;
    void *temp_r0_10;
    void *temp_r0_11;
    void *temp_r0_12;
    void *temp_r0_13;
    void *temp_r0_2;
    void *temp_r0_3;
    void *temp_r0_4;
    void *temp_r0_5;
    void *temp_r0_6;
    void *temp_r0_7;
    void *temp_r0_8;
    void *temp_r0_9;
    void *temp_r2;
    void *temp_r2_2;
    void *temp_r2_3;
    void *temp_r2_4;
    s32 flag;
    s32 flag2;
    s32 bits;

    Event_Begin();
    temp_r0 = Actor_Get(0x11);
    FIELD(temp_r0, u8 *, 0x55) = (u8)(0xFA & FIELD(temp_r0, u8 *, 0x55));
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x0000cccc, 0x00006666);
    Actor_SetSpeed(ACTOR_GERALD, 0x0000cccc, 0x00006666);
    Actor_SetSpeed(ACTOR_IVAN, 0x0000cccc, 0x00006666);
    Actor_SetSpeed(ACTOR_MIA, 0x0000cccc, 0x00006666);
    temp_r0_2 = Actor_Get(ACTOR_PARTY_LEADER);
    if (temp_r0_2 != 0) {
        Actor_SetPosition(ACTOR_GERALD, FIELD(temp_r0_2, s32 *, 8), FIELD(temp_r0_2, s32 *, 0x10));
    }
    temp_r0_3 = Actor_Get(ACTOR_PARTY_LEADER);
    if (temp_r0_3 != 0) {
        Actor_SetPosition(ACTOR_IVAN, FIELD(temp_r0_3, s32 *, 8), FIELD(temp_r0_3, s32 *, 0x10));
    }
    temp_r0_4 = Actor_Get(ACTOR_PARTY_LEADER);
    if (temp_r0_4 != 0) {
        Actor_SetPosition(ACTOR_MIA, FIELD(temp_r0_4, s32 *, 8), FIELD(temp_r0_4, s32 *, 0x10));
    }
    Task_Wait(1);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x158, 0xE8);
    Actor_WalkTo(ACTOR_GERALD, 0x148, 0xE8);
    Actor_WalkTo(ACTOR_IVAN, 0x158, 0xF8);
    Actor_WalkTo(ACTOR_MIA, 0x148, 0xF8);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xA000, 0);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_GERALD, 0xC000, 0);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_IVAN, 0xA000, 0);
    Actor_WaitForMove(ACTOR_MIA);
    Actor_FaceDirection(ACTOR_MIA, 0xC000, 0);
    Event_Wait(50);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Camera_SetSpeed(0x18000, 0x3000);
    Camera_MoveTo(PIXELS(0x148), PIXELS(0x28), PIXELS(0xB0), 1);
    FIELD(Battle_GetWorkObject1e0Far(), s8 *, 0x55) = 0;
    Actor_SetSpeed(ACTOR_GERALD, 0x18000, 0xC000);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x148, 0xD8);
    Camera_WaitForMove();
    Actor_SetSpeed(ACTOR_GERALD, 0x0000cccc, 0x00006666);
    Event_Wait(60);
    Camera_MoveTo(PIXELS(0x158), PIXELS(0x18), PIXELS(0xE8), 1);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 20);
    Camera_WaitForMove();
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Event_SetValue1d8Far((s32)MsgMakyuriLighthouseAlreadyLit);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Event_Wait(60);
    Actor_SetSpeed(ACTOR_MIA, 0x18000, 0xC000);
    Actor_WalkToAndWait(ACTOR_MIA, 0x148, 0xE8);
    Event_Wait(10);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 30);
    Object_LinkPairFar(2, 0, 0);
    Event_Wait(60);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x150, 0xF8);
    Actor_FaceDirection(ACTOR_IVAN, 0xA000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Event_Wait(30);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, 0, 30);
    UiText_OpenMessageAtObjectFar(3, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_ShowEmote(ACTOR_MIA, 0x100, 60);
        Actor_FaceDirection(ACTOR_GERALD, 0x4000, 10);
        Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
        Actor_FaceDirection(ACTOR_MIA, 0xC000, 0);
        Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
        Event_Wait(20);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
        temp_r2 = gWork;
        FIELD(temp_r2, u16 *, 0x1D8) = (u16)(FIELD(temp_r2, u16 *, 0x1D8) + 2);
    } else {
        temp_r2_2 = gWork;
        FIELD(temp_r2_2, u16 *, 0x1D8) = (u16)(FIELD(temp_r2_2, u16 *, 0x1D8) + 2);
        Event_Wait(20);
        Actor_ShowEmote(ACTOR_MIA, 0x00000101, 60);
        Actor_FaceDirection(ACTOR_GERALD, 0x4000, 10);
        Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
        Actor_FaceDirection(ACTOR_MIA, 0xC000, 0);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        Event_Wait(20);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    }
    Actor_ShowEmote(ACTOR_MIA, 0x00000101, 60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Object_LinkPairFar(0, 1, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_MIA, 0, 10);
    Actor_SetAnimation(ACTOR_MIA, 0x10);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 60);
    Audio_PlayCue(0x11);
    Actor_SetPosition(5, PIXELS(0xD8), PIXELS(0xC8));
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Actor_SetPosition(5, PIXELS(0x78), PIXELS(0xA0));
    Actor_FaceDirection(5, 0, 0);
    Actor_SetAnimation(ACTOR_MIA, 1);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 10);
    Actor_FaceDirection(ACTOR_GERALD, 0xE000, 20);
    Actor_FaceDirection(ACTOR_IVAN, 0xA000, 10);
    Actor_FaceDirection(ACTOR_MIA, 0x8000, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xA000, 5);
    Actor_FaceDirection(ACTOR_GERALD, 0xC000, 10);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 5);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 5);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Camera_SetSpeed(0x30000, 0x6000);
    Camera_MoveTo(PIXELS(0x78), PIXELS(0xFFE8), PIXELS(0xA8), 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Actor_RunRepeatedMotion(0x15, 1);
    Event_Wait(20);
    Audio_PlayCue(0x3D);
    Event_ShowMessageAndWait(0x15, 0, 20);
    Actor_RunRepeatedMotion(0x17, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x17, 0, 20);
    Actor_FaceDirection(0x17, 0xC000, 60);
    Actor_FaceDirection(0x17, 0, 20);
    Event_ShowMessageAndWait(0x17, 0, 20);
    Actor_SetPosition(ACTOR_GERALD, PIXELS(0x108), PIXELS(0x120));
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_SetPosition(ACTOR_GERALD, PIXELS(0x148), PIXELS(0xD8));
    Actor_SetAnimationAndWait(0x17, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x17, 0, 20);
    Actor_SetAnimationAndWait(0x17, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x17, 0, 20);
    Actor_FaceDirection(0x17, 0xA000, 10);
    Actor_FaceDirection(0x15, 0x2000, 10);
    Actor_RunRepeatedMotion(0x15, 1);
    Event_Wait(20);
    Actor_FaceDirection(5, 0x8000, 0);
    Actor_FaceDirection(0x14, 0x8000, 30);
    Actor_FaceDirection(0x15, 0, 30);
    Actor_RunRepeatedMotion(5, 2);
    Actor_SetAnimationAndWait(0x15, 4);
    Event_Wait(20);
    Actor_FaceDirection(0x15, 0x2000, 10);
    Actor_SetAnimationAndWait(0x15, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(0x17, 3);
    Actor_SetSpeed(0x15, 0x0000cccc, 0x00006666);
    Actor_WalkToAndWait(0x15, 0x68, 0xA8);
    Event_Wait(20);
    Actor_FaceDirection(0x17, 0, 20);
    Actor_FaceDirection(0x14, 0, 10);
    Actor_ShowEmote(5, 0x102, 0);
    Actor_ShowEmote(0x14, 0x102, 70);
    Actor_SetPosition(0x16, PIXELS(0x108), PIXELS(0x120));
    Event_ShowMessageAndWait(0x16, 0, 20);
    Actor_SetPosition(0x16, PIXELS(0x128), PIXELS(0x78));
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xA000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xC000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xC000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xC000, 0);
    Actor_FaceDirection(5, 0, 0);
    Actor_FaceDirection(0x14, 0, 0);
    Camera_SetSpeed(0x18000, 0x3000);
    Camera_MoveTo(PIXELS(0xE8), PIXELS(0x28), PIXELS(0x98), 1);
    Actor_SetSpeed(0x16, 0x0000cccc, 0x00006666);
    Actor_WalkToAndWait(0x16, 0x110, 0x80);
    Actor_WalkToAndWait(0x16, 0x108, 0x98);
    Actor_WalkToAndWait(0x16, 0x118, 0xA8);
    Camera_WaitForMove();
    Actor_FaceDirection(0x16, 0x5000, 20);
    Event_Wait(20);
    Event_ShowMessage(0x17, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(0x16, 3);
    Event_Wait(20);
    Event_ShowMessage(0x16, 0);
    Camera_MoveTo(PIXELS(0x128), PIXELS(0x28), PIXELS(0xD8), 1);
    Actor_WalkToAndWait(0x16, 0x130, 0xB0);
    Actor_FaceDirection(0x16, 0x2000, 0);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_Wait(20);
    Event_Wait(20);
    Actor_SetAnimationAndWait(0x16, 3);
    Event_Wait(20);
    Event_ShowMessage(0x16, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x00000107, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x00000107, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x00000107, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x00000107, 70);
    Actor_RunRepeatedMotion(0x16, 1);
    Event_Wait(30);
    Event_ShowMessage(0x16, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x00000105, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x00000105, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x00000105, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x00000105, 70);
    Actor_SetAnimationAndWait(0x16, 3);
    Event_Wait(20);
    Event_ShowMessage(0x16, 0);
    Event_Wait(20);
    Event_ShowMessage(0x17, 0);
    Event_Wait(10);
    Actor_FaceDirection(0x16, 0x5000, 0);
    Event_Wait(40);
    Event_ShowMessage(0x16, 0);
    Event_Wait(30);
    Event_ShowMessage(0x17, 0);
    Event_Wait(10);
    Actor_FaceDirection(0x16, 0x3000, 0);
    Event_Wait(30);
    Event_ShowMessage(0x16, 0);
    Event_Wait(20);
    temp_r0_5 = Actor_Get(ACTOR_GERALD);
    FIELD(temp_r0_5, u8 *, 0x5A) = (u8)(0xFE & FIELD(temp_r0_5, u8 *, 0x5A));
    Actor_WalkTo(ACTOR_GERALD, 0x148, 0xE0);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x158, 0xE0);
    Actor_WalkTo(ACTOR_IVAN, 0x158, 0xE8);
    Actor_WaitForMove(ACTOR_GERALD);
    flag = 1;
    temp_r0_6 = Actor_Get(ACTOR_GERALD);
    bits = FIELD(temp_r0_6, u8 *, 0x5A) | flag;
    FIELD(temp_r0_6, u8 *, 0x5A) = bits;
    Actor_FaceDirection(ACTOR_GERALD, 0xC000, 0);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xA000, 0);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_IVAN, 0xC000, 0);
    Event_Wait(30);
    Actor_SetPosition(0x17, PIXELS(0xA8), PIXELS(0xC8));
    Event_ShowMessageAndWait(0x17, 0, 20);
    Actor_SetPosition(0x17, PIXELS(0x68), PIXELS(0xC8));
    Actor_RunRepeatedMotion(0x16, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x16, 0, 20);
    Camera_SetSpeed(0x30000, 0x6000);
    Camera_MoveTo(PIXELS(0x78), PIXELS(0xFFE8), PIXELS(0xA8), 1);
    Camera_WaitForMove();
    Actor_SetAnimationAndWait(0x17, 3);
    Event_Wait(10);
    Actor_FaceDirection(0x17, 0xD000, 20);
    Actor_SetAnimationAndWait(0x17, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x17, 0, 20);
    Actor_FaceDirection(5, 0x5000, 0);
    Actor_FaceDirection(0x14, 0x3000, 70);
    Object_LinkPairFar(5, 0x14, 0);
    Event_Wait(50);
    Actor_FaceDirection(5, 0x5000, 0);
    Actor_FaceDirection(0x14, 0x3000, 20);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, 4);
    Event_Wait(20);
    Actor_ShowEmote(0x17, 0x100, 60);
    Event_ShowMessageAndWait(0x17, 0, 20);
    Actor_ShowEmote(5, 0x00000105, 60);
    Actor_FaceDirection(0x15, 0xD000, 20);
    Actor_RunRepeatedMotion(0x15, 1);
    Actor_SetAnimationAndWait(0x17, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x17, 0, 20);
    Actor_SetAnimationAndWait(0x17, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x17, 0, 20);
    Actor_SetAttachedEffect(0x15, 0x102);
    Event_Wait(60);
    Actor_SetAnimationAndWait(0x15, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x15, 0, 20);
    Actor_SetAttachedEffect(ACTOR_JASMINE, 0x102);
    Event_Wait(60);
    Event_ShowMessageAndWait(5, 0, 40);
    Actor_SetAnimationAndWait(0x15, 3);
    Event_Wait(60);
    Actor_FaceDirection(5, 0xA000, 10);
    Actor_SetAnimationAndWait(0x14, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x14, 0, 30);
    Actor_SetSpeed(5, 0x0000b333, 0x00005999);
    Actor_SetSpeed(0x14, 0x0000b333, 0x00005999);
    Actor_WalkTo(5, 0x80, 0x90);
    Actor_WalkToAndWait(0x14, 0x78, 0x88);
    Actor_FaceDirection(0x14, 0x5000, 0);
    Actor_WaitForMove(5);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 20);
    Actor_FaceDirection(0x15, 0x3000, 20);
    temp_r0_7 = ((s32)Object_GetById(0x15));
    FIELD(temp_r0_7, u8 *, 0x5A) = (u8)(0xFE & FIELD(temp_r0_7, u8 *, 0x5A));
    Actor_WalkToAndWait(0x15, 0x58, 0x98);
    temp_r0_8 = Actor_Get(0x15);
    bits = FIELD(temp_r0_8, u8 *, 0x5A) | flag;
    FIELD(temp_r0_8, u8 *, 0x5A) = bits;
    Actor_FaceDirection(0x17, 0xB000, 20);
    Actor_SetAnimation(0x17, 3);
    Actor_SetAnimationAndWait(0x15, 3);
    Event_Wait(40);
    Actor_SetSpeed(0x17, 0x30000, 0x20000);
    FIELD(Actor_Get(0x17), s32 *, 0x28) = 0x40000;
    Audio_PlayCue(0x98);
    temp_r0_9 = Actor_Get(0x17);
    FIELD(temp_r0_9, u8 *, 0x55) = (u8)(0x7E & FIELD(temp_r0_9, u8 *, 0x55));
    SetOverlayObjectMode(((s32)Object_GetById(0x17)), 0);
    FIELD(((s32)Object_GetById(0x11)), s8 *, 0x55) = 4;
    ObjectMotion_SetPositionAndCommitFar(0x17, 0x68, 0xA8);
    SetOverlayObjectMode(((s32)Object_GetById(0x17)), 1);
    FIELD(Actor_Get(0x17), s8 *, 0x55) = 3;
    Actor_FaceDirection(0x17, 0, 30);
    Actor_FaceDirection(0x15, 0, 10);
    Event_ShowMessageAndWait(0x15, 0, 20);
    Actor_RunRepeatedMotion(5, 2);
    Actor_SetAnimationAndWait(5, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(5, 0, 20);
    Event_ShowMessageAndWait(0x17, 0, 20);
    Actor_SetSpritePriority(0x11, 0);
    Actor_SetSpritePriority(0x12, 0);
    MakyuriChojo_LowerCollectedActors();
    Actor_SetSpritePriority(0x11, 1);
    Actor_SetSpritePriority(0x12, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x6000, 0);
    Camera_MoveTo(PIXELS(0x130), PIXELS(0x20), PIXELS(0xD8), 1);
    Camera_WaitForMove();
    Actor_SetPosition(0x14, PIXELS(0x110), PIXELS(0x118));
    Event_ShowMessageAndWait(0x14, 0, 20);
    Actor_SetPosition(0x14, 0, 0);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_SetSpeed(ACTOR_GERALD, 0x18000, 0xC000);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x138, 0xD8);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 10);
    Event_ShowMessageAndWait(0x16, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xA000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xC000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xC000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xC000, 0);
    Actor_WalkToAndWait(0x16, 0x138, 0xB8);
    Actor_FaceDirection(0x16, 0x3000, 20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 60);
    Actor_SetSpeed(ACTOR_GERALD, 0x20000, 0x10000);
    temp_r0_10 = Actor_Get(ACTOR_GERALD);
    FIELD(temp_r0_10, u8 *, 0x5A) = (u8)(0xFE & FIELD(temp_r0_10, u8 *, 0x5A));
    Actor_WalkToAndWait(ACTOR_GERALD, 0x148, 0xE0);
    Event_Wait(1);
    temp_r0_11 = Actor_Get(ACTOR_GERALD);
    flag |= FIELD(temp_r0_11, u8 *, 0x5A);
    FIELD(temp_r0_11, u8 *, 0x5A) = flag;
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_Wait(20);
    Actor_RunRepeatedMotion(0x16, 1);
    Event_Wait(10);
    Event_ShowMessageAndWait(0x16, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Event_Wait(10);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_ShowEmote(0x16, 0x00000101, 60);
    UiText_OpenMessageAtObjectFar(0x16, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAnimationAndWait(0x16, 4);
        Event_Wait(20);
        Event_ShowMessageAndWait(0x16, 0, 20);
        temp_r2_3 = gWork;
        FIELD(temp_r2_3, u16 *, 0x1D8) = (u16)(FIELD(temp_r2_3, u16 *, 0x1D8) + 1);
    } else {
        Event_Wait(20);
        Actor_SetAnimationAndWait(0x16, 4);
        Event_Wait(20);
        temp_r2_4 = gWork;
        FIELD(temp_r2_4, u16 *, 0x1D8) = (u16)(FIELD(temp_r2_4, u16 *, 0x1D8) + 1);
        Event_ShowMessageAndWait(0x16, 0, 20);
    }
    Actor_RunRepeatedMotion(0x16, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x16, 0, 20);
    Actor_WalkToAndWait(0x16, 0x148, 0xC8);
    Actor_RunRepeatedMotion(0x16, 2);
    Actor_FaceDirection(0x16, 0xB000, 20);
    temp_r0_12 = Actor_Get(0x16);
    FIELD(temp_r0_12, u8 *, 0x5A) = (u8)(0xFE & FIELD(temp_r0_12, u8 *, 0x5A));
    Actor_WalkToAndWait(0x16, 0x150, 0xD0);
    Event_Wait(1);
    temp_r0_13 = ((s32)Object_GetById(0x16));
    flag2 = 1;
    flag2 |= FIELD(temp_r0_13, u8 *, 0x5A);
    FIELD(temp_r0_13, u8 *, 0x5A) = flag2;
    Actor_ShowEmote(0x16, 0x102, 60);
    Actor_ShowEmote(ACTOR_MIA, 0x00000101, 60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Event_ShowMessageAndWait(0x16, 0, 20);
    Actor_FaceDirection(0x16, 0x5000, 20);
    Actor_RunRepeatedMotion(0x16, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x16, 0, 20);
    Actor_WalkToAndWait(0x16, 0x150, 0xD8);
    Party_SetFields1ceAnd1d0((s32)&SceneId_MakyuriChojo1, 2);
    /* FAKEMATCH: the do/while loads the game state's base before the 0x22b
       offset, which fixes their registers and literal-pool order. */
    do {
        gGameState.unknown_200[0x22b - 0x200] = 3;
    } while (0);
    BattleFx_SetWeightedResult(0x24, 2);
    Event_End();
}

void RunScene59Sequence(void)
{
    s32 actor9_fixed_y;
    void *actor_one_record;
    void *scene_counter_initial;
    void *scene_counter_initial_alt;
    void *scene_counter_mid_a;
    void *scene_counter_mid_b;
    void *scene_counter_first_a;
    void *scene_counter_first_b;
    void *scene_counter_system_a;
    void *scene_counter_system_b;
    void *scene_counter_later_a;
    void *scene_counter_later_b;
    void *scene_counter_final_a;
    void *scene_counter_final_b;

    Event_Begin();
    FIELD(Actor_Get(9), s8 *, 0x55) = 0;
    Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(0x158), PIXELS(0xE0));
    Actor_SetPosition(ACTOR_GERALD, PIXELS(0x148), PIXELS(0xE0));
    Actor_SetPosition(ACTOR_IVAN, PIXELS(0x158), PIXELS(0xE8));
    Actor_SetPosition(ACTOR_MIA, PIXELS(0x148), PIXELS(0xE8));
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xA000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xC000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xC000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xC000, 0);
    Actor_SetPosition(0x16, PIXELS(0x150), PIXELS(0xB0));
    Actor_SetAnimation(0x16, 9);
    SetOverlayObjectMode(((s32)Object_GetById(0x16)), 0);
    Camera_MoveTo(PIXELS(0x150), -1, PIXELS(0xD0), 0);
    Task_Wait(1);
    Map_Redraw();
    Task_Wait(1);
    /* The ROM loads this IWRAM pointer cell before the request store. */
    EVENT_TRANSITION(gWork) = 0x100;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0, 20);
    Event_SetValue1d8Far((s32)MsgMakyuriGot);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(0x16, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x16, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xC000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xC000, 20);
    Actor_ShowEmote(ACTOR_IVAN, 0x00000101, 60);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_RunRepeatedMotion(0x16, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x16, 0, 30);
    Actor_SetPosition(0x18, PIXELS(0x138), PIXELS(0x70));
    Audio_PlayCue(0x120);
    Event_ShowMessageAndWait(0x18, 0, 20);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 30);
    Audio_PlayCue(0x1D);
    Camera_SetSpeed(0x0000cccc, 0x00001999);
    FIELD(Battle_GetWorkObject1e0Far(), s8 *, 0x55) = 0;
    Camera_MoveTo(PIXELS(0x150), -1, PIXELS(0xA8), 1);
    Event_Wait(20);
    Actor_SetSpeed(0x18, 0x0000cccc, 0x00006666);
    Actor_WalkToAndWait(0x18, 0x158, 0x88);
    Actor_FaceDirection(0x18, 0x5000, 20);
    Camera_WaitForMove();
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 30);
    Camera_MoveTo(PIXELS(0x150), -1, PIXELS(0xB8), 1);
    Actor_WalkToAndWait(0x18, 0x158, 0xA0);
    Actor_WalkToAndWait(0x18, 0x148, 0xA8);
    Actor_WalkToAndWait(0x18, 0x138, 0xB0);
    Actor_FaceDirection(0x18, 0x3000, 20);
    Camera_WaitForMove();
    Actor_SetAnimationAndWait(0x18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x18, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 20);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_SetAnimationAndWait(0x18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x18, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xA000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xE000, 60);
    Actor_FaceDirection(0x18, 0xB000, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xA000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xC000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xA000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xC000, 20);
    Event_ShowMessageAndWait(0x18, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_StartRepeatedMotion(0x18, 2);
    Actor_ShowEmote(0x18, 0x100, 60);
    Actor_FaceDirection(0x18, 0x3000, 20);
    Event_ShowMessageAndWait(0x18, 0, 20);
    Actor_ShowEmote(ACTOR_MIA, 0x00000101, 60);
    Actor_SetAnimationAndWait(0x18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x18, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_SetAnimationAndWait(0x18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x18, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_WalkToAndWait(0x18, 0x138, 0xB8);
    Actor_FaceDirection(0x18, 0x3000, 20);
    Event_ShowMessageAndWait(0x18, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0, 20);
    UiText_OpenMessageAtObjectFar(1, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
        scene_counter_initial = gWork;
        EVENT_MESSAGE(scene_counter_initial) = (u16)(EVENT_MESSAGE(scene_counter_initial) + 1);
    } else {
        scene_counter_initial_alt = gWork;
        EVENT_MESSAGE(scene_counter_initial_alt) = (u16)(EVENT_MESSAGE(scene_counter_initial_alt) + 1);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    }
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0xC000, 20);
    Actor_SetAnimationAndWait(0x18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x18, 0, 20);
    Actor_FaceDirection(0x18, 0xD000, 20);
    Event_ShowMessageAndWait(0x18, 0, 20);
    Actor_FaceDirection(0x18, 0x3000, 20);
    Actor_SetAnimationAndWait(0x18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x18, 0, 20);
    Actor_ShowEmote(ACTOR_MIA, 0x00000101, 60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_SetAnimationAndWait(0x18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x18, 0, 20);
    Actor_FaceDirection(0x18, 0xB000, 20);
    Event_ShowMessageAndWait(0x18, 0, 20);
    Event_Wait(10);
    Actor_RunRepeatedMotion(0x18, 2);
    Event_Wait(30);
    Event_ShowMessage(0x18, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(30);
    Event_ShowMessage(1, 0);
    Event_Wait(10);
    Actor_FaceDirection(0x18, 0x2000, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(0x18, 4);
    Event_Wait(20);
    Event_ShowMessage(0x18, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(0x18, 2);
    Event_Wait(30);
    Event_ShowMessage(0x18, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 60);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(0x18, 3);
    Event_Wait(30);
    Event_ShowMessage(0x18, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(0x18, 2);
    Event_Wait(30);
    Event_ShowMessage(0x18, 0);
    Event_Wait(10);
    Actor_FaceDirection(0x18, 0x1000, 0);
    Event_Wait(30);
    Actor_ShowEmote(0x18, 0x108, 60);
    Event_Wait(10);
    Actor_RunRepeatedMotion(0x16, 2);
    Event_Wait(30);
    Actor_SetAnimation(0x16, 8);
    Event_Wait(45);
    Actor_SetAnimation(0x16, 1);
    SetOverlayObjectMode(((s32)Object_GetById(0x16)), 1);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 60);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_FaceDirection(0x16, 0x2000, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(0x16, 4);
    Event_Wait(30);
    Event_ShowMessage(0x16, 0);
    Event_Wait(20);
    Actor_FaceDirection(0x18, 0x2000, 0);
    Event_Wait(30);
    Event_ShowMessage(0x18, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(30);
    Event_ShowMessage(3, 0);
    Event_Wait(10);
    Object_LinkPairFar(0x18, 0x16, 0);
    Event_Wait(35);
    Actor_SetAnimationAndWait(0x16, 3);
    Event_Wait(30);
    Actor_SetAnimationAndWait(0x18, 3);
    Event_Wait(20);
    Actor_WalkToAndWait(0x18, 0x148, 0xB0);
    Actor_FaceDirection(0x18, 0x3000, 20);
    Actor_SetAnimation(0x18, 5);
    Actor_SetAnimation(0x16, 7);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    Event_ShowMessage(1, 0);
    Event_Wait(10);
    Actor_ShowEmote(0x18, 0x00000101, 60);
    UiText_OpenMessageAtObjectFar(0x18, 0);
    Event_Wait(30);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xC000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xE000, 0);
    Event_Wait(20);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(30);
        Event_ShowMessage(0x18, 0);
        scene_counter_first_a = gWork;
        EVENT_MESSAGE(scene_counter_first_a) = (u16)(EVENT_MESSAGE(scene_counter_first_a) + 1);
    } else {
        Event_Wait(30);
        /* This branch needs its own workspace load; stale register contents
         * are not a valid C dependency. */
        scene_counter_first_b = gWork;
        EVENT_MESSAGE(scene_counter_first_b) = (u16)(EVENT_MESSAGE(scene_counter_first_b) + 1);
        Event_ShowMessage(0x18, 0);
    }
    Event_Wait(20);
    Actor_SetAnimationAndWait(0x18, 4);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0xC000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xA000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xC000, 0);
    Event_Wait(20);
    Event_ShowMessage(0x18, 0);
    Actor_RunRepeatedMotion(0x16, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x16, 0, 20);
    MakyuriChojo_SinkActorPair();
    Event_Wait(20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 5);
    Actor_FaceDirection(ACTOR_GERALD, 0xE000, 5);
    Actor_FaceDirection(ACTOR_IVAN, 0xA000, 5);
    Actor_FaceDirection(ACTOR_MIA, 0x8000, 5);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xE000, 5);
    Actor_FaceDirection(ACTOR_GERALD, 0xA000, 5);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 5);
    Actor_FaceDirection(ACTOR_MIA, 0, 5);
    FIELD(Battle_GetWorkObject1e0Far(), s8 *, 0x55) = 0;
    Camera_MoveTo(PIXELS(0x118), -1, PIXELS(0xE8), 1);
    Camera_WaitForMove();
    Actor_SetChildValue(0x16, 0xF);
    Actor_SetChildValue(0x18, 0xF);
    Actor_SetPosition(0x16, PIXELS(0xF0), PIXELS(0xD0));
    Actor_SetPosition(0x18, PIXELS(0xE8), PIXELS(0xD0));
    Actor_FaceDirection(0x16, 0x5000, 0);
    Actor_FaceDirection(0x18, 0x3000, 0);
    MakyuriChojo_RiseActorPair();
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 10);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 15);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x8000, 20);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x148, 0xD0);
    Actor_WalkTo(ACTOR_IVAN, 0x150, 0xE0);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x138, 0xD8);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 0);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 30);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 20);
    Event_ShowMessageAndWait(0x18, 0, 20);
    Camera_SetSpeed(0x30000, 0x6000);
    {
        void **scene_system_cell;
        Camera_MoveTo(PIXELS(0x98), -1, PIXELS(0xD8), 1);
        scene_system_cell = (void **)(gCam + 0x164);
        FIELD(scene_system_cell, s32 *, 0xC) = 0x03800000;
        scene_system_cell = (void **)&gCam;
        Map_Redraw();
        Task_Wait(1);
        FIELD(Actor_Get(9), s8 *, 0x55) = 0;
        Actor_SetPosition(9, PIXELS(0x68), PIXELS(0x108));
        actor9_fixed_y = 0xffe00000;
        FIELD(Actor_Get(9), s32 *, 0xC) = actor9_fixed_y;
        FIELD(((s32)Object_GetById(9)), s32 *, 0x3C) = actor9_fixed_y;
        Map_CopyCellsTo(0x1D, 0x4A, 4, 0x4A, 5, 4);
        Actor_SetSpritePriority(0x11, 0);
        Actor_SetSpritePriority(0x12, 0);
        MakyuriChojo_RaiseCollectedActors();
        Actor_SetSpritePriority(0x11, 1);
        Actor_SetSpritePriority(0x12, 1);
        Camera_WaitForMove();
        Event_Wait(30);
        Actor_FaceDirection(0x18, 0x8000, 20);
        Event_ShowMessageAndWait(0x18, 0, 20);
        Camera_MoveTo(PIXELS(0x118), -1, PIXELS(0xD8), 1);
        Camera_WaitForMove();
        Actor_FaceDirection(0x18, 0x3000, 20);
        Actor_SetAnimation(0x18, 5);
        Event_ShowMessageAndWait(0x18, 0, 20);
        Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
        Event_Wait(20);
        Actor_FaceDirection(ACTOR_IVAN, 0xA000, 20);
        Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
        Event_Wait(20);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
        Actor_ShowEmote(0x18, 0x00000101, 60);
        UiText_OpenMessageAtObjectFar(0x18, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_Wait(20);
            Actor_RunRepeatedMotion(0x16, 2);
            Event_Wait(20);
            Event_ShowMessageAndWait(0x16, 0, 20);
            scene_counter_system_a = FIELD(scene_system_cell, void **, 0x4C);
            EVENT_MESSAGE(scene_counter_system_a) = (u16)(EVENT_MESSAGE(scene_counter_system_a) + 1);
        } else {
            scene_counter_system_b = FIELD(scene_system_cell, void **, 0x4C);
            EVENT_MESSAGE(scene_counter_system_b) = (u16)(EVENT_MESSAGE(scene_counter_system_b) + 1);
            Event_Wait(20);
            Event_ShowMessageAndWait(0x16, 0, 20);
        }
    }
    Actor_RunRepeatedMotion(0x18, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x18, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x00000101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x00000101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x00000101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x00000101, 60);
    UiText_OpenMessageAtObjectFar(0x18, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_RunRepeatedMotion(0x18, 2);
        Event_Wait(20);
        Event_ShowMessageAndWait(0x18, 0, 20);
        scene_counter_mid_a = gWork;
        EVENT_MESSAGE(scene_counter_mid_a) = (u16)(EVENT_MESSAGE(scene_counter_mid_a) + 1);
    } else {
        scene_counter_mid_b = gWork;
        EVENT_MESSAGE(scene_counter_mid_b) = (u16)(EVENT_MESSAGE(scene_counter_mid_b) + 1);
        Event_Wait(20);
        Actor_RunRepeatedMotion(0x18, 1);
        Event_Wait(20);
        Event_ShowMessageAndWait(0x18, 0, 20);
    }
    Event_Wait(20);
    MakyuriChojo_SinkActorPair();
    Event_Wait(20);
    Camera_MoveTo(PIXELS(0x80), -1, PIXELS(0xC8), 1);
    Camera_WaitForMove();
    Actor_SetAnimation(0x16, 1);
    Actor_SetAnimation(0x18, 1);
    Actor_SetChildValue(0x16, 0xF);
    Actor_SetChildValue(0x18, 0xF);
    Actor_SetPosition(0x16, PIXELS(0x78), PIXELS(0x98));
    Actor_SetPosition(0x18, PIXELS(0x70), PIXELS(0xA0));
    Actor_FaceDirection(0x16, 0x5000, 0);
    Actor_FaceDirection(0x18, 0x3000, 0);
    MakyuriChojo_RiseActorPair();
    Event_Wait(30);
    Actor_FaceDirection(0x18, 0, 20);
    UiText_OpenMessageAtObjectFar(0x18, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(0x18, 3);
        Event_Wait(20);
        Event_ShowMessageAndWait(0x18, 0, 20);
        scene_counter_later_a = gWork;
        EVENT_MESSAGE(scene_counter_later_a) = (u16)(EVENT_MESSAGE(scene_counter_later_a) + 1);
    } else {
        scene_counter_later_b = gWork;
        EVENT_MESSAGE(scene_counter_later_b) = (u16)(EVENT_MESSAGE(scene_counter_later_b) + 1);
        Actor_SetAnimationAndWait(0x18, 4);
        Event_Wait(20);
        Event_ShowMessageAndWait(0x18, 0, 20);
    }
    Event_ShowMessageAndWait(0x18, 0, 20);
    Actor_RunRepeatedMotion(0x16, 1);
    Event_Wait(20);
    Actor_FaceDirection(0x16, 0, 20);
    Event_ShowMessageAndWait(0x16, 0, 20);
    Event_Wait(20);
    Actor_FaceDirection(0x18, 0, 20);
    Actor_SetSpritePriority(0x11, 0);
    Actor_SetSpritePriority(0x12, 0);
    MakyuriChojo_LowerCollectedActors();
    Actor_SetSpritePriority(0x11, 1);
    Actor_SetSpritePriority(0x12, 1);
    Audio_PlayCue(0x11);
    Camera_MoveTo(PIXELS(0x150), -1, PIXELS(0xD8), 1);
    Camera_WaitForMove();
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Event_Wait(20);
    Audio_PlayCueFromEventWorkFar();
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 22);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(10);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_ShowEmote(ACTOR_MIA, 0x00000105, 60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, 0xA000, 20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, 0x8000, 20);
    Actor_SetAnimation(ACTOR_MIA, 0x10);
    FIELD(Actor_Get(ACTOR_MIA), s32 *, 0x18) = (s32) 0xffff0000;
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 40);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_IVAN, 0xA000, 20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Event_Wait(20);
    UiText_OpenMessageAtObjectFar(2, 0);
    Actor_SetAnimation(ACTOR_MIA, 1);
    FIELD(Actor_Get(ACTOR_MIA), s32 *, 0x18) = 0x10000;
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xE000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xC000, 20);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
        Event_Wait(20);
        Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
        scene_counter_final_a = gWork;
        EVENT_MESSAGE(scene_counter_final_a) = (u16)(EVENT_MESSAGE(scene_counter_final_a) + 1);
    } else {
        scene_counter_final_b = gWork;
        EVENT_MESSAGE(scene_counter_final_b) = (u16)(EVENT_MESSAGE(scene_counter_final_b) + 1);
        Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
        Event_Wait(20);
        Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    }
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, 0xE000, 30);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(10);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, 0xA000, 20);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x00000101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x00000101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x00000101, 60);
    Actor_SetSpeed(ACTOR_MIA, 0x0000cccc, 0x00006666);
    Actor_SetSpeed(ACTOR_GERALD, 0x0000cccc, 0x00006666);
    Actor_WalkToAndWait(ACTOR_MIA, 0x148, 0xD8);
    Actor_FaceDirection(ACTOR_IVAN, 0xA000, 0);
    actor_one_record = Actor_Get(ACTOR_GERALD);
    FIELD(actor_one_record, u8 *, 0x5A) = (u8)(0xFE & FIELD(actor_one_record, u8 *, 0x5A));
    Actor_WalkTo(ACTOR_GERALD, 0x138, 0xC8);
    Actor_WalkTo(ACTOR_MIA, 0x118, 0xD8);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Actor_WaitForMove(ACTOR_MIA);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_MIA, 0, 20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Event_Wait(60);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, 0xE000, 20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 30);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xA000, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 40);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x138, 0xD8);
    Actor_WalkTo(ACTOR_GERALD, 0x138, 0xD8);
    Actor_WalkTo(ACTOR_IVAN, 0x138, 0xD8);
    Actor_WalkTo(ACTOR_MIA, 0x138, 0xD8);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_WaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    {
        void **record;
        record = (void **)(gCam + 0x164);
        FIELD(record, s32 *, 0xC) = 0x04000000;
    }
    Map_Redraw();
    Task_Wait(1);
    Map_CopyCellsTo(4, 0x46, 4, 0x4A, 5, 4);
    GameFlag_Set(0x880);
    GameFlag_Set(0x00000881);
    Event_End();
}

void FieldScene_RunScene39d_02002ddc(void)
{
    s32 record;
    u8 *work;

    work = gCam + 0x164;
    Event_Begin();
    *(s32 *)(work + 12) = 0x3800000;
    Map_Redraw();
    Task_Wait(1);
    *(u8 *)(((s32)Object_GetById(9)) + 85) = 0;
    Actor_SetPosition(9, PIXELS(0x68), PIXELS(0x108));
    record = ((s32)Object_GetById(9));
    *(s32 *)(record + 12) = -0x200000;
    record = Actor_Get(9);
    *(s32 *)(record + 60) = -0x200000;
    *(u8 *)(Battle_GetWorkObject1e0Far() + 85) = 0;
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(PIXELS(0x80), -1, PIXELS(0xB8), 1);
    Camera_WaitForMove();
    Event_Wait(30);
    Map_CopyCellsTo(29, 74, 4, 74, 5, 4);
    Actor_SetSpritePriority(17, 0);
    Actor_SetSpritePriority(18, 0);
    MakyuriChojo_RaiseCollectedActors();
    Actor_SetSpritePriority(17, 1);
    Actor_SetSpritePriority(18, 1);
    Event_Wait(20);
    GameFlag_Set(0x251);
    Event_End();
}

void FieldScene_RunScene39d_02002eb8(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 104, 152);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 60);
    Actor_SetSpritePriority(17, 0);
    Actor_SetSpritePriority(18, 0);
    MakyuriChojo_LowerCollectedActors();
    Camera_MoveTo(-1, -1, -1, 0);
    Event_RequestExit(1);
    Event_End();
}

/* Actors 22 and 24 sink away, throwing sparks in turn for 32 frames. */
void MakyuriChojo_SinkActorPair(void)
{
    struct EffectParams params;
    struct EffectParams *p;
    struct Actor *left;
    struct Actor *right;
    u32 i;
    s32 x;
    s32 y;
    s32 r;

    left = (struct Actor *)Object_GetById(22);
    right = (struct Actor *)Object_GetById(24);
    Engine_AudioPlayCue(190);
    Call2(Engine_ActorSetChildValue, 22, 0x100);
    Engine_ActorSetChildValue(24, 0x100);
    SetOverlayObjectMode((struct Actor *)Object_GetById(22), 0);
    SetOverlayObjectMode((struct Actor *)Object_GetById(24), 0);
    p = &params;
    p->count = 1;
    p->kind = 5;
    p->tile = 0x11c;
    p->spread = 0x6666;
    p->rise = 0x30000;
    for (i = 0; i <= 31; i++) {
        Engine_EventWait(1);
        if (i & 1) {
            r = ((((u32)Engine_RandomNext() * 24) >> 16) << 16);
            x = left->x;
            x += r;
            x += -0xc0000;
            Effect_Spawn(x, left->y + ((((u32)Engine_RandomNext() << 5) >> 16) << 16) + -0x100000, left->z, 0, 0x40000, 0, 0x1b0000, p);
        } else {
            r = ((((u32)Engine_RandomNext() * 24) >> 16) << 16);
            x = right->x;
            x += r;
            x += -0xc0000;
            Effect_Spawn(x, right->y + ((((u32)Engine_RandomNext() << 5) >> 16) << 16) + -0x100000, right->z, 0, 0x40000, 0, 0x1b0000, p);
        }
    }
    Engine_ActorSetPosition(22, 0, 0);
    Engine_ActorSetPosition(24, 0, 0);
}

/* Actors 22 and 24 rise back, throwing sparks in turn for 32 frames. */
void MakyuriChojo_RiseActorPair(void)
{
    struct EffectParams params;
    struct EffectParams *p;
    struct Actor *left;
    struct Actor *right;
    u32 i;
    s32 x;
    s32 y;
    s32 r;

    left = (struct Actor *)Object_GetById(22);
    right = (struct Actor *)Object_GetById(24);
    Engine_AudioPlayCue(190);
    SetOverlayObjectMode((struct Actor *)Object_GetById(22), 0);
    SetOverlayObjectMode((struct Actor *)Object_GetById(24), 0);
    p = &params;
    p->count = 1;
    p->kind = 5;
    p->tile = 0x11c;
    p->spread = 0x6666;
    p->rise = 0x30000;
    for (i = 0; i <= 31; i++) {
        Engine_EventWait(1);
        if (i & 1) {
            r = ((((u32)Engine_RandomNext() * 24) >> 16) << 16);
            x = left->x;
            x += r;
            x += -0xc0000;
            Effect_Spawn(x, left->y + ((((u32)Engine_RandomNext() << 5) >> 16) << 16) + 0x200000, left->z, 0, -0x40000, 0, 0x1b0000, p);
        } else {
            r = ((((u32)Engine_RandomNext() * 24) >> 16) << 16);
            x = right->x;
            x += r;
            x += -0xc0000;
            Effect_Spawn(x, right->y + ((((u32)Engine_RandomNext() << 5) >> 16) << 16) + 0x200000, right->z, 0, -0x40000, 0, 0x1b0000, p);
        }
        if (i == 20) {
            Call2(Engine_ActorSetChildValue, 22, 0x100);
            Engine_ActorSetChildValue(24, 0x100);
        }
    }
    Engine_ActorSetChildValue(22, 0);
    Engine_ActorSetChildValue(24, 0);
    SetOverlayObjectMode((struct Actor *)Object_GetById(22), 1);
    SetOverlayObjectMode((struct Actor *)Object_GetById(24), 1);
}

/* Append to out the slot of every record inside the band below the caller's
 * y: x cells 4 to 8, and rows 8 to 10 below 64 - y. */
void MakyuriChojo_CollectBandSlots(s32 *out, s32 y)
{
    struct RecList_39d *list;
    u32 slot;

    list = (struct RecList_39d *)gEventWork;
    y = 64 - (y >> 20);
    for (slot = 0; slot <= 65; slot++) {
        struct Rec_39d *rec = list->recs[slot];

        if (rec != 0) {
            s32 cell = (rec->x >> 20) - 4;
            s32 row = rec->y >> 20;

            if ((u32)cell <= 4 && y + 8 <= row && row < y + 11)
                *out++ = slot;
        }
    }
}

void MakyuriChojo_LowerCollectedActors(void)
{
    s32 ids[5];
    u8 *work;
    u32 i;
    u32 j;
    u32 n;
    s32 speed;
    u8 *actor;

    work = *(u8 **)&gMapWork + 0x164;
    speed = 0x1999;
    n = 0;
    for (i = 0; i <= 4; i++)
        ids[i] = 66;
    ((s32 (*)())MakyuriChojo_CollectBandSlots)(ids, *(s32 *)(work + 12), work);
    for (i = 0; i <= 4; i++) {
        if (ids[i] == 66)
            break;
        ((struct Flags85 *)Object_GetById(ids[i]))->flags = 0;
        n++;
    }
    Engine_AudioPlayCue(223);
    for (i = 0; i <= 227; i++) {
        *(s32 *)(work + 12) -= speed;
        for (j = 0; j < n; j++) {
            *(s32 *)((u8 *)Object_GetById(ids[j]) + 16) += speed;
            actor = (u8 *)Object_GetById(ids[j]);
            *(s32 *)(actor + 64) = *(s32 *)((u8 *)Object_GetById(ids[j]) + 16);
        }
        if ((i & 3) == 3)
            speed += 0x1999;
        if (speed > 0x17fff)
            speed = 0x18000;
        Engine_TaskWait(1);
    }
    for (i = 0; i < n; i++)
        ((struct Flags85 *)Object_GetById(ids[i]))->flags = 0;
}

/* Slide the work position up while lowering the collected actors, easing
 * the speed off over the last ten frames, then redraw the map. */
void MakyuriChojo_RaiseCollectedActors(void)
{
    s32 ids[5];
    u8 *work;
    u32 i;
    u32 j;
    u32 n;
    s32 speed;
    u8 *actor;

    work = *(u8 **)&gMapWork + 0x164;
    speed = 0x18000;
    n = 0;
    for (i = 0; i <= 4; i++)
        ids[i] = 66;
    MakyuriChojo_CollectBandSlots(ids, *(s32 *)(work + 12));
    for (i = 0; i <= 4; i++) {
        if (ids[i] == 66)
            break;
        ((struct Flags85 *)Object_GetById(ids[i]))->flags = 0;
        n++;
    }
    Engine_AudioPlayCue(223);
    for (i = 0; i <= 85; i++) {
        *(s32 *)(work + 12) += speed;
        for (j = 0; j < n; j++) {
            *(s32 *)((u8 *)Object_GetById(ids[j]) + 16) -= speed;
            actor = (u8 *)Object_GetById(ids[j]);
            *(s32 *)(actor + 64) = *(s32 *)((u8 *)Object_GetById(ids[j]) + 16);
        }
        if ((i & 3) == 3 && i > 75)
            speed += -0x3333;
        if (speed < 0xccc)
            speed = 0xccc;
        Engine_TaskWait(1);
    }
    *(s32 *)(work + 12) = 0x4000000;
    Engine_MapRedraw();
    Engine_TaskWait(2);
}

void FieldScene_ConfigureValue93Scene(void)
{
    Psynergy_Begin(93, 1);
    Psynergy_SetTarget(24, 9);
    Psynergy_RaiseHands();
    Psynergy_PlayEffect(1);
    Psynergy_LowerHands();
    BattleEffect_CleanupSceneObjectsFar();
}

void SceneEffect_UpdateArcPosition(struct OverlayObject *object)
{
    struct OverlayObject *parent;
    u16 angle;

    parent = object->linked_object;
    angle = object->angle_64;
    object->coordinate_08 = parent->coordinate_08 + Math_Cos(angle) * (object->field_30 + 28);
    object->coordinate_10 = (Math_Sin(angle) << 4) + 0x900000;
    object->coordinate_38 = object->coordinate_08;
    object->coordinate_40 = object->coordinate_10;
    object->angle_64 -= 0x200;
}
