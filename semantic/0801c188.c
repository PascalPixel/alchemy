#include "types.h"

struct SelectionNode_0801c188 {
    u8 padding_00[8];
    u16 resource_index;
    u16 type;
};

struct TransferState_0801c188 {
    u8 padding_00[8];
    u16 resource_index;
    u16 active;
    u16 handle;
    u16 transfer_id;
    u8 padding_10[18];
    s16 x;
    s16 y;
    s16 width;
};

struct ResourceBuffer_0801c188 {
    u8 payload[0x604];
    void *resource;
};

extern u8 *Data_03001e98;
extern u8 Value_000000f1;

struct SelectionNode_0801c188 *Func_0801b36c(void *state);
struct ResourceBuffer_0801c188 *Func_080048b0(s32 owner, s32 size);
u8 *Func_08002f40(s32 resource_group);
void Func_080053e8(void *source, void *destination);
u16 Func_08004080(void);
u16 Func_08003fa4(s32 handle, s32 size, void *buffer);
void Func_08002dd8(s32 owner);

void Func_0801c188(void)
{
    u8 *state = Data_03001e98;
    struct SelectionNode_0801c188 *selection = Func_0801b36c(state);
    struct TransferState_0801c188 *transfer;
    struct ResourceBuffer_0801c188 *buffer;
    u8 *resource_table;
    void *resource;
    s32 resource_index;

    if (selection->type != 1 && selection->type != 6)
        return;

    buffer = Func_080048b0(17, 0x608);
    transfer = (struct TransferState_0801c188 *)(state + 0x30C);
    resource_index = selection->resource_index;
    resource_table = Func_08002f40((s32)&Value_000000f1);
    resource = resource_table
        + *(u16 *)(resource_table + selection->resource_index * 2);
    buffer->resource = resource;
    Func_080053e8(resource, buffer);

    if (transfer->active == 0)
        transfer->handle = Func_08004080();
    transfer->transfer_id =
        Func_08003fa4(transfer->handle, 0x400, buffer);
    transfer->active = 1;
    transfer->resource_index = resource_index;
    transfer->x = 40;
    transfer->y = 40;
    transfer->width = 240;
    Func_08002dd8(17);
}
