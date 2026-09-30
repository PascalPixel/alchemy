.syntax unified
	.thumb
	.global Func_08014e74
	.thumb_func
Func_08014e74:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r1, r0, #0
	adds r3, #212
	ldr r0, .L_08014e8c
	adds r2, #12
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bx lr
	.2byte 0x0000
.L_08014e8c:
	.4byte gTransform
