typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct {
    u16 unused[4];
    u16 first;
    u16 padding;
    u16 second;
} State;

extern u32 Data_03001800;
extern State Data_03001ad0;
extern u8 Value_0000ffff;

void Func_080f03c0(void) {
    if ((Data_03001800 & 3) == 0) {
        State *state = &Data_03001ad0;
        u32 decrement = (u32)&Value_0000ffff;
        state->first += decrement;
        state->second += decrement;
    }
}
