#include "colosso_log_rolling_stage.h"


extern u16 Data_0200dc34;
extern u16 Data_0200dbc0;
extern u16 Data_0200db98;
extern u16 Data_0200dbb4;
extern u16 Data_0200dbcc;

void Func_02007362();          /* sibling item-28 owner, via per-site veneer */
void Func_020082d4(s32 taskAddress, s32 frameBudget);

void ColossoLogRollingStage_StartPaletteTask(u32 first_value, u32 second_value, u32 mode)
{
    Func_02007362(first_value, second_value, mode);

    Data_0200dc34 = (u16)first_value;
    Data_0200dbc0 = (u16)second_value;
    Data_0200db98 = (u16)(mode & 3);
    Data_0200dbb4 = 0;
    Data_0200dbcc = 0;

    {
        s32 budget = 0xc80;
        s32 task = 0x0200b91d;
        Func_020082d4(task, budget);
    }
}
