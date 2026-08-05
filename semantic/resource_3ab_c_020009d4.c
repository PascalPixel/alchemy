#include "types.h"
extern u8 *Data_03001ebc;
extern s32 Func_080770c0(s32); extern void Func_080770c8(s32);
extern void Func_0808a470(void); extern void Func_0808a148(s32,s32,s32);
extern void Func_0808a1e8(s32,s32,s32); extern void Func_0808a170(s32);
extern void Func_0808a180(s32,s32); extern void Func_0808a130(s32,s32);
void Func_020009d4(void)
{
    if (*(s16 *)(0x02000240 + 588) == 0 &&
        Func_080770c0(0x941) != 0 && Func_080770c0(0x94d) == 0) {
        if (Func_080770c0(0x9af) == 0) {
            Func_0808a470();
            *(u16 *)(*(u8 **)Data_03001ebc + 382) = 0;
            Func_0808a148(8, 0, 0);
            Func_0808a1e8(8, 0x101, 60);
            Func_0808a170(0x24db);
            Func_080770c8(0x9af);
        } else {
            Func_0808a170(0x24e7);
        }
        Func_0808a180(8, 0);
        Func_0808a130(8, 1);
        Func_0808a170(0x24dc);
    } else {
        Func_0808a170(0x1bbf);
    }
    Func_0808a180(8, 0);
}
