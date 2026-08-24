#include "types.h"

void RenderOutput_Release(void *);

void RenderOutput_ClearList(void *arg0) {
    void *next;
    void *node;

    next = NULL;
    /* 単方向リストを先頭から解放する。 */
    if (arg0 != NULL) {
        node = *(void **)arg0;
        *(void **)((u8 *)arg0 + 4) = arg0;
        *(void **)arg0 = next;
        while (node != NULL) {
            next = *(void **)node;
            RenderOutput_Release(node);
            node = next;
        }
    }
}
