typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08092878(void *first, void *second)
{
    s32 firstDifference;
    s32 secondDifference;
    u32 secondTarget;
    u32 remaining;
    s32 counter;
    u16 firstAngle;
    u16 secondAngle;
    u32 firstTarget;

    if (first != 0 && second != 0) {
        firstTarget = (u16)Func_080044d0(
            FIELD(second, s32 *, 0x10) - FIELD(first, s32 *, 0x10),
            FIELD(second, s32 *, 8) - FIELD(first, s32 *, 8));
        secondTarget = firstTarget + 0x8000;
        counter = 0;
loop:
        firstAngle = FIELD(first, u16 *, 6);
        firstDifference = (s16)(firstTarget - firstAngle);
        remaining = 2;
        if (firstDifference != 0) {
            if (firstDifference > 0x1000)
                firstDifference = 0x1000;
            if (firstDifference < -0x1000)
                firstDifference = -0x1000;
            FIELD(first, u16 *, 6) = firstAngle + firstDifference;
        } else {
            remaining = 1;
        }

        secondAngle = FIELD(second, u16 *, 6);
        secondDifference = (s16)(secondTarget - secondAngle);
        if (secondDifference != 0) {
            if (secondDifference > 0x1000)
                secondDifference = 0x1000;
            if (secondDifference < -0x1000)
                secondDifference = -0x1000;
            FIELD(second, u16 *, 6) = secondAngle + secondDifference;
        } else {
            remaining--;
        }

        if (remaining != 0) {
            Func_080030f8(1U);
            counter++;
            if (counter <= 0x3b)
                goto loop;
        }
    }
}
