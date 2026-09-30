.syntax unified
	.thumb
	.global Func_08025038
	.thumb_func
Func_08025038:
	push {r5, lr}
	adds r5, r0, #0
	ldr r2, [r5, #12]
	movs r3, #0
	movs r1, #0
	bl Object_SetPositionAndResetMotion
	adds r3, r5, #0
	adds r3, #85
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0802506e
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r2, [r5, #16]
	ldr r1, [r5, #8]
	bl Func_0802d45c
	ldr r3, [r5, #12]
	ldr r2, [r5, #20]
	str r0, [r5, #20]
	subs r3, r3, r2
	adds r3, r3, r0
	str r3, [r5, #12]
.L_0802506e:
	ldrh r3, [r5, #4]
	movs r0, #1
	adds r3, #1
	strh r3, [r5, #4]
	pop {r5, pc}
