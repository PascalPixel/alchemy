.syntax unified
	.thumb
	.global Func_0810b0bc
	.thumb_func
Func_0810b0bc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	movs r2, #64
	adds r2, r2, r6
	movs r7, #0
	ldrsb r7, [r2, r7]
	sub sp, #12
	mov r8, r2
	cmp r7, #0
	bne .L_0810b136
	ldr r3, [r6, #20]
	mov r5, sp
	str r3, [r5]
	ldr r3, [r6, #24]
	str r3, [r5, #8]
	bl Random16
	adds r1, r0, #0
	movs r0, #160
	lsls r0, r0, #14
	adds r2, r5, #0
	bl Func_0801489c
	ldr r1, [r5]
	ldr r2, [r5, #8]
	adds r0, r6, #0
	bl EffectSlot_SetPositionFar
	ldr r3, [r6, #20]
	str r3, [r5]
	ldr r3, [r6, #24]
	str r3, [r5, #8]
	bl Random16
	adds r1, r0, #0
	movs r0, #128
	adds r2, r5, #0
	lsls r0, r0, #11
	bl Func_0801489c
	ldr r3, [r5]
	mov r2, r8
	str r3, [r6, #12]
	ldr r3, [r5, #8]
	str r3, [r6, #16]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r6, #32]
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r6, #36]
	adds r3, r6, #0
	adds r3, #66
	strb r7, [r3]
	ldrb r3, [r2]
	adds r3, #1
	strb r3, [r2]
	b .L_0810b15e
.L_0810b136:
	cmp r7, #1
	bne .L_0810b14a
	adds r0, r6, #0
	bl Func_080c8590
	cmp r0, #0
	bne .L_0810b15e
	mov r3, r8
	strb r0, [r3]
	b .L_0810b15e
.L_0810b14a:
	cmp r7, #2
	bne .L_0810b15e
	adds r0, r6, #0
	bl Func_080c8590
	cmp r0, #0
	bne .L_0810b15e
	adds r0, r6, #0
	bl Func_080c85b8
.L_0810b15e:
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
