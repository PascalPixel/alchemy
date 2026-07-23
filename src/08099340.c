typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;
typedef unsigned short u16;

struct Triple08099340 {
    s32 x;
    s32 y;
    s32 z;
};

extern s32 *Data_03001f30;
extern u8 Data_0809f0b0[];

void Func_0800447c(s32, s32, struct Triple08099340 *);
void Func_08009098(void *, void *);

void Func_08099340(u8 *arg)
{
    s32 *global = Data_03001f30;
    struct Triple08099340 local;
    s16 value;
    s32 raw;

    if (arg != 0) {
        raw = *(u16 *)(arg + 100) - 1;
        *(u16 *)(arg + 100) = raw;
        value = (s16)raw;
        if (value != 0) {
            local.x = global[1];
            local.y = global[2] + 0xA0000;
            local.z = global[3];
            Func_0800447c(value << 16,
                          *(s16 *)(arg + 102) + (value << 11),
                          &local);
            *(s32 *)(arg + 8) = local.x;
            *(s32 *)(arg + 12) = local.y;
            *(s32 *)(arg + 16) = local.z;
        } else {
            Func_08009098(arg, Data_0809f0b0);
        }
    }
}
