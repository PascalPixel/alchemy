#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "STAGED_ACTOR_PAIR_SCENE.H"
#include "STAGED_ACTOR.H"
extern u8 MsgKaragoruWarriorsHaveBeenFightingWhile[];
extern u8 MsgKaragoruWeMissedColossoBecauseWe[];

struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};

struct HeightTrackedObject {
    u8 pad00[12];
    s32 height;                 /* +12 */
};

s32 *Object_GetById();

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Actor_SetPosition(actor, x, y);
}

void ActorPresentation_SelectActorNineScript(void)
{
    if (GameFlag_IsSet(2384) != 0 && GameFlag_IsSet(2415) == 0)
        Event_SetMessage((s32)MsgKaragoruWeMissedColossoBecauseWe);
    else
        Event_SetMessage((s32)MsgKaragoruWarriorsHaveBeenFightingWhile);
    Event_ShowMessage(9, 0);
}

void FieldScene_RunScene3be_02001080(void)
{
    extern u8 *Data_03001ebc;

    u8 *work;

    work = Data_03001ebc;
    Event_Begin();
    if (GameFlag_IsSet(0x204) != 0) {
        GameFlag_Clear(0x9a3);
        GameFlag_Clear(0x9a5);
        GameFlag_Clear(0x9a4);
        GameFlag_Clear(0x9a6);
        GameFlag_Set(0x9a5);
        GameFlag_Set(0x9a4);
    }
    Event_RequestExit(*(s16 *)(work + 0x16c));
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_End();
}

void StagedActorPairScene_RunStep(void)
{
    Leader_CheckAhead();
}

void ActorPresentation_RunActorEightThresholdScene(void)
{
    Actor_Get(8);
    Event_Begin();
    {
        s32 *actor = Actor_Get(8);

        if ((actor[2] >> 20) <= 30) {
            StagedActorPairScene_RunSpinningLeap(8);
            {
                s32 x = 27;
                s32 y = 19;

                Map_CopyCellAttributes(29, 19, 1, 1, x, y);
            }
            GameFlag_Set(2466);
        }
    }
    Event_End();
}

void StagedActorPairScene_RunUpdate(void)
{
    StagedActor_AdvancePair();
    ActorPresentation_RunActorNineThresholdScene();
}

void ActorPresentation_RunActorNineThresholdScene(void)
{
    Event_Begin();
    if ((Object_GetById(9)[2] >> 20) > 42) {
        s32 x = 107;
        s32 y = 17;

        Map_CopyCellAttributes(108, 17, 1, 1, x, y);
        Event_Wait(8);
        Actor_SetPosition(9, 0, 0);
        PlaceActor(10, 45613056, 18874368);
        Actor_SetAnimation(10, 3);
        Audio_PlayCue(154);
        GameFlag_Set(2469);
    }
    Event_End();
}

void StagedActorPairScene_NoopActorCallback(void){}

void StagedActorPairScene_RotateActorPart(u8 *actor)
{
    u8 *sprite_part = *(u8 **)(actor + 80);

    *(u16 *)(sprite_part + 30) -= 0x400;
}

void StagedActorPairScene_WaitForHeight(struct HeightTrackedObject *object,
                                       s32 limit)
{
    extern u8 Data_03001ebc[];

    s32 frames = 40;

    while (frames != 0) {
        Task_Wait(1);
        frames--;
        if (object->height <= limit) {
            break;
        }
    }
}
