#include "types.h"
#include "motobj.h"

// Per-frame update: spins an object 1/8 turn a frame.
void ObjectMotion_AdvanceSpinAngle(struct MotionObject *object) { object->angle += 0x2000; }
