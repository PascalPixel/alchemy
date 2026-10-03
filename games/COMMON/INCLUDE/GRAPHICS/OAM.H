#ifndef ALCHEMY_OAM_H
#define ALCHEMY_OAM_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

struct OamUsage {
    u16 used;
    u16 limit;
};

LAYOUT_SIZE_GUARD(OamUsage_Size, struct OamUsage, 4);
LAYOUT_OFFSET_GUARD(OamUsage_Limit, struct OamUsage, limit, 2);

extern struct OamUsage gOamUsage;
void Render_BuildOamList(void *work);

#endif
