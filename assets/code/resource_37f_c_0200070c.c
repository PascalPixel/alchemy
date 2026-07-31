typedef signed int s32;

extern s32 *Func_020022de(s32);
extern void Func_020022be(s32);
extern void Func_020022c4(s32);
extern void Func_020022c6(s32);
extern void Func_020022d4(s32);
extern void Func_02001f06(s32);

void Func_0200070c(void)
{
    s32 *record = Func_020022de(13);
    s32 heading;

    if (record == 0) {
        return;
    }
    heading = record[2] >> 20;
    Func_020022be(0x318);
    Func_020022c4(0x319);
    if (heading == 115) {
        Func_020022c6(0x319);
    } else if (heading == 117) {
        Func_020022d4(0x318);
    }
    Func_02001f06(0);
}
