#include "types.h"

extern u8 Data_0802e108[];
extern u8 Data_0802de88[];

s32 Ui_CountSecondTableEntries(void)
{
    return (Data_0802e108 - Data_0802de88) >> 2;
}
