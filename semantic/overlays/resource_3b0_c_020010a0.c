typedef unsigned short u16; typedef unsigned int u32;
extern u32 *Data_03001e70; extern u32 Data_02009940; extern u32 Data_02009928;
extern u32 Data_02009924; extern u32 Data_02009920;
extern u32 Func_08000120(u32); extern u32 Func_08000118(u32); extern u32 Func_080000f8(void);
/* Complete 144-byte randomised background-offset updater through its six-word pool. */
void Func_020010a0(void)
{
    u32 *pair=*(u32 **)*Data_03001e70;
    u32 dx=Func_08000120(Data_02009940), dz=Func_08000118(Data_02009928);
    pair[0]+=dx; pair[1]+=dz*4; Data_02009924+=dx; Data_02009920+=dz*4;
    Data_02009940 += (Func_080000f8()*384u)>>16;
    Data_02009928 = (Data_02009928 + (Func_080000f8()>>7)) & 0xffff;
    Data_02009940 = (u16)Data_02009940;
}
