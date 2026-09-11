#include "types.h"
#include "scene.h"
#include "abi/field/event_table/get_row_limit.h"

/* types.h maps this semantic owner name back to Field_Run. */
#include "types.h"

int EventTable_GetRowLimit(void)
{
    return 35;
}
