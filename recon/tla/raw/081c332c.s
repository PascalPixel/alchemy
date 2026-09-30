.syntax unified
	.thumb
	.global Func_081c332c
	.thumb_func
Func_081c332c:
	push {lr}
	ldr r2, .L_081c333c
	ldr r2, [r2]
	bl _call_via_r2
	pop {r0}
	bx r0
	.2byte 0x0000
.L_081c333c:
	.4byte Sound_CommandTable
