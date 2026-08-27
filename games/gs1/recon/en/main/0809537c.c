#include "types.h"

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

extern s32 Data_02000240[];

void *Func_08092054(s32);
void Func_080916b0(void);
void Func_080030f8(s32);
void Func_080f9010(s32);
void Func_080925cc(s32, s32);
void Func_08092adc(s32, s32, s32);
void Func_08096bec(void *, s32, s32);
void Func_08092560(s32, s32, s32);
void Func_08009158(void *);
void *Func_080090c8(s32, s32, s32, s32);
void Func_08009240(void *, s32);
void Func_08009080(void *, s32);
void Func_080091e0(void *, s32);
s32 Func_08096c48(void *, s32);
s32 Func_08091750(void);

s32 Func_0809537c(s32 arg0)
{
    void *base;
    void *base2;
    void *field50;
    s32 style;
    s32 msg;
    s16 kind;
    s32 sel;
    s32 accum;
    void *link;
    void *list[8];
    void **list_ptr;
    s8 *style_flag_ptr;
    s32 i;

    base = Func_08092054(arg0);
    style = (M2C_FIELD(Func_08092054(Data_02000240[125]), u16 *, 6) + 0x2000) & 0xC000;
    Func_080916b0();
    Func_080030f8(0xA);
    Func_080f9010(0xAD);
    Func_080925cc(arg0, 1);
    Func_080f9010(0xAF);
    Func_080925cc(arg0, 1);
    msg = style + 0x8000;
    Func_080030f8(0x14);
    Func_08092adc(arg0, msg, 0);
    Func_080030f8(0xA);
    field50 = M2C_FIELD(base, void **, 0x50);
    M2C_FIELD(field50, u8 *, 9) = M2C_FIELD(field50, u8 *, 9) & -0xD;
    M2C_FIELD(base, u16 *, 6) = (u16)msg;
    base2 = Func_08092054(arg0);
    M2C_FIELD(base2, u8 *, 0x5A) = (u8)(0xFE & M2C_FIELD(base2, u8 *, 0x5A));
    style_flag_ptr = (s8 *)base + 0x55;
    *style_flag_ptr = 2;
    Func_08096bec(base, 0x100000, style);
    Func_080f9010(0x98);
    Func_08092560(arg0, 4, 0);
    Func_08009158(base);
    Func_08096bec(base, 0x100000, style);
    Func_080f9010(0x98);
    Func_08092560(arg0, 4, 0);
    Func_08009158(base);
    Func_08096bec(base, 0x100000, style);
    Func_080f9010(0x98);
    Func_08092560(arg0, 4, 0);
    Func_08009158(base);
    Func_080030f8(0x14);
    kind = *M2C_FIELD(M2C_FIELD(base, void **, 0x50), s16 **, 0x28);
    sel = 9;
    if (kind == 0x5A) {
        sel = 2;
    }
    if (kind == 0x5C) {
        sel = 0xA;
    }
    if (kind == 0x5B) {
        sel = 9;
    }
    accum = 0;
    list_ptr = list;
    link = base;
    for (i = 0; i <= 7; i++) {
        void *object = Func_080090c8(
            kind,
            M2C_FIELD(base, s32 *, 8),
            M2C_FIELD(base, s32 *, 0xC),
            M2C_FIELD(base, s32 *, 0x10));
        list_ptr[i] = object;
        if (object != NULL) {
            M2C_FIELD(object, s32 *, 0x1C) = 0xF000;
            M2C_FIELD(object, s32 *, 0x18) = 0xF000;
            M2C_FIELD(object, s8 *, 0x55) = 0;
            M2C_FIELD(object, s8 *, 0x23) = 2;
            M2C_FIELD(object, u8 *, 0x5A) = (u8)(M2C_FIELD(object, u8 *, 0x5A) | 1);
            M2C_FIELD(object, s32 *, 0x6C) = 0x08095349;
            {
                void *objfield50 = M2C_FIELD(object, void **, 0x50);
                M2C_FIELD(object, u16 *, 6) = (u16)M2C_FIELD(base, u16 *, 6);
                M2C_FIELD(objfield50, u8 *, 9) = (u8)(M2C_FIELD(objfield50, u8 *, 9) & ~0xC);
                Func_08009240(object, sel);
            }
            Func_08009080(object, 0);
            Func_080091e0(object, 0);
            accum = Func_08096c48(M2C_FIELD(object, void **, 0x50), accum);
            M2C_FIELD(object, void **, 0x68) = link;
            link = object;
        }
    }
    Func_08096bec(base, 0x400000, style + 0x8000);
    Func_080f9010(0x88);
    Func_08092560(arg0, 0xC, 0);
    Func_080030f8(0x18);
    *style_flag_ptr = 0;
    M2C_FIELD(base, s32 *, 0x24) = 0;
    M2C_FIELD(base, s32 *, 0x2C) = 0;
    M2C_FIELD(base, s32 *, 0x28) = 0;
    M2C_FIELD(base, s32 *, 0x38) = 0x80000000;
    M2C_FIELD(base, s32 *, 0x40) = 0x80000000;
    M2C_FIELD(base, s32 *, 0x3C) = 0x80000000;
    Func_08009080(base, 0);
    field50 = M2C_FIELD(base, void **, 0x50);
    M2C_FIELD(field50, u8 *, 9) = (u8)((-0xD & M2C_FIELD(field50, u8 *, 9)) | 8);
    return Func_08091750();
}
