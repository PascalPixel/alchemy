.syntax unified
	.thumb
	.global Func_0803cca8
	.thumb_func
Func_0803cca8:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	movs r4, #152
	lsls r4, r4, #5
	adds r4, #108
	adds r2, r3, r4
	subs r4, #32
	movs r1, #0
	movs r0, #0
	adds r3, r3, r4
.L_0803ccc0:
	adds r1, #1
	stmia r3!, {r0}
	strh r0, [r2]
	adds r2, #2
	cmp r1, #8
	bne .L_0803ccc0
	pop {pc}
	.2byte 0x0000
