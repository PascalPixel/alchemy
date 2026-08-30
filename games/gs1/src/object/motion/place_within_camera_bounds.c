#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void Object_ResetMotion(void *);
void WaitFrames(u32);
void Func_08009128(void);
void Object_SetPosition(void *, s32, s32, s32);

void ObjectMotion_PlaceWithinCameraBounds(s32 requested_x, s32 requested_y, s32 requested_z, s32 use_setter)
{
    s32 should_use_setter;
    void *runtime_block;
    s32 minimum_x;
    s32 minimum_z;
    s32 maximum_x;
    s32 object_z_offset;
    s32 maximum_z;
    s32 position_x;
    s32 position_z;
    s32 position_y;
    void *camera_state;
    void *object;

    position_x = requested_x;
    position_y = requested_y;
    should_use_setter = use_setter;
    position_z = requested_z;
    runtime_block = Runtime_AllocateBlock(0x1B, 0xCCC);
    object = FIELD_AT_OFFSET(runtime_block, void **, 0x1E0);
    camera_state = *(void **)ADDR_03001E70;
    minimum_x = FIELD_AT_OFFSET(camera_state, s32, 0xEC) + 0x780000;
    object_z_offset = FIELD_AT_OFFSET(object, s32, 0xC);
    minimum_z = FIELD_AT_OFFSET(camera_state, s32, 0xF0) + object_z_offset + 0x600000;
    maximum_x = FIELD_AT_OFFSET(camera_state, s32, 0xF4) + 0xFF880000;
    maximum_z = FIELD_AT_OFFSET(camera_state, s32, 0xF8) + object_z_offset + 0xFFC00000;
    FIELD_AT_OFFSET(camera_state, void **, 0) = (void *) (object + 8);
    Object_ResetMotion(object);
    if (position_x == -1) {
        position_x = FIELD_AT_OFFSET(object, s32, 8);
    }
    if (position_y == -1) {
        position_y = FIELD_AT_OFFSET(object, s32, 0xC);
    }
    if (position_z == -1) {
        position_z = FIELD_AT_OFFSET(object, s32, 0x10);
    }
    if (position_x < minimum_x) {
        position_x = minimum_x;
    }
    if (position_z < minimum_z) {
        position_z = minimum_z;
    }
    if (position_x > maximum_x) {
        position_x = maximum_x;
    }
    if (position_z > maximum_z) {
        position_z = maximum_z;
    }
    if (should_use_setter == 0) {
        FIELD_AT_OFFSET(object, s32, 8) = position_x;
        FIELD_AT_OFFSET(object, s32, 0xC) = position_y;
        FIELD_AT_OFFSET(object, s32, 0x10) = position_z;
        WaitFrames(1U);
        if (FIELD_AT_OFFSET(runtime_block, s16, 0x19E) != 3) {
            Func_08009128();
        }
    } else {
        Object_SetPosition(object, position_x, position_y, position_z);
    }
}
