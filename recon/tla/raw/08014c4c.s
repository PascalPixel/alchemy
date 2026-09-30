.syntax unified
	.thumb
	.global Func_08014c4c
	.thumb_func
Func_08014c4c:
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_08014c64
	ldr r1, .L_08014c68
	adds r2, #224
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bx lr
	.2byte 0x0000
.L_08014c64:
	.4byte Data_08017b10
.L_08014c68:
	.4byte 0x05000200
