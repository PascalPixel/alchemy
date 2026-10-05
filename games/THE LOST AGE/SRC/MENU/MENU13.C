#include "RUNTIME_MEM.H"
#include "GLYPH.H"
#include "SELECT.H"
#include "TYPES.H"
#include "RESOURCE.H"
#include "SYSTEM.H"
#include "RAM_BUFFER.H"
#include "NODE_CHAIN.H"

extern char ResourceId_CommandIcons;

void Resource_DecodeByteLzInRam(void *source, void *destination);

/* The head of ☀️'s MENU13.C over ⚓️'s heap: the selection screen in the
   menu work slot and the command icon transfer in heap block 68. */
void Menu_LoadSelectedResource(void)
{
    struct SelectionScreen *state = Ram_HeapSlots->menu_work;
    struct SelectionNode *selection = NodeChain_GetNodeAtCount(state);
    struct SelectionNode *transfer;
    GlyphTransfer *buffer;
    u8 *tbl;
    void *resource;
    s32 no;

    if (selection->kind != 1 && selection->kind != 6)
        return;

    buffer = (GlyphTransfer *)Runtime_AllocateHeapBlock(68, sizeof(GlyphTransfer));
    transfer = &state->records[15];
    no = selection->base;
    tbl = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
    {
        u8 **destination = &buffer->encoded;
        resource = tbl
            + *(u16 *)(tbl + selection->base * 2);
        *destination = resource;
    }
    Resource_DecodeByteLzInRam(resource, buffer);

    if (transfer->kind == 0)
        transfer->slot = Resource_FindFreeEntry();
    transfer->tile =
        VramBlock_LoadCached(transfer->slot, 0x400, buffer);
    transfer->kind = 1;
    transfer->base = no;
    transfer->scale = 40;
    transfer->scale_step = 40;
    transfer->scale_end = 240;
    Runtime_ReleaseHeapBlock(68);
}

void Resource_ResetPendingTransfer(void)
{
    struct SelectionNode *transfer =
        &((struct SelectionScreen *)Ram_HeapSlots->menu_work)->records[15];

    if (transfer->kind != 0) {
        Resource_ResetEntry(transfer->slot);
        transfer->kind = 0;
    }
}
