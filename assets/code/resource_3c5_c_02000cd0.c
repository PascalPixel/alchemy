typedef signed int s32;

extern void Func_02003a5e();

void Func_02000cd0(s32 *arg0) {
    s32 i = 60;

    while (i != 0) {
        Func_02003a5e(1);
        i--;
        if (arg0[10] == 0) {
            break;
        }
    }
}
