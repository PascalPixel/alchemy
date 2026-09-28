/* Draft of FieldScene_UpdateActorPairInteraction, resource_3bf at 0x02009150,
 * built with games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/FORTRESS.H.
 * Remaining difference: its double arithmetic reaches compiler-library float
 * routines that this overlay carries among its data rows (0x0200da78,
 * 0x0200daf0, 0x0200db6c), which are not linked from the library yet.
 * The listing keeps these rows. */
#include "FORTRESS.H"

double Func_02006ce6(s32);
double Func_02006c7a(double, double);
s32 Func_02006d72(double);

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
            work[8] = Func_02006d72(Func_02006c7a(8912896.0, Func_02006ce6(actor->x)));
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
