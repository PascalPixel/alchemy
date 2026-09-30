#include "TYPES.H"
#include "KANPAN.H"
#include "FIELD_EVENT.H"
#include "CALL.H"
extern u8 MsgFuneDontRowAnymore[];
extern u8 MsgFuneMadeFinallyLucky[];
extern u8 MsgFuneMakeOldMan[];
extern u8 MsgFuneOhhhHaventWorkout[];
extern u8 MsgFuneOtherPassengersAlready[];
extern u8 MsgFuneRowingShipMore[];
extern u8 MsgFuneThanksHardWork[];
extern u8 MsgFuneTotallyLostOcean[];

s32 SceneState_FindFirstSetFlagOfGroup(s32 index);
void FieldScene_RunScene3af_02000bb8(void);
void FieldScene_RunStepThen10(s32 actor);
void FieldScene_CallPairWith10(s32 actor, s32 value);
void Engine_ActorStartAction(s32 actor);

extern const u8 FuneKanpan_RosterActions[];

/* Walks up to four present party members in one at a time, each with a line chosen by who they are, then clears story flag 0x12f. */
void FieldScene_RunPartyRosterScene(void)
{
    s32 roster[4];
    s32 matched[4];
    s32 category[4];
    u32 index;
    s32 found;
    s32 actor;
    s32 member;
    struct FieldActor *record;

    found = 0;
    for (index = 0; index <= 3; index++) {
        roster[index] = SceneState_FindFirstSetFlagOfGroup((s32)index);
        category[index] = 0;
    }

    /* Phase 2 -- nine search blocks, in source order. */
    member = 23;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = found;
            found++;
            break;
        }
    }
    member = 24;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = found;
            found++;
            break;
        }
    }
    member = 25;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = found;
            found++;
            break;
        }
    }
    member = 27;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = found;
            found++;
            break;
        }
    }
    if (found == 4)
        goto scene;
    member = 28;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = found;
            found++;
            break;
        }
    }
    if (found == 4)
        goto scene;
    member = 29;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = found;
            found++;
            break;
        }
    }
    if (found == 4)
        goto scene;
    member = 26;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = 10;
            found++;
            break;
        }
    }
    if (found == 4)
        goto scene;
    member = 30;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = 11;
            found++;
            break;
        }
    }
    if (found == 4)
        goto scene;
    member = 31;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = 20;
            break;
        }
    }

scene:
    /* Phase 3 -- open the scene. */
    Engine_EventBegin();
    Engine_ActorSetChildValue(0, 15);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(0), 0);
    record = Engine_ActorGet(0);
    if (record != 0) {
        Engine_ActorSetPosition(32, record->x.fixed, record->z.fixed);
    }
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Engine_CameraFollowActor(32, 1);

    FieldScene_RunScene3af_02000bb8();

    Engine_EventWait(10);

    /* Phase 4 -- one beat per matched member. */
    for (index = 0; index <= 3; index++) {
        actor = matched[index];
        Call3(Engine_ActorSetPosition, actor, 216 << 16, 146 << 18);
        if (category[index] == 20) {
            Call3(Engine_ActorSetSpeed, actor, 0xcccc, 0x6666);
        } else {
            Call3(Engine_ActorSetSpeed, actor, 128 << 9, 128 << 8);
        }
        Call3(Engine_ActorWalkToAndWait, actor, 216, 150 << 2);
        Call3(Engine_ActorWalkToAndWait, actor, 192, 0x26a);
        Engine_ActorWalkToAndWait(actor, 192, 164 << 2);

        switch (category[index]) {
        case 0:
            Engine_ActorShowEmote(actor, 129 << 1, 60);
            Engine_EventSetMessage((s32)MsgFuneMadeFinallyLucky);
            break;
        case 1:
            FieldScene_CallPairWith10(actor, 208 << 8);
            Engine_ActorShowEmote(actor, 129 << 1, 60);
            Engine_EventSetMessage((s32)MsgFuneOtherPassengersAlready);
            break;
        case 2:
            Engine_ActorShowEmote(actor, 0x105, 60);
            Engine_EventSetMessage((s32)MsgFuneDontRowAnymore);
            break;
        case 3:
            Engine_ActorRunRepeatedMotion(actor, 1);
            Engine_EventSetMessage((s32)MsgFuneOhhhHaventWorkout);
            break;
        case 10:
            Engine_ActorSetAnimationAndWait(actor, 3);
            Engine_EventSetMessage((s32)MsgFuneRowingShipMore);
            break;
        case 11:
            Engine_ActorSetAnimation(actor, 4);
            Engine_EventSetMessage((s32)MsgFuneTotallyLostOcean);
            break;
        case 20:
            Engine_ActorSetAnimationAndWait(actor, 4);
            Engine_ActorShowEmote(actor, 0x107, 40);
            Engine_EventSetMessage((s32)MsgFuneMakeOldMan);
            break;
        default:
            break;
        }

        FieldScene_RunStepThen10(actor);
        Engine_ActorEnableActionCallback(actor, FuneKanpan_RosterActions);
    }

    /* Phase 5 -- teardown. */
    Engine_ActorStartAction(actor);
    Engine_EventWait(40);
    Engine_ActorSetPosition(0, 216 << 16, 146 << 18);
    Engine_TaskWait(1);
    Engine_ActorSetChildValue(0, 0);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(0), 1);
    Call3(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkToAndWait, 0, 216, 150 << 2);
    Call3(Engine_ActorWalkToAndWait, 0, 190, 153 << 2);
    Engine_EventSetMessage((s32)MsgFuneThanksHardWork);
    FieldScene_RunStepThen10(20);
    Engine_ActorFaceDirection(0, 192 << 8, 0);

    Engine_ActorGet(32)->motion_flags = 0;
    Call3(Engine_ActorSetSpeed, 32, 128 << 10, 128 << 9);
    Call3(Engine_ActorSetDestination, 32, 196, 141 << 2);

    Call3(Engine_ActorSetSpeed, 20, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkToAndWait, 20, 182, 0x22b);
    Call3(Engine_ActorFaceDirection, 20, 192 << 6, 20);
    Engine_ActorRunRepeatedMotion(20, 1);
    FieldScene_RunStepThen10(20);
    Engine_ActorSetAnimationAndWait(20, 3);
    FieldScene_RunStepThen10(20);
    Engine_ActorFaceDirection(20, 128 << 8, 40);
    FieldScene_RunStepThen10(20);
    FieldScene_CallPairWith10(20, 192 << 6);
    FieldScene_RunStepThen10(20);
    Engine_ActorSetAnimationAndWait(20, 3);
    Engine_CameraFollowActor(0, 1);
    Call3(Engine_ActorWalkToAndWait, 20, 188, 128 << 2);
    Engine_ActorFaceDirection(20, 192 << 6, 0);
    Engine_ActorSetPosition(32, 0, 0);

    Engine_GameFlagClear(0x12f);
    Engine_EventEnd();
}

