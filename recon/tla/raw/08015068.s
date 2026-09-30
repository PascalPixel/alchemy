.syntax unified
	.thumb
	.global Func_08015068
	.thumb_func
Func_08015068:
	push {r5, r6, lr}
	sub sp, #48
	adds r6, r0, #0
	mov r5, sp
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
	ldr r3, .L_080150a8
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	add sp, #48
	pop {r5, r6, pc}
.L_080150a8:
	.4byte IwramTransformMatrix
