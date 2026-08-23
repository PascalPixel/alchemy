#include "object_runtime.h"

void Func_0809163c(s32 arg0);
void Audio_PlayCue(s32 arg0);

void Func_08092560(u32 object_id, s32 speed, s32 event_id)
{
    struct ObjectRuntime *object;
    u8 *flags;
    s32 val;
    s32 shifted;

    object = ObjectTable_Get(object_id);
    if (object != NULL) {
        flags = &object->flags;
        val = *flags | 2;
        shifted = speed << 16;
        *flags = val;
        object->velocity_y = shifted;
        if (speed > 5) {
            Audio_PlayCue(0x99);
        } else {
            Audio_PlayCue(0x98);
        }
        Func_0809163c(event_id);
    }
}
