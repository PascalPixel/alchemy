/* Choose each drifting slot's mode from the first set flag of its group,
   or mode 3 when none is set. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KANPAN.H"

/* The slots' modes, uninitialised work after the deck's block. */
s32 FuneKanpan_SlotMode[8];
s32 SceneState_FindFirstSetFlagOfGroup(u32 group);
s32 SceneData_SelectTableEntryByFlagGroup(u32 group);

union Slot {
    s32 w;
    s16 h[2];
};

extern s32 FuneKanpan_FlagGroupEntries[];
extern u8 LinkedMessage_TheresNothingWeCanDo[];
s32 BuildMotionCountdown(s32, s16);

extern u16 FuneKanpan_SlotPhase[];
extern u16 FuneKanpan_SlotValue[];
void SceneEffect_AdvanceSlotByValueBand(s32 actor, s32 slot);
void SceneEffect_SelectSlotValueAndPosition(s32 actor, s32 slot, s32 row);

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

s32 SceneData_SelectTableEntryByFlagGroup(u32 sel)
{
    s32 base = 0;
    u32 i;

    switch (sel) {
    case 0:
        base = 0x92C;
        break;
    case 1:
        base = 0x935;
        break;
    case 2:
        base = 0x917;
        break;
    case 3:
        base = 0x990;
        break;
    }
    for (i = 0; i < 9; i++) {
        if (GameFlag_IsSet(base + i) != 0) return FuneKanpan_FlagGroupEntries[i];
    }
    return 0;
}

void SceneState_ConfigureEntries8Through19(void)
{
    SceneEffect_AdvanceSlotByValueBand(8, 0);
    FuneKanpan_SlotPhase[0] = 0;
    SceneEffect_AdvanceSlotByValueBand(9, 1);
    FuneKanpan_SlotPhase[1] += 0x80;
    SceneEffect_AdvanceSlotByValueBand(10, 2);
    FuneKanpan_SlotPhase[2] += 0x100;
    SceneEffect_AdvanceSlotByValueBand(11, 3);
    FuneKanpan_SlotPhase[3] += 0x200;
    SceneEffect_SelectSlotValueAndPosition(12, 0, 0);
    SceneEffect_SelectSlotValueAndPosition(13, 1, 0);
    SceneEffect_SelectSlotValueAndPosition(14, 2, 0);
    SceneEffect_SelectSlotValueAndPosition(15, 3, 0);
    SceneEffect_SelectSlotValueAndPosition(16, 4, 1);
    SceneEffect_SelectSlotValueAndPosition(17, 5, 1);
    SceneEffect_SelectSlotValueAndPosition(18, 6, 1);
    SceneEffect_SelectSlotValueAndPosition(19, 7, 1);
}

void SceneEffect_AdvanceSlotByValueBand(s32 actor, s32 slot)
{
    u16 value = FuneKanpan_SlotValue[slot];

    if (value >= 0x6801 && value <= 0x6fff) {
        FuneKanpan_SlotPhase[slot] += 0x70;
        Engine_ActorSetAnimation(actor, 3);
    } else if (value >= 0xe801 && value <= 0xefff) {
        FuneKanpan_SlotPhase[slot] += 0xe0;
        Engine_ActorSetAnimation(actor, 3);
    } else if (value >= 0x7001 && value <= 0xefff) {
        FuneKanpan_SlotPhase[slot] += 0x1c0;
        Engine_ActorSetAnimation(actor, 2);
    } else {
        FuneKanpan_SlotPhase[slot] += 0x300;
        Engine_ActorSetAnimation(actor, 1);
    }
}
