typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct GlobalData_0800eaf8 {
    u8 padding_000[0x210];
    u16 mask_210;
    u16 padding_212;
    u16 mask_214;
    u16 mask_216;
    u16 mask_218;
    u16 mask_21a;
    u8 padding_21c[4];
    u16 value_220;
    u16 value_222;
};

extern u16 *Data_03001ebc;
extern volatile u32 Data_03001c94;

u32 Func_0800ea60(u32);

s32 Func_0800eaf8(void)
{
    struct GlobalData_0800eaf8 *data = (void *)0x02000240;
    u16 *state = Data_03001ebc;
    s32 result = 0;
    s32 output = 0;
    s32 index;

    if (state != 0) {
        if (Data_03001c94 & data->mask_214) {
            index = 0xB9;
            goto store;
        }
        if (Data_03001c94 & data->mask_210) {
            index = 0xBA;
            goto store;
        }
        if (Data_03001c94 & data->mask_216) {
            index = 0xBB;
store:
            state[index] = 1;
            output = 1;
        } else if (Data_03001c94 & data->mask_218) {
            output = Func_0800ea60(data->value_220);
        } else if (Data_03001c94 & data->mask_21a) {
            output = Func_0800ea60(data->value_222);
        }
        result = output;
    }
    return result;
}
