typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

extern u8 *Func_02006614(s32 kind, s32 x, s32 y, s32 z);
extern void Func_0200661a(u8 *object, const void *script);
extern s32 Func_02006636(s32 bank, s32 item);
extern void Func_020066ae(s32 value);
extern void Func_0200665e(s32 frame, s32 mode, s32 destination);
extern void Func_0200665c(s32 bank);
extern void Func_0200664c(s32 frames);
extern void Func_02006698(u8 *object, const void *script);

/* Configure object 22, animate it for sixty frames, then swap its script. */
void Func_02003098(s32 presentationValue, s32 x, s32 y, s32 z)
{
    u8 *object = Func_02006614(22, x, y, z);
    u8 *record;
    s32 destination;
    s32 frame;

    if (object == 0)
        return;
    Func_0200661a(object, (const void *)0x0200b8f8);
    record = *(u8 **)(object + 80);
    record[38] = 0;
    record[39] = 0;
    record[5] &= (u8)~0x20;
    record[9] &= 0x0f;
    *(s32 *)(object + 40) = 0x20000;
    *(s32 *)(object + 72) = 0x4000;

    destination = Func_02006636(17, 0x608);
    Func_020066ae(presentationValue);
    destination += 0x400;
    Func_0200665e(record[28], 0x80, destination);
    Func_0200665c(17);

    for (frame = 0; frame < 60; frame++) {
        if ((u32)(*(s32 *)(object + 40) + 255) <= 510)
            object[85] = 0;
        Func_0200664c(1);
    }
    Func_02006698(object, (const void *)0x0200ba9c);
}
