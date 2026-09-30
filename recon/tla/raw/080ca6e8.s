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
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	mov r8, r0
	mov r10, r3
	ldr r3, [r3]
	movs r0, #0
	mov r9, r1
	sub sp, #20
	mov r11, r0
	movs r1, #0
	cmp r3, r8
	beq .L_080ca734
	cmp r3, #0
	bne .L_080ca71c
	mov r1, r8
	mov r2, r10
	str r1, [r2]
	b .L_080ca734
.L_080ca71c:
	adds r1, #1
	cmp r1, #3
	bgt .L_080ca734
	lsls r2, r1, #2
	mov r0, r10
	ldr r3, [r0, r2]
	cmp r3, r8
	beq .L_080ca734
	cmp r3, #0
	bne .L_080ca71c
	mov r1, r8
	str r1, [r0, r2]
.L_080ca734:
	mov r3, r8
	ldrh r2, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	movs r1, #1
	b .L_080ca9a4
.L_080ca740:
	lsls r3, r2, #16
	asrs r3, r3, #16
	cmp r3, #7
	bgt .L_080ca74c
	mov r11, r3
	b .L_080ca75c
.L_080ca74c:
	movs r0, #156
	lsls r0, r0, #6
	adds r0, #5
	cmp r3, r0
	bgt .L_080ca75c
	movs r1, #1
	mov r11, r9
	add r9, r1
.L_080ca75c:
	mov r3, r8
	movs r2, #2
	ldrsh r7, [r3, r2]
	adds r0, r7, #0
	bl GameFlag_IsConditionActive
	cmp r0, #0
	bne .L_080ca76e
	b .L_080ca994
.L_080ca76e:
	adds r5, r7, #0
	subs r5, #48
	cmp r5, #79
	bhi .L_080ca7d4
	movs r1, #20
	adds r0, r5, #0
	bl Math_Div
	movs r1, #20
	adds r6, r0, #0
	adds r0, r5, #0
	bl __modsi3
	cmp r0, #19
	bne .L_080ca7b6
	movs r0, #200
	ldr r2, .L_080ca9c0
	lsls r0, r0, #5
	adds r0, #80
	adds r3, r6, r0
	ldrsb r0, [r2, r3]
	cmp r0, #0
	bne .L_080ca79e
	b .L_080ca994
.L_080ca79e:
	lsls r3, r6, #2
	adds r3, r3, r6
	lsls r3, r3, #2
	adds r3, r3, r0
	adds r7, r3, #0
	adds r7, #47
	adds r0, r7, #0
	bl GameFlag_IsConditionActive
	cmp r0, #0
	bne .L_080ca7b6
	b .L_080ca994
.L_080ca7b6:
	movs r3, #197
	lsls r3, r3, #1
	add r3, r10
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	beq .L_080ca7d4
	adds r0, r7, #0
	adds r0, #80
	bl GameFlag_IsConditionActive
	cmp r0, #0
	bne .L_080ca7d4
	b .L_080ca994
.L_080ca7d4:
	mov r2, r8
	movs r1, #0
	ldrsh r0, [r2, r1]
	bl Func_080ca6a4
	adds r7, r0, #0
	mov r0, r11
	bl ObjectTable_Get
	adds r6, r0, #0
	cmp r6, #0
	bne .L_080ca870
	mov r3, r8
	ldrb r2, [r3, #23]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080ca814
	add r5, sp, #8
	str r6, [r5]
	str r6, [r5, #4]
	str r6, [r5, #8]
	mov r1, r8
	ldr r0, [r1, #8]
	adds r1, r5, #0
	bl Func_080ca9cc
	ldr r1, [r5]
	ldr r3, [r5, #8]
	adds r0, r7, #0
	movs r2, #0
	b .L_080ca820
.L_080ca814:
	mov r2, r8
	mov r0, r8
	ldr r1, [r2, #8]
	ldr r3, [r0, #16]
	ldr r2, [r2, #12]
	adds r0, r7, #0
.L_080ca820:
	bl Func_080200c0
	adds r6, r0, #0
	mov r1, r8
	ldrb r2, [r1, #23]
	movs r3, #1
	ands r3, r2
	movs r1, #1
	cmp r3, #0
	beq .L_080ca892
	mov r0, r11
	subs r0, #1
	str r1, [sp, #4]
	bl ObjectTable_Get
	adds r3, r0, #0
	adds r3, #84
	ldrb r3, [r3]
	ldr r1, [sp, #4]
	cmp r3, #1
	bne .L_080ca892
	adds r3, r6, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_080ca892
	ldr r7, [r0, #80]
	ldrb r3, [r7, #17]
	ldrb r5, [r7, #16]
	orrs r3, r1
	strb r3, [r7, #17]
	ldr r7, [r6, #80]
	ldrb r3, [r7, #17]
	ldrb r0, [r7, #16]
	orrs r3, r1
	strb r3, [r7, #17]
	bl Resource_ResetEntry
	strb r5, [r7, #16]
	b .L_080ca892
.L_080ca870:
	mov r0, r8
	movs r2, #0
	ldrsh r3, [r0, r2]
	ldr r2, .L_080ca9c4
	movs r1, #133
	lsls r1, r1, #2
	adds r2, r2, r1
	ldr r2, [r2]
	cmp r3, r2
	bne .L_080ca886
	b .L_080ca994
.L_080ca886:
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	adds r0, r6, #0
	bl Object_SetPositionAndResetMotionFar
.L_080ca892:
	cmp r6, #0
	beq .L_080ca98a
	adds r0, r6, #0
	movs r1, #1
	ldr r7, [r6, #80]
	bl Object_SetMode
	adds r3, r6, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_080ca8e0
	cmp r7, #0
	beq .L_080ca8e0
	ldrb r3, [r7, #27]
	movs r1, #0
	cmp r1, r3
	bge .L_080ca8dc
	adds r2, r7, #0
	adds r2, #40
.L_080ca8ba:
	ldmia r2!, {r5}
	cmp r5, #0
	beq .L_080ca8d6
	str r1, [sp, #4]
	str r2, [sp, #0]
	bl Random16
	lsls r3, r0, #2
	adds r3, r3, r0
	lsrs r3, r3, #15
	strh r3, [r5, #2]
	ldr r2, [sp, #0]
	ldrb r3, [r7, #27]
	ldr r1, [sp, #4]
.L_080ca8d6:
	adds r1, #1
	cmp r1, r3
	blt .L_080ca8ba
.L_080ca8dc:
	movs r3, #1
	strb r3, [r7, #25]
.L_080ca8e0:
	mov r2, r8
	ldrh r3, [r2, #20]
	adds r2, r6, #0
	strh r3, [r6, #6]
	adds r2, #89
	movs r3, #1
	strb r3, [r2]
	mov r3, r8
	ldr r1, [r3, #4]
	adds r0, r6, #0
	bl Object_SetActionCallback
	ldr r2, [r6, #8]
	cmp r2, #0
	bge .L_080ca906
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r2, r2, r0
.L_080ca906:
	adds r3, r6, #0
	adds r3, #100
	asrs r2, r2, #16
	strh r2, [r3]
	ldr r3, [r6, #16]
	cmp r3, #0
	bge .L_080ca91c
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
.L_080ca91c:
	adds r2, r6, #0
	asrs r3, r3, #16
	adds r2, #102
	strh r3, [r2]
	ldr r3, [r6, #12]
	cmp r3, #0
	beq .L_080ca93a
	subs r2, #17
	movs r3, #4
	strb r3, [r2]
	movs r2, #128
	ldr r3, [r6, #12]
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r6, #12]
.L_080ca93a:
	movs r3, #197
	lsls r3, r3, #1
	add r3, r10
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080ca970
	adds r1, r6, #0
	adds r1, #85
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	movs r0, #33
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080ca982
	movs r1, #192
	ldr r0, [r7, #12]
	ldr r3, .L_080ca9c8
	lsls r1, r1, #8
	mov lr, r3
	.2byte 0xf800
	str r0, [r7, #12]
	b .L_080ca982
.L_080ca970:
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	movs r0, #0
	bl Map_GetTerrainHeightFar
	ldr r3, [r6, #12]
	str r0, [r6, #20]
	adds r3, r3, r0
	str r3, [r6, #12]
.L_080ca982:
	adds r2, r6, #0
	adds r2, #35
	movs r3, #1
	strb r3, [r2]
.L_080ca98a:
	mov r0, r11
	lsls r3, r0, #2
	adds r3, #20
	mov r1, r10
	str r6, [r1, r3]
.L_080ca994:
	movs r2, #24
	add r8, r2
	mov r0, r8
	ldrh r3, [r0]
	movs r1, #1
	adds r2, r3, #0
	lsls r3, r2, #16
	asrs r3, r3, #16
.L_080ca9a4:
	negs r1, r1
	cmp r3, r1
	beq .L_080ca9b2
	mov r3, r9
	cmp r3, #63
	bgt .L_080ca9b2
	b .L_080ca740
.L_080ca9b2:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080ca9c0:
	.4byte Data_02001000
.L_080ca9c4:
	.4byte gPartyState
.L_080ca9c8:
	.4byte IwramMulQ16
