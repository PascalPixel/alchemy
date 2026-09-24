@ Far-call stub table heading the party, item and flag code at 0x08077000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global Trade_GetOfferStateFar
	.global Func_08077000
	.thumb_func
Trade_GetOfferStateFar:
Func_08077000:
	.irp target, 0x08077331, 0x08077395, 0x08077429, 0x08078415
	overlay_veneer \target
	.endr
	.irp target, 0x080784b1, 0x08078589, 0x08078619, 0x08078665
	overlay_veneer \target
	.endr
	.irp target, 0x08078699
	overlay_veneer \target
	.endr
	.global PartyInventory_RemoveFar
	.global Func_08077048
	.thumb_func
PartyInventory_RemoveFar:
Func_08077048:
	.irp target, 0x080789dd, 0x08078709, 0x080788c5, 0x08078a35
	overlay_veneer \target
	.endr
	.irp target, 0x08078a61, 0x08078a8d
	overlay_veneer \target
	.endr
	.global Inventory_GetEquippedItemFar
	.global Func_08077078
	.thumb_func
Inventory_GetEquippedItemFar:
Func_08077078:
	.irp target, 0x08078871, 0x08078b9d, 0x08078e29, 0x08078bc1
	overlay_veneer \target
	.endr
	.irp target, 0x08077d39, 0x08078ecd, 0x08078ee9, 0x08079005
	overlay_veneer \target
	.endr
	.global Func_080770b8
	.thumb_func
Func_080770b8:
	.irp target, 0x080792c5, 0x08079339, 0x08079359, 0x08079375
	overlay_veneer \target
	.endr
	.irp target, 0x08079391
	overlay_veneer \target
	.endr
	.global GameFlag_GetByteFar
	.global Func_080770e0
	.thumb_func
GameFlag_GetByteFar:
Func_080770e0:
	.irp target, 0x080793b9, 0x080793c9, 0x080793d9, 0x080793f9
	overlay_veneer \target
	.endr
	.irp target, 0x08079419, 0x08079435, 0x08077321
	overlay_veneer \target
	.endr
	.global Owner_AdjustFirstValueFar
	.global Func_08077118
	.thumb_func
Owner_AdjustFirstValueFar:
Func_08077118:
	.irp target, 0x080783a5, 0x080783dd, 0x0807822d, 0x080782a1
	overlay_veneer \target
	.endr
	.irp target, 0x08078321
	overlay_veneer \target
	.endr
	.global Func_08077140
	.thumb_func
Func_08077140:
	.irp target, 0x08079461, 0x080795fd, 0x0807961d, 0x080796c5
	overlay_veneer \target
	.endr
	.irp target, 0x08079d1d, 0x08079665, 0x08079c8d, 0x08079f11
	overlay_veneer \target
	.endr
	.irp target, 0x08079bf9, 0x08079c31, 0x08079c5d, 0x080773d9
	overlay_veneer \target
	.endr
	.irp target, 0x08079bc5, 0x0807a1b5, 0x0807a2e5, 0x0807a351
	overlay_veneer \target
	.endr
	.irp target, 0x0807a3a9, 0x0807a459, 0x0807a499, 0x0807977d
	overlay_veneer \target
	.endr
	.irp target, 0x080797d5, 0x0807a5b1, 0x080792fd, 0x0807987d
	overlay_veneer \target
	.endr
	.irp target, 0x0807a551
	overlay_veneer \target
	.endr
	.global Func_08077208
	.thumb_func
Func_08077208:
	.irp target, 0x0807a2bd, 0x0807a1f9, 0x0807842d, 0x0807845d
	overlay_veneer \target
	.endr
	.irp target, 0x080787dd, 0x08079701, 0x08078481, 0x08078ad1
	overlay_veneer \target
	.endr
	.irp target, 0x080784d9, 0x08078501
	overlay_veneer \target
	.endr
	.global Owner_GetLevelThresholdFar
	.global Func_08077258
	.thumb_func
Owner_GetLevelThresholdFar:
Func_08077258:
	.irp target, 0x08079009, 0x0807808d, 0x08078145, 0x08077f41
	overlay_veneer \target
	.endr
	.irp target, 0x08077f71, 0x08078229
	overlay_veneer \target
	.endr
	.global Djinn_AddToLeastLoadedOwnerFar
	.global Func_08077288
	.thumb_func
Djinn_AddToLeastLoadedOwnerFar:
Func_08077288:
	.irp target, 0x0807a0f5, 0x0807a5bd, 0x08079729, 0x08079755
	overlay_veneer \target
	.endr
	.irp target, 0x08078981, 0x08078949, 0x08079ef9, 0x08078bf1
	overlay_veneer \target
	.endr
	.irp target, 0x08077349, 0x08079ae9
	overlay_veneer \target
	.endr
	.global Inventory_CountItemFar
	.global Func_080772d8
	.thumb_func
Inventory_CountItemFar:
Func_080772d8:
	.irp target, 0x08078af9, 0x08078b61, 0x08078a09
	overlay_veneer \target
	.endr
	.global GameFlag_RefreshLureCapFar
	.global Func_080772f0
	.thumb_func
GameFlag_RefreshLureCapFar:
Func_080772f0:
	.irp target, 0x08077c11, 0x08079cbd, 0x08077cb9, 0x08078551
	overlay_veneer \target
	.endr
	.irp target, 0x0807a665, 0x0807a7a1
	overlay_veneer \target
	.endr
