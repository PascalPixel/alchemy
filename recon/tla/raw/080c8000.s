.syntax unified
	.thumb
	.global Resource_FarCall006
Resource_FarCall006:
	.global Func_080c8000
	.thumb_func
Func_080c8000:
	ldr r4, .L_080c8004
	bx r4
.L_080c8004:
	.4byte Func_080cb91c
