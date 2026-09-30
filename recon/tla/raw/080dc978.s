.syntax unified
	.thumb
	.global Func_080dc978
	.thumb_func
Func_080dc978:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	mov r8, r1
	movs r1, #166
	adds r6, r0, #0
	lsls r1, r1, #2
	movs r0, #88
	sub sp, #4
	bl Runtime_AllocateBlock
	movs r3, #0
	adds r5, r0, #0
	mov r0, sp
	str r3, [r0]
	movs r2, #133
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r1, r5, #0
	adds r2, #166
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_080dca18
	movs r1, #180
	ldr r0, [r3]
	lsls r1, r1, #1
	lsls r0, r0, #1
	bl Math_ModU
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #142
	adds r3, r5, r2
	adds r2, #2
	strh r0, [r3]
	adds r3, r5, r2
	adds r2, #2
	strh r6, [r3]
	adds r3, r5, r2
	mov r2, r8
	strh r2, [r3]
	movs r3, #165
	lsls r3, r3, #2
	adds r2, r5, r3
	movs r3, #8
	strb r3, [r2]
	movs r2, #203
	ldr r1, .L_080dca14
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r5, r2
	strb r1, [r3]
	bl BattleEffect_InitializeSharedScene
	bl Func_080dce60
	movs r2, #199
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r5, r2
	subs r2, #1
	movs r0, #0
	ldrsb r0, [r3, r0]
	adds r3, r5, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	lsls r0, r0, #10
	lsls r3, r3, #5
	orrs r0, r3
	movs r3, #198
	lsls r3, r3, #1
	adds r3, #255
	adds r5, r5, r3
	b .L_080dca1c
	.2byte 0x0000
.L_080dca14:
	.4byte 0x00000000
.L_080dca18:
	.4byte Data_0300122c
.L_080dca1c:
	movs r3, #0
	ldrsb r3, [r5, r3]
	movs r1, #1
	orrs r0, r3
	movs r3, #128
	lsls r3, r3, #14
	orrs r0, r3
	bl Func_080d170c
	movs r0, #8
	bl Func_080d17ac
	bl Func_080dca84
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080dca4c
	bl Func_080145a8
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080dca4c:
	.4byte Func_080dcb44
