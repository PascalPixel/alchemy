.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_080ad318
	.thumb_func
Func_080ad318:
	overlay_veneer Func_080adc90
	.global Func_080ad320
	.thumb_func
Func_080ad320:
	overlay_veneer Func_080ae868
	.global Owner_SumDjinnCountsFar
	.thumb_func
Owner_SumDjinnCountsFar:
	overlay_veneer Owner_SumDjinnCounts
	.global Func_080ad330
	.thumb_func
Func_080ad330:
	overlay_veneer Func_080ae5fc
