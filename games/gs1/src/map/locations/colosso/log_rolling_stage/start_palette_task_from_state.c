#include "colosso_log_rolling_stage.h"

extern u16 Data_0200dba0;
extern u16 Data_0200dc40;
extern u16 Data_0200dc34;
extern u16 Data_0200dbe4;
extern u16 Data_0200dbc0;
extern u16 Data_0200dbfc;
extern u16 Data_0200dbcc;
extern u16 Data_0200db90;

void Func_02008330(s32 taskAddress, s32 frameBudget);

void ColossoLogRollingStage_StartPaletteTaskFromState(u32 first_value, u32 second_value, u32 mode)
{
    Data_0200dba0 = (u16)first_value;
    Data_0200dc40 = (u16)second_value;
    Data_0200dbe4 = Data_0200dc34;
    Data_0200dbfc = Data_0200dbc0;
    Data_0200dbcc = (u16)mode;
    Data_0200db90 = 0;

    {
        s32 budget = 0xc80;
        s32 task = 0x0200b91d;
        Func_02008330(task, budget);
    }
}
