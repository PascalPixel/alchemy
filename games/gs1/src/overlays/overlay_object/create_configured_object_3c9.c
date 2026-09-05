#define CreateOverlayObject Func_02005d1e
#define SetOverlayObjectMode Func_02005d88
#define SetOverlayObjectSlot Func_02005ed0

#include "create_configured_overlay_object.h"

#define OverlayObject_CreateConfiguredObject Func_020000a0

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "create_configured_overlay_object_body.inc"
}
