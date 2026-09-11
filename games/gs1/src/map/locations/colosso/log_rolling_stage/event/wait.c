#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/event/wait.h"
#include "colosso_log_rolling_stage.h"

extern s32 gOv;
extern s32 gOv2;

extern u8 Value_0000000a;

void Colosso_WaitForSceneTask(void)
{
    s32 polls;

    /* 素直な while ループ。goto 版では初回の読みがテストへ沈む。
     * A plain while loop. The goto-scaffolded version let gcc sink the first
     * read of gOv into the test block, where the reference loads it
     * before the loop. And the frame count is a literal ten: the reference
     * emits `movs r0, #10`, which a Value_ symbol cannot produce. */
    Map_Run(10);

    polls = 0;
    while (gOv != 3 || gOv2 != 1) {
        Map_Run2(1);
        polls++;
        if (polls > 119) {
            return;
        }
    }
}
