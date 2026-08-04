#include "types.h"

extern s16 Data_02000240[];
extern s32 Data_0809e270[];

void Func_0808b25c(s32 arg0, s32 arg1) {
    s32 target = Data_02000240[224];
    s32 *table = Data_0809e270;
    s32 entry = *table++;
    s32 result = arg1;

    if (entry != 0 && entry != target) {
        do {
            if (entry & 0x80000000) {
                result = entry & 0xFFFF;
            }
            entry = *table++;
        } while (entry != 0 && entry != target);
    }
    Data_02000240[235] = (s16)result;
}
