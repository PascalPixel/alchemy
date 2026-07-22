typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_00000001;

void Func_080090d0(void);

void Func_0809b364(void *arg0) {
    u8 *object;
    u8 *source;
    s32 position;
    s32 threshold;
    s32 speed;

    object = arg0;
    threshold = *(s32 *)(object + 0x14) + 0xA0000;
    source = *(u8 **)(object + 0x68);
    if (Data_02000240[237] == (s32)&Value_00000001)
        threshold = *(s32 *)(object + 0x14) + 0x40000;

    position = *(s32 *)(object + 0x0C);
    if (position <= threshold) {
        Func_080090d0();
        return;
    }

    speed = *(s32 *)(object + 0x18) + 0xC00;
    if (speed > 0x10000)
        speed = 0x10000;
    *(s32 *)(object + 0x18) = speed;
    *(s32 *)(object + 0x1C) = speed;
    *(s32 *)(object + 8) = *(s32 *)(source + 8);
    *(s32 *)(object + 0x0C) = position + (s32)0xFFFE0000;
    *(s32 *)(object + 0x10) =
        *(s32 *)(source + 0x10) + ((0x10000 - speed) * 5) + 0x90000;
}
