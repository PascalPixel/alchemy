#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 Main_080772c8(void);
void Main_0808a5b0(void);
void Main_08015120(s32 value, s32 digits);
void Main_0808a270(s32 value, s32 mode);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;
extern u8 Data_00000089[];

/* Tolbi game attendant: quote ten coins per level; without enough coins say so, otherwise record the coins and, if the player accepts, walk in and start the game area. */
void TorebiIzumi_PayToPlay(void)
{
    u32 cost;
    u32 coins;

    cost = Main_080772c8() * 10;
    Engine_EventBegin();
    coins = Data_02000240_t.words[4];
    if (coins < cost) {
        Engine_EventSetMessage(0xe12);
        Engine_EventOpenMessage(9, 0);
        return;
    }
    *(u32 *)0x02001000 = coins;
    Main_0808a5b0();
    Engine_EventSetMessage(0xe0e);
    Main_08015120(cost, 5);
    Engine_EventOpenMessage(9, 0);
    if (Engine_UiWorkWaitThenFinalizeCapacity(0, 0) == 0) {
        Engine_EventShowMessage(9, 0);
        Engine_ActorWalkToAndWait(0, 120, 128);
        Engine_ActorWalkToAndWait(0, 120, 152);
        Engine_ActorFaceDirection(0, 0x8000, 0);
        Engine_EventWait(20);
        Main_0808a270(0x1fd, 0);
        Engine_GameStateSetReturn((s32)Data_00000089, 13);
    } else {
        gEventWork->message++;
        Engine_EventShowMessage(9, 0);
    }
    Engine_EventEnd();
}
