/// @file
/// Memory: zero a nonzero word.

#include "types.h"
#include "scene.h"

/// Clears *p if nonzero and returns p.
s32 *Runtime_ClearWordIfSet(s32 *p) {
  if (*p != 0) {
    *p = 0;
  }
  return p;
}
