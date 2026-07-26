#include "types.h"

void Func_08016594(void *);

void Func_080164ac(void *arg0) {
    void *next;
    void *node;

    next = NULL;
    if (arg0 != NULL) {
        node = *(void **)arg0;
        *(void **)((u8 *)arg0 + 4) = arg0;
        *(void **)arg0 = next;
        while (node != NULL) {
            next = *(void **)node;
            Func_08016594(node);
            node = next;
        }
    }
}
