.syntax unified
	.thumb
	.global Func_080e0cec
	.thumb_func
Func_080e0cec:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	adds r7, r0, #0
	ldr r2, [r3, #16]
	movs r3, #64
	adds r3, r3, r7
	mov r10, r3
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	sub sp, #12
	mov r8, r3
	cmp r3, #0
	bne .L_080e0d94
	ldr r3, [r2, #8]
	mov r6, sp
	str r3, [r6]
	adds r0, r6, #0
	ldr r3, [r2, #12]
	str r3, [r6, #4]
	ldr r3, [r2, #16]
	str r3, [r6, #8]
	bl Func_080dc390
	ldr r3, [r6]
	movs r2, #128
	str r3, [r7, #20]
	str r3, [r7, #4]
	lsls r2, r2, #12
	ldr r3, [r6, #8]
	adds r3, r3, r2
	str r3, [r7, #24]
	str r3, [r7, #8]
	bl Random16
	adds r5, r0, #0
	bl Random16
	lsls r5, r5, #13
	lsls r0, r0, #13
	lsrs r0, r0, #16
	lsrs r5, r5, #16
	movs r3, #192
	lsls r3, r3, #8
	subs r5, r5, r0
	adds r5, r5, r3
	movs r0, #240
	adds r2, r6, #0
	lsls r0, r0, #15
	adds r1, r5, #0
	bl Func_0801489c
	ldr r3, [r6]
	mov r2, r8
	str r3, [r7, #12]
	ldr r3, [r6, #8]
	str r3, [r7, #16]
	movs r3, #160
	lsls r3, r3, #11
	str r3, [r7, #36]
	str r3, [r7, #32]
	adds r3, r7, #0
	adds r3, #66
	strb r2, [r3]
	mov r2, r10
	ldrb r3, [r2]
	adds r3, #1
	strb r3, [r2]
	ldr r3, .L_080e0dd0
	movs r2, #2
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_080e0dc4
	movs r0, #246
	bl Audio_PlayCue
	b .L_080e0dc4
.L_080e0d94:
	mov r3, r8
	cmp r3, #1
	bne .L_080e0dae
	adds r0, r7, #0
	bl Func_080ebe70
	cmp r0, #0
	bne .L_080e0dc4
	mov r2, r10
	ldrb r3, [r2]
	subs r3, #1
	strb r3, [r2]
	b .L_080e0dc4
.L_080e0dae:
	mov r3, r8
	cmp r3, #2
	bne .L_080e0dc4
	adds r0, r7, #0
	bl Func_080ebe70
	cmp r0, #0
	bne .L_080e0dc4
	adds r0, r7, #0
	bl Func_080ebf68
.L_080e0dc4:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e0dd0:
	.4byte Data_0300122c
