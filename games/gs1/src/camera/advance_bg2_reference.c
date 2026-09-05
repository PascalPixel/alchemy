#include "types.h"
#include "global_cells.h"

#define Camera_AdvanceBg2Reference Func_080c9138

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Camera_AdvanceBg2Reference(void) {
    s32 cnt;
    void *state;

    state = *(void **)ADDR_03001EEC;
    cnt = FIELD_AT_OFFSET(state, s32 *, 0x7790) + 1;
    FIELD_AT_OFFSET(state, s32 *, 0x7790) = cnt;
    if (cnt == FIELD_AT_OFFSET(state, s32 *, 0x7794)) {
        FIELD_AT_OFFSET((void *)0x04000028, s32 *, 0) = (s32) FIELD_AT_OFFSET(state, s32 *, 0x77D0);
        FIELD_AT_OFFSET((void *)0x04000028, s32 *, 4) = (s32) FIELD_AT_OFFSET(state, s32 *, 0x77D4);
        FIELD_AT_OFFSET(state, s32 *, 0x77D0) = (s32) (FIELD_AT_OFFSET(state, s32 *, 0x77D0) + FIELD_AT_OFFSET(state, s32 *, 0x7798));
        FIELD_AT_OFFSET(state, s32 *, 0x77D4) = (s32) (FIELD_AT_OFFSET(state, s32 *, 0x77D4) + FIELD_AT_OFFSET(state, s32 *, 0x779C));
        FIELD_AT_OFFSET(state, s32 *, 0x7790) = 0;
    }
}
