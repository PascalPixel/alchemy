#include "types.h"

#define UiWork_UpdateListTail Func_0801656c

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void UiWork_UpdateListTail(s32 *list) {
    s32 *prev;
    s32 *node;

    /* 末尾要素をキャッシュする。 */
    node = FIELD_AT_OFFSET(list, s32 **, 0);
    prev = list;
    if (node != NULL) {
        do {
            prev = node;
            node = *prev;
        } while (node != NULL);
    }
    FIELD_AT_OFFSET(list, s32 **, 4) = prev;
}
