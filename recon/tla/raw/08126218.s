.syntax unified
	.thumb
	.global Func_08126218
	.thumb_func
Func_08126218:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #36]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #108
	sub sp, #4
	adds r4, r1, #0
	adds r5, r0, r3
	mov r6, sp
	ldr r3, .L_08126288
	ldrh r2, [r3]
	str r2, [r6]
	strh r3, [r3]
	cmp r4, #0
	bne .L_08126254
	movs r3, #128
	movs r1, #160
	movs r2, #128
	lsls r3, r3, #19
	lsls r1, r1, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r5, #0
	adds r1, #192
	adds r2, #128
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	b .L_0812627c
.L_08126254:
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #108
	adds r0, r0, r3
	lsls r3, r4, #4
	adds r3, r3, r4
	lsls r3, r3, #4
	adds r3, r3, r4
	movs r2, #128
	lsls r3, r3, #2
	movs r1, #160
	lsls r2, r2, #9
	subs r2, r2, r3
	lsls r1, r1, #19
	str r2, [r0]
	adds r1, #192
	adds r0, r5, #0
	movs r3, #128
	bl ColorBuffer_Scale
.L_0812627c:
	ldr r2, [r6]
	ldr r3, .L_08126288
	add sp, #4
	strh r2, [r3]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_08126288:
	.4byte 0x04000208
