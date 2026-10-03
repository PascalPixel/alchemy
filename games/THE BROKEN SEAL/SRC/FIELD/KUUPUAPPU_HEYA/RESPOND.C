/* Actor 25 answers when the party speaks to it. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SERVICE.H"

extern u8 MsgKuupuappuNoLeaveAlone[];
extern const u8 KuupuappuHeya_PairScriptR[];
extern const u8 KuupuappuHeya_PairScriptP[];
/* The walk that takes the actor on from each of its four stops, clockwise
   and counter-clockwise. */
extern const u8 *KuupuappuHeya_ResponseScripts[2][4];

void Object_RefreshSelectorById(s32 actor);

/* The actor's record, as far as the response reads it. */
struct Responder {
    u8 unknown_00[6];
    u16 facing;
    u8 unknown_08[0x5c];
    /* Which of its stops the actor stands at: 0 to 3, or 4 for the middle. */
    s16 stop;
};

/* Sends the actor on along one row of the walks and moves its stop on by
   the turn, within the pair of stops it is in. */
static inline void KuupuappuHeya_WalkOn(struct Responder *actor, s32 row, s32 half, u16 turn)
{
    Engine_ActorEnableActionCallback(25, KuupuappuHeya_ResponseScripts[row][actor->stop]);
    actor->stop = actor->stop - half * 2 + turn;
}

/*
 * The actor refuses and walks away from the party: from the middle to one
 * side or the other, and from a stop on to the next or back to the last,
 * by which way it is facing.
 */
void KuupuappuHeya_RunActor25Response(void)
{
    struct Responder *actor;
    u32 facing;
    s32 half;

    actor = (struct Responder *)Object_GetById(25);
    facing = actor->facing & 0xf000;
    half = actor->stop >> 1;
    Engine_EventBegin();
    Engine_ActorRunRepeatedMotion(25, 2);
    Engine_EventSetMessage((s32)MsgKuupuappuNoLeaveAlone);
    Engine_EventShowMessage(25, 0);
    Engine_ActorSetSpeed(25, 0x38000, 0x1c000);
    switch (actor->stop) {
    case 4:
        if (facing > 0x2000 && facing < 0xa000) {
            Engine_ActorEnableActionCallback(25, KuupuappuHeya_PairScriptR);
            actor->stop = 2;
        } else {
            Engine_ActorEnableActionCallback(25, KuupuappuHeya_PairScriptP);
            actor->stop = 3;
        }
        break;
    case 0:
    case 2:
        if (facing > 0x2000 && facing < 0xa000) {
            KuupuappuHeya_WalkOn(actor, half, half, 1);
        } else {
            KuupuappuHeya_WalkOn(actor, half ^ 1, half, -1);
        }
        break;
    case 1:
    case 3:
        if (facing > 0x6000 && facing < 0xe000) {
            KuupuappuHeya_WalkOn(actor, half, half, 1);
        } else {
            KuupuappuHeya_WalkOn(actor, half ^ 1, half, -1);
        }
        break;
    }
    actor->stop &= 3;
    Object_RefreshSelectorById(25);
    Engine_EventEnd();
}
