.syntax unified
	.thumb
	.global Func_08014b70
	.thumb_func
Func_08014b70:
	ldr r3, .L_08014b9c
	sub sp, #4
	mov r0, sp
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r1, .L_08014ba0
	ldr r2, .L_08014ba4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_08014ba8
	ldr r3, .L_08014ba0
	str r3, [r2]
	movs r2, #128
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #3
	adds r3, #8
	strh r2, [r3]
	add sp, #4
	bx lr
.L_08014b9c:
	.4byte 0xf000f000
.L_08014ba0:
	.4byte 0x06002000
.L_08014ba4:
	.4byte 0x85000140
.L_08014ba8:
	.4byte Data_030011c4
