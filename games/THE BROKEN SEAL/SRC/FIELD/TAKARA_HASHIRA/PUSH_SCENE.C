#include "TYPES.H"

void Engine_EventBegin();
struct Probe {
    s32 word[6];
};

s32 StagedActor_FindClearPosition(struct Probe *probe);
void SceneActor_MoveAndRedraw(struct Probe probe);
void StagedActor_PlaceAtObjectTenCell();
void StagedActor_AdvancePair();
void TakaraHashira_DropActorTen();
void Engine_EventEnd();

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
        TakaraHashira_DropActorTen();
    }
    Engine_EventEnd();
}
