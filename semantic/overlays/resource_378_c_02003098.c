typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

extern u8 *Func_080090c8(s32 kind, s32 x, s32 y, s32 z);
extern void Func_08009098(u8 *object, const void *script);
extern s32 Func_08000140(s32 bank, s32 item);
extern void Func_08015250(s32 value);
extern void Func_080001c8(s32 frame, s32 mode, s32 destination);
extern void Func_08000150(s32 bank);
extern void Func_080000c0(s32 frames);

/* Configure object 22, animate it for sixty frames, then swap its script. */
void Func_02003098(s32 presentationValue, s32 x, s32 y, s32 z)
{
    u8 *object = Func_080090c8(22, x, y, z);
    u8 *record;
    s32 destination;
    s32 frame;

    if (object == 0)
        return;
    Func_08009098(object, (const void *)0x0200b8f8);
    record = *(u8 **)(object + 80);
    record[38] = 0;
    record[39] = 0;
    record[5] &= (u8)~0x20;
    record[9] &= 0x0f;
    *(s32 *)(object + 40) = 0x20000;
    *(s32 *)(object + 72) = 0x4000;

    destination = Func_08000140(17, 0x608);
    Func_08015250(presentationValue);
    destination += 0x400;
    Func_080001c8(record[28], 0x80, destination);
    Func_08000150(17);

    for (frame = 0; frame < 60; frame++) {
        if ((u32)(*(s32 *)(object + 40) + 255) <= 510)
            object[85] = 0;
        Func_080000c0(1);
    }
    Func_08009098(object, (const void *)0x0200ba9c);
}
