#include "types.h"

typedef struct { s32 unk0; s32 unk4; s32 unk8; } Desc;
typedef struct { u8 filler0[0x28]; s16 *unk28; } Sub;
typedef struct {
    u8 filler0[6]; u16 unk6; s32 unk8; s32 unkC; s32 unk10;
    u8 filler14[0x10]; s32 unk24; u8 filler28[4]; s32 unk2C;
    u8 filler30[8]; s32 unk38; u8 filler3C[4]; s32 unk40;
    u8 filler44[0xC]; Sub *unk50;
} Ent;

extern s32 Data_0200ad68[];
extern s32 Data_0200ada8[];
extern Ent *Func_02000342(Desc *, Ent *);
extern s32 Func_02002f06(Ent *, Desc *);

s32 Func_020002a8(Ent *a)
{
    Desc d;
    u32 idx;
    s32 m;
    Ent *r;

    idx = a->unk6 >> 12;
    m = Data_0200ad68[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    r = Func_02000342(&d, a);
    if (r != 0) {
        u32 i = 0;
        s32 v = *r->unk50->unk28;
        s32 *p = Data_0200ada8;

        do {
            if (v == *p++) goto done;
            i++;
        } while (i <= 5);
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
    m = Data_0200ad68[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    if (Func_02002f06(a, &d) > 0) {
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
done:
    return 0;
}
