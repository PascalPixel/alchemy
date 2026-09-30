.syntax unified
	.thumb
	.global Func_080d45b8
	.thumb_func
Func_080d45b8:
	movs r4, #192
	lsls r4, r4, #18
	ldr r4, [r4, #32]
	mov r12, r4
	adds r4, #236
	str r0, [r4]
	mov r0, r12
	adds r0, #240
	str r1, [r0]
	mov r1, r12
	adds r1, #244
	str r2, [r1]
	mov r2, r12
	adds r2, #248
	str r3, [r2]
	bx lr
