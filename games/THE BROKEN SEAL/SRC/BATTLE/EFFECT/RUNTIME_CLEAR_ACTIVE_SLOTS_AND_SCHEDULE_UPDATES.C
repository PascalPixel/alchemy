#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "EFFECT_0809B11C.H"

void ScheduleCallback(void *);
void Func_0809bb34(struct EffectSlot *);
#define BattleFx_ClearOwnedSlot Func_0809bb34
void Func_08002dd8(s32);
#define Runtime_ReleaseHeapBlock Func_08002dd8
void WaitFrames(s32);
extern u8 Data_08095885;

struct EffectScene {
    u8 unknown_00[0x58];
    struct EffectSlot slots[24];
};

void BattleFx_ClearActiveSlotsAndScheduleUpdates(void)
{
    struct EffectScene *scene = *(struct EffectScene **)ADDR_03001F30;
    struct EffectSlot *slot;
    s32 i;

    ScheduleCallback(&Data_08095885);
    for (i = 0; i < 24; i++) {
        slot = &scene->slots[i];
        if (slot->active != 0)
            BattleFx_ClearOwnedSlot(slot);
    }
    Runtime_ReleaseHeapBlock(56);
    WaitFrames(1);
}

void Func_0809592c(void *object)
{
    *(u16 *)((u8 *)object + 6) += 0x2000;
}
