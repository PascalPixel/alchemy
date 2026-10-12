/* Near miss, differ 4: at the loop end the native forms the party count address, then
 * increments the index, then loads the count; this draft increments first. The load and
 * the increment tie in the scheduler (see 080df820.c). The barrier below already fixes
 * the first pair. */
#include "types.h"
#include "ram_buffer.h"

struct ItemMenuIcons {
    u8 unknown_000[0xf8];
    void *owner_icons[8];
    u8 unknown_118[0xfd];
    u8 party_count;
};

void Animation_ApplyChildArgumentFar(void *object, s32 argument);
void Scheduler_RemoveCallback(void (*callback)(void));
void Func_080fabe0(void);

void Func_080fac58(void)
{
    struct ItemMenuIcons *menu = Ram_HeapSlots->menu_runtime;
    s8 i;

    /* FAKEMATCH: the barrier loads the menu work before the party count offset is started; without it the offset is started first (one reordered pair, differ 2). */
    asm volatile("");
    for (i = 0; i < menu->party_count; i++)
        Animation_ApplyChildArgumentFar(menu->owner_icons[i], 1);
    Scheduler_RemoveCallback(Func_080fabe0);
}
