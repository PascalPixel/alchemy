/* Draft, differ 0: overlay 699's Func_02000fd8 with this overlay's addresses, which needs its asm barrier (the native loads the list and its count after the timer store). Held back: the device ratchet (tla asm-barrier 197) may not rise. */
#include "types.h"
#include "field_event.h"

extern s16 Data_0200265e;
extern struct FieldActor *Data_02002660[];
extern s16 Data_02002762;

struct FieldActor *Object_GetById(s32 id);

void Func_02000f5c(s32 id, s32 value)
{
    struct FieldActor *object = Object_GetById(id);

    if (object != NULL) {
        s16 *count;

        object->timer = value;
        asm volatile(""); /* FAKEMATCH: ⚓️ loads the list and its count after the timer store */
        count = &Data_0200265e;
        Data_02002660[*count] = object;
        (*count)++;
    }
}
