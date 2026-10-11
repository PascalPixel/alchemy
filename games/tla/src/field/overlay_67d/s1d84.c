#include "types.h"
#include "ram_buffer.h"

#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

typedef s32 (*HandlerFn)(void);

void Func_02002294(s32, s32);
void Func_0200229c(s32, s32);
void Func_020022a4(void);
void Func_020022ac(void);
void Func_020022b4(void);
void Battle_WaitMode0(s32 frames);
void Field_DispatchTypeHandler(s32 type);

s32 Func_02001d84(void)
{
    return -1;
}

void Func_02001d8c(void)
{
    Func_02002294(140, 1);
    Func_0200229c(15, -1);
    FIELD_AT(Ram_HeapSlots->effect_work, HandlerFn, 36) = Func_02001d84;
    FIELD_AT(Ram_HeapSlots->effect_work, u8, 32) = 1;
    Func_020022b4();
    Battle_WaitMode0(10);
    Field_DispatchTypeHandler(0);
    Func_020022a4();
    Func_020022ac();
}
