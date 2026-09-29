/* Draft of SuharaSabaku_DropAndLeave, resource_3c0 at 0x02008c00 (was
 * FIELD/SUHARA_SABAKU/DROP_AND_LEAVE.C).
 * Remaining difference: the ROM loads scene 0xa5 from its literal pool as a
 * link-time value; GCC compares an immediate (40 bytes differ).
 * The listing keeps these rows. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

/* Drop the selected subject and the actor GameFlag_GetByte(0x218) names: play cue 219, accelerate both along y for 60 frames, close the screen, set flag 0x122 and leave for map 2, entrance 77 when state row 224 is 0xa5 and that actor is 11, else entrance 27. */
void SuharaSabaku_DropAndLeave(void)
{
    s32 other;
    struct FieldActor *leader;
    struct FieldActor *partner;
    s32 i;

    other = GameFlag_GetByte(0x218);
    leader = Engine_ActorGet((*(union GameStateRows *)&gGameState).words[125]);
    partner = Engine_ActorGet(other);
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_AudioPlayCue(219);
    Engine_ActorSetSpriteFlags((struct FieldActor *)(*(union GameStateRows *)&gGameState).words[125], 0);
    partner->motion_flags = 0;
    leader->motion_flags = 0;
    leader->velocity_y = 0;
    leader->unknown_5d[4] = 1;
    partner->unknown_5d[4] = 1;
    for (i = 0; i < 60; i++) {
        leader->velocity_y += 0x3333;
        partner->velocity_y += 0x3333;
        Engine_TaskWait(1);
    }
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
    GameFlag_Set(0x122);
    if ((*(union GameStateRows *)&gGameState).halves[224][0] == 0xa5 && GameFlag_GetByte(0x218) == 11)
        Event_SetPairWork1c0(0x2, 77);
    else
        Event_SetPairWork1c0(0x2, 27);
}
