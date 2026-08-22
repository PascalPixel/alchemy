#include "resource_384.h"

/* Overlay-owned descriptor block exported through the header veneer table. */
u8 *Func_02000030(void)
{
    return RESOURCE384_HEADER_DATA;
}
