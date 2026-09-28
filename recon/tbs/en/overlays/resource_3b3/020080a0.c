/* resource_3b3 0x020080a0 OverlayObject_CreateConfiguredObject, written
 * against HASHIRA.H; compiles exactly. Remaining difference: its shared body
 * include names the object veneers CreateOverlayObject, SetOverlayObjectMode
 * and SetOverlayObjectSlot, while the stage's other code reaches the same
 * veneers as Engine_ObjectCreate, Engine_ActorSetSpriteFlags and
 * Engine_ObjectSetPalette; one veneer takes one name. */
#include "HASHIRA.H"

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "CREATE_CONFIGURED_OVERLAY_OBJECT_BODY.INC"
}
