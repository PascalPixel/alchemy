typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct State_0807842c {
    u8 padding[296];
    volatile u8 shift;
};

struct Entry_0807842c {
    u8 padding[4];
    u16 flags;
};

struct State_0807842c *Func_08077394(s32);
struct Entry_0807842c *Func_08078414(s32);

s32 Func_0807842c(s32 owner, s32 index)
{
    struct State_0807842c *state = Func_08077394(owner);
    struct Entry_0807842c *entry = Func_08078414(index);
    u32 amount = state->shift;
    s32 result = entry->flags;

    if (amount > 7) {
        result = 0;
    } else {
        result >>= state->shift;
        result &= 1;
    }
    return result;
}
