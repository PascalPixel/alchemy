#include "TYPES.H"

struct ObjectTableWork {
    u8 unknown_00[0x14];
    void *objects[192];
};

extern struct ObjectTableWork *gEventWork;
void *ObjectTable_Get(s32);
void Object_Destroy(void *);

void ObjectTable_DestroyAtIndex(s32 index)
{
    struct ObjectTableWork *state = gEventWork;
    void *object = ObjectTable_Get(index);

    if (object != NULL) {
        Object_Destroy(object);
        state->objects[index] = NULL;
    }
}
