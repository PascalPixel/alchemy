.syntax unified
	.thumb
	.global ObjectMotion_WaitForAnimationChange
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	push {r5, r6, r7, lr}
	sub sp, #4
	bl ObjectTable_Get
	cmp r0, #0
	beq .L_080d32b2
	adds r3, r0, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_080d32b2
	ldr r5, [r0, #80]
	mov r6, sp
	ldrb r3, [r5, #24]
	movs r7, #0
	str r3, [r6]
	b .L_080d328c
.L_080d328a:
	adds r7, #1
.L_080d328c:
	cmp r7, #89
	bgt .L_080d32b2
	movs r0, #1
	bl WaitFrames
	ldrb r2, [r5, #24]
	ldr r3, [r6]
	cmp r3, r2
	bne .L_080d32b2
	ldr r3, [r5, #40]
	cmp r3, #0
	beq .L_080d328a
	ldr r2, [r3, #16]
	cmp r2, #0
	beq .L_080d328a
	ldrb r3, [r3, #20]
	ldrb r3, [r2, r3]
	cmp r3, #241
	bne .L_080d328a
.L_080d32b2:
	add sp, #4
	pop {r5, r6, r7, pc}
	.2byte 0x0000
