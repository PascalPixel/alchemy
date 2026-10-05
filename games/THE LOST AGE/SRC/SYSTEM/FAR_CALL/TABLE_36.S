.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.global Func_080c8950
	.thumb_func
Func_080c8950:
	overlay_veneer Func_080d072c
	.global Func_080c8958
	.thumb_func
Func_080c8958:
	overlay_veneer Func_080d25c8
	.global Func_080c8960
	.thumb_func
Func_080c8960:
.if EDITION_INTERNATIONAL
	overlay_veneer Func_080ccd78
.else
	overlay_veneer SceneEvent_FindActiveRecord
.endif
	.global Func_080c8968
	.thumb_func
Func_080c8968:
	overlay_veneer Func_080d1760
