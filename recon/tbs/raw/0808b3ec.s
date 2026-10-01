.syntax unified
	.thumb
	.global Event_SpawnObjectTable
	.thumb_func
Event_SpawnObjectTable:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0808b638
	ldr r3, [r3]
	mov r9, r3
	ldr r3, [r3]
	adds r7, r0, #0
	mov r11, r1
	sub sp, #8
	movs r1, #0
	cmp r3, r7
	beq .L_0808b42e
	cmp r3, #0
	bne .L_0808b418
	mov r0, r9
	str r7, [r0]
	b .L_0808b42e
.L_0808b418:
	adds r1, #1
	cmp r1, #3
	bgt .L_0808b42e
	lsls r2, r1, #2
	mov r0, r9
	ldr r3, [r0, r2]
	cmp r3, r7
	beq .L_0808b42e
	cmp r3, #0
	bne .L_0808b418
	str r7, [r0, r2]
.L_0808b42e:
	movs r0, #1
	movs r1, #0
	ldrsh r3, [r7, r1]
	negs r0, r0
	ldrh r2, [r7]
	b .L_0808b618
.L_0808b43a:
	lsls r3, r2, #16
	asrs r3, r3, #16
	cmp r3, #7
	bgt .L_0808b446
	str r3, [sp, #4]
	b .L_0808b454
.L_0808b446:
	ldr r2, .L_0808b63c
	cmp r3, r2
	bgt .L_0808b454
	mov r3, r11
	movs r0, #1
	str r3, [sp, #4]
	add r11, r0
.L_0808b454:
	movs r1, #2
	ldrsh r5, [r7, r1]
	adds r0, r5, #0
	bl GameFlag_IsConditionActive
	cmp r0, #0
	bne .L_0808b464
	b .L_0808b60a
.L_0808b464:
	adds r3, r5, #0
	subs r3, #48
	cmp r3, #79
	bhi .L_0808b488
	movs r3, #207
	lsls r3, r3, #1
	add r3, r9
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	beq .L_0808b488
	adds r0, r5, #0
	adds r0, #80
	bl GameFlag_IsConditionActive
	cmp r0, #0
	bne .L_0808b488
	b .L_0808b60a
.L_0808b488:
	movs r3, #0
	ldrsh r0, [r7, r3]
	bl Party_RemapCharacterIdByFlags
	mov r10, r0
	ldr r0, [sp, #4]
	bl ObjectTable_Get
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0808b512
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	mov r0, r10
	bl Object_CreateFar
	ldrb r2, [r7, #23]
	movs r3, #1
	ands r3, r2
	adds r6, r0, #0
	movs r1, #1
	cmp r3, #0
	beq .L_0808b4f6
	ldr r0, [sp, #4]
	subs r0, #1
	str r1, [sp, #0]
	bl ObjectTable_Get
	adds r3, r0, #0
	adds r3, #84
	ldrb r3, [r3]
	ldr r1, [sp, #0]
	cmp r3, #1
	bne .L_0808b4f6
	adds r3, r6, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_0808b4f6
	ldr r0, [r0, #80]
	ldrb r3, [r0, #29]
	orrs r3, r1
	strb r3, [r0, #29]
	ldrb r5, [r0, #28]
	ldr r0, [r6, #80]
	ldrb r3, [r0, #29]
	orrs r3, r1
	mov r8, r0
	strb r3, [r0, #29]
	ldrb r0, [r0, #28]
	bl Resource_ResetEntry
	mov r1, r8
	strb r5, [r1, #28]
.L_0808b4f6:
	movs r0, #33
	bl GameFlag_IsSet
	cmp r0, #0
	beq .L_0808b528
	mov r3, r10
	subs r3, #18
	cmp r3, #1
	bhi .L_0808b528
	adds r0, r6, #0
	movs r1, #226
	bl ObjectDispatch_RegisterChildMetadataFar
	b .L_0808b528
.L_0808b512:
	ldr r0, .L_0808b640
	bl GameFlag_IsSet
	cmp r0, #0
	bne .L_0808b528
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	adds r0, r6, #0
	bl Object_SetPositionAndResetMotionFar
.L_0808b528:
	cmp r6, #0
	beq .L_0808b600
	adds r0, r6, #0
	movs r1, #1
	bl Object_SetMode
	adds r3, r6, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_0808b556
	ldr r2, [r6, #80]
	mov r8, r2
	cmp r2, #0
	beq .L_0808b556
	bl Random16
	movs r1, #30
	bl __umodsi3
	mov r3, r8
	adds r3, #36
	strb r0, [r3]
.L_0808b556:
	ldrh r3, [r7, #20]
	adds r2, r6, #0
	strh r3, [r6, #6]
	adds r2, #89
	movs r3, #1
	strb r3, [r2]
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl ObjectMotion_SetActionCallback
	adds r0, r6, #0
	movs r1, #1
	bl Object_SetMode
	ldr r2, [r6, #8]
	cmp r2, #0
	bge .L_0808b57c
	ldr r3, .L_0808b644
	adds r2, r2, r3
.L_0808b57c:
	adds r3, r6, #0
	adds r3, #100
	asrs r2, r2, #16
	strh r2, [r3]
	ldr r3, [r6, #16]
	cmp r3, #0
	bge .L_0808b58e
	ldr r0, .L_0808b644
	adds r3, r3, r0
.L_0808b58e:
	adds r2, r6, #0
	asrs r3, r3, #16
	adds r2, #102
	strh r3, [r2]
	ldr r3, [r6, #12]
	cmp r3, #0
	beq .L_0808b5ac
	movs r3, #4
	subs r2, #17
	strb r3, [r2]
	movs r1, #128
	ldr r3, [r6, #12]
	lsls r1, r1, #8
	adds r3, r3, r1
	str r3, [r6, #12]
.L_0808b5ac:
	movs r3, #207
	lsls r3, r3, #1
	add r3, r9
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_0808b5e6
	adds r1, r6, #0
	adds r1, #85
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	movs r0, #33
	bl GameFlag_IsSet
	cmp r0, #0
	bne .L_0808b5f8
	mov r1, r8
	ldr r0, [r1, #24]
	movs r1, #192
	ldr r3, .L_0808b648
	lsls r1, r1, #8
	movs r0, r0
	mov r12, pc
	bx r3
	mov r2, r8
	str r0, [r2, #24]
	b .L_0808b5f8
.L_0808b5e6:
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	movs r0, #0
	bl Map_GetTerrainHeightFar
	ldr r3, [r6, #12]
	adds r3, r3, r0
	str r0, [r6, #20]
	str r3, [r6, #12]
.L_0808b5f8:
	adds r2, r6, #0
	adds r2, #35
	movs r3, #1
	strb r3, [r2]
.L_0808b600:
	ldr r0, [sp, #4]
	lsls r3, r0, #2
	adds r3, #20
	mov r1, r9
	str r6, [r1, r3]
.L_0808b60a:
	adds r7, #24
	ldrh r3, [r7]
	adds r2, r3, #0
	lsls r3, r2, #16
	movs r0, #1
	asrs r3, r3, #16
	negs r0, r0
.L_0808b618:
	cmp r3, r0
	beq .L_0808b624
	mov r1, r11
	cmp r1, #65
	bgt .L_0808b624
	b .L_0808b43a
.L_0808b624:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_0808b638:
	.4byte gEventWork
.L_0808b63c:
	.4byte 0x00002705
.L_0808b640:
	.4byte 0x00000109
.L_0808b644:
	.4byte 0x0000ffff
.L_0808b648:
	.4byte IwramMulQ16ReturnIp
