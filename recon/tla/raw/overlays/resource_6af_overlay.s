.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080ce,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200815a,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200815c,"ax",%progbits
	.global Func_0200015c
	.thumb_func
Func_0200015c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	str r3, [sp, #0]
	ldr r3, .L_0200832c
	mov r8, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r1, #0
	ldr r1, [sp, #44]
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	mov r10, r1
	ldr r7, [sp, #48]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #13
	mov r1, r10
	ands r3, r1
	mov r9, r0
	cmp r3, #0
	beq .L_020081a4
	cmp r7, #0
	beq .L_020081a4
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_020081ac
.L_020081a4:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_020081ac:
	mov r3, r8
	bl Engine_ObjectCreate
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020081ba
	b .L_0200831e
.L_020081ba:
	ldr r3, [r6, #80]
	mov r1, r10
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	mov r8, r3
	bl Func_02000f7c
	ldr r2, .L_02008330
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl Func_02000f84
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r3, .L_02008334
	mov r1, r9
	str r3, [r6, #108]
	ldr r3, [sp, #0]
	adds r0, r6, #0
	str r3, [r6, #68]
	ldr r3, [sp, #36]
	str r3, [r6, #72]
	ldr r3, [sp, #40]
	str r3, [r6, #76]
	ldr r3, [r1, #80]
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl Object_SetSpritePriority
	movs r2, #100
	adds r2, r2, r6
	mov r9, r2
	mov r3, r9
	str r5, [r6, #48]
	str r5, [r6, #52]
	strh r5, [r3]
	ldr r3, .L_02008338
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0200831e
	cmp r7, #0
	beq .L_0200831e
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_0200823c
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_0200823c:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_02008274
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r3, #3
	ldrb r2, [r7]
	adds r0, r6, #0
	ands r2, r3
	mov r3, r8
	ldrb r1, [r3, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r1
	lsls r2, r2, #2
	mov r1, r8
	orrs r3, r2
	strb r3, [r1, #9]
	ldr r1, [r7]
	bl Object_SetSpritePriority
.L_02008274:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r10
	ands r2, r3
	cmp r2, #0
	beq .L_02008288
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_02008288:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_020082ce
	ldr r3, .L_02008330
	mov r1, r11
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_020082b6
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_020082c8
.L_020082b6:
	ldr r2, .L_02008338
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_02008338
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_020082c8:
	bl __divsi3
	str r0, [r6, #52]
.L_020082ce:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_020082ea
	adds r0, r6, #0
	movs r1, #1
	bl Func_02000f7c
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02000f84
.L_020082ea:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_020082fc
	ldrh r3, [r7, #32]
	mov r1, r8
	strh r3, [r1, #18]
.L_020082fc:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200830e
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_0200830e:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200831e
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_0200831e:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200832c:
	.4byte gPartyState
.L_02008330:
	.4byte Data_02001170
.L_02008334:
	.4byte BattleFx_UpdateObjectMotionScaleAndLinkedAngle
.L_02008338:
	.4byte 0xffff0000
	.section .text.x0200843c,"ax",%progbits
	.balign 4
	.global Func_0200043c
	.thumb_func
Func_0200043c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	lsls r1, r1, #16
	mov r11, r1
	mov r3, r11
	adds r7, r0, #0
	asrs r3, r3, #16
	movs r0, #192
	movs r1, #192
	mov r11, r3
	lsls r0, r0, #11
	lsls r1, r1, #8
	sub sp, #56
	bl Func_02001004
	mov r3, r11
	lsls r6, r3, #4
	adds r6, r6, r7
	ldr r0, [r6]
	ldr r2, [r6, #4]
	movs r3, #128
	lsls r3, r3, #12
	mov r9, r3
	lsls r0, r0, #20
	lsls r2, r2, #20
	movs r1, #1
	movs r3, #1
	add r0, r9
	add r2, r9
	negs r1, r1
	bl Motion_CamBounds
	bl Func_02001014
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #13
	lsls r2, r2, #9
	bl Func_02000fb4
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02000fb4
	movs r0, #156
	lsls r0, r0, #1
	adds r0, #255
	bl Func_0200106c
	ldr r3, .L_020085a0
	add r5, sp, #16
	str r3, [r5, #8]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r5, #12]
	movs r3, #1
	str r3, [r5]
	movs r3, #0
	mov r8, r3
	movs r3, #22
	adds r3, #255
	strh r3, [r5, #24]
	ldr r3, .L_020085a4
	ldr r2, [r6, #4]
	str r3, [r5, #28]
	movs r3, #160
	ldr r0, [r6]
	lsls r3, r3, #13
	lsls r2, r2, #20
	adds r2, r2, r3
	mov r3, r8
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r3, #232
	lsls r3, r3, #14
	lsls r0, r0, #20
	str r3, [sp, #8]
	mov r10, r3
	add r0, r9
	movs r1, #0
	movs r3, #0
	str r5, [sp, #12]
	bl Func_0200015c
	movs r3, #153
	lsls r3, r3, #8
	adds r3, #153
	str r3, [r5, #8]
	ldr r3, .L_020085a8
	ldr r0, [r6]
	ldr r2, [r6, #4]
	lsls r0, r0, #20
	adds r0, r0, r3
	mov r3, r8
	str r3, [sp, #0]
	str r3, [sp, #4]
	lsls r2, r2, #20
	mov r3, r10
	str r3, [sp, #8]
	add r2, r9
	movs r1, #0
	movs r3, #0
	str r5, [sp, #12]
	bl Func_0200015c
	ldr r0, [r6]
	ldr r2, [r6, #4]
	movs r3, #144
	lsls r3, r3, #13
	lsls r0, r0, #20
	adds r0, r0, r3
	mov r3, r8
	str r3, [sp, #0]
	str r3, [sp, #4]
	lsls r2, r2, #20
	mov r3, r10
	str r3, [sp, #8]
	add r2, r9
	movs r1, #0
	movs r3, #0
	str r5, [sp, #12]
	bl Func_0200015c
	movs r0, #10
	bl Battle_WaitMode0
	ldr r2, [r6]
	ldr r3, [r6, #4]
	adds r2, #63
	subs r3, #2
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r1, #100
	movs r2, #3
	movs r3, #4
	movs r0, #6
	bl Func_02000f9c
	movs r0, #10
	bl Battle_WaitMode0
	ldr r2, [r6]
	ldr r3, [r6, #4]
	adds r2, #63
	subs r3, #2
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #3
	movs r3, #4
	movs r1, #100
	movs r0, #3
	bl Func_02000f9c
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r7, #0
	mov r1, r11
	bl Func_020003fc
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020085a0:
	.4byte 0x00013333
.L_020085a4:
	.4byte Data_02001128
.L_020085a8:
	.4byte 0xfffe0000
	.section .text.x020085ac,"ax",%progbits
	.global Func_020005ac
	.thumb_func
Func_020005ac:
	push {r5, lr}
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	movs r5, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020085dc
	bl Func_02000fcc
	movs r0, #0
	bl Func_0200104c
	ldr r0, .L_02008680
	movs r1, #0
	bl Func_0200043c
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_SetBit
	movs r5, #1
.L_020085dc:
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200860c
	cmp r5, #0
	bne .L_020085f0
	bl Func_02000fcc
.L_020085f0:
	movs r0, #0
	bl Func_0200104c
	ldr r0, .L_02008680
	movs r1, #1
	bl Func_0200043c
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_SetBit
	adds r3, r5, #1
	lsls r3, r3, #16
	asrs r5, r3, #16
.L_0200860c:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008640
	cmp r5, #0
	bne .L_02008622
	bl Func_02000fcc
.L_02008622:
	movs r0, #0
	bl Func_0200104c
	ldr r0, .L_02008680
	movs r1, #2
	bl Func_0200043c
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_SetBit
	adds r3, r5, #1
	lsls r3, r3, #16
	asrs r5, r3, #16
.L_02008640:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008674
	cmp r5, #0
	bne .L_02008656
	bl Func_02000fcc
.L_02008656:
	movs r0, #0
	bl Func_0200104c
	ldr r0, .L_02008680
	movs r1, #3
	bl Func_0200043c
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_SetBit
	adds r3, r5, #1
	lsls r3, r3, #16
	asrs r5, r3, #16
.L_02008674:
	cmp r5, #0
	beq .L_0200867c
	bl Func_02000fd4
.L_0200867c:
	pop {r5, pc}
	.2byte 0x0000
.L_02008680:
	.4byte Data_0200117c
	.section .text.x02008684,"ax",%progbits
	.global Func_02000684
	.thumb_func
Func_02000684:
	push {r5, lr}
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #10
	movs r5, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020086b4
	bl Func_02000fcc
	movs r0, #0
	bl Func_0200104c
	ldr r0, .L_02008724
	movs r1, #0
	bl Func_0200043c
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #10
	bl GameFlag_SetBit
	movs r5, #1
.L_020086b4:
	movs r0, #131
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020086e8
	cmp r5, #0
	bne .L_020086ca
	bl Func_02000fcc
.L_020086ca:
	movs r0, #0
	bl Func_0200104c
	ldr r0, .L_02008724
	movs r1, #1
	bl Func_0200043c
	movs r0, #131
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_SetBit
	adds r3, r5, #1
	lsls r3, r3, #16
	asrs r5, r3, #16
.L_020086e8:
	movs r0, #195
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008718
	cmp r5, #0
	bne .L_020086fc
	bl Func_02000fcc
.L_020086fc:
	movs r0, #0
	bl Func_0200104c
	ldr r0, .L_02008724
	movs r1, #2
	bl Func_0200043c
	movs r0, #195
	lsls r0, r0, #2
	bl GameFlag_SetBit
	adds r3, r5, #1
	lsls r3, r3, #16
	asrs r5, r3, #16
.L_02008718:
	cmp r5, #0
	beq .L_02008720
	bl Func_02000fd4
.L_02008720:
	pop {r5, pc}
	.2byte 0x0000
.L_02008724:
	.4byte Data_020011cc
	.section .text.x02008760,"ax",%progbits
	.global Func_02000760
	.thumb_func
Func_02000760:
	push {r5, r6, lr}
	adds r6, r1, #0
	adds r0, r6, #0
	bl Object_GetById
	ldr r3, .L_02008850
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008854
	adds r5, r0, #0
	cmp r2, r3
	bne .L_020087be
	cmp r6, #8
	bne .L_02008792
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #16
	bne .L_02008792
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_02008792:
	cmp r6, #9
	bne .L_020087a8
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #20
	bne .L_020087a8
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_SetBit
.L_020087a8:
	cmp r6, #10
	bne .L_020087be
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #27
	bne .L_020087be
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
.L_020087be:
	ldr r3, .L_02008850
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008858
	cmp r2, r3
	bne .L_020087fc
	cmp r6, #10
	bne .L_020087e4
	ldr r3, [r5, #8]
	asrs r3, r3, #19
	cmp r3, #33
	bne .L_020087e4
	movs r0, #194
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_020087e4:
	cmp r6, #11
	bne .L_020087fc
	ldr r3, [r5, #8]
	asrs r3, r3, #19
	cmp r3, #85
	bne .L_020087fc
	ldr r3, [r5, #16]
	asrs r3, r3, #19
	cmp r3, #83
	bne .L_020087fc
	bl Func_02000728
.L_020087fc:
	ldr r3, .L_02008850
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200885c
	cmp r2, r3
	bne .L_02008824
	cmp r6, #10
	bne .L_02008824
	ldr r3, [r5, #8]
	asrs r3, r3, #19
	cmp r3, #83
	bne .L_02008824
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #9
	bl GameFlag_SetBit
.L_02008824:
	ldr r3, .L_02008850
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008860
	cmp r2, r3
	bne .L_0200884c
	cmp r6, #8
	bne .L_0200884c
	ldr r3, [r5, #8]
	asrs r3, r3, #19
	cmp r3, #87
	bne .L_0200884c
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #13
	bl GameFlag_SetBit
.L_0200884c:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008850:
	.4byte gPartyState
.L_02008854:
	.4byte 0x0000012a
.L_02008858:
	.4byte 0x0000012b
.L_0200885c:
	.4byte 0x0000012c
.L_02008860:
	.4byte 0x0000012d
	.section .text.x02008880,"ax",%progbits
	.balign 4
	.global Func_02000880
	.thumb_func
Func_02000880:
	push {r5, lr}
	sub sp, #8
	cmp r2, #49
	bgt .L_0200888e
	ldr r0, .L_0200890c
	subs r2, #40
	b .L_02008892
.L_0200888e:
	ldr r0, .L_02008910
	subs r2, #50
.L_02008892:
	cmp r1, #15
	bne .L_020088aa
	lsls r3, r2, #4
	adds r3, r3, r0
	ldr r2, [r3]
	ldr r3, [r3, #4]
	adds r2, #63
	subs r3, #2
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #3
	b .L_020088c0
.L_020088aa:
	cmp r1, #30
	bne .L_020088cc
	lsls r3, r2, #4
	adds r3, r3, r0
	ldr r2, [r3]
	ldr r3, [r3, #4]
	adds r2, #63
	subs r3, #2
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #6
.L_020088c0:
	movs r1, #100
	movs r2, #3
	movs r3, #4
	bl Func_02000f9c
	b .L_02008906
.L_020088cc:
	cmp r1, #45
	bne .L_02008906
	lsls r5, r2, #4
	adds r5, r5, r0
	ldr r2, [r5]
	ldr r3, [r5, #4]
	adds r2, #63
	subs r3, #2
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #9
	movs r1, #100
	movs r2, #3
	movs r3, #4
	bl Func_02000f9c
	ldr r3, [r5]
	ldr r2, [r5, #4]
	movs r0, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02000f94
	ldrh r0, [r5, #12]
	bl GameFlag_ClearBit
.L_02008906:
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
.L_0200890c:
	.4byte Data_0200117c
.L_02008910:
	.4byte Data_020011cc
	.section .text.x02008ba0,"ax",%progbits
	.balign 4
	.global Func_02000ba0
	.thumb_func
Func_02000ba0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r5, #0
	ldrsh r3, [r3, r5]
	cmp r3, #0
	beq .L_02008bbc
	b .L_02008cca
.L_02008bbc:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r8, r3
	cmp r3, #0
	beq .L_02008bd2
	b .L_02008cca
.L_02008bd2:
	ldr r7, .L_02008cd0
	movs r2, #1
	ldr r3, [r7]
	negs r2, r2
	cmp r3, r2
	bne .L_02008c14
	ldr r6, .L_02008cd4
	movs r1, #160
	lsls r1, r1, #19
	ldr r5, .L_02008cd8
	adds r0, r6, #0
	adds r1, #96
	movs r2, #32
	mov lr, r5
	.2byte 0xf800
	adds r1, r6, #0
	movs r2, #32
	ldr r0, .L_02008cdc
	mov lr, r5
	.2byte 0xf800
	ldr r6, .L_02008ce0
	ldr r1, .L_02008ce4
	movs r2, #32
	adds r0, r6, #0
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_02008ce8
	adds r1, r6, #0
	movs r2, #32
	mov lr, r5
	.2byte 0xf800
	mov r3, r8
	str r3, [r7]
.L_02008c14:
	ldr r3, [r7]
	cmp r3, #0
	bge .L_02008c1c
	adds r3, #7
.L_02008c1c:
	asrs r2, r3, #3
	adds r3, r2, #0
	cmp r2, #0
	bge .L_02008c26
	adds r3, r2, #3
.L_02008c26:
	asrs r1, r3, #2
	lsls r3, r1, #2
	subs r1, r2, r3
	ldr r5, .L_02008ce8
	ldr r2, .L_02008ce0
	ldr r7, .L_02008cdc
	ldr r6, .L_02008cd4
	movs r4, #0
	mov r12, r5
	mov r8, r2
	movs r0, #4
.L_02008c3c:
	adds r2, r1, r4
	adds r3, r2, #0
	cmp r2, #0
	bge .L_02008c46
	adds r3, r2, #3
.L_02008c46:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r2, r3
	ldrh r2, [r6, r0]
	lsls r3, r3, #1
	adds r3, #4
	strh r2, [r7, r3]
	mov r5, r8
	ldrh r5, [r5, r0]
	mov r2, r12
	adds r4, #1
	strh r5, [r2, r3]
	adds r0, #2
	cmp r4, #3
	ble .L_02008c3c
	ldr r0, .L_02008cec
	ldr r1, .L_02008cf0
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_02008c94
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r0
	adds r3, #4
	adds r2, #1
	stmia r3!, {r7}
	strh r2, [r0]
	movs r2, #160
	lsls r2, r2, #19
	adds r2, #96
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #8
	str r2, [r3]
.L_02008c94:
	strh r4, [r1]
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_02008cc0
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r0
	adds r3, #4
	strh r2, [r0]
	mov r2, r12
	stmia r3!, {r2}
	ldr r2, .L_02008ce4
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #8
	str r2, [r3]
.L_02008cc0:
	strh r4, [r1]
	ldr r2, .L_02008cd0
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_02008cca:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008cd0:
	.4byte Data_02001aa4
.L_02008cd4:
	.4byte Data_02001aa8
.L_02008cd8:
	.4byte IwramCopyWords
.L_02008cdc:
	.4byte Data_02001ac8
.L_02008ce0:
	.4byte Data_02001ae8
.L_02008ce4:
	.4byte 0x05000140
.L_02008ce8:
	.4byte Data_02001b08
.L_02008cec:
	.4byte gIoWriteQueue
.L_02008cf0:
	.4byte 0x04000208
	.section .text.x02008cf4,"ax",%progbits
	.global Func_02000cf4
	.thumb_func
Func_02000cf4:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #129
	adds r3, r3, r1
	lsls r2, r2, #2
	str r2, [r3]
	bl Func_0200105c
	ldr r3, .L_02008f28
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008f2c
	cmp r2, r3
	bne .L_02008d8c
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008d54
	movs r1, #132
	movs r2, #220
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02000fec
.L_02008d54:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008d70
	movs r1, #164
	movs r2, #220
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02000fec
.L_02008d70:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008d8c
	movs r1, #220
	movs r2, #172
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02000fec
.L_02008d8c:
	ldr r3, .L_02008f28
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008f30
	cmp r2, r3
	bne .L_02008e38
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008f34
	bl Scheduler_AddOrUpdateCallback
	movs r0, #0
	movs r1, #8
	movs r2, #9
	bl Func_02001064
	movs r0, #194
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008dcc
	movs r1, #132
	movs r2, #184
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02000fec
.L_02008dcc:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008de2
	ldr r0, .L_02008f38
	movs r1, #0
	bl Func_020003fc
.L_02008de2:
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008df6
	ldr r0, .L_02008f38
	movs r1, #1
	bl Func_020003fc
.L_02008df6:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008e0c
	ldr r0, .L_02008f38
	movs r1, #2
	bl Func_020003fc
.L_02008e0c:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008e22
	ldr r0, .L_02008f38
	movs r1, #3
	bl Func_020003fc
.L_02008e22:
	movs r0, #130
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008e38
	ldr r0, .L_02008f38
	movs r1, #4
	bl Func_020003fc
.L_02008e38:
	ldr r3, .L_02008f28
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008f3c
	cmp r2, r3
	bne .L_02008eba
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008f34
	bl Scheduler_AddOrUpdateCallback
	movs r0, #0
	movs r1, #8
	movs r2, #9
	bl Func_02001064
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #9
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008e7a
	movs r1, #166
	movs r2, #248
	movs r0, #10
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02000fec
.L_02008e7a:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008e90
	ldr r0, .L_02008f40
	movs r1, #0
	bl Func_020003fc
.L_02008e90:
	movs r0, #131
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008ea6
	ldr r0, .L_02008f40
	movs r1, #1
	bl Func_020003fc
.L_02008ea6:
	movs r0, #195
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008eba
	ldr r0, .L_02008f40
	movs r1, #2
	bl Func_020003fc
.L_02008eba:
	ldr r3, .L_02008f28
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008f44
	cmp r2, r3
	bne .L_02008f22
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #13
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008ee8
	movs r1, #174
	movs r2, #150
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02000fec
.L_02008ee8:
	movs r1, #17
	movs r2, #18
	movs r0, #0
	bl Func_02001064
	movs r1, #19
	movs r2, #20
	movs r0, #1
	bl Func_02001064
	movs r0, #2
	movs r1, #21
	movs r2, #22
	bl Func_02001064
	movs r5, #0
.L_02008f08:
	adds r0, r5, #0
	adds r0, #9
	bl Object_GetById
	ldr r2, [r0, #80]
	movs r1, #12
	adds r2, #37
	ldrb r3, [r2]
	adds r5, #1
	orrs r3, r1
	strb r3, [r2]
	cmp r5, #7
	ble .L_02008f08
.L_02008f22:
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
.L_02008f28:
	.4byte gPartyState
.L_02008f2c:
	.4byte 0x0000012a
.L_02008f30:
	.4byte 0x0000012b
.L_02008f34:
	.4byte Func_02000ba0
.L_02008f38:
	.4byte Data_0200117c
.L_02008f3c:
	.4byte 0x0000012c
.L_02008f40:
	.4byte Data_020011cc
.L_02008f44:
	.4byte 0x0000012d
	.section .rodata.x02009074,"a",%progbits
.L_02009074:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
.L_020090b0:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
.L_020090ec:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_02001128
Data_02001128:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000002c
	.4byte 0xc0010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_02001170
Data_02001170:
	.4byte .L_02009074
	.4byte .L_020090b0
	.4byte .L_020090ec
	.global Data_0200117c
Data_0200117c:
	.4byte 0x00000007
	.4byte 0x00000015
	.4byte 0x0000000c
	.4byte 0x00000303
	.4byte 0x00000007
	.4byte 0x00000011
	.4byte 0x0000000d
	.4byte 0x00000304
	.4byte 0x0000000c
	.4byte 0x0000000f
	.4byte 0x0000000e
	.4byte 0x00000305
	.4byte 0x00000009
	.4byte 0x0000000b
	.4byte 0x0000000f
	.4byte 0x00000306
	.4byte 0x0000002e
	.4byte 0x00000026
	.4byte 0x00000010
	.4byte 0x00000307
	.global Data_020011cc
Data_020011cc:
	.4byte 0x00000029
	.4byte 0x00000017
	.4byte 0x0000000b
	.4byte 0x0000030a
	.4byte 0x0000002f
	.4byte 0x00000017
	.4byte 0x0000000c
	.4byte 0x0000030b
	.4byte 0x00000035
	.4byte 0x00000017
	.4byte 0x0000000d
	.4byte 0x0000030c
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x0000012a
	.4byte 0x10157002
	.4byte 0xffffffff
	.4byte 0x1020312a
	.4byte 0xffffffff
	.4byte 0x1030212a
	.4byte 0xffffffff
	.4byte 0x1040512a
	.4byte 0xffffffff
	.4byte 0x1050412a
	.4byte 0xffffffff
	.4byte 0x1060712a
	.4byte 0xffffffff
	.4byte 0x1070612a
	.4byte 0xffffffff
	.4byte 0x1080112b
	.4byte 0xffffffff
	.4byte 0x0000012b
	.4byte 0x1010812a
	.4byte 0xffffffff
	.4byte 0x1020312b
	.4byte 0xffffffff
	.4byte 0x1030212b
	.4byte 0xffffffff
	.4byte 0x1040112c
	.4byte 0xffffffff
	.4byte 0x1050212c
	.4byte 0xffffffff
	.4byte 0x0000012c
	.4byte 0x1010412b
	.4byte 0xffffffff
	.4byte 0x1020512b
	.4byte 0xffffffff
	.4byte 0x1030412c
	.4byte 0xffffffff
	.4byte 0x1040312c
	.4byte 0xffffffff
	.4byte 0x1050112d
	.4byte 0xffffffff
	.4byte 0x0000012d
	.4byte 0x1010512c
	.4byte 0xffffffff
	.4byte 0x1020312d
	.4byte 0xffffffff
	.4byte 0x1030212d
	.4byte 0xffffffff
	.4byte 0x1040512d
	.4byte 0xffffffff
	.4byte 0x1050412d
	.4byte 0xffffffff
	.4byte 0x1060112e
	.4byte 0xffffffff
	.4byte 0x0000012e
	.4byte 0x1010612d
	.4byte 0xffffffff
	.4byte 0x1020312e
	.4byte 0xffffffff
	.4byte 0x1030212e
	.4byte 0xffffffff
	.4byte 0x000001ff
.L_0200931c:
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02001330
Data_02001330:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001390
Data_02001390:
	.4byte 0xffff0197
	.4byte .L_0200931c
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte .L_0200931c
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00024000
	.4byte 0xffff01a8
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01a8
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01a8
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01a8
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01a8
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001480
Data_02001480:
	.4byte 0xffff0197
	.4byte .L_0200931c
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte .L_0200931c
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
.L_020094e0:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02001504
Data_02001504:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00024000
	.4byte 0xffff0149
	.4byte .L_020094e0
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte .L_020094e0
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte .L_020094e0
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte .L_020094e0
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte .L_020094e0
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte .L_020094e0
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte .L_020094e0
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte .L_020094e0
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte .L_0200931c
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte .L_0200931c
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte .L_0200931c
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte .L_0200931c
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte .L_0200931c
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte .L_0200931c
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0x007d00f6
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200169c
Data_0200169c:
	.4byte 0x09bb00a9
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0x004300f3
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020016e4
Data_020016e4:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000760
	.4byte 0x00008c15
	.4byte 0x03000008
	.4byte Func_02000760
	.4byte 0x00008c15
	.4byte 0x03010009
	.4byte Func_02000760
	.4byte 0x00008c15
	.4byte 0x0302000a
	.4byte Func_02000760
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001780
Data_02001780:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte Func_020005ac
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte Func_02000728
	.4byte 0x00000602
	.4byte 0xffff0023
	.4byte Func_02000f4c
	.4byte 0x50009805
	.4byte 0x13030028
	.4byte Func_02000880
	.4byte 0x50009805
	.4byte 0x13040029
	.4byte Func_02000880
	.4byte 0x50009805
	.4byte 0x1305002a
	.4byte Func_02000880
	.4byte 0x50009805
	.4byte 0x1306002b
	.4byte Func_02000880
	.4byte 0x50009805
	.4byte 0x1307002c
	.4byte Func_02000880
	.4byte 0x00008c15
	.4byte 0x0308000a
	.4byte Func_02000760
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte Func_02000760
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000760
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200184c
Data_0200184c:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x0000c602
	.4byte 0xffff0023
	.4byte Func_02000f4c
	.4byte 0x00000602
	.4byte 0xffff0023
	.4byte Func_02000f4c
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte Func_02000684
	.4byte 0x50009805
	.4byte 0x130a0032
	.4byte Func_02000880
	.4byte 0x50009805
	.4byte 0x130b0033
	.4byte Func_02000880
	.4byte 0x50009805
	.4byte 0x130c0034
	.4byte Func_02000880
	.4byte 0x00008c15
	.4byte 0x0309000a
	.4byte Func_02000760
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000760
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020018f4
Data_020018f4:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00008602
	.4byte 0x0200001f
	.4byte Func_02000f4c
	.4byte 0x00000602
	.4byte 0x02000020
	.4byte Func_02000f4c
	.4byte 0x00008602
	.4byte 0x02010021
	.4byte Func_02000f4c
	.4byte 0x00000602
	.4byte 0x02010022
	.4byte Func_02000f4c
	.4byte 0x00008602
	.4byte 0x02020023
	.4byte Func_02000f4c
	.4byte 0x00000602
	.4byte 0x02020024
	.4byte Func_02000f4c
	.4byte 0x00008602
	.4byte 0x02030025
	.4byte Func_02000f4c
	.4byte 0x00000602
	.4byte 0x02030026
	.4byte Func_02000f4c
	.4byte 0x00008602
	.4byte 0x02040027
	.4byte Func_02000f4c
	.4byte 0x00000602
	.4byte 0x02040028
	.4byte Func_02000f4c
	.4byte 0x00008602
	.4byte 0x02050029
	.4byte Func_02000f4c
	.4byte 0x00000602
	.4byte 0x0205002a
	.4byte Func_02000f4c
	.4byte MsgRustySword2
	.4byte 0xffff0017
	.4byte Func_02000864
	.4byte 0x00008c15
	.4byte 0x030d0008
	.4byte Func_02000760
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000760
	.4byte 0x00008f15
	.4byte 0x02000009
	.4byte Func_02000914
	.4byte 0x00008f15
	.4byte 0x0201000a
	.4byte Func_02000940
	.4byte 0x00008f15
	.4byte 0x0202000b
	.4byte Func_02000970
	.4byte 0x00008f15
	.4byte 0x0203000c
	.4byte Func_020009a0
	.4byte 0x00008f15
	.4byte 0x0204000d
	.4byte Func_020009d0
	.4byte 0x00008f15
	.4byte 0x0205000e
	.4byte Func_020009fc
	.4byte 0x00008f15
	.4byte 0x0206000f
	.4byte Func_02000a2c
	.4byte 0x00008f15
	.4byte 0x02070010
	.4byte Func_02000a5c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001a5c
Data_02001a5c:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x09bb002a
	.4byte Func_02000a8c
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_02000870
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001aa4
Data_02001aa4:
	.4byte 0xffffffff
	.section .bss,"aw",%nobits
	.global Data_02001aa8
Data_02001aa8:
	.space 0x00000020
	.global Data_02001ac8
Data_02001ac8:
	.space 0x00000020
	.global Data_02001ae8
Data_02001ae8:
	.space 0x00000020
	.global Data_02001b08
Data_02001b08:
