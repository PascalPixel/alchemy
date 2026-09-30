.syntax unified
	.thumb
	.global Func_080d5aa0
	.thumb_func
Func_080d5aa0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r2, .L_080d5bd8
	movs r3, #133
	mov r10, r2
	lsls r3, r3, #2
	add r3, r10
	ldr r0, [r3]
	sub sp, #12
	bl ObjectTable_Get
	adds r5, r0, #0
	ldr r3, [r5, #80]
	movs r2, #194
	mov r8, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	strh r2, [r3]
	ldr r3, .L_080d5bdc
	ldr r1, .L_080d5be0
	ldr r3, [r3]
	movs r2, #15
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrh r7, [r1, r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	movs r0, #0
	cmp r7, r3
	beq .L_080d5bc8
	ldr r2, [r5, #8]
	ldr r3, [r5, #16]
	ldr r0, .L_080d5be4
	movs r1, #128
	lsls r1, r1, #12
	ands r2, r0
	ands r3, r0
	mov r6, sp
	adds r2, r2, r1
	adds r3, r3, r1
	str r3, [r5, #16]
	str r2, [r5, #8]
	str r2, [r6]
	movs r0, #128
	ldr r3, [r5, #12]
	lsls r0, r0, #13
	str r3, [r6, #4]
	adds r1, r7, #0
	ldr r3, [r5, #16]
	adds r2, r6, #0
	str r3, [r6, #8]
	bl Func_0801489c
	ldr r2, .L_080d5be8
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #20]
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Func_080201c8 + 0x8
	cmp r0, #0
	bne .L_080d5b40
	adds r0, r5, #0
	adds r1, r6, #0
	bl Func_08020210
	cmp r0, #0
	beq .L_080d5b44
.L_080d5b40:
	movs r0, #0
	b .L_080d5bc8
.L_080d5b44:
	strh r7, [r5, #6]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	adds r3, #100
	ldrh r3, [r3]
	adds r2, r5, #0
	adds r2, #85
	strb r3, [r2]
	adds r3, r5, #0
	adds r3, #102
	ldrh r3, [r3]
	mov r2, r8
	strb r3, [r2, #26]
	mov r3, r8
	strh r0, [r3, #18]
	movs r0, #152
	bl Audio_PlayCue
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #40]
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetMode
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	adds r0, r5, #0
	bl Object_SetPosition
	adds r0, r5, #0
	bl Object_CommitPosition
	adds r3, r5, #0
	adds r3, #98
	ldrb r2, [r3]
	movs r1, #3
	mov r3, r8
	ands r2, r1
	ldrb r1, [r3, #9]
	movs r3, #13
	negs r3, r3
	lsls r2, r2, #2
	ands r3, r1
	orrs r3, r2
	mov r2, r8
	strb r3, [r2, #9]
	movs r2, #128
	lsls r2, r2, #2
	ldr r3, .L_080d5bd4
	adds r2, #18
	add r2, r10
	strb r3, [r2]
	movs r0, #1
.L_080d5bc8:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d5bd4:
	.4byte 0x00000000
.L_080d5bd8:
	.4byte gPartyState
.L_080d5bdc:
	.4byte gInput
.L_080d5be0:
	.4byte Data_080f0890
.L_080d5be4:
	.4byte 0xfff00000
.L_080d5be8:
	.4byte 0xfff80000
