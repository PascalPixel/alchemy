@ Far-call stub table at 0x080c8520: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c8520
	.thumb_func
Func_080c8520:
@ Spanish/Italian entries dispatch the scale clamp.
.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_SnapScaleToFull
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_SnapScaleToFull
.else
	overlay_veneer Event_GetSpecialValue
.endif
.endif
	.global BattleFx_StartItemBreakFar
	.thumb_func
BattleFx_StartItemBreakFar:
	overlay_veneer BattleFx_StartItemBreak
	.global BattleFx_SnapScaleToFullFar
	.thumb_func
BattleFx_SnapScaleToFullFar:
@ This entry changes the active party's second value in Spanish/Italian.
.ifdef TLA_EDITION_ES
	overlay_veneer BattleParty_ApplySecondValueDelta
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleParty_ApplySecondValueDelta
.else
	overlay_veneer BattleFx_SnapScaleToFull
.endif
.endif
	.global UpdateRisingParticleBurstFar
	.thumb_func
UpdateRisingParticleBurstFar:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleParty_ApplyHealthDelta
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleParty_ApplyHealthDelta
.else
	overlay_veneer UpdateRisingParticleBurst
.endif
.endif
	.global Func_080c8540
	.thumb_func
Func_080c8540:
	overlay_veneer Func_080cb6c8
	.global Func_080c8548
	.thumb_func
Func_080c8548:
.ifdef TLA_EDITION_ES
	overlay_veneer Menu_RunDefaultConfirmation
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Menu_RunDefaultConfirmation
.else
	overlay_veneer BattleParty_ApplyHealthDelta
.endif
.endif
	.global Func_080c8550
	.thumb_func
Func_080c8550:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_GetWeightedResult
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_GetWeightedResult
.else
	overlay_veneer Func_080dc0b8
.endif
.endif
	.global Func_080c8558
	.thumb_func
Func_080c8558:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_GetPhaseResult
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_GetPhaseResult
.else
	overlay_veneer Func_080d295c
.endif
.endif
	.global Func_080c8560
	.thumb_func
Func_080c8560:
.ifdef TLA_EDITION_ES
	overlay_veneer Audio_PlayPartyCue
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Audio_PlayPartyCue
.else
	overlay_veneer BattleFx_GetWeightedResult
.endif
.endif
	.global Func_080c8568
	.thumb_func
Func_080c8568:
.ifdef TLA_EDITION_ES
	overlay_veneer Audio_SelectPartyCueForArea
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Audio_SelectPartyCueForArea
.else
	overlay_veneer BattleFx_GetPhaseResult
.endif
.endif
	.global Func_080c8570
	.thumb_func
Func_080c8570:
	overlay_veneer Func_080c9dc8
	.global Func_080c8578
	.thumb_func
Func_080c8578:
	overlay_veneer Func_080ca1bc
	.global Func_080c8580
	.thumb_func
Func_080c8580:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_HasReachedTarget
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_HasReachedTarget
.else
	overlay_veneer Func_080d7788
.endif
.endif
