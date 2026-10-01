.syntax unified
	.thumb
	.global AudioTrack_PackStream
	.thumb_func
AudioTrack_PackStream:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #44
	ldr r5, .L_080f8270
	ldr r3, .L_080f8274
	str r0, [sp, #40]
	movs r0, #0
	str r1, [sp, #36]
	str r0, [sp, #32]
	str r3, [r5]
	adds r6, r2, #0
	bl AudioTrack_ResetSlotBuckets
	ldr r2, [r5]
	ldr r1, .L_080f8278
	adds r3, r2, r1
	str r6, [r3]
	ldr r6, .L_080f827c
	ldr r4, [sp, #32]
	ldr r7, .L_080f8280
	adds r3, r2, r6
	ldr r0, .L_080f8284
	str r4, [r3]
	adds r3, r2, r7
	str r4, [r3]
	adds r3, r2, r0
	str r4, [r3]
	movs r3, #208
	lsls r3, r3, #6
	ldr r4, .L_080f8288
	add r7, sp, #32
	adds r1, r2, r3
	ldrb r7, [r7]
	movs r3, #128
	str r3, [r1]
	subs r0, #56
	adds r3, r2, r4
	strb r7, [r3]
	adds r2, r2, r0
	movs r3, #1
	str r3, [r2]
	movs r1, #168
	lsls r1, r1, #2
	ldr r2, [sp, #40]
	movs r0, #0
	bl AudioTrack_ConsumeSlotBytes
	ldr r2, [r5]
	ldr r3, [r2, r6]
	ldr r1, .L_080f828c
	lsls r3, r3, #2
	adds r3, r3, r1
	ldr r3, [r2, r3]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_080f7ff6
	b .L_080f868a
.L_080f7ff6:
	ldr r0, .L_080f8270
	ldr r4, .L_080f827c
	ldr r1, [r0]
	adds r3, r1, r4
	ldr r3, [r3]
	ldr r5, .L_080f8290
	mov r12, r3
	adds r2, r1, r5
	movs r3, #1
	str r3, [r2]
	mov r7, r12
	ldr r2, .L_080f828c
	lsls r3, r7, #2
	adds r3, r3, r2
	movs r4, #1
	ldr r3, [r1, r3]
	negs r4, r4
	cmp r3, r4
	bne .L_080f801e
	b .L_080f816e
.L_080f801e:
	movs r5, #192
	lsls r3, r3, #2
	lsls r5, r5, #6
	adds r3, r3, r5
	ldr r6, [r1, r3]
	movs r3, #136
	lsls r3, r3, #1
	ldr r7, .L_080f8294
	add r3, r12
	cmp r3, r7
	ble .L_080f80d4
	cmp r6, #0
	bne .L_080f803a
	b .L_080f816e
.L_080f803a:
	ldr r2, [r6, #8]
	mov r0, r12
	ldr r1, .L_080f8294
	subs r7, r0, r2
	ands r7, r1
	subs r3, r7, #1
	cmp r3, #62
	bhi .L_080f80cc
	ldr r3, .L_080f8270
	ldr r0, .L_080f8294
	adds r5, r2, #0
	mov r2, r12
	adds r2, #1
	ldr r1, [r3]
	mov lr, r3
	ands r2, r0
	ldr r3, .L_080f828c
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r8, r3
	adds r3, r5, #1
	ands r3, r0
	ldr r2, .L_080f828c
	lsls r3, r3, #2
	adds r2, r2, r3
	mov r10, r2
	mov r3, r8
	ldr r2, [r1, r3]
	mov r3, r10
	ldr r1, [r1, r3]
	movs r4, #1
	cmp r2, r1
	bne .L_080f80ae
.L_080f807c:
	ldr r1, .L_080f8298
	adds r4, #1
	cmp r4, r1
	bgt .L_080f80ae
	mov r2, lr
	mov r3, r12
	ldr r1, [r2]
	adds r2, r3, r4
	ands r2, r0
	ldr r3, .L_080f828c
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r8, r3
	adds r3, r5, r4
	ands r3, r0
	ldr r2, .L_080f828c
	lsls r3, r3, #2
	adds r2, r2, r3
	mov r10, r2
	mov r3, r8
	ldr r2, [r1, r3]
	mov r3, r10
	ldr r1, [r1, r3]
	cmp r2, r1
	beq .L_080f807c
.L_080f80ae:
	ldr r5, .L_080f8270
	ldr r0, .L_080f8290
	ldr r2, [r5]
	adds r1, r2, r0
	ldr r3, [r1]
	cmp r3, r4
	bge .L_080f80cc
	ldr r5, .L_080f829c
	adds r3, r2, r5
	str r7, [r3]
	movs r7, #136
	lsls r7, r7, #1
	str r4, [r1]
	cmp r4, r7
	beq .L_080f816e
.L_080f80cc:
	ldr r6, [r6]
	cmp r6, #0
	bne .L_080f803a
	b .L_080f816e
.L_080f80d4:
	cmp r6, #0
	beq .L_080f816e
	mov r2, r12
	ldr r4, .L_080f82a0
	lsls r3, r2, #2
	ldr r0, .L_080f8294
	ldr r1, .L_080f8270
	adds r4, r3, r4
	adds r2, r3, #0
	str r4, [sp, #8]
	mov r9, r0
	mov r10, r1
	mov r11, r2
.L_080f80ee:
	ldr r0, [r6, #8]
	mov r7, r12
	subs r5, r7, r0
	mov r1, r9
	ands r5, r1
	subs r3, r5, #1
	cmp r3, #62
	bhi .L_080f8168
	mov r2, r10
	mov r7, r9
	adds r3, r0, #1
	ldr r1, [r2]
	ands r3, r7
	ldr r2, .L_080f828c
	lsls r3, r3, #2
	ldr r7, [sp, #8]
	adds r3, r3, r2
	ldr r3, [r1, r3]
	ldr r2, [r1, r7]
	movs r4, #1
	cmp r2, r3
	bne .L_080f814a
	ldr r1, .L_080f8270
	adds r7, r0, #0
	ldr r2, .L_080f8294
	ldr r0, .L_080f82a0
	mov r8, r1
	mov lr, r2
	add r0, r11
.L_080f8128:
	ldr r3, .L_080f8298
	adds r4, #1
	adds r0, #4
	cmp r4, r3
	bgt .L_080f814a
	mov r2, r8
	ldr r1, [r2]
	adds r3, r7, r4
	mov r2, lr
	ands r3, r2
	ldr r2, .L_080f828c
	lsls r3, r3, #2
	adds r3, r3, r2
	ldr r3, [r1, r3]
	ldr r2, [r1, r0]
	cmp r2, r3
	beq .L_080f8128
.L_080f814a:
	mov r3, r10
	ldr r2, [r3]
	ldr r7, .L_080f8290
	adds r1, r2, r7
	ldr r3, [r1]
	cmp r3, r4
	bge .L_080f8168
	ldr r0, .L_080f829c
	adds r3, r2, r0
	str r5, [r3]
	str r4, [r1]
	movs r1, #136
	lsls r1, r1, #1
	cmp r4, r1
	beq .L_080f816e
.L_080f8168:
	ldr r6, [r6]
	cmp r6, #0
	bne .L_080f80ee
.L_080f816e:
	ldr r2, [sp, #32]
	cmp r2, #0
	beq .L_080f8176
	b .L_080f84fe
.L_080f8176:
	ldr r3, .L_080f8270
	ldr r4, .L_080f8290
	ldr r2, [r3]
	adds r1, r2, r4
	ldr r5, [r1]
	str r5, [sp, #28]
	cmp r5, #1
	bgt .L_080f8188
	b .L_080f85ee
.L_080f8188:
	ldr r7, .L_080f829c
	adds r3, r2, r7
	ldr r3, [r3]
	ldr r0, .L_080f827c
	str r3, [sp, #24]
	str r5, [sp, #20]
	adds r3, r2, r0
	ldr r3, [r3]
	ldr r4, .L_080f8294
	adds r3, #1
	ands r3, r4
	mov r12, r3
	mov r5, r12
	movs r3, #1
	ldr r7, .L_080f828c
	str r3, [r1]
	lsls r3, r5, #2
	adds r3, r3, r7
	movs r0, #1
	ldr r3, [r2, r3]
	negs r0, r0
	cmp r3, r0
	bne .L_080f81b8
	b .L_080f833e
.L_080f81b8:
	movs r1, #192
	lsls r3, r3, #2
	lsls r1, r1, #6
	adds r3, r3, r1
	ldr r6, [r2, r3]
	movs r3, #136
	lsls r3, r3, #1
	add r3, r12
	cmp r3, r4
	ble .L_080f82a4
	cmp r6, #0
	bne .L_080f81d2
	b .L_080f833e
.L_080f81d2:
	ldr r2, [r6, #8]
	mov r3, r12
	ldr r4, .L_080f8294
	subs r7, r3, r2
	ands r7, r4
	subs r3, r7, #1
	cmp r3, #62
	bhi .L_080f8266
	ldr r5, .L_080f8270
	mov lr, r5
	adds r5, r2, #0
	mov r2, lr
	ldr r1, [r2]
	ldr r0, .L_080f8294
	mov r2, r12
	adds r2, #1
	ands r2, r0
	ldr r3, .L_080f828c
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r8, r3
	adds r3, r5, #1
	ands r3, r0
	ldr r2, .L_080f828c
	lsls r3, r3, #2
	adds r2, r2, r3
	mov r10, r2
	mov r3, r8
	ldr r2, [r1, r3]
	mov r3, r10
	ldr r1, [r1, r3]
	movs r4, #1
	cmp r2, r1
	bne .L_080f8248
.L_080f8216:
	ldr r1, .L_080f8298
	adds r4, #1
	cmp r4, r1
	bgt .L_080f8248
	mov r2, lr
	mov r3, r12
	ldr r1, [r2]
	adds r2, r3, r4
	ands r2, r0
	ldr r3, .L_080f828c
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r8, r3
	adds r3, r5, r4
	ands r3, r0
	ldr r2, .L_080f828c
	lsls r3, r3, #2
	adds r2, r2, r3
	mov r10, r2
	mov r3, r8
	ldr r2, [r1, r3]
	mov r3, r10
	ldr r1, [r1, r3]
	cmp r2, r1
	beq .L_080f8216
.L_080f8248:
	ldr r5, .L_080f8270
	ldr r0, .L_080f8290
	ldr r2, [r5]
	adds r1, r2, r0
	ldr r3, [r1]
	cmp r3, r4
	bge .L_080f8266
	ldr r5, .L_080f829c
	adds r3, r2, r5
	str r7, [r3]
	movs r7, #136
	lsls r7, r7, #1
	str r4, [r1]
	cmp r4, r7
	beq .L_080f833e
.L_080f8266:
	ldr r6, [r6]
	cmp r6, #0
	bne .L_080f81d2
	b .L_080f833e
	.2byte 0x0000
.L_080f8270:
	.4byte Flash_Handler3
.L_080f8274:
	.4byte gMapCellBuffer
.L_080f8278:
	.4byte 0x00004440
.L_080f827c:
	.4byte 0x00004434
.L_080f8280:
	.4byte 0x00004438
.L_080f8284:
	.4byte 0x0000443c
.L_080f8288:
	.4byte 0x00004408
.L_080f828c:
	.4byte 0x00003404
.L_080f8290:
	.4byte 0x00004430
.L_080f8294:
	.4byte 0x000003ff
.L_080f8298:
	.4byte 0x0000010f
.L_080f829c:
	.4byte 0x0000442c
.L_080f82a0:
	.4byte 0x00003408
.L_080f82a4:
	cmp r6, #0
	beq .L_080f833e
	mov r2, r12
	ldr r4, .L_080f856c
	lsls r3, r2, #2
	ldr r0, .L_080f8570
	ldr r1, .L_080f8574
	adds r4, r3, r4
	adds r2, r3, #0
	str r4, [sp, #4]
	mov r9, r0
	mov r10, r1
	mov r11, r2
.L_080f82be:
	ldr r0, [r6, #8]
	mov r7, r12
	subs r5, r7, r0
	mov r1, r9
	ands r5, r1
	subs r3, r5, #1
	cmp r3, #62
	bhi .L_080f8338
	mov r2, r10
	mov r7, r9
	adds r3, r0, #1
	ldr r1, [r2]
	ands r3, r7
	ldr r2, .L_080f8578
	lsls r3, r3, #2
	ldr r7, [sp, #4]
	adds r3, r3, r2
	ldr r3, [r1, r3]
	ldr r2, [r1, r7]
	movs r4, #1
	cmp r2, r3
	bne .L_080f831a
	ldr r1, .L_080f8574
	adds r7, r0, #0
	ldr r2, .L_080f8570
	ldr r0, .L_080f856c
	mov r8, r1
	mov lr, r2
	add r0, r11
.L_080f82f8:
	ldr r3, .L_080f857c
	adds r4, #1
	adds r0, #4
	cmp r4, r3
	bgt .L_080f831a
	mov r2, r8
	ldr r1, [r2]
	adds r3, r7, r4
	mov r2, lr
	ands r3, r2
	ldr r2, .L_080f8578
	lsls r3, r3, #2
	adds r3, r3, r2
	ldr r3, [r1, r3]
	ldr r2, [r1, r0]
	cmp r2, r3
	beq .L_080f82f8
.L_080f831a:
	mov r3, r10
	ldr r2, [r3]
	ldr r7, .L_080f8580
	adds r1, r2, r7
	ldr r3, [r1]
	cmp r3, r4
	bge .L_080f8338
	ldr r0, .L_080f8584
	adds r3, r2, r0
	str r5, [r3]
	str r4, [r1]
	movs r1, #136
	lsls r1, r1, #1
	cmp r4, r1
	beq .L_080f833e
.L_080f8338:
	ldr r6, [r6]
	cmp r6, #0
	bne .L_080f82be
.L_080f833e:
	ldr r3, .L_080f8574
	ldr r4, .L_080f8580
	ldr r2, [r3]
	adds r1, r2, r4
	ldr r3, [r1]
	cmp r3, #2
	bgt .L_080f834e
	b .L_080f84ea
.L_080f834e:
	ldr r5, .L_080f8588
	adds r3, #1
	str r3, [sp, #12]
	adds r3, r2, r5
	ldr r3, [r3]
	ldr r7, [sp, #28]
	adds r3, r3, r7
	mov r12, r3
	ldr r3, .L_080f8570
	mov r0, r12
	ands r0, r3
	ldr r4, .L_080f8578
	movs r3, #1
	str r3, [r1]
	lsls r3, r0, #2
	adds r3, r3, r4
	movs r5, #1
	ldr r3, [r2, r3]
	negs r5, r5
	mov r12, r0
	cmp r3, r5
	bne .L_080f837c
	b .L_080f84ce
.L_080f837c:
	movs r7, #192
	lsls r3, r3, #2
	lsls r7, r7, #6
	adds r3, r3, r7
	ldr r6, [r2, r3]
	movs r3, #136
	lsls r3, r3, #1
	ldr r0, .L_080f8570
	add r3, r12
	cmp r3, r0
	ble .L_080f8434
	cmp r6, #0
	bne .L_080f8398
	b .L_080f84ce
.L_080f8398:
	ldr r2, [r6, #8]
	ldr r3, .L_080f8570
	mov r1, r12
	subs r7, r1, r2
	ands r7, r3
	subs r3, r7, #1
	cmp r3, #62
	bhi .L_080f842c
	ldr r5, .L_080f8574
	mov lr, r5
	adds r5, r2, #0
	mov r2, lr
	ldr r1, [r2]
	ldr r0, .L_080f8570
	mov r2, r12
	adds r2, #1
	ands r2, r0
	ldr r3, .L_080f8578
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r8, r3
	adds r3, r5, #1
	ands r3, r0
	ldr r2, .L_080f8578
	lsls r3, r3, #2
	adds r2, r2, r3
	mov r10, r2
	mov r3, r8
	ldr r2, [r1, r3]
	mov r3, r10
	ldr r1, [r1, r3]
	movs r4, #1
	cmp r2, r1
	bne .L_080f840e
.L_080f83dc:
	ldr r1, .L_080f857c
	adds r4, #1
	cmp r4, r1
	bgt .L_080f840e
	mov r2, lr
	mov r3, r12
	ldr r1, [r2]
	adds r2, r3, r4
	ands r2, r0
	ldr r3, .L_080f8578
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r8, r3
	adds r3, r5, r4
	ands r3, r0
	ldr r2, .L_080f8578
	lsls r3, r3, #2
	adds r2, r2, r3
	mov r10, r2
	mov r3, r8
	ldr r2, [r1, r3]
	mov r3, r10
	ldr r1, [r1, r3]
	cmp r2, r1
	beq .L_080f83dc
.L_080f840e:
	ldr r5, .L_080f8574
	ldr r0, .L_080f8580
	ldr r2, [r5]
	adds r1, r2, r0
	ldr r3, [r1]
	cmp r3, r4
	bge .L_080f842c
	ldr r5, .L_080f8584
	adds r3, r2, r5
	str r7, [r3]
	movs r7, #136
	lsls r7, r7, #1
	str r4, [r1]
	cmp r4, r7
	beq .L_080f84ce
.L_080f842c:
	ldr r6, [r6]
	cmp r6, #0
	bne .L_080f8398
	b .L_080f84ce
.L_080f8434:
	cmp r6, #0
	beq .L_080f84ce
	mov r2, r12
	ldr r4, .L_080f856c
	lsls r3, r2, #2
	ldr r0, .L_080f8570
	ldr r1, .L_080f8574
	adds r4, r3, r4
	adds r2, r3, #0
	str r4, [sp, #0]
	mov r9, r0
	mov r10, r1
	mov r11, r2
.L_080f844e:
	ldr r0, [r6, #8]
	mov r7, r12
	subs r5, r7, r0
	mov r1, r9
	ands r5, r1
	subs r3, r5, #1
	cmp r3, #62
	bhi .L_080f84c8
	mov r2, r10
	mov r7, r9
	adds r3, r0, #1
	ldr r1, [r2]
	ands r3, r7
	ldr r2, .L_080f8578
	lsls r3, r3, #2
	ldr r7, [sp, #0]
	adds r3, r3, r2
	ldr r3, [r1, r3]
	ldr r2, [r1, r7]
	movs r4, #1
	cmp r2, r3
	bne .L_080f84aa
	ldr r1, .L_080f8574
	adds r7, r0, #0
	ldr r2, .L_080f8570
	ldr r0, .L_080f856c
	mov r8, r1
	mov lr, r2
	add r0, r11
.L_080f8488:
	ldr r3, .L_080f857c
	adds r4, #1
	adds r0, #4
	cmp r4, r3
	bgt .L_080f84aa
	mov r2, r8
	ldr r1, [r2]
	adds r3, r7, r4
	mov r2, lr
	ands r3, r2
	ldr r2, .L_080f8578
	lsls r3, r3, #2
	adds r3, r3, r2
	ldr r3, [r1, r3]
	ldr r2, [r1, r0]
	cmp r2, r3
	beq .L_080f8488
.L_080f84aa:
	mov r3, r10
	ldr r2, [r3]
	ldr r7, .L_080f8580
	adds r1, r2, r7
	ldr r3, [r1]
	cmp r3, r4
	bge .L_080f84c8
	ldr r0, .L_080f8584
	adds r3, r2, r0
	str r5, [r3]
	str r4, [r1]
	movs r1, #136
	lsls r1, r1, #1
	cmp r4, r1
	beq .L_080f84ce
.L_080f84c8:
	ldr r6, [r6]
	cmp r6, #0
	bne .L_080f844e
.L_080f84ce:
	ldr r2, .L_080f8574
	ldr r4, .L_080f8580
	ldr r3, [r2]
	adds r3, r3, r4
	ldr r3, [r3]
	ldr r5, [sp, #28]
	ldr r7, [sp, #12]
	adds r3, r3, r5
	str r3, [sp, #16]
	cmp r7, r3
	blt .L_080f84ea
	movs r0, #1
	str r0, [sp, #20]
	str r0, [sp, #32]
.L_080f84ea:
	ldr r1, .L_080f8574
	ldr r4, .L_080f8584
	ldr r2, [r1]
	ldr r5, [sp, #24]
	adds r3, r2, r4
	str r5, [r3]
	ldr r7, .L_080f8580
	ldr r0, [sp, #20]
	adds r2, r2, r7
	str r0, [r2]
.L_080f84fe:
	ldr r1, .L_080f8574
	ldr r2, .L_080f8580
	ldr r5, [r1]
	adds r6, r5, r2
	ldr r3, [r6]
	cmp r3, #1
	ble .L_080f85ee
	movs r3, #0
	ldr r4, .L_080f858c
	movs r7, #208
	str r3, [sp, #32]
	lsls r7, r7, #6
	adds r1, r5, r4
	adds r3, r5, r7
	ldr r3, [r3]
	ldrb r2, [r1]
	orrs r3, r2
	strb r3, [r1]
	ldr r0, [r6]
	cmp r0, #16
	bgt .L_080f8594
	ldr r1, .L_080f8584
	adds r3, r5, r1
	ldr r2, [r3]
	ldr r3, .L_080f8564
	lsls r1, r2, #4
	ands r1, r3
	movs r3, #255
	ands r2, r3
	orrs r1, r2
	subs r3, r0, #1
	ldr r2, .L_080f8568
	lsls r3, r3, #8
	ands r3, r2
	ldr r2, .L_080f8590
	adds r0, r5, r2
	ldr r2, [r0]
	orrs r1, r3
	lsls r1, r1, #16
	ldr r7, .L_080f858c
	adds r3, r2, r4
	adds r2, #1
	asrs r4, r1, #16
	lsrs r1, r1, #24
	strb r1, [r5, r3]
	str r2, [r0]
	adds r3, r2, r7
	adds r2, #1
	strb r4, [r5, r3]
	str r2, [r0]
	b .L_080f8618
.L_080f8564:
	.4byte 0xfffff000
.L_080f8568:
	.4byte 0x00000f00
.L_080f856c:
	.4byte 0x00003408
.L_080f8570:
	.4byte 0x000003ff
.L_080f8574:
	.4byte Flash_Handler3
.L_080f8578:
	.4byte 0x00003404
.L_080f857c:
	.4byte 0x0000010f
.L_080f8580:
	.4byte 0x00004430
.L_080f8584:
	.4byte 0x0000442c
.L_080f8588:
	.4byte 0x00004434
.L_080f858c:
	.4byte 0x00004408
.L_080f8590:
	.4byte 0x00004404
.L_080f8594:
	ldr r0, .L_080f85dc
	adds r3, r5, r0
	ldr r1, [r3]
	ldr r2, .L_080f85d8
	lsls r3, r1, #4
	ands r3, r2
	movs r2, #255
	ands r1, r2
	orrs r3, r1
	ldr r1, .L_080f85e0
	lsls r3, r3, #16
	adds r0, r5, r1
	ldr r2, [r0]
	asrs r4, r3, #16
	ldr r3, .L_080f85e4
	ldr r7, .L_080f85e4
	adds r1, r2, r3
	adds r2, #1
	lsrs r3, r4, #8
	strb r3, [r5, r1]
	adds r3, r2, r7
	adds r1, r2, #1
	str r2, [r0]
	strb r4, [r5, r3]
	str r1, [r0]
	ldr r3, .L_080f85e8
	adds r2, r2, r3
	ldr r3, [r6]
	adds r1, #1
	subs r3, #17
	strb r3, [r5, r2]
	str r1, [r0]
	b .L_080f85ec
	.2byte 0x0000
.L_080f85d8:
	.4byte 0xfffff000
.L_080f85dc:
	.4byte 0x0000442c
.L_080f85e0:
	.4byte 0x00004404
.L_080f85e4:
	.4byte 0x00004408
.L_080f85e8:
	.4byte 0x00004409
.L_080f85ec:
	b .L_080f8618
.L_080f85ee:
	ldr r4, .L_080f86d8
	ldr r5, .L_080f86dc
	ldr r2, [r4]
	adds r4, r2, r5
	adds r5, #48
	ldr r1, [r4]
	ldr r7, .L_080f86e0
	adds r3, r2, r5
	ldr r3, [r3]
	adds r0, r1, r7
	ldr r7, .L_080f86e4
	lsls r3, r3, #2
	adds r3, r3, r7
	ldr r3, [r2, r3]
	strb r3, [r2, r0]
	ldr r0, .L_080f86e8
	adds r1, #1
	adds r2, r2, r0
	movs r3, #1
	str r1, [r4]
	str r3, [r2]
.L_080f8618:
	ldr r7, .L_080f86d8
	ldr r5, .L_080f86ec
	ldr r3, [r7]
	ldr r6, .L_080f86e8
	ldr r0, [r3, r5]
	movs r1, #168
	lsls r1, r1, #2
	adds r0, r0, r1
	ldr r2, [sp, #40]
	ldr r1, [r3, r6]
	bl AudioTrack_ConsumeSlotBytes
	ldr r1, [r7]
	adds r5, r1, r5
	ldr r2, [r1, r6]
	ldr r3, [r5]
	adds r3, r3, r2
	ldr r2, .L_080f86f0
	ands r3, r2
	str r3, [r5]
	movs r3, #208
	lsls r3, r3, #6
	adds r1, r1, r3
	ldr r3, [r1]
	asrs r5, r3, #1
	str r5, [r1]
	cmp r5, #0
	bne .L_080f8670
	ldr r0, [sp, #36]
	bl AudioTrack_CopyBufferedBytes
	ldr r2, [r7]
	movs r4, #208
	lsls r4, r4, #6
	adds r1, r2, r4
	movs r3, #128
	ldr r0, .L_080f86e0
	str r3, [r1]
	ldr r1, .L_080f86dc
	adds r3, r2, r0
	strb r5, [r3]
	adds r2, r2, r1
	movs r3, #1
	str r3, [r2]
.L_080f8670:
	ldr r2, [r7]
	ldr r4, .L_080f86ec
	adds r3, r2, r4
	ldr r3, [r3]
	ldr r5, .L_080f86e4
	lsls r3, r3, #2
	adds r3, r3, r5
	movs r7, #1
	ldr r3, [r2, r3]
	negs r7, r7
	cmp r3, r7
	beq .L_080f868a
	b .L_080f7ff6
.L_080f868a:
	ldr r6, .L_080f86d8
	ldr r0, .L_080f86e0
	ldr r4, [r6]
	movs r2, #208
	lsls r2, r2, #6
	adds r1, r4, r0
	adds r3, r4, r2
	ldr r3, [r3]
	ldrb r2, [r1]
	orrs r3, r2
	strb r3, [r1]
	ldr r3, .L_080f86dc
	adds r1, r4, r3
	ldr r3, [r1]
	movs r5, #0
	adds r2, r3, r0
	adds r3, #1
	strb r5, [r4, r2]
	adds r0, r3, r0
	str r3, [r1]
	adds r3, #1
	strb r5, [r4, r0]
	str r3, [r1]
	ldr r0, [sp, #36]
	bl AudioTrack_CopyBufferedBytes
	ldr r3, [r6]
	ldr r4, .L_080f86f4
	adds r3, r3, r4
	ldr r0, [r3]
	add sp, #44
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_080f86d8:
	.4byte Flash_Handler3
.L_080f86dc:
	.4byte 0x00004404
.L_080f86e0:
	.4byte 0x00004408
.L_080f86e4:
	.4byte 0x00003404
.L_080f86e8:
	.4byte 0x00004430
.L_080f86ec:
	.4byte 0x00004434
.L_080f86f0:
	.4byte 0x000003ff
.L_080f86f4:
	.4byte 0x0000443c
