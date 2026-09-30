.syntax unified
	.thumb
	.global Resource_FarCall00B
Resource_FarCall00B:
	.global Func_081a6000
	.thumb_func
Func_081a6000:
	ldr r4, .L_081a6004
	bx r4
.L_081a6004:
	.4byte Func_081a729c
