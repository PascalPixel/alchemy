#include "VILLAGE.H"

/* The village hides its secrets and floats its Nut; the gate sets where a
 * retreat returns to, brings out a party thrown out or sneaking out, starts
 * the guards watching and forgets what the party did in the fortress. */
s32 Scene_Initialize(void)
{
    struct FieldActor *puddle;

    if (gGameState.scene == (s32)&SceneId_RunpaMura1) {
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        Reveal_HideSecrets();
        if (GameFlag_IsSet(FLAG_LUNPA_NUT_CAUGHT) == 0) {
            FloatingNut_Initialize(ACTOR_FLOATING_NUT);
        }
        puddle = Actor_Get(ACTOR_HIDDEN_PUDDLE);
        if (puddle != NULL) {
            Actor_SetSpriteFlags(puddle, 0);
        }
        GameFlag_Set(FLAG_LUNPA_SECRETS_HIDDEN);
    }
    if (gGameState.scene == (s32)&SceneId_RunpaMura2) {
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        gGameState.retreat_entrance = GATE_RETREAT_ENTRANCE;
        if (gGameState.entrance == GATE_ENTRANCE_THROWN_OUT
            && GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
            Party_ThrownOut();
        }
        if (gGameState.entrance == GATE_ENTRANCE_SNEAKING_OUT
            && GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
            Leader_SneaksOut();
        }
        if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0
            && GameFlag_IsSet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
            Task_AddCallback(Party_WatchForFortress, TASK_PRIORITY_SCENE);
        }
        Task_AddCallback(Guards_Watch, TASK_PRIORITY_SCENE);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 1);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 2);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 3);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 4);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 5);
        GameFlag_Clear(FLAG_FORTRESS_VISIT);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 6);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 7);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 8);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 9);
    }
    return 0;
}
