#include "colosso_log_rolling_stage.h"

extern u16 Data_0200dbd0;
extern u16 Data_0200dba4;
extern u16 Data_0200dbdc;
extern s32 Data_0200dbe0;
extern u16 Data_0200dc38;
extern u16 Data_0200dbac;
extern s32 Data_0200dbb0;

extern s32 Data_0200d9a6;   /* default handler (mode not 2/3/4) */
extern s32 Data_0200ccba;   /* handler for mode == 2 */
extern s32 Data_0200d9d2;   /* handler for mode == 4 */
extern s32 Data_0200cce2;   /* handler for mode == 3, parameter != 0 */
extern s32 Data_0200da50;   /* handler for mode == 3, parameter == 0 */

void Func_02007c38(s32 taskAddress, s32 frameBudget);

void ColossoLogRollingStage_InitializeModeTask(u32 mode, u32 parameter)
{
    s32 handler;

    Data_0200dbd0 = (u16)mode;
    Data_0200dba4 = (u16)(parameter << 4);

    {
        s32 budget = 0xc80;
        s32 task = 0x0200aee9;
        Func_02007c38(task, budget);
    }

    handler = (s32)&Data_0200d9a6;
    if (mode == 2) {
        handler = (s32)&Data_0200ccba;
    }
    if (mode == 4) {
        handler = (s32)&Data_0200d9d2;
    }
    if (mode == 3) {
        if (parameter != 0) {
            handler = (s32)&Data_0200cce2;
        } else {
            handler = (s32)&Data_0200da50;
        }
    }

    Data_0200dbdc = 0;
    Data_0200dbe0 = handler;
    Data_0200dc38 = 0;
    Data_0200dbac = 0;
    Data_0200dbb0 = 0;
}
