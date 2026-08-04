#include "types.h"

struct Object_080a9c18 {
    u8 padding0[6];
    u16 value1;
    u16 value2;
};

struct State_080a9c18 {
    u8 padding0[72];
    struct Object_080a9c18 *objects[32];
};

struct Entry_080a9c18 {
    u8 padding0[2];
    u8 kind;
};

extern struct State_080a9c18 *Data_03001f2c;

void Func_080a9cbc(void);
struct Entry_080a9c18 *Func_08077018(s32);
void Func_080a17c4(struct Object_080a9c18 *);

void Func_080a9c18(u16 *entries)
{
    struct State_080a9c18 *state;
    struct Object_080a9c18 **slot;
    s32 remaining;
    s32 value1;

    state = Data_03001f2c;
    Func_080a9cbc();
    remaining = 14;
    value1 = 216;
    slot = state->objects;

    do {
        s32 entry = *entries++;

        if (entry != 0 && (entry & 0x200) != 0) {
            struct Object_080a9c18 *object = *slot;

            if (object != 0) {
                struct Entry_080a9c18 *data;
                s32 value2;

                data = Func_08077018(entry & 0x1FF);
                switch (data->kind) {
                case 1:
                    value2 = 32;
                    break;
                case 2:
                    value2 = 80;
                    break;
                case 3:
                    value2 = 64;
                    break;
                case 4:
                    value2 = 48;
                    break;
                default:
                    goto update;
                }
                object->value1 = value1;
                object->value2 = value2;
update:
                Func_080a17c4(object);
            }
        }
        remaining--;
        slot++;
    } while (remaining >= 0);
}
