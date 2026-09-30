#include "FUNE.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "CALL.H"

struct Other {
    u8 filler00[30];
    u16 x;
};

extern s32 **gMapWork;

struct Object {
    u8 filler00[8];
    s32 x;
    s32 z;
    u8 filler10[64];
    struct Other *other;
};

u32 Random16(void);

/* A swaying object: the actor record with its drift speed and the two sway
 * states (0 rising, 1 falling, 9 stopped) in the free words. */
struct SwayActor {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    u8 unknown_10[0x3c];
    s32 drift;
    u8 unknown_50[0x14];
    s16 x_state;
    s16 y_state;
};

void battle_owner_69(void);
void FuneHobashira_UpdateSway(void);
void FieldScene_RunScene3b0_0200040c(void);
void FieldScene_RunScene3b0_02000468(void);
void Scene_RunFourActorStagingSequence(void);
void FieldScene_RunActorNinePresentationCycles(void);
void FieldScene_RunPrimarySequence(void);
void FieldScene_RunSevenActorEnsemble(void);
extern u8 MsgFuneShipsCourseClear[];
extern u8 MsgFuneAvast[];

/* The IWRAM field globals: the map work first, the event work at +0x4c. */
struct FieldGlobals {
    s32 **map;
    u8 unknown_04[0x48];
    struct EventWork *event;
};

extern u8 MsgFuneMonsters2[];
extern u8 MsgFuneLandHo[];

/* Each actor's walk ashore, where the overlay's data lies. */
extern u8 FuneHobashira_EnsembleWalk9[];
extern u8 FuneHobashira_EnsembleWalk10[];
extern u8 FuneHobashira_EnsembleWalk11[];
extern u8 FuneHobashira_EnsembleWalk12[];
extern u8 FuneHobashira_EnsembleWalk13[];
extern u8 FuneHobashira_EnsembleWalk14[];
extern u8 FuneHobashira_EnsembleWalk15[];
void OverlayObject_InitWithRandomFields(s32 object);
s32 SceneData_GetDifferenceOfPairSums(void);

/* The mast's own work, past its image: the drift the sway has added up, its
 * two angles, and the camera centre and origin the staging saves for the
 * camera-offset placement. */
s32 FuneHobashira_DriftY __attribute__((section(".bss")));
s32 FuneHobashira_DriftX __attribute__((section(".bss")));
s32 FuneHobashira_SwayY __attribute__((section(".bss")));
s32 FuneHobashira_SwayUnused __attribute__((section(".bss")));
s32 FuneHobashira_CameraCenter[2] __attribute__((section(".bss")));
s32 FuneHobashira_CameraOrigin[2] __attribute__((section(".bss")));
s32 FuneHobashira_SwayX __attribute__((section(".bss")));

/* Places the object where the camera has moved relative to the staging's
 * saved camera origin, around the saved centre. */
s32 Object_PlaceFromCameraOffset(struct Object *object)
{
    s32 *position = *gMapWork;
    s32 *origin = FuneHobashira_CameraOrigin;
    s32 *center = FuneHobashira_CameraCenter;
    s32 q0 = *position++;
    s32 q1 = *position;

    object->x = center[0] + (q0 - origin[0]);
    /* FAKEMATCH: the do/while keeps the z store ahead of the other load. */
    do {
        object->z = center[1] + (q1 - origin[1]) / 2;
    } while (0);
    object->other->x += 0x600;
    return 0;
}

/*
 * The shift pair is a windowed extraction of bits 10..15 of the sample, giving
 * a value in 0..63; a plain right shift would let larger values reach the
 * comparisons.  The headings 0xd000 and 0xb000 are built from an immediate and
 * a shift, and the local in each arm is what forces that.  The halfword at
 * object + 6 is the facing angle.  What Random16 samples is not
 * established here.
 */
s32 SceneActor_SetFacingFromSample(u8 *object)
{
    u32 sample = (u32)(Random16() << 6) >> 16;   /* bits 10..15 */

    if (sample == 6) {
        s32 value = 0xd000;

        *(u16 *)(object + 6) = value;
    } else if (sample == 9) {
        s32 value = 0xb000;

        *(u16 *)(object + 6) = value;
    }

    return 1;
}

void OverlayObject_Add160ToFields18And1c(u8 *o)
{
    if (*(s32 *)(o + 24) < 0x10000) {
        *(s32 *)(o + 24) += 160;
        *(s32 *)(o + 28) += 160;
    }
}

/* Sways the object back and forth by a random drift within its range and
 * bobs it up and down between heights 0 and one cell. */
s32 FuneHobashira_SwayActor(struct SwayActor *actor)
{
    s16 *state = &actor->x_state;
    s32 next;

    if (*state == 9) {
        actor->drift = 0;
    } else if (*state != 0) {
        actor->drift -= (u32)(Random16() << 11) >> 16;
        if (actor->drift < -0xc000) {
            next = 0;
            *state = next;
        }
    } else {
        actor->drift += (u32)(Random16() << 11) >> 16;
        if (actor->drift > 0xc000) {
            next = 1;
            *state = next;
        }
    }
    if (actor->x > 0x280000 && actor->x < 0x1400000) {
        actor->x += actor->drift;
    }
    state = &actor->y_state;
    if (*state == 9) {
        actor->y = 0;
    } else if (*state != 0) {
        actor->y -= (u32)(Random16() * 3 << 14) >> 16;
        if (actor->y < 0) {
            next = 0;
            *state = next;
        }
    } else {
        actor->y += (u32)(Random16() * 3 << 14) >> 16;
        if (actor->y > 0x100000) {
            next = 1;
            *state = next;
        }
    }
    return 1;
}

s32 OverlayObject_ShrinkScaleThenStop(u8 *o)
{
    u8 *t = *(u8 **)(o + 0x50);

    t[9] |= 12;
    *(s32 *)(o + 48) = 0x20000;
    *(s32 *)(o + 52) = 0x10000;
    if (*(s32 *)(o + 24) > 0x1000) {
        *(s32 *)(o + 24) += 0xFFFFFC00;
        *(s32 *)(o + 28) += 0xFFFFFC00;
    } else {
        *(s32 *)(o + 8) = 0;
        *(s32 *)(o + 12) = 0;
        *(s32 *)(o + 16) = 0;
        *(s32 *)(o + 36) = 0;
        *(s32 *)(o + 40) = 0;
        *(s32 *)(o + 44) = 0;
    }
    return 1;
}

u8 *SceneData_GetTable95c0(void)
{
    return FuneHobashira_SceneTableA;
}

u8 *SceneData_GetTable9680(void)
{
    return FuneHobashira_SceneTableB;
}

u8 *SceneData_GetTable96a0(void)
{
    return FuneHobashira_SceneTableC;
}

u8 *SceneData_GetTable96C4(void)
{
    return FuneHobashira_SceneTableD;
}

u8 *SceneData_GetTable988c(void)
{
    return FuneHobashira_SceneTableE;
}

void FieldScene_CallHelper14d0(void)
{
    FieldEffect_UpdateGridPlacement();
}

void FieldScene_RunActor232SceneWhenFlag923Or922(void)
{
    if (GameFlag_IsSet(FLAG_MAST_923) != 0 || GameFlag_IsSet(FLAG_MAST_922) != 0) {
        Event_Begin();
        Item_ShowFound(ITEM_ANCHOR_CHARM, 3);
        Party_GiveItem(ITEM_ANCHOR_CHARM, 0);
        GameFlag_Set(FLAG_MAST_924);
        Event_End();
    }
}

s32 FuneHobashira_ApplyEntryState(void)
{
    struct FieldActor *leader;

    Engine_GameFlagSet(0x144);
    gEventWork->start_transition = 0x209;
    if ((Engine_GameFlagIsSet(0x927) != 0 || Engine_GameFlagIsSet(0x928) != 0)
        && Engine_GameFlagIsSet(0x93e) == 0 && Engine_GameFlagIsSet(0x8a0) == 0) {
        FuneHobashira_SwayX = (u16)Random16();
        FuneHobashira_SwayY = (u16)Random16();
        Engine_TaskAddCallback(FuneHobashira_UpdateSway, 0xc80);
    }
    if (Engine_GameFlagIsSet(0x925) != 0 && Engine_GameFlagIsSet(0x93e) == 0) {
        Actor_SetPosition(8, 0xa40000, 0x1480000);
    }
    switch (gGameState.entrance) {
    case 1:
        if (Engine_GameFlagIsSet(0x109) == 0) {
            leader = Engine_ActorGet(ACTOR_PARTY_LEADER);
            Engine_EventBegin();
            battle_owner_69();
            leader->y.fixed = 0x380000;
            Camera_MoveTo(-1, -1, -1, 0);
            Engine_TaskWait(1);
            Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
            Engine_MapRedraw();
            Engine_TaskWait(1);
            Engine_EventEnd();
        }
        break;
    case 10:
        if (Engine_GameFlagIsSet(0x928) != 0) {
            FieldScene_RunScene3b0_02000468();
        } else {
            FieldScene_RunScene3b0_0200040c();
        }
        break;
    case 11:
        gGameState.saved_scene = (s32)&SceneId_FuneHeya;
        gGameState.saved_entrance = 30;
        Scene_RunFourActorStagingSequence();
        break;
    case 12:
        gGameState.saved_scene = (s32)&SceneId_FuneHeya;
        gGameState.saved_entrance = 30;
        FieldScene_RunActorNinePresentationCycles();
        break;
    case 13:
        gGameState.saved_scene = (s32)&SceneId_FuneHeya;
        gGameState.saved_entrance = 30;
        FieldScene_RunPrimarySequence();
        break;
    case 14:
        FieldScene_RunSevenActorEnsemble();
        break;
    }
    return 0;
}

void FieldScene_RunScene3b0_0200040c(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    *(u8 *)(Battle_GetWorkObject1e0() + 85) = 0;
    Call3(Engine_CameraMoveTo, 0xa40000, 0x400000, 0x1410000);
    Map_Redraw();
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    FieldScene_RunScene3b0_020004b0();
    Event_End();
}

void FieldScene_RunScene3b0_02000468(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xa40000, 0x1410000);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    record = (u8 *)Engine_ActorGet(0);
    Actor_SetSpriteFlags(record, 0);
    Task_Wait(1);
    Map_Redraw();
    Task_Wait(1);
    FieldScene_RunScene3b0_020004b0();
    Event_End();
}

void FieldScene_RunScene3b0_020004b0(void)
{
    gEventWork->start_transition = 0x202;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_SetSpeed(8, 0x10000, 0x8000);
    Actor_WalkToAndWait(8, 164, 0x141);
    Actor_FaceDirection(8, 0xd000, 40);
    Actor_FaceDirection(8, 0xb000, 40);
    Actor_FaceDirection(8, 0xd000, 40);
    Actor_FaceDirection(8, 0x3000, 10);
    Actor_WalkToAndWait(8, 164, 0x14e);
    Actor_Jump(8, 4, 40);
    Actor_StartRepeatedMotion(8, 2);
    Event_SetMessage((s32)MsgFuneShipsCourseClear);
    Event_ShowMessageAndWait(8, 0, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(10);
}

/* Runs the record-8/record-9 pair through two near-identical setup-then-move
 * cycles (position waypoints, a movement flag reset, then animation/sound
 * calls), followed by a shorter closing cycle for record 8 alone. */


/* Places the staging objects and starts the lookout's actions, saves the
 * camera position as the origin actor 9's camera-following actions measure
 * from, grows actors 10 to 12 in and walks them to their places, then the
 * lookout walks and leaps before the scene exits. */
void Scene_RunFourActorStagingSequence(void)
{
    s32 *placement = *(*(struct FieldGlobals *)&gMapWork).map;
    struct FieldActor *actor;
    struct EventWork **event;
    s32 x;
    s32 zero = 0;
    s32 scale;
    s32 update;

    Engine_EventBegin();
    Event_CallWithLastActiveObjectId((s32)FuneHobashira_StagingObjects);
    Engine_TaskWait(1);
    Engine_ActorSetChildValue(0, 15);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(0), 0);
    Engine_ActorEnableActionCallback(8, FuneHobashira_LookoutActions);
    event = &(*(struct FieldGlobals *)&gMapWork).event;
    (*event)->start_transition = 0x203;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    FuneHobashira_CameraOrigin[0] = *placement++;
    FuneHobashira_CameraOrigin[1] = *placement;
    Call3((void (*)())Engine_ActorSetPosition, 9, 0x500000, 0xd20000);
    x = 0x500000;
    Engine_ActorGet(9)->motion_flags = zero;
    FuneHobashira_CameraCenter[0] = x;
    FuneHobashira_CameraCenter[1] = zero;
    Engine_ActorEnableActionCallback(9, FuneHobashira_FollowCameraActions);
    Engine_EventWait(20);
    Engine_AudioPlayCue(29);
    Engine_GameFlagSet(0x8f0);
    Engine_ActorStop(8);
    Engine_TaskWait(1);
    Call3((void (*)())Engine_ActorShowEmote, 8, 0x100, 0);
    Engine_ActorFaceDirection(8, 0xb000, 0);
    Engine_EventSetMessage((s32)MsgFuneAvast);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Call3((void (*)())Engine_ActorSetPosition, 10, x, 0xd20000);
    Call3((void (*)())Engine_ActorSetPosition, 11, x, 0xd20000);
    ((void (*)())Engine_ActorSetPosition)(12, x, 0xd20000);
    Engine_ActorSetSpritePriority(10, 3);
    Engine_ActorSetSpritePriority(11, 3);
    Engine_ActorSetSpritePriority(12, 3);
    Engine_ActorSetChildValue(10, 3);
    Engine_ActorSetChildValue(11, 3);
    Engine_ActorSetChildValue(12, 3);
    actor = Engine_ActorGet(10);
    scale = 0x8000;
    update = (s32)OverlayObject_Add160ToFields18And1c;
    actor->scale_y = scale;
    actor->scale_x = scale;
    actor->update = (void (*)(union FieldObject *))update;
    actor = Engine_ActorGet(11);
    actor->scale_y = scale;
    actor->scale_x = scale;
    actor->update = (void (*)(union FieldObject *))update;
    actor = Engine_ActorGet(12);
    actor->scale_y = scale;
    actor->scale_x = scale;
    actor->update = (void (*)(union FieldObject *))update;
    Engine_TaskWait(1);
    Call3((void (*)())Engine_ActorSetSpeed, 10, 0x851e, 0x428f);
    Call3((void (*)())Engine_ActorSetSpeed, 11, 0x7333, 0x3999);
    Call3((void (*)())Engine_ActorSetSpeed, 12, 0x9999, 0x4ccc);
    Call3((void (*)())Engine_ActorSetDestination, 10, 128, 345);
    Call3((void (*)())Engine_ActorSetDestination, 11, 136, 330);
    ((void (*)())Engine_ActorSetDestination)(12, 156, 340);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(8, 2);
    Call3((void (*)())Engine_ActorWalkToAndWait, 8, 164, 344);
    Engine_ActorJump(8, 4, 10);
    Engine_ActorJump(8, 6, 40);
    Engine_ActorStartRepeatedMotion(8, 3);
    Engine_EventShowMessageAndWait(8, 0, 20);
    (*event)->start_transition = 0x202;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(11);
    Engine_EventEnd();
}

void FieldScene_RunActorNinePresentationCycles(void)
{
    u32 i;
    u8 *rec9;
    u8 *record;

    Event_Begin();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    record = (u8 *)Engine_ActorGet(0);
    Actor_SetSpriteFlags(record, 0);
    Task_Wait(1);
    ((void (*)())Event_CallWithLastActiveObjectId)((s32)FuneHobashira_EnsembleObjects);
    Task_Wait(1);
    OverlayObject_InitWithRandomFields(9);
    OverlayObject_InitWithRandomFields(10);
    OverlayObject_InitWithRandomFields(11);
    OverlayObject_InitWithRandomFields(12);
    OverlayObject_InitWithRandomFields(13);
    OverlayObject_InitWithRandomFields(14);
    OverlayObject_InitWithRandomFields(15);
    Engine_ActorEnableActionCallback(8, FuneHobashira_LookoutActions);
    gEventWork->start_transition = 0x203;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(120);
    rec9 = (u8 *)Engine_ActorGet(9);
    Actor_Stop(9);
    /* Reset record 9's waypoint/velocity fields: three fields to the
     * minimum s32, then four fields to zero. */
    RECORD_S32(rec9, 56) = -0x80000000;
    RECORD_S32(rec9, 60) = -0x80000000;
    RECORD_S32(rec9, 64) = -0x80000000;
    RECORD_S32(rec9, 36) = 0;
    RECORD_S32(rec9, 40) = 0;
    RECORD_S32(rec9, 44) = 0;
    RECORD_S32(rec9, 76) = 0;
    Event_Wait(20);
    Actor_SetSpeed(9, 0x80000, 0x40000);
    Call4(Object_SetPosition, rec9, 0xa40000, 0x900000, 0x1410000);
    Object_CommitPosition(rec9);
    Call4(Object_SetPosition, rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition(rec9);
    Call4(Object_SetPosition, rec9, 0xcc0000, 0x7c0000, 0x1410000);
    Object_CommitPosition(rec9);
    Call4(Object_SetPosition, rec9, 0x900000, 0, 0xa90000);
    Actor_Stop(8);
    Task_Wait(1);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_ShowEmote(8, 0x103, 60);
    Actor_SetSpeed(9, 0x20000, 0x10000);
    OverlayObject_InitWithRandomFields(9);
    Event_Wait(20);
    Engine_ActorEnableActionCallback(8, FuneHobashira_LookoutActions);
    Event_Wait(120);
    Actor_Stop(9);
    /* Same reset pattern on record 9 for the second cycle. */
    RECORD_S32(rec9, 56) = -0x80000000;
    RECORD_S32(rec9, 60) = -0x80000000;
    RECORD_S32(rec9, 64) = -0x80000000;
    RECORD_S32(rec9, 36) = 0;
    RECORD_S32(rec9, 40) = 0;
    RECORD_S32(rec9, 44) = 0;
    RECORD_S32(rec9, 76) = 0;
    Event_Wait(20);
    Actor_SetSpeed(9, 0x80000, 0x40000);
    Call4(Object_SetPosition, rec9, 0xa40000, 0x900000, 0x1410000);
    Object_CommitPosition(rec9);
    Actor_SetSpeed(9, 0x50000, 0x28000);
    Call4(Object_SetPosition, rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition(rec9);
    Call4(Object_SetPosition, rec9, 0xa40000, 0x720000, 0x1410000);
    Object_CommitPosition(rec9);
    Call4(Object_SetPosition, rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition(rec9);
    Call4(Object_SetPosition, rec9, 0xcc0000, 0x7c0000, 0x1410000);
    Object_CommitPosition(rec9);
    Object_SetPosition(rec9, 0x900000, 0, 0xa90000);
    Actor_Stop(8);
    Task_Wait(1);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_ShowEmote(8, 0x103, 60);
    Actor_SetSpeed(9, 0x20000, 0x10000);
    OverlayObject_InitWithRandomFields(9);
    Actor_Jump(8, 4, 20);
    Actor_Jump(8, 6, 40);
    Audio_PlayCue(29);
    GameFlag_Set(0x8f0);
    Event_SetMessage((s32)MsgFuneMonsters2);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_ShowEmote(8, 0x100, 0);
    Actor_WalkToAndWait(8, 164, 0x158);
    Event_Wait(40);
    Actor_RunRepeatedMotion(8, 2);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(12);
    Event_End();
}

void OverlayObject_InitWithRandomFields(s32 a)
{
    u8 *obj;
    u32 x;

    obj = (u8 *)Engine_ActorGet(a);
    Actor_SetSpritePriority(a, 1);
    obj[0x55] = 0;
    *(u16 *)(obj + 0x64) = Random16() >> 15;
    *(u16 *)(obj + 0x66) = Random16() >> 15;
    x = Random16();
    x <<= 2;
    x >>= 16;
    x <<= 16;
    x += 0x60000;
    *(s32 *)(obj + 0xc) = x;
    x = Random16();
    *(s32 *)(obj + 0x4c) = ((x * 3 << 13) >> 16) - 0x3000;
    *(s32 *)(obj + 0x18) = 0x14000;
    *(s32 *)(obj + 0x1c) = 0x14000;
    Actor_EnableActionCallback(a, FuneHobashira_DriftActions);
}

/* Drives ids 8 through 18 through position, scale, and flag updates in
 * sequence, stepping the shared scene phase at the start and end. */
void FieldScene_RunPrimarySequence(void)
{
    u32 i;
    s32 rec;
    s32 id0_state;

    Event_Begin();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    id0_state = (u8 *)Engine_ActorGet(0);
    Actor_SetSpriteFlags(id0_state, 0);
    Task_Wait(1);
    ((void (*)())Event_CallWithLastActiveObjectId)((s32)FuneHobashira_EnsembleObjects);
    Task_Wait(1);
    ((void (*)())Event_CallWithLastActiveObjectId)((s32)FuneHobashira_LandingObjects);
    Task_Wait(1);
    OverlayObject_InitWithRandomFields(9);
    OverlayObject_InitWithRandomFields(10);
    OverlayObject_InitWithRandomFields(11);
    OverlayObject_InitWithRandomFields(12);
    OverlayObject_InitWithRandomFields(13);
    OverlayObject_InitWithRandomFields(14);
    OverlayObject_InitWithRandomFields(15);
    Actor_EnableActionCallback(8, FuneHobashira_LookoutActions);
    gEventWork->start_transition = 0x203;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(0x12c);
    Audio_PlayCue(147);
    Event_Wait(100);
    Actor_Stop(9);
    Actor_Stop(10);
    Actor_Stop(11);
    Actor_Stop(12);
    Actor_Stop(13);
    Actor_Stop(14);
    Actor_Stop(15);
    Actor_SetSpeed(9, 0x30000, 0x18000);
    Actor_SetSpeed(10, 0x30000, 0x18000);
    Actor_SetSpeed(11, 0x30000, 0x18000);
    Actor_SetSpeed(12, 0x30000, 0x18000);
    Actor_SetSpeed(13, 0x30000, 0x18000);
    Actor_SetSpeed(14, 0x30000, 0x18000);
    Actor_SetSpeed(15, 0x30000, 0x18000);
    Actor_SetDestination(9, 0, 100);
    Actor_SetDestination(10, 60, 100);
    Actor_SetDestination(11, 120, 100);
    Actor_SetDestination(12, 180, 100);
    Actor_SetDestination(13, 240, 100);
    Actor_SetDestination(14, 0x140, 100);
    Actor_SetDestination(15, 0x17c, 100);
    Event_Wait(40);
    Actor_ShowEmote(8, 0x101, 0);
    Event_Wait(20);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(11, 0, 0);
    Actor_SetPosition(12, 0, 0);
    Actor_SetPosition(13, 0, 0);
    Actor_SetPosition(14, 0, 0);
    Actor_SetPosition(15, 0, 0);
    Event_Wait(100);
    rec = (u8 *)Engine_ActorGet(18);
    *(s32 *)(rec + 24) = 0x1999;
    *(s32 *)(rec + 28) = 0x1999;
    Actor_SetPosition(18, 0xac0000, 0x1540000);
    Actor_Stop(8);
    Task_Wait(1);
    Actor_RunRepeatedMotion(8, 1);
    Actor_FaceDirection(8, 0x3000, 0);
    Audio_PlayCue(29);
    GameFlag_Set(0x8f0);
    for (i = 0; i < 32; i++) {
        *(s32 *)(rec + 24) += 0xccc;
        *(s32 *)(rec + 28) += 0xccc;
        Task_Wait(1);
    }
    Actor_ShowEmote(8, 0x101, 60);
    Actor_RunRepeatedMotion(8, 2);
    Actor_WalkToAndWait(8, 168, 0x154);
    Actor_WalkToAndWait(8, 200, 0x154);
    Actor_FaceDirection(8, 0x8000, 0);
    rec = (u8 *)Engine_ActorGet(17);
    *(s32 *)(rec + 24) = 0x12666;
    *(s32 *)(rec + 28) = 0x12666;
    *(s32 *)(rec + 8) = 0xac0000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x1540000;
    {
        /* Clear the flag word at +6. */
        s32 shown = 0;

        *(u16 *)(rec + 6) = shown;
    }
    *(s32 *)(rec + 68) = 0x6666;
    *(s32 *)(rec + 72) = 0x30000;
    Event_Wait(20);
    Actor_Jump(8, 6, 20);
    Audio_PlayCue(147);
    Event_Wait(20);
    Actor_EnableActionCallback(8, FuneHobashira_WaveActions);
    Event_Wait(80);
    Actor_SetSpritePriority(17, 1);
    Actor_SetSpeed(17, 0x10000, 0x8000);
    *(s32 *)(rec + 68) = 0x1999;
    *(s32 *)(rec + 72) = 0xb333;
    Audio_PlayCue(153);
    *(s32 *)(rec + 40) = 0x80000;
    Actor_SetDestination(17, 132, 0x168);
    Actor_SetDestination(18, 132, 0x168);
    Event_Wait(40);
    Actor_SetPosition(17, 0, 0);
    rec = (u8 *)Engine_ActorGet(8);
    *(s32 *)(rec + 24) = 0x10000;
    *(s32 *)(rec + 28) = 0x10000;
    {
        /* Set the flag word at +6. */
        s32 shown = 0x5000;

        *(u16 *)(rec + 6) = shown;
    }
    Event_Wait(40);
    gEventWork->start_transition = 0x202;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(13);
    Event_End();
}

void FieldScene_RunSevenActorEnsemble(void)
{
    Event_Begin();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Actor_SetSpriteFlags(Engine_ActorGet(ACTOR_PARTY_LEADER), 0);
    Event_CallWithLastActiveObjectId((s32)FuneHobashira_EnsembleObjects);
    Task_Wait(1);
    OverlayObject_InitWithRandomFields(9);
    OverlayObject_InitWithRandomFields(10);
    OverlayObject_InitWithRandomFields(11);
    OverlayObject_InitWithRandomFields(12);
    OverlayObject_InitWithRandomFields(13);
    OverlayObject_InitWithRandomFields(14);
    OverlayObject_InitWithRandomFields(15);
    Actor_EnableActionCallback(8, FuneHobashira_LookoutActions);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(400);
    Actor_Stop(9);
    Actor_Stop(10);
    Actor_Stop(11);
    Actor_Stop(12);
    Actor_Stop(13);
    Actor_Stop(14);
    Actor_Stop(15);
    Actor_SetSpeed(9, 0x30000, 0x18000);
    Actor_SetSpeed(10, 0x30000, 0x18000);
    Actor_SetSpeed(11, 0x30000, 0x18000);
    Actor_SetSpeed(12, 0x30000, 0x18000);
    Actor_SetSpeed(13, 0x30000, 0x18000);
    Actor_SetSpeed(14, 0x30000, 0x18000);
    Actor_SetSpeed(15, 0x30000, 0x18000);
    Actor_EnableActionCallback(9, FuneHobashira_EnsembleWalk9);
    Actor_EnableActionCallback(10, FuneHobashira_EnsembleWalk10);
    Actor_EnableActionCallback(11, FuneHobashira_EnsembleWalk11);
    Actor_EnableActionCallback(12, FuneHobashira_EnsembleWalk12);
    Actor_EnableActionCallback(13, FuneHobashira_EnsembleWalk13);
    Actor_EnableActionCallback(14, FuneHobashira_EnsembleWalk14);
    Actor_EnableActionCallback(15, FuneHobashira_EnsembleWalk15);
    Event_Wait(40);
    Actor_StartRepeatedMotion(8, 3);
    Actor_SetAttachedEffect(8, 258);
    Event_Wait(120);
    Actor_StartRepeatedMotion(8, 1);
    Actor_ShowEmote(8, 256, 60);
    Actor_SetSpeed(8, 0x10000, 0x8000);
    Actor_WalkToAndWait(8, 164, 344);
    Actor_Jump(8, 4, 10);
    Actor_Jump(8, 6, 20);
    Event_SetMessage((s32)MsgFuneLandHo);
    Event_ShowMessageAndWait(8, 0, 20);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Event_CloseScreen();
    Event_WaitForScreen();
    gGameState.saved_scene = (s32)&SceneId_FuneHeya;
    gGameState.saved_entrance = 2;
    if (SceneData_GetDifferenceOfPairSums() == 11) {
        Event_RequestExit(15);
    } else {
        Event_RequestExit(14);
    }
    Event_End();
}

/* Sway: add cos(SwayX) and 4 * sin(SwayY) to the point the map work's first
 * word names and to the drift totals, then advance both angles by small
 * random steps, kept to 16 bits. */
void FuneHobashira_UpdateSway(void)
{
    s32 *pos = *gMapWork;
    s32 dx = Engine_MathCos(FuneHobashira_SwayX);
    s32 dy = Engine_MathSin(FuneHobashira_SwayY);

    *pos++ += dx;
    dy <<= 2;
    *pos += dy;
    FuneHobashira_DriftX += dx;
    FuneHobashira_DriftY += dy;
    FuneHobashira_SwayX += (u32)(Random16() * 3 << 7) >> 16;
    FuneHobashira_SwayY += (u32)(Random16() << 9) >> 16;
    FuneHobashira_SwayX &= 0xffff;
    FuneHobashira_SwayY &= 0xffff;
}

s32 SceneData_GetDifferenceOfPairSums(void)
{
    s32 a;
    s32 b;

    a = SceneData_GetValueByFirstSetFlag(0);
    a += SceneData_GetValueByFirstSetFlag(2);
    b = SceneData_GetValueByFirstSetFlag(1);
    b += SceneData_GetValueByFirstSetFlag(3);
    return a - b;
}

s32 SceneData_GetValueByFirstSetFlag(u32 a)
{
    s32 base = 0;
    u32 i;

    switch (a) {
    case 0:
        base = 0x92c;
        break;
    case 1:
        base = 0x935;
        break;
    case 2:
        base = 0x917;
        break;
    case 3:
        base = 0x990;
        break;
    }
    for (i = 0; i <= 8; i++) {
        if (GameFlag_IsSet(base + i) != 0)
            return FuneHobashira_FlagValues[i];
    }
    return 0;
}
