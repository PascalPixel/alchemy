/// @file
/// Memory: zero a nonzero word.

#include "types.h"
#include "scene.h"

/// Clear *p if nonzero and return p.
s32 *Runtime_ClearWordIfSet(s32 *p) {
  if (*p != 0) {
    *p = 0;
  }
  return p;
}
