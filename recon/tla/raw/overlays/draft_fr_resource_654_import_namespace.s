@ Preserved import near miss before identifying the complete resident callees.
@ FR: complete scene 18968 bytes; 1 import literal bytes differ.
@ These reduced veneers retain the original source target names.
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Func_020039a0
	.thumb_func
Func_020039a0:
	overlay_veneer Func_080201b0
