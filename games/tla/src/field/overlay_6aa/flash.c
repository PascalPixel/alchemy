#include "ride.h"

void Func_02002614(struct ObjectRuntime *object)
{
    switch (gFrameCount & 7) {
    case 0:
        Animation_ApplyChildValues(object, 10);
        break;
    case 4:
        Animation_ApplyChildValues(object, 0);
        break;
    }
}
