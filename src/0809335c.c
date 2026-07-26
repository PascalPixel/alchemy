#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_08009128(void);
s32 Func_0808ba1c(u32 arg0);
void *Func_080048f4(s32 arg0, s32 arg1);
void Func_080090e0(void *, void *);
void Func_080030f8(s32);

void Func_0809335c(s32 arg0, s32 arg1) {
    s32 temp_r6;
    void *temp_r5;
    void *temp_r8;
    s32 *ptr;

    temp_r6 = Func_0808ba1c(arg0);
    temp_r8 = Func_080048f4(0x1B, 0xCCC);
    temp_r5 = M2C_FIELD(temp_r8, void **, 0x1E0);
    ptr = *(s32 **)0x03001E70;
    if (temp_r6 != 0) {
        *ptr = (s32)((u8 *)temp_r5 + 8);
        Func_080090e0(temp_r5, (void *)temp_r6);
        if (arg1 == 0) {
            M2C_FIELD(temp_r5, s32 *, 8) = (s32) M2C_FIELD(temp_r6, s32 *, 8);
            M2C_FIELD(temp_r5, s32 *, 0xC) = (s32) M2C_FIELD(temp_r6, s32 *, 0xC);
            M2C_FIELD(temp_r5, s32 *, 0x10) = (s32) M2C_FIELD(temp_r6, s32 *, 0x10);
            Func_080030f8(1);
            if (M2C_FIELD(temp_r8, s16 *, 0x19E) != 3) {
                Func_08009128();
            }
        }
    }
}
