typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

extern volatile u32 Data_03001ae8;
extern volatile u32 Data_03001b04;
extern u8 * volatile Data_03001e74;
extern volatile u32 Data_03001c94;

void Func_080030f8(s32);

s32 Func_080c0eec(s32 result)
{
    u8 *base;
    volatile u32 *buttons;

    if (Data_03001ae8 & 8) {
        buttons = &Data_03001b04;
loop:
        base = Data_03001e74;
        if (*buttons & 0x20)
            *(s32 *)(base + 0x828) -= 1;
        if (*buttons & 0x10)
            *(s32 *)(base + 0x828) += 1;
        if (*buttons & 0x40)
            *(s32 *)(base + 0x828) -= 100;
        if (*buttons & 0x80)
            *(s32 *)(base + 0x828) += 100;
        if (Data_03001c94 & 1) {
            result = *(s32 *)(base + 0x828);
            goto done;
        }
        Func_080030f8(1);
        goto loop;
    }
done:
    if (Data_03001ae8 & 4)
        result = 0x18f;
    return result;
}
