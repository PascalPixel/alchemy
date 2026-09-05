#define CreateOverlayObject Func_02001132
#define SetOverlayObjectMode Func_0200117c
#define SetOverlayObjectSlot Func_02001244

#include "create_configured_overlay_object.h"

#define OverlayObject_CreateConfiguredObjectB Func_020000a0

void *OverlayObject_CreateConfiguredObjectB(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "create_configured_overlay_object_body.inc"
}
