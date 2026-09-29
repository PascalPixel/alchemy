/* The deck's slot actors 8 to 19: each of the four drifting slots takes a
   phase step by the band its value falls in, and the eight standing slots
   take their value and position. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

extern u16 FuneKanpan_SlotPhase[];
extern u16 FuneKanpan_SlotValue[];

void SceneEffect_AdvanceSlotByValueBand(s32 actor, s32 slot);
void SceneEffect_SelectSlotValueAndPosition(s32 actor, s32 slot, s32 row);

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
