.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_080c8828
	.thumb_func
Func_080c8828:
	overlay_veneer Func_080e4244
	.global Func_080c8830
	.thumb_func
Func_080c8830:
	overlay_veneer Func_080dbdc8
	.global Func_080c8838
	.thumb_func
Func_080c8838:
	overlay_veneer Func_080dbde8
	.global Func_080c8840
	.thumb_func
Func_080c8840:
	overlay_veneer Func_080dbdd4
	.global Func_080c8848
	.thumb_func
Func_080c8848:
	overlay_veneer Func_080dbdf4
	.global Func_080c8850
	.thumb_func
Func_080c8850:
	overlay_veneer Func_080dbe08
	.global Func_080c8858
	.thumb_func
Func_080c8858:
	overlay_veneer Func_080dbe44
	.global Func_080c8860
	.thumb_func
Func_080c8860:
	overlay_veneer Func_080d3b28
	.global Func_080c8868
	.thumb_func
Func_080c8868:
	overlay_veneer Func_080da908
	.global Func_080c8870
	.thumb_func
Func_080c8870:
	overlay_veneer Func_080da938
	.global Func_080c8878
	.thumb_func
Func_080c8878:
	overlay_veneer Func_080d4ab4
	.global Func_080c8880
	.thumb_func
Func_080c8880:
	overlay_veneer Func_080e3698
	.global Func_080c8888
	.thumb_func
Func_080c8888:
	overlay_veneer Func_080e37e0
	.global Func_080c8890
	.thumb_func
Func_080c8890:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_FillOverlayTiles
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_FillOverlayTiles
	.else
	overlay_veneer BattleEffect_DropObjectAtAnchor
	.endif
	.endif
	.global Func_080c8898
	.thumb_func
Func_080c8898:
	overlay_veneer Func_080d2cc4
	.global Func_080c88a0
	.thumb_func
Func_080c88a0:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_DrawSpriteAtWorldPosition
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_DrawSpriteAtWorldPosition
	.else
	overlay_veneer BattleEffect_FillOverlayTiles
	.endif
	.endif
	.global Func_080c88a8
	.thumb_func
Func_080c88a8:
	overlay_veneer Func_080d1dac
	.global Func_080c88b0
	.thumb_func
Func_080c88b0:
	.ifdef TLA_EDITION_ES
	overlay_veneer ObjectGroup_ApplyRandomChildValues
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer ObjectGroup_ApplyRandomChildValues
	.else
	overlay_veneer BattleEffect_DrawSpriteAtWorldPosition
	.endif
	.endif
	.global Func_080c88b8
	.thumb_func
Func_080c88b8:
	ldr r4, [pc, #0]
	bx r4
	.ifdef TLA_EDITION_ES
	.global BattleEffect_DecodeFrameTranslationBase
BattleEffect_DecodeFrameTranslationBase:
	.endif
	.ifdef TLA_EDITION_IT
	.global BattleEffect_DecodeFrameTranslationBase
BattleEffect_DecodeFrameTranslationBase:
	.endif
	.4byte Func_080d1e18
	.global ObjectGroup_ApplyRandomChildValuesFar
	.thumb_func
ObjectGroup_ApplyRandomChildValuesFar:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080e2774
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080e2774
.else
	overlay_veneer ObjectGroup_ApplyRandomChildValues
.endif
.endif
