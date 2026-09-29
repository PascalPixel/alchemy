/* Choose each drifting slot's mode from the first set flag of its group,
   or mode 3 when none is set. */
#include "TYPES.H"

/* The slots' modes, uninitialised work after the deck's block. */
s32 FuneKanpan_SlotMode[8];

s32 SceneState_FindFirstSetFlagOfGroup(u32 group);
s32 SceneData_SelectTableEntryByFlagGroup(u32 group);

void FuneKanpan_ChooseSlotModes(void)
{
    s32 *mode = FuneKanpan_SlotMode;
    u32 i;

    for (i = 0; i <= 3; i++) {
        if (SceneState_FindFirstSetFlagOfGroup(i) != 0)
            *mode = SceneData_SelectTableEntryByFlagGroup(i);
        else
            *mode = 3;
        mode++;
    }
    if (SceneState_FindFirstSetFlagOfGroup(0) != 0)
        FuneKanpan_SlotMode[0] = SceneData_SelectTableEntryByFlagGroup(0);
    else
        FuneKanpan_SlotMode[0] = 3;
    if (SceneState_FindFirstSetFlagOfGroup(2) != 0)
        FuneKanpan_SlotMode[1] = SceneData_SelectTableEntryByFlagGroup(2);
    else
        FuneKanpan_SlotMode[1] = 3;
    FuneKanpan_SlotMode[2] = 3;
    FuneKanpan_SlotMode[3] = 3;
    if (SceneState_FindFirstSetFlagOfGroup(1) != 0)
        FuneKanpan_SlotMode[4] = SceneData_SelectTableEntryByFlagGroup(1);
    else
        FuneKanpan_SlotMode[4] = 3;
    if (SceneState_FindFirstSetFlagOfGroup(3) != 0)
        FuneKanpan_SlotMode[5] = SceneData_SelectTableEntryByFlagGroup(3);
    else
        FuneKanpan_SlotMode[5] = 3;
    FuneKanpan_SlotMode[6] = 3;
    FuneKanpan_SlotMode[7] = 3;
}
