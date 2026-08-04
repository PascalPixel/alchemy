#include "types.h"

struct DmaTransfer_08097540 {
    const void *source;
    void *destination;
    u32 control;
};

struct Object_08097540 {
    u8 pad_000[651];
    s8 field_28b;
    s8 field_28c;
    s8 field_28d;
    u16 field_28e;
    u16 field_290;
    u16 field_292;
    u8 field_294;
};

extern s32 Data_03001e40;

struct Object_08097540 *Func_080048f4(s32 kind, s32 size);
void Func_08097384(void);
s32 Func_08002304(s32 value, s32 limit);
void Func_080978c4(void);
void Func_08091200(s32 value, s32 mode);
void Func_08091254(s32 mode);
void Func_08097a7c(void);
void Func_080041d8(const void *callback, s32 period);

void Func_08097540(s32 arg0, s32 arg1)
{
    struct Object_08097540 *object;
    u32 zero;
    u32 *source;
    struct DmaTransfer_08097540 *dma;
    s32 value;

    object = Func_080048f4(22, 664);
    Func_08097384();
    source = &zero;
    *source = 0;
    dma = (struct DmaTransfer_08097540 *)0x040000d4;
    dma->source = source;
    dma->destination = object;
    dma->control = 0x850000a6;
    object->field_28e = Func_08002304(Data_03001e40 * 2, 360);
    Func_080978c4();
    value = (object->field_28d << 10) | (object->field_28c << 5)
          | object->field_28b | 0x200000;
    Func_08091200(value, 1);
    Func_08091254(8);
    object->field_290 = arg0;
    object->field_292 = arg1;
    object->field_294 = 8;
    Func_08097a7c();
    Func_080041d8((const void *)0x08097645, 0xc80);
}
