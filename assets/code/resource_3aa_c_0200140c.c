typedef signed int s32;

/* One import reached through four veneer slots twelve bytes apart, so the four
 * `bl` encodings are identical and the call sites must name distinct symbols. */
extern void Func_02002f4e(s32, s32, s32);
extern void Func_02002f5a(s32, s32, s32);
extern void Func_02002f66(s32, s32, s32);
extern void Func_02002f72(s32, s32, s32);
extern void Func_02002ec4(s32);

void Func_0200140c(s32 delay)
{
    Func_02002f4e(0, 0x6000, 0);
    Func_02002f5a(1, 0xE000, 0);
    Func_02002f66(2, 0x2000, 0);
    Func_02002f72(3, 0xA000, 0);
    if (delay != 0) {
        Func_02002ec4(delay);
    }
}
