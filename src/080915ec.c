typedef unsigned char u8;
typedef signed int s32;

extern void *Data_03001ebc;
extern volatile u8 Data_03001f54;
extern volatile s32 Data_03001c94;

void Func_080915ec(void)
{
    void *runtime = Data_03001ebc;

    if (Data_03001f54 != 0) {
        if (Data_03001c94 & 0x200) {
            *(s32 *)((u8 *)runtime + 0x1CC) = 0;
        }
        if (Data_03001c94 & 0x100) {
            *(s32 *)((u8 *)runtime + 0x1CC) = -1;
        }
    }
}
