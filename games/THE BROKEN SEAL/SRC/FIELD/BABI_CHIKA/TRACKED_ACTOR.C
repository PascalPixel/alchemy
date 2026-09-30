#include "TYPES.H"
#define FIELD_STAGED_ACTOR_IMPORTS
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"

extern u8 *gActorEffectWork;

/* Babi's tunnel: publish the player actor at +24 of the event work once it
 * has passed the row limit of the current area and story step. */
void BabiChika_UpdateTrackedActor(void)
{
    u8 *actor;
    u8 *work;
    s32 limit;

    actor = (u8 *)Object_GetById(0);
    work = gActorEffectWork;
    limit = 0;
    if (gGameState.scene == (s32)&SceneId_BabiChika1) {
        switch (gGameState.entrance) {
        case 3:
        case 4:
            limit = 94;
            break;
        case 8:
        case 9:
            limit = 74;
            break;
        case 12:
        case 13:
            limit = 118;
            break;
        }
    } else if (gGameState.entrance == 12) {
        limit = 93;
    }
    if ((*(s32 *)(actor + 16) >> 19) <= limit) {
        *(s32 *)(work + 24) = 0;
    } else {
        *(s32 *)(work + 24) = (s32)actor;
    }
}
