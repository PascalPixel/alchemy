.syntax unified
	.thumb
	.global Func_080150ac
	.thumb_func
Func_080150ac:
	push {r5, r6, lr}
	adds r6, r0, #0
	ldr r5, .L_080150e0
	adds r0, r5, #0
	movs r1, #128
	lsls r1, r1, #9
	movs r2, #0
	movs r3, #0
	movs r4, #0
	stmia r0!, {r1, r2, r3, r4}
	stmia r0!, {r1, r2, r3, r4}
	stmia r0!, {r1, r2, r3, r4}
	movs r3, #128
	lsls r3, r3, #7
	adds r0, r6, r3
	bl Trig_Sin
	str r0, [r5]
	str r0, [r5, #32]
	adds r0, r6, #0
	bl Trig_Sin
	negs r3, r0
	str r3, [r5, #8]
	str r0, [r5, #24]
	pop {r5, r6, pc}
.L_080150e0:
	.4byte gTransform
