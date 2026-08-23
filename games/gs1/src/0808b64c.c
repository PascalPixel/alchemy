#include "object_lookup.h"
#include "types.h"

struct Fields_0808b64c {
    u8 filler[20];
    void *objects[1];
};

extern struct Fields_0808b64c *Data_03001ebc;

void Object_Destroy(void *);

void Func_0808b64c(s32 arg0) {
    struct Fields_0808b64c *state = Data_03001ebc;
    void *object = ObjectTable_Get(arg0);
    if (object != 0) {
        Object_Destroy(object);
        state->objects[arg0] = 0;
    }
}
