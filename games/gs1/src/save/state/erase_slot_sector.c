#include "types.h"
#include "scene.h"

typedef u16 (*Callback_08005904)(u16);
extern Callback_08005904 gOv;

u16 SaveState_EraseSlotSector(u16 value)
{
    return gOv(value);
}
