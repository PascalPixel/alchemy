/* Near miss: differ 2 (29/29 instructions), one ARM9 scheduling swap. After
   the second Random16 the listing loads the heading (ldrh r3, [r6, #6])
   before shifting the first draw (lsrs r5, r5, #4); this draft shifts it
   first. Draws held in u32 or s32 locals, pre-shifted, both calls inline,
   the difference in its own local or the add written out all keep it. The
   saved heading must be a 32-bit local: a u16 one is loaded with ldrsh
   (13). Goes between SPRITE_SHAPE.C and EVENT_POSE.C. */
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "SYSTEM.H"

/* Shakes the object's heading at random for sixteen frames, then restores
   it. */
void Object_ShakeAngle(u32 object_id)
{
    struct ObjectRuntime *object;
    s32 angle;
    s32 i;

    object = ObjectTable_Get(object_id);
    if (object == 0)
        return;
    angle = object->angle;
    for (i = 15; i >= 0; i--) {
        u32 a = Random16();
        u32 b = Random16();

        object->angle += (a >> 4) - (b >> 4);
        WaitFrames(1);
    }
    object->angle = angle;
}
