#include "TYPES.H"
#include "FIELD_EVENT.H"

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

s32 Main_080770e0(s32 flag);
void Main_0808a238(s32 map, s32 entrance);

extern union GameStateRows gGameStateRows;
extern u8 Data_00000002[];
extern u8 Data_000000a5[];

/* Drop the selected subject and the actor Main_080770e0(0x218) names: play cue 219, accelerate both along y for 60 frames, close the screen, set flag 0x122 and leave for map 2, entrance 77 when state row 224 is 0xa5 and that actor is 11, else entrance 27. */
void SuharaSabaku_DropAndLeave(void)
{
    s32 other;
    struct FieldActor *leader;
    struct FieldActor *partner;
    s32 i;

    other = Main_080770e0(0x218);
    leader = Engine_ActorGet(gGameStateRows.words[125]);
    partner = Engine_ActorGet(other);
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_AudioPlayCue(219);
    Engine_ActorSetSpriteFlags((struct FieldActor *)gGameStateRows.words[125], 0);
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
    if (gGameStateRows.halves[224][0] == (s32)Data_000000a5 && Main_080770e0(0x218) == 11)
        Main_0808a238((s32)Data_00000002, 77);
    else
        Main_0808a238((s32)Data_00000002, 27);
}
