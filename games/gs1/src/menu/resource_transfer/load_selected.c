#include "types.h"
#include "resource.h"

struct SelectionNode_0801c188 {
    u8 padding_00[8];
    u16 no;
    u16 type;
};

struct TransferState_0801c188 {
    u8 padding_00[8];
    u16 no;
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
struct ResourceBuffer_0801c188 *Runtime_AllocateHeapBlock(s32 owner, s32 size);
void Func_080053e8(void *source, void *destination);
u16 Resource_FindFreeSlot(void);
u16 Resource_CopyData(s32 handle, s32 size, void *buffer);
void Func_08002dd8(s32 owner);

void Menu_LoadSelectedResource(void)
{
    u8 *state = Data_03001e98;
    struct SelectionNode_0801c188 *selection = Func_0801b36c(state);
    struct TransferState_0801c188 *transfer;
    struct ResourceBuffer_0801c188 *buffer;
    u8 *tbl;
    void *resource;
    s32 no;

    if (selection->type != 1 && selection->type != 6)
        return;

    buffer = Runtime_AllocateHeapBlock(17, 0x608);
    transfer = (struct TransferState_0801c188 *)(state + 0x30C);
    no = selection->no;
    tbl = GetResource((s32)&Value_000000f1);
    {
        void **destination = &buffer->resource;
        resource = tbl
            + *(u16 *)(tbl + selection->no * 2);
        *destination = resource;
    }
    Func_080053e8(resource, buffer);

    if (transfer->active == 0)
        transfer->handle = Resource_FindFreeSlot();
    transfer->transfer_id =
        Resource_CopyData(transfer->handle, 0x400, buffer);
    transfer->active = 1;
    transfer->no = no;
    transfer->x = 40;
    transfer->y = 40;
    transfer->width = 240;
    Func_08002dd8(17);
}
