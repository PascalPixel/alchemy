/* Draft, differ 26 of 32, registers only: the shape agrees instruction for instruction, but the native keeps
 * the work in r2 and the index in r0, copied from the cleared slot pointer (movs r4, #0; adds r0, r4, #0);
 * this form keeps the work in r0 and clears the index in r1 by itself. Starting the index from the slot
 * pointer measures the same. */
#include "types.h"
#include "ram_buffer.h"

struct Slot02000630 {
    s16 kind;
    s16 unk02;
    s32 position;
    u8 unk08[8];
    s32 speed;
    u8 unk14[8];
};

struct Work02000630 {
    s32 unk00;
    struct Slot02000630 slots[8];
};

// 種類 8 の枠を探して進める
void Func_02000630(void)
{
    struct Work02000630 *work = Ram_HeapSlots->unk84[8];
    struct Slot02000630 *slot = NULL;
    s32 i;

    for (i = 0; i < 8; i++) {
        if (work->slots[i].kind == 8) {
            slot = &work->slots[i];
            break;
        }
    }
    slot->speed = 0x4000;
    slot->position += 128;
}
