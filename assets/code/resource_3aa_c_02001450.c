typedef signed int s32;

/* One import reached through four veneer slots twelve bytes apart, so the four
 * `bl` encodings are identical and the call sites must name distinct symbols. */
extern void Func_02002f92(s32, s32, s32);
extern void Func_02002f9e(s32, s32, s32);
extern void Func_02002faa(s32, s32, s32);
extern void Func_02002fb6(s32, s32, s32);
extern void Func_02002f08(s32);

void Func_02001450(s32 delay)
{
    Func_02002f92(0, 0xC000, 0);
    Func_02002f9e(1, 0xC000, 0);
    Func_02002faa(2, 0xC000, 0);
    Func_02002fb6(3, 0xC000, 0);
    if (delay != 0) {
        Func_02002f08(delay);
    }
}
