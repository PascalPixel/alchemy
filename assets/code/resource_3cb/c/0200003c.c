typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

u32 Func_02001796(void);
void Func_0200179a(void);

u32 Func_0200003c(void) {
    volatile u16 *interruptMaster = (volatile u16 *)0x04000208;
    u32 savedInterruptMaster = *interruptMaster;
    u32 result;

    *interruptMaster = (u16)(u32)interruptMaster;
    Func_0200179a();
    result = Func_02001796();
    *interruptMaster = savedInterruptMaster;
    return result;
}
