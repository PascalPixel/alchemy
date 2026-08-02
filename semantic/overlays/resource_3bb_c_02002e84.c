typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed short s16;
typedef signed int s32;

/*
 * Advance the active screen marker toward its destination, then publish its
 * three-word display command during the visible part of a 20-frame cycle.
 * This is the independently addressed resource_3bb member of the same
 * instruction family as resource_3ba:2bec and resource_3bc:391c.  All local
 * state addresses and all three callees below were resolved from this image.
 */
extern s16 Data_0200cb46;
extern u8 Data_03001b10[];
extern s16 Data_0200cc2c;
extern u32 Data_0200cc50[];
extern u16 Data_0200cbf0;
extern u16 Data_0200cc94;
extern s16 Data_0200cc44;
extern s16 Data_0200cc00;
extern u16 Data_0200cc20;
extern s16 Data_0200cca0;
extern u16 Data_0200cc5c;
extern s16 Data_0200cc14;
extern s16 Data_0200cbf8;

extern s32 Func_03000380(s32 value, s32 divisor);
extern void Func_080001e8(void *command, s32 opacity);

void Func_02002e84(void)
{
    s32 tile = *(u16 *)(Data_03001b10 + Data_0200cb46 * 4 + 2) >> 5;
    s16 duration = Data_0200cc2c;
    s16 phase;

    if (duration != 0) {
        s16 frame = (s16)(Data_0200cbf0 + 1);
        s32 delta;

        Data_0200cbf0 = (u16)frame;

        delta = Data_0200cc00 - Data_0200cc44;
        Data_0200cc94 = (u16)(Data_0200cc94 +
            Func_03000380(frame * delta, duration));

        delta = Data_0200cca0 - (s16)Data_0200cc5c;
        Data_0200cc20 = (u16)(Data_0200cc5c +
            Func_03000380(frame * delta, duration));

        if (frame >= duration)
            Data_0200cc2c = 0;
        Data_0200cc14 = 0;
    }

    phase = (s16)(Data_0200cc14 + 1);
    Data_0200cc14 = phase;

    if (phase <= 13) {
        u32 packed;

        Data_0200cc50[0] = 0;
        packed = (u32)((s32)(s16)Data_0200cc20 - 8);
        packed |= (u32)((s32)(s16)Data_0200cc94 - 8) << 16;
        packed |= 0x40000000;
        packed |= (u32)Data_0200cbf8 << 28;
        Data_0200cc50[1] = packed;
        Data_0200cc50[2] = (u32)tile | 0x400;
        Func_080001e8(Data_0200cc50, 255);
    } else if (phase > 19) {
        Data_0200cc14 = 0;
    }
}
