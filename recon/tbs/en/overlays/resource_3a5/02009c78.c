/* Draft of resource_3a5 0x02009c78 (SceneState_SetHalfwordB030), from
 * games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/SELECTED_ACTOR_SCENE_ID_SELECTED_ACTOR_SCENE.C.
 * Remaining difference: none in its instructions, but the halfword it sets
 * is overlay work past the image with no definition yet. The listing keeps
 * these rows. */
#include "RAMAKAN.H"

void SceneState_SetHalfwordB030(u16 value)
{
    *(u16 *)0x0200b030 = value;
}
