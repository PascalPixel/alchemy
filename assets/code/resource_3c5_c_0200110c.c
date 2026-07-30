typedef signed int s32;

extern s32 *Func_02003f62();
extern s32 *Func_02003f6a();
extern s32 *Func_02003f78();
extern void Func_0200223c();
extern void Func_0200408e();

void Func_0200110c(void) {
    s32 *p = Func_02003f62(9);

    if ((Func_02003f6a(0)[2] >> 20) <= 12) {
        p = Func_02003f78(8);
        if ((p[2] >> 20) == 6) {
            if ((p[4] >> 20) == 20) {
                Func_0200223c();
                return;
            }
        }
    } else {
        if ((p[2] >> 20) == 18) {
            if ((p[4] >> 20) == 20) {
                Func_0200223c();
                return;
            }
        }
    }
    Func_0200408e();
}
