#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "STAGED_ACTOR_PAIR_SCENE.H"
#include "STAGED_ACTOR.H"

enum StagedPairMessage {
    MSG_WARRIORS_HAVE_BEEN_FIGHTING_WHILE = 0x23d2,
    MSG_WE_MISSED_COLOSSO_BECAUSE_WE = 0x23d5,
    MSG_IVE_BEEN_WAITING_FOR_ROBIN = 0x23d9,
    MSG_WHY_GOING_BACK_ROBIN_DO = 0x23da
};

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

extern u8 LinkedMessage_DoYouWishCrossInto[];

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Actor_SetPosition(actor, x, y);
}

void StagedActorPairScene_NoopSceneCallback(void){}

void StagedActorPairScene_RunActorTwelveCommand(void)
{
    Actor_SetPosition(12, 0, 0);
}
