#include "object_lookup.h"
#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
void RotateVectorByMagnitude(s32, s32, void *);
void BattleEffect_SetupObjectPair(s32 first_object_id, s32 second_object_id) {
    void *first_object; void *second_object; s32 facing_quadrant; void *state;
    state = *(void **)ADDR_03001F30;
    FIELD_AT_OFFSET(state, s16, 0x18) = first_object_id;
    first_object = ObjectTable_Get((s16) first_object_id);
    FIELD_AT_OFFSET(state, s16, 0x1A) = second_object_id;
    FIELD_AT_OFFSET(state, s32 *, 0x10) = (s32) first_object;
    second_object = ObjectTable_Get((s16) second_object_id);
    facing_quadrant = (FIELD_AT_OFFSET(first_object, u16, 6) + 0x2000) & 0xC000;
    FIELD_AT_OFFSET(state, s32 *, 0x14) = (s32) second_object;
    FIELD_AT_OFFSET(state, s32 *, 0) = facing_quadrant;
    if (second_object != 0) {
        FIELD_AT_OFFSET(state, s32 *, 0x38) = (s32) FIELD_AT_OFFSET(second_object, s32 *, 0x6C);
        FIELD_AT_OFFSET(state, s32 *, 0x3C) = (s32) FIELD_AT_OFFSET(second_object, s32 *, 0);
        {
            u8 object_variant = (u8) FIELD_AT_OFFSET(FIELD_AT_OFFSET(FIELD_AT_OFFSET(second_object, void **, 0x50), void **, 0x28), u8, 5);
            FIELD_AT_OFFSET(state, u8, 0x44) = object_variant;
        }
        FIELD_AT_OFFSET(state, s32 *, 4) = (s32) FIELD_AT_OFFSET(second_object, s32 *, 8);
        FIELD_AT_OFFSET(state, s32 *, 0xC) = (s32) FIELD_AT_OFFSET(second_object, s32 *, 0x10);
        FIELD_AT_OFFSET(state, s32 *, 8) = (s32) FIELD_AT_OFFSET(second_object, s32 *, 0xC);
        return;
    }
    FIELD_AT_OFFSET(state, s32 *, 4) = (s32) FIELD_AT_OFFSET(first_object, s32 *, 8);
    FIELD_AT_OFFSET(state, s32 *, 0xC) = (s32) FIELD_AT_OFFSET(first_object, s32 *, 0x10);
    FIELD_AT_OFFSET(state, s32 *, 8) = (s32) FIELD_AT_OFFSET(first_object, s32 *, 0xC);
    RotateVectorByMagnitude(0x100000, facing_quadrant, (u8 *)state + 4);
}
