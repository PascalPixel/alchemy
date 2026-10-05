#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "SYSTEM.H"

/* Empty inventory hook. */
void Inventory_ReservedNoOp(void)
{
}

/* Waits up to 90 frames for an object's animation record to leave frame
   value. ⚓️'s record keeps its last frame number at 0x18, twelve bytes
   before ☀️'s, and the loop reads it through the record each frame. */
void Object_WaitUntilChildValueDiffers(s32 object_id, s32 value)
{
    struct ObjectRuntime *obj = ObjectTable_Get(object_id);

    if (obj != 0 && obj->animation_kind == 1) {
        s32 cnt = 0;
        u8 *anim = (u8 *)obj->animation;

        while (cnt <= 89) {
            WaitFrames(1);
            if (value != anim[0x18]) {
                break;
            }
            cnt++;
        }
    }
}
