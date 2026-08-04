typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/* Start an interpolated marker move from the current position. */
extern u16 Data_0200cc00;
extern u16 Data_0200cca0;
extern u16 Data_0200cc94;
extern u16 Data_0200cc44;
extern u16 Data_0200cc20;
extern u16 Data_0200cc5c;
extern u16 Data_0200cc2c;
extern u16 Data_0200cbf0;

/* Per-site veneer (raw sub_ symbol from the overlay .s). */
extern void Func_02006df8(s32 task_address, s32 frame_budget);

void Func_02003024(u32 x, u32 y, u32 duration)
{
    Data_0200cc00 = (u16)x;
    Data_0200cca0 = (u16)y;
    Data_0200cc44 = Data_0200cc94;
    Data_0200cc5c = Data_0200cc20;
    Data_0200cc2c = (u16)duration;
    Data_0200cbf0 = 0;

    {
        s32 frame_budget = 0xc80;
        Func_02006df8(0x0200ae85, frame_budget);
    }
}
