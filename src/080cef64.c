typedef unsigned int u32;
typedef signed int s32;
typedef unsigned char u8;

s32 Func_080ed408(s32, s32, s32, s32, s32);
extern u8 Data_03001e50[];

void Func_080cef64(s32 alternate, u32 *output)
{
    if (alternate == 0) {
        u8 *state;
        u32 value;

        Func_080ed408(alternate = 46, 7, 7, 3, 2);
        state = Data_03001e50;
        value = *(u32 *)(state + 184);
        alternate = 47;
        output[0] = value;
        Func_080ed408(alternate, 7, 7, 3, 3);
        output[1] = *(u32 *)(state += 188);
    } else {
        u8 *state;
        u32 value;

        Func_080ed408(alternate = 46, 7, 7, 7, 2);
        state = Data_03001e50;
        value = *(u32 *)(state + 184);
        alternate = 47;
        output[0] = value;
        Func_080ed408(alternate, 7, 7, 7, 3);
        output[1] = *(u32 *)(state += 188);
    }
}
