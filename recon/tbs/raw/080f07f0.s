.syntax unified
	.thumb
	.global Unnamed_080f07f0
	.thumb_func
Unnamed_080f07f0:
	.global Func_080f07f0
Func_080f07f0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r6, #144
	lsls r6, r6, #4
	sub sp, #44
	mov r10, r0
	adds r0, r6, #0
	str r1, [sp, #8]
	adds r7, r2, #0
	bl Runtime_BumpAllocateAlternatePool
	movs r1, #0
	movs r2, #192
	mov r3, r10
	str r0, [sp, #4]
	str r1, [sp, #0]
	mov r9, r2
	cmp r3, #0
	bne .L_080f0826
	movs r0, #1
	negs r0, r0
	b .L_080f0a16
.L_080f0826:
	movs r5, #128
	lsls r5, r5, #2
	adds r0, r5, #0
	bl GameFlag_IsSet
	cmp r0, #0
	bne .L_080f0848
	ldr r3, .L_080f0a28
	ldr r0, [sp, #4]
	adds r1, r6, #0
	movs r2, #0
	bl _call_via_r3
	adds r0, r5, #0
	bl GameFlag_SetBitFar
	b .L_080f086e
.L_080f0848:
	ldr r4, [sp, #4]
	movs r5, #128
	lsls r5, r5, #4
	movs r2, #128
	adds r1, r4, r5
	ldr r3, .L_080f0a2c
	lsls r2, r2, #1
	adds r0, r4, #0
	bl _call_via_r3
	movs r2, #128
	ldr r1, [sp, #4]
	lsls r2, r2, #1
	adds r0, r1, r2
	ldr r3, .L_080f0a28
	adds r1, r5, #0
	movs r2, #0
	bl _call_via_r3
.L_080f086e:
	mov r4, r10
	ldrb r0, [r4]
	movs r3, #0
	mov r8, r3
	adds r4, #1
	cmp r0, #0
	beq .L_080f0892
	ldr r2, .L_080f0a30
.L_080f087e:
	cmp r0, #31
	bls .L_080f088a
	adds r3, r0, #0
	subs r3, #32
	ldrb r3, [r2, r3]
	add r8, r3
.L_080f088a:
	ldrb r0, [r4]
	adds r4, #1
	cmp r0, #0
	bne .L_080f087e
.L_080f0892:
	cmp r7, #2
	bne .L_080f08a0
	mov r4, r9
	mov r1, r8
	subs r4, r4, r1
	str r4, [sp, #0]
	b .L_080f08b2
.L_080f08a0:
	cmp r7, #1
	bne .L_080f08b2
	mov r2, r9
	mov r4, r8
	subs r3, r2, r4
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [sp, #0]
.L_080f08b2:
	mov r4, r10
	ldrb r0, [r4]
	movs r1, #0
	adds r4, #1
	mov r8, r1
	mov r10, r4
	cmp r0, #0
	beq .L_080f0938
.L_080f08c2:
	cmp r0, #31
	bls .L_080f092c
	movs r2, #32
	negs r2, r2
	adds r2, r2, r0
	ldr r1, .L_080f0a34
	lsls r3, r2, #3
	adds r4, r1, r3
	mov lr, r2
	ldr r1, [sp, #0]
	ldr r2, [sp, #4]
	adds r3, r2, r1
	mov r2, r8
	adds r1, r3, r2
	movs r3, #0
	mov r12, r3
	movs r2, #1
	movs r3, #15
	mov r11, r2
	mov r9, r3
.L_080f08ea:
	ldr r3, .L_080f0a38
	ldrb r7, [r4]
	movs r6, #128
	adds r4, #1
	movs r5, #7
	adds r2, r1, r3
.L_080f08f6:
	adds r3, r7, #0
	ands r3, r6
	cmp r3, #0
	beq .L_080f0906
	mov r3, r11
	strb r3, [r2]
	mov r3, r9
	strb r3, [r1]
.L_080f0906:
	subs r5, #1
	adds r2, #1
	adds r1, #1
	lsrs r6, r6, #1
	cmp r5, #0
	bge .L_080f08f6
	movs r2, #1
	add r12, r2
	mov r3, r12
	adds r1, #248
	cmp r3, #7
	ble .L_080f08ea
	movs r3, #1
	cmp r0, #31
	bls .L_080f092a
	ldr r4, .L_080f0a30
	mov r1, lr
	ldrb r3, [r4, r1]
.L_080f092a:
	add r8, r3
.L_080f092c:
	mov r2, r10
	ldrb r0, [r2]
	movs r3, #1
	add r10, r3
	cmp r0, #0
	bne .L_080f08c2
.L_080f0938:
	movs r4, #24
	movs r2, #96
	mov r10, r4
	ldr r4, [sp, #4]
	mov r8, r2
	movs r6, #128
	movs r3, #7
	movs r2, #192
	adds r1, r4, #0
	movs r7, #96
	lsls r6, r6, #1
	mov r12, r3
	mov lr, r2
.L_080f0952:
	cmp r7, #0
	beq .L_080f0970
	mov r5, r8
	adds r2, r4, #0
.L_080f095a:
	ldrb r3, [r2, #1]
	ldrb r0, [r2]
	lsls r3, r3, #4
	orrs r0, r3
	subs r5, #1
	strb r0, [r1]
	adds r2, #2
	adds r4, #2
	adds r1, #1
	cmp r5, #0
	bne .L_080f095a
.L_080f0970:
	subs r3, r1, r7
	mov r2, lr
	adds r1, r3, r6
	subs r3, r4, r2
	adds r4, r3, r6
	movs r3, #1
	negs r3, r3
	add r12, r3
	mov r2, r12
	cmp r2, #0
	bge .L_080f0952
	mov r3, r10
	cmp r3, #0
	beq .L_080f0a0e
	ldr r4, [sp, #8]
	ldr r0, [sp, #4]
	lsls r1, r4, #5
	mov r12, r10
.L_080f0994:
	ldr r3, .L_080f0a3c
	ldr r4, .L_080f0a40
	adds r2, r1, r3
	ldr r3, [r0]
	str r3, [r2]
	adds r2, r1, r4
	movs r4, #128
	lsls r4, r4, #1
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #128
	str r3, [r2]
	ldr r3, .L_080f0a44
	lsls r4, r4, #2
	adds r2, r1, r3
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #192
	str r3, [r2]
	ldr r3, .L_080f0a48
	lsls r4, r4, #2
	adds r2, r1, r3
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #128
	str r3, [r2]
	ldr r3, .L_080f0a4c
	lsls r4, r4, #3
	adds r2, r1, r3
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #160
	str r3, [r2]
	ldr r3, .L_080f0a50
	lsls r4, r4, #3
	adds r2, r1, r3
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #192
	str r3, [r2]
	ldr r3, .L_080f0a54
	lsls r4, r4, #3
	adds r2, r1, r3
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #224
	str r3, [r2]
	ldr r3, .L_080f0a58
	lsls r4, r4, #3
	adds r2, r1, r3
	adds r3, r0, r4
	ldr r3, [r3]
	str r3, [r2]
	movs r2, #1
	negs r2, r2
	add r12, r2
	mov r3, r12
	adds r1, #32
	adds r0, #4
	cmp r3, #0
	bne .L_080f0994
.L_080f0a0e:
	ldr r0, [sp, #4]
	bl Runtime_BumpFree
	movs r0, #0
.L_080f0a16:
	add sp, #44
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_080f0a28:
	.4byte IwramFillWords
.L_080f0a2c:
	.4byte IwramCopyWords
.L_080f0a30:
	.4byte Data_080f11bd
.L_080f0a34:
	.4byte Data_080f1770
.L_080f0a38:
	.4byte 0x00000101
.L_080f0a3c:
	.4byte 0x06010000
.L_080f0a40:
	.4byte 0x06010004
.L_080f0a44:
	.4byte 0x06010008
.L_080f0a48:
	.4byte 0x0601000c
.L_080f0a4c:
	.4byte 0x06010010
.L_080f0a50:
	.4byte 0x06010014
.L_080f0a54:
	.4byte 0x06010018
.L_080f0a58:
	.4byte 0x0601001c
