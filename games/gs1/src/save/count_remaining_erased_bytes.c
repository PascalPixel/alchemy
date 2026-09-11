#include "scene.h"
#include "abi/save/count_remaining_erased_bytes.h"
#include "flash.h"

/*
 * A global holding a pointer.  Only its field 4, the byte budget, is read
 * here; the object is not otherwise identified.
 */
extern u8 *gOv;

/*
 * Count a run of consecutive 0xff bytes starting at ptr, capped by the budget
 * read from the global, and return the budget left when the scan stopped.
 * A leaf with no prologue: the owner runs to its bx lr plus one alignment
 * halfword, and its single pool word sits inline mid-body, skipped by the
 * branch that enters the loop at its test.
 */
s32 CountRemainingErasedFlashBytes(u8 *ptr)
{
    s32 remaining = *(s32 *)(gOv + 4);

    while (remaining != 0) {
        u8 byte = *ptr;
        ptr++;
        if (byte != 0xff) {
            break;
        }
        remaining--;
    }

    return remaining;
}
