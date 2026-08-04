#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

struct Runtime_080b7b30 {
    void *object;
    u8 padding_004[0x1c];
    s32 field_20;
    s32 field_24;
};

struct Object_080b7b30 {
    u8 padding_000[0x50];
    void *field_50;
    s8 field_54;
};

void *Func_080b7dd0(s32 arg0);
void Func_08009038(void *);
void *Func_080b7f70(struct Fields_080b7f70 *arg0, s32 arg1);
void Func_080b7b30(s32 arg0) {
    struct Runtime_080b7b30 *runtime;
    struct Object_080b7b30 *obj;
    s32 i;
    void *r;

    runtime = Func_080b7dd0(arg0);
    if (runtime != NULL) {
        obj = runtime->object;
        if (obj != NULL) {
            runtime->field_20 = 0;
            runtime->field_24 = 0;
            i = 0;
            while ((r = Func_080b7f70(obj, i)) != NULL) { Func_08009038(r); i += 1; }
            obj->field_54 = (s8) r;
            obj->field_50 = r;
        }
    }
}
