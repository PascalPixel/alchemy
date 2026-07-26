#include "types.h"

void *Func_0808ba1c(s32);
void Func_08092878(void *, void *);
void Func_0809163c(s32);

void Func_08092848(s32 arg0, s32 arg1, s32 arg2) {
    void *first = Func_0808ba1c(arg0);
    void *second = Func_0808ba1c(arg1);
    if (first != NULL && second != NULL) {
        Func_08092878(first, second);
        Func_0809163c(arg2);
    }
}
