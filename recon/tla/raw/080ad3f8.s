.syntax unified
	.thumb
	.global Owner_RecalculateStats
	.thumb_func
Owner_RecalculateStats:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	movs r0, #96
	sub sp, #4
	bl Runtime_BumpAllocateAlternatePool
	adds r6, r0, #0
	adds r0, r5, #0
	bl Owner_GetState
	adds r7, r0, #0
	movs r0, #16
	ldrsh r3, [r7, r0]
	movs r5, #3
	str r3, [r6]
	movs r1, #18
	ldrsh r3, [r7, r1]
	adds r1, r7, #0
	str r3, [r6, #4]
	adds r1, #36
	ldrh r3, [r7, #24]
	str r3, [r6, #8]
	ldrh r3, [r7, #26]
	str r3, [r6, #12]
	ldrh r3, [r7, #28]
	str r3, [r6, #16]
	ldrb r3, [r7, #30]
	str r3, [r6, #24]
	ldrb r2, [r7, #31]
	movs r3, #15
	ands r3, r2
	str r3, [r6, #28]
	adds r3, r7, #0
	adds r3, #32
	ldrb r3, [r3]
	adds r2, r6, #0
	str r3, [r6, #32]
	adds r3, r7, #0
	adds r3, #33
	ldrb r3, [r3]
	adds r2, #40
	str r3, [r6, #36]
.L_080ad452:
	movs r0, #0
	ldrsh r3, [r1, r0]
	subs r5, #1
	str r3, [r2]
	movs r0, #2
	ldrsh r3, [r1, r0]
	adds r1, #4
	str r3, [r2, #4]
	adds r2, #8
	cmp r5, #0
	bge .L_080ad452
	movs r1, #52
	ldrsh r2, [r7, r1]
	movs r0, #20
	ldrsh r3, [r7, r0]
	muls r3, r2
	adds r2, r3, #0
	cmp r3, #0
	bge .L_080ad480
	movs r1, #252
	lsls r1, r1, #6
	adds r1, #255
	adds r2, r3, r1
.L_080ad480:
	asrs r0, r2, #14
	movs r2, #56
	ldrsh r1, [r7, r2]
	subs r2, r0, r1
	cmp r2, #0
	blt .L_080ad492
	cmp r2, #1
	bgt .L_080ad4c8
	b .L_080ad498
.L_080ad492:
	subs r3, r1, r0
	cmp r3, #1
	bgt .L_080ad4c8
.L_080ad498:
	movs r3, #54
	ldrsh r2, [r7, r3]
	movs r0, #22
	ldrsh r3, [r7, r0]
	muls r3, r2
	adds r2, r3, #0
	cmp r3, #0
	bge .L_080ad4b0
	movs r1, #252
	lsls r1, r1, #6
	adds r1, #255
	adds r2, r3, r1
.L_080ad4b0:
	asrs r0, r2, #14
	movs r2, #58
	ldrsh r1, [r7, r2]
	subs r2, r0, r1
	cmp r2, #0
	blt .L_080ad4c2
	cmp r2, #1
	bgt .L_080ad4c8
	b .L_080ad4d8
.L_080ad4c2:
	subs r3, r1, r0
	cmp r3, #1
	ble .L_080ad4d8
.L_080ad4c8:
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r7, #20]
	strh r3, [r7, #22]
	ldrh r3, [r7, #52]
	strh r3, [r7, #56]
	ldrh r3, [r7, #54]
	strh r3, [r7, #58]
.L_080ad4d8:
	movs r3, #152
	lsls r3, r3, #1
	adds r1, r7, r3
	ldrb r3, [r1]
	movs r0, #4
	negs r0, r0
	ands r0, r3
	movs r3, #4
	ands r3, r0
	strb r0, [r1]
	cmp r3, #0
	beq .L_080ad4f8
	movs r2, #1
	adds r3, r0, #0
	orrs r3, r2
	strb r3, [r1]
.L_080ad4f8:
	movs r0, #162
	lsls r0, r0, #1
	adds r3, r7, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080ad50a
	ldr r3, [r6, #28]
	adds r3, #1
	str r3, [r6, #28]
.L_080ad50a:
	movs r1, #161
	lsls r1, r1, #1
	adds r2, r7, r1
	movs r3, #0
	strb r3, [r2]
	movs r2, #42
	adds r2, #255
	adds r3, r7, r2
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080ad522
	b .L_080ad8d2
.L_080ad522:
	movs r5, #0
.L_080ad524:
	lsls r3, r5, #1
	adds r1, r3, #0
	adds r1, #216
	ldrh r2, [r7, r1]
	movs r3, #128
	lsls r3, r3, #2
	ands r3, r2
	cmp r3, #0
	bne .L_080ad538
	b .L_080ad6c0
.L_080ad538:
	ldrh r0, [r7, r1]
	bl Item_GetDirect
	ldrb r2, [r0, #3]
	movs r3, #1
	ands r3, r2
	str r0, [r6, #88]
	cmp r3, #0
	beq .L_080ad558
	movs r3, #152
	lsls r3, r3, #1
	adds r1, r7, r3
	ldrb r2, [r1]
	movs r3, #3
	orrs r3, r2
	strb r3, [r1]
.L_080ad558:
	ldr r1, [r6, #88]
	ldr r3, [r6, #8]
	movs r0, #8
	ldrsh r2, [r1, r0]
	adds r3, r3, r2
	str r3, [r6, #8]
	movs r2, #10
	ldrsb r2, [r1, r2]
	ldr r3, [r6, #12]
	movs r1, #0
	adds r3, r3, r2
	str r3, [r6, #12]
	mov r8, r1
.L_080ad572:
	ldr r2, [r6, #88]
	mov r0, r8
	lsls r3, r0, #2
	adds r3, #24
	ldrb r1, [r2, r3]
	adds r2, r2, r3
	movs r3, #1
	ldrsb r3, [r2, r3]
	str r1, [r6, #72]
	str r3, [r6, #84]
	cmp r1, #26
	bls .L_080ad58c
	b .L_080ad6b4
.L_080ad58c:
	ldr r2, .L_080ad8a8
	lsls r3, r1, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_080ad594:
	.4byte .L_080ad6b4
	.4byte .L_080ad600
	.4byte .L_080ad60a
	.4byte .L_080ad614
	.4byte .L_080ad61e
	.4byte .L_080ad628
	.4byte .L_080ad632
	.4byte .L_080ad6b4
	.4byte .L_080ad6b4
	.4byte .L_080ad6b4
	.4byte .L_080ad6b4
	.4byte .L_080ad6b4
	.4byte .L_080ad6b4
	.4byte .L_080ad6b4
	.4byte .L_080ad6b4
	.4byte .L_080ad63c
	.4byte .L_080ad646
	.4byte .L_080ad650
	.4byte .L_080ad65a
	.4byte .L_080ad664
	.4byte .L_080ad66e
	.4byte .L_080ad678
	.4byte .L_080ad682
	.4byte .L_080ad68c
	.4byte .L_080ad6b4
	.4byte .L_080ad69c
	.4byte .L_080ad6ac
.L_080ad600:
	ldr r3, [r6]
	ldr r2, [r6, #84]
	adds r3, r3, r2
	str r3, [r6]
	b .L_080ad6b4
.L_080ad60a:
	ldr r3, [r6, #32]
	ldr r2, [r6, #84]
	adds r3, r3, r2
	str r3, [r6, #32]
	b .L_080ad6b4
.L_080ad614:
	ldr r3, [r6, #4]
	ldr r2, [r6, #84]
	adds r3, r3, r2
	str r3, [r6, #4]
	b .L_080ad6b4
.L_080ad61e:
	ldr r3, [r6, #36]
	ldr r2, [r6, #84]
	adds r3, r3, r2
	str r3, [r6, #36]
	b .L_080ad6b4
.L_080ad628:
	ldr r3, [r6, #16]
	ldr r2, [r6, #84]
	adds r3, r3, r2
	str r3, [r6, #16]
	b .L_080ad6b4
.L_080ad632:
	ldr r3, [r6, #24]
	ldr r2, [r6, #84]
	adds r3, r3, r2
	str r3, [r6, #24]
	b .L_080ad6b4
.L_080ad63c:
	ldr r3, [r6, #40]
	ldr r2, [r6, #84]
	adds r3, r3, r2
	str r3, [r6, #40]
	b .L_080ad6b4
.L_080ad646:
	ldr r3, [r6, #48]
	ldr r2, [r6, #84]
	adds r3, r3, r2
	str r3, [r6, #48]
	b .L_080ad6b4
.L_080ad650:
	ldr r3, [r6, #56]
	ldr r2, [r6, #84]
	adds r3, r3, r2
	str r3, [r6, #56]
	b .L_080ad6b4
.L_080ad65a:
	ldr r3, [r6, #64]
	ldr r2, [r6, #84]
	adds r3, r3, r2
	str r3, [r6, #64]
	b .L_080ad6b4
.L_080ad664:
	ldr r3, [r6, #44]
	ldr r2, [r6, #84]
	adds r3, r3, r2
	str r3, [r6, #44]
	b .L_080ad6b4
.L_080ad66e:
	ldr r3, [r6, #52]
	ldr r2, [r6, #84]
	adds r3, r3, r2
	str r3, [r6, #52]
	b .L_080ad6b4
.L_080ad678:
	ldr r3, [r6, #60]
	ldr r2, [r6, #84]
	adds r3, r3, r2
	str r3, [r6, #60]
	b .L_080ad6b4
.L_080ad682:
	ldr r3, [r6, #68]
	ldr r2, [r6, #84]
	adds r3, r3, r2
	str r3, [r6, #68]
	b .L_080ad6b4
.L_080ad68c:
	movs r2, #161
	lsls r2, r2, #1
	adds r1, r7, r2
	ldrb r3, [r1]
	ldr r2, [r6, #84]
	adds r3, r3, r2
	strb r3, [r1]
	b .L_080ad6b4
.L_080ad69c:
	movs r0, #152
	lsls r0, r0, #1
	adds r3, r7, r0
	ldrb r1, [r3]
	movs r2, #8
	orrs r2, r1
	strb r2, [r3]
	b .L_080ad6b4
.L_080ad6ac:
	ldr r3, [r6, #28]
	ldr r2, [r6, #84]
	adds r3, r3, r2
	str r3, [r6, #28]
.L_080ad6b4:
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #3
	bgt .L_080ad6c0
	b .L_080ad572
.L_080ad6c0:
	adds r5, #1
	cmp r5, #14
	bgt .L_080ad6c8
	b .L_080ad524
.L_080ad6c8:
	movs r3, #152
	lsls r3, r3, #1
	adds r1, r7, r3
	ldrb r2, [r1]
	movs r3, #8
	ands r3, r2
	cmp r3, #0
	beq .L_080ad6e0
	movs r3, #10
	negs r3, r3
	ands r3, r2
	strb r3, [r1]
.L_080ad6e0:
	movs r0, #132
	lsls r0, r0, #1
	adds r0, r0, r7
	movs r4, #0
	mov r8, r0
.L_080ad6ea:
	mov r1, r8
	ldr r1, [r1]
	movs r5, #0
	mov r10, r1
.L_080ad6f2:
	movs r3, #1
	lsls r3, r5
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_080ad746
	adds r0, r4, #0
	adds r1, r5, #0
	str r4, [sp, #0]
	bl Djinn_GetDefinition
	ldr r3, [r6]
	movs r2, #4
	ldrsb r2, [r0, r2]
	ldr r4, [sp, #0]
	adds r3, r3, r2
	str r3, [r6]
	movs r2, #5
	ldrsb r2, [r0, r2]
	ldr r3, [r6, #4]
	adds r3, r3, r2
	str r3, [r6, #4]
	movs r2, #6
	ldrsb r2, [r0, r2]
	ldr r3, [r6, #8]
	adds r3, r3, r2
	str r3, [r6, #8]
	movs r2, #7
	ldrsb r2, [r0, r2]
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r2, #8
	ldrsb r2, [r0, r2]
	ldr r3, [r6, #16]
	adds r3, r3, r2
	str r3, [r6, #16]
	movs r2, #9
	ldrsb r2, [r0, r2]
	ldr r3, [r6, #24]
	adds r3, r3, r2
	str r3, [r6, #24]
.L_080ad746:
	adds r5, #1
	cmp r5, #19
	ble .L_080ad6f2
	movs r3, #4
	adds r4, #1
	add r8, r3
	cmp r4, #3
	ble .L_080ad6ea
	movs r0, #42
	adds r0, #255
	adds r3, r7, r0
	ldrb r0, [r3]
	bl Owner_GetRecordStride84
	adds r5, r0, #0
	ldrb r2, [r5, #8]
	ldr r3, [r6]
	movs r1, #10
	adds r0, r2, #0
	muls r0, r3
	bl __divsi3
	ldrb r2, [r5, #9]
	ldr r3, [r6, #4]
	str r0, [r6]
	movs r1, #10
	adds r0, r2, #0
	muls r0, r3
	bl __divsi3
	ldrb r2, [r5, #10]
	ldr r3, [r6, #8]
	str r0, [r6, #4]
	movs r1, #10
	adds r0, r2, #0
	muls r0, r3
	bl __divsi3
	ldrb r2, [r5, #11]
	ldr r3, [r6, #12]
	str r0, [r6, #8]
	movs r1, #10
	adds r0, r2, #0
	muls r0, r3
	bl __divsi3
	ldrb r2, [r5, #12]
	ldr r3, [r6, #16]
	str r0, [r6, #12]
	movs r1, #10
	adds r0, r2, #0
	muls r0, r3
	bl __divsi3
	ldrb r2, [r5, #13]
	ldr r3, [r6, #24]
	str r0, [r6, #16]
	movs r1, #10
	adds r0, r2, #0
	muls r0, r3
	bl __divsi3
	str r0, [r6, #24]
	movs r5, #0
.L_080ad7c6:
	lsls r3, r5, #1
	adds r1, r3, #0
	adds r1, #216
	ldrh r2, [r7, r1]
	movs r3, #128
	lsls r3, r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080ad8ca
	ldrh r0, [r7, r1]
	bl Item_GetDirect
	str r0, [r6, #88]
	movs r1, #0
	mov r8, r1
.L_080ad7e4:
	ldr r2, [r6, #88]
	mov r0, r8
	lsls r3, r0, #2
	adds r3, #24
	ldrb r1, [r2, r3]
	adds r2, r2, r3
	movs r3, #1
	ldrsb r3, [r2, r3]
	str r1, [r6, #72]
	subs r1, #7
	str r3, [r6, #84]
	cmp r1, #7
	bhi .L_080ad8c0
	ldr r2, .L_080ad8ac
	lsls r3, r1, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080ad808:
	.4byte .L_080ad828
	.4byte .L_080ad83a
	.4byte .L_080ad84c
	.4byte .L_080ad85e
	.4byte .L_080ad870
	.4byte .L_080ad882
	.4byte .L_080ad894
	.4byte .L_080ad8b0
.L_080ad828:
	ldr r2, [r6]
	ldr r3, [r6, #84]
	movs r1, #10
	adds r0, r3, #0
	muls r0, r2
	bl __divsi3
	str r0, [r6]
	b .L_080ad8c0
.L_080ad83a:
	ldr r2, [r6, #32]
	ldr r3, [r6, #84]
	movs r1, #10
	adds r0, r3, #0
	muls r0, r2
	bl __divsi3
	str r0, [r6, #32]
	b .L_080ad8c0
.L_080ad84c:
	ldr r2, [r6, #4]
	ldr r3, [r6, #84]
	movs r1, #10
	adds r0, r3, #0
	muls r0, r2
	bl __divsi3
	str r0, [r6, #4]
	b .L_080ad8c0
.L_080ad85e:
	ldr r2, [r6, #36]
	ldr r3, [r6, #84]
	movs r1, #10
	adds r0, r3, #0
	muls r0, r2
	bl __divsi3
	str r0, [r6, #36]
	b .L_080ad8c0
.L_080ad870:
	ldr r2, [r6, #8]
	ldr r3, [r6, #84]
	movs r1, #10
	adds r0, r3, #0
	muls r0, r2
	bl __divsi3
	str r0, [r6, #8]
	b .L_080ad8c0
.L_080ad882:
	ldr r2, [r6, #12]
	ldr r3, [r6, #84]
	movs r1, #10
	adds r0, r3, #0
	muls r0, r2
	bl __divsi3
	str r0, [r6, #12]
	b .L_080ad8c0
.L_080ad894:
	ldr r2, [r6, #16]
	ldr r3, [r6, #84]
	movs r1, #10
	adds r0, r3, #0
	muls r0, r2
	bl __divsi3
	str r0, [r6, #16]
	b .L_080ad8c0
	.2byte 0x0000
.L_080ad8a8:
	.4byte .L_080ad594
.L_080ad8ac:
	.4byte .L_080ad808
.L_080ad8b0:
	ldr r2, [r6, #24]
	ldr r3, [r6, #84]
	movs r1, #10
	adds r0, r3, #0
	muls r0, r2
	bl __divsi3
	str r0, [r6, #24]
.L_080ad8c0:
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #3
	ble .L_080ad7e4
.L_080ad8ca:
	adds r5, #1
	cmp r5, #14
	bgt .L_080ad8d2
	b .L_080ad7c6
.L_080ad8d2:
	movs r0, #52
	adds r0, #255
	adds r3, r7, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	ldr r2, [r6, #8]
	adds r3, #8
	muls r3, r2
	cmp r3, #0
	bge .L_080ad8ea
	adds r3, #7
.L_080ad8ea:
	asrs r3, r3, #3
	str r3, [r6, #8]
	movs r1, #54
	adds r1, #255
	adds r3, r7, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	ldr r2, [r6, #12]
	adds r3, #8
	muls r3, r2
	cmp r3, #0
	bge .L_080ad906
	adds r3, #7
.L_080ad906:
	asrs r3, r3, #3
	str r3, [r6, #12]
	movs r2, #72
	adds r2, #255
	adds r3, r7, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	ldr r2, [r6, #16]
	adds r3, #8
	muls r3, r2
	cmp r3, #0
	bge .L_080ad922
	adds r3, #7
.L_080ad922:
	asrs r3, r3, #3
	str r3, [r6, #16]
	movs r3, #150
	lsls r3, r3, #1
	movs r4, #40
	movs r5, #3
	adds r0, r7, r3
.L_080ad930:
	ldrb r3, [r0]
	subs r5, #1
	lsls r3, r3, #24
	asrs r3, r3, #24
	adds r2, r3, #0
	muls r2, r3
	adds r2, r2, r3
	ldr r3, [r4, r6]
	lsls r1, r2, #2
	adds r1, r1, r2
	adds r3, r3, r1
	str r3, [r4, r6]
	adds r0, #1
	adds r4, #8
	cmp r5, #0
	bge .L_080ad930
	movs r1, #56
	adds r1, #255
	adds r0, r7, r1
	movs r5, #3
	movs r1, #44
.L_080ad95a:
	movs r3, #0
	ldrsb r3, [r0, r3]
	subs r5, #1
	lsls r2, r3, #2
	adds r2, r2, r3
	ldr r3, [r1, r6]
	lsls r2, r2, #2
	adds r3, r3, r2
	str r3, [r1, r6]
	adds r1, #8
	cmp r5, #0
	bge .L_080ad95a
	movs r2, #42
	adds r2, #255
	adds r3, r7, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080ad9e6
	movs r1, #165
	lsls r1, r1, #1
	adds r3, r7, r1
	ldrh r3, [r3]
	movs r0, #0
	cmp r3, #7
	bhi .L_080ad9dc
	ldr r2, .L_080adbe8
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_080ad994:
	.4byte .L_080ad9c0
	.4byte .L_080ad9b4
	.4byte .L_080ad9ca
	.4byte .L_080ad9b8
	.4byte .L_080ad9c0
	.4byte .L_080ad9bc
	.4byte .L_080ad9ca
	.4byte .L_080ad9d4
.L_080ad9b4:
	movs r0, #137
	b .L_080ad9c2
.L_080ad9b8:
	movs r0, #18
	b .L_080ad9cc
.L_080ad9bc:
	movs r0, #137
	b .L_080ad9c2
.L_080ad9c0:
	movs r0, #136
.L_080ad9c2:
	lsls r0, r0, #1
	bl GameFlag_Test
	b .L_080ad9dc
.L_080ad9ca:
	movs r0, #20
.L_080ad9cc:
	adds r0, #255
	bl GameFlag_Test
	b .L_080ad9dc
.L_080ad9d4:
	movs r0, #18
	adds r0, #255
	bl GameFlag_Test
.L_080ad9dc:
	cmp r0, #0
	beq .L_080ad9e6
	ldr r3, [r6, #36]
	adds r3, #4
	str r3, [r6, #36]
.L_080ad9e6:
	ldr r3, [r6, #8]
	cmp r3, #0
	bge .L_080ad9f0
	movs r3, #0
	str r3, [r6, #8]
.L_080ad9f0:
	movs r2, #186
	lsls r2, r2, #2
	adds r2, #255
	cmp r3, r2
	ble .L_080ad9fc
	str r2, [r6, #8]
.L_080ad9fc:
	ldr r3, [r6, #12]
	cmp r3, #0
	bge .L_080ada06
	movs r3, #0
	str r3, [r6, #12]
.L_080ada06:
	cmp r3, r2
	ble .L_080ada0c
	str r2, [r6, #12]
.L_080ada0c:
	ldr r3, [r6, #16]
	cmp r3, #0
	bge .L_080ada16
	movs r3, #0
	str r3, [r6, #16]
.L_080ada16:
	cmp r3, r2
	ble .L_080ada1c
	str r2, [r6, #16]
.L_080ada1c:
	ldr r3, [r6, #24]
	cmp r3, #0
	bge .L_080ada26
	movs r3, #0
	str r3, [r6, #24]
.L_080ada26:
	cmp r3, #99
	ble .L_080ada2e
	movs r3, #99
	str r3, [r6, #24]
.L_080ada2e:
	movs r2, #42
	adds r2, #255
	adds r3, r7, r2
	ldrb r2, [r3]
	cmp r2, #0
	bne .L_080ada4c
	ldr r3, [r6, #28]
	cmp r3, #0
	bge .L_080ada44
	str r2, [r6, #28]
	movs r3, #0
.L_080ada44:
	cmp r3, #4
	ble .L_080ada5e
	movs r3, #4
	b .L_080ada5c
.L_080ada4c:
	ldr r3, [r6, #28]
	cmp r3, #0
	bge .L_080ada56
	movs r3, #0
	str r3, [r6, #28]
.L_080ada56:
	cmp r3, #2
	ble .L_080ada5e
	movs r3, #2
.L_080ada5c:
	str r3, [r6, #28]
.L_080ada5e:
	ldr r3, [r6, #32]
	cmp r3, #0
	bge .L_080ada68
	movs r3, #0
	str r3, [r6, #32]
.L_080ada68:
	movs r2, #156
	lsls r2, r2, #6
	adds r2, #16
	cmp r3, r2
	ble .L_080ada74
	str r2, [r6, #32]
.L_080ada74:
	ldr r3, [r6, #36]
	cmp r3, #0
	bge .L_080ada7e
	movs r3, #0
	str r3, [r6, #36]
.L_080ada7e:
	cmp r3, #200
	ble .L_080ada86
	movs r3, #200
	str r3, [r6, #36]
.L_080ada86:
	movs r3, #200
	adds r2, r6, #0
	adds r1, r6, #0
	movs r5, #0
	movs r0, #0
	mov r12, r3
	movs r4, #44
	adds r2, #40
	adds r1, #44
.L_080ada98:
	ldr r3, [r2]
	cmp r3, #0
	bge .L_080adaa2
	str r0, [r2]
	adds r3, r0, #0
.L_080adaa2:
	cmp r3, #200
	ble .L_080adaaa
	mov r3, r12
	str r3, [r2]
.L_080adaaa:
	ldr r3, [r1]
	cmp r3, #0
	bge .L_080adab4
	str r0, [r1]
	adds r3, r0, #0
.L_080adab4:
	cmp r3, #200
	ble .L_080adabc
	mov r3, r12
	str r3, [r6, r4]
.L_080adabc:
	adds r5, #1
	adds r1, #8
	adds r4, #8
	adds r2, #8
	cmp r5, #3
	ble .L_080ada98
	ldr r3, [r6, #8]
	adds r1, r7, #0
	strh r3, [r7, #60]
	adds r1, #72
	ldr r3, [r6, #12]
	movs r5, #3
	strh r3, [r7, #62]
	adds r3, r7, #0
	ldr r2, [r6, #16]
	adds r3, #64
	strh r2, [r3]
	adds r2, r7, #0
	ldr r3, [r6, #24]
	adds r2, #66
	strb r3, [r2]
	adds r2, #1
	ldr r3, [r6, #28]
	strb r3, [r2]
	adds r3, r7, #0
	ldr r2, [r6, #32]
	adds r3, #68
	strb r2, [r3]
	adds r2, r7, #0
	ldr r3, [r6, #36]
	adds r2, #69
	strb r3, [r2]
	adds r2, r6, #0
	adds r2, #40
.L_080adb00:
	ldr r3, [r2]
	subs r5, #1
	strh r3, [r1]
	ldr r3, [r2, #4]
	adds r2, #8
	strh r3, [r1, #2]
	adds r1, #4
	cmp r5, #0
	bge .L_080adb00
	movs r0, #42
	adds r0, #255
	adds r3, r7, r0
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080adb2c
	movs r1, #252
	movs r0, #156
	lsls r1, r1, #6
	lsls r0, r0, #6
	adds r1, #255
	adds r0, #15
	b .L_080adb34
.L_080adb2c:
	movs r0, #218
	lsls r0, r0, #3
	adds r0, #255
	adds r1, r0, #0
.L_080adb34:
	movs r3, #52
	ldrsh r2, [r7, r3]
	ldr r3, [r6]
	cmp r3, #0
	bge .L_080adb42
	movs r3, #0
	str r3, [r6]
.L_080adb42:
	cmp r3, r1
	ble .L_080adb4a
	str r1, [r6]
	adds r3, r1, #0
.L_080adb4a:
	strh r3, [r7, #52]
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r2, r3
	beq .L_080adb86
	movs r3, #20
	ldrsh r2, [r7, r3]
	ldr r3, [r6]
	muls r2, r3
	cmp r2, #0
	bge .L_080adb68
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	adds r2, r2, r3
.L_080adb68:
	asrs r2, r2, #14
	cmp r2, #0
	bge .L_080adb70
	movs r2, #0
.L_080adb70:
	cmp r2, r1
	ble .L_080adb76
	adds r2, r1, #0
.L_080adb76:
	movs r1, #56
	ldrsh r3, [r7, r1]
	cmp r3, #0
	beq .L_080adb84
	cmp r2, #0
	bne .L_080adb84
	movs r2, #1
.L_080adb84:
	strh r2, [r7, #56]
.L_080adb86:
	movs r3, #54
	ldrsh r2, [r7, r3]
	ldr r3, [r6, #4]
	cmp r3, #0
	bge .L_080adb94
	movs r3, #0
	str r3, [r6, #4]
.L_080adb94:
	cmp r3, r0
	ble .L_080adb9c
	str r0, [r6, #4]
	adds r3, r0, #0
.L_080adb9c:
	strh r3, [r7, #54]
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r2, r3
	beq .L_080adbd8
	movs r1, #22
	ldrsh r2, [r7, r1]
	ldr r3, [r6, #4]
	muls r2, r3
	cmp r2, #0
	bge .L_080adbba
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	adds r2, r2, r3
.L_080adbba:
	asrs r2, r2, #14
	cmp r2, #0
	bge .L_080adbc2
	movs r2, #0
.L_080adbc2:
	cmp r2, r0
	ble .L_080adbc8
	adds r2, r0, #0
.L_080adbc8:
	movs r0, #58
	ldrsh r3, [r7, r0]
	cmp r3, #0
	beq .L_080adbd6
	cmp r2, #0
	bne .L_080adbd6
	movs r2, #1
.L_080adbd6:
	strh r2, [r7, #58]
.L_080adbd8:
	adds r0, r6, #0
	bl Sys_Free
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_080adbe8:
	.4byte .L_080ad994
