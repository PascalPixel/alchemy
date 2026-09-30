#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "STAGED_ACTOR_PAIR_SCENE.H"
#include "STAGED_ACTOR.H"
extern u8 MsgKaragoruIveBeenWaitingForRobin[];
extern u8 MsgKaragoruDoYouWishCrossInto[];

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

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Actor_SetPosition(actor, x, y);
}

void ActorPresentation_RunActorElevenRecoveryScene(void)
{
    Event_Begin();
    Actor_FaceActor(11, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 11, 0);
    Event_SetMessage((s32)MsgKaragoruIveBeenWaitingForRobin);
    Event_ShowMessage(11, 0);
    Actor_SetAnimation(11, 2);
    {
        s16 *position = Actor_Get(ACTOR_PARTY_LEADER);

        if (position != 0)
            Actor_SetDestination(11, position[5], position[9]);
    }
    Actor_WaitForMove(11);
    Actor_SetPosition(11, 0, 0);
    Event_Wait(20);
    GameFlag_Set(2464);
    Event_End();
}

void KaragoruDou_AskToCross(void)
{
    s32 base;

    base = (s32)MsgKaragoruDoYouWishCrossInto;
    Event_SetMessage(base);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        if (GameFlag_IsSet(0x950) != 0) {
            if (GameFlag_IsSet(0x96f) == 0) {
                Event_SetMessage((base + 8));
            }
        }
        Event_ShowMessage(8, 0);
    } else {
        bump_step(1);
        Event_ShowMessage(8, 0);
    }
}
