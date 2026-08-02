typedef unsigned short u16;
typedef unsigned int u32;

extern u32 Data_03001e40;
extern u16 Data_0200e7a0;
extern void Func_08000318(u32 address, u32 value);

/* Restore the blend registers using the active display bank's mask. */
void Func_020038fc(void)
{
    Func_08000318(0x04000050, 0x3f41);
    if ((Data_03001e40 & 2) != 0) {
        Func_08000318(0x04000052, Data_0200e7a0 | 0x0c);
    } else {
        Func_08000318(0x04000052, Data_0200e7a0 | 0x10);
    }
}
