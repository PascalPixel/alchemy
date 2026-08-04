#include "types.h"

extern s8 Data_080af2a6;
extern s8 Data_080af2d0;
extern s8 Data_080af2bc;
extern s8 Data_080af2b1;

void Func_080a1f74(s32 kind, s8 *destination)
{
    const s8 *source;
    s8 value;
    s32 index;

    source = &Data_080af2a6;
    switch (kind) {
    case 0:
        source = &Data_080af2d0;
        break;
    case 1:
        source = &Data_080af2bc;
        break;
    case 2:
        source = &Data_080af2b1;
        break;
    }
    value = *source;
    *destination = value;
    index = 0;
    while (value != -1) {
        index++;
        if (index > 31) {
            break;
        }
        source++;
        value = *source;
        destination++;
        *destination = value;
    }
}
