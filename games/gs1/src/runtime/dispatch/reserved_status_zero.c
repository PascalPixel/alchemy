#include "types.h"

/* Reserved ROM-to-IWRAM dispatch target with a constant success/status value. */
int RuntimeDispatch_ReservedStatusZero(void)
{
    return 0;
}
