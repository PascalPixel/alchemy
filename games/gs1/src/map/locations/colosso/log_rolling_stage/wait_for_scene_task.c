#include "colosso_log_rolling_stage.h"

extern s32 Data_0200d480;
extern s32 Data_0200d484;

extern u8 Value_0000000a;
void Func_02004a0e();
void Func_02004a1e();

void ColossoLogRollingStage_WaitForSceneTask(void)
{
    s32 polls;

    /* 素直な while ループ。goto 版では初回の読みがテストへ沈む。
     * A plain while loop. The goto-scaffolded version let gcc sink the first
     * read of Data_0200d480 into the test block, where the reference loads it
     * before the loop. And the frame count is a literal ten: the reference
     * emits `movs r0, #10`, which a Value_ symbol cannot produce. */
    Func_02004a0e(10);

    polls = 0;
    while (Data_0200d480 != 3 || Data_0200d484 != 1) {
        Func_02004a1e(1);
        polls++;
        if (polls > 119) {
            return;
        }
    }
}
