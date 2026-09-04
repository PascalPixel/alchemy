#include "types.h"

#define Animation_SetIndexAndInitObjects Func_0800c408

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

struct State_0800b7c0;

s32 InitializeAnimationObjects(struct State_0800b7c0 *);

void Animation_SetIndexAndInitObjects(void *obj, s32 no) {
    if ((obj != NULL) && ((0xF & FIELD_AT_OFFSET(obj, u8 *, 0x54)) == 1)) {
        obj = FIELD_AT_OFFSET(obj, void **, 0x50);
        if (no >= 0) {
            *FIELD_AT_OFFSET(obj, s16 **, 0x28) = (s16) no;
            InitializeAnimationObjects((struct State_0800b7c0 *)obj);
        }
    }
}
