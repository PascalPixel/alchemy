#include "types.h"

struct ObjectTableWork {
    u8 unknown_00[0x14];
    void *objects[192];
};

extern struct ObjectTableWork *Data_03001ebc;
void *Func_0808ba1c(s32);
void Object_Destroy(void *);

void ObjectTable_DestroyAtIndex(s32 index)
{
    struct ObjectTableWork *state = Data_03001ebc;
    void *object = Func_0808ba1c(index);

    if (object != NULL) {
        Object_Destroy(object);
        state->objects[index] = NULL;
    }
}
