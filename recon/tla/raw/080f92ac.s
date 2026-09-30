.syntax unified
	.thumb
	.global Func_080f92ac
	.thumb_func
Func_080f92ac:
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_080f92d0
	ldr r1, .L_080f92d4
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #128
	lsls r2, r2, #24
	ldr r0, .L_080f92d8
	adds r1, #28
	adds r2, #1
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bx lr
.L_080f92d0:
	.4byte 0x05000200
.L_080f92d4:
	.4byte 0x050001c0
.L_080f92d8:
	.4byte 0x050001e8
