/* Near miss: listing 81 instructions, draft 81, common 62, differ 38; the rest differs in instruction order. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct MoveScene {
    u16 mode;
    u8 unk02[14];
    struct ObjectRuntime *mainObject;
};

struct MapCell;
u8 Func_080dbde8(s32 x, s32 z, s32 layer);
struct MapCell *Func_080dbb78(s32 x, s32 z, s32 layer);
void ObjectDispatch_SetSpritePriorityFar(struct ObjectRuntime *object, s32 priority);
void ObjectDispatch_InitializeFar(struct ObjectRuntime *object, const s32 *script);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Motion_SetTargetPositionFromMagnitudeAngle(struct ObjectRuntime *object, s32 magnitude, s32 angle);
void WaitFrames(s32 frames);
u32 Random16(void);
extern const s32 Data_080f0e78[];

void Func_080dbf94(struct ObjectRuntime *object, s32 x, s32 y, s32 z)
{
    struct ObjectRuntime *mainObject = ((struct MoveScene *)Ram_HeapSlots->effectWork)->mainObject;
    s32 start[3];
    s32 i;
    s32 scale;

    start[0] = mainObject->x;
    start[1] = mainObject->y + 0x100000;
    start[2] = mainObject->z;
    i = 0;
    scale = 0;
    do {
        object->x = x + i * (start[0] - x) / 10;
        object->y = y + i * (start[1] - y) / 10;
        object->z = z + i * (start[2] - z) / 10;
        object->scaleX = object->scaleY = scale / 10 + 0x10000;
        WaitFrames(1);
        i++;
        scale -= 0xc000;
    } while (i <= 10);
}
