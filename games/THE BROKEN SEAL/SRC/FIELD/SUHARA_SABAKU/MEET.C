/* The Suhara desert: meeting one of the stranded actors 8 to 12. */
#include "SABAKU.H"
#include "SCENE_IDS.H"

void BattleFx_SetWeightedResult(s32 actor, s32 mode);

/* The game state seen as rows, as this handler addresses it. */
union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

void SuharaSabaku_MeetActor(s32 a0, s32 actor)
{
    register s32 a1 asm("r6") = actor; /* FAKEMATCH: pins the actor id to r6 */
    union GameStateRows *state;

    if (gEventWork->raised_trigger == 99) {
        {
            u16 *target = (u16 *)&gEventWork->raised_trigger;
            s32 shown = 0;

            *target = shown;
        }
    }
    Engine_GameFlagClear(0x20f);
    if (gGameState.scene == (s32)&SceneId_SuharaSabaku1) {
        Engine_GameFlagSet((a1 + 0x2f9));
    } else {
        if (gGameState.scene == (s32)&SceneId_SuharaSabaku2) {
            Engine_GameFlagSet((a1 + 0x309));
        }
    }
    GameFlag_SetByte(0x210, 0);
    ((void (*)())BattleFx_SetWeightedResult)(98, 5);
    /* FAKEMATCH: publish the byte before retaining the scene pointer. */
    do {
        ((union GameStateRows *)&gGameState)->bytes[277][1] = 3;
    } while (0);
    state = (union GameStateRows *)&gGameState;
    if (state->halves[224][0] == (s32)&SceneId_SuharaSabaku2) {
        if (a1 == 11) {
            BattleFx_SetWeightedResult(98, 7);
        } else {
            if (a1 == 12) {
                BattleFx_SetWeightedResult(98, 6);
                Engine_ActorStop(12);
                Engine_ActorSetPosition(12, 0, 0);
            }
        }
    }
    Engine_ActorGet(state->words[125])->motion_flags = 3;
}
