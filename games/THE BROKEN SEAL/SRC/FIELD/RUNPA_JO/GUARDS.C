/* Lunpa fortress: each frame the two guards sway with the map, and unless
 * the alarm is already raised they watch for the party: a cloaked party
 * that walks within four steps of either guard is caught, and an uncloaked
 * one they can talk to sets the alarm. */
#include "FORTRESS.H"

void FieldScene_UpdateActorPairInteraction(void)
{
    struct ObjectRuntime *actor = Object_GetById(9);
    struct ObjectRuntime *other = Object_GetById(10);
    s32 *work = (s32 *)(*(u8 **)gCam + 0x164);
    s16 *scene = *(s16 **)(gCam + 0x4c);

    if (gFrameCount & 1) {
        work[6] = 1;
        work[7] = 1;
    } else {
        work[6] = -1;
        work[7] = -1;
    }
    if (GameFlag_IsSet(0x106) || scene[191] != 0 || scene[192] != 0) {
        actor->movement_state = 1;
        other->movement_state = 1;
    } else if (!GameFlag_IsSet(0x214)) {
        actor->movement_state = 0;
        other->movement_state = 0;
        if (!GameFlag_IsSet(0x214) && actor->movement_state == 0) {
            work[8] = 8912896.0 - actor->x;
        }
        if (!IsPlayerInAccidentTriggerArea()) {
            if (gGameState.cloaked != 0) {
                if (IsSceneActorWithinFourSteps(9) && gGameState.cloaked != 0) {
                    SetSceneValue(&scene[191], 0x2092);
                    return;
                }
                if (IsSceneActorWithinFourSteps(10) && gGameState.cloaked != 0) {
                    SetSceneValue(&scene[191], 0x2092);
                    return;
                }
            }
            if (gGameState.cloaked == 0) {
                if (IsActorInteractionAvailable(9)) {
                    GameFlag_Set(0x215);
                    GameFlag_Set(0x214);
                }
                if (IsActorInteractionAvailable(10)) {
                    GameFlag_Set(0x215);
                    GameFlag_Set(0x214);
                }
            }
            if (GameFlag_IsSet(0x214)) {
                SetSceneValue(&scene[193], 91);
            }
        }
    }
}
