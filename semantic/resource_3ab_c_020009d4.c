#include "types.h"
extern u8 *Data_03001ebc;
 
 
 
 
extern s32 Func_020023aa(s32);
extern s32 Func_020023b4(s32);
extern s32 Func_020023be(s32);
extern void Func_0200252a(void);
extern void Func_0200247c(s32,s32,s32);
extern void Func_020024c6(s32,s32,s32);
extern void Func_0200249c(s32);
extern void Func_020023fa(s32);
extern void Func_020024aa(s32);
extern void Func_020024c2(s32,s32);
extern void Func_0200249a(s32,s32);
extern void Func_020024c0(s32);
extern void Func_020024c8(s32);
extern void Func_020024e0(s32,s32);
void Func_020009d4(void)
{
    if (*(s16 *)(0x02000240 + 588) == 0 &&
        Func_020023aa(0x941) != 0 && Func_020023b4(0x94d) == 0) {
        if (Func_020023be(0x9af) == 0) {
            Func_0200252a();
            *(u16 *)(*(u8 **)Data_03001ebc + 382) = 0;
            Func_0200247c(8, 0, 0);
            Func_020024c6(8, 0x101, 60);
            Func_0200249c(0x24db);
            Func_020023fa(0x9af);
        } else {
            Func_020024aa(0x24e7);
        }
        Func_020024c2(8, 0);
        Func_0200249a(8, 1);
        Func_020024c0(0x24dc);
    } else {
        Func_020024c8(0x1bbf);
    }
    Func_020024e0(8, 0);
}
