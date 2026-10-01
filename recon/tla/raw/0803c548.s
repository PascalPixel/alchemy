.syntax unified
	.thumb
	.global Func_0803c548
	.thumb_func
Func_0803c548:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	movs r1, #161
	mov r9, r3
	lsls r1, r1, #3
	add r1, r9
	movs r2, #0
	sub sp, #24
	mov r10, r1
	mov r11, r2
.L_0803c56c:
	mov r3, r10
	ldrh r2, [r3, #22]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_0803c57a
	b .L_0803c988
.L_0803c57a:
	mov r1, r10
	ldr r6, [r1]
	ldr r1, .L_0803c5dc
	b .L_0803c97a
.L_0803c582:
	ldrb r3, [r6, #5]
	adds r7, r6, #0
	subs r3, #2
	adds r7, #16
	cmp r3, #16
	bls .L_0803c590
	b .L_0803c922
.L_0803c590:
	ldr r2, .L_0803c5e0
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0803c598:
	.4byte .L_0803c5e4
	.4byte .L_0803c922
	.4byte .L_0803c810
	.4byte .L_0803c67c
	.4byte .L_0803c6e8
	.4byte .L_0803c792
	.4byte .L_0803c922
	.4byte .L_0803c91c
	.4byte .L_0803c91c
	.4byte .L_0803c91c
	.4byte .L_0803c91c
	.4byte .L_0803c922
	.4byte .L_0803c8b2
	.4byte .L_0803c886
	.4byte .L_0803c89c
	.4byte .L_0803c870
	.4byte .L_0803c8e0
.L_0803c5dc:
	.4byte gFrameTick
.L_0803c5e0:
	.4byte .L_0803c598
.L_0803c5e4:
	movs r1, #152
	lsls r1, r1, #5
	adds r1, #70
	add r1, r9
	ldrh r3, [r1]
	cmp r3, #96
	bne .L_0803c5f4
	b .L_0803c922
.L_0803c5f4:
	ldr r3, .L_0803c644
	lsls r2, r0, #7
	adds r2, r2, r3
	ldrh r0, [r1]
	movs r1, #128
	bl VramBlock_LoadCached
	ldr r3, .L_0803c640
	ldrh r2, [r7, #8]
	ands r0, r3
	ldr r3, .L_0803c648
	movs r5, #13
	ands r3, r2
	orrs r3, r0
	strh r3, [r7, #8]
	strb r3, [r6, #14]
	ldrb r3, [r7, #5]
	negs r5, r5
	ands r5, r3
	movs r3, #17
	negs r3, r3
	ldrb r2, [r7, #7]
	ands r5, r3
	movs r3, #32
	orrs r5, r3
	movs r3, #63
	adds r4, r3, #0
	ands r5, r3
	movs r3, #128
	ands r4, r2
	orrs r5, r3
	strb r4, [r7, #7]
	strb r5, [r7, #5]
	ldr r3, .L_0803c64c
	ldrb r2, [r6, #8]
	ldr r0, [r3]
	b .L_0803c650
	.2byte 0x0000
.L_0803c640:
	.4byte 0x000003ff
.L_0803c644:
	.4byte Data_0805ec84
.L_0803c648:
	.4byte 0xfffffc00
.L_0803c64c:
	.4byte gFrameTick
.L_0803c650:
	mov r8, r2
	ldr r2, .L_0803c6e0
	movs r1, #80
	str r2, [sp, #4]
	str r4, [sp, #0]
	bl __umodsi3
	ldr r2, [sp, #4]
	mov r1, r8
	ldrb r3, [r2, r0]
	ldr r4, [sp, #0]
	adds r3, r1, r3
	adds r3, #2
	strb r3, [r7, #4]
	movs r3, #4
	negs r3, r3
	ands r5, r3
	subs r3, #59
	ands r3, r4
	strb r5, [r7, #5]
	strb r3, [r7, #7]
	b .L_0803c922
.L_0803c67c:
	ldr r3, [r1]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_0803c688
	b .L_0803c922
.L_0803c688:
	bl Random16
	adds r5, r0, #0
	bl Random16
	lsls r2, r5, #1
	lsls r3, r0, #1
	adds r2, r2, r5
	adds r3, r3, r0
	ldrh r1, [r6, #6]
	lsrs r3, r3, #16
	lsrs r2, r2, #16
	adds r2, r2, r3
	lsrs r2, r2, #1
	ldr r3, .L_0803c6dc
	adds r1, r1, r2
	subs r1, #1
	ands r1, r3
	ldrh r2, [r7, #6]
	ldr r3, .L_0803c6e4
	ands r3, r2
	orrs r3, r1
	strh r3, [r7, #6]
	bl Random16
	adds r5, r0, #0
	bl Random16
	lsls r2, r5, #1
	lsls r3, r0, #1
	adds r2, r2, r5
	adds r3, r3, r0
	ldrb r1, [r6, #8]
	lsrs r2, r2, #16
	lsrs r3, r3, #16
	adds r2, r2, r3
	lsrs r2, r2, #1
	adds r1, r1, r2
	subs r1, #1
	strb r1, [r7, #4]
	b .L_0803c922
	.2byte 0x0000
.L_0803c6dc:
	.4byte 0x000001ff
.L_0803c6e0:
	.4byte Data_0805c12c
.L_0803c6e4:
	.4byte 0xfffffe00
.L_0803c6e8:
	ldrh r3, [r6, #12]
	cmp r3, #0
	beq .L_0803c764
	ldr r1, .L_0803c75c
	ldr r3, [sp, #16]
	movs r2, #128
	movs r5, #255
	ands r3, r1
	lsls r2, r2, #2
	lsls r5, r5, #8
	orrs r3, r2
	adds r5, #255
	movs r2, #128
	lsls r2, r2, #18
	ands r3, r5
	orrs r3, r2
	str r3, [sp, #16]
	add r0, sp, #16
	ldr r3, [r0, #4]
	ands r3, r1
	str r3, [r0, #4]
	bl AffineMatrix_BuildForEffect
	ldrb r2, [r7, #7]
	movs r3, #31
	ands r0, r3
	movs r3, #63
	negs r3, r3
	ands r3, r2
	lsls r0, r0, #1
	orrs r3, r0
	strb r3, [r7, #7]
	ldrb r3, [r7, #5]
	movs r2, #3
	orrs r3, r2
	strb r3, [r7, #5]
	movs r3, #255
	ldrh r2, [r6, #6]
	lsls r3, r3, #8
	adds r3, #251
	adds r2, r2, r3
	ldr r3, .L_0803c758
	ldrh r1, [r7, #6]
	ands r2, r3
	ldr r3, .L_0803c760
	ands r3, r1
	orrs r3, r2
	strh r3, [r7, #6]
	ldrb r3, [r6, #8]
	adds r3, #251
	strb r3, [r7, #4]
	ldrh r3, [r6, #12]
	adds r3, r3, r5
	strh r3, [r6, #12]
	b .L_0803c922
	.2byte 0x0000
.L_0803c758:
	.4byte 0x000001ff
.L_0803c75c:
	.4byte 0xffff0000
.L_0803c760:
	.4byte 0xfffffe00
.L_0803c764:
	ldrb r2, [r7, #7]
	movs r3, #63
	negs r3, r3
	ands r3, r2
	ldrb r2, [r7, #5]
	strb r3, [r7, #7]
	movs r3, #4
	negs r3, r3
	ands r3, r2
	strb r3, [r7, #5]
	movs r2, #128
	ldrh r3, [r6, #6]
	lsls r2, r2, #1
	adds r2, #255
	ands r2, r3
	ldrh r1, [r7, #6]
	ldr r3, .L_0803c80c
	ands r3, r1
	orrs r3, r2
	strh r3, [r7, #6]
	ldrh r3, [r6, #8]
	strb r3, [r7, #4]
	b .L_0803c922
.L_0803c792:
	movs r3, #128
	add r5, sp, #16
	lsls r3, r3, #1
	strh r3, [r5]
	strh r3, [r5, #2]
	ldrh r3, [r6, #12]
	movs r1, #192
	lsls r1, r1, #2
	adds r3, r3, r1
	strh r3, [r6, #12]
	strh r3, [r5, #4]
	adds r0, r5, #0
	bl AffineMatrix_BuildForEffect
	ldrb r2, [r7, #7]
	movs r3, #31
	ands r0, r3
	movs r3, #63
	negs r3, r3
	lsls r0, r0, #1
	ands r3, r2
	orrs r3, r0
	ldrb r2, [r7, #5]
	strb r3, [r7, #7]
	movs r3, #4
	negs r3, r3
	ands r3, r2
	ldrh r0, [r5, #4]
	movs r2, #1
	orrs r3, r2
	movs r2, #232
	lsls r2, r2, #8
	strb r3, [r7, #5]
	adds r0, r0, r2
	bl Trig_Sin
	ldrh r2, [r6, #6]
	asrs r0, r0, #14
	ldr r3, .L_0803c808
	subs r2, r2, r0
	subs r2, #2
	ands r2, r3
	ldrh r1, [r7, #6]
	ldr r3, .L_0803c80c
	ldrh r0, [r5, #4]
	ands r3, r1
	orrs r3, r2
	strh r3, [r7, #6]
	movs r3, #208
	lsls r3, r3, #7
	adds r0, r0, r3
	bl Trig_Cos
	ldrb r3, [r6, #8]
	asrs r0, r0, #14
	subs r3, r3, r0
	subs r3, #2
	strb r3, [r7, #4]
	b .L_0803c922
.L_0803c808:
	.4byte 0x000001ff
.L_0803c80c:
	.4byte 0xfffffe00
.L_0803c810:
	ldr r3, [r1]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0803c820
	ldrh r3, [r6, #12]
	adds r3, #1
	strh r3, [r6, #12]
.L_0803c820:
	ldr r4, .L_0803c868
	ldrh r0, [r6, #12]
	movs r1, #20
	str r4, [sp, #0]
	bl __umodsi3
	ldr r4, [sp, #0]
	lsls r0, r0, #16
	lsrs r0, r0, #15
	ldrsb r3, [r4, r0]
	ldrh r2, [r6, #6]
	ldrh r1, [r7, #6]
	adds r2, r2, r3
	ldr r3, .L_0803c864
	ands r2, r3
	ldr r3, .L_0803c86c
	ands r3, r1
	orrs r3, r2
	strh r3, [r7, #6]
	movs r1, #20
	ldrh r0, [r6, #12]
	bl __umodsi3
	lsls r0, r0, #16
	ldr r4, [sp, #0]
	lsrs r0, r0, #15
	adds r0, #1
	ldrb r5, [r6, #8]
	ldrb r3, [r4, r0]
	adds r5, r5, r3
	subs r5, #2
	strb r5, [r7, #4]
	b .L_0803c922
	.2byte 0x0000
.L_0803c864:
	.4byte 0x000001ff
.L_0803c868:
	.4byte Data_0805c17c
.L_0803c86c:
	.4byte 0xfffffe00
.L_0803c870:
	ldrh r3, [r6, #12]
	ldr r0, .L_0803c8d8
	adds r3, #1
	movs r2, #15
	strh r3, [r6, #12]
	ands r3, r2
	ldrb r1, [r6, #8]
	ldrb r3, [r0, r3]
	subs r1, r1, r3
	strb r1, [r7, #4]
	b .L_0803c922
.L_0803c886:
	ldrh r3, [r6, #12]
	ldr r0, .L_0803c8d8
	adds r3, #1
	movs r2, #15
	strh r3, [r6, #12]
	ands r3, r2
	ldrb r1, [r6, #8]
	ldrb r3, [r0, r3]
	adds r1, r1, r3
	strb r1, [r7, #4]
	b .L_0803c922
.L_0803c89c:
	ldrh r3, [r6, #12]
	ldr r1, .L_0803c8d8
	adds r3, #1
	movs r2, #15
	strh r3, [r6, #12]
	ands r3, r2
	ldrsb r3, [r1, r3]
	ldrh r2, [r6, #6]
	ldrh r1, [r7, #6]
	subs r2, r2, r3
	b .L_0803c8c6
.L_0803c8b2:
	ldrh r3, [r6, #12]
	ldr r1, .L_0803c8d8
	adds r3, #1
	movs r2, #15
	strh r3, [r6, #12]
	ands r3, r2
	ldrsb r3, [r1, r3]
	ldrh r2, [r6, #6]
	ldrh r1, [r7, #6]
	adds r2, r2, r3
.L_0803c8c6:
	ldr r3, .L_0803c8d4
	ands r2, r3
	ldr r3, .L_0803c8dc
	ands r3, r1
	orrs r3, r2
	strh r3, [r7, #6]
	b .L_0803c922
.L_0803c8d4:
	.4byte 0x000001ff
.L_0803c8d8:
	.4byte Data_0805c1b4
.L_0803c8dc:
	.4byte 0xfffffe00
.L_0803c8e0:
	ldrh r3, [r6, #12]
	ldr r4, .L_0803c914
	adds r3, #1
	movs r0, #15
	strh r3, [r6, #12]
	ands r3, r0
	ldrh r2, [r6, #6]
	ldrsb r3, [r4, r3]
	ldrh r1, [r7, #6]
	subs r2, r2, r3
	ldr r3, .L_0803c910
	ands r2, r3
	ldr r3, .L_0803c918
	ands r3, r1
	orrs r3, r2
	strh r3, [r7, #6]
	ldrh r3, [r6, #12]
	ldrb r2, [r6, #8]
	ands r0, r3
	ldrb r3, [r4, r0]
	adds r2, r2, r3
	strb r2, [r7, #4]
	b .L_0803c922
	.2byte 0x0000
.L_0803c910:
	.4byte 0x000001ff
.L_0803c914:
	.4byte Data_0805c1b4
.L_0803c918:
	.4byte 0xfffffe00
.L_0803c91c:
	adds r0, r6, #0
	bl Func_0803c40c
.L_0803c922:
	ldrb r3, [r6, #5]
	cmp r3, #2
	bne .L_0803c940
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #70
	add r3, r9
	ldrh r3, [r3]
	cmp r3, #96
	beq .L_0803c974
	adds r0, r7, #0
	movs r1, #255
	bl Runtime_PushSlotEntry
	b .L_0803c974
.L_0803c940:
	cmp r3, #19
	bne .L_0803c968
	movs r1, #6
	ldrsh r3, [r6, r1]
	movs r1, #8
	ldrsh r2, [r6, r1]
	movs r1, #152
	adds r3, #64
	lsls r1, r1, #1
	cmp r3, r1
	bcs .L_0803c974
	adds r3, r2, #0
	adds r3, #64
	cmp r3, #223
	bhi .L_0803c974
	ldrb r1, [r6, #15]
	adds r0, r7, #0
	bl Runtime_PushSlotEntry
	b .L_0803c974
.L_0803c968:
	cmp r3, #13
	beq .L_0803c974
	ldrb r1, [r6, #15]
	adds r0, r7, #0
	bl Runtime_PushSlotEntry
.L_0803c974:
	ldr r3, .L_0803c9a8
	ldr r6, [r6]
	adds r1, r3, #0
.L_0803c97a:
	ldr r3, [r1]
	lsrs r0, r3, #2
	movs r3, #7
	ands r0, r3
	cmp r6, #0
	beq .L_0803c988
	b .L_0803c582
.L_0803c988:
	movs r3, #1
	add r11, r3
	movs r2, #36
	mov r1, r11
	add r10, r2
	cmp r1, #12
	beq .L_0803c998
	b .L_0803c56c
.L_0803c998:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0803c9a8:
	.4byte gFrameTick
