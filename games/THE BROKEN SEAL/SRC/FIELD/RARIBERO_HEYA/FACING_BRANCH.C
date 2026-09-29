#include "TYPES.H"
extern u8 MsgRariberoWantedSendShips[];
extern u8 MsgRariberoWarriorRightPlease[];

/*
 * Raribero house: a face-toward branch. If the party faces the door (north)
 * the door line plays; otherwise a flag picks one of the two default lines.
 */

u16 *Engine_ActorGet(s32);
s32 Engine_GameFlagIsSet(s32);
void Shop_Run(s32, s32);
void Engine_EventSetMessage(s32);
void Engine_EventSetMessage(s32);
void Engine_EventShowMessage(s32, s32);
void Engine_EventShowMessage(s32, s32);

void Dialogue_HandleFacingBranch(s32 no)
{
    u16 party_facing = (Engine_ActorGet(0)[3] + 0x2000) & ~0x3fff;
    if (party_facing == 0xc000) {
        Shop_Run(33, no);
    } else if (Engine_GameFlagIsSet(0x9a7)) {
        Engine_EventSetMessage((s32)MsgRariberoWantedSendShips);
        Engine_EventShowMessage(no, 0);
    } else {
        Engine_EventSetMessage((s32)MsgRariberoWarriorRightPlease);
        Engine_EventShowMessage(no, 0);
    }
}
