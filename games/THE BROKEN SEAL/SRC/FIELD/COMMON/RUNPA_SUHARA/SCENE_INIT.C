#include "INTERIORS.H"
#include "SCENE_IDS.H"

/* Lunpa's interiors open with the window transition. Entering from the
   Suhara gate house clears the location-name flag and saves these rooms and
   that entrance; the three shop counters' sprite flags are cleared. */
s32 Scene_Initialize(void)
{
    s16 entrance;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    entrance = gGameState.entrance;
    if (entrance == ROOM_SUHARA_GATE_HOUSE) {
        GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
        gGameState.saved_scene = (s32)&SceneId_RunpaSuhara;
        gGameState.saved_entrance = entrance;
    }
    Actor_SetSpriteFlags(Actor_Get(ACTOR_WEAPON_COUNTER), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_ARMOR_COUNTER), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_ITEM_COUNTER), 0);
    return 0;
}

