#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void RenderOutput_AppendToList(void *arg0, s8 *arg1) {
    /* 新しい末尾をリストとキャッシュの両方へ接続する。 */
    if (arg0 != NULL) {
        *(volatile s32 *)FIELD_AT_OFFSET(arg0, s32 **, 4) = (s32) arg1;
        *(s32 * volatile *)((u8 *)arg0 + 4) = (s32 *)arg1;
    }
}
