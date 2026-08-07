#include "types.h"

/*
 * resource_3ba owner at 0x02002ea0, 196 bytes, paired structurally with the
 * reviewed resource_3bc owner at 0x02003bd0.  When the selected scene entity
 * exists and its kind byte is 1, it rebuilds the entity handle's palette:
 * reserve a gradient workspace, DMA-zero 32 words, upload a new ramp, release
 * the previous handle, and merge the upload id into the packed field.
 *
 * The high-register prologue and 4-byte frame unwind through the sole
 * interworking return at 0x02002f54-0x02002f62.  The four-word literal pool is
 * branch-skipped at 0x02002f42-0x02002f53.  Two arguments (r0, r1), void.
 * Seven calls across six established targets match the sibling independently.
 * The alignment halfword at 0x02002e9e belongs to the preceding unknown owner
 * and is deliberately not claimed here.
 */

u8 *Func_0808a080();
s32 Func_08000140();
s32 Func_08015250();
s32 Func_080001c8();
void Func_08000150();
void Func_08009060();

void Func_02002ea0(s32 selector, s32 arg1)
{
    u8 *entity;
    u8 *handle;
    s32 kind;
    s32 gradient;
    s32 zeroWord;
    s32 uploadId;
    u16 merged;
    u32 previousHandle;

    entity = Func_0808a080(selector);
    if (entity == 0) {
        return;
    }

    kind = entity[0x54];
    if (kind != 1) {
        return;
    }

    handle = *(u8 **)(entity + 0x50);
    gradient = Func_08000140(17, 0xc1 << 3) + (0x80 << 3);

    *(volatile u32 *)0x040000dc = 0x85000020;
    zeroWord = 0;
    *(volatile u32 *)0x040000d8 = (u32)gradient;
    *(volatile u32 *)0x040000d4 = (u32)&zeroWord;

    Func_08015250(arg1);

    uploadId = Func_080001c8(handle[0x1c], 0x80, gradient);
    Func_08000150(17);

    entity[0x5c] = (u8)kind;

    previousHandle = *(u32 *)(handle + 0x28);
    *(u32 *)(handle + 0x28) = 0;
    handle[0x27] = 0;
    handle[5] = (u8)(handle[5] & ~0x20);
    Func_08009060(previousHandle);

    merged = (*(u16 *)(handle + 8) & 0xfc00) | (uploadId & 0x3ff);
    *(u16 *)(handle + 8) = merged;

    handle[0x25] = 0;
    handle[0x26] = 0;
}
