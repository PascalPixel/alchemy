typedef signed int s32;

extern s32 *Func_0200224e(s32);
extern void Func_0200222e(s32);
extern void Func_02002234(s32);
extern void Func_02002236(s32);
extern void Func_02002244(s32);
extern void Func_02001e76(s32);

void Func_0200067c(void)
{
    s32 *record = Func_0200224e(11);
    s32 heading;

    if (record == 0) {
        return;
    }
    heading = record[2] >> 20;
    Func_0200222e(0x314);
    Func_02002234(0x315);
    if (heading == 107) {
        Func_02002236(0x315);
    } else if (heading == 109) {
        Func_02002244(0x314);
    }
    Func_02001e76(0);
}
