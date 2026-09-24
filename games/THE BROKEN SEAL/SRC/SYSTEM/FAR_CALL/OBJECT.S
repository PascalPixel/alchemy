@ Far-call stub table heading the object motion and battle effect code at 0x0808a000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global Func_0808a000
	.thumb_func
Func_0808a000:
	.irp target, 0x0808c4f9
	overlay_veneer \target
	.endr
	.global Game_ResetForNewGameFar
	.global Func_0808a008
	.thumb_func
Game_ResetForNewGameFar:
Func_0808a008:
	.irp target, 0x0808a8e5, 0x0809163d, 0x080916b1, 0x08091751
	overlay_veneer \target
	.endr
	.irp target, 0x08091781, 0x080917ad, 0x0808b869, 0x080917c5
	overlay_veneer \target
	.endr
	.irp target, 0x080917d1, 0x080917f5, 0x08091891, 0x08091a59
	overlay_veneer \target
	.endr
	.irp target, 0x08091c1d, 0x08091c7d, 0x08091d95
	overlay_veneer \target
	.endr
	.global Object_GetByIdFar
	.global Func_0808a080
	.thumb_func
Object_GetByIdFar:
Func_0808a080:
	.irp target, 0x08092055, 0x08092925, 0x08092065, 0x0809207d
	overlay_veneer \target
	.endr
	.irp target, 0x080920e9, 0x080920a1, 0x080920fd, 0x08092129
	overlay_veneer \target
	.endr
	.irp target, 0x08092159, 0x0809218d, 0x080921c5, 0x08092209
	overlay_veneer \target
	.endr
	.irp target, 0x0809228d, 0x080923c5, 0x080923e5, 0x08092455
	overlay_veneer \target
	.endr
	.irp target, 0x080924d5, 0x080924ed, 0x08092549, 0x08092505
	overlay_veneer \target
	.endr
	.irp target, 0x0809255d, 0x08092561, 0x0809259d, 0x080925cd
	overlay_veneer \target
	.endr
	.irp target, 0x08092709, 0x0809280d, 0x08092849, 0x08092951
	overlay_veneer \target
	.endr
	.irp target, 0x080929d9, 0x08092a1d, 0x08092b95, 0x08092c41
	overlay_veneer \target
	.endr
	.irp target, 0x08092f85, 0x08093041, 0x08093055, 0x080930b9
	overlay_veneer \target
	.endr
	.irp target, 0x080930bd, 0x0809315d, 0x08093169, 0x08092add
	overlay_veneer \target
	.endr
	.irp target, 0x08092ab5, 0x080931d5, 0x080931ed, 0x08093305
	overlay_veneer \target
	.endr
	.irp target, 0x08092b09, 0x080937b9, 0x08093875, 0x08092b55
	overlay_veneer \target
	.endr
	.irp target, 0x0809335d, 0x080933d5, 0x080933f9, 0x08093531
	overlay_veneer \target
	.endr
	.irp target, 0x08093501, 0x08093555, 0x080935b1
	overlay_veneer \target
	.endr
	.global Event_SetPairWork1c0Far
	.global Func_0808a238
	.thumb_func
Event_SetPairWork1c0Far:
Func_0808a238:
	.irp target, 0x08091e3d, 0x08091e6d, 0x08091e9d, 0x08091eb1
	overlay_veneer \target
	.endr
	.irp target, 0x08091f15, 0x08091f91, 0x08091fa9, 0x08091fc1
	overlay_veneer \target
	.endr
	.global Object_GetScreenPositionFar
	.global Func_0808a278
	.thumb_func
Object_GetScreenPositionFar:
Func_0808a278:
	.irp target, 0x08094155, 0x0808ce75, 0x0808b399, 0x08097c3d
	overlay_veneer \target
	.endr
	.irp target, 0x08099839, 0x08093c01, 0x08093e29, 0x08093fa1
	overlay_veneer \target
	.endr
	.irp target, 0x08091661, 0x08094ac9, 0x08095161, 0x08095215
	overlay_veneer \target
	.endr
	.irp target, 0x08095241, 0x08095269, 0x08095291, 0x08094da1
	overlay_veneer \target
	.endr
	.irp target, 0x0809509d, 0x0808fe39, 0x08090825, 0x0808fefd
	overlay_veneer \target
	.endr
	.irp target, 0x080901c1, 0x08090379, 0x08091175
	overlay_veneer \target
	.endr
	.global BattleFx_ApplyColorToTargetBufferFar
	.global Func_0808a330
	.thumb_func
BattleFx_ApplyColorToTargetBufferFar:
Func_0808a330:
	.irp target, 0x08091201, 0x08091221, 0x0809088d
	overlay_veneer \target
	.endr
	.global BattleFx_StartBufferInterpolationFar
	.global Func_0808a348
	.thumb_func
BattleFx_StartBufferInterpolationFar:
Func_0808a348:
	.irp target, 0x08091255, 0x080911e9, 0x08091241
	overlay_veneer \target
	.endr
	.global Event_SetStatus1c6Far
	.global Func_0808a360
	.thumb_func
Event_SetStatus1c6Far:
Func_0808a360:
	.irp target, 0x08091dc9, 0x08091df5, 0x08091e21, 0x08094731
	overlay_veneer \target
	.endr
	.irp target, 0x080947e5, 0x0808e079, 0x0808f141, 0x0808f1c1
	overlay_veneer \target
	.endr
	.irp target, 0x0808ed4d, 0x0808ece1, 0x0808ec51, 0x0808ed1d
	overlay_veneer \target
	.endr
	.irp target, 0x0808edad, 0x0808ed79, 0x0808ee0d, 0x0808c44d
	overlay_veneer \target
	.endr
	.irp target, 0x0808c4c1, 0x0808b675, 0x0808b3ed, 0x0808b64d
	overlay_veneer \target
	.endr
	.irp target, 0x0808ba1d, 0x08091495, 0x08091541, 0x08091551
	overlay_veneer \target
	.endr
	.irp target, 0x080967e5, 0x08096fb1, 0x080970f9, 0x08097175
	overlay_veneer \target
	.endr
	.irp target, 0x08097195, 0x0809728d, 0x08097385, 0x0809748d
	overlay_veneer \target
	.endr
	.global Battle_ResetEffectCounterFar
	.global Func_0808a460
	.thumb_func
Battle_ResetEffectCounterFar:
Func_0808a460:
	.irp target, 0x0808e119, 0x08097541, 0x08097609, 0x08097a7d
	overlay_veneer \target
	.endr
	.irp target, 0x08097add
	overlay_veneer \target
	.endr
	.global BattleFx_HasMatchingEvent5Far
	.global Func_0808a488
	.thumb_func
BattleFx_HasMatchingEvent5Far:
Func_0808a488:
	.irp target, 0x0808e96d, 0x0808e991, 0x08096c49
	overlay_veneer \target
	.endr
	.global Func_0808a4a0
	.thumb_func
Func_0808a4a0:
	.irp target, 0x0808b249, 0x08098071, 0x08098185, 0x080981b1
	overlay_veneer \target
	.endr
	.irp target, 0x0808c2dd, 0x0808c30d, 0x08096c25, 0x08091d85
	overlay_veneer \target
	.endr
	.irp target, 0x0808b05d, 0x0808b075, 0x0808acc5, 0x080955b1
	overlay_veneer \target
	.endr
	.global EffectSlot_UpdateFar
	.global Func_0808a500
	.thumb_func
EffectSlot_UpdateFar:
Func_0808a500:
	.irp target, 0x0809b805, 0x0809ba35, 0x0809ba5d, 0x0809ba71
	overlay_veneer \target
	.endr
	.irp target, 0x0809ba7d, 0x0809ba91, 0x0809bb35, 0x080974d9
	overlay_veneer \target
	.endr
	.global BattleFx_GetResourceIdFar
	.global Func_0808a540
	.thumb_func
BattleFx_GetResourceIdFar:
Func_0808a540:
	.irp target, 0x080915ad, 0x08091859, 0x080958a9, 0x080958e5
	overlay_veneer \target
	.endr
	.irp target, 0x0808b8e9, 0x0808b98d, 0x0809233d, 0x080922c5
	overlay_veneer \target
	.endr
	.irp target, 0x08092305, 0x080920c1, 0x0809ad91, 0x0809ade9
	overlay_veneer \target
	.endr
	.irp target, 0x08092be1, 0x08092ba9
	overlay_veneer \target
	.endr
	.global SaveState_CaptureObjectTableFar
	.global Func_0808a5b0
	.thumb_func
SaveState_CaptureObjectTableFar:
Func_0808a5b0:
	.irp target, 0x0808ba39, 0x0808bb2d, 0x080936a1, 0x08093711
	overlay_veneer \target
	.endr
	.global BattleFx_FindConditionResourceFar
	.global Func_0808a5d0
	.thumb_func
BattleFx_FindConditionResourceFar:
Func_0808a5d0:
	.irp target, 0x0808b159, 0x08093a6d, 0x08091ff1, 0x0809202d
	overlay_veneer \target
	.endr
	.global Func_0808a5f0
	.thumb_func
Func_0808a5f0:
	.irp target, 0x0808b3d1
	overlay_veneer \target
	.endr
