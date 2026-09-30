.syntax unified
	.thumb
	.global Func_080d3164
	.thumb_func
Func_080d3164:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	lsls r3, r3, #16
	asrs r3, r3, #16
	adds r6, r1, #0
	adds r7, r2, #0
	mov r8, r3
	bl ObjectTable_Get
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080d31ba
	bl Object_ResetMotion
	ldr r2, [r5, #12]
	adds r3, r7, #0
	adds r0, r5, #0
	adds r1, r6, #0
	bl Object_SetPositionAndResetMotionFar
	mov r3, r8
	strh r3, [r5, #6]
	adds r3, r5, #0
	adds r3, #85
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080d31ba
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r2, [r5, #16]
	ldr r1, [r5, #8]
	bl Func_080201c0
	ldr r3, [r5, #12]
	ldr r2, [r5, #20]
	str r0, [r5, #20]
	subs r3, r3, r2
	adds r3, r3, r0
	str r3, [r5, #12]
.L_080d31ba:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
