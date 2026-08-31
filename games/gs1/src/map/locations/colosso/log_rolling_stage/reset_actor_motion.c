#include "colosso_log_rolling_stage.h"


u8 *Func_020082f8();           /* scene-record accessor, established (veneer to Scene_GetRecord) */
void Func_020081e6();          /* unestablished */

void ColossoLogRollingStage_ResetActorMotion(s32 selector)
{
    u8 *record;

    record = Func_020082f8(selector);
    Func_020081e6();

    *(u32 *)(record + 36) = 0;
    *(u32 *)(record + 44) = 0;
    *(u32 *)(record + 56) = 0x80000000;
    *(u32 *)(record + 64) = 0x80000000;
}
