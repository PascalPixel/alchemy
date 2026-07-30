typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];
extern u8 Value_00000067;

extern void Func_020004ae(void);

s32 Func_02000230(void)
{
    *(s32 *)(Data_03001ebc + 448) = 0x209;
    if (Data_02000240[224] == (s32) (u32) &Value_00000067) {
        Func_020004ae();
    }
    return 0;
}
