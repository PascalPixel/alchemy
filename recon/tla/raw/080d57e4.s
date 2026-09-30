.syntax unified
	.thumb
	.global Func_080d57e4
	.thumb_func
Func_080d57e4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r2, .L_080d591c
	movs r3, #133
	mov r10, r2
	lsls r3, r3, #2
	add r3, r10
	ldr r0, [r3]
	sub sp, #12
	bl ObjectTable_Get
	adds r6, r0, #0
	ldr r3, [r6, #80]
	movs r2, #194
	mov r8, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	strh r2, [r3]
	ldr r3, .L_080d5920
	ldr r1, .L_080d5924
	ldr r3, [r3]
	movs r2, #15
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrh r5, [r1, r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	movs r0, #0
	cmp r5, r3
	beq .L_080d590e
	ldr r2, [r6, #8]
	ldr r3, [r6, #16]
	ldr r0, .L_080d5928
	movs r1, #128
	lsls r1, r1, #12
	ands r2, r0
	ands r3, r0
	mov r7, sp
	adds r2, r2, r1
	adds r3, r3, r1
	str r3, [r6, #16]
	str r2, [r6, #8]
	str r2, [r7]
	movs r0, #128
	ldr r3, [r6, #12]
	lsls r0, r0, #13
	str r3, [r7, #4]
	adds r1, r5, #0
	ldr r3, [r6, #16]
	adds r2, r7, #0
	str r3, [r7, #8]
	bl Func_0801489c
	ldr r2, .L_080d592c
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #20]
	adds r3, r6, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r7]
	ldr r2, [r7, #8]
	bl Func_080201d0
	cmp r0, #0
	bne .L_080d5884
	adds r0, r6, #0
	adds r1, r7, #0
	bl Func_08020210
	cmp r0, #0
	beq .L_080d5888
.L_080d5884:
	movs r0, #0
	b .L_080d590e
.L_080d5888:
	strh r5, [r6, #6]
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	adds r3, r6, #0
	adds r3, #100
	ldrh r3, [r3]
	adds r2, r6, #0
	movs r5, #128
	adds r2, #85
	lsls r5, r5, #9
	strb r3, [r2]
	str r5, [r6, #24]
	adds r3, r6, #0
	adds r3, #102
	ldrh r3, [r3]
	mov r2, r8
	strb r3, [r2, #26]
	mov r3, r8
	strh r0, [r3, #18]
	movs r0, #152
	bl Audio_PlayCue
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r6, #48]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r6, #40]
	adds r0, r6, #0
	movs r1, #2
	str r5, [r6, #52]
	bl Object_SetMode
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl Object_SetPosition
	adds r0, r6, #0
	bl Object_CommitPosition
	adds r3, r6, #0
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
	ldr r3, .L_080d5918
	adds r2, #18
	add r2, r10
	strb r3, [r2]
	movs r0, #1
.L_080d590e:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_080d5918:
	.4byte 0x00000000
.L_080d591c:
	.4byte gPartyState
.L_080d5920:
	.4byte gInput
.L_080d5924:
	.4byte Data_080f0890
.L_080d5928:
	.4byte 0xfff00000
.L_080d592c:
	.4byte 0xfff80000
