#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/state/interaction/add_parts.h"
#include "add_parts_record.h"

extern SoftFloatRecord *GetSoftFloatNaNRecord(void);

SoftFloatRecord *AddSoftFloatParts(SoftFloatRecord *left,
                                   SoftFloatRecord *right,
                                   SoftFloatRecord *result)
{
#include "add_parts_body.inc"
}
