typedef signed int s32;

extern s32 Value_00000f2c;

s32 Func_02002040(s32, s32);
void Func_0200662a(s32);

void Func_02001000(void)
{
    if (Func_02002040(10, 6) != 0) {
        Func_0200662a((s32)&Value_00000f2c);
    }
}
