typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern u8 *Func_08077008(s32 group);
extern s32 Func_08077028(s32 group, s32 value);
extern void Func_08077050(s32 group, s32 index);

/* Apply a value to every matching member of a fifteen-slot group. */
void Func_02000f8c(s32 group, s32 value)
{
    u8 *state = Func_08077008(group);
    s32 i;
    Func_08077028(group, value);
    for (i = 0; i < 15; i++) {
        if (*(u16 *)(state + 216 + i * 2) == value)
            Func_08077050(group, i);
    }
}
