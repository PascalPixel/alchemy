/* Draft of resource_3ad 0x02009a0c..0x02009ad4 (200 bytes with pool),
 * Scene_Initialize, the overlay's first entry; the listing keeps the rows.
 * Remaining difference: the reference compares the scene with the cave's own
 * id 0x6a loaded from its literal pool, a link-time value; the integer scene
 * is an immediate (196 bytes, 81 differ from +0x12). */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/CAVE.H"

s32 Scene_Initialize(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (gGameState.scene == 0x6a) {
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
