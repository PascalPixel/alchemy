#include "tundaria_tower.h"

void Func_02002fb0(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02002fb8(struct ObjectRuntime *object);
void Func_02003178(void);
void Object_AttachWorkTargetToObject(u16 actor, s32 value);
s32 MapCell_GetAttributes(u32 layer, s32 x, s32 z);
void WaitFrames(s32 frames);

// The event's object keeps stepping a cell at a time in the direction it was pushed (its cell
// against the one Func_02000bfc stored in its action) while the next cell's layer 1 attribute is
// 50 and its layer 2 attribute is not 255, then settles on the middle of its cell.
void Func_02000c64(s32 unused, s32 actorArg)
{
    u8 *work = Ram_HeapSlots->event_work;
    struct ObjectRuntime *object;
    s16 actor;
    s32 dx;
    s32 dz;

    /* FAKEMATCH: an empty asm after the event work load, with the actor taken as s32 and narrowed
     * after it, so the load comes before the 0xcbc offset is built, as in the native. Without the
     * asm, plain or with the actor narrowed late: differ 2; a one-pass block there: differ 2 or 6. */
    asm("");
    actor = actorArg;
    object = *(struct ObjectRuntime **)(work + 0xcbc);
    Func_02003048();
    Func_02003148(0);
    dx = (object->x >> 20) - (object->action >> 8);
    dz = (object->z >> 20) - (object->action & 0xff);
    object->speed_limit = 0x8000;
    while ((MapCell_GetAttributes(1, (object->x >> 20) + dx, (object->z >> 20) + dz) >> 8) == 50
           && (MapCell_GetAttributes(2, (object->x >> 20) + dx, (object->z >> 20) + dz) >> 8) != 255) {
        Func_02002fb0(object, (dx << 20) + object->x, object->y, (dz << 20) + object->z);
        Object_AttachWorkTargetToObject(actor, 1);
        Func_02002fb8(object);
        WaitFrames(1);
    }
    object->x = ((object->x >> 19) | 1) << 19;
    object->z = ((object->z >> 19) | 1) << 19;
    Func_02003178();
    Func_02003050();
}
