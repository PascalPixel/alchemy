/* resource_396 0x020080a0..0x02008104 OverlayObject_CreateConfigured: compiles
 * exactly with GCC 2.96 against FIELD/TORETO_HEYA/HEYA.H.
 * Remaining difference: none in the code. The shared body include spells the
 * object veneers CreateOverlayObject, SetOverlayObjectMode and
 * SetOverlayObjectSlot, while the shared COMMON/EFFECT/SPAWN.C this overlay
 * links reaches the same veneers as Engine_ObjectCreate,
 * Engine_ActorSetSpriteFlags and Engine_ObjectSetPalette; one veneer takes one
 * name, so the rows stay in the listing until the shared sources agree. */
#include "HEYA.H"

void *OverlayObject_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "CREATE_CONFIGURED_OVERLAY_OBJECT_BODY.INC"
}
