.syntax unified
	.thumb
	.global Func_08016990
	.thumb_func
Func_08016990:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_080169d4
	movs r2, #1
	ldr r3, [r3]
	ldr r1, .L_080169d0
	lsls r3, r3, #26
	lsrs r3, r3, #30
	bics r2, r3
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r2, .L_080169d8
	lsls r3, r3, #3
	adds r3, r3, r2
	mov r12, r3
	ldr r3, .L_080169dc
	mov r8, r1
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	ldr r6, .L_080169e0
	cmp r3, #3
	beq .L_080169c2
	b .L_08016bbc
.L_080169c2:
	ldr r7, .L_080169e4
	ldr r5, [r7]
	cmp r5, #0
	bne .L_080169cc
	b .L_08016adc
.L_080169cc:
	b .L_080169e8
	.2byte 0x0000
.L_080169d0:
	.4byte 0x00000001
.L_080169d4:
	.4byte 0x04000128
.L_080169d8:
	.4byte Data_02003870
.L_080169dc:
	.4byte gLinkStatus
.L_080169e0:
	.4byte gSerialTransfer
.L_080169e4:
	.4byte gSerialReceiveDest
.L_080169e8:
	ldrb r3, [r6, #2]
	cmp r3, #1
	bne .L_08016ad8
	mov r2, r12
	ldrb r3, [r2, #3]
	movs r1, #128
	adds r3, #255
	lsls r3, r3, #24
	lsls r1, r1, #17
	cmp r3, r1
	bhi .L_08016ad8
	ldr r0, .L_08016bc4
	movs r2, #127
	mov r3, r12
	ldrb r1, [r0]
	mov lr, r2
	ldrb r2, [r3]
	mov r3, lr
	ands r3, r1
	cmp r2, r3
	bne .L_08016a92
	movs r1, #0
	mov lr, r1
	mov r2, lr
	strb r2, [r6]
	mov r3, r12
	ldrb r4, [r3, #3]
	cmp r4, #1
	beq .L_08016a28
	cmp r4, #2
	beq .L_08016a5c
	b .L_08016a84
.L_08016a28:
	movs r3, #128
	movs r2, #132
	mov r0, r12
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, #4
	adds r1, r5, #0
	adds r2, #5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, [r7]
	ldr r2, .L_08016bc8
	adds r3, #20
	str r3, [r7]
	ldrh r3, [r2]
	movs r1, #128
	adds r3, #20
	strh r3, [r2]
	ldrb r3, [r6, #1]
	negs r1, r1
	adds r3, #1
	adds r2, r1, #0
	orrs r3, r2
	strb r3, [r6, #1]
	b .L_08016a84
.L_08016a5c:
	movs r3, #128
	movs r2, #132
	mov r0, r12
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, #4
	adds r1, r5, #0
	adds r2, #5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_08016bc8
	strb r4, [r6, #2]
	ldrh r3, [r2]
	adds r3, #20
	strh r3, [r2]
	mov r2, lr
	mov r3, r8
	strb r2, [r6, #1]
	strb r3, [r6]
.L_08016a84:
	ldr r3, .L_08016bc4
	movs r1, #127
	ldrb r2, [r3]
	adds r2, #1
	ands r2, r1
	strb r2, [r3]
	b .L_08016adc
.L_08016a92:
	ldrb r2, [r0]
	movs r4, #128
	adds r3, r4, #0
	ands r3, r2
	movs r1, #128
	cmp r3, #0
	beq .L_08016aca
	ldrb r1, [r6]
	adds r3, r4, #0
	ands r3, r1
	lsls r3, r3, #24
	lsrs r2, r3, #24
	cmp r2, #0
	beq .L_08016ab4
	mov r1, r8
	strb r1, [r6]
	b .L_08016adc
.L_08016ab4:
	lsls r3, r1, #24
	movs r1, #128
	lsls r1, r1, #17
	cmp r3, r1
	bne .L_08016adc
	strb r2, [r6]
	mov r3, lr
	ldrb r2, [r0]
	ands r3, r2
	strb r3, [r0]
	b .L_08016adc
.L_08016aca:
	ldrb r3, [r0]
	orrs r3, r1
	strb r3, [r6]
	ldrb r3, [r0]
	orrs r3, r1
	strb r3, [r0]
	b .L_08016adc
.L_08016ad8:
	movs r3, #0
	strb r3, [r6]
.L_08016adc:
	ldr r7, .L_08016bcc
	ldr r0, [r7]
	cmp r0, #0
	beq .L_08016b96
	mov r2, r12
	ldrb r2, [r2, #2]
	mov lr, r2
	cmp r2, #1
	bne .L_08016b7c
	mov r3, r12
	ldrb r2, [r3]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	beq .L_08016b26
	ldr r5, .L_08016bc4
	mov r2, r12
	ldrb r1, [r5]
	ldrb r3, [r2]
	movs r4, #127
	subs r1, r1, r3
	ands r1, r4
	lsls r2, r1, #2
	adds r2, r2, r1
	lsls r2, r2, #2
	subs r3, r0, r2
	ldr r0, .L_08016bd0
	str r3, [r7]
	ldrh r3, [r0]
	adds r3, r3, r2
	strh r3, [r0]
	ldrb r3, [r5]
	subs r3, r3, r1
	strb r3, [r5]
	ldrb r3, [r5]
	ands r4, r3
	strb r4, [r5]
.L_08016b26:
	ldr r4, .L_08016bd0
	ldrh r3, [r4]
	cmp r3, #0
	beq .L_08016b7c
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, [r7]
	adds r1, r6, #4
	adds r2, #5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldrh r3, [r4]
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #236
	adds r3, r3, r1
	strh r3, [r4]
	ldrh r3, [r4]
	cmp r3, #0
	beq .L_08016b5c
	mov r2, lr
	ldr r3, .L_08016bc4
	strb r2, [r6, #3]
	b .L_08016b62
.L_08016b5c:
	movs r3, #2
	strb r3, [r6, #3]
	ldr r3, .L_08016bc4
.L_08016b62:
	ldrb r2, [r3]
	movs r3, #127
	ands r3, r2
	strb r3, [r6]
	ldr r3, [r7]
	adds r3, #20
	str r3, [r7]
	ldr r3, .L_08016bc4
	movs r1, #127
	ldrb r2, [r3]
	adds r2, #1
	ands r2, r1
	strb r2, [r3]
.L_08016b7c:
	ldrb r3, [r6, #3]
	cmp r3, #2
	bne .L_08016b96
	mov r1, r12
	ldrb r3, [r1, #2]
	cmp r3, #2
	bne .L_08016b96
	ldr r2, .L_08016bcc
	movs r3, #0
	str r3, [r2]
	strb r3, [r6, #3]
	movs r3, #1
	strb r3, [r6]
.L_08016b96:
	ldrb r3, [r6, #2]
	cmp r3, #2
	bne .L_08016bac
	mov r2, r12
	ldrb r3, [r2, #3]
	cmp r3, #2
	beq .L_08016bbc
	ldr r2, .L_08016bd4
	movs r3, #0
	str r3, [r2]
	b .L_08016bba
.L_08016bac:
	movs r3, #0
	strb r3, [r6, #2]
	ldr r3, .L_08016bd4
	ldr r3, [r3]
	cmp r3, #0
	beq .L_08016bbc
	movs r3, #1
.L_08016bba:
	strb r3, [r6, #2]
.L_08016bbc:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08016bc4:
	.4byte gSerialBlockSequence
.L_08016bc8:
	.4byte gSerialReceivedSize
.L_08016bcc:
	.4byte gSerialSendSource
.L_08016bd0:
	.4byte gSerialSendSize
.L_08016bd4:
	.4byte gSerialReceiveDest
