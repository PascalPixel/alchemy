#include "TYPES.H"
#include "SYSTEM.H"

/* An object whose word at 0x28 stays nonzero while it is busy. */
struct BusyObject {
    u8 unknown_00[0x28];
    s32 busy;
};

/* Waits a frame at a time, at most sixty, until the object is idle, as ☀️'s
   OverlayObject_WaitUntilIdle does. */
void OverlayObject_WaitUntilIdle(struct BusyObject *object)
{
    s32 count;
    s32 busy;

    count = 60;
    while (count != 0) {
        WaitFrames(1);
        busy = object->busy;
        count--;
        if (busy == 0) {
            break;
        }
    }
}
