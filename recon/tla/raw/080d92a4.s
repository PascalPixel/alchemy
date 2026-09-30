.syntax unified
	.thumb
	.global Func_080d92a4
	.thumb_func
Func_080d92a4:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #156
	ldr r3, [r3]
	lsls r0, r0, #5
	adds r3, r3, r0
	adds r3, #12
	ldr r2, [r3, #28]
	movs r0, #4
	ldrsh r3, [r3, r0]
	cmp r3, #0
	ble .L_080d92c6
.L_080d92be:
	subs r3, #1
	ldr r2, [r2]
	cmp r3, #0
	bne .L_080d92be
.L_080d92c6:
	ldr r3, [r2, #4]
	str r3, [r1]
	ldr r3, [r2, #8]
	str r3, [r1, #4]
	ldr r3, [r2, #12]
	str r3, [r1, #8]
	pop {pc}
