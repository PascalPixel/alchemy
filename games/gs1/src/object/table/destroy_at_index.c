#include "object_lookup.h"
#include "types.h"
#include "scene.h"

struct Fields_0808b64c {
    u8 filler[20];
    void *objects[1];
};

extern struct Fields_0808b64c *gWork;

void Object_Destroy(void *);

void ObjectTable_DestroyAtIndex(s32 arg0)
{
    struct Fields_0808b64c *state = gWork;
    void *object = ObjectTable_Get(arg0);
    if (object != 0) {
        Object_Destroy(object);
        state->objects[arg0] = 0;
    }
}
