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
        Engine_EventBegin();
        Engine_ItemShowFound(ITEM_ANCHOR_CHARM, 3);
        Engine_PartyGiveItem(ITEM_ANCHOR_CHARM, 0);
        GameFlag_Set(FLAG_MAST_924);
        Engine_EventEnd();
    }
}

/* The mast on arrival: record the visit, start the deck's sway while the
 * voyage flags allow it, stand actor 8 by the rail, then run the scene the
 * entrance names. */
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
            leader = Object_GetById(ACTOR_PARTY_LEADER);
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
    register s32 x asm("r0"); /* FAKEMATCH: preserve camera argument setup order. */
    register s32 y asm("r1"); /* FAKEMATCH: preserve camera argument setup order. */
    register s32 pan asm("r3"); /* FAKEMATCH: inherit camera pan from the byte store. */

    /* FAKEMATCH: r3 carries the preceding zero byte store into the
       camera's fourth argument. Capturing it reorders the x/y setup;
       these two moves preserve that setup with all four arguments present. */

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    *(u8 *)(Battle_GetWorkObject1e0() + 85) = 0;
    /* FAKEMATCH: keep y before x while capturing the store's zero in pan. */
    asm ("mov %0, #128\n\tmov %1, #164"
         : "=r" (y), "=r" (x), "=r" (pan));
    Call4(Engine_CameraMoveTo, x << 16, y << 15, 0x1410000, pan);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    FieldScene_RunScene3b0_020004b0();
    Engine_EventEnd();
}

void FieldScene_RunScene3b0_02000468(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xa40000, 0x1410000);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    record = (u8 *)Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_TaskWait(1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    FieldScene_RunScene3b0_020004b0();
    Engine_EventEnd();
}

void FieldScene_RunScene3b0_020004b0(void)
{
    gEventWork->start_transition = 0x202;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Actor_SetSpeed(8, 0x10000, 0x8000);
    Actor_WalkToAndWait(8, 164, 0x141);
    Actor_FaceDirection(8, 0xd000, 40);
    Actor_FaceDirection(8, 0xb000, 40);
    Actor_FaceDirection(8, 0xd000, 40);
    Actor_FaceDirection(8, 0x3000, 10);
    Actor_WalkToAndWait(8, 164, 0x14e);
    Engine_ActorJump(8, 4, 40);
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_EventSetMessage((s32)MsgFuneShipsCourseClear);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(10);
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
    Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
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
    Object_GetById(9)->motion_flags = zero;
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
    Engine_ActorSetPosition(12, x, 0xd20000);
    Engine_ActorSetSpritePriority(10, 3);
    Engine_ActorSetSpritePriority(11, 3);
    Engine_ActorSetSpritePriority(12, 3);
    Engine_ActorSetChildValue(10, 3);
    Engine_ActorSetChildValue(11, 3);
    Engine_ActorSetChildValue(12, 3);
    actor = Object_GetById(10);
    scale = 0x8000;
    update = (s32)OverlayObject_Add160ToFields18And1c;
    actor->scale_y = scale;
    actor->scale_x = scale;
    actor->update = (void (*)(union FieldObject *))update;
    actor = Object_GetById(11);
    actor->scale_y = scale;
    actor->scale_x = scale;
    actor->update = (void (*)(union FieldObject *))update;
    actor = Object_GetById(12);
    actor->scale_y = scale;
    actor->scale_x = scale;
    actor->update = (void (*)(union FieldObject *))update;
    Engine_TaskWait(1);
    Call3((void (*)())Engine_ActorSetSpeed, 10, 0x851e, 0x428f);
    Call3((void (*)())Engine_ActorSetSpeed, 11, 0x7333, 0x3999);
    Call3((void (*)())Engine_ActorSetSpeed, 12, 0x9999, 0x4ccc);
    Call3((void (*)())Engine_ActorSetDestination, 10, 128, 345);
    Call3((void (*)())Engine_ActorSetDestination, 11, 136, 330);
    Engine_ActorSetDestination(12, 156, 340);
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

    Engine_EventBegin();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    record = (u8 *)Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_TaskWait(1);
    Event_CallWithLastActiveObjectId((s32)FuneHobashira_EnsembleObjects);
    Engine_TaskWait(1);
    OverlayObject_InitWithRandomFields(9);
    OverlayObject_InitWithRandomFields(10);
    OverlayObject_InitWithRandomFields(11);
    OverlayObject_InitWithRandomFields(12);
    OverlayObject_InitWithRandomFields(13);
    OverlayObject_InitWithRandomFields(14);
    OverlayObject_InitWithRandomFields(15);
    Engine_ActorEnableActionCallback(8, FuneHobashira_LookoutActions);
    gEventWork->start_transition = 0x203;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(120);
    rec9 = (u8 *)Object_GetById(9);
    Engine_ActorStop(9);
    /* Reset record 9's waypoint/velocity fields: three fields to the
     * minimum s32, then four fields to zero. */
    RECORD_S32(rec9, 56) = -0x80000000;
    RECORD_S32(rec9, 60) = -0x80000000;
    RECORD_S32(rec9, 64) = -0x80000000;
    RECORD_S32(rec9, 36) = 0;
    RECORD_S32(rec9, 40) = 0;
    RECORD_S32(rec9, 44) = 0;
    RECORD_S32(rec9, 76) = 0;
    Engine_EventWait(20);
    Actor_SetSpeed(9, 0x80000, 0x40000);
    Call4(Object_SetPosition, rec9, 0xa40000, 0x900000, 0x1410000);
    Object_CommitPosition(rec9);
    Call4(Object_SetPosition, rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition(rec9);
    Call4(Object_SetPosition, rec9, 0xcc0000, 0x7c0000, 0x1410000);
    Object_CommitPosition(rec9);
    Call4(Object_SetPosition, rec9, 0x900000, 0, 0xa90000);
    Engine_ActorStop(8);
    Engine_TaskWait(1);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_ShowEmote(8, 0x103, 60);
    Actor_SetSpeed(9, 0x20000, 0x10000);
    OverlayObject_InitWithRandomFields(9);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(8, FuneHobashira_LookoutActions);
    Engine_EventWait(120);
    Engine_ActorStop(9);
    /* Same reset pattern on record 9 for the second cycle. */
    RECORD_S32(rec9, 56) = -0x80000000;
    RECORD_S32(rec9, 60) = -0x80000000;
    RECORD_S32(rec9, 64) = -0x80000000;
    RECORD_S32(rec9, 36) = 0;
    RECORD_S32(rec9, 40) = 0;
    RECORD_S32(rec9, 44) = 0;
    RECORD_S32(rec9, 76) = 0;
    Engine_EventWait(20);
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
    Engine_ActorStop(8);
    Engine_TaskWait(1);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_ShowEmote(8, 0x103, 60);
    Actor_SetSpeed(9, 0x20000, 0x10000);
    OverlayObject_InitWithRandomFields(9);
    Engine_ActorJump(8, 4, 20);
    Engine_ActorJump(8, 6, 40);
    Audio_PlayCue(29);
    GameFlag_Set(0x8f0);
    Engine_EventSetMessage((s32)MsgFuneMonsters2);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_ShowEmote(8, 0x100, 0);
    Actor_WalkToAndWait(8, 164, 0x158);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(12);
    Engine_EventEnd();
}

void OverlayObject_InitWithRandomFields(s32 a)
{
    u8 *obj;
    u32 x;

    obj = (u8 *)Object_GetById(a);
    Engine_ActorSetSpritePriority(a, 1);
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
    Engine_ActorEnableActionCallback(a, FuneHobashira_DriftActions);
}

/* Drives ids 8 through 18 through position, scale, and flag updates in
 * sequence, stepping the shared scene phase at the start and end. */
void FieldScene_RunPrimarySequence(void)
{
    u32 i;
    s32 rec;
    s32 id0_state;

    Engine_EventBegin();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    id0_state = (u8 *)Object_GetById(0);
    Engine_ActorSetSpriteFlags(id0_state, 0);
    Engine_TaskWait(1);
    Event_CallWithLastActiveObjectId((s32)FuneHobashira_EnsembleObjects);
    Engine_TaskWait(1);
    Event_CallWithLastActiveObjectId((s32)FuneHobashira_LandingObjects);
    Engine_TaskWait(1);
    OverlayObject_InitWithRandomFields(9);
    OverlayObject_InitWithRandomFields(10);
    OverlayObject_InitWithRandomFields(11);
    OverlayObject_InitWithRandomFields(12);
    OverlayObject_InitWithRandomFields(13);
    OverlayObject_InitWithRandomFields(14);
    OverlayObject_InitWithRandomFields(15);
    Engine_ActorEnableActionCallback(8, FuneHobashira_LookoutActions);
    gEventWork->start_transition = 0x203;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(0x12c);
    Audio_PlayCue(147);
    Engine_EventWait(100);
    Engine_ActorStop(9);
    Engine_ActorStop(10);
    Engine_ActorStop(11);
    Engine_ActorStop(12);
    Engine_ActorStop(13);
    Engine_ActorStop(14);
    Engine_ActorStop(15);
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
    Engine_EventWait(40);
    Actor_ShowEmote(8, 0x101, 0);
    Engine_EventWait(20);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(11, 0, 0);
    Actor_SetPosition(12, 0, 0);
    Actor_SetPosition(13, 0, 0);
    Actor_SetPosition(14, 0, 0);
    Actor_SetPosition(15, 0, 0);
    Engine_EventWait(100);
    rec = (u8 *)Object_GetById(18);
    *(s32 *)(rec + 24) = 0x1999;
    *(s32 *)(rec + 28) = 0x1999;
    Actor_SetPosition(18, 0xac0000, 0x1540000);
    Engine_ActorStop(8);
    Engine_TaskWait(1);
    Engine_ActorRunRepeatedMotion(8, 1);
    Actor_FaceDirection(8, 0x3000, 0);
    Audio_PlayCue(29);
    GameFlag_Set(0x8f0);
    for (i = 0; i < 32; i++) {
        *(s32 *)(rec + 24) += 0xccc;
        *(s32 *)(rec + 28) += 0xccc;
        Engine_TaskWait(1);
    }
    Actor_ShowEmote(8, 0x101, 60);
    Engine_ActorRunRepeatedMotion(8, 2);
    Actor_WalkToAndWait(8, 168, 0x154);
    Actor_WalkToAndWait(8, 200, 0x154);
    Actor_FaceDirection(8, 0x8000, 0);
    rec = (u8 *)Object_GetById(17);
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
    Engine_EventWait(20);
    Engine_ActorJump(8, 6, 20);
    Audio_PlayCue(147);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(8, FuneHobashira_WaveActions);
    Engine_EventWait(80);
    Engine_ActorSetSpritePriority(17, 1);
    Actor_SetSpeed(17, 0x10000, 0x8000);
    *(s32 *)(rec + 68) = 0x1999;
    *(s32 *)(rec + 72) = 0xb333;
    Audio_PlayCue(153);
    *(s32 *)(rec + 40) = 0x80000;
    Actor_SetDestination(17, 132, 0x168);
    Actor_SetDestination(18, 132, 0x168);
    Engine_EventWait(40);
    Actor_SetPosition(17, 0, 0);
    rec = (u8 *)Object_GetById(8);
    *(s32 *)(rec + 24) = 0x10000;
    *(s32 *)(rec + 28) = 0x10000;
    {
        /* Set the flag word at +6. */
        s32 shown = 0x5000;

        *(u16 *)(rec + 6) = shown;
    }
    Engine_EventWait(40);
    gEventWork->start_transition = 0x202;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(13);
    Engine_EventEnd();
}

/* Land ho: the ensemble's seven actors drift in, then each walks its way
 * ashore while the lookout cheers, and the party leaves the mast for the
 * ship's cabin; the exit depends on which way the voyage went. */
void FieldScene_RunSevenActorEnsemble(void)
{
    Engine_EventBegin();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
    Event_CallWithLastActiveObjectId((s32)FuneHobashira_EnsembleObjects);
    Engine_TaskWait(1);
    OverlayObject_InitWithRandomFields(9);
    OverlayObject_InitWithRandomFields(10);
    OverlayObject_InitWithRandomFields(11);
    OverlayObject_InitWithRandomFields(12);
    OverlayObject_InitWithRandomFields(13);
    OverlayObject_InitWithRandomFields(14);
    OverlayObject_InitWithRandomFields(15);
    Engine_ActorEnableActionCallback(8, FuneHobashira_LookoutActions);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(400);
    Engine_ActorStop(9);
    Engine_ActorStop(10);
    Engine_ActorStop(11);
    Engine_ActorStop(12);
    Engine_ActorStop(13);
    Engine_ActorStop(14);
    Engine_ActorStop(15);
    Actor_SetSpeed(9, 0x30000, 0x18000);
    Actor_SetSpeed(10, 0x30000, 0x18000);
    Actor_SetSpeed(11, 0x30000, 0x18000);
    Actor_SetSpeed(12, 0x30000, 0x18000);
    Actor_SetSpeed(13, 0x30000, 0x18000);
    Actor_SetSpeed(14, 0x30000, 0x18000);
    Actor_SetSpeed(15, 0x30000, 0x18000);
    Engine_ActorEnableActionCallback(9, FuneHobashira_EnsembleWalk9);
    Engine_ActorEnableActionCallback(10, FuneHobashira_EnsembleWalk10);
    Engine_ActorEnableActionCallback(11, FuneHobashira_EnsembleWalk11);
    Engine_ActorEnableActionCallback(12, FuneHobashira_EnsembleWalk12);
    Engine_ActorEnableActionCallback(13, FuneHobashira_EnsembleWalk13);
    Engine_ActorEnableActionCallback(14, FuneHobashira_EnsembleWalk14);
    Engine_ActorEnableActionCallback(15, FuneHobashira_EnsembleWalk15);
    Engine_EventWait(40);
    Engine_ActorStartRepeatedMotion(8, 3);
    Actor_SetAttachedEffect(8, 258);
    Engine_EventWait(120);
    Engine_ActorStartRepeatedMotion(8, 1);
    Actor_ShowEmote(8, 256, 60);
    Actor_SetSpeed(8, 0x10000, 0x8000);
    Actor_WalkToAndWait(8, 164, 344);
    Engine_ActorJump(8, 4, 10);
    Engine_ActorJump(8, 6, 20);
    Engine_EventSetMessage((s32)MsgFuneLandHo);
    Event_ShowMessageAndWait(8, 0, 20);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    gGameState.saved_scene = (s32)&SceneId_FuneHeya;
    gGameState.saved_entrance = 2;
    if (SceneData_GetDifferenceOfPairSums() == 11) {
        Engine_EventRequestExit(15);
    } else {
        Engine_EventRequestExit(14);
    }
    Engine_EventEnd();
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
