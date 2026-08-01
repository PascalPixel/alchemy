typedef unsigned char u8;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

extern u8 *Data_03001f30;

extern void Func_02004a24(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200492c(void *object, s32 arg1);
extern void Func_02004962(s32 arg0);
extern u8 *Func_020048f6(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void Func_02000c98(void)
{
    u8 *owner = Data_03001f30;
    u8 *object = *(u8 **)(owner + 16);
    u8 *replacement;
    u32 *dma3;
    u32 source;
    u32 destination;
    u32 control;

    Func_02004a24(*(s16 *)(owner + 24), 0x4000, 0);
    Func_0200492c(object, 0);
    Func_02004962(20);
    replacement = Func_020048f6(0, *(s32 *)(object + 8),
                                 *(s32 *)(object + 12), *(s32 *)(object + 16));
    if (replacement != 0) {
        dma3 = (u32 *)0x040000d4;
        source = (u32)object;
        destination = (u32)replacement;
        control = 0x8400001c;
        dma3[0] = source;
        dma3[1] = destination;
        dma3[2] = control;
        *(s32 *)(object + 108) = 0;
        *(u8 **)(owner + 16) = replacement;
        object[84] = 0;
    }
}
