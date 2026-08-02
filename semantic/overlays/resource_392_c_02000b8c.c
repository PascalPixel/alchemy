typedef unsigned int u32;

extern u32 Data_03001ed0;

/* Complete DMA3 descriptor writer; no calls or branches. */
void Func_02000b8c(void)
{
    *(volatile u32 *)0x040000d4 = Data_03001ed0;
    *(volatile u32 *)0x040000d8 = 0x020090b0;
    *(volatile u32 *)0x040000dc = 0x840000e0;
}
