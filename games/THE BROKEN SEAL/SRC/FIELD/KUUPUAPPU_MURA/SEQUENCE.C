/*
 * Later scene steps: the leader's arrival for scene twelve and thirteen, and
 * the sequences of actors 23 and 19.
 */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

struct Presentation {
    u8 reserved_00[9];
    u8 flags;
};

struct SceneActor_0200113c {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

void SceneActor_PlaceAndSetSceneDelay(s32 x, s32 y, s32 continuation);
void PartyInventory_Discard();

/* Map cell steps played as the leader arrives in scene twelve. */
extern const u16 KuupuappuMura_Scene12Cells[];

void ActorPresentation_SetupActorZeroForSceneTwelveAt72_160(void)
{

    struct SceneActor_0200113c *actor = Actor_Get(ACTOR_PARTY_LEADER);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Audio_PlayCue(158);
    Map_AnimateCells(KuupuappuMura_Scene12Cells, 35, 9);
    {
        s32 cell = 4;
        s32 row = 10;

        Map_CopyCellAttributes(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    SceneActor_PlaceAndSetSceneDelay(72, 160, 12);
}

void FieldScene_SetupScene13At152_264(void) { Engine_AudioPlayCue(123); SceneActor_PlaceAndSetSceneDelay(152, 264, 13); }

void ActorPresentation_MoveActorToPositionAndWait(int actor, int x, int z, int field40)
{
    void Task_Wait(int);
    void Actor_SetPosition(int, int, int);

    u8 *record = Actor_Get(actor); int frames;
    Actor_SetSpeed(actor, 0x30000, 0x18000); *(s32 *)(record + 72) = 0x8000;
    *(s32 *)(record + 68) = 0; *(s32 *)(record + 40) = field40; Actor_SetSpriteFlags(record, 0);
    Actor_MoveToAndWait(actor, x, z); Actor_SetPosition(actor, x << 16, z << 16);
    for (frames = 60; frames != 0; --frames) { Task_Wait(1); if (*(s16 *)(record + 42) == 0) break; }
    Actor_SetSpriteFlags(record, 1); *(s32 *)(record + 72) = 0x10000;
}

void FieldScene_RunActor23SequenceOnceByFlag867(void)
{
    void Event_Wait();

    u32 i;
    s32 record;

    Event_Begin();
    Audio_PlayCue(100);
    Event_Wait(40);
    if (GameFlag_IsSet(0x867) == 0) {
        Actor_SetAttachedEffect(23, 0x102);
        Actor_Jump(23, 4, 0);
        Event_Wait(12);
        Actor_Jump(23, 4, 0);
        Event_Wait(20);
        ActorPresentation_MoveActorToPositionAndWait(23, 0x188, 104, 0x70000);
        Event_Wait(20);
        Actor_WalkToAndWait(23, 0x198, 104);
        Actor_WalkToAndWait(23, 0x198, 120);
        GameFlag_Set(0x867);
    }
    Event_End();
}

void FieldScene_RunActor19MotionSequence(void)
{
    u32 i;
    s32 record;

    PartyInventory_Discard(231);
    Event_Begin();
    Event_Wait(10);
    Actor_RunRepeatedMotion(19, 2);
    Actor_SetSpeed(19, 0xcccc, 0x6666);
    Actor_WalkToAndWait(19, 216, 0x198);
    Event_Wait(10);
    Actor_FaceDirection(19, 0x4000, 20);
    Actor_Jump(19, 6, 0);
    Event_Wait(30);
    Actor_Jump(19, 6, 0);
    Event_Wait(30);
    Actor_Jump(19, 6, 0);
    Event_Wait(30);
    Actor_WalkToAndWait(19, 216, 0x188);
    Event_Wait(10);
    Actor_FaceDirection(19, 0x4000, 20);
    GameFlag_Set(0x858);
    Event_End();
}
