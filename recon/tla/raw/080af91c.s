.syntax unified
	.thumb
	.global Owner_LevelUp
	.thumb_func
Owner_LevelUp:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r1, #0
	mov r11, r0
	bl Owner_GetState
	mov r10, r0
	movs r0, #44
	bl Runtime_BumpAllocateAlternatePool
	movs r3, #42
	adds r3, #255
	add r3, r10
	ldrb r3, [r3]
	mov r9, r0
	str r3, [r0]
	mov r0, r10
	ldrb r1, [r0, #15]
	movs r3, #255
	mov r2, r9
	lsls r3, r3, #8
	str r1, [r2, #4]
	adds r3, #255
	movs r2, #0
	strh r1, [r6]
	strh r3, [r6, #2]
	strh r2, [r6, #4]
	strh r2, [r6, #6]
	strh r2, [r6, #8]
	strh r2, [r6, #10]
	strh r2, [r6, #12]
	strh r2, [r6, #14]
	cmp r1, #98
	ble .L_080af96c
	b .L_080afb6a
.L_080af96c:
	ldrb r3, [r0, #15]
	adds r3, #1
	strb r3, [r0, #15]
	adds r3, r1, #1
	strh r3, [r6]
	ldrb r1, [r0, #15]
	mov r0, r11
	bl Owner_GetLevelThreshold
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_080af994
	movs r2, #146
	lsls r2, r2, #1
	add r2, r10
	ldr r3, [r2]
	cmp r3, r0
	bcs .L_080af994
	str r0, [r2]
.L_080af994:
	mov r0, r11
	bl Owner_GetRecordStride180
	movs r2, #0
	ldrsh r3, [r6, r2]
	adds r1, r0, #0
	mov r0, r9
	str r1, [r0, #8]
	ldrh r0, [r6]
	cmp r3, #1
	bne .L_080af9f2
	adds r2, r1, #0
	adds r2, #80
	ldrh r3, [r6, #4]
	ldrh r2, [r2]
	adds r3, r3, r2
	adds r2, r1, #0
	adds r2, #92
	ldrh r2, [r2]
	strh r3, [r6, #4]
	ldrh r3, [r6, #6]
	adds r3, r3, r2
	adds r2, r1, #0
	adds r2, #104
	ldrh r2, [r2]
	strh r3, [r6, #6]
	ldrh r3, [r6, #8]
	adds r3, r3, r2
	adds r2, r1, #0
	adds r2, #116
	ldrh r2, [r2]
	strh r3, [r6, #8]
	ldrh r3, [r6, #10]
	adds r3, r3, r2
	adds r2, r1, #0
	adds r2, #128
	ldrh r2, [r2]
	strh r3, [r6, #10]
	ldrh r3, [r6, #12]
	adds r3, r3, r2
	strh r3, [r6, #12]
	adds r3, r1, #0
	adds r3, #140
	ldrb r2, [r3]
	ldrh r3, [r6, #14]
	adds r3, r3, r2
	strh r3, [r6, #14]
.L_080af9f2:
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r1, #20
	bl Math_Div
	lsls r0, r0, #16
	asrs r5, r0, #16
	cmp r5, #0
	bge .L_080afa06
	movs r5, #0
.L_080afa06:
	cmp r5, #4
	ble .L_080afa0c
	movs r5, #4
.L_080afa0c:
	lsls r0, r5, #1
	mov r3, r9
	mov r8, r0
	ldr r1, [r3, #8]
	mov r3, r8
	adds r3, #82
	ldrsh r2, [r1, r3]
	subs r3, #2
	ldrsh r3, [r1, r3]
	subs r7, r2, r3
	bl Random16
	adds r3, r0, #0
	lsls r0, r3, #2
	adds r0, r0, r3
	lsls r0, r0, #2
	lsrs r0, r0, #16
	adds r0, r0, r7
	movs r1, #20
	bl Math_DivU
	ldrh r3, [r6, #4]
	adds r3, r3, r0
	strh r3, [r6, #4]
	mov r3, r9
	ldr r2, [r3, #8]
	mov r3, r8
	adds r3, #94
	ldrsh r1, [r2, r3]
	subs r3, #2
	ldrsh r3, [r2, r3]
	subs r7, r1, r3
	bl Random16
	adds r3, r0, #0
	lsls r0, r3, #2
	adds r0, r0, r3
	lsls r0, r0, #2
	lsrs r0, r0, #16
	adds r0, r0, r7
	movs r1, #20
	bl Math_DivU
	ldrh r3, [r6, #6]
	adds r3, r3, r0
	strh r3, [r6, #6]
	mov r3, r9
	ldr r2, [r3, #8]
	mov r3, r8
	adds r3, #106
	ldrh r1, [r2, r3]
	subs r3, #2
	ldrh r3, [r2, r3]
	subs r7, r1, r3
	bl Random16
	adds r3, r0, #0
	lsls r0, r3, #2
	adds r0, r0, r3
	lsls r0, r0, #2
	lsrs r0, r0, #16
	adds r0, r0, r7
	movs r1, #20
	bl Math_DivU
	ldrh r3, [r6, #8]
	adds r3, r3, r0
	mov r0, r9
	ldr r2, [r0, #8]
	strh r3, [r6, #8]
	mov r3, r8
	adds r3, #118
	ldrh r1, [r2, r3]
	subs r3, #2
	ldrh r3, [r2, r3]
	subs r7, r1, r3
	bl Random16
	adds r3, r0, #0
	lsls r0, r3, #2
	adds r0, r0, r3
	lsls r0, r0, #2
	lsrs r0, r0, #16
	adds r0, r0, r7
	movs r1, #20
	bl Math_DivU
	ldrh r3, [r6, #10]
	adds r3, r3, r0
	strh r3, [r6, #10]
	mov r3, r9
	ldr r2, [r3, #8]
	mov r3, r8
	adds r3, #130
	ldrh r1, [r2, r3]
	subs r3, #2
	ldrh r3, [r2, r3]
	subs r7, r1, r3
	bl Random16
	adds r3, r0, #0
	lsls r0, r3, #2
	adds r0, r0, r3
	lsls r0, r0, #2
	lsrs r0, r0, #16
	adds r0, r0, r7
	movs r1, #20
	bl Math_DivU
	ldrh r3, [r6, #12]
	adds r3, r3, r0
	mov r0, r9
	ldr r2, [r0, #8]
	strh r3, [r6, #12]
	adds r3, r5, #0
	adds r3, #141
	ldrb r1, [r2, r3]
	subs r3, #1
	ldrb r3, [r2, r3]
	subs r7, r1, r3
	bl Random16
	adds r3, r0, #0
	lsls r0, r3, #2
	adds r0, r0, r3
	lsls r0, r0, #2
	lsrs r0, r0, #16
	movs r1, #20
	adds r0, r0, r7
	bl Math_DivU
	mov r2, r10
	ldrh r3, [r2, #16]
	ldrh r1, [r6, #14]
	ldrh r2, [r6, #4]
	adds r1, r1, r0
	adds r3, r3, r2
	mov r0, r10
	strh r3, [r0, #16]
	ldrh r2, [r6, #6]
	ldrh r3, [r0, #18]
	strh r1, [r6, #14]
	adds r3, r3, r2
	mov r2, r10
	strh r3, [r2, #18]
	ldrh r3, [r2, #24]
	ldrh r2, [r6, #8]
	adds r3, r3, r2
	strh r3, [r0, #24]
	ldrh r2, [r6, #10]
	ldrh r3, [r0, #26]
	adds r3, r3, r2
	mov r2, r10
	strh r3, [r2, #26]
	ldrh r3, [r2, #28]
	ldrh r2, [r6, #12]
	adds r3, r3, r2
	strh r3, [r0, #28]
	ldrb r3, [r0, #30]
	movs r2, #0
	adds r3, r3, r1
	strb r3, [r0, #30]
	movs r3, #1
	strb r3, [r0, #31]
	mov r3, r10
	adds r3, #32
	strb r2, [r3]
	adds r3, #1
	strb r2, [r3]
	mov r0, r11
	bl Owner_RefreshClassActions
	mov r0, r11
	bl Owner_RecalculateStats
.L_080afb6a:
	mov r0, r9
	bl Sys_Free
	adds r0, r6, #0
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
