typedef signed int s32;

extern s32 *Func_02002326(s32);
extern void Func_02002304(s32);
extern void Func_0200230a(s32);
extern void Func_0200230c(s32);
extern void Func_02002318(s32);
extern void Func_02001f4a(s32);

void Func_02000754(void)
{
    s32 *record = Func_02002326(14);
    s32 heading;

    if (record == 0) return;
    heading = record[2] >> 20;
    Func_02002304(0x31a);
    Func_0200230a(0x31b);
    if (heading == 119) {
        Func_0200230c(0x31b);
    } else if (heading == 121) {
        Func_02002318(0x31a);
    }
    Func_02001f4a(0);
}
