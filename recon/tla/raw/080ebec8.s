.syntax unified
	.thumb
	.global Func_080ebec8
	.thumb_func
Func_080ebec8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r2, #0
	mov r0, sp
	movs r2, #0
	adds r7, r3, #0
	mov r8, r2
	str r2, [r0]
	movs r3, #128
	movs r2, #133
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r4, r1, #0
	adds r3, #212
	adds r1, r5, #0
	adds r2, #18
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r4, #0
	bl Func_08020040
	str r0, [r5]
	cmp r0, #0
	beq .L_080ebf08
	ldrb r2, [r0, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	strb r3, [r0, #9]
.L_080ebf08:
	adds r1, r6, #0
	adds r0, r5, #0
	adds r2, r7, #0
	bl EffectSlot_SetPosition
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #32]
	movs r2, #128
	ldr r3, [r5]
	lsls r2, r2, #9
	str r2, [r5, #40]
	str r2, [r5, #44]
	str r2, [r5, #36]
	mov r2, r8
	str r6, [r5, #20]
	str r7, [r5, #24]
	strb r2, [r3, #26]
	adds r3, r5, #0
	movs r2, #1
	adds r3, #65
	strb r2, [r3]
	adds r3, #1
	strb r2, [r3]
	adds r3, #1
	strb r2, [r3]
	adds r3, #1
	strb r2, [r3]
	adds r3, #1
	strb r2, [r3]
	bl Random16
	adds r3, r5, #0
	adds r3, #70
	adds r2, r5, #0
	strb r0, [r3]
	adds r2, #71
	movs r3, #4
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #1
	bl Func_080ebea8
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
