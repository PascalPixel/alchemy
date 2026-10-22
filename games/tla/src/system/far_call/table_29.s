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
	overlay_veneer BattleEffect_FinishSceneAndReleaseHeapBlock
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_FinishSceneAndReleaseHeapBlock
.else
	overlay_veneer EventRuntime_ResolvePendingActions
.endif
.endif
	.global Func_080c84e8
	.thumb_func
Func_080c84e8:
.ifdef TLA_EDITION_ES
	overlay_veneer Ui_FillBank15PaletteGrey
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Ui_FillBank15PaletteGrey
.else
	overlay_veneer Func_080dc978
.endif
.endif
	.global Func_080c84f0
	.thumb_func
Func_080c84f0:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080dcadc
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080dcadc
.else
	overlay_veneer BattleEffect_FinishSceneAndReleaseHeapBlock
.endif
.endif
	.global Func_080c84f8
	.thumb_func
Func_080c84f8:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_HasMatchingEvent5
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_HasMatchingEvent5
.else
	overlay_veneer Ui_FillBank15PaletteGrey
.endif
.endif
	.global Func_080c8500
	.thumb_func
Func_080c8500:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_HasTrigger
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_HasTrigger
.else
	overlay_veneer Func_080dcadc
.endif
.endif
