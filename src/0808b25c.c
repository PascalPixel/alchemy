#include "types.h"

extern s16 Data_02000240[];
extern s32 Data_0809e270[];

void Func_0808b25c(s32 arg0, s32 arg1) {
    s32 target = Data_02000240[224];
    s32 *table = Data_0809e270;
    s32 elem = *table++;
    s32 result = arg1;

    if (elem != 0 && elem != target) {
        do {
            if (elem & 0x80000000) {
                result = elem & 0xFFFF;
            }
            elem = *table++;
        } while (elem != 0 && elem != target);
    }
    Data_02000240[235] = (s16)result;
}
