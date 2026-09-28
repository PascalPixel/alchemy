/* Draft of resource_3ab 0x02009668..0x020097d8 (368 bytes with pool),
 * Scene_Initialize; the listing keeps the rows. Remaining difference: the
 * reference loads the scene numbers 0x68 and 0x9f from its literal pool, as a
 * link-time scene symbol does; the constants compile to cmp with an immediate
 * and the function is 12 bytes shorter. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/VILLAGE.H"

s32 Scene_Initialize(void)
{
    struct FieldActor *puddle;

    if (gGameState.scene == SCENE_RUNPA_MURA) {
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
    if (gGameState.scene == SCENE_RUNPA_JO_GATE) {
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
