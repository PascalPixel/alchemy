#include "types.h"
#include "scene.h"
#include "abi/object/group/apply_random_child_values.h"

extern volatile s32 gIw;

void ObjectGroup_ApplyRandomChildValues(void *owner)
{
    s32 state;
    void *target;
    s32 initial_count;
    s32 count;
    void **entry;
    void *current;
    volatile s32 *global;
    s32 value;

    state = *(u8 *)((u8 *)owner + 84);
    /* 有効な所有物へ共有値を6で割った余りを配る。 */
    if (state == 1) {
        target = *(void **)((u8 *)owner + 80);
        if (target != 0 && (*(u8 *)((u8 *)target + 29) & state) == 0) {
            initial_count = *(u8 *)((u8 *)target + 39);
            if (initial_count != 0) {
                global = &gIw;
                entry = (void **)((u8 *)target + 40);
                count = initial_count;
                do {
                    current = *entry++;
                    value = Obj_Apply(*global, 6);
                    count--;
                    *(u8 *)((u8 *)current + 5) = value;
                } while (count != 0);
            }
            *(u8 *)((u8 *)target + 37) = 1;
        }
    }
}
