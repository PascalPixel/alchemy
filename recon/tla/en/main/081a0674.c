#include "TYPES.H"

void DisplayScroll_BuildHblankWordTable(u32 *arg0)
{
    s32 count;
    u32 value = 0x01FF01FF;
    u32 step = 0x10000;

    count = 31;
    do {
        count--;
        *arg0++ = value;
    } while (count >= 0);
    count = 239;
    do {
        count--;
        *arg0++ = step;
        step += 0x20002;
    } while (count >= 0);
    count = 47;
    do {
        count--;
        *arg0++ = value;
    } while (count >= 0);
    step = 0;
    count = 191;
    do {
        count--;
        *arg0++ = step;
    } while (count >= 0);
}
