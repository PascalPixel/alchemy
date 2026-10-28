#include "types.h"
#include "actor.h"

void Actor_Destroy(struct Actor *object);

void Actor_ReleaseIfPresent(struct Actor *object) {
  if (object != 0)
    Actor_Destroy(object);
}

/// Nothing in the image calls it.
void Func_080cf34c(void) {}
