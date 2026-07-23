typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

void Func_080fa68c(void *);

struct Object {
    u32 field_00;
    u32 field_04;
    u8 field_08;
    u8 pad_09[0x23];
    struct Entry *field_2c;
    u32 pad_30;
    u32 field_34;
    u32 field_38;
    u32 field_3c;
};

struct Entry {
    s8 value;
    u8 pad[0x4f];
};

struct State {
    u32 tag;
    u8 pad_04[0x1c];
    u32 callback;
    struct Object *current;
};

void Func_080fa9e0(struct Object *object, struct Entry *entries_arg, u32 count_arg)
{
    register struct Entry *entries = entries_arg;
    u8 count = count_arg;
    u32 *state;

    if (count == 0)
        return;
    if (count > 16)
        count = 16;

    state = *(u32 **)0x03007ff0;
    if (state[0] != 0x68736d53)
        return;

    state[0]++;
    Func_080fa68c(object);
    object->field_2c = entries;
    object->field_08 = count;
    object->field_04 = 0x80000000;

    if (count != 0) {
        do {
            u32 next;
            entries->value = 0;
            next = count - 1;
            count = next;
            entries++;
        } while (count != 0);
    }

    if (state[8] != 0) {
        object->field_38 = state[8];
        object->field_3c = state[9];
        state[8] = 0;
    }

    state[9] = (u32)object;
    state[8] = 0x080f9c91;
    state[0] = 0x68736d53;
    object->field_34 = 0x68736d53;
}
