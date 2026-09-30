.syntax unified
	.thumb
	.global Func_08013d48
	.thumb_func
Func_08013d48:
	push {r5, r6, lr}
	ldr r4, .L_08013d7c
	adds r6, r0, #0
	adds r0, r1, #0
	ldr r1, .L_08013d80
	ldrh r3, [r1]
	adds r5, r3, #0
	strh r1, [r1]
	ldrh r2, [r4]
	cmp r2, #31
	bgt .L_08013d76
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r4
	adds r3, #4
	stmia r3!, {r0}
	adds r2, #1
	stmia r3!, {r6}
	strh r2, [r4]
	movs r2, #176
	lsls r2, r2, #12
	str r2, [r3]
.L_08013d76:
	strh r5, [r1]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_08013d7c:
	.4byte Data_020038e0
.L_08013d80:
	.4byte 0x04000208
