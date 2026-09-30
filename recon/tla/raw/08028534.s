.syntax unified
	.thumb
	.global Func_08028534
	.thumb_func
Func_08028534:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	adds r1, r7, #0
	adds r1, #8
	sub sp, #96
	movs r0, #0
	str r0, [sp, #4]
	adds r0, r1, #0
	str r1, [sp, #0]
	bl GetWorldMapCollision
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	subs r3, r0, #1
	cmp r3, #2
	bhi .L_08028570
	movs r2, #208
	add r4, sp, #4
	lsls r2, r2, #4
	ldrb r4, [r4]
	adds r2, #58
	adds r3, r1, r2
	strb r4, [r3]
.L_08028570:
	subs r3, r0, #5
	cmp r3, #1
	bhi .L_08028582
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #58
	adds r2, r1, r3
	movs r3, #1
	strb r3, [r2]
.L_08028582:
	cmp r0, #4
	bne .L_08028592
	movs r4, #208
	lsls r4, r4, #4
	adds r4, #58
	adds r2, r1, r4
	movs r3, #2
	strb r3, [r2]
.L_08028592:
	subs r3, r0, #7
	cmp r3, #5
	bhi .L_080285a4
	movs r0, #208
	lsls r0, r0, #4
	adds r0, #58
	adds r2, r1, r0
	movs r3, #3
	strb r3, [r2]
.L_080285a4:
	movs r3, #160
	movs r1, #0
	lsls r3, r3, #9
	str r1, [sp, #12]
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	ldr r5, .L_08028904
	movs r2, #15
	ldr r3, [r5]
	ldr r1, .L_08028908
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	movs r0, #255
	str r2, [sp, #8]
	lsls r0, r0, #8
	lsls r2, r2, #16
	lsrs r6, r2, #16
	adds r0, #255
	mov r9, r2
	cmp r6, r0
	bne .L_080285e4
	movs r1, #4
	str r1, [sp, #12]
	b .L_080287f6
.L_080285dc:
	mov r2, r11
	asrs r2, r2, #16
	str r2, [sp, #8]
	b .L_080287f6
.L_080285e4:
	ldr r4, [sp, #0]
	add r3, sp, #84
	mov r8, r3
	ldr r3, [r4]
	mov r0, r8
	str r3, [r0]
	ldr r3, [r7, #12]
	movs r1, #128
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	lsls r1, r1, #12
	mov r10, r1
	str r3, [r0, #8]
	adds r1, r6, #0
	mov r0, r10
	mov r2, r8
	bl Vector_AddPolarOffset
	ldr r3, .L_0802890c
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0802861e
	ldr r3, [r5]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0802861e
	b .L_080287f6
.L_0802861e:
	adds r0, r7, #0
	mov r1, r8
	bl Func_0802db88
	cmp r0, #0
	bne .L_080286ca
	ldr r2, [sp, #0]
	add r5, sp, #72
	ldr r3, [r2]
	mov r0, r10
	str r3, [r5]
	ldr r3, [r7, #12]
	adds r2, r5, #0
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	movs r3, #128
	lsls r3, r3, #5
	adds r1, r6, r3
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802db88
	cmp r0, #0
	bne .L_080286ca
	ldr r4, [sp, #0]
	ldr r0, .L_08028910
	ldr r3, [r4]
	adds r1, r6, r0
	str r3, [r5]
	ldr r3, [r7, #12]
	mov r0, r10
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802db88
	cmp r0, #0
	bne .L_080286ca
	ldr r1, [sp, #0]
	movs r2, #128
	ldr r3, [r1]
	lsls r2, r2, #6
	str r3, [r5]
	ldr r3, [r7, #12]
	adds r1, r6, r2
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	mov r0, r10
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802db88
	cmp r0, #0
	bne .L_080286ca
	ldr r4, [sp, #0]
	ldr r0, .L_08028914
	ldr r3, [r4]
	adds r1, r6, r0
	str r3, [r5]
	ldr r3, [r7, #12]
	mov r0, r10
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802db88
	cmp r0, #0
	bne .L_080286ca
	b .L_080287f6
.L_080286ca:
	add r1, sp, #16
	mov r10, r1
	mov r2, r9
	ldr r1, .L_08028910
	movs r4, #128
	lsrs r3, r2, #16
	lsls r4, r4, #5
	adds r2, r3, r4
	mov r0, r10
	strh r2, [r0]
	mov r4, r10
	adds r2, r3, r1
	strh r2, [r4, #2]
	movs r0, #128
	ldr r4, .L_08028914
	lsls r0, r0, #6
	adds r2, r3, r0
	mov r1, r10
	strh r2, [r1, #4]
	mov r0, r10
	adds r2, r3, r4
	strh r2, [r0, #6]
	movs r1, #192
	ldr r0, .L_08028918
	lsls r1, r1, #6
	adds r2, r3, r1
	mov r4, r10
	strh r2, [r4, #8]
	adds r3, r3, r0
	mov r1, r10
	movs r2, #6
	strh r3, [r1, #10]
	str r2, [sp, #4]
	movs r3, #0
	mov r9, r3
.L_08028710:
	mov r4, r9
	lsls r3, r4, #1
	mov r0, r10
	ldrsh r2, [r0, r3]
	ldr r3, [r7, #8]
	mov r4, r8
	str r3, [r4]
	ldr r3, [r7, #12]
	lsls r2, r2, #16
	str r3, [r4, #4]
	ldr r3, [r7, #16]
	lsrs r6, r2, #16
	movs r0, #128
	lsls r0, r0, #12
	adds r1, r6, #0
	str r3, [r4, #8]
	mov r11, r2
	mov r2, r8
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	mov r1, r8
	bl Func_0802db88
	cmp r0, #0
	bne .L_080287e4
	ldr r3, [r7, #8]
	add r5, sp, #72
	str r3, [r5]
	ldr r3, [r7, #12]
	movs r0, #128
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	lsls r0, r0, #5
	adds r1, r6, r0
	movs r0, #128
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802db88
	cmp r0, #0
	bne .L_080287e4
	ldr r3, [r7, #8]
	ldr r2, .L_08028910
	str r3, [r5]
	ldr r3, [r7, #12]
	movs r0, #128
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	adds r1, r6, r2
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802db88
	cmp r0, #0
	bne .L_080287e4
	ldr r3, [r7, #8]
	movs r0, #128
	str r3, [r5]
	ldr r3, [r7, #12]
	lsls r0, r0, #12
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	adds r2, r5, #0
	str r3, [r5, #8]
	movs r3, #128
	lsls r3, r3, #6
	adds r1, r6, r3
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802db88
	cmp r0, #0
	bne .L_080287e4
	ldr r3, [r7, #8]
	ldr r4, .L_08028914
	str r3, [r5]
	ldr r3, [r7, #12]
	movs r0, #128
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	adds r1, r6, r4
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802db88
	cmp r0, #0
	bne .L_080287e4
	b .L_080285dc
.L_080287e4:
	ldr r1, [sp, #4]
	movs r0, #1
	add r9, r0
	cmp r9, r1
	blt .L_08028710
	ldr r2, [sp, #12]
	movs r3, #1
	orrs r2, r3
	str r2, [sp, #12]
.L_080287f6:
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	cmp r1, #0
	beq .L_0802882c
	ldr r3, [sp, #12]
	movs r2, #3
	ands r2, r3
	cmp r2, #0
	beq .L_08028818
	movs r4, #194
	lsls r4, r4, #1
	adds r2, r1, r4
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_08028820
.L_08028818:
	movs r0, #194
	lsls r0, r0, #1
	adds r3, r1, r0
	strh r2, [r3]
.L_08028820:
	ldr r3, .L_08028904
	movs r4, #195
	ldr r3, [r3]
	lsls r4, r4, #1
	adds r2, r1, r4
	strh r3, [r2]
.L_0802882c:
	ldr r0, [sp, #12]
	cmp r0, #0
	beq .L_0802883c
	adds r0, r7, #0
	movs r1, #1
	bl ObjectDispatch_ApplyArgumentToChildren
	b .L_08028844
.L_0802883c:
	adds r0, r7, #0
	movs r1, #2
	bl ObjectDispatch_ApplyArgumentToChildren
.L_08028844:
	ldr r1, .L_0802891c
	ldr r0, .L_08028904
	movs r2, #140
	lsls r2, r2, #2
	adds r3, r1, r2
	ldrh r2, [r3]
	ldr r3, [r0, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_08028870
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r4, #181
	lsls r4, r4, #1
	adds r2, r3, r4
	movs r3, #252
	strh r3, [r2]
	ldr r2, [sp, #12]
	movs r3, #4
	orrs r2, r3
	str r2, [sp, #12]
.L_08028870:
	movs r4, #143
	lsls r4, r4, #2
	adds r3, r1, r4
	ldrh r2, [r3]
	ldr r3, [r0, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_080288aa
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	movs r0, #208
	lsls r0, r0, #4
	adds r0, #57
	adds r3, r5, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080288aa
	movs r0, #72
	adds r0, #255
	bl GameFlag_ClearBit
	movs r1, #181
	lsls r1, r1, #1
	adds r2, r5, r1
	movs r3, #253
	strh r3, [r2]
.L_080288aa:
	ldr r2, [sp, #12]
	cmp r2, #0
	beq .L_08028920
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #60]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	movs r3, #3
	ands r3, r2
	cmp r3, #0
	beq .L_080288ec
	ldr r4, [sp, #8]
	ldrh r2, [r7, #6]
	lsls r3, r4, #16
	lsrs r3, r3, #16
	subs r3, r3, r2
	lsls r3, r3, #16
	asrs r1, r3, #16
	movs r3, #128
	lsls r3, r3, #5
	cmp r1, r3
	ble .L_080288e0
	adds r1, r3, #0
.L_080288e0:
	ldr r3, .L_08028910
	cmp r1, r3
	bge .L_080288e8
	adds r1, r3, #0
.L_080288e8:
	adds r3, r2, r1
	strh r3, [r7, #6]
.L_080288ec:
	movs r0, #100
	adds r0, r0, r7
	mov r8, r0
	movs r3, #0
	mov r1, r8
	adds r2, r7, #0
	strh r3, [r1]
	adds r2, #102
	movs r3, #2
	strh r3, [r2]
	b .L_080289a2
	.2byte 0x0000
.L_08028904:
	.4byte gInput
.L_08028908:
	.4byte Data_0802ec5c
.L_0802890c:
	.4byte Data_03001238
.L_08028910:
	.4byte 0xfffff000
.L_08028914:
	.4byte 0xffffe000
.L_08028918:
	.4byte 0xffffd000
.L_0802891c:
	.4byte gPartyState
.L_08028920:
	add r3, sp, #84
	ldr r2, [r3, #4]
	ldr r1, [r3]
	adds r0, r7, #0
	ldr r3, [r3, #8]
	bl Object_SetMoveTarget
	ldr r1, [r7, #36]
	ldr r6, .L_08028a1c
	adds r0, r1, #0
	mov lr, r6
	.2byte 0xf800
	ldr r1, [r7, #44]
	adds r5, r0, #0
	adds r0, r1, #0
	mov lr, r6
	.2byte 0xf800
	adds r5, r5, r0
	adds r0, r5, #0
	bl Func_080149e0
	ldr r2, [sp, #12]
	str r2, [r7, #36]
	str r2, [r7, #44]
	ldr r4, [sp, #8]
	adds r2, r7, #0
	lsls r3, r4, #16
	lsrs r5, r3, #16
	adds r2, #36
	adds r1, r5, #0
	bl Vector_AddPolarOffset
	movs r0, #100
	adds r0, r0, r7
	movs r1, #0
	ldrsh r3, [r0, r1]
	mov r8, r0
	ldrh r2, [r0]
	cmp r3, #0
	beq .L_08028976
	subs r3, r2, #1
	mov r2, r8
	strh r3, [r2]
.L_08028976:
	adds r1, r7, #0
	adds r1, #90
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldrh r2, [r7, #6]
	subs r3, r5, r2
	lsls r3, r3, #16
	asrs r1, r3, #16
	movs r3, #128
	lsls r3, r3, #5
	cmp r1, r3
	ble .L_08028996
	movs r1, #128
	lsls r1, r1, #3
.L_08028996:
	ldr r4, .L_08028a20
	cmp r1, r4
	bge .L_0802899e
	ldr r1, .L_08028a24
.L_0802899e:
	adds r3, r2, r1
	strh r3, [r7, #6]
.L_080289a2:
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	ldr r3, .L_08028a28
	ldr r1, .L_08028a2c
	ldr r3, [r3]
	movs r2, #15
	lsrs r3, r3, #4
	ands r3, r2
	movs r2, #143
	lsls r3, r3, #2
	lsls r2, r2, #1
	ldr r4, [r1, r3]
	adds r1, r0, r2
	ldrh r0, [r1]
	subs r3, r4, r0
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r3, #0
	bge .L_080289cc
	adds r3, #7
.L_080289cc:
	asrs r2, r3, #3
	movs r3, #128
	lsls r3, r3, #2
	cmp r2, r3
	ble .L_080289d8
	adds r2, r3, #0
.L_080289d8:
	ldr r3, .L_08028a30
	cmp r2, r3
	bge .L_080289e0
	adds r2, r3, #0
.L_080289e0:
	adds r3, r2, #0
	adds r3, #15
	cmp r3, #30
	bhi .L_080289ec
	ldrh r3, [r1]
	subs r2, r4, r3
.L_080289ec:
	adds r3, r0, r2
	strh r3, [r1]
	adds r3, r7, #0
	adds r3, #84
	ldr r4, .L_08028a18
	ldrb r3, [r3]
	mov r9, r4
	mov r10, r3
	cmp r3, #1
	bne .L_08028a8c
	ldr r0, [sp, #0]
	bl GetWorldMapCollision
	mov r1, r8
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #0
	bne .L_08028a8c
	ldr r2, [sp, #12]
	cmp r2, #0
	bne .L_08028a8c
	b .L_08028a34
.L_08028a18:
	.4byte 0x00000000
.L_08028a1c:
	.4byte IwramMulQ16
.L_08028a20:
	.4byte 0xfffff000
.L_08028a24:
	.4byte 0xfffffc00
.L_08028a28:
	.4byte gInput
.L_08028a2c:
	.4byte Data_0802eca0
.L_08028a30:
	.4byte 0xfffffe00
.L_08028a34:
	ldr r3, [sp, #0]
	movs r0, #14
	ldr r1, [r3]
	adds r0, #255
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	bl Func_08023220
	adds r5, r0, #0
	cmp r5, #0
	beq .L_08028a8c
	ldr r1, .L_08028aa8
	ldr r6, [r5, #80]
	bl ObjectDispatch_Initialize
	adds r3, r5, #0
	adds r3, #85
	mov r4, r9
	strb r4, [r3]
	mov r0, r10
	subs r3, #51
	strb r0, [r3]
	cmp r6, #0
	beq .L_08028a86
	movs r1, #2
	adds r0, r6, #0
	bl Animation_ApplyChildArgument
	mov r1, r9
	strb r1, [r6, #26]
	movs r2, #13
	ldrb r1, [r6, #5]
	negs r2, r2
	adds r3, r2, #0
	ands r3, r1
	strb r3, [r6, #5]
	ldrb r3, [r6, #9]
	ands r2, r3
	movs r3, #8
	orrs r2, r3
	strb r2, [r6, #9]
.L_08028a86:
	movs r3, #10
	mov r2, r8
	strh r3, [r2]
.L_08028a8c:
	bl Func_08026e60
	ldrh r3, [r7, #4]
	movs r0, #1
	adds r3, #1
	strh r3, [r7, #4]
	add sp, #96
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08028aa8:
	.4byte Data_0802ec94
