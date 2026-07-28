#include "types.h"

extern s16 Data_02000240[];
extern const s16 Data_0809e1d8[];

void Func_0808b1d8(void)
{
    s16 first = Data_02000240[224];
    s16 second = Data_02000240[225];
    const s16 *entry = Data_0809e1d8;

    /* -1で終端する4半語の表を検索する。 */
    while (entry[0] != -1) {
        if (entry[0] == first &&
            (entry[1] == -1 || entry[1] == second)) {
            Data_02000240[226] = entry[2];
            Data_02000240[227] = entry[3];
            return;
        }
        entry += 4;
    }
}
