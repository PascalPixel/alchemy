typedef signed int s32;

s32 Func_020011ee();        /* tests a flag id; nonzero when set */
void Func_02001216();       /* opens a scripted sequence */
void Func_020012a4();       /* queues a cue / script id */
s32 Func_020012cc();
void Func_02001230();       /* closes the scripted sequence */
s32 Func_020006be(void);
void Func_02001342();
void Func_02001240();
void Func_020012ce();
s32 Func_0200122c();
void Func_020012de();
void Func_020012f6();
void Func_0200126a();

void Func_02000360(void)
{
    if (Func_020011ee(0x242) == 0) {
        Func_02001216();
        Func_020012a4(0x18e7);
        /* r1 is set before r0 here; the argument order is unchanged. */
        Func_020012cc(15, 0);
        Func_02001230();
        return;
    }

    if (Func_020006be() != 0) {
        Func_02001342(19, 15);
        return;
    }

    Func_02001240();
    Func_020012ce(0x18ea);
    if (Func_0200122c(0x909) != 0) {
        Func_020012de(0x1941);
    }
    Func_020012f6(15, 0);
    Func_0200126a();
}
