#include "TYPES.H"
#include "IO_REG.H"

s32 Scheduler_EnableCallbacks(u32 callback);
void Object_EffectSpawnCallback(void);

void Battle_UpdateModeFromShoulderButtons(void)
{
    struct BattleRuntime *runtime = Data_03001ebc;

    /*
     * 0x03001c94 is the button latch. 0x200 is L and 0x100 is R in the GBA
     * key layout, so the two arms are the shoulder buttons setting the mode
     * word either way round.
     */
    if (gDebugMode != 0) {
        if (gKeyState & KEY_L) {
            runtime->mode_1cc = 0;
        }
        if (gKeyState & KEY_R) {
            runtime->mode_1cc = -1;
        }
    }
}
