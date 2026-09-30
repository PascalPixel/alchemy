.syntax unified
	.thumb
	.global Func_0813bba0
	.thumb_func
Func_0813bba0:
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #176
	ldrh r1, [r3, #10]
	movs r2, #197
	lsls r2, r2, #8
	adds r2, #255
	ands r2, r1
	strh r2, [r3, #10]
	movs r2, #254
	ldrh r1, [r3, #10]
	lsls r2, r2, #7
	adds r2, #255
	ands r2, r1
	strh r2, [r3, #10]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r3, #10]
	ldr r0, .L_0813bbd0
	adds r1, #64
	ldr r2, .L_0813bbd4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bx lr
.L_0813bbd0:
	.4byte gMapCellBuffer
.L_0813bbd4:
	.4byte 0xa2600001
