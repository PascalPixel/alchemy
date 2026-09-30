.syntax unified
	.thumb
	.global Func_081a08c4
	.thumb_func
Func_081a08c4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_081a0918
	movs r4, #7
	ldrh r3, [r3]
	ands r4, r3
	lsls r3, r3, #16
	asrs r2, r3, #16
	cmp r2, #0
	bge .L_081a08de
	adds r2, #7
.L_081a08de:
	ldr r3, .L_081a0914
	asrs r2, r2, #3
	ands r2, r3
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r1, r3, #3
	ldr r3, .L_081a091c
	ldr r2, .L_081a0920
	ldr r3, [r3]
	movs r7, #128
	adds r0, r3, #0
	negs r3, r4
	adds r3, #16
	mov lr, r3
	movs r3, #192
	lsls r3, r3, #2
	adds r0, #192
	movs r6, #0
	mov r10, r2
	mov r8, r3
	lsls r7, r7, #14
.L_081a0908:
	movs r5, #192
	mov r12, lr
	lsls r5, r5, #13
	movs r4, #5
	b .L_081a0924
	.2byte 0x0000
.L_081a0914:
	.4byte 0x0000001f
.L_081a0918:
	.4byte Data_02007504
.L_081a091c:
	.4byte Data_02007510
.L_081a0920:
	.4byte 0x40004000
.L_081a0924:
	mov r3, r12
	mov r2, r10
	orrs r3, r5
	orrs r3, r2
	adds r2, r0, #0
	stmia r2!, {r3}
	adds r0, #8
	str r1, [r2]
	adds r1, #4
	cmp r1, r8
	bne .L_081a093c
	movs r1, #0
.L_081a093c:
	subs r4, #1
	adds r5, r5, r7
	cmp r4, #0
	bge .L_081a0924
	movs r3, #8
	adds r6, #1
	add lr, r3
	cmp r6, #15
	ble .L_081a0908
	ldr r2, .L_081a09a0
	movs r3, #128
	lsls r3, r3, #19
	movs r1, #224
	ldr r0, [r2]
	adds r3, #212
	lsls r1, r1, #19
	ldr r2, .L_081a09a4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r1, .L_081a09a8
	movs r2, #0
	ldrsh r3, [r1, r2]
	cmp r3, #1
	bne .L_081a0984
	ldr r3, .L_081a09ac
	movs r2, #3
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_081a0984
	ldr r3, .L_081a099c
	ldr r2, .L_081a09b0
	strh r3, [r1]
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_081a0984:
	movs r3, #0
	ldrsh r2, [r1, r3]
	cmp r2, #3
	bne .L_081a09b4
	ldr r3, .L_081a09ac
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_081a09b4
	ldr r3, .L_081a099c
	strh r3, [r1]
	b .L_081a09b4
.L_081a099c:
	.4byte 0x00000000
.L_081a09a0:
	.4byte Data_02007510
.L_081a09a4:
	.4byte 0x84000100
.L_081a09a8:
	.4byte Data_02007508
.L_081a09ac:
	.4byte gInput
.L_081a09b0:
	.4byte Data_02007500
.L_081a09b4:
	movs r2, #0
	ldrsh r3, [r1, r2]
	cmp r3, #0
	bne .L_081a09ec
	ldr r0, .L_081a09f4
	movs r1, #3
	ldr r3, [r0]
	ands r3, r1
	cmp r3, #0
	bne .L_081a09d0
	ldr r2, .L_081a09f8
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_081a09d0:
	ldr r3, .L_081a09fc
	movs r2, #2
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_081a09ec
	ldr r3, [r0]
	ands r3, r1
	cmp r3, #0
	beq .L_081a09ec
	ldr r2, .L_081a09f8
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_081a09ec:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_081a09f4:
	.4byte gFrameTick
.L_081a09f8:
	.4byte Data_02007504
.L_081a09fc:
	.4byte gInput
