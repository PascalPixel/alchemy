#include "TYPES.H"
extern u8 MsgRariberoDidntKnowTime[];
extern u8 MsgRariberoWantStayLet[];

/*
 * Raribero house: a face-toward cue branch. If the party faces the door
 * (north) the cue line plays; otherwise a flag picks one of the two default
 * lines.
 */

u16 *Engine_ActorGet(s32);
s32 Engine_GameFlagIsSet(s32);
void Inn_CheckIn(s32, s32);
void Engine_EventSetMessage(s32);
void Engine_EventSetMessage(s32);
void Engine_EventShowMessage(s32, s32);
void Engine_EventShowMessage(s32, s32);

void Dialogue_HandleFacingCueBranch(s32 no)
{
    u16 party_facing = (Engine_ActorGet(0)[3] + 0x2000) & ~0x3fff;
    if (party_facing == 0xc000) {
        Inn_CheckIn(11, no);
    } else if (Engine_GameFlagIsSet(0x9a7)) {
        Engine_EventSetMessage((s32)MsgRariberoDidntKnowTime);
        Engine_EventShowMessage(no, 0);
    } else {
        Engine_EventSetMessage((s32)MsgRariberoWantStayLet);
        Engine_EventShowMessage(no, 0);
    }
}
