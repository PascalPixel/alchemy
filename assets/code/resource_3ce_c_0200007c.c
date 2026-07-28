typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

extern s16 Data_02000240[];
extern volatile s32 Data_03001ae8;

extern s32 Func_0200127e(s32, s32, s32, s32);
extern void Func_02001298(s32, s32);
extern void Func_020012a0(s32, s32);
extern void Func_020012a8(s32, s32);
extern void Func_02000112(s32);
extern void Func_02001264(s32);
extern void Func_020012fc(void);
extern void Func_020012b4(s32, s32);

void Func_0200007c(s32 x, s32 count)
{
    s16 *table = Data_02000240;
    volatile s32 *flags;
    s32 token;
    s32 i;

    *(u8 *)&table[262] = 2;
    token = Func_0200127e(125, 0, 0, 0);
    for (i = 0; i < count; i++) {
        flags = &Data_03001ae8;
        Func_02001298(1, 1);
        Func_020012a0(141, 2);
        Func_020012a8(0x1e240, 5);
        Func_02000112(x);
        goto test;
retry:
        if (*flags != 0) {
            goto next;
        }
        Func_02001264(1);
test:
        if ((*flags & 2) != 0) {
            goto end;
        }
        if ((*flags & 1) != 0) {
            goto inc;
        }
        if ((*flags & 0x80) == 0) {
            goto other;
        }
inc:
        x++;
        goto next;
other:
        if ((*flags & 0x40) != 0) {
            x--;
            goto next;
        }
        goto retry;
next:;
    }
end:
    Func_020012fc();
    Func_020012b4(token, 2);
}
