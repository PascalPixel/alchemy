#include "RUNTIME_MEM.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "PALQUEUE.H"

/* Takes heap slot 28 for the palette cycle queue and clears its four
   cycles; ⚓️ passes the slot's byte offset. */
void Runtime_AllocateAndClearQueue(void)
{
    struct PaletteSnapshotWork *queue;
    struct PaletteSnapshot *entry;
    u16 i;
    u16 j;

    queue = Runtime_AllocateBlock(112, sizeof(struct PaletteSnapshotWork));
    entry = queue->entries;
    for (i = 0; i != 4; i++) {
        entry->source = 0;
        entry->position = 0;
        entry->timer = 0;
        entry->delay = 0;
        entry->count = 0;
        for (j = 0; j != 16; j++) {
            entry->colors[j] = 0;
        }
        entry++;
    }
    queue->count = 0;
}
