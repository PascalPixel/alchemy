.syntax unified
	.thumb
	.global Func_080dd43c
	.thumb_func
Func_080dd43c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	movs r2, #64
	adds r2, r2, r6
	sub sp, #12
	mov r8, r2
.L_080dd44c:
	mov r3, r8
	movs r7, #0
	ldrsb r7, [r3, r7]
	cmp r7, #0
	bne .L_080dd48e
	ldr r3, [r6, #20]
	mov r5, sp
	str r3, [r5]
	ldr r3, [r6, #24]
	str r3, [r5, #8]
	bl Random16
	adds r1, r0, #0
	lsls r1, r1, #16
	movs r0, #240
	adds r2, r5, #0
	lsls r0, r0, #13
	lsrs r1, r1, #16
	bl Func_0801489c
	ldr r3, [r5]
	mov r2, r8
	str r3, [r6, #12]
	ldr r3, [r5, #8]
	str r3, [r6, #16]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r6, #36]
	str r3, [r6, #32]
	adds r3, r6, #0
	adds r3, #66
	strb r7, [r3]
	b .L_080dd4c2
.L_080dd48e:
	cmp r7, #1
	bne .L_080dd4a6
	adds r0, r6, #0
	bl Func_080ebe70
	cmp r0, #0
	bne .L_080dd4de
	mov r2, r8
	ldrb r3, [r2]
	adds r3, #1
	strb r3, [r2]
	b .L_080dd44c
.L_080dd4a6:
	cmp r7, #2
	bne .L_080dd4ca
	ldr r3, [r6, #20]
	adds r2, r6, #0
	str r3, [r6, #12]
	ldr r3, [r6, #24]
	adds r2, #66
	str r3, [r6, #16]
	movs r3, #128
	lsls r3, r3, #3
	strh r3, [r6, #50]
	movs r3, #1
	strb r3, [r2]
	mov r2, r8
.L_080dd4c2:
	ldrb r3, [r2]
	adds r3, #1
	strb r3, [r2]
	b .L_080dd4de
.L_080dd4ca:
	cmp r7, #3
	bne .L_080dd4de
	adds r0, r6, #0
	bl Func_080ebe70
	cmp r0, #0
	bne .L_080dd4de
	adds r0, r6, #0
	bl Func_080ebf68
.L_080dd4de:
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
