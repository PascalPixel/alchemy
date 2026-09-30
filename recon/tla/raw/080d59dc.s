.syntax unified
	.thumb
	.global Func_080d59dc
	.thumb_func
Func_080d59dc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r2, .L_080d5a94
	movs r3, #133
	mov r8, r2
	lsls r3, r3, #2
	add r3, r8
	ldr r0, [r3]
	bl ObjectTable_Get
	adds r5, r0, #0
	adds r0, #8
	ldr r7, [r5, #80]
	bl Func_080d5930
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #194
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r6, r0, #0
	movs r2, #0
	strh r2, [r3]
	movs r0, #0
	cmp r6, #0
	beq .L_080d5a98
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
	str r6, [r5, #104]
	bl Object_SetMode
	ldr r1, [r6, #4]
	ldr r2, [r6, #8]
	ldr r3, [r6, #12]
	adds r0, r5, #0
	bl Object_SetPosition
	adds r0, r5, #0
	bl Object_CommitPosition
	adds r3, r5, #0
	adds r3, #85
	ldrb r2, [r3]
	adds r3, #15
	strh r2, [r3]
	ldrb r1, [r7, #9]
	ldrb r3, [r7, #26]
	adds r2, r5, #0
	adds r2, #102
	strh r3, [r2]
	lsls r3, r1, #28
	lsrs r3, r3, #30
	subs r2, #4
	strb r3, [r2]
	adds r0, r5, #0
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	ldr r3, .L_080d5a90
	movs r2, #4
	strb r3, [r7, #26]
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #18
	strb r3, [r7, #9]
	add r2, r8
	movs r3, #4
	strb r3, [r2]
	movs r0, #1
	b .L_080d5a98
.L_080d5a90:
	.4byte 0x00000000
.L_080d5a94:
	.4byte gPartyState
.L_080d5a98:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
