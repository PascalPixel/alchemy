#include "types.h"

extern unsigned char *Data_03001ebc;

extern void Func_020012fc(void);
extern void Func_0200137a(s32);
/* 引数は u8 ではない。参照は 0x301 をプール語から渡す。
 * NOT u8. The reference loads a pool word into r0 before this call, so the
 * argument is the whole 0x301; declared u8 it truncated to 1 and the row
 * scored 7 differing halfwords, which read as a near-miss it is not. */
extern s32 Func_020012e8(s32);
extern void Func_020013a4(s32, s32);
extern void Func_02001312(s32);
extern void Func_02001336(void);

void Func_02000240(void)
{
    Func_020012fc();
    Func_0200137a(0x13ae);
    if (Func_020012e8(0x301)) {
        ((u16 *)Data_03001ebc)[236]++;
    }
    Func_020013a4(9, 0);
    Func_02001312(0x301);
    Func_02001336();
}
