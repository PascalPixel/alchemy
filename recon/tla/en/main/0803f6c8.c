#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "RESOURCE.H"
#include "SYSTEM.H"

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

extern u8 *gResQueueWork;

struct SelectionNode_0801c188 *NodeChain_GetNodeAtCount(void *state);
void Resource_DecodeByteLz(void *source, void *destination);

void Menu_LoadSelectedResource(void)
{
    u8 *state = gResQueueWork;
    struct SelectionNode_0801c188 *selection = NodeChain_GetNodeAtCount(state);
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
    tbl = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
    {
        void **destination = &buffer->resource;
        resource = tbl
            + *(u16 *)(tbl + selection->no * 2);
        *destination = resource;
    }
    Resource_DecodeByteLz(resource, buffer);

    if (transfer->active == 0)
        transfer->handle = Resource_FindFreeEntry();
    transfer->transfer_id =
        VramBlock_LoadCached(transfer->handle, 0x400, buffer);
    transfer->active = 1;
    transfer->no = no;
    transfer->x = 40;
    transfer->y = 40;
    transfer->width = 240;
    Runtime_ReleaseHeapBlock(17);
}
