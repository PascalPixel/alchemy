#include "types.h"
#include "animspr.h"
#include "ram_buffer.h"

// Takes the first free animation entry of the 64 and clears it for use. With
// none free it clears the entry at address 0.
struct AnimationEntry *Func_0815b24c(void)
{
    struct AnimationEntry *entry = Ram_HeapSlots->unknown_0c[0];
    struct AnimationEntry *found = NULL;
    s32 i = 0;
    /* FAKEMATCH: the script field's zero held from the start lives longest,
     * so the word zero takes r3 and the byte zero the pool load in r2 as the
     * native does; stored as plain NULLs the two zeros trade registers
     * (differ 24). */
    void *none = NULL;

    while (i <= 63) {
        if (entry->kind == 0) {
            found = entry;
            break;
        }
        i++;
        entry++;
    }
    found->anim_id = 0;
    found->scripts = NULL;
    found->frames = NULL;
    found->script = none;
    found->mode = 0;
    found->frame = 0;
    found->pos = 0;
    found->kind = 1;
    found->param = 0;
    return found;
}
