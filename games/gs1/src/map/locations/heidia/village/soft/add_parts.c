#include "types.h"
#include "scene.h"
#include "abi/map/locations/heidia/village/soft/add_parts.h"
#include "add_parts_record.h"

/* Second embedded copy of the compiler runtime's soft-float addition core. */

extern SoftFloatRecord *GetSoftFloatNaNRecord(void);

SoftFloatRecord *AddSoftFloatParts(SoftFloatRecord *left,
                                   SoftFloatRecord *right,
                                   SoftFloatRecord *result)
{
#include "add_parts_body.inc"
}
