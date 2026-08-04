#include "types.h"

struct Record_0808d394 {
    s16 id;
    u8 padding[22];
};

extern struct Record_0808d394 **Data_03001ebc;

struct Record_0808d394 *Func_0808d394(s32 id) {
    struct Record_0808d394 **lists;
    struct Record_0808d394 *record;
    u16 current;
    s32 test;
    s32 listIndex;
    s32 numbered;
    s32 initialSentinel;

    lists = Data_03001ebc;
    initialSentinel = -1;
    numbered = 8;
    listIndex = 0;
    do {
        record = *lists++;
        if (record != 0) {
            if (id <= 7) {
                test = record->id;
                    current = *(volatile u16 *)record;
                if (test != initialSentinel) {
                    do {
                        test = (s16)current;
                        if (test == id)
                            goto done;
                        record++;
                        test = record->id;
                        current = *(volatile u16 *)record;
                        if (test == -1)
                            break;
                    } while (1);
                }
            } else {
                current = *(volatile u16 *)record;
                test = (s16)current;
                if (test != initialSentinel) {
                    do {
                        test = (s16)current;
                        if (test > 7) {
                            if (numbered == id)
                                goto done;
                            numbered++;
                        }
                        record++;
                        current = *(volatile u16 *)record;
                    } while ((s16)current != -1);
                }
            }
        }
        listIndex++;
    } while (listIndex <= 3);
    current = *(volatile u16 *)record;

done:
    test = (s16)current;
    if (test == -1)
        record = 0;
    return record;
}
