.syntax unified
	.thumb
	.global Func_08014c6c
	.thumb_func
Func_08014c6c:
	sub sp, #4
	mov r0, sp
	movs r3, #0
	str r3, [r0]
	movs r4, #192
	movs r3, #128
	movs r2, #133
	lsls r4, r4, #18
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r1, r4, #0
	adds r2, #64
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_08014c98
	add sp, #4
	str r3, [r4, #4]
	ldr r3, .L_08014c9c
	str r3, [r4]
	bx lr
	.2byte 0x0000
.L_08014c98:
	.4byte Data_03001300
.L_08014c9c:
	.4byte Data_02030000
