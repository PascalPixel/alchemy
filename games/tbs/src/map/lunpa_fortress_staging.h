#ifndef LUNPA_FORTRESS_STAGING_H
#define LUNPA_FORTRESS_STAGING_H

#include "types.h"
#include "lunpa_fortress.h"
#include "map_tables.h"
#include "item_ids.h"

#include "staged_object_event.h"
#include "object_runtime.h"
#include "object_event.h"

static __inline__ void SetFieldWorkValue(s16 *field, s32 value) {
  *field = value;
}

#endif
