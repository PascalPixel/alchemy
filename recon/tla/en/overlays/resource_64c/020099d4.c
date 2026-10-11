/* Near miss, differ 8 of 22: after the cycle the native reads the frame
   counter again into r3 on both paths (ldr r3, [r6]; movs r2, #15); this
   draft keeps the first read in r0 and branches past the reload. A local for
   the first read, a reassigned local and an inline cycle helper compile the
   same; only a volatile second read matches, and no device is spent on it. */
#include "types.h"
#include "frame.h"
#include "object_runtime.h"

void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
void Func_02001ad0(struct ObjectRuntime *object);

void Func_020019d4(struct ObjectRuntime *object)
{
    u32 frame = gFrameCount;

    if (frame & 1)
        Animation_ApplyChildValues(object, (frame >> 1) % 6);
    if ((gFrameCount & 15) == 0)
        Func_02001ad0(object);
}
