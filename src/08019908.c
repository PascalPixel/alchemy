typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

struct State_08019908 {
    u8 filler0[0x12bc];
    u32 values[8];
    u16 flags[8];
};

extern struct State_08019908 *Data_03001e8c;

/* Keep the constant loop bound live so the original compiler retains its register allocation. */
#define SLOT_COUNT(seed) (((seed) | ~(seed)) + 9)

void Func_08019908(u32 value, u32 flag)
{
    struct State_08019908 *state = Data_03001e8c;
    u32 index = 0;
    u32 limit = SLOT_COUNT(value);

    do {
        if (state->flags[index] == 0) {
            state->values[index] = value;
            state->flags[index] = flag;
            break;
        }
        index++;
    } while (index != limit);
}
