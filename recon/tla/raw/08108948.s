.syntax unified
	.thumb
	.global Func_08108948
	.thumb_func
Func_08108948:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r7, r0, #0
	cmp r7, #0
	beq .L_081089da
	movs r1, #13
	ldrsb r1, [r7, r1]
	mov r10, r1
	cmp r1, #0
	beq .L_081089da
	ldr r2, [r7]
	ldrb r6, [r7, #12]
	mov r8, r2
	movs r1, #8
	ldrsh r3, [r7, r1]
	movs r1, #4
	ldrsh r2, [r7, r1]
	adds r6, #1
	strb r6, [r7, #12]
	lsls r6, r6, #24
	subs r3, r3, r2
	asrs r6, r6, #24
	adds r0, r6, #0
	muls r0, r3
	mov r1, r10
	bl Math_Div
	ldrh r5, [r7, #4]
	mov r3, r8
	adds r5, r5, r0
	strh r5, [r3, #6]
	ldr r2, .L_081089c8
	movs r3, #128
	lsls r3, r3, #1
	mov r1, r8
	adds r3, #255
	ands r5, r3
	mov r9, r2
	ldr r3, .L_081089cc
	ldrh r2, [r1, #22]
	ands r3, r2
	orrs r3, r5
	mov r2, r8
	strh r3, [r2, #22]
	movs r1, #6
	ldrsh r2, [r7, r1]
	movs r1, #10
	ldrsh r3, [r7, r1]
	mov r1, r10
	subs r3, r3, r2
	adds r0, r6, #0
	muls r0, r3
	bl Math_Div
	ldrh r5, [r7, #6]
	mov r2, r8
	adds r5, r5, r0
	strh r5, [r2, #8]
	strb r5, [r2, #20]
	b .L_081089d0
	.2byte 0x0000
.L_081089c8:
	.4byte 0x00000000
.L_081089cc:
	.4byte 0xfffffe00
.L_081089d0:
	cmp r6, r10
	bne .L_081089da
	mov r3, r9
	strb r3, [r7, #13]
	strb r3, [r7, #12]
.L_081089da:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
