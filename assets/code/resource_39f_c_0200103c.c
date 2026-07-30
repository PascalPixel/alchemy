typedef signed int s32;

extern s32 *Func_02003e00(s32 a);
extern void Func_02001d38(s32 *p);

void Func_0200103c(void) {
    s32 v[3];
    s32 *p = Func_02003e00(0);

    v[0] = (p[2] & 0xfff00000) + 0x80000;
    v[1] = p[3];
    v[2] = (p[4] & 0xfff00000) + 0xffe80000;
    Func_02001d38(v);
}
