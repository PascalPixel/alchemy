typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern s32 Func_08077040(s32 value);
extern s32 Func_08077038(s32 group, s32 value);
extern u8 *Func_08077008(s32 group);

void Func_0200172c(s32 value, u16 setting)
{
    s32 group = Func_08077040(value);

    if (group != -1) {
        s32 slot = Func_08077038(group, value);

        if (slot != -1) {
            u8 *record = Func_08077008(group);
            *(u16 *)(record + 216 + slot * 2) = setting;
        }
    }
}
