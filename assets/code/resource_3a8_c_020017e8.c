#include "types.h"

extern s16 Data_02000240[];

extern s32 Func_020053c2(s32);
extern void Func_020053d4(s32);
extern void Func_020053da(s32);
extern void Func_020053e0(s32);
extern s32 Func_020053e6(s32);
extern void Func_020053f8(s32);
extern s32 Func_02005406(s32);
extern void Func_02003842(void);

void Func_020017e8(void) {
    if (Func_020053c2(0x941) != 0) {
        Func_020053d4(0x321);
        Func_020053da(0x913);
        Func_020053e0(0x912);
        Func_020053e6(0x915);
    }
    if (Func_020053e6(0x940) != 0) {
        Func_020053f8(0x321);
    }
    if (Data_02000240[225] != 0) {
        if (Func_02005406(0x912) == 0) {
            Func_02003842();
        }
    }
}
