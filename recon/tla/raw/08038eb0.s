.syntax unified
	.thumb
	.global Func_08038eb0
	.thumb_func
Func_08038eb0:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	movs r2, #227
	lsls r2, r2, #4
	adds r1, r3, r2
	ldr r0, [r1]
	cmp r0, #0
	beq .L_08038eda
	ldr r2, [r0]
	cmp r2, #0
	bne .L_08038ed4
	movs r4, #224
	lsls r4, r4, #4
	adds r4, #52
	adds r3, r3, r4
	str r1, [r3]
.L_08038ed4:
	movs r3, #0
	str r2, [r1]
	str r3, [r0]
.L_08038eda:
	pop {pc}
