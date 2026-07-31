typedef signed int s32;

extern s32 *Func_02002296(s32);
extern void Func_02002274(s32);
extern void Func_0200227a(s32);
extern void Func_0200227c(s32);
extern void Func_02002288(s32);
extern void Func_02001eba(s32);

void Func_020006c4(void)
{
    s32 *record = Func_02002296(12);
    s32 heading;

    if (record == 0) {
        return;
    }
    heading = record[2] >> 20;
    Func_02002274(0x316);
    Func_0200227a(0x317);
    if (heading == 111) {
        Func_0200227c(0x317);
    } else if (heading == 113) {
        Func_02002288(0x316);
    }
    Func_02001eba(0);
}
