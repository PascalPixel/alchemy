typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;

extern s32 Func_080795fc(void);
extern void *Func_08077394(s32);
extern s32 Func_080022ec(s32, s32);
extern u8 Data_02000240[];

void Func_0807808c(s32 arg0)
{
    s32 count;
    s32 n;
    u8 *obj;
    s32 t;
    s32 v14;
    s32 v16;
    s32 one;
    s16 v34;

    count = Func_080795fc();
    for (n = 0; n < count; n++) {
        obj = (u8 *)Func_08077394(Data_02000240[504 + n]);

        do {
            *(u16 *)(obj + 0x38) = *(u16 *)(obj + 0x34);
            *(u16 *)(obj + 0x3A) = *(u16 *)(obj + 0x36);
        } while (0);

        v34 = *(s16 *)(obj + 0x34);
        t = Func_080022ec(v34 << 14, v34);
        v14 = 0x4000;
        if (t <= 0x4000) {
            v14 = 0;
            if (t >= 0) {
                v14 = t;
            }
        }
        *(s16 *)(obj + 0x14) = (s16)v14;
        if (((v14 << 16) == 0) && (*(s16 *)(obj + 0x38) != 0)) {
            one = 1;
            *(s16 *)(obj + 0x14) = (s16)one;
        }

        t = Func_080022ec(*(s16 *)(obj + 0x3A) << 14, *(s16 *)(obj + 0x36));
        v16 = 0x4000;
        if (t <= 0x4000) {
            v16 = 0;
            if (t >= 0) {
                v16 = t;
            }
        }
        *(s16 *)(obj + 0x16) = (s16)v16;
        if (((v16 << 16) == 0) && (*(s16 *)(obj + 0x3A) != 0)) {
            one = 1;
            *(s16 *)(obj + 0x16) = (s16)one;
        }

        if (arg0 == 1) {
            *(s8 *)(obj + 0x131) = 0;
            *(s8 *)(obj + 0x140) = 0;
        }
    }
}
