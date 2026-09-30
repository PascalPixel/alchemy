.syntax unified
	.thumb
	.global Func_08118d2c
	.thumb_func
Func_08118d2c:
	push {lr}
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_08118d50
	ldr r1, .L_08118d54
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_08118d58
	movs r1, #20
	ldr r0, .L_08118d5c
	mov lr, r3
	.2byte 0xf800
	pop {pc}
	.2byte 0x0000
.L_08118d50:
	.4byte 0x06000290
.L_08118d54:
	.4byte 0x06000280
.L_08118d58:
	.4byte IwramClearWords
.L_08118d5c:
	.4byte 0x0600028c
