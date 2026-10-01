@ Reduced near miss: the localized resident camera entry was misidentified;
@ Func_080c85c0 is not exported by these maintained source compositions.
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Func_020029b0
	.thumb_func
Func_020029b0:
	overlay_veneer Func_080c85c0
