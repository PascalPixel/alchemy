.syntax unified
	.thumb
	.global Func_080d3214
	.thumb_func
Func_080d3214:
	push {r5, r6, lr}
	adds r5, r1, #0
	bl ObjectTable_Get
	adds r6, r0, #0
	adds r0, r5, #0
	bl ObjectTable_Get
	adds r5, r0, #0
	cmp r6, #0
	beq .L_080d323e
	cmp r5, #0
	beq .L_080d323e
	ldr r3, [r5, #16]
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	adds r0, r6, #0
	bl Object_SetPositionAndResetMotionFar
	ldr r3, [r5, #20]
	str r3, [r6, #20]
.L_080d323e:
	pop {r5, r6, pc}
