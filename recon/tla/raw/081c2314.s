.syntax unified
	.thumb
	.global Func_081c2314
	.thumb_func
Func_081c2314:
	push {lr}
	ldr r1, .L_081c2324
	ldr r1, [r1]
	bl _call_via_r1
	pop {r0}
	bx r0
	.2byte 0x0000
.L_081c2324:
	.4byte Data_02006888
