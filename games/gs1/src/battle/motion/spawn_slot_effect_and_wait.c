#include "types.h"
#include "scene.h"
#include "abi/battle/motion/spawn_slot_effect_and_wait.h"


s32 WaitFrames(s32);
void *Battle_Run(s32, s32);

void **GetBattleObjectSlot();

void BattleMotion_SpawnSlotEffectAndWait(void)
{
    void *object;
    void *effect;

    object = *GetBattleObjectSlot();
    if ((object != NULL) && ((0xF & FIELD_AT_OFFSET(object, u8 *, 0x54)) == 1)) {
        effect = Battle_Run(FIELD_AT_OFFSET(object, s32 *, 0x50), 0x11B);
        if (effect != NULL) {
            Battle_Apply(effect, 1);
            FIELD_AT_OFFSET(effect, s8 *, 6) = 3;
        }
        WaitFrames(0xA);
    }
}
