.syntax unified
	.thumb
	.global Func_080b0084
	.thumb_func
Func_080b0084:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	mov r10, r1
	sub sp, #16
	bl Owner_GetState
	movs r1, #42
	adds r2, r0, #0
	adds r1, #255
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080b00d4
	adds r1, #33
	adds r3, r2, r1
	ldrh r0, [r3]
	bl Owner_GetRecord
	adds r0, #42
	ldrb r0, [r0]
	cmp r0, #47
	bls .L_080b00b6
	movs r0, #0
.L_080b00b6:
	ldr r2, .L_080b0138
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	adds r3, r3, r2
	adds r2, r3, #0
	movs r6, #0
	mov r1, r10
	adds r2, #8
.L_080b00c8:
	ldmia r2!, {r3}
	adds r6, #1
	stmia r1!, {r3}
	cmp r6, #3
	ble .L_080b00c8
	b .L_080b012e
.L_080b00d4:
	movs r1, #165
	lsls r1, r1, #1
	adds r3, r2, r1
	mov r5, sp
	adds r1, r2, #0
	ldrh r0, [r3]
	adds r2, r5, #0
	adds r1, #248
	bl Owner_GetDigitValues
	ldr r7, .L_080b013c
	mov r8, r5
	movs r6, #3
	movs r5, #0
.L_080b00f0:
	mov r3, r8
	ldr r0, [r5, r3]
	ldr r1, .L_080b0140
	bl Math_UnsignedMulHigh
	mov r1, r8
	lsls r3, r0, #2
	ldr r2, [r5, r1]
	adds r3, r3, r0
	lsls r3, r3, #1
	subs r4, r2, r3
	cmp r0, #15
	ble .L_080b010c
	movs r0, #15
.L_080b010c:
	cmp r0, #0
	bge .L_080b0112
	movs r0, #0
.L_080b0112:
	mov r3, r10
	lsls r2, r0, #2
	adds r1, r5, r3
	ldrh r3, [r7, r2]
	adds r2, r2, r7
	adds r3, r3, r4
	strh r3, [r1]
	subs r6, #1
	ldrh r3, [r2, #2]
	adds r5, #4
	adds r3, r3, r4
	strh r3, [r1, #2]
	cmp r6, #0
	bge .L_080b00f0
.L_080b012e:
	add sp, #16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_080b0138:
	.4byte Data_080c6684
.L_080b013c:
	.4byte Data_080c6644
.L_080b0140:
	.4byte 0x1999999a
