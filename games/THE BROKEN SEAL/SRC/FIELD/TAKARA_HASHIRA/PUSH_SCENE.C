#include "TYPES.H"

void Engine_EventBegin();
struct Probe {
    s32 word[6];
};

s32 StagedActor_FindClearPosition(struct Probe *probe);
void SceneActor_MoveAndRedraw(struct Probe probe);
void StagedActor_PlaceAtObjectTenCell();
void StagedActor_AdvancePair();
void Local_020024cc();
void Engine_EventEnd();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Crossbone Isle: move the pushed actor when the probe finds room, otherwise
 * put it back on its cell and step the staged pair. */
void TakaraHashira_RunPushScene(void)
{
    struct Probe probe;

    Engine_EventBegin();
    if (StagedActor_FindClearPosition(&probe) != 0) {
        SceneActor_MoveAndRedraw(probe);
    } else {
        StagedActor_PlaceAtObjectTenCell();
        StagedActor_AdvancePair();
        Local_020024cc();
    }
    Engine_EventEnd();
}
