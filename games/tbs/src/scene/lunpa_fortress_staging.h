#ifndef LUNPA_FORTRESS_STAGING_H
#define LUNPA_FORTRESS_STAGING_H

#include "types.h"
#include "lunpa_fortress.h"
#include "map_tables.h"
#include "item_ids.h"

#include "pushed_object.h"
#include "object.h"
#include "object_event.h"

static __inline__ void SetFieldWorkValue(s16 *field, s32 value) {
  *field = value;
}

#endif
