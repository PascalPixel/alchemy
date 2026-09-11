#include "types.h"
#include "scene.h"
#include "abi/ui/icon/count_table_entries.h"

extern u8 gRom[];
extern u8 gRom2[];

/* 2つの境界アドレス間を4バイト単位で数える。 */
s32 Ui_CountIconTableEntries(void)
{
    return (s32)((u32)gRom - (u32)gRom2) >> 2;
}
