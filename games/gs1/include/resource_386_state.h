#ifndef ALCHEMY_RESOURCE_386_STATE_H
#define ALCHEMY_RESOURCE_386_STATE_H

#include "layout_guard.h"

/* The first accessor exposes only the signed word consumed at +0x08. */
struct Resource386FirstView {
    u8 unknown_00[8];
    s32 sample_08;
};

/* The second accessor exposes only the signed word consumed at +0x10. */
struct Resource386SecondView {
    u8 unknown_00[0x10];
    s32 sample_10;
};

LAYOUT_OFFSET_GUARD(
    Resource386FirstView_SampleOffset,
    struct Resource386FirstView,
    sample_08,
    0x08);
LAYOUT_OFFSET_GUARD(
    Resource386SecondView_SampleOffset,
    struct Resource386SecondView,
    sample_10,
    0x10);

/* These names describe only the observed branch and return-value roles. */

void Resource386_OnWindowMatch(s32);
void Resource386_OnWindowMiss(s32);
void *Resource386_GetFirstView(s32);
void *Resource386_GetSecondView(s32);

#endif
