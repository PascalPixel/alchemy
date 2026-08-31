#include "colosso_log_rolling_stage.h"

extern s32 Func_02005504(void);
extern void Func_020058ac(void);

void ColossoLogRollingStage_RunSceneEventIfReady(void)
{
    if (Func_02005504() == 0) {
        Func_020058ac();
    }
}
