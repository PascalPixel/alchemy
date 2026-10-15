/* Near miss, differ 5 of 29: the native keeps the work address in r3 for the
   hook call and copies it to r5 (adds r5, r3, #0), then steps r5 to the first
   entry after the call (adds r5, #8); this draft loads the address straight
   into r5 and has no copy. Indexing VENT_ROW(i) inside the loop is further
   away (differ 34). */
#include "../../../../../games/tla/src/scene/aqua_rock_inside/local.h"

void Func_02003a70(void)
{
    struct VentWork *work = VENT_WORK;
    struct VentEntry *entry;
    s32 i;

    work->hook();
    entry = work->entries;
    for (i = 15; i >= 0; i--) {
        if (entry->active != 0) {
            if (entry->timer <= 0) {
                Func_020039f0(&entry->x, entry->value);
                entry->timer = 9;
            } else {
                entry->timer--;
            }
        }
        entry++;
    }
}
