.syntax unified
	.thumb
	.global Func_0802632c
	.thumb_func
Func_0802632c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	adds r2, r6, #0
	sub sp, #96
	movs r1, #0
	adds r2, #85
	movs r3, #4
	movs r0, #2
	str r1, [sp, #12]
	str r0, [sp, #8]
	strb r3, [r2]
	ldr r3, .L_08026688
	ldr r1, .L_0802668c
	movs r2, #143
	lsls r2, r2, #2
	adds r3, r3, r2
	ldrh r2, [r3]
	ldr r3, [r1]
	ands r3, r2
	cmp r3, #0
	beq .L_08026374
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r6, #48]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r6, #52]
	movs r3, #5
	str r3, [sp, #8]
	b .L_08026380
.L_08026374:
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r6, #48]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r6, #52]
.L_08026380:
	ldr r5, .L_0802668c
	ldr r1, .L_08026690
	ldr r3, [r5]
	movs r2, #15
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r3, [r1, r3]
	movs r2, #255
	lsls r3, r3, #16
	lsls r2, r2, #8
	lsrs r1, r3, #16
	adds r2, #255
	str r3, [sp, #4]
	cmp r1, r2
	bne .L_080263aa
	ldr r0, [sp, #12]
	movs r3, #4
	orrs r0, r3
	str r0, [sp, #12]
	b .L_08026632
.L_080263aa:
	add r3, sp, #84
	mov r11, r3
	ldr r3, [r6, #8]
	mov r0, r11
	movs r2, #0
	str r2, [sp, #12]
	str r3, [r0]
	mov r2, r11
	ldr r3, [r6, #12]
	str r3, [r0, #4]
	ldr r3, [r6, #16]
	str r3, [r0, #8]
	movs r0, #128
	lsls r0, r0, #12
	bl Func_0801489c
	ldr r3, .L_08026694
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080263e0
	ldr r3, [r5]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080263e0
	b .L_08026632
.L_080263e0:
	adds r0, r6, #0
	mov r1, r11
	bl Func_0802d958
	cmp r0, #0
	bne .L_08026492
	ldr r3, [r6, #8]
	add r5, sp, #72
	str r3, [r5]
	movs r1, #128
	ldr r3, [r6, #12]
	lsls r1, r1, #12
	str r3, [r5, #4]
	mov r8, r1
	ldr r3, [r6, #16]
	mov r0, r8
	str r3, [r5, #8]
	ldr r2, [sp, #4]
	movs r3, #128
	lsrs r7, r2, #16
	lsls r3, r3, #5
	adds r1, r7, r3
	adds r2, r5, #0
	bl Func_0801489c
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_0802d958
	cmp r0, #0
	bne .L_08026492
	ldr r3, [r6, #8]
	ldr r0, .L_08026698
	str r3, [r5]
	adds r1, r7, r0
	ldr r3, [r6, #12]
	mov r0, r8
	str r3, [r5, #4]
	adds r2, r5, #0
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Func_0801489c
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_0802d958
	cmp r0, #0
	bne .L_08026492
	ldr r3, [r6, #8]
	movs r2, #128
	str r3, [r5]
	lsls r2, r2, #6
	ldr r3, [r6, #12]
	adds r1, r7, r2
	str r3, [r5, #4]
	mov r0, r8
	ldr r3, [r6, #16]
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Func_0801489c
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_0802d958
	cmp r0, #0
	bne .L_08026492
	ldr r3, [r6, #8]
	mov r0, r8
	str r3, [r5]
	adds r2, r5, #0
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	ldr r3, .L_0802669c
	adds r1, r7, r3
	bl Func_0801489c
	adds r1, r5, #0
	adds r0, r6, #0
	bl Func_0802d958
	ldr r1, [sp, #4]
	str r1, [sp, #0]
	cmp r0, #0
	bne .L_08026492
	b .L_080265c2
.L_08026492:
	ldr r0, [sp, #4]
	movs r2, #16
	movs r1, #128
	lsrs r3, r0, #16
	add r2, sp
	lsls r1, r1, #5
	mov r9, r2
	adds r2, r3, r1
	ldr r1, .L_08026698
	mov r0, r9
	strh r2, [r0]
	adds r2, r3, r1
	movs r1, #128
	lsls r1, r1, #6
	strh r2, [r0, #2]
	adds r2, r3, r1
	ldr r1, .L_0802669c
	strh r2, [r0, #4]
	adds r2, r3, r1
	movs r1, #192
	lsls r1, r1, #6
	strh r2, [r0, #6]
	adds r2, r3, r1
	ldr r1, .L_080266a0
	strh r2, [r0, #8]
	adds r3, r3, r1
	mov r2, r9
	strh r3, [r2, #10]
	movs r3, #0
	mov r10, r3
	mov r8, r11
.L_080264d0:
	mov r0, r10
	lsls r3, r0, #1
	mov r1, r9
	ldrsh r2, [r1, r3]
	ldr r3, [r6, #8]
	mov r1, r8
	str r3, [r1]
	lsls r2, r2, #16
	ldr r3, [r6, #12]
	lsrs r7, r2, #16
	str r3, [r1, #4]
	movs r0, #128
	ldr r3, [r6, #16]
	lsls r0, r0, #12
	str r3, [r1, #8]
	str r2, [sp, #0]
	adds r1, r7, #0
	mov r2, r8
	bl Func_0801489c
	adds r0, r6, #0
	mov r1, r8
	bl Func_0802d958
	cmp r0, #0
	bne .L_080265a2
	ldr r3, [r6, #8]
	add r5, sp, #72
	str r3, [r5]
	movs r2, #128
	ldr r3, [r6, #12]
	lsls r2, r2, #5
	str r3, [r5, #4]
	movs r0, #128
	ldr r3, [r6, #16]
	adds r1, r7, r2
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Func_0801489c
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_0802d958
	cmp r0, #0
	bne .L_080265a2
	ldr r3, [r6, #8]
	movs r0, #128
	str r3, [r5]
	lsls r0, r0, #12
	ldr r3, [r6, #12]
	adds r2, r5, #0
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	ldr r3, .L_08026698
	adds r1, r7, r3
	bl Func_0801489c
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_0802d958
	cmp r0, #0
	bne .L_080265a2
	ldr r3, [r6, #8]
	movs r0, #128
	str r3, [r5]
	lsls r0, r0, #6
	ldr r3, [r6, #12]
	adds r1, r7, r0
	str r3, [r5, #4]
	movs r0, #128
	ldr r3, [r6, #16]
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Func_0801489c
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_0802d958
	cmp r0, #0
	bne .L_080265a2
	ldr r3, [r6, #8]
	ldr r2, .L_0802669c
	str r3, [r5]
	movs r0, #128
	ldr r3, [r6, #12]
	adds r1, r7, r2
	str r3, [r5, #4]
	lsls r0, r0, #12
	ldr r3, [r6, #16]
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Func_0801489c
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_0802d958
	cmp r0, #0
	beq .L_080265c2
.L_080265a2:
	movs r3, #1
	add r10, r3
	mov r0, r10
	cmp r0, #6
	blt .L_080264d0
	ldr r3, [r6, #8]
	mov r1, r11
	str r3, [r1]
	ldr r3, [r6, #12]
	str r3, [r1, #4]
	ldr r3, [r6, #16]
	str r3, [r1, #8]
	ldr r2, [sp, #12]
	movs r3, #1
	orrs r2, r3
	str r2, [sp, #12]
.L_080265c2:
	add r3, sp, #60
	mov r8, r3
	ldr r3, [r6, #8]
	mov r0, r8
	str r3, [r0]
	ldr r3, [r6, #12]
	str r3, [r0, #4]
	ldr r3, [r6, #16]
	str r3, [r0, #8]
	ldr r2, [sp, #0]
	movs r0, #128
	lsrs r1, r2, #16
	lsls r0, r0, #11
	mov r2, r8
	bl Func_0801489c
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r3, #63
	adds r7, r5, #0
	mov r10, r3
	adds r7, #89
.L_080265f0:
	ldrh r3, [r6, #32]
	subs r1, r3, #2
	ldr r3, [r5]
	cmp r3, #0
	beq .L_08026622
	ldrb r2, [r7]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08026622
	cmp r5, r6
	beq .L_08026622
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	mov r2, r8
	bl Func_08026f80
	cmp r0, #0
	blt .L_08026622
	ldr r0, [sp, #12]
	movs r3, #2
	orrs r0, r3
	str r0, [sp, #12]
.L_08026622:
	movs r1, #1
	negs r1, r1
	add r10, r1
	mov r2, r10
	adds r7, #128
	adds r5, #128
	cmp r2, #0
	bge .L_080265f0
.L_08026632:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	cmp r3, #0
	beq .L_0802665c
	ldr r0, [sp, #12]
	movs r2, #3
	ands r2, r0
	cmp r2, #0
	beq .L_08026654
	movs r1, #194
	lsls r1, r1, #1
	adds r2, r3, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0802665c
.L_08026654:
	movs r0, #194
	lsls r0, r0, #1
	adds r3, r3, r0
	strh r2, [r3]
.L_0802665c:
	ldr r1, [sp, #12]
	cmp r1, #0
	beq .L_080266a4
	ldr r3, .L_08026688
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Owner_GetState
	movs r1, #56
	ldrsh r3, [r0, r1]
	movs r5, #9
	cmp r3, #0
	bne .L_0802667c
	movs r5, #22
.L_0802667c:
	adds r0, r6, #0
	adds r1, r5, #0
	bl ObjectDispatch_ApplyArgumentToChildren
	b .L_080266ac
	.2byte 0x0000
.L_08026688:
	.4byte gPartyState
.L_0802668c:
	.4byte gInput
.L_08026690:
	.4byte Data_0802ec5c
.L_08026694:
	.4byte Data_03001238
.L_08026698:
	.4byte 0xfffff000
.L_0802669c:
	.4byte 0xffffe000
.L_080266a0:
	.4byte 0xffffd000
.L_080266a4:
	adds r0, r6, #0
	ldr r1, [sp, #8]
	bl ObjectDispatch_ApplyArgumentToChildren
.L_080266ac:
	ldr r2, [sp, #12]
	cmp r2, #0
	beq .L_080266f6
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r6, #56]
	str r3, [r6, #60]
	str r3, [r6, #64]
	movs r3, #3
	ands r3, r2
	cmp r3, #0
	beq .L_080266e6
	ldr r0, [sp, #4]
	ldrh r1, [r6, #6]
	lsrs r3, r0, #16
	subs r3, r3, r1
	lsls r3, r3, #16
	movs r2, #128
	asrs r3, r3, #16
	lsls r2, r2, #5
	cmp r3, r2
	ble .L_080266da
	adds r3, r2, #0
.L_080266da:
	ldr r2, .L_080267b0
	cmp r3, r2
	bge .L_080266e2
	adds r3, r2, #0
.L_080266e2:
	adds r3, r1, r3
	strh r3, [r6, #6]
.L_080266e6:
	adds r3, r6, #0
	adds r3, #100
	movs r2, #0
	strh r2, [r3]
	adds r2, r6, #0
	adds r2, #102
	movs r3, #2
	b .L_08026714
.L_080266f6:
	add r3, sp, #84
	ldr r1, [r3]
	ldr r2, [r3, #4]
	adds r0, r6, #0
	ldr r3, [r3, #8]
	bl Object_SetMoveTarget
	adds r2, r6, #0
	adds r2, #100
	movs r0, #0
	ldrsh r3, [r2, r0]
	ldrh r1, [r2]
	cmp r3, #0
	beq .L_08026716
	subs r3, r1, #1
.L_08026714:
	strh r3, [r2]
.L_08026716:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r4, #0
	movs r0, #0
	cmp r3, #0
	beq .L_080267a2
	ldr r1, .L_080267b4
	ldr r0, .L_080267b8
	movs r2, #141
	lsls r2, r2, #2
	adds r3, r1, r2
	ldrh r2, [r3]
	ldr r3, [r0, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_0802677c
	movs r2, #140
	lsls r2, r2, #2
	adds r3, r1, r2
	ldrh r2, [r3]
	ldr r3, [r0, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_0802677c
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #54
	adds r3, r1, r2
	ldrh r2, [r3]
	ldr r3, [r0, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_0802677c
	movs r2, #142
	lsls r2, r2, #2
	adds r3, r1, r2
	ldrh r2, [r3]
	ldr r3, [r0, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_0802677c
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #58
	adds r3, r1, r2
	ldrh r2, [r3]
	ldr r3, [r0, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_0802677e
.L_0802677c:
	movs r4, #1
.L_0802677e:
	cmp r4, #0
	beq .L_0802678a
	movs r0, #113
	bl Audio_PlayCue
	b .L_080267a0
.L_0802678a:
	adds r3, r6, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Func_0802d45c
	ldrh r3, [r6, #4]
	str r0, [r6, #20]
	adds r3, #1
	strh r3, [r6, #4]
.L_080267a0:
	movs r0, #1
.L_080267a2:
	add sp, #96
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080267b0:
	.4byte 0xfffff000
.L_080267b4:
	.4byte gPartyState
.L_080267b8:
	.4byte gInput
