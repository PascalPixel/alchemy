#include "types.h"
#include "map.h"

struct WorldPosition;

s32 CheckWorldMapCollisionRange(s32 unused, struct WorldPosition *position) {
    if ((u32)(GetWorldMapCollision(position) - 5) <= 7U) {
        return 0;
    }
    return -1;
}
