.syntax unified
	.thumb
	.global Func_08026fc8
	.thumb_func
Func_08026fc8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r7, r0, #0
	ldr r2, [r7, #12]
	movs r3, #128
	lsls r3, r3, #11
	adds r2, r2, r3
	mov r10, r1
	ldr r3, [r7, #16]
	ldr r1, [r7, #8]
	movs r0, #14
	bl Func_08023220
	ldr r2, [r7, #80]
	adds r3, r7, #0
	adds r3, #102
	mov r8, r2
	ldrh r2, [r3]
	movs r3, #1
	ands r3, r2
	adds r6, r0, #0
	cmp r3, #0
	bne .L_08027004
	movs r0, #184
	lsls r0, r0, #1
	adds r0, #255
	bl Audio_PlayCue
.L_08027004:
	cmp r6, #0
	beq .L_08027056
	ldr r3, [r7, #20]
	ldr r5, [r6, #80]
	str r3, [r6, #20]
	adds r0, r6, #0
	ldr r1, .L_08027060
	bl ObjectDispatch_Initialize
	adds r3, r6, #0
	adds r3, #85
	movs r6, #0
	strb r6, [r3]
	cmp r5, #0
	beq .L_08027056
	movs r1, #1
	adds r0, r5, #0
	bl Animation_ApplyChildArgument
	strb r6, [r5, #26]
	mov r2, r8
	ldrb r3, [r2, #9]
	ldrb r1, [r5, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r5, #9]
	mov r3, r10
	cmp r3, #2
	bne .L_08027056
	adds r0, r5, #0
	movs r1, #13
	bl Animation_ApplyChildValuesToRecord
	adds r0, r5, #0
	movs r1, #8
	bl Animation_ApplyChildValue
.L_08027056:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08027060:
	.4byte Data_0802ec88
