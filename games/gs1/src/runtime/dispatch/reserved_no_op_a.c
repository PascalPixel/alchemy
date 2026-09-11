#include "types.h"
#include "scene.h"

/* Reserved ROM-to-IWRAM dispatch target; its complete body is `bx lr`. */
void RuntimeDispatch_ReservedNoOpA(void)
{
}
