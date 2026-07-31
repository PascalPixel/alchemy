typedef signed int s32;

s32 Func_020012d8();        /* tests a flag id; nonzero when set */
void Func_02001300();       /* opens a scripted sequence */
void Func_0200138e();       /* queues a cue / script id */
void Func_020013a6();
void Func_0200131a();       /* closes the scripted sequence */
s32 Func_020007a8(void);
void Func_0200142c();
void Func_0200132a();
void Func_020013b8();
s32 Func_02001316();
void Func_020013c8();
void Func_020013e0();
void Func_02001354();

void Func_02000448(void)
{
    if (Func_020012d8(0x240) == 0) {
        Func_02001300();
        Func_0200138e(0x18f1);
        Func_020013a6(21, 0);
        Func_0200131a();
        return;
    }

    if (Func_020007a8() != 0) {
        Func_0200142c(21, 16);
        return;
    }

    Func_0200132a();
    Func_020013b8(0x18f2);
    if (Func_02001316(0x909) != 0) {
        Func_020013c8(0x1945);
    }
    Func_020013e0(16, 0);
    Func_02001354();
}
