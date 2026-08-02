typedef unsigned short u16;
typedef unsigned int u32;

/* Rotate seven adjacent palette entries once every eight frames. */
void Func_02002dcc(void)
{
    volatile u32 *frame_counter = (volatile u32 *)0x03001e40;

    if ((*frame_counter & 7) == 0) {
        volatile u16 *palette = (volatile u16 *)0x050000c2;
        u16 saved = palette[0];
        unsigned int index;

        palette[6] = saved;
        for (index = 0; index <= 5; index++)
            palette[index] = palette[index + 1];
    }
}
