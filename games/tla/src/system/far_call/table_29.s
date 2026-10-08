.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global EventRuntime_ResolveAllPendingActionsFar
	.thumb_func
EventRuntime_ResolveAllPendingActionsFar:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080cdec8
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080cdec8
.else
	overlay_veneer EventRuntime_ResolveAllPendingActions
.endif
.endif
	.global EventRuntime_ResolvePendingActionsFar
	.thumb_func
EventRuntime_ResolvePendingActionsFar:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080cded4
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080cded4
.else
	overlay_veneer EventRuntime_ResolvePendingActions
.endif
.endif
	.global Func_080c84e8
	.thumb_func
Func_080c84e8:
	overlay_veneer Func_080dc978
	.global Func_080c84f0
	.thumb_func
Func_080c84f0:
	overlay_veneer Func_080dca50
	.global Func_080c84f8
	.thumb_func
Func_080c84f8:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_HasMatchingEvent5
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_HasMatchingEvent5
.else
	overlay_veneer Func_080dca84
.endif
.endif
	.global Func_080c8500
	.thumb_func
Func_080c8500:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_HasTrigger
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_HasTrigger
.else
	overlay_veneer Func_080dcadc
.endif
.endif
