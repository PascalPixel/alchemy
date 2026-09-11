#include "types.h"
#include "scene.h"
#include "abi/ui/count_second_table_entries.h"

extern u8 gRom[];
extern u8 gRom2[];

s32 Ui_CountSecondTableEntries(void)
{
    return (gRom - gRom2) >> 2;
}
