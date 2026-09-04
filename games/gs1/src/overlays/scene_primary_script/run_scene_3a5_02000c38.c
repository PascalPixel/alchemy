#include "types.h"

#define FieldScene_RunScene3a5_02000c38 Func_02000c38

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0000005b[];
extern u8 Data_02000240[];
void Func_02002a04();
void Func_02002a54();
void Func_02002a5e();

void FieldScene_RunScene3a5_02000c38(void)
{
    Func_02002a04(8, 2);
    Func_02002a54((s32)Data_0000005b, 5);
    do {
        Data_02000240[0x22b] = 3;
    } while (0);
    Func_02002a5e(53, 5);
}
