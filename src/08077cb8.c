typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern s32 Func_08002f40(s32);
extern volatile u8 Data_03001f54;
extern u8 Data_00000002[];

u16 Func_08077cb8(void)
{
    u8 *digits;
    s32 hourTens;
    s32 hourUnits;
    s32 minuteTens;
    s32 minuteUnits;
    s32 secondTens;
    s32 secondUnits;
    s32 hours;
    s32 minutes;
    s32 seconds;
    s32 packed;
    s32 shifted;
    s32 result;

    digits = (u8 *)Func_08002f40((s32)Data_00000002);
    hourTens = *digits;
    hours = (hourTens - '0') * 10;
    digits++;
    hourUnits = *digits;
    digits++;
    hours += hourUnits - '0';
    minuteTens = *digits;
    minutes = (minuteTens - '0') * 10;
    digits++;
    minuteUnits = *digits;
    digits++;
    minutes += minuteUnits - '0';
    secondTens = digits[0];
    seconds = (secondTens - '0') * 10;
    secondUnits = digits[1];
    seconds += secondUnits - '0';
    packed = (((hours << 4) + minutes) << 6) + seconds;
    shifted = 0x80 << 21;
    shifted |= packed << 16;
    result = shifted >> 16;
    if (Data_03001f54 != 0) {
        result |= (s32)0xffff8000;
    }
    return (u16)result;
}
