#include "PROBE.H"

void FieldScene_RunFourStepSequence(void)
{
    Event_Begin();
    StagedActor_AdvancePair();
    MakyuriHeya_SyncBlockFlags();
    Event_End();
}
