.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Resource_FarCall00E
Resource_FarCall00E:
	.global Func_081b2000
	.thumb_func
Func_081b2000:
	overlay_veneer Func_081b2008
