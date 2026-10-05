#ifndef ALCHEMY_ARC_EFFECT_H
#define ALCHEMY_ARC_EFFECT_H

#include "TYPES.H"

/* The arc effect's work in heap block 62: two groups of two 40-byte arc
   objects, each group's age since it was spawned, the shared resource
   entry, the group spawned next and the object it follows. */
struct ArcEffectWork {
    u8 objects[2][80];
    s16 age[2];
    s16 resource;
    s16 group;
    s32 target;
};

void BattleFx_SpawnScaledArcObjects(s32 object);

#endif
