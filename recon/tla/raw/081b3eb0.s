.syntax unified
	.thumb
	.global Func_081b3eb0
	.thumb_func
Func_081b3eb0:
	push {lr}
	ldr r3, .L_081b3ed4
	ldr r1, [r3]
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r4, r3, #4
	ldr r0, [r1, r4]
	cmp r0, #0
	beq .L_081b3ed2
	ldr r2, [r1, r3]
	cmp r2, #0
	beq .L_081b3ecc
	str r0, [r2, #4]
.L_081b3ecc:
	ldr r2, [r1, r4]
	ldr r3, [r1, r3]
	str r3, [r2]
.L_081b3ed2:
	pop {pc}
.L_081b3ed4:
	.4byte Flash_Handler3
