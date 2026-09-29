#include "CAVE.H"

/* The cave's scene start: open with the window transition, hide the four
   puddles, set the north one's scale, and redraw the gate and the pillars
   its flags record. */
s32 Scene_Initialize(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (gGameState.scene == (s32)&SceneId_RunpaDou) {
        Actor_SetSpriteFlags(Actor_Get(ACTOR_HIDDEN_PUDDLE), 0);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_SOUTH_PUDDLE), 0);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_GATE_PUDDLE), 0);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_NORTH_PUDDLE), 0);
        Actor_Get(ACTOR_NORTH_PUDDLE)->scale_y = 0xf333;
        if (GameFlag_IsSet(FLAG_CAVE_GATE_LOWERED) != 0) {
            Gate_Lower();
        }
        if (GameFlag_IsSet(FLAG_CAVE_GATE_RAISED) != 0) {
            Gate_Raise();
        }
        if (GameFlag_IsSet(FLAG_CAVE_GATE_PROPPED) != 0) {
            Gate_DrawPropped();
        }
        if (GameFlag_IsSet(FLAG_CAVE_NORTH_PILLAR) != 0) {
            Actor_SetAnimation(ACTOR_NORTH_PUDDLE, PUDDLE_ANIM_FROZEN);
        }
        if (GameFlag_IsSet(FLAG_CAVE_SOUTH_PILLAR) != 0) {
            Actor_SetAnimation(ACTOR_SOUTH_PUDDLE, PUDDLE_ANIM_FROZEN);
        }
    }
    return 0;
}
