typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

struct Approach390Subject {
    u16 unknown_00[3];
    u16 position;                   /* 0x06, wrapped 16-bit */
};

extern struct Approach390Subject *Func_0200041a(s32);
extern void Func_02000408(void);
extern void Func_02000452(s32, s32);
extern void Func_0200044a(s32);
extern void Func_0200045a(s32, s32);
extern void Func_02000436(void);

void Func_0200012c(void)
{
    /* The local is wider than the field: read into a u16 the compiler reloads
     * it signed and re-normalises across the call. */
    u32 position = Func_0200041a(0)->position;

    Func_02000408();

    if (position + 0xFFFF5FFF <= 0x3FFE) {
        Func_02000452(15, 18);
    } else {
        Func_0200044a(0x16B1);
        Func_0200045a(18, 0);
    }

    Func_02000436();
}
