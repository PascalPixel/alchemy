.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Resource_FarCall007
Resource_FarCall007:
	.global Func_080f8000
	.thumb_func
Func_080f8000:
	overlay_veneer Func_080f94a4
	.global PsynergyMenu_RunFar
	.thumb_func
PsynergyMenu_RunFar:
	overlay_veneer Func_080fcab8
	.global Func_080f8010
	.thumb_func
Func_080f8010:
	overlay_veneer Func_080fe274
	.global Func_080f8018
	.thumb_func
Func_080f8018:
	overlay_veneer Func_080fe184
	.global Icon_CreateWithResourceVariantFar
	.thumb_func
Icon_CreateWithResourceVariantFar:
	overlay_veneer Icon_CreateWithResourceVariant
