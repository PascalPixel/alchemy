typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

u32 Func_080022f4(s32, s32);
s32 Func_08002304(s32, s32);
u8 *Func_08017dd4(void *, s32, s32);

void *Func_0801f680(s32 value, u8 *output)
{
    u8 buffer[64];
    u32 time;
    u32 minutes;
    s32 seconds;
    u8 *text;
    u8 *cursor;

    time = Func_080022f4(value, 0xe10);
    if (time > 0xea5f)
        time = 0xea5f;

    minutes = Func_080022f4(time, 60);
    seconds = Func_08002304(time, 60);

    text = Func_08017dd4(buffer, minutes, 3);
    *output = *text;
    text++;
    cursor = output + 1;
    *cursor = *text;
    cursor++;
    *cursor = text[1];
    seconds += 100;
    cursor++;
    *cursor = ':';

    text = Func_08017dd4(buffer, seconds, 2);
    cursor++;
    *cursor = text[0];
    cursor++;
    *cursor = text[1];
    cursor[1] = 0;

    return output;
}
