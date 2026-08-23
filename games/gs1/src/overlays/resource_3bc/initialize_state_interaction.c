#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000008f;
extern u8 Value_00000090;
extern u8 Value_00002076;
extern u8 Value_00002078;
extern u8 Value_0000207a;

extern void Func_020074d2(s32, s32);
extern void Func_02007632(s32);
extern void Func_0200764a(s32, s32);

void Func_02002b50(s32 a, s32 b)
{
    s32 v;
    s32 id;

    Func_020074d2(b, 5);
    v = Data_02000240[224];
    if (v == (s32)&Value_0000008f) {
        id = (s32)&Value_00002076;
    } else if (v == (s32)&Value_00000090) {
        id = (s32)&Value_00002078;
    } else {
        id = (s32)&Value_0000207a;
    }
    Func_02007632(id + 1);
    Func_0200764a(a, 0);
}
