#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "TEMPLE.H"

s32 StopXianActor(void *actor)
{
    Actor_SetSpriteFlags(actor, 0);
    return 0;
}

s32 FaceXianActorToPlayer(void *actor)
{
    void *player = Actor_Get(ACTOR_PARTY_LEADER);
    FIELD(actor, u16, 6) = ArcTan2Far(FIELD(player, s32, 0x10) - FIELD(actor, s32, 0x10), FIELD(player, s32, 8) - FIELD(actor, s32, 8));
    return 0;
}
