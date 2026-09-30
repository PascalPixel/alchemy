.syntax unified
	.thumb
	.global Func_08013c94
	.thumb_func
Func_08013c94:
	push {r5, r6, lr}
	ldr r4, .L_08013cc8
	adds r6, r0, #0
	adds r0, r1, #0
	ldr r1, .L_08013ccc
	ldrh r3, [r1]
	adds r5, r3, #0
	strh r1, [r1]
	ldrh r2, [r4]
	cmp r2, #31
	bgt .L_08013cc2
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r4
	adds r3, #4
	stmia r3!, {r0}
	adds r2, #1
	stmia r3!, {r6}
	strh r2, [r4]
	movs r2, #224
	lsls r2, r2, #11
	str r2, [r3]
.L_08013cc2:
	strh r5, [r1]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_08013cc8:
	.4byte Data_020038e0
.L_08013ccc:
	.4byte 0x04000208
