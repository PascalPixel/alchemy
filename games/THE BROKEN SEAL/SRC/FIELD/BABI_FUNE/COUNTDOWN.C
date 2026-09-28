#include "TYPES.H"

extern s32 BabiFune_CountTicks;
extern s32 BabiFune_Count;

/* Every forty ticks, count down while the count is above four. */
void SceneState_CountDownEveryFortyTicks(void)
{
    s32 n = BabiFune_CountTicks + 1;

    BabiFune_CountTicks = n;
    if (n == 40) {
        if (BabiFune_Count > 4) {
            BabiFune_Count -= 1;
            BabiFune_CountTicks = 0;
        }
    }
}
