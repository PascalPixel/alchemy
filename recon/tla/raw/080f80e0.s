.syntax unified
	.thumb
	.global Func_080f80e0
	.thumb_func
Func_080f80e0:
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #4
	adds r3, #220
	ldr r4, [r3]
	mov r0, sp
	movs r3, #0
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r1, r4, #0
	ldr r2, .L_080f8118
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #255
	strb r3, [r4, #28]
	adds r2, r4, #0
	movs r3, #1
	strb r3, [r4, #30]
	strb r3, [r4, #31]
	adds r2, #246
	adds r4, #247
	strb r3, [r2]
	add sp, #4
	strb r3, [r4]
	bx lr
	.2byte 0x0000
.L_080f8118:
	.4byte 0x8500033b
