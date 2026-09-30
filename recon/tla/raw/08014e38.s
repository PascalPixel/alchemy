.syntax unified
	.thumb
	.global Func_08014e38
	.thumb_func
Func_08014e38:
	push {r5, lr}
	ldr r5, .L_08014e68
	ldr r3, [r5]
	cmp r3, #0
	bgt .L_08014e64
	ldr r4, .L_08014e6c
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_08014e70
	ldr r1, [r4]
	adds r2, #12
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, [r5]
	adds r3, #1
	str r3, [r5]
	ldr r3, [r4]
	adds r3, #48
	str r3, [r4]
.L_08014e64:
	pop {r5, pc}
	.2byte 0x0000
.L_08014e68:
	.4byte Data_030011cc
.L_08014e6c:
	.4byte Data_03001220
.L_08014e70:
	.4byte gTransform
