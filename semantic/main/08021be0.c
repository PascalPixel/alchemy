typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

extern u8 Data_08015afc;
extern u8 Data_03001e50;
extern u8 Value_00000278;
extern u8 Value_00000604;

void *Func_080048b0(s32, u32);
void Func_080072f0(u32, void *, void *, u32);
void Func_08002dd8(s32);

void Func_08021be0(u8 *object)
{
    u32 size = (u32)&Value_00000278;
    void *buffer = Func_080048b0(49, size);
    u32 control = 0x84000000;
    u32 words = size >> 2;
    u32 *dma = (u32 *)0x040000d4;
    u32 source = (u32)&Data_08015afc;
    u32 destination = (u32)buffer;

    control |= words;
    dma[0] = source;
    dma[1] = destination;
    dma[2] = control;
    {
        u8 *header = (u8 *)&Data_03001e50;
        u32 *slot = (u32 *)(object + (u32)&Value_00000604);

        Func_080072f0(*slot, object, slot, *(u32 *)(header + 196));
    }
    Func_08002dd8(49);
}
