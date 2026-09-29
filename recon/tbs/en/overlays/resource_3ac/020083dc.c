/* Draft of resource_3ac 0x020083dc (Scene_Initialize), from
 * games/THE BROKEN SEAL/SRC/FIELD/COMMON/RUNPA_SUHARA/INTERIORS.C.
 * Remaining difference: the ROM loads scene 0x69 from the literal pool, as a
 * link-time scene symbol would; C builds it with movs. The listing keeps
 * these rows. */
#include "INTERIORS.H"

extern u8 LinkedScene_RunpaSuhara;

s32 Scene_Initialize(void)
{
    s16 entrance;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    entrance = gGameState.entrance;
    if (entrance == ROOM_SUHARA_GATE_HOUSE) {
        GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
        gGameState.saved_scene = (s32)&LinkedScene_RunpaSuhara;
        gGameState.saved_entrance = entrance;
    }
    Actor_SetSpriteFlags(Actor_Get(ACTOR_WEAPON_COUNTER), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_ARMOR_COUNTER), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_ITEM_COUNTER), 0);
    return 0;
}

