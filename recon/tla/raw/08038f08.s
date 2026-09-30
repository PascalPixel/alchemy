.syntax unified
	.thumb
	.global Func_08038f08
	.thumb_func
Func_08038f08:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #60]
	movs r1, #230
	lsls r1, r1, #3
	adds r2, r0, r1
	movs r1, #227
	lsls r1, r1, #4
	adds r3, r0, r1
	str r2, [r3]
	movs r3, #62
.L_08038f20:
	adds r1, r2, #0
	adds r1, #28
	subs r3, #1
	str r1, [r2]
	adds r2, r1, #0
	cmp r3, #0
	bge .L_08038f20
	movs r2, #224
	lsls r2, r2, #4
	movs r3, #0
	adds r2, #52
	str r3, [r1]
	adds r3, r0, r2
	str r1, [r3]
	pop {pc}
	.2byte 0x0000
