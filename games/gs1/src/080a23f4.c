#include "m7_interfaces.h"

void Func_080a23f4(struct Bounds080a23f4 *bounds, s32 right, s32 bottom,
    s32 left, s32 top) {
    if (bounds != NULL) {
        bounds->left = left;
        bounds->right = right;
        bounds->top = top;
        bounds->bottom = bottom;
    }
}
