.syntax unified
	.thumb
	.global Func_0802c98c
	.thumb_func
Func_0802c98c:
	movs r2, #168
	movs r3, #128
	lsls r2, r2, #8
	lsls r3, r3, #19
	adds r2, #10
	adds r3, #14
	strh r2, [r3]
	movs r2, #170
	lsls r2, r2, #8
	adds r2, #14
	subs r3, #2
	strh r2, [r3]
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #1
	subs r3, #2
	strh r2, [r3]
	adds r3, #202
	ldr r0, .L_0802c9bc
	ldr r1, .L_0802c9c0
	ldr r2, .L_0802c9c4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bx lr
.L_0802c9bc:
	.4byte Data_02038000
.L_0802c9c0:
	.4byte 0x06008000
.L_0802c9c4:
	.4byte 0x84002000
