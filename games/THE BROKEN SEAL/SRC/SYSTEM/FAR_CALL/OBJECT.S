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
	overlay_veneer 0x0808c4f9
	.global Game_ResetForNewGameFar
	.global Func_0808a008
	.thumb_func
Game_ResetForNewGameFar:
Func_0808a008:
	overlay_veneer 0x0808a8e5
	overlay_veneer 0x0809163d
	overlay_veneer 0x080916b1
	overlay_veneer 0x08091751
	overlay_veneer 0x08091781
	overlay_veneer 0x080917ad
	overlay_veneer 0x0808b869
	overlay_veneer 0x080917c5
	overlay_veneer 0x080917d1
	overlay_veneer 0x080917f5
	overlay_veneer 0x08091891
	overlay_veneer 0x08091a59
	overlay_veneer 0x08091c1d
	overlay_veneer 0x08091c7d
	overlay_veneer 0x08091d95
	.global Object_GetByIdFar
	.global Func_0808a080
	.thumb_func
Object_GetByIdFar:
Func_0808a080:
	overlay_veneer 0x08092055
	overlay_veneer 0x08092925
	overlay_veneer 0x08092065
	overlay_veneer 0x0809207d
	overlay_veneer 0x080920e9
	overlay_veneer 0x080920a1
	overlay_veneer 0x080920fd
	overlay_veneer 0x08092129
	overlay_veneer 0x08092159
	overlay_veneer 0x0809218d
	overlay_veneer 0x080921c5
	overlay_veneer 0x08092209
	overlay_veneer 0x0809228d
	overlay_veneer 0x080923c5
	overlay_veneer 0x080923e5
	overlay_veneer 0x08092455
	overlay_veneer 0x080924d5
	overlay_veneer 0x080924ed
	overlay_veneer 0x08092549
	overlay_veneer 0x08092505
	overlay_veneer 0x0809255d
	overlay_veneer 0x08092561
	overlay_veneer 0x0809259d
	overlay_veneer 0x080925cd
	overlay_veneer 0x08092709
	overlay_veneer 0x0809280d
	overlay_veneer 0x08092849
	overlay_veneer 0x08092951
	overlay_veneer 0x080929d9
	overlay_veneer 0x08092a1d
	overlay_veneer 0x08092b95
	overlay_veneer 0x08092c41
	overlay_veneer 0x08092f85
	overlay_veneer 0x08093041
	overlay_veneer 0x08093055
	overlay_veneer 0x080930b9
	overlay_veneer 0x080930bd
	overlay_veneer 0x0809315d
	overlay_veneer 0x08093169
	overlay_veneer 0x08092add
	overlay_veneer 0x08092ab5
	overlay_veneer 0x080931d5
	overlay_veneer 0x080931ed
	overlay_veneer 0x08093305
	overlay_veneer 0x08092b09
	overlay_veneer 0x080937b9
	overlay_veneer 0x08093875
	overlay_veneer 0x08092b55
	overlay_veneer 0x0809335d
	overlay_veneer 0x080933d5
	overlay_veneer 0x080933f9
	overlay_veneer 0x08093531
	overlay_veneer 0x08093501
	overlay_veneer 0x08093555
	overlay_veneer 0x080935b1
	.global Event_SetPairWork1c0Far
	.global Func_0808a238
	.thumb_func
Event_SetPairWork1c0Far:
Func_0808a238:
	overlay_veneer 0x08091e3d
	overlay_veneer 0x08091e6d
	overlay_veneer 0x08091e9d
	overlay_veneer 0x08091eb1
	overlay_veneer 0x08091f15
	overlay_veneer 0x08091f91
	overlay_veneer 0x08091fa9
	overlay_veneer 0x08091fc1
	.global Object_GetScreenPositionFar
	.global Func_0808a278
	.thumb_func
Object_GetScreenPositionFar:
Func_0808a278:
	overlay_veneer 0x08094155
	.global Object_GetTriggerTileAheadOfCurrentFar
	.thumb_func
Object_GetTriggerTileAheadOfCurrentFar:
	overlay_veneer 0x0808ce75
	.global Party_RemapCharacterIdByFlagsFar
	.thumb_func
Party_RemapCharacterIdByFlagsFar:
	overlay_veneer 0x0808b399
	overlay_veneer 0x08097c3d
	overlay_veneer 0x08099839
	overlay_veneer 0x08093c01
	overlay_veneer 0x08093e29
	overlay_veneer 0x08093fa1
	overlay_veneer 0x08091661
	overlay_veneer 0x08094ac9
	overlay_veneer 0x08095161
	overlay_veneer 0x08095215
	overlay_veneer 0x08095241
	overlay_veneer 0x08095269
	overlay_veneer 0x08095291
	overlay_veneer 0x08094da1
	overlay_veneer 0x0809509d
	overlay_veneer 0x0808fe39
	overlay_veneer 0x08090825
	overlay_veneer 0x0808fefd
	overlay_veneer 0x080901c1
	overlay_veneer 0x08090379
	overlay_veneer 0x08091175
	.global BattleFx_ApplyColorToTargetBufferFar
	.global Func_0808a330
	.thumb_func
BattleFx_ApplyColorToTargetBufferFar:
Func_0808a330:
	overlay_veneer 0x08091201
	overlay_veneer 0x08091221
	overlay_veneer 0x0809088d
	.global BattleFx_StartBufferInterpolationFar
	.global Func_0808a348
	.thumb_func
BattleFx_StartBufferInterpolationFar:
Func_0808a348:
	overlay_veneer 0x08091255
	overlay_veneer 0x080911e9
	overlay_veneer 0x08091241
	.global Event_SetStatus1c6Far
	.global Func_0808a360
	.thumb_func
Event_SetStatus1c6Far:
Func_0808a360:
	overlay_veneer 0x08091dc9
	.global Event_ClearStatus1c6Far
	.thumb_func
Event_ClearStatus1c6Far:
	overlay_veneer 0x08091df5
	.global Event_WaitValue1c8FramesFar
	.thumb_func
Event_WaitValue1c8FramesFar:
	overlay_veneer 0x08091e21
	overlay_veneer 0x08094731
	overlay_veneer 0x080947e5
	overlay_veneer 0x0808e079
	overlay_veneer 0x0808f141
	overlay_veneer 0x0808f1c1
	overlay_veneer 0x0808ed4d
	overlay_veneer 0x0808ece1
	overlay_veneer 0x0808ec51
	overlay_veneer 0x0808ed1d
	overlay_veneer 0x0808edad
	overlay_veneer 0x0808ed79
	overlay_veneer 0x0808ee0d
	overlay_veneer 0x0808c44d
	overlay_veneer 0x0808c4c1
	overlay_veneer 0x0808b675
	overlay_veneer 0x0808b3ed
	overlay_veneer 0x0808b64d
	overlay_veneer 0x0808ba1d
	overlay_veneer 0x08091495
	overlay_veneer 0x08091541
	overlay_veneer 0x08091551
	overlay_veneer 0x080967e5
	overlay_veneer 0x08096fb1
	overlay_veneer 0x080970f9
	overlay_veneer 0x08097175
	overlay_veneer 0x08097195
	overlay_veneer 0x0809728d
	overlay_veneer 0x08097385
	overlay_veneer 0x0809748d
	.global Battle_ResetEffectCounterFar
	.global Func_0808a460
	.thumb_func
Battle_ResetEffectCounterFar:
Func_0808a460:
	overlay_veneer 0x0808e119
	overlay_veneer 0x08097541
	overlay_veneer 0x08097609
	overlay_veneer 0x08097a7d
	overlay_veneer 0x08097add
	.global BattleFx_HasMatchingEvent5Far
	.global Func_0808a488
	.thumb_func
BattleFx_HasMatchingEvent5Far:
Func_0808a488:
	overlay_veneer 0x0808e96d
	.global BattleFx_HasTriggerFar
	.thumb_func
BattleFx_HasTriggerFar:
	overlay_veneer 0x0808e991
	overlay_veneer 0x08096c49
	.global Func_0808a4a0
	.thumb_func
Func_0808a4a0:
	overlay_veneer 0x0808b249
	overlay_veneer 0x08098071
	overlay_veneer 0x08098185
	overlay_veneer 0x080981b1
	overlay_veneer 0x0808c2dd
	overlay_veneer 0x0808c30d
	overlay_veneer 0x08096c25
	overlay_veneer 0x08091d85
	overlay_veneer 0x0808b05d
	overlay_veneer 0x0808b075
	overlay_veneer 0x0808acc5
	overlay_veneer 0x080955b1
	.global EffectSlot_UpdateFar
	.global Func_0808a500
	.thumb_func
EffectSlot_UpdateFar:
Func_0808a500:
	overlay_veneer 0x0809b805
	.global BattleFx_HasReachedTargetFar
	.thumb_func
BattleFx_HasReachedTargetFar:
	overlay_veneer 0x0809ba35
	.global EffectSlot_SetPositionFar
	.thumb_func
EffectSlot_SetPositionFar:
	overlay_veneer 0x0809ba5d
	.global Func_0808a518
	.thumb_func
Func_0808a518:
	overlay_veneer 0x0809ba71
	.global Func_0808a520
	.thumb_func
Func_0808a520:
	overlay_veneer 0x0809ba7d
	.global Func_0808a528
	.thumb_func
Func_0808a528:
	overlay_veneer 0x0809ba91
	.global BattleFx_ClearOwnedSlotFar
	.thumb_func
BattleFx_ClearOwnedSlotFar:
	.global Func_0808a530
	.thumb_func
Func_0808a530:
	overlay_veneer 0x0809bb35
	overlay_veneer 0x080974d9
	.global BattleFx_GetResourceIdFar
	.global Func_0808a540
	.thumb_func
BattleFx_GetResourceIdFar:
Func_0808a540:
	overlay_veneer 0x080915ad
	.global Event_ClearInvalidPackedValuesFar
	.thumb_func
Event_ClearInvalidPackedValuesFar:
	overlay_veneer 0x08091859
	overlay_veneer 0x080958a9
	overlay_veneer 0x080958e5
	overlay_veneer 0x0808b8e9
	overlay_veneer 0x0808b98d
	overlay_veneer 0x0809233d
	overlay_veneer 0x080922c5
	overlay_veneer 0x08092305
	overlay_veneer 0x080920c1
	overlay_veneer 0x0809ad91
	overlay_veneer 0x0809ade9
	overlay_veneer 0x08092be1
	overlay_veneer 0x08092ba9
	.global SaveState_CaptureObjectTableFar
	.global Func_0808a5b0
	.thumb_func
SaveState_CaptureObjectTableFar:
Func_0808a5b0:
	overlay_veneer 0x0808ba39
	overlay_veneer 0x0808bb2d
	overlay_veneer 0x080936a1
	overlay_veneer 0x08093711
	.global BattleFx_FindConditionResourceFar
	.global Func_0808a5d0
	.thumb_func
BattleFx_FindConditionResourceFar:
Func_0808a5d0:
	overlay_veneer 0x0808b159
	overlay_veneer 0x08093a6d
	overlay_veneer 0x08091ff1
	overlay_veneer 0x0809202d
	.global Func_0808a5f0
	.thumb_func
Func_0808a5f0:
	overlay_veneer 0x0808b3d1
