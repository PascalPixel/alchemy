#include "types.h"

#define FieldScene_RunScene39aSequenceB Func_02001a34

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0000003f[];
extern u8 Data_00000040[];
extern s16 Data_02000240[];
void Func_02003530();
void Func_0200353c();
void Func_02003562();
s32 Func_02003d9e();

void FieldScene_RunScene39aSequenceB(void)
{
    u8 *work;
    s16 *table;
    s16 scene;

    work = *(u8 **)0x03001ebc;
    table = Data_02000240;
    scene = *(s16 *)(work + 0x16c);
    if (table[0xE0] == (s32)Data_0000003f) {
        if (scene == 17) {
            Func_02003530(0, -32);
        } else {
            Func_0200353c(-32, 0);
        }
    }
    if (table[0xE0] == (s32)Data_00000040) {
        if (scene == 25) {
            if (Func_02003d9e(0x309) != 0) {
                Func_02003562(0, 32);
            }
        }
    }
}
