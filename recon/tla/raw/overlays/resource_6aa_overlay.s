.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {lr}
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #0
	pop {pc}
	.section .text.x02008044,"ax",%progbits
	.global Func_02000044
	.thumb_func
Func_02000044:
	push {lr}
	movs r0, #11
	movs r1, #57
	bl Func_02004d38
	pop {pc}
	.section .text.x02008050,"ax",%progbits
	.global Func_02000050
	.thumb_func
Func_02000050:
	push {lr}
	movs r0, #17
	movs r1, #36
	bl Func_02004d38
	pop {pc}
	.section .text.x02008070,"ax",%progbits
	.global Func_02000070
	.thumb_func
Func_02000070:
	push {lr}
	movs r1, #65
	movs r0, #0
	bl Func_0200246c
	movs r0, #0
	pop {pc}
	.2byte 0x0000
	.section .text.x02008080,"ax",%progbits
	.global Func_02000080
	.thumb_func
Func_02000080:
	push {lr}
	ldr r3, .L_020080d8
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020080dc
	cmp r2, r3
	bne .L_02008098
	ldr r0, .L_020080e0
	b .L_020080d6
.L_02008098:
	ldr r3, .L_020080e4
	cmp r2, r3
	bne .L_020080a2
	ldr r0, .L_020080e8
	b .L_020080d6
.L_020080a2:
	ldr r3, .L_020080ec
	cmp r2, r3
	bne .L_020080ac
	ldr r0, .L_020080f0
	b .L_020080d6
.L_020080ac:
	ldr r3, .L_020080f4
	cmp r2, r3
	bne .L_020080b6
	ldr r0, .L_020080f8
	b .L_020080d6
.L_020080b6:
	ldr r3, .L_020080fc
	cmp r2, r3
	bne .L_020080c0
	ldr r0, .L_02008100
	b .L_020080d6
.L_020080c0:
	ldr r3, .L_02008104
	cmp r2, r3
	bne .L_020080ca
	ldr r0, .L_02008108
	b .L_020080d6
.L_020080ca:
	ldr r3, .L_0200810c
	cmp r2, r3
	bne .L_020080d4
	ldr r0, .L_02008110
	b .L_020080d6
.L_020080d4:
	ldr r0, .L_02008114
.L_020080d6:
	pop {pc}
.L_020080d8:
	.4byte gPartyState
.L_020080dc:
	.4byte 0x00000117
.L_020080e0:
	.4byte Data_02005b0c
.L_020080e4:
	.4byte 0x00000118
.L_020080e8:
	.4byte Data_02005b54
.L_020080ec:
	.4byte 0x00000119
.L_020080f0:
	.4byte Data_02005bcc
.L_020080f4:
	.4byte 0x0000011a
.L_020080f8:
	.4byte Data_02005d4c
.L_020080fc:
	.4byte 0x0000011b
.L_02008100:
	.4byte Data_02005eb4
.L_02008104:
	.4byte 0x0000011c
.L_02008108:
	.4byte Data_02005f74
.L_0200810c:
	.4byte 0x0000011d
.L_02008110:
	.4byte Data_020060c4
.L_02008114:
	.4byte Data_02005af4
	.section .text.x02008118,"ax",%progbits
	.global Func_02000118
	.thumb_func
Func_02000118:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #56
	adds r3, r3, r2
	movs r2, #1
	strb r2, [r3]
	bx lr
	.section .text.x0200812c,"ax",%progbits
	.global Func_0200012c
	.thumb_func
Func_0200012c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	cmp r4, #0
	bne .L_0200813c
	movs r0, #0
	b .L_02008162
.L_0200813c:
	cmp r0, #2
	bhi .L_02008150
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r3, r3, #3
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r4, [r4, r3]
	b .L_02008152
.L_02008150:
	ldr r4, .L_02008164
.L_02008152:
	lsls r3, r2, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r4, r4, r3
	ldrb r0, [r4, #2]
	ldrb r3, [r4, #3]
	lsls r0, r0, #8
	orrs r0, r3
.L_02008162:
	pop {pc}
.L_02008164:
	.4byte gMapCellBuffer
	.section .text.x02008168,"ax",%progbits
	.global Func_02000168
	.thumb_func
Func_02000168:
	push {r5, lr}
	adds r5, r1, #0
	adds r1, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	cmp r4, #0
	bne .L_0200817c
	movs r0, #0
	b .L_020081a8
.L_0200817c:
	cmp r0, #2
	bhi .L_02008190
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r3, r3, #3
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r4, [r4, r3]
	b .L_02008192
.L_02008190:
	ldr r4, .L_020081ac
.L_02008192:
	lsls r3, r2, #7
	adds r3, r5, r3
	lsls r3, r3, #2
	adds r4, r4, r3
	ldrb r0, [r4, #2]
	ldrb r3, [r4, #3]
	lsls r0, r0, #8
	orrs r0, r3
	asrs r3, r1, #8
	strb r3, [r4, #2]
	strb r1, [r4, #3]
.L_020081a8:
	pop {r5, pc}
	.2byte 0x0000
.L_020081ac:
	.4byte gMapCellBuffer
	.section .text.x020081f6,"ax",%progbits
	.2byte 0x0000
	.section .text.x020081f8,"ax",%progbits
	.global Func_020001f8
	.thumb_func
Func_020001f8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	ldr r3, [r5, #8]
	movs r0, #0
	asrs r3, r3, #20
	mov r9, r3
	ldr r3, [r5, #16]
	mov r1, r9
	asrs r3, r3, #20
	mov r10, r3
	mov r2, r10
	bl Func_0200012c
	mov r1, r9
	mov r2, r10
	mov r8, r0
	movs r0, #2
	bl Func_0200012c
	movs r2, #34
	adds r2, r2, r5
	adds r6, r0, #0
	mov r11, r2
	ldrb r0, [r2]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	bl Map_GetTerrainHeight
	adds r3, r5, #0
	adds r3, #100
	asrs r7, r0, #19
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008268
	ldr r3, .L_020082b8
	mov r2, r8
	ands r6, r3
	movs r3, #129
	negs r3, r3
	ands r2, r3
	ldr r3, [r5, #20]
	mov r8, r2
	asrs r3, r3, #19
	cmp r3, r7
	beq .L_02008282
	subs r7, #4
	b .L_02008282
.L_02008268:
	movs r3, #255
	ands r6, r3
	lsls r3, r3, #8
	mov r2, r8
	orrs r6, r3
	movs r3, #128
	orrs r2, r3
	adds r0, r5, #0
	movs r1, #2
	mov r8, r2
	adds r7, #4
	bl Object_SetSpritePriority
.L_02008282:
	mov r1, r9
	mov r2, r10
	mov r3, r8
	movs r0, #0
	bl Func_02000168
	mov r1, r9
	mov r2, r10
	adds r3, r6, #0
	movs r0, #2
	bl Func_02000168
	mov r3, r9
	mov r2, r10
	lsls r0, r3, #20
	mov r3, r11
	lsls r1, r2, #20
	ldrb r2, [r3]
	adds r3, r7, #0
	bl Func_02004c80
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020082b8:
	.4byte 0xffff00ff
	.section .text.x020082bc,"ax",%progbits
	.global Func_020002bc
	.thumb_func
Func_020002bc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_02008330
	adds r7, r0, #0
.L_020082d0:
	ldrh r0, [r7]
	bl Object_GetById
	movs r3, #4
	ldrsh r2, [r7, r3]
	movs r1, #0
	mov r8, r2
	adds r6, r0, #0
	movs r3, #2
	ldrsh r5, [r7, r3]
	bl ObjectDispatch_SetSingleChildField26
	mov r2, r8
	lsls r0, r2, #16
	lsrs r0, r0, #16
	bl GameFlag_Test
	lsls r5, r5, #16
	lsrs r5, r5, #16
	adds r5, r5, r0
	adds r1, r5, #0
	adds r0, r6, #0
	bl Func_02004bc0
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r1, r6, #0
	adds r1, #89
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	adds r3, r6, #0
	adds r3, #100
	mov r2, r8
	strh r2, [r3]
	adds r0, r6, #0
	adds r7, #6
	bl Func_020001f8
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_020082d0
.L_02008330:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008338,"ax",%progbits
	.global Func_02000338
	.thumb_func
Func_02000338:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	mov r9, r0
	movs r2, #26
	ldrsh r0, [r3, r2]
	sub sp, #56
	adds r6, r1, #0
	mov r10, r3
	bl Object_GetById
	adds r7, r0, #0
	cmp r6, #7
	bgt .L_020083cc
	add r5, sp, #16
	movs r3, #0
	mov r8, r3
	str r3, [r5]
	movs r3, #24
	adds r3, #255
	strh r3, [r5, #24]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #12]
	str r3, [r5, #8]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #20]
	str r3, [r5, #16]
	bl Random16Far
	ldr r3, .L_020083c0
	ands r0, r3
	lsls r0, r0, #12
	strh r0, [r5, #32]
	bl Random16Far
	ldr r1, [r7, #12]
	lsls r2, r6, #1
	adds r2, r2, r6
	lsls r2, r2, #16
	movs r4, #128
	subs r1, r1, r2
	lsls r4, r4, #13
	movs r3, #15
	ands r3, r0
	adds r1, r1, r4
	mov r4, r8
	ldr r2, [r7, #16]
	ldr r0, [r7, #8]
	subs r3, #8
	str r4, [sp, #0]
	str r4, [sp, #4]
	movs r4, #180
	lsls r3, r3, #13
	lsls r4, r4, #15
	str r4, [sp, #8]
	str r5, [sp, #12]
	bl Func_02003b30
	b .L_020083c4
	.2byte 0x0000
.L_020083c0:
	.4byte 0x0000000f
.L_020083c4:
	ldr r3, [r7, #28]
	ldr r2, .L_02008408
	adds r3, r3, r2
	str r3, [r7, #28]
.L_020083cc:
	mov r3, r9
	cmp r3, #1
	bne .L_020083fa
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #28]
	adds r3, r7, #0
	adds r3, #100
	movs r4, #0
	ldrsh r0, [r3, r4]
	bl GameFlag_SetBit
	mov r3, r10
	movs r2, #26
	ldrsh r0, [r3, r2]
	bl Object_GetById
	bl Func_020001f8
	movs r3, #0
	str r3, [r7, #16]
	str r3, [r7, #12]
	str r3, [r7, #8]
.L_020083fa:
	add sp, #56
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008408:
	.4byte 0xffffe100
	.section .text.x0200840c,"ax",%progbits
	.global Func_0200040c
	.thumb_func
Func_0200040c:
	push {lr}
	bl Func_02000338
	pop {pc}
	.section .text.x02008414,"ax",%progbits
	.global Func_02000414
	.thumb_func
Func_02000414:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	bl Object_GetById
	movs r1, #5
	mov r9, r0
	mov r11, r1
.L_0200842c:
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r2, r9
	ldr r2, [r2, #8]
	lsls r5, r5, #4
	mov r8, r2
	add r8, r5
	lsls r0, r0, #4
	mov r3, r8
	subs r3, r3, r0
	mov r8, r3
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r1, r9
	ldr r2, [r1, #12]
	ldr r3, [r1, #16]
	lsls r6, r6, #3
	adds r6, r6, r2
	lsls r5, r5, #4
	movs r2, #128
	lsls r0, r0, #4
	lsls r2, r2, #11
	adds r3, r3, r5
	subs r3, r3, r0
	adds r6, r6, r2
	movs r0, #70
	adds r0, #255
	mov r1, r8
	adds r2, r6, #0
	bl Func_02004bd8
	adds r7, r0, #0
	cmp r7, #0
	beq .L_020084fc
	bl Random16Far
	mov r8, r0
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	movs r3, #128
	adds r5, r0, #0
	lsls r3, r3, #7
	adds r0, r7, #0
	ldr r1, .L_02008514
	lsrs r5, r5, #1
	adds r5, r5, r3
	bl Func_02004bd0
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	mov r1, r8
	movs r2, #128
	lsls r2, r2, #10
	lsls r3, r1, #2
	adds r3, r3, r2
	str r3, [r7, #40]
	mov r0, r8
	bl Math_Cosine
	ldr r3, .L_02008518
	lsls r6, r6, #3
	adds r1, r0, #0
	mov r10, r3
	adds r0, r6, #0
	mov lr, r10
	.2byte 0xf800
	str r0, [r7, #44]
	mov r0, r8
	bl Math_Sine
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r10
	.2byte 0xf800
	movs r3, #0
	str r3, [r7, #52]
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #8
	str r0, [r7, #36]
	str r5, [r7, #24]
	str r5, [r7, #28]
	str r3, [r7, #68]
	adds r0, r7, #0
	movs r1, #1
	bl Animation_ApplyChildValues
.L_020084fc:
	movs r1, #1
	negs r1, r1
	add r11, r1
	mov r2, r11
	cmp r2, #0
	bge .L_0200842c
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008514:
	.4byte Data_02005394
.L_02008518:
	.4byte IwramMulQ16
	.section .text.x0200851c,"ax",%progbits
	.global Func_0200051c
	.thumb_func
Func_0200051c:
	push {r5, r6, lr}
	movs r0, #143
	lsls r0, r0, #2
	sub sp, #8
	bl Func_02004de0
	movs r6, #32
	movs r5, #6
.L_0200852c:
	movs r3, #39
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r0, r6, #0
	movs r1, #125
	movs r2, #3
	movs r3, #3
	bl Func_02004c30
	movs r0, #10
	bl Func_02000414
	subs r5, #1
	movs r0, #10
	bl WaitFrames
	adds r6, #3
	cmp r5, #0
	bge .L_0200852c
	bl Func_02004db0
	movs r0, #20
	bl Func_02004d30
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008564,"ax",%progbits
	.global Func_02000564
	.thumb_func
Func_02000564:
	push {r5, lr}
	ldr r5, .L_0200858c
	movs r1, #1
	adds r0, r5, #0
	adds r5, #1
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #0
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #173
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
	pop {r5, pc}
.L_0200858c:
	.4byte 0x00002e6e
	.section .text.x02008590,"ax",%progbits
	.global Func_02000590
	.thumb_func
Func_02000590:
	push {r5, r6, lr}
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #54
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200861c
	bl Func_020046b4
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	bl Func_02004dd0
	ldr r5, .L_02008644
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	str r6, [r0, #108]
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r1, #172
	movs r2, #180
	ldr r0, [r5]
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02004cd8
	movs r0, #172
	movs r1, #1
	movs r2, #180
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02004d20
	bl Func_02004dc8
	movs r0, #40
	bl Battle_WaitMode0
	bl Func_02004718
	bl BattleFx_PrepareBufferInterpolation
	movs r0, #10
	bl WaitFrames
	bl Func_02004cb0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #54
	bl GameFlag_SetBit
	b .L_02008642
.L_0200861c:
	bl Func_020046b4
	bl Func_02004dd0
	ldr r3, .L_02008644
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	movs r0, #18
	bl Func_02004d30
	movs r0, #40
	bl Battle_WaitMode0
	bl Func_02004718
.L_02008642:
	pop {r5, r6, pc}
.L_02008644:
	.4byte gPartyState
	.section .text.x02008648,"ax",%progbits
	.global Func_02000648
	.thumb_func
Func_02000648:
	push {lr}
	bl Func_020046b4
	bl Func_02004dd0
	ldr r3, .L_02008674
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	movs r0, #17
	bl Func_02004d30
	movs r0, #40
	bl Battle_WaitMode0
	bl Func_02004718
	pop {pc}
	.2byte 0x0000
.L_02008674:
	.4byte gPartyState
	.section .text.x02008678,"ax",%progbits
	.global Func_02000678
	.thumb_func
Func_02000678:
	push {r5, lr}
	sub sp, #8
	adds r5, r1, #0
	cmp r0, #1
	bne .L_020086d8
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #62
	bl Func_02004de0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02004d58
	movs r0, #60
	bl Func_02004d60
	movs r3, #47
	movs r2, #100
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #94
	movs r2, #10
	movs r3, #6
	bl Func_02004c30
	movs r3, #46
	movs r2, #40
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #110
	movs r1, #40
	movs r2, #12
	movs r3, #4
	bl Func_02004c28
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
	movs r0, #148
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
.L_020086d8:
	adds r3, r5, #0
	subs r3, #97
	cmp r3, #174
	bhi .L_020086f2
	adds r0, r5, #0
	movs r1, #30
	bl __modsi3
	cmp r0, #0
	bne .L_020086f2
	movs r0, #18
	bl Func_02000414
.L_020086f2:
	movs r3, #138
	lsls r3, r3, #1
	cmp r5, r3
	bne .L_0200870c
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	movs r1, #0
	bl Func_02004d58
	movs r0, #20
	bl Func_02004d60
.L_0200870c:
	add sp, #8
	pop {r5, pc}
	.section .text.x02008710,"ax",%progbits
	.global Func_02000710
	.thumb_func
Func_02000710:
	push {lr}
	bl Func_02001a00
	pop {pc}
	.section .text.x02008718,"ax",%progbits
	.global Func_02000718
	.thumb_func
Func_02000718:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r0, #131
	movs r3, #0
	strh r3, [r2]
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_02004d10
	movs r0, #168
	movs r2, #166
	lsls r2, r2, #18
	movs r3, #1
	movs r1, #0
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_02004d20
	movs r0, #10
	bl WaitFrames
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl Func_02004d10
	movs r0, #236
	movs r2, #166
	movs r1, #0
	lsls r2, r2, #18
	movs r3, #1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02004d20
	movs r0, #90
	bl Battle_WaitMode0
	bl Func_02004cb0
	pop {pc}
	.2byte 0x0000
	.section .text.x02008798,"ax",%progbits
	.global Func_02000798
	.thumb_func
Func_02000798:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #8
	movs r6, #14
	movs r5, #41
	movs r0, #14
	movs r1, #55
	movs r2, #14
	movs r3, #3
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02004c28
	movs r3, #78
	str r3, [sp, #0]
	mov r8, r3
	movs r0, #64
	movs r1, #64
	movs r2, #14
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02004c30
	movs r3, #104
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #14
	movs r3, #3
	str r6, [sp, #0]
	bl Func_02004c30
	mov r3, r8
	str r3, [sp, #0]
	movs r0, #64
	movs r1, #64
	movs r2, #14
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02004c28
	movs r3, #105
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #14
	movs r3, #3
	str r6, [sp, #0]
	bl Func_02004c28
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008808,"ax",%progbits
	.global Func_02000808
	.thumb_func
Func_02000808:
	push {r5, lr}
	movs r0, #20
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #71
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200885a
	movs r0, #159
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02004de0
	movs r0, #153
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	adds r3, r5, #0
	adds r3, #100
	movs r2, #0
	ldrsh r3, [r3, r2]
	movs r0, #160
	lsls r3, r3, #16
	str r3, [r5, #8]
	adds r3, r5, #0
	adds r3, #102
	movs r2, #0
	ldrsh r3, [r3, r2]
	lsls r0, r0, #4
	lsls r3, r3, #16
	str r3, [r5, #16]
	adds r0, #72
	bl GameFlag_SetBit
	bl Func_02000798
.L_0200885a:
	pop {r5, pc}
	.section .text.x0200885c,"ax",%progbits
	.global Func_0200085c
	.thumb_func
Func_0200085c:
	push {r5, r6, lr}
	sub sp, #8
	adds r6, r1, #0
	cmp r0, #1
	bne .L_020088ba
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #62
	bl Func_02004de0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02004d58
	movs r0, #60
	bl Func_02004d60
	movs r3, #100
	str r3, [sp, #4]
	movs r5, #28
	movs r0, #28
	movs r1, #92
	movs r2, #8
	movs r3, #8
	str r5, [sp, #0]
	bl Func_02004c30
	movs r3, #38
	str r3, [sp, #4]
	movs r0, #92
	movs r1, #38
	movs r2, #8
	movs r3, #8
	str r5, [sp, #0]
	bl Func_02004c28
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #71
	bl GameFlag_SetBit
.L_020088ba:
	adds r3, r6, #0
	subs r3, #97
	cmp r3, #174
	bhi .L_020088d4
	adds r0, r6, #0
	movs r1, #30
	bl __modsi3
	cmp r0, #0
	bne .L_020088d4
	movs r0, #20
	bl Func_02000414
.L_020088d4:
	movs r3, #138
	lsls r3, r3, #1
	cmp r6, r3
	bne .L_020088ee
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	movs r1, #0
	bl Func_02004d58
	movs r0, #20
	bl Func_02004d60
.L_020088ee:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020088f4,"ax",%progbits
	.global Func_020008f4
	.thumb_func
Func_020008f4:
	push {r5, lr}
	cmp r1, #8
	bne .L_0200892c
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #20
	bne .L_0200892c
	movs r0, #5
	bl Battle_WaitMode0
	ldr r3, [r5, #16]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	movs r0, #149
	adds r2, r5, #0
	str r3, [r5, #16]
	adds r2, #98
	movs r3, #1
	lsls r0, r0, #4
	strb r3, [r2]
	adds r0, #255
	bl GameFlag_SetBit
.L_0200892c:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008930,"ax",%progbits
	.global Func_02000930
	.thumb_func
Func_02000930:
	push {lr}
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
	pop {pc}
	.section .text.x0200893c,"ax",%progbits
	.global Func_0200093c
	.thumb_func
Func_0200093c:
	push {lr}
	bl Func_020046b4
	bl Func_02004dd0
	ldr r3, .L_02008968
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	movs r0, #14
	bl Func_02004d30
	movs r0, #40
	bl Battle_WaitMode0
	bl Func_02004718
	pop {pc}
	.2byte 0x0000
.L_02008968:
	.4byte gPartyState
	.section .text.x0200896c,"ax",%progbits
	.global Func_0200096c
	.thumb_func
Func_0200096c:
	push {r5, lr}
	adds r0, r1, #0
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #11
	bne .L_02008998
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r5, #52]
	movs r1, #152
	movs r3, #198
	ldr r2, [r5, #12]
	lsls r1, r1, #16
	lsls r3, r3, #18
	bl Func_02004bf8
.L_02008998:
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #10
	bne .L_020089bc
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r5, #52]
	movs r1, #200
	movs r3, #198
	ldr r2, [r5, #12]
	adds r0, r5, #0
	lsls r1, r1, #16
	lsls r3, r3, #18
	bl Func_02004bf8
.L_020089bc:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020089c0,"ax",%progbits
	.global Func_020009c0
	.thumb_func
Func_020009c0:
	push {lr}
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_02004d10
	movs r0, #176
	movs r1, #1
	movs r2, #174
	lsls r0, r0, #15
	negs r1, r1
	lsls r2, r2, #18
	movs r3, #1
	bl Motion_CamBounds
	bl Func_020019c4
	pop {pc}
	.2byte 0x0000
	.section .text.x020089e8,"ax",%progbits
	.global Func_020009e8
	.thumb_func
Func_020009e8:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r0, #131
	movs r3, #0
	strh r3, [r2]
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	bl Func_02004d20
	movs r0, #120
	bl Battle_WaitMode0
	bl Func_02004cb0
	pop {pc}
	.section .text.x02008a24,"ax",%progbits
	.global Func_02000a24
	.thumb_func
Func_02000a24:
	push {r5, lr}
	movs r0, #12
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #77
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008a72
	movs r0, #159
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02004de0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #50
	bl GameFlag_SetBit
	adds r3, r5, #0
	adds r3, #100
	movs r2, #0
	ldrsh r3, [r3, r2]
	movs r0, #160
	lsls r3, r3, #16
	str r3, [r5, #8]
	adds r3, r5, #0
	adds r3, #102
	movs r2, #0
	ldrsh r3, [r3, r2]
	lsls r0, r0, #4
	lsls r3, r3, #16
	str r3, [r5, #16]
	adds r0, #76
	bl GameFlag_SetBit
.L_02008a72:
	pop {r5, pc}
	.section .text.x02008a74,"ax",%progbits
	.global Func_02000a74
	.thumb_func
Func_02000a74:
	push {lr}
	bl Func_020019e0
	pop {pc}
	.section .text.x02008a7c,"ax",%progbits
	.global Func_02000a7c
	.thumb_func
Func_02000a7c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r0, #131
	movs r3, #0
	strh r3, [r2]
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_02004d10
	movs r0, #218
	movs r1, #1
	movs r2, #168
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02004d20
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #218
	movs r1, #1
	movs r2, #132
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02004d20
	movs r0, #120
	bl Battle_WaitMode0
	bl Func_02004cb0
	pop {pc}
	.2byte 0x0000
	.section .text.x02008af4,"ax",%progbits
	.global Func_02000af4
	.thumb_func
Func_02000af4:
	push {lr}
	bl Func_020046b4
	bl Func_02004dd0
	ldr r3, .L_02008b20
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	movs r0, #14
	bl Func_02004d30
	movs r0, #40
	bl Battle_WaitMode0
	bl Func_02004718
	pop {pc}
	.2byte 0x0000
.L_02008b20:
	.4byte gPartyState
	.section .text.x02008b24,"ax",%progbits
	.global Func_02000b24
	.thumb_func
Func_02000b24:
	push {r5, lr}
	sub sp, #8
	movs r3, #53
	str r3, [sp, #0]
	movs r5, #48
	movs r0, #53
	movs r1, #30
	movs r2, #4
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02004c20
	movs r3, #117
	str r3, [sp, #0]
	movs r0, #117
	movs r1, #30
	movs r2, #4
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02004c30
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008b54,"ax",%progbits
	.global Func_02000b54
	.thumb_func
Func_02000b54:
	push {r5, lr}
	sub sp, #8
	movs r3, #53
	str r3, [sp, #0]
	movs r5, #48
	movs r0, #53
	movs r1, #40
	movs r2, #4
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02004c20
	movs r3, #117
	str r3, [sp, #0]
	movs r0, #117
	movs r1, #45
	movs r2, #4
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02004c30
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008b84,"ax",%progbits
	.global Func_02000b84
	.thumb_func
Func_02000b84:
	push {r5, r6, lr}
	sub sp, #8
	adds r6, r1, #0
	cmp r0, #1
	bne .L_02008be2
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #62
	bl Func_02004de0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02004d58
	movs r0, #60
	bl Func_02004d60
	movs r3, #101
	str r3, [sp, #4]
	movs r5, #12
	movs r0, #12
	movs r1, #95
	movs r2, #8
	movs r3, #6
	str r5, [sp, #0]
	bl Func_02004c30
	movs r3, #40
	str r3, [sp, #4]
	movs r0, #76
	movs r1, #40
	movs r2, #8
	movs r3, #5
	str r5, [sp, #0]
	bl Func_02004c28
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #77
	bl GameFlag_SetBit
.L_02008be2:
	adds r3, r6, #0
	subs r3, #97
	cmp r3, #174
	bhi .L_02008bfc
	adds r0, r6, #0
	movs r1, #30
	bl __modsi3
	cmp r0, #0
	bne .L_02008bfc
	movs r0, #12
	bl Func_02000414
.L_02008bfc:
	movs r3, #138
	lsls r3, r3, #1
	cmp r6, r3
	bne .L_02008c16
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	movs r1, #0
	bl Func_02004d58
	movs r0, #20
	bl Func_02004d60
.L_02008c16:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008c1c,"ax",%progbits
	.global Func_02000c1c
	.thumb_func
Func_02000c1c:
	push {r5, r6, lr}
	sub sp, #8
	movs r6, #53
	movs r5, #11
	movs r0, #35
	movs r1, #11
	movs r2, #3
	movs r3, #6
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02004c28
	movs r3, #117
	str r3, [sp, #0]
	movs r0, #35
	movs r1, #75
	movs r2, #3
	movs r3, #6
	str r5, [sp, #4]
	bl Func_02004c30
	movs r3, #75
	str r3, [sp, #4]
	movs r1, #75
	movs r2, #3
	movs r3, #6
	movs r0, #35
	str r6, [sp, #0]
	bl Func_02004c30
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #73
	bl GameFlag_SetBit
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008c68,"ax",%progbits
	.global Func_02000c68
	.thumb_func
Func_02000c68:
	push {lr}
	sub sp, #8
	movs r3, #26
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #25
	movs r1, #9
	movs r2, #11
	movs r3, #3
	bl Func_02004c28
	add sp, #8
	pop {pc}
	.section .text.x02008c84,"ax",%progbits
	.global Func_02000c84
	.thumb_func
Func_02000c84:
	push {lr}
	bl Func_020046b4
	bl Func_02004dd0
	ldr r3, .L_02008cb0
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	movs r0, #12
	bl Func_02004d30
	movs r0, #40
	bl Battle_WaitMode0
	bl Func_02004718
	pop {pc}
	.2byte 0x0000
.L_02008cb0:
	.4byte gPartyState
	.section .text.x02008cb4,"ax",%progbits
	.global Func_02000cb4
	.thumb_func
Func_02000cb4:
	push {lr}
	bl Func_020046b4
	bl Func_02004dd0
	ldr r3, .L_02008ce0
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	movs r0, #13
	bl Func_02004d30
	movs r0, #40
	bl Battle_WaitMode0
	bl Func_02004718
	pop {pc}
	.2byte 0x0000
.L_02008ce0:
	.4byte gPartyState
	.section .text.x02008ce4,"ax",%progbits
	.global Func_02000ce4
	.thumb_func
Func_02000ce4:
	push {r5, lr}
	cmp r1, #8
	bne .L_02008d14
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #5
	bne .L_02008d14
	movs r0, #5
	bl Battle_WaitMode0
	ldr r3, [r5, #16]
	movs r2, #128
	lsls r2, r2, #9
	movs r0, #160
	adds r3, r3, r2
	lsls r0, r0, #4
	str r3, [r5, #16]
	adds r0, #78
	bl GameFlag_SetBit
.L_02008d14:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008d18,"ax",%progbits
	.global Func_02000d18
	.thumb_func
Func_02000d18:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	ldr r3, .L_02008dd8
	adds r2, r1, #0
	adds r2, #228
	ldr r6, [r2]
	ldr r2, [r2, #4]
	ands r6, r3
	ands r2, r3
	ldr r3, [r1]
	mov r8, r2
	ldr r3, [r3, #4]
	ldr r2, .L_02008ddc
	mov r10, r3
	ldr r3, .L_02008de0
	ldrh r3, [r3]
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	movs r2, #133
	lsrs r7, r3, #5
	ldr r3, .L_02008de4
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	asrs r3, r1, #20
	lsls r3, r3, #20
	movs r0, #0
	subs r6, r3, r6
	bl Map_GetTerrainHeight
	mov r3, r10
	subs r0, r0, r3
	ldr r3, [r5, #16]
	mov r2, r8
	asrs r3, r3, #20
	lsls r3, r3, #20
	subs r3, r3, r2
	mov r2, r10
	subs r3, r3, r2
	subs r4, r3, r0
	asrs r2, r4, #16
	adds r0, r0, r3
	asrs r6, r6, #16
	adds r4, r2, #0
	asrs r0, r0, #16
	adds r3, r6, #0
	movs r2, #167
	adds r1, r0, #0
	adds r3, #15
	lsls r2, r2, #1
	adds r4, #16
	adds r1, #58
	cmp r3, r2
	bhi .L_02008dd0
	movs r3, #15
	negs r3, r3
	cmp r4, r3
	blt .L_02008dd0
	cmp r4, #239
	bgt .L_02008dd0
	movs r3, #128
	lsls r3, r3, #1
	ldr r2, .L_02008de8
	adds r3, #255
	ands r6, r3
	movs r3, #255
	ands r4, r3
	movs r3, #0
	stmia r2!, {r3}
	lsls r3, r6, #16
	orrs r4, r3
	ldr r3, .L_02008dec
	ldr r0, .L_02008de8
	orrs r4, r3
	stmia r2!, {r4}
	movs r3, #128
	lsls r3, r3, #3
	orrs r7, r3
	str r7, [r2]
	bl Func_02004b80
.L_02008dd0:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008dd8:
	.4byte 0xffff0000
.L_02008ddc:
	.4byte ResourceTableEntries
.L_02008de0:
	.4byte Data_020068a2
.L_02008de4:
	.4byte gPartyState
.L_02008de8:
	.4byte Data_020068a4
.L_02008dec:
	.4byte 0x80008800
	.section .text.x02008df0,"ax",%progbits
	.global Func_02000df0
	.thumb_func
Func_02000df0:
	push {r5, r6, lr}
	movs r0, #128
	lsls r0, r0, #1
	bl Runtime_BumpAllocate
	adds r6, r0, #0
	adds r1, r6, #0
	movs r2, #63
.L_02008e00:
	ldr r3, .L_02008e30
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bge .L_02008e00
	ldr r5, .L_02008e34
	bl Resource_FindFreeEntry
	strh r0, [r5]
	movs r1, #128
	lsls r1, r1, #1
	adds r2, r6, #0
	ldrh r0, [r5]
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008e38
	bl Scheduler_AddOrUpdateCallback
	pop {r5, r6, pc}
.L_02008e30:
	.4byte 0x11111111
.L_02008e34:
	.4byte Data_020068a2
.L_02008e38:
	.4byte Func_02000d18
	.section .text.x02008e3c,"ax",%progbits
	.global Func_02000e3c
	.thumb_func
Func_02000e3c:
	push {r5, r6, r7, lr}
	ldr r5, .L_02008ecc
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r5, r1
	adds r7, r0, #0
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #0
	movs r1, #128
	str r3, [r6, #108]
	movs r2, #0
	lsls r1, r1, #1
	ldr r0, [r5]
	bl Func_02004d00
	adds r0, r6, #0
	movs r1, #27
	bl Func_02004bc0
	movs r1, #48
	adds r0, r6, #0
	bl ObjectDispatch_ApplyValueToChildren
	movs r0, #30
	bl WaitFrames
	movs r3, #128
	ldr r2, .L_02008ec8
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	ldr r2, .L_02008ed0
	ldr r3, [r6, #8]
	asrs r4, r3, #20
	ldr r3, [r6, #16]
	asrs r0, r3, #20
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	cmp r1, #0
	beq .L_02008ed4
	adds r3, r6, #0
	adds r3, #34
	ldrb r3, [r3]
	movs r2, #3
	ands r2, r3
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [r1, r3]
	b .L_02008ed4
.L_02008ec8:
	.4byte 0x00000000
.L_02008ecc:
	.4byte gPartyState
.L_02008ed0:
	.4byte gMapCellBuffer
.L_02008ed4:
	lsls r3, r0, #7
	adds r3, r4, r3
	lsls r3, r3, #2
	movs r1, #128
	adds r3, r2, r3
	lsls r1, r1, #2
	adds r2, r3, r1
	ldrb r2, [r2, #3]
	movs r3, #64
	ands r3, r2
	cmp r3, #0
	bne .L_02008f0a
	movs r2, #226
	movs r3, #128
	lsls r2, r2, #4
	lsls r3, r3, #19
	adds r2, #255
	adds r3, #74
	strh r2, [r3]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_02008f38
	orrs r3, r2
	strh r3, [r1]
	bl Func_02000df0
.L_02008f0a:
	ldr r3, .L_02008f3c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl Battle_WaitMode0
	movs r5, #0
.L_02008f22:
	cmp r5, #5
	bne .L_02008f2c
	movs r0, #204
	bl Func_02004de0
.L_02008f2c:
	ldr r3, [r6, #24]
	ldr r1, .L_02008f40
	ldr r2, .L_02008f44
	adds r3, r3, r1
	str r3, [r6, #24]
	b .L_02008f48
.L_02008f38:
	.4byte 0x00008000
.L_02008f3c:
	.4byte gPartyState
.L_02008f40:
	.4byte 0xfffffc00
.L_02008f44:
	.4byte 0xfffffd00
.L_02008f48:
	ldr r3, [r6, #28]
	ldr r1, .L_02008f9c
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r5, #1
	bl WaitFrames
	cmp r5, #39
	ble .L_02008f22
	ldr r3, .L_02008fa0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #0
	adds r0, #84
	strb r3, [r0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	adds r0, r7, #0
	bl Func_02004d30
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008f9c:
	.4byte 0xffff6667
.L_02008fa0:
	.4byte gPartyState
	.section .text.x02008fa4,"ax",%progbits
	.global Func_02000fa4
	.thumb_func
Func_02000fa4:
	push {lr}
	movs r0, #146
	lsls r0, r0, #2
	bl Func_02004de0
	movs r0, #126
	adds r0, #255
	bl GameFlag_SetBit
	ldr r1, .L_02008fd0
	movs r0, #11
	bl ObjectMotion_EnableActionAndSetCallback
	movs r1, #3
	movs r0, #0
	bl Func_0200246c
	movs r0, #141
	lsls r0, r0, #2
	bl GameFlag_SetBit
	pop {pc}
.L_02008fd0:
	.4byte Data_0200503c
	.section .text.x02008fd4,"ax",%progbits
	.global Func_02000fd4
	.thumb_func
Func_02000fd4:
	push {lr}
	bl Func_02003564
	bl Func_020029a0
	pop {pc}
	.section .text.x02008fe0,"ax",%progbits
	.global Func_02000fe0
	.thumb_func
Func_02000fe0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009098
	movs r2, #133
	lsls r2, r2, #2
	adds r2, r2, r3
	mov r10, r0
	ldr r0, [r2]
	mov r8, r2
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	ldr r5, [r6, #16]
	asrs r7, r3, #20
	movs r0, #208
	lsls r0, r0, #2
	adds r1, r7, #0
	bl GameFlag_SetByte
	asrs r5, r5, #20
	movs r0, #210
	adds r1, r5, #0
	lsls r0, r0, #2
	bl GameFlag_SetByte
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	mov r3, r8
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #0
	mov r2, r8
	movs r1, #128
	str r3, [r6, #108]
	ldr r0, [r2]
	lsls r1, r1, #1
	movs r2, #0
	bl Func_02004d00
	adds r0, r6, #0
	movs r1, #27
	bl Func_02004bc0
	adds r0, r6, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
	movs r0, #30
	bl WaitFrames
	movs r3, #128
	ldr r2, .L_02009090
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	mov r3, r10
	ldrb r2, [r3, #3]
	movs r3, #64
	ands r3, r2
	cmp r3, #0
	bne .L_020090a0
	movs r2, #226
	movs r3, #128
	lsls r2, r2, #4
	lsls r3, r3, #19
	adds r2, #255
	adds r3, #74
	strh r2, [r3]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_02009094
	orrs r3, r2
	strh r3, [r1]
	b .L_0200909c
	.2byte 0x0000
.L_02009090:
	.4byte 0x00000000
.L_02009094:
	.4byte 0x00008000
.L_02009098:
	.4byte gPartyState
.L_0200909c:
	bl Func_02000df0
.L_020090a0:
	mov r2, r8
	ldr r0, [r2]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl Battle_WaitMode0
	movs r7, #0
.L_020090b2:
	cmp r7, #5
	bne .L_020090bc
	movs r0, #204
	bl Func_02004de0
.L_020090bc:
	ldr r3, [r6, #24]
	ldr r2, .L_0200911c
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, .L_02009120
	ldr r3, [r6, #28]
	adds r7, #1
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r2, .L_02009124
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	bl WaitFrames
	cmp r7, #39
	ble .L_020090b2
	ldr r3, .L_02009128
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #0
	adds r0, #84
	strb r3, [r0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #11
	bl Func_02004d30
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200911c:
	.4byte 0xfffffc00
.L_02009120:
	.4byte 0xfffffd00
.L_02009124:
	.4byte 0xffff6667
.L_02009128:
	.4byte gPartyState
	.section .text.x0200912c,"ax",%progbits
	.global Func_0200112c
	.thumb_func
Func_0200112c:
	push {lr}
	bl Func_02004d40
	bl Func_02003884
	pop {pc}
	.section .text.x02009138,"ax",%progbits
	.global Func_02001138
	.thumb_func
Func_02001138:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #32]
	ldr r3, .L_02009194
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r1, .L_02009198
	asrs r6, r3, #20
	ldr r3, [r0, #16]
	asrs r4, r3, #20
	cmp r5, #0
	beq .L_02009174
	adds r3, r0, #0
	adds r3, #34
	ldrb r3, [r3]
	movs r2, #3
	ands r2, r3
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r3, r3, #3
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r1, [r5, r3]
.L_02009174:
	lsls r3, r4, #7
	adds r3, r6, r3
	lsls r3, r3, #2
	adds r1, r1, r3
	ldrb r2, [r1, #3]
	movs r3, #64
	ands r3, r2
	cmp r3, #0
	beq .L_02009190
	movs r3, #128
	lsls r3, r3, #2
	adds r0, r1, r3
	bl Func_02000fe0
.L_02009190:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009194:
	.4byte gPartyState
.L_02009198:
	.4byte gMapCellBuffer
	.section .text.x0200919c,"ax",%progbits
	.global Func_0200119c
	.thumb_func
Func_0200119c:
	push {lr}
	ldr r3, .L_020091f4
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020091f8
	cmp r2, r3
	bne .L_020091b4
	ldr r0, .L_020091fc
	b .L_020091f2
.L_020091b4:
	ldr r3, .L_02009200
	cmp r2, r3
	bne .L_020091be
	ldr r0, .L_02009204
	b .L_020091f2
.L_020091be:
	ldr r3, .L_02009208
	cmp r2, r3
	bne .L_020091c8
	ldr r0, .L_0200920c
	b .L_020091f2
.L_020091c8:
	ldr r3, .L_02009210
	cmp r2, r3
	bne .L_020091d2
	ldr r0, .L_02009214
	b .L_020091f2
.L_020091d2:
	ldr r3, .L_02009218
	cmp r2, r3
	bne .L_020091dc
	ldr r0, .L_0200921c
	b .L_020091f2
.L_020091dc:
	ldr r3, .L_02009220
	cmp r2, r3
	bne .L_020091e6
	ldr r0, .L_02009224
	b .L_020091f2
.L_020091e6:
	ldr r3, .L_02009228
	cmp r2, r3
	bne .L_020091f0
	ldr r0, .L_0200922c
	b .L_020091f2
.L_020091f0:
	ldr r0, .L_02009230
.L_020091f2:
	pop {pc}
.L_020091f4:
	.4byte gPartyState
.L_020091f8:
	.4byte 0x00000117
.L_020091fc:
	.4byte Data_02006118
.L_02009200:
	.4byte 0x00000118
.L_02009204:
	.4byte Data_0200613c
.L_02009208:
	.4byte 0x00000119
.L_0200920c:
	.4byte Data_02006250
.L_02009210:
	.4byte 0x0000011a
.L_02009214:
	.4byte Data_020063b8
.L_02009218:
	.4byte 0x0000011b
.L_0200921c:
	.4byte Data_020064fc
.L_02009220:
	.4byte 0x0000011c
.L_02009224:
	.4byte Data_02006628
.L_02009228:
	.4byte 0x0000011d
.L_0200922c:
	.4byte Data_02006778
.L_02009230:
	.4byte Data_0200610c
	.section .text.x02009234,"ax",%progbits
	.global Func_02001234
	.thumb_func
Func_02001234:
	push {r5, lr}
	bl Object_GetById
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009252
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r3, r5, #0
	adds r3, #89
	movs r2, #0
	strb r2, [r3]
	subs r3, #4
	strb r2, [r3]
.L_02009252:
	pop {r5, pc}
	.section .text.x02009254,"ax",%progbits
	.global Func_02001254
	.thumb_func
Func_02001254:
	push {r5, lr}
	ldr r3, .L_0200927c
	ldr r5, .L_02009280
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #59
	bgt .L_02009284
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	bne .L_02009298
	movs r0, #0
	movs r1, #3
	bl Func_0200246c
	ldr r3, .L_02009278
	b .L_02009296
	.2byte 0x0000
.L_02009278:
	.4byte 0x00000001
.L_0200927c:
	.4byte Data_020068a0
.L_02009280:
	.4byte Data_02006868
.L_02009284:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #1
	bne .L_02009298
	movs r0, #0
	movs r1, #67
	bl Func_0200246c
	ldr r3, .L_020092b0
.L_02009296:
	strh r3, [r5]
.L_02009298:
	ldr r2, .L_020092b4
	movs r1, #150
	ldrh r3, [r2]
	lsls r1, r1, #17
	adds r3, #1
	strh r3, [r2]
	lsls r3, r3, #16
	cmp r3, r1
	ble .L_020092b8
	ldr r3, .L_020092b0
	strh r3, [r2]
	b .L_020092b8
.L_020092b0:
	.4byte 0x00000000
.L_020092b4:
	.4byte Data_020068a0
.L_020092b8:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020092bc,"ax",%progbits
	.global Func_020012bc
	.thumb_func
Func_020012bc:
	push {lr}
	ldr r3, .L_020092e0
	ldr r2, [r3]
	movs r3, #7
	ands r2, r3
	cmp r2, #0
	beq .L_020092d0
	cmp r2, #6
	beq .L_020092d8
	b .L_020092de
.L_020092d0:
	movs r1, #0
	bl Animation_ApplyChildValues
	b .L_020092de
.L_020092d8:
	movs r1, #10
	bl Animation_ApplyChildValues
.L_020092de:
	pop {pc}
.L_020092e0:
	.4byte gFrameCount
	.section .text.x020092e4,"ax",%progbits
	.global Func_020012e4
	.thumb_func
Func_020012e4:
	push {lr}
	ldr r2, [r0, #80]
	movs r1, #128
	ldrh r3, [r2, #18]
	lsls r1, r1, #4
	adds r3, r3, r1
	adds r1, r0, #0
	adds r1, #100
	strh r3, [r2, #18]
	ldrh r3, [r1]
	movs r2, #128
	adds r3, #1
	strh r3, [r1]
	lsls r2, r2, #12
	lsls r3, r3, #16
	movs r4, #0
	cmp r3, r2
	bne .L_0200930e
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r0, #40]
.L_0200930e:
	movs r2, #0
	ldrsh r3, [r1, r2]
	cmp r3, #9
	ble .L_0200932e
	ldr r3, [r0, #8]
	movs r2, #128
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r2, .L_02009340
	ldr r3, [r0, #24]
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
.L_0200932e:
	movs r2, #0
	ldrsh r3, [r1, r2]
	cmp r3, #32
	bne .L_0200933c
	str r4, [r0, #8]
	str r4, [r0, #16]
	str r4, [r0, #108]
.L_0200933c:
	pop {pc}
	.2byte 0x0000
.L_02009340:
	.4byte 0xfffff800
	.section .text.x02009344,"ax",%progbits
	.global Func_02001344
	.thumb_func
Func_02001344:
	push {lr}
	ldr r3, [r0, #104]
	ldr r1, [r0, #8]
	ldr r3, [r3, #8]
	subs r2, r1, r3
	cmp r2, #0
	blt .L_0200935c
	movs r3, #128
	lsls r3, r3, #14
	cmp r2, r3
	bgt .L_02009372
	b .L_02009366
.L_0200935c:
	movs r2, #128
	subs r3, r3, r1
	lsls r2, r2, #14
	cmp r3, r2
	bgt .L_02009372
.L_02009366:
	adds r3, r0, #0
	adds r3, #100
	movs r2, #0
	strh r2, [r3]
	ldr r3, .L_02009374
	str r3, [r0, #108]
.L_02009372:
	pop {pc}
.L_02009374:
	.4byte Func_020012e4
	.section .text.x02009378,"ax",%progbits
	.global Func_02001378
	.thumb_func
Func_02001378:
	push {lr}
	bl Func_020029bc
	cmp r0, #0
	beq .L_02009388
	movs r0, #1
	bl Func_02003878
.L_02009388:
	pop {pc}
	.2byte 0x0000
	.section .text.x0200938c,"ax",%progbits
	.global Func_0200138c
	.thumb_func
Func_0200138c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r2, .L_02009728
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r0, #0
	ldrsh r1, [r3, r0]
	movs r0, #214
	mov r8, r1
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r7, [r3, r2]
	ldr r2, .L_0200972c
	movs r3, #0
	str r3, [r2]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	lsls r0, r0, #1
	movs r2, #129
	adds r3, r3, r0
	lsls r2, r2, #2
	str r2, [r3]
	subs r0, #154
	sub sp, #8
	bl GameFlag_SetBit
	bl Func_02002270
	ldr r3, .L_02009730
	cmp r8, r3
	bne .L_020093dc
	movs r0, #1
	bl Func_02004dd8
	b .L_020098be
.L_020093dc:
	ldr r3, .L_02009734
	cmp r8, r3
	bne .L_0200944c
	movs r0, #0
	bl Func_02004d88
	movs r0, #10
	bl Func_02001234
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009420
	movs r3, #53
	movs r5, #3
	str r3, [sp, #4]
	movs r0, #35
	movs r1, #41
	movs r2, #24
	movs r3, #10
	str r5, [sp, #0]
	bl Func_02004c30
	movs r3, #117
	str r3, [sp, #4]
	movs r0, #35
	movs r1, #105
	movs r2, #24
	movs r3, #10
	str r5, [sp, #0]
	bl Func_02004c30
.L_02009420:
	ldr r0, .L_02009738
	bl Func_020002bc
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #62
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009436
	b .L_020098be
.L_02009436:
	movs r3, #39
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #50
	movs r1, #125
	movs r2, #3
	movs r3, #3
	bl Func_02004c30
	b .L_020098be
.L_0200944c:
	ldr r3, .L_0200973c
	cmp r8, r3
	beq .L_02009454
	b .L_02009680
.L_02009454:
	movs r0, #0
	bl Func_02004d88
	ldr r0, .L_02009740
	ldr r1, .L_02009744
	movs r2, #0
	movs r3, #88
	bl Func_020022e4
	ldr r0, .L_02009748
	bl Func_020002bc
	movs r0, #148
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020094ae
	movs r3, #47
	movs r2, #100
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #94
	movs r2, #10
	movs r3, #6
	bl Func_02004c30
	movs r3, #46
	movs r2, #40
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #110
	movs r1, #40
	movs r2, #12
	movs r3, #4
	bl Func_02004c28
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
	b .L_020094e0
.L_020094ae:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #62
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020094d6
	movs r0, #140
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #18
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	b .L_020094e0
.L_020094d6:
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
.L_020094e0:
	movs r0, #20
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #20
	bl ObjectMotion_SetActionVariant
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #71
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200953c
	movs r3, #100
	str r3, [sp, #4]
	movs r5, #28
	movs r0, #28
	movs r1, #92
	movs r2, #8
	movs r3, #8
	str r5, [sp, #0]
	bl Func_02004c30
	movs r3, #38
	str r3, [sp, #4]
	movs r0, #92
	movs r1, #38
	movs r2, #8
	movs r3, #8
	str r5, [sp, #0]
	bl Func_02004c28
	bl Func_02000798
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
	b .L_02009564
.L_0200953c:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #72
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200955a
	bl Func_02000798
	movs r0, #153
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02009564
.L_0200955a:
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
.L_02009564:
	movs r0, #149
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200958c
	movs r1, #164
	movs r2, #240
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl Func_02004cd8
	movs r0, #8
	bl Object_GetById
	movs r3, #1
	adds r0, #98
	strb r3, [r0]
.L_0200958c:
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020095a4
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
	b .L_020095ae
.L_020095a4:
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
.L_020095ae:
	cmp r7, #20
	beq .L_020095b4
	b .L_020098be
.L_020095b4:
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl Motion_CamBounds
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Func_02004c50
	ldr r3, .L_02009728
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
	movs r1, #0
	movs r2, #0
	movs r0, #18
	bl Func_02004cd8
	movs r0, #18
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #18
	bl Object_GetById
	adds r3, r0, #0
	adds r3, #100
	adds r0, #102
	movs r2, #0
	ldrsh r1, [r3, r2]
	movs r3, #0
	ldrsh r2, [r0, r3]
	lsls r1, r1, #16
	lsls r2, r2, #16
	movs r0, #18
	bl Func_02004cd8
	movs r0, #159
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02004de0
	movs r5, #2
.L_0200963c:
	movs r0, #18
	bl Func_02000414
	subs r5, #1
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200963c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #62
	bl GameFlag_SetBit
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02004c50
	movs r0, #120
	bl Battle_WaitMode0
	movs r0, #20
	bl Func_02004d30
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
	b .L_020098be
.L_02009680:
	ldr r3, .L_0200974c
	cmp r8, r3
	bne .L_020096b0
	movs r0, #0
	bl Func_02004d88
	cmp r7, #10
	beq .L_02009694
	cmp r7, #1
	bne .L_020096a8
.L_02009694:
	ldr r0, .L_02009750
	ldr r1, .L_02009754
	ldr r2, .L_02009758
	movs r3, #88
	bl Func_020022e4
	movs r0, #0
	movs r1, #1
	bl Func_0200248c
.L_020096a8:
	ldr r0, .L_0200975c
	bl Func_020002bc
	b .L_020098be
.L_020096b0:
	ldr r3, .L_02009760
	cmp r8, r3
	bne .L_02009784
	movs r0, #0
	bl Func_02004d88
	movs r0, #12
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #77
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200970c
	movs r3, #101
	str r3, [sp, #4]
	movs r5, #12
	movs r0, #12
	movs r1, #95
	movs r2, #8
	movs r3, #6
	str r5, [sp, #0]
	bl Func_02004c30
	movs r3, #40
	str r3, [sp, #4]
	movs r0, #76
	movs r1, #40
	movs r2, #8
	movs r3, #5
	str r5, [sp, #0]
	bl Func_02004c28
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
	b .L_0200976e
.L_0200970c:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #76
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009764
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #50
	bl GameFlag_SetBit
	b .L_0200976e
	.2byte 0x0000
.L_02009728:
	.4byte gPartyState
.L_0200972c:
	.4byte Data_02006878
.L_02009730:
	.4byte 0x00000117
.L_02009734:
	.4byte 0x00000118
.L_02009738:
	.4byte Data_020053d8
.L_0200973c:
	.4byte 0x00000119
.L_02009740:
	.4byte Data_020053e6
.L_02009744:
	.4byte Data_020053f8
.L_02009748:
	.4byte Data_020053fe
.L_0200974c:
	.4byte 0x0000011a
.L_02009750:
	.4byte Data_0200540c
.L_02009754:
	.4byte Data_02005412
.L_02009758:
	.4byte Func_020019a4
.L_0200975c:
	.4byte Data_02005418
.L_02009760:
	.4byte 0x0000011b
.L_02009764:
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
.L_0200976e:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #73
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200977e
	b .L_020098be
.L_0200977e:
	bl Func_02000c1c
	b .L_020098be
.L_02009784:
	ldr r3, .L_020098e8
	cmp r8, r3
	beq .L_0200978c
	b .L_020098b2
.L_0200978c:
	movs r0, #0
	bl Func_02004d88
	cmp r7, #2
	beq .L_020097a2
	cmp r7, #4
	beq .L_020097a2
	cmp r7, #6
	beq .L_020097a2
	cmp r7, #13
	bne .L_020097b6
.L_020097a2:
	ldr r0, .L_020098ec
	ldr r1, .L_020098f0
	ldr r2, .L_020098f4
	movs r3, #88
	bl Func_020022e4
	movs r0, #0
	movs r1, #1
	bl Func_0200248c
.L_020097b6:
	subs r3, r7, #7
	cmp r3, #1
	bls .L_020097c0
	cmp r7, #11
	bne .L_02009884
.L_020097c0:
	ldr r0, .L_020098f8
	ldr r1, .L_020098fc
	ldr r2, .L_02009900
	movs r3, #88
	bl Func_020022e4
	movs r0, #0
	movs r1, #1
	bl Func_0200248c
	cmp r7, #11
	bne .L_020097fa
	ldr r1, .L_02009904
	movs r0, #11
	bl ObjectMotion_EnableActionAndSetCallback
	movs r1, #3
	movs r0, #0
	bl Func_0200246c
	movs r1, #144
	ldr r0, .L_02009908
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r0, #126
	adds r0, #255
	bl GameFlag_SetBit
.L_020097fa:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200990c
	bl Scheduler_AddOrUpdateCallback
	movs r0, #11
	bl Object_GetById
	ldr r3, .L_02009910
	str r3, [r0, #108]
	movs r0, #1
	bl Func_020022d8
	ldr r0, .L_02009914
	bl Func_020002bc
	movs r0, #20
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #13
	bl Object_GetById
	ldr r5, .L_02009918
	str r5, [r0, #108]
	movs r0, #14
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #15
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #16
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #17
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #18
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #13
	bl Object_GetById
	str r6, [r0, #104]
	movs r0, #14
	bl Object_GetById
	str r6, [r0, #104]
	movs r0, #15
	bl Object_GetById
	str r6, [r0, #104]
	movs r0, #16
	bl Object_GetById
	str r6, [r0, #104]
	movs r0, #17
	bl Object_GetById
	str r6, [r0, #104]
	movs r0, #18
	bl Object_GetById
	str r6, [r0, #104]
.L_02009884:
	movs r0, #9
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #78
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020098be
	movs r1, #176
	movs r2, #174
	movs r0, #8
	lsls r1, r1, #15
	lsls r2, r2, #18
	bl Func_02004cd8
	b .L_020098be
.L_020098b2:
	ldr r3, .L_0200991c
	cmp r8, r3
	bne .L_020098be
	movs r0, #0
	bl Func_02004d88
.L_020098be:
	ldr r3, .L_020098e8
	cmp r8, r3
	bne .L_020098d0
	cmp r7, #13
	bne .L_020098d0
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_ClearBit
.L_020098d0:
	ldr r0, .L_02009920
	ldr r1, .L_02009924
	ldr r2, .L_02009928
	ldr r3, .L_0200992c
	bl Func_020045f8
	movs r0, #0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020098e8:
	.4byte 0x0000011c
.L_020098ec:
	.4byte Data_02005438
.L_020098f0:
	.4byte Data_0200543e
.L_020098f4:
	.4byte Func_0200196c
.L_020098f8:
	.4byte Data_02005448
.L_020098fc:
	.4byte Data_0200544e
.L_02009900:
	.4byte Func_02001988
.L_02009904:
	.4byte Data_02005314
.L_02009908:
	.4byte Func_02001254
.L_0200990c:
	.4byte Func_02001378
.L_02009910:
	.4byte Func_020012bc
.L_02009914:
	.4byte Data_02005454
.L_02009918:
	.4byte Func_02001344
.L_0200991c:
	.4byte 0x0000011d
.L_02009920:
	.4byte Data_0200586c
.L_02009924:
	.4byte Data_0200587c
.L_02009928:
	.4byte Data_020058a8
.L_0200992c:
	.4byte Data_020058d4
	.section .text.x02009930,"ax",%progbits
	.global Func_02001930
	.thumb_func
Func_02001930:
	push {lr}
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200995a
	ldr r3, .L_02009960
	movs r2, #253
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	ldr r3, .L_02009964
	ldr r2, .L_02009968
	movs r1, #160
	subs r3, r3, r2
	adds r0, r0, r3
	lsls r1, r1, #19
	bl Func_02004c88
.L_0200995a:
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_02009960:
	.4byte gPartyState
.L_02009964:
	.4byte 0x00000121
.L_02009968:
	.4byte 0x0000010e
	.section .text.x0200996c,"ax",%progbits
	.global Func_0200196c
	.thumb_func
Func_0200196c:
	push {lr}
	ldr r3, .L_02009984
	sub sp, #4
	str r3, [sp, #0]
	movs r1, #19
	movs r2, #20
	movs r3, #0
	movs r0, #10
	bl Func_02002f4c
	add sp, #4
	pop {pc}
.L_02009984:
	.4byte Data_0200547c
	.section .text.x02009988,"ax",%progbits
	.global Func_02001988
	.thumb_func
Func_02001988:
	push {lr}
	ldr r3, .L_020099a0
	sub sp, #4
	str r3, [sp, #0]
	movs r1, #19
	movs r2, #20
	movs r3, #0
	movs r0, #12
	bl Func_02002f4c
	add sp, #4
	pop {pc}
.L_020099a0:
	.4byte Data_020054cc
	.section .text.x020099a4,"ax",%progbits
	.global Func_020019a4
	.thumb_func
Func_020019a4:
	push {lr}
	ldr r2, .L_020099c0
	sub sp, #4
	movs r3, #128
	str r2, [sp, #0]
	lsls r3, r3, #7
	movs r1, #18
	movs r2, #20
	movs r0, #9
	bl Func_02002f4c
	add sp, #4
	pop {pc}
	.2byte 0x0000
.L_020099c0:
	.4byte Data_02005528
	.section .text.x020099c4,"ax",%progbits
	.global Func_020019c4
	.thumb_func
Func_020019c4:
	push {lr}
	ldr r3, .L_020099dc
	sub sp, #4
	str r3, [sp, #0]
	movs r1, #13
	movs r2, #14
	movs r3, #0
	movs r0, #9
	bl Func_02002f4c
	add sp, #4
	pop {pc}
.L_020099dc:
	.4byte Data_02005578
	.section .text.x020099e0,"ax",%progbits
	.global Func_020019e0
	.thumb_func
Func_020019e0:
	push {lr}
	ldr r2, .L_020099fc
	sub sp, #4
	movs r3, #128
	str r2, [sp, #0]
	lsls r3, r3, #7
	movs r1, #13
	movs r2, #14
	movs r0, #11
	bl Func_02002f4c
	add sp, #4
	pop {pc}
	.2byte 0x0000
.L_020099fc:
	.4byte Data_020055d4
	.section .text.x02009a00,"ax",%progbits
	.global Func_02001a00
	.thumb_func
Func_02001a00:
	push {lr}
	ldr r3, .L_02009a18
	sub sp, #4
	str r3, [sp, #0]
	movs r1, #21
	movs r2, #22
	movs r3, #0
	movs r0, #13
	bl Func_02002f4c
	add sp, #4
	pop {pc}
.L_02009a18:
	.4byte Data_02005644
	.section .text.x02009a1c,"ax",%progbits
	.global Func_02001a1c
	.thumb_func
Func_02001a1c:
	push {lr}
	ldr r4, [r0, #8]
	ldr r3, [r1, #8]
	subs r2, r4, r3
	cmp r2, #0
	blt .L_02009a32
	movs r3, #128
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009a3c
	b .L_02009a7c
.L_02009a32:
	movs r2, #128
	subs r3, r3, r4
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009a7c
.L_02009a3c:
	ldr r4, [r0, #12]
	ldr r3, [r1, #12]
	subs r2, r4, r3
	cmp r2, #0
	blt .L_02009a50
	movs r3, #128
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009a5a
	b .L_02009a7c
.L_02009a50:
	movs r2, #128
	subs r3, r3, r4
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009a7c
.L_02009a5a:
	ldr r0, [r0, #16]
	ldr r1, [r1, #16]
	subs r2, r0, r1
	cmp r2, #0
	blt .L_02009a6e
	movs r3, #128
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009a78
	b .L_02009a7c
.L_02009a6e:
	movs r2, #128
	subs r3, r1, r0
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009a7c
.L_02009a78:
	movs r0, #1
	b .L_02009a7e
.L_02009a7c:
	movs r0, #0
.L_02009a7e:
	pop {pc}
	.section .text.x02009a80,"ax",%progbits
	.global Func_02001a80
	.thumb_func
Func_02001a80:
	push {lr}
	ldr r4, [r0, #8]
	ldr r3, [r1, #8]
	subs r2, r4, r3
	cmp r2, #0
	blt .L_02009a96
	movs r3, #192
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009aa0
	b .L_02009ad2
.L_02009a96:
	movs r2, #192
	subs r3, r3, r4
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009ad2
.L_02009aa0:
	ldr r2, [r1, #12]
	ldr r3, [r0, #12]
	subs r3, r3, r2
	ldr r2, .L_02009ad8
	adds r3, r3, r2
	ldr r2, .L_02009adc
	cmp r3, r2
	bhi .L_02009ad2
	ldr r0, [r0, #16]
	ldr r1, [r1, #16]
	subs r2, r0, r1
	cmp r2, #0
	blt .L_02009ac4
	movs r3, #192
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009ace
	b .L_02009ad2
.L_02009ac4:
	movs r2, #192
	subs r3, r1, r0
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009ad2
.L_02009ace:
	movs r0, #1
	b .L_02009ad4
.L_02009ad2:
	movs r0, #0
.L_02009ad4:
	pop {pc}
	.2byte 0x0000
.L_02009ad8:
	.4byte 0x0007ffff
.L_02009adc:
	.4byte 0x001ffffe
	.section .text.x02009ae0,"ax",%progbits
	.global Func_02001ae0
	.thumb_func
Func_02001ae0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02009c3c
	ldr r2, .L_02009c40
	mov r10, r3
	movs r3, #133
	lsls r3, r3, #2
	add r3, r10
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #4
	mov r8, r2
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r5, [r6, #68]
	mov r9, r3
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	adds r4, r0, #0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #16]
	str r4, [sp, #0]
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	ldr r4, [sp, #0]
	cmp r7, #0
	bge .L_02009b3c
	adds r3, #15
.L_02009b3c:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	ldr r1, [r6, #80]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #18]
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_02009bce
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001a1c
	cmp r0, #0
	beq .L_02009bce
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	add r3, r10
	ldrb r2, [r3]
	cmp r2, #0
	bne .L_02009bce
	ldr r1, [r6, #76]
	cmp r1, #0
	beq .L_02009ba2
	mov r3, r8
	adds r3, #104
	strh r2, [r3]
	mov r2, r8
	adds r2, #106
	cmp r1, #0
	ble .L_02009b9a
	movs r3, #1
	b .L_02009ba0
.L_02009b9a:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
.L_02009ba0:
	strh r3, [r2]
.L_02009ba2:
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r1, #0
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	mov r2, r8
	movs r3, #1
	strh r3, [r2, #4]
	ldrh r2, [r2, #10]
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	strh r2, [r3]
.L_02009bce:
	movs r3, #84
	mov r2, r8
	ldrh r0, [r2, r3]
	movs r7, #0
	cmp r0, #0
	beq .L_02009c2e
	mov r5, r8
	adds r5, #84
.L_02009bde:
	bl Object_GetById
	adds r4, r0, #0
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_02009c20
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001a80
	cmp r0, #0
	beq .L_02009c20
	ldrh r1, [r5, #2]
	cmp r1, #0
	bne .L_02009c1a
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	b .L_02009c20
.L_02009c1a:
	movs r3, #1
	mov r2, r8
	strh r3, [r2, #6]
.L_02009c20:
	adds r7, #1
	adds r5, #4
	cmp r7, #3
	bgt .L_02009c2e
	ldrh r0, [r5]
	cmp r0, #0
	bne .L_02009bde
.L_02009c2e:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009c3c:
	.4byte gPartyState
.L_02009c40:
	.4byte Data_020023c4 + 0x188
	.section .text.x02009c44,"ax",%progbits
	.global Func_02001c44
	.thumb_func
Func_02001c44:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r3, #4
	strb r6, [r3]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r1, .L_02009c70
	adds r0, r5, #0
	bl Func_02004bd0
	adds r3, r5, #0
	adds r3, #100
	strh r6, [r3]
	str r6, [r5, #48]
	str r6, [r5, #52]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009c70:
	.4byte Data_020056a0
	.section .text.x02009c74,"ax",%progbits
	.global Func_02001c74
	.thumb_func
Func_02001c74:
	push {r5, r6, r7, lr}
	ldr r3, [r0, #8]
	adds r6, r1, #0
	adds r7, r2, #0
	ldr r1, [r0]
	ldr r2, [r0, #4]
	movs r0, #183
	lsls r0, r0, #1
	bl Func_02004bd8
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009cb4
	movs r3, #0
	str r3, [r5, #68]
	str r3, [r5, #72]
	str r6, [r5, #76]
	movs r1, #2
	bl Func_02001c44
	ldr r3, .L_02009cb8
	str r3, [r5, #108]
	cmp r7, #0
	beq .L_02009cac
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
.L_02009cac:
	adds r0, r5, #0
	movs r1, #2
	bl Func_02004bc0
.L_02009cb4:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009cb8:
	.4byte Func_02001ae0
	.section .text.x02009cbc,"ax",%progbits
	.global Func_02001cbc
	.thumb_func
Func_02001cbc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02009e14
	ldr r2, .L_02009e18
	mov r10, r3
	movs r3, #133
	lsls r3, r3, #2
	add r3, r10
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #4
	mov r8, r2
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r5, [r6, #68]
	mov r9, r3
	ldr r3, [r6, #16]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #16]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #8]
	adds r4, r0, #0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #8]
	str r4, [sp, #0]
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	ldr r4, [sp, #0]
	cmp r7, #0
	bge .L_02009d18
	adds r3, #15
.L_02009d18:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	ldr r1, [r6, #80]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #18]
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_02009da8
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001a1c
	cmp r0, #0
	beq .L_02009da8
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	add r3, r10
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_02009da8
	ldr r2, [r6, #76]
	cmp r2, #0
	beq .L_02009d7c
	mov r1, r8
	adds r1, #106
	strh r3, [r1]
	subs r1, #2
	cmp r2, #0
	ble .L_02009d74
	movs r3, #1
	b .L_02009d7a
.L_02009d74:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
.L_02009d7a:
	strh r3, [r1]
.L_02009d7c:
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r1, #0
	ldr r3, [r6, #16]
	ldr r2, [r6, #68]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	mov r2, r8
	movs r3, #1
	strh r3, [r2, #4]
	ldrh r2, [r2, #10]
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	strh r2, [r3]
.L_02009da8:
	movs r3, #84
	mov r2, r8
	ldrh r0, [r2, r3]
	movs r7, #0
	cmp r0, #0
	beq .L_02009e08
	mov r5, r8
	adds r5, #84
.L_02009db8:
	bl Object_GetById
	adds r4, r0, #0
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_02009dfa
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001a80
	cmp r0, #0
	beq .L_02009dfa
	ldrh r1, [r5, #2]
	cmp r1, #0
	bne .L_02009df4
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	b .L_02009dfa
.L_02009df4:
	movs r3, #1
	mov r2, r8
	strh r3, [r2, #6]
.L_02009dfa:
	adds r7, #1
	adds r5, #4
	cmp r7, #3
	bgt .L_02009e08
	ldrh r0, [r5]
	cmp r0, #0
	bne .L_02009db8
.L_02009e08:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02009e14:
	.4byte gPartyState
.L_02009e18:
	.4byte Data_020023c4 + 0x188
	.section .text.x02009e1c,"ax",%progbits
	.global Func_02001e1c
	.thumb_func
Func_02001e1c:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r3, #4
	strb r6, [r3]
	movs r3, #3
	ldr r0, [r5, #80]
	ands r1, r3
	ldrb r2, [r0, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	lsls r1, r1, #2
	orrs r3, r1
	strb r3, [r0, #9]
	movs r1, #0
	adds r0, r5, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r1, .L_02009e5c
	adds r0, r5, #0
	bl Func_02004bd0
	adds r3, r5, #0
	adds r3, #100
	strh r6, [r3]
	str r6, [r5, #48]
	str r6, [r5, #52]
	pop {r5, r6, pc}
.L_02009e5c:
	.4byte Data_020056a0
	.section .text.x02009e60,"ax",%progbits
	.global Func_02001e60
	.thumb_func
Func_02001e60:
	push {r5, r6, r7, lr}
	ldr r3, [r0, #8]
	adds r6, r1, #0
	adds r7, r2, #0
	ldr r1, [r0]
	ldr r2, [r0, #4]
	movs r0, #183
	lsls r0, r0, #1
	bl Func_02004bd8
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009ea2
	movs r3, #0
	str r3, [r5, #68]
	str r3, [r5, #72]
	negs r3, r6
	str r3, [r5, #76]
	movs r1, #3
	bl Func_02001e1c
	ldr r3, .L_02009ea4
	str r3, [r5, #108]
	cmp r7, #0
	beq .L_02009e9a
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
.L_02009e9a:
	adds r0, r5, #0
	movs r1, #2
	bl Func_02004bc0
.L_02009ea2:
	pop {r5, r6, r7, pc}
.L_02009ea4:
	.4byte Func_02001ae0
	.section .text.x02009ea8,"ax",%progbits
	.global Func_02001ea8
	.thumb_func
Func_02001ea8:
	push {r5, r6, r7, lr}
	ldr r3, [r0, #8]
	adds r6, r1, #0
	adds r7, r2, #0
	ldr r1, [r0]
	ldr r2, [r0, #4]
	movs r0, #183
	lsls r0, r0, #1
	bl Func_02004bd8
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009ee8
	movs r3, #0
	str r3, [r5, #68]
	str r3, [r5, #72]
	str r6, [r5, #76]
	movs r1, #3
	bl Func_02001e1c
	ldr r3, .L_02009eec
	str r3, [r5, #108]
	cmp r7, #0
	beq .L_02009ee0
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
.L_02009ee0:
	adds r0, r5, #0
	movs r1, #2
	bl Func_02004bc0
.L_02009ee8:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009eec:
	.4byte Func_02001cbc
	.section .text.x02009ef0,"ax",%progbits
	.global Func_02001ef0
	.thumb_func
Func_02001ef0:
	push {r5, r6, r7, lr}
	ldr r3, [r0, #8]
	adds r6, r1, #0
	adds r7, r2, #0
	ldr r1, [r0]
	ldr r2, [r0, #4]
	movs r0, #183
	lsls r0, r0, #1
	bl Func_02004bd8
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009f32
	movs r3, #0
	str r3, [r5, #68]
	str r3, [r5, #72]
	negs r3, r6
	str r3, [r5, #76]
	movs r1, #3
	bl Func_02001e1c
	ldr r3, .L_02009f34
	str r3, [r5, #108]
	cmp r7, #0
	beq .L_02009f2a
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
.L_02009f2a:
	adds r0, r5, #0
	movs r1, #2
	bl Func_02004bc0
.L_02009f32:
	pop {r5, r6, r7, pc}
.L_02009f34:
	.4byte Func_02001cbc
	.section .text.x02009f38,"ax",%progbits
	.global Func_02001f38
	.thumb_func
Func_02001f38:
	push {lr}
	ldr r4, [r0, #8]
	ldr r3, [r1, #8]
	subs r2, r4, r3
	cmp r2, #0
	blt .L_02009f4e
	movs r3, #192
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009f58
	b .L_02009f88
.L_02009f4e:
	movs r2, #192
	subs r3, r3, r4
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009f88
.L_02009f58:
	ldr r2, [r1, #12]
	ldr r3, [r0, #12]
	subs r3, r3, r2
	ldr r2, .L_02009f8c
	subs r3, #1
	cmp r3, r2
	bhi .L_02009f88
	ldr r0, [r0, #16]
	ldr r1, [r1, #16]
	subs r2, r0, r1
	cmp r2, #0
	blt .L_02009f7a
	movs r3, #128
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009f84
	b .L_02009f88
.L_02009f7a:
	movs r2, #128
	subs r3, r1, r0
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009f88
.L_02009f84:
	movs r0, #1
	b .L_02009f8a
.L_02009f88:
	movs r0, #0
.L_02009f8a:
	pop {pc}
.L_02009f8c:
	.4byte 0x000ffffe
	.section .text.x02009f90,"ax",%progbits
	.global Func_02001f90
	.thumb_func
Func_02001f90:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200a028
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	adds r6, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldr r5, [r6, #68]
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	mov r8, r0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #16]
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_02009fd4
	adds r3, #15
.L_02009fd4:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	ldr r1, [r6, #80]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #18]
	mov r3, r8
	ldr r2, [r3, #12]
	ldr r3, [r3, #20]
	cmp r2, r3
	bne .L_0200a022
	adds r0, r6, #0
	mov r1, r8
	bl Func_02001f38
	cmp r0, #0
	beq .L_0200a022
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	movs r3, #0
	str r3, [r6, #76]
.L_0200a022:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200a028:
	.4byte gPartyState
	.section .text.x0200a02c,"ax",%progbits
	.global Func_0200202c
	.thumb_func
Func_0200202c:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r3, #4
	strb r6, [r3]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #2
	bl Func_02004bc0
	adds r0, r5, #0
	ldr r1, .L_0200a068
	bl Func_02004bd0
	adds r0, r5, #0
	movs r1, #10
	bl Object_SetPartAttribute
	adds r3, r5, #0
	adds r3, #100
	strh r6, [r3]
	str r6, [r5, #48]
	str r6, [r5, #52]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a068:
	.4byte Data_020056a0
	.section .text.x0200a06c,"ax",%progbits
	.global Func_0200206c
	.thumb_func
Func_0200206c:
	push {r5, r6, lr}
	ldr r3, [r0, #8]
	ldr r2, [r0, #4]
	adds r6, r1, #0
	ldr r1, [r0]
	ldr r0, .L_0200a0e0
	adds r3, r3, r0
	movs r0, #30
	adds r0, #255
	bl Func_02004bd8
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200a0de
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #12
	movs r3, #192
	lsls r3, r3, #6
	lsrs r0, r0, #16
	adds r0, r0, r3
	bl Math_Cosine
	str r0, [r5, #68]
	bl Random16Far
	lsls r0, r0, #16
	lsrs r0, r0, #16
	str r0, [r5, #72]
	bl Random16Far
	lsls r0, r0, #17
	lsrs r0, r0, #16
	adds r0, r0, r6
	str r0, [r5, #76]
	bl Random16Far
	ldr r3, .L_0200a0e4
	lsls r0, r0, #16
	lsrs r0, r0, #16
	adds r0, r0, r3
	adds r3, r5, #0
	adds r3, #100
	strh r0, [r3]
	movs r1, #2
	adds r0, r5, #0
	bl Func_0200202c
	ldr r3, .L_0200a0e8
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r1, #1
	bl Animation_SetStateFlags
.L_0200a0de:
	pop {r5, r6, pc}
.L_0200a0e0:
	.4byte 0xfffe0000
.L_0200a0e4:
	.4byte 0xffff8000
.L_0200a0e8:
	.4byte Func_02001f90
	.section .text.x0200a0ec,"ax",%progbits
	.global Func_020020ec
	.thumb_func
Func_020020ec:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r0, .L_0200a268
	sub sp, #8
	mov r8, r0
	movs r0, #192
	lsls r0, r0, #18
	ldr r1, [r0, #32]
	mov r7, r8
	adds r2, r1, #0
	adds r2, #228
	ldr r3, [r2]
	ldr r2, [r2, #4]
	mov r9, r3
	ldr r3, .L_0200a26c
	mov r4, r9
	ands r4, r3
	ands r2, r3
	ldr r3, [r1]
	mov r9, r4
	ldr r3, [r3, #4]
	adds r7, #20
	str r3, [sp, #4]
	mov r10, r2
	ldr r0, [r0, #108]
	str r0, [sp, #0]
	mov r0, r8
	movs r4, #6
	ldrsh r3, [r0, r4]
	cmp r3, #0
	beq .L_0200a146
	movs r1, #8
	ldrsh r3, [r0, r1]
	cmp r3, #0
	bne .L_0200a146
	ldr r3, [r0, #16]
	cmp r3, #0
	beq .L_0200a146
	mov lr, r3
	.2byte 0xf800
.L_0200a146:
	mov r2, r8
	ldrh r3, [r2, #6]
	mov r4, r8
	movs r2, #0
	mov r0, r8
	strh r3, [r4, #8]
	strh r2, [r0, #6]
	movs r1, #3
	mov r11, r1
.L_0200a158:
	movs r2, #0
	ldrsh r3, [r7, r2]
	cmp r3, #0
	beq .L_0200a248
	ldr r5, [r7, #8]
	cmp r5, #0
	beq .L_0200a248
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	ldr r4, [sp, #0]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r4, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a182
	movs r3, #1
	orrs r0, r3
.L_0200a182:
	adds r6, r5, #0
	adds r6, #91
	strb r0, [r6]
	mov r0, r8
	movs r4, #14
	ldrsh r3, [r0, r4]
	cmp r3, #0
	beq .L_0200a19c
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	strb r0, [r6]
.L_0200a19c:
	ldr r2, [r5, #8]
	mov r1, r9
	ldr r3, [r5, #16]
	subs r2, r2, r1
	ldr r1, [r5, #12]
	mov r4, r10
	subs r3, r3, r4
	subs r1, r3, r1
	movs r0, #6
	ldrsh r4, [r7, r0]
	asrs r3, r1, #16
	adds r1, r3, #0
	asrs r2, r2, #16
	subs r1, #8
	cmp r4, #0
	bne .L_0200a1d2
	adds r3, r2, #7
	movs r2, #167
	lsls r2, r2, #1
	cmp r3, r2
	bhi .L_0200a248
	movs r3, #48
	negs r3, r3
	cmp r1, r3
	ble .L_0200a248
	cmp r1, #239
	bgt .L_0200a248
.L_0200a1d2:
	movs r0, #2
	ldrsh r3, [r7, r0]
	ldrh r1, [r7, #2]
	cmp r3, #0
	bgt .L_0200a244
	ldrh r3, [r7, #4]
	movs r1, #240
	ands r1, r3
	cmp r1, #32
	beq .L_0200a218
	cmp r1, #32
	bgt .L_0200a1f4
	cmp r1, #0
	beq .L_0200a234
	cmp r1, #16
	beq .L_0200a226
	b .L_0200a240
.L_0200a1f4:
	cmp r1, #48
	beq .L_0200a20a
	cmp r1, #128
	bne .L_0200a240
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_0200206c
	b .L_0200a240
.L_0200a20a:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02001e60
	b .L_0200a240
.L_0200a218:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02001ef0
	b .L_0200a240
.L_0200a226:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02001ea8
	b .L_0200a240
.L_0200a234:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02001c74
.L_0200a240:
	movs r3, #8
	b .L_0200a246
.L_0200a244:
	subs r3, r1, #1
.L_0200a246:
	strh r3, [r7, #2]
.L_0200a248:
	movs r1, #1
	negs r1, r1
	add r11, r1
	mov r2, r11
	adds r7, #16
	cmp r2, #0
	blt .L_0200a258
	b .L_0200a158
.L_0200a258:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a268:
	.4byte Data_020023c4 + 0x188
.L_0200a26c:
	.4byte 0xffff0000
	.section .text.x0200a270,"ax",%progbits
	.global Func_02002270
	.thumb_func
Func_02002270:
	push {r5, r6, lr}
	movs r0, #10
	adds r0, #255
	ldr r6, .L_0200a2bc
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a28a
	ldr r3, .L_0200a2c0
	adds r0, r6, #0
	movs r1, #116
	mov lr, r3
	.2byte 0xf800
.L_0200a28a:
	movs r0, #110
	movs r1, #1
	movs r2, #0
	movs r3, #0
	adds r0, #255
	bl Func_02004bd8
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
	movs r1, #1
	bl Func_02004bc0
	ldr r1, [r5, #80]
	movs r2, #1
	ldrb r3, [r1, #16]
	str r5, [r6, #112]
	strh r3, [r6, #12]
	ldrb r3, [r1, #17]
	orrs r3, r2
	strb r3, [r1, #17]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a2bc:
	.4byte Data_020023c4 + 0x188
.L_0200a2c0:
	.4byte IwramClearWords
	.section .text.x0200a2c4,"ax",%progbits
	.global Func_020022c4
	.thumb_func
Func_020022c4:
	push {r5, lr}
	ldr r5, .L_0200a2d4
	ldr r0, [r5, #112]
	bl Func_02004be0
	movs r3, #0
	str r3, [r5, #112]
	pop {r5, pc}
.L_0200a2d4:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a2d8,"ax",%progbits
	.global Func_020022d8
	.thumb_func
Func_020022d8:
	ldr r3, .L_0200a2e0
	strh r0, [r3, #14]
	bx lr
	.2byte 0x0000
.L_0200a2e0:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a2e4,"ax",%progbits
	.global Func_020022e4
	.thumb_func
Func_020022e4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r8, r1
	ldr r1, .L_0200a410
	sub sp, #16
	adds r6, r0, #0
	movs r0, #10
	str r1, [sp, #4]
	adds r0, #255
	adds r1, #20
	str r2, [sp, #12]
	str r3, [sp, #8]
	mov r9, r1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a3c8
	ldrh r3, [r6]
	movs r2, #0
	mov r11, r2
	mov r10, r3
	adds r6, #2
	cmp r3, #0
	ble .L_0200a386
.L_0200a31e:
	ldrh r7, [r6]
	movs r1, #15
	ands r1, r7
	movs r3, #240
	mov r0, r10
	str r1, [sp, #0]
	ands r7, r3
	bl Object_GetById
	adds r5, r0, #0
	adds r6, #2
	cmp r5, #0
	beq .L_0200a372
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r3, r5, #0
	movs r2, #128
	adds r3, #98
	movs r1, #1
	ands r2, r7
	strb r1, [r3]
	cmp r2, #0
	bne .L_0200a352
	subs r3, #9
	strb r2, [r3]
.L_0200a352:
	mov r2, r9
	mov r3, r9
	strh r1, [r2]
	mov r0, r10
	strh r7, [r3, #4]
	bl Object_GetById
	mov r1, r9
	str r0, [r1, #8]
	ldr r2, [sp, #0]
	lsls r3, r2, #16
	str r3, [r1, #12]
	mov r3, r11
	strh r3, [r1, #2]
	movs r2, #16
	add r9, r2
.L_0200a372:
	movs r3, #1
	add r11, r3
	mov r1, r11
	cmp r1, #3
	bgt .L_0200a386
	ldrh r2, [r6]
	adds r6, #2
	mov r10, r2
	cmp r2, #0
	bgt .L_0200a31e
.L_0200a386:
	mov r3, r8
	cmp r3, #0
	beq .L_0200a3c8
	movs r1, #0
	ldrh r2, [r3]
	mov r11, r1
	ldr r1, [sp, #4]
	movs r3, #2
	add r8, r3
	movs r3, #84
	strh r2, [r1, r3]
	cmp r2, #0
	ble .L_0200a3c8
	adds r2, r1, #0
	adds r2, #84
.L_0200a3a4:
	mov r1, r8
	ldrh r3, [r1]
	movs r1, #1
	strh r3, [r2, #2]
	add r11, r1
	movs r3, #2
	add r8, r3
	mov r3, r11
	adds r2, #4
	cmp r3, #3
	bgt .L_0200a3c8
	mov r1, r8
	ldrh r3, [r1]
	movs r1, #2
	add r8, r1
	strh r3, [r2]
	cmp r3, #0
	bgt .L_0200a3a4
.L_0200a3c8:
	ldr r2, [sp, #12]
	ldr r3, [sp, #4]
	add r1, sp, #8
	str r2, [r3, #16]
	ldrh r1, [r1]
	ldr r2, [sp, #4]
	strh r1, [r2, #10]
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #80
	ldrh r3, [r1]
	cmp r3, #0
	bne .L_0200a3f8
	movs r3, #192
	movs r2, #128
	lsls r3, r3, #4
	lsls r2, r2, #19
	adds r3, #8
	adds r2, #82
	strh r3, [r2]
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #16
	strh r3, [r1]
.L_0200a3f8:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a414
	bl Scheduler_AddOrUpdateCallback
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a410:
	.4byte Data_020023c4 + 0x188
.L_0200a414:
	.4byte Func_020020ec
	.section .text.x0200a418,"ax",%progbits
	.global Func_02002418
	.thumb_func
Func_02002418:
	ldr r3, .L_0200a424
	lsls r0, r0, #4
	adds r0, r0, r3
	movs r3, #20
	ldrsh r0, [r0, r3]
	bx lr
.L_0200a424:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a428,"ax",%progbits
	.global Func_02002428
	.thumb_func
Func_02002428:
	ldr r3, .L_0200a434
	lsls r0, r0, #4
	adds r0, r0, r3
	strh r1, [r0, #20]
	bx lr
	.2byte 0x0000
.L_0200a434:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a438,"ax",%progbits
	.global Func_02002438
	.thumb_func
Func_02002438:
	ldr r3, .L_0200a440
	ldr r0, [r3]
	bx lr
	.2byte 0x0000
.L_0200a440:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a444,"ax",%progbits
	.global Func_02002444
	.thumb_func
Func_02002444:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r0, #130
	lsls r0, r0, #1
	ldr r5, .L_0200a468
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a462
	ldr r3, [r5]
	adds r3, #1
	str r3, [r5]
	cmp r3, r6
	blt .L_0200a462
	str r0, [r5]
.L_0200a462:
	ldr r0, [r5]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a468:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a46c,"ax",%progbits
	.global Func_0200246c
	.thumb_func
Func_0200246c:
	ldr r3, .L_0200a488
	lsls r0, r0, #4
	adds r0, r0, r3
	movs r3, #15
	ands r3, r1
	adds r0, #20
	lsls r3, r3, #16
	str r3, [r0, #12]
	ldr r3, .L_0200a484
	ands r1, r3
	strh r1, [r0, #4]
	bx lr
.L_0200a484:
	.4byte 0x000000f0
.L_0200a488:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a48c,"ax",%progbits
	.global Func_0200248c
	.thumb_func
Func_0200248c:
	ldr r3, .L_0200a498
	lsls r0, r0, #4
	adds r0, r0, r3
	strh r1, [r0, #26]
	bx lr
	.2byte 0x0000
.L_0200a498:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a49c,"ax",%progbits
	.global Func_0200249c
	.thumb_func
Func_0200249c:
	ldr r3, .L_0200a4a8
	lsls r0, r0, #4
	adds r0, r0, r3
	movs r3, #24
	ldrsh r0, [r0, r3]
	bx lr
.L_0200a4a8:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a4ac,"ax",%progbits
	.global Func_020024ac
	.thumb_func
Func_020024ac:
	push {lr}
	ldr r2, .L_0200a4bc
	cmp r0, #3
	bhi .L_0200a4ba
	lsls r3, r0, #2
	adds r3, #84
	strh r1, [r2, r3]
.L_0200a4ba:
	pop {pc}
.L_0200a4bc:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a4c0,"ax",%progbits
	.global Func_020024c0
	.thumb_func
Func_020024c0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #16
	ldr r6, [r3, #108]
	bl Func_02004dc0
	mov r8, r0
	bl Object_GetById
	bl Party_CountActiveOwners
	movs r5, #0
	adds r7, r0, #0
	cmp r5, r7
	bge .L_0200a502
.L_0200a4e6:
	ldr r2, .L_0200a5a8
	movs r1, #134
	lsls r1, r1, #2
	adds r3, r5, r1
	ldrb r0, [r2, r3]
	bl Owner_GetState
	ldrh r3, [r0, #56]
	lsls r2, r5, #1
	mov r1, sp
	adds r5, #1
	strh r3, [r1, r2]
	cmp r5, r7
	blt .L_0200a4e6
.L_0200a502:
	movs r0, #10
	negs r0, r0
	movs r1, #0
	bl Func_02004da8
	movs r2, #182
	lsls r2, r2, #1
	movs r4, #183
	adds r3, r6, r2
	lsls r4, r4, #1
	movs r2, #0
	strh r2, [r3]
	movs r1, #129
	adds r3, r6, r4
	strh r2, [r3]
	mov r0, r8
	lsls r1, r1, #1
	movs r5, #0
	bl Func_02004d08
	cmp r5, r7
	bge .L_0200a59c
.L_0200a52e:
	ldr r1, .L_0200a5a8
	movs r2, #134
	lsls r2, r2, #2
	adds r2, r2, r5
	ldrb r0, [r1, r2]
	mov r10, r1
	mov r8, r2
	bl Owner_GetState
	movs r4, #56
	ldrsh r3, [r0, r4]
	cmp r3, #0
	ble .L_0200a556
	movs r1, #183
	lsls r1, r1, #1
	adds r2, r6, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200a596
.L_0200a556:
	mov r3, sp
	lsls r2, r5, #1
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a596
	movs r2, #182
	lsls r2, r2, #1
	adds r1, r6, r2
	ldrh r3, [r1]
	movs r4, #184
	adds r2, r3, #1
	lsls r3, r3, #16
	lsls r4, r4, #1
	asrs r3, r3, #15
	strh r2, [r1]
	adds r3, r3, r4
	mov r1, r10
	mov r4, r8
	ldrb r2, [r1, r4]
	movs r1, #181
	strh r2, [r6, r3]
	movs r3, #255
	lsls r1, r1, #1
	lsls r3, r3, #8
	adds r2, r6, r1
	adds r3, #255
	strh r3, [r2]
	movs r3, #50
	adds r3, #255
	adds r2, r0, r3
	movs r3, #0
	strb r3, [r2]
.L_0200a596:
	adds r5, #1
	cmp r5, r7
	blt .L_0200a52e
.L_0200a59c:
	add sp, #16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a5a8:
	.4byte gPartyState
	.section .text.x0200a5ac,"ax",%progbits
	.global Func_020025ac
	.thumb_func
Func_020025ac:
	push {r5, r6, r7, lr}
	ldr r3, .L_0200a610
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	adds r6, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldr r5, [r6, #68]
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	adds r0, r5, #0
	adds r3, r3, r7
	movs r1, #18
	str r3, [r6, #16]
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_0200a5ea
	adds r3, #15
.L_0200a5ea:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	movs r1, #128
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	lsls r1, r1, #5
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r2, [r6, #80]
	ldrh r3, [r2, #18]
	adds r3, r3, r1
	strh r3, [r2, #18]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a610:
	.4byte gPartyState
	.section .text.x0200a614,"ax",%progbits
	.global Func_02002614
	.thumb_func
Func_02002614:
	push {lr}
	ldr r3, .L_0200a638
	ldr r2, [r3]
	movs r3, #7
	ands r2, r3
	cmp r2, #0
	beq .L_0200a628
	cmp r2, #4
	beq .L_0200a630
	b .L_0200a636
.L_0200a628:
	movs r1, #10
	bl Animation_ApplyChildValues
	b .L_0200a636
.L_0200a630:
	movs r1, #0
	bl Animation_ApplyChildValues
.L_0200a636:
	pop {pc}
.L_0200a638:
	.4byte gFrameCount
	.section .text.x0200a63c,"ax",%progbits
	.global Func_0200263c
	.thumb_func
Func_0200263c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_0200a830
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r5, r0
	ldrb r3, [r3]
	sub sp, #16
	cmp r3, #0
	beq .L_0200a65e
	b .L_0200a81c
.L_0200a65e:
	movs r0, #10
	movs r1, #0
	negs r0, r0
	bl Func_020024c0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r2, .L_0200a834
	adds r6, r0, #0
	str r2, [sp, #0]
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	ldr r3, .L_0200a838
	adds r0, r6, #0
	str r3, [r6, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	movs r1, #49
	bl Func_02004bc0
.L_0200a696:
	adds r3, r6, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Func_02004c18
	ldr r3, [sp, #0]
	ldr r1, [sp, #0]
	adds r3, #104
	adds r1, #106
	mov r9, r1
	mov r10, r3
	add r1, sp, #4
	cmp r0, #7
	bne .L_0200a712
	movs r2, #0
	ldrsh r3, [r3, r2]
	mov r1, r9
	lsls r3, r3, #17
	str r3, [r6, #36]
	movs r5, #0
	movs r0, #0
	ldrsh r3, [r1, r0]
	lsls r3, r3, #17
	str r3, [r6, #44]
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r6, #52]
.L_0200a6d0:
	mov r0, r10
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #19
	add r1, sp, #4
	adds r3, r3, r2
	str r3, [r1]
	mov r0, r9
	ldr r3, [r6, #12]
	str r3, [r1, #4]
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r6, #0
	bl Func_02004c38
	cmp r0, #0
	beq .L_0200a704
	movs r3, #0
	str r3, [r6, #36]
	str r3, [r6, #44]
	b .L_0200a810
.L_0200a704:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #9
	ble .L_0200a6d0
	b .L_0200a810
.L_0200a712:
	mov r0, r10
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1]
	mov r0, r9
	ldr r3, [r6, #12]
	str r3, [r1, #4]
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r6, #0
	bl Func_02004c38
	cmp r0, #0
	bgt .L_0200a810
	cmp r0, #0
	bge .L_0200a75c
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	ldr r3, .L_0200a830
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	ldr r0, [r3]
	movs r1, #6
	negs r2, r2
	bl Func_02004ce8
	b .L_0200a810
.L_0200a75c:
	ldrh r3, [r6, #32]
	movs r2, #0
	subs r3, #2
	mov r11, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	mov r8, r2
	adds r7, r5, #0
	adds r7, #89
.L_0200a770:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200a7a0
	ldrb r2, [r7]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200a7a0
	cmp r5, r6
	beq .L_0200a7a0
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	mov r1, r11
	add r2, sp, #4
	bl Func_02004c78
	cmp r0, #0
	blt .L_0200a7a0
	movs r0, #1
	bl WaitFrames
	b .L_0200a810
.L_0200a7a0:
	movs r3, #1
	add r8, r3
	mov r0, r8
	adds r7, #128
	adds r5, #128
	cmp r0, #63
	ble .L_0200a770
	mov r2, r10
	movs r1, #0
	ldrsh r3, [r2, r1]
	ldr r2, [r6, #8]
	lsls r3, r3, #17
	adds r1, r2, r3
	str r1, [r6, #8]
	mov r2, r9
	movs r0, #0
	ldrsh r3, [r2, r0]
	ldr r2, [r6, #16]
	ldr r7, .L_0200a83c
	lsls r3, r3, #17
	ldr r0, .L_0200a840
	adds r5, r2, r3
	adds r3, r1, #0
	ands r3, r7
	movs r4, #128
	adds r2, r3, r0
	lsls r4, r4, #9
	str r5, [r6, #16]
	cmp r2, r4
	ble .L_0200a7de
	adds r2, r4, #0
.L_0200a7de:
	ldr r0, .L_0200a844
	cmp r2, r0
	bge .L_0200a7e6
	adds r2, r0, #0
.L_0200a7e6:
	subs r3, r1, r2
	ldr r1, .L_0200a840
	str r3, [r6, #8]
	adds r3, r5, #0
	ands r3, r7
	adds r2, r3, r1
	cmp r2, r4
	ble .L_0200a7f8
	adds r2, r4, #0
.L_0200a7f8:
	cmp r2, r0
	bge .L_0200a7fe
	adds r2, r0, #0
.L_0200a7fe:
	subs r3, r5, r2
	str r3, [r6, #16]
	ldr r2, [sp, #0]
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #1
	bl WaitFrames
	b .L_0200a696
.L_0200a810:
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_0200a81c:
	ldr r0, [sp, #0]
	movs r3, #0
	strh r3, [r0, #4]
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a830:
	.4byte gPartyState
.L_0200a834:
	.4byte Data_020023c4 + 0x188
.L_0200a838:
	.4byte Func_02002614
.L_0200a83c:
	.4byte 0x000fffff
.L_0200a840:
	.4byte 0xfff80000
.L_0200a844:
	.4byte 0xffff0000
	.section .text.x0200a848,"ax",%progbits
	.global Func_02002848
	.thumb_func
Func_02002848:
	push {lr}
	ldr r3, .L_0200a85c
	movs r2, #4
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a858
	bl Func_0200263c
.L_0200a858:
	pop {pc}
	.2byte 0x0000
.L_0200a85c:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a860,"ax",%progbits
	.global Func_02002860
	.thumb_func
Func_02002860:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	movs r1, #217
	lsls r1, r1, #1
	adds r6, r5, r1
	ldrh r3, [r6]
	sub sp, #12
	cmp r3, #0
	bne .L_0200a88c
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r5, r2
	adds r1, #2
	ldr r0, [r3]
	adds r3, r5, r1
	ldr r1, [r3]
	bl Func_02004d50
	movs r3, #1
	strh r3, [r6]
.L_0200a88c:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r2, #179
	lsls r2, r2, #1
	movs r1, #173
	adds r3, r5, r2
	lsls r1, r1, #1
	movs r2, #0
	strh r2, [r3]
	adds r3, r5, r1
	adds r1, #4
	strh r2, [r3]
	adds r3, r5, r1
	subs r1, #2
	strh r2, [r3]
	adds r3, r5, r1
	adds r1, #8
	strh r2, [r3]
	movs r0, #10
	adds r3, r5, r1
	strh r2, [r3]
	movs r1, #0
	negs r0, r0
	bl Func_020024c0
	movs r0, #224
	movs r1, #224
	lsls r1, r1, #8
	lsls r0, r0, #11
	bl Func_02004d10
	ldr r3, .L_0200a990
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #131
	lsls r0, r0, #1
	ldr r7, .L_0200a994
	bl GameFlag_SetBit
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	ldr r3, .L_0200a998
	movs r1, #49
	str r3, [r6, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r0, r6, #0
	bl Func_02004bc0
	ldr r3, [r7, #108]
	adds r3, #100
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200a978
.L_0200a918:
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	movs r0, #0
	bl Func_02004c18
	ldr r1, [r7, #108]
	cmp r0, #7
	beq .L_0200a938
	ldr r2, [r1, #112]
	ldr r3, [r6, #8]
	adds r3, r3, r2
	str r3, [r6, #8]
	ldr r3, [r6, #16]
	ldr r2, [r1, #120]
	adds r3, r3, r2
	b .L_0200a960
.L_0200a938:
	ldr r3, [r1, #112]
	cmp r3, #0
	beq .L_0200a94c
	ldr r3, [r6, #8]
	ldr r2, .L_0200a99c
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r6, #8]
.L_0200a94c:
	ldr r3, [r7, #108]
	ldr r3, [r3, #120]
	cmp r3, #0
	beq .L_0200a962
	ldr r3, [r6, #16]
	ldr r2, .L_0200a99c
	movs r1, #128
	ands r3, r2
	lsls r1, r1, #12
	adds r3, r3, r1
.L_0200a960:
	str r3, [r6, #16]
.L_0200a962:
	movs r3, #0
	strh r3, [r7, #4]
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #108]
	adds r3, #100
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a918
.L_0200a978:
	movs r5, #0
	movs r0, #30
	bl Battle_WaitMode0
	adds r0, r6, #0
	str r5, [r6, #108]
	movs r1, #0
	bl Animation_ApplyChildValues
	strh r5, [r7, #4]
	add sp, #12
	pop {r5, r6, r7, pc}
.L_0200a990:
	.4byte gPartyState
.L_0200a994:
	.4byte Data_020023c4 + 0x188
.L_0200a998:
	.4byte Func_02002614
.L_0200a99c:
	.4byte 0xfff00000
	.section .text.x0200a9a0,"ax",%progbits
	.global Func_020029a0
	.thumb_func
Func_020029a0:
	push {lr}
	ldr r3, .L_0200a9b8
	movs r2, #4
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a9b4
	bl Func_02002860
	movs r0, #1
	b .L_0200a9b6
.L_0200a9b4:
	movs r0, #0
.L_0200a9b6:
	pop {pc}
.L_0200a9b8:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a9bc,"ax",%progbits
	.global Func_020029bc
	.thumb_func
Func_020029bc:
	ldr r3, .L_0200a9c4
	movs r2, #4
	ldrsh r0, [r3, r2]
	bx lr
.L_0200a9c4:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a9c8,"ax",%progbits
	.global Func_020029c8
	.thumb_func
Func_020029c8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_0200ab30
	sub sp, #4
	ldr r3, [r1, #112]
	mov r11, r0
	cmp r3, #0
	bne .L_0200a9e4
	b .L_0200ab3c
.L_0200a9e4:
	movs r2, #0
	str r2, [sp, #0]
.L_0200a9e8:
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r3, r11
	ldr r3, [r3, #8]
	lsls r5, r5, #4
	mov r8, r3
	add r8, r5
	lsls r0, r0, #4
	mov r1, r8
	subs r1, r1, r0
	mov r8, r1
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r2, r11
	ldr r3, [r2, #16]
	ldr r2, [r2, #12]
	lsls r5, r5, #4
	lsls r6, r6, #3
	movs r1, #128
	lsls r0, r0, #4
	adds r6, r6, r2
	lsls r1, r1, #11
	adds r3, r3, r5
	subs r3, r3, r0
	adds r6, r6, r1
	movs r0, #234
	adds r0, #255
	mov r1, r8
	adds r2, r6, #0
	bl Func_02004bd8
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200ab1c
	bl Random16Far
	mov r10, r0
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	movs r2, #128
	lsls r2, r2, #8
	adds r5, r5, r0
	ldr r1, .L_0200ab34
	adds r0, r7, #0
	mov r9, r2
	bl Func_02004bd0
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	mov r1, r10
	movs r2, #128
	lsls r2, r2, #10
	lsls r3, r1, #2
	adds r3, r3, r2
	str r3, [r7, #40]
	mov r0, r10
	bl Math_Cosine
	ldr r3, .L_0200ab38
	lsls r6, r6, #3
	mov r8, r3
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	str r0, [r7, #44]
	mov r0, r10
	bl Math_Sine
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	ldr r2, .L_0200ab30
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	str r3, [r7, #72]
	lsrs r5, r5, #2
	ldr r3, [r2, #112]
	add r5, r9
	movs r6, #0
	mov r1, r9
	str r5, [r7, #24]
	str r5, [r7, #28]
	str r1, [r7, #68]
	ldr r5, [r7, #80]
	str r0, [r7, #36]
	str r6, [r7, #52]
	ldr r3, [r3, #80]
	ldrb r0, [r5, #16]
	mov r8, r3
	bl Resource_ResetEntry
	ldrb r3, [r5, #17]
	ldr r1, .L_0200ab30
	movs r2, #1
	orrs r3, r2
	strb r3, [r5, #17]
	ldrh r3, [r1, #12]
	ldr r0, [r5, #40]
	strb r3, [r5, #16]
	bl ResourceMetadata_ClearRecord
	str r6, [r5, #40]
	strb r6, [r5, #27]
	mov r2, r8
	ldrb r3, [r2, #20]
	ldrb r0, [r5, #5]
	strb r3, [r5, #20]
	ldrb r3, [r2, #21]
	strb r3, [r5, #21]
	ldrb r1, [r2, #5]
	movs r2, #63
	adds r3, r2, #0
	lsrs r1, r1, #6
	lsls r1, r1, #6
	ands r3, r0
	orrs r3, r1
	strb r3, [r5, #5]
	mov r1, r8
	ldrb r3, [r1, #7]
	ldrb r1, [r5, #7]
	lsrs r3, r3, #6
	lsls r3, r3, #6
	ands r2, r1
	orrs r2, r3
	strb r2, [r5, #7]
	mov r3, r8
	ldrh r2, [r3, #8]
	ldr r1, .L_0200ab2c
	ldrh r3, [r5, #8]
	lsls r2, r2, #22
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
.L_0200ab1c:
	ldr r1, [sp, #0]
	subs r1, #1
	str r1, [sp, #0]
	cmp r1, #0
	blt .L_0200ab28
	b .L_0200a9e8
.L_0200ab28:
	b .L_0200ab3c
	.2byte 0x0000
.L_0200ab2c:
	.4byte 0xfffffc00
.L_0200ab30:
	.4byte Data_020023c4 + 0x188
.L_0200ab34:
	.4byte Data_020056d0
.L_0200ab38:
	.4byte IwramMulQ16
.L_0200ab3c:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200ab4c,"ax",%progbits
	.global Func_02002b4c
	.thumb_func
Func_02002b4c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	str r0, [sp, #12]
	str r1, [sp, #8]
	str r2, [sp, #4]
	str r3, [sp, #0]
	movs r3, #1
	mov r11, r3
.L_0200ab68:
	movs r0, #70
	adds r0, #255
	ldr r1, [sp, #12]
	ldr r2, [sp, #8]
	ldr r3, [sp, #4]
	bl Func_02004bd8
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200ac0a
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	ldr r3, [sp, #0]
	lsrs r5, r5, #4
	adds r5, r3, r5
	lsrs r0, r0, #4
	movs r3, #128
	subs r5, r5, r0
	lsls r3, r3, #7
	mov r10, r3
	adds r3, r5, #0
	add r3, r10
	mov r9, r3
	bl Random16Far
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #3
	mov r8, r3
	bl Random16Far
	ldr r1, .L_0200ac24
	adds r6, r0, #0
	adds r0, r7, #0
	bl Func_02004bd0
	movs r1, #0
	adds r0, r7, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #160
	lsls r3, r3, #9
	adds r5, r5, r3
	str r5, [r7, #40]
	mov r0, r9
	bl Math_Cosine
	ldr r5, .L_0200ac28
	adds r1, r0, #0
	mov r0, r8
	mov lr, r5
	.2byte 0xf800
	str r0, [r7, #44]
	mov r0, r9
	bl Math_Sine
	adds r1, r0, #0
	mov r0, r8
	mov lr, r5
	.2byte 0xf800
	movs r3, #0
	str r3, [r7, #52]
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	lsrs r6, r6, #1
	str r3, [r7, #72]
	movs r3, #128
	add r6, r10
	lsls r3, r3, #8
	str r0, [r7, #36]
	str r6, [r7, #24]
	str r6, [r7, #28]
	str r3, [r7, #68]
	adds r0, r7, #0
	movs r1, #1
	bl Animation_ApplyChildValues
.L_0200ac0a:
	movs r3, #1
	negs r3, r3
	add r11, r3
	mov r3, r11
	cmp r3, #0
	bge .L_0200ab68
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200ac24:
	.4byte Data_02005714
.L_0200ac28:
	.4byte IwramMulQ16
	.section .text.x0200ac2c,"ax",%progbits
	.global Func_02002c2c
	.thumb_func
Func_02002c2c:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	adds r5, #91
	strb r0, [r5]
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200ac40,"ax",%progbits
	.global Func_02002c40
	.thumb_func
Func_02002c40:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r3, #192
	movs r0, #100
	lsls r3, r3, #18
	adds r0, r0, r5
	ldr r6, [r3, #108]
	movs r1, #0
	ldrsh r3, [r0, r1]
	sub sp, #56
	mov r8, r0
	cmp r3, #0
	beq .L_0200ac68
	b .L_0200aef4
.L_0200ac68:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	beq .L_0200ac82
	movs r3, #1
	orrs r0, r3
.L_0200ac82:
	adds r3, r5, #0
	adds r3, #91
	strb r0, [r3]
	add r7, sp, #44
	ldr r3, [r5, #8]
	movs r0, #128
	str r3, [r7]
	lsls r0, r0, #12
	ldr r3, [r5, #12]
	adds r2, r7, #0
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	str r3, [r7, #8]
	ldrh r1, [r5, #6]
	bl Vector_AddPolarOffsetFar
	ldr r1, [r7]
	ldr r2, [r7, #8]
	movs r0, #0
	bl Func_02004c18
	cmp r0, #7
	bne .L_0200acf8
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #64]
	str r3, [r5, #60]
	str r3, [r5, #56]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r5, #52]
	mov r0, r8
	movs r3, #1
	strh r3, [r0]
	movs r0, #145
	bl Func_02004de0
	movs r0, #160
	lsls r0, r0, #11
	movs r2, #128
	adds r1, r0, #0
	lsls r2, r2, #9
	bl Func_02004c50
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02004c50
	ldr r3, [r5, #104]
	cmp r3, #0
	beq .L_0200acf8
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
.L_0200acf8:
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	ldr r3, [r5, #8]
	ldr r1, [r5, #16]
	asrs r3, r3, #20
	str r3, [sp, #32]
	movs r3, #184
	lsls r3, r3, #1
	ldr r4, [sp, #32]
	asrs r1, r1, #20
	adds r2, r0, r3
	ldr r2, [r2]
	lsls r3, r1, #7
	adds r3, r4, r3
	lsls r3, r3, #2
	adds r2, r2, r3
	str r2, [sp, #28]
	movs r4, #212
	lsls r4, r4, #1
	adds r2, r0, r4
	ldr r2, [r2]
	subs r4, #92
	adds r2, r2, r3
	str r2, [sp, #24]
	movs r2, #164
	lsls r2, r2, #1
	adds r3, r0, r2
	ldr r3, [r3]
	asrs r3, r3, #20
	str r3, [sp, #20]
	adds r3, r0, r4
	adds r4, #52
	ldr r2, [r3]
	adds r3, r0, r4
	ldr r3, [r3]
	adds r4, #4
	asrs r3, r3, #20
	str r3, [sp, #16]
	adds r3, r0, r4
	ldr r3, [r3]
	movs r0, #1
	asrs r3, r3, #20
	adds r3, r1, r3
	subs r3, #2
	asrs r2, r2, #20
	negs r0, r0
	str r3, [sp, #8]
	str r0, [sp, #40]
	subs r3, r1, #1
	adds r1, r1, r2
	subs r1, #1
	mov r8, r3
	mov r11, r1
.L_0200ad64:
	ldr r0, [sp, #32]
	ldr r1, [sp, #16]
	ldr r2, [sp, #20]
	movs r4, #1
	adds r3, r0, r1
	subs r3, #1
	negs r4, r4
	mov r9, r3
	str r4, [sp, #36]
	adds r3, r0, r2
	adds r6, r0, #0
	subs r3, #1
	subs r6, #1
	mov r10, r3
.L_0200ad80:
	ldr r4, [sp, #40]
	ldr r0, [sp, #36]
	lsls r3, r4, #7
	adds r3, r3, r0
	lsls r3, r3, #2
	ldr r1, [sp, #28]
	str r3, [sp, #12]
	adds r2, r3, r1
	ldrb r3, [r2, #2]
	cmp r3, #77
	bne .L_0200ade0
	movs r3, #0
	strb r3, [r2, #2]
	mov r2, r10
	mov r3, r11
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #1
	movs r3, #1
	bl Func_02004c30
	mov r4, r8
	movs r1, #64
	movs r2, #1
	movs r3, #1
	movs r0, #64
	str r4, [sp, #4]
	str r6, [sp, #0]
	bl Func_02004c28
	movs r0, #159
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02004de0
	mov r3, r8
	movs r4, #128
	lsls r4, r4, #12
	lsls r2, r3, #20
	lsls r0, r6, #20
	adds r0, r0, r4
	ldr r1, [r5, #12]
	ldrh r3, [r5, #6]
	adds r2, r2, r4
	bl Func_02002b4c
.L_0200ade0:
	ldr r4, [sp, #12]
	ldr r0, [sp, #24]
	adds r2, r4, r0
	ldrb r3, [r2, #2]
	cmp r3, #77
	bne .L_0200ae34
	movs r3, #0
	strb r3, [r2, #2]
	ldr r2, [sp, #8]
	mov r1, r9
	str r1, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #1
	movs r3, #2
	bl Func_02004c30
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #64
	movs r2, #1
	movs r3, #1
	movs r0, #64
	str r6, [sp, #0]
	bl Func_02004c28
	movs r0, #143
	lsls r0, r0, #2
	bl Func_02004de0
	mov r3, r8
	movs r4, #128
	lsls r4, r4, #12
	lsls r2, r3, #20
	lsls r0, r6, #20
	adds r0, r0, r4
	ldr r1, [r5, #12]
	ldrh r3, [r5, #6]
	adds r2, r2, r4
	bl Func_02002b4c
.L_0200ae34:
	ldr r0, [sp, #36]
	movs r4, #1
	adds r0, #1
	add r9, r4
	adds r6, #1
	add r10, r4
	str r0, [sp, #36]
	cmp r0, #1
	ble .L_0200ad80
	ldr r1, [sp, #8]
	ldr r2, [sp, #40]
	adds r1, #1
	adds r2, #1
	str r1, [sp, #8]
	add r8, r4
	add r11, r4
	str r2, [sp, #40]
	cmp r2, #1
	ble .L_0200ad64
	ldr r3, [r5, #24]
	movs r4, #128
	lsls r4, r4, #9
	cmp r3, r4
	bge .L_0200ae72
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r3, [r5, #28]
	adds r3, r3, r2
	str r3, [r5, #28]
.L_0200ae72:
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r5, #0
	bl Func_02004bf8
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	ldr r3, .L_0200af38
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	ldr r6, .L_0200af3c
	bl Object_GetById
	ldr r1, [r5, #8]
	ldr r3, [r0, #8]
	subs r2, r1, r3
	cmp r2, #0
	blt .L_0200aea8
	movs r1, #160
	lsls r1, r1, #13
	cmp r2, r1
	blt .L_0200aeb2
	b .L_0200af28
.L_0200aea8:
	movs r2, #160
	subs r3, r3, r1
	lsls r2, r2, #13
	cmp r3, r2
	bge .L_0200af28
.L_0200aeb2:
	ldr r3, [r5, #12]
	ldr r2, [r0, #12]
	ldr r4, .L_0200af40
	ldr r1, .L_0200af44
	subs r3, r3, r2
	adds r3, r3, r4
	cmp r3, r1
	bhi .L_0200af28
	ldr r3, [r5, #16]
	ldr r0, [r0, #16]
	subs r2, r3, r0
	cmp r2, #0
	blt .L_0200aed6
	movs r3, #160
	lsls r3, r3, #13
	cmp r2, r3
	blt .L_0200aee0
	b .L_0200af28
.L_0200aed6:
	movs r4, #160
	subs r3, r0, r3
	lsls r4, r4, #13
	cmp r3, r4
	bge .L_0200af28
.L_0200aee0:
	movs r3, #2
	strh r3, [r6, #4]
	ldrh r3, [r6, #10]
	movs r0, #170
	lsls r0, r0, #1
	adds r3, #2
	adds r2, r7, r0
	str r5, [r6, #108]
	strh r3, [r2]
	b .L_0200af28
.L_0200aef4:
	cmp r3, #1
	bne .L_0200af28
	adds r3, r5, #0
	adds r3, #91
	movs r2, #0
	strb r2, [r3]
	ldr r3, [r5, #24]
	cmp r3, #0
	ble .L_0200af1a
	ldr r2, .L_0200af48
	adds r0, r5, #0
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r3, [r5, #28]
	adds r3, r3, r2
	str r3, [r5, #28]
	bl Func_020029c8
	b .L_0200af28
.L_0200af1a:
	str r2, [r5, #16]
	str r2, [r5, #12]
	str r2, [r5, #8]
	str r2, [r5, #44]
	str r2, [r5, #40]
	str r2, [r5, #36]
	str r2, [r5, #108]
.L_0200af28:
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200af38:
	.4byte gPartyState
.L_0200af3c:
	.4byte Data_020023c4 + 0x188
.L_0200af40:
	.4byte 0x0007ffff
.L_0200af44:
	.4byte 0x001ffffe
.L_0200af48:
	.4byte 0xfffff000
	.section .text.x0200af4c,"ax",%progbits
	.global Func_02002f4c
	.thumb_func
Func_02002f4c:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	ldr r6, [sp, #24]
	adds r5, r1, #0
	mov r9, r2
	mov r10, r3
	bl Object_GetById
	mov r8, r0
	adds r0, r5, #0
	bl Object_GetById
	adds r5, r0, #0
	mov r0, r8
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	ldr r1, [r0, #8]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
	adds r0, r5, #0
	adds r1, r6, #0
	bl Func_02004bd0
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r3, r5, #0
	adds r3, #85
	movs r6, #0
	strb r6, [r3]
	adds r0, r5, #0
	movs r1, #1
	bl Func_02004bc0
	adds r3, r5, #0
	mov r2, r8
	adds r3, #100
	str r2, [r5, #104]
	mov r0, r10
	strh r6, [r3]
	adds r3, #2
	strh r0, [r3]
	mov r2, r9
	subs r3, #4
	strb r2, [r3]
	ldr r3, .L_0200afc0
	str r3, [r5, #108]
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_0200afc0:
	.4byte Func_02002c2c
	.section .text.x0200afc4,"ax",%progbits
	.global Func_02002fc4
	.thumb_func
Func_02002fc4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r0
	mov r3, r11
	adds r3, #98
	ldrb r0, [r3]
	sub sp, #12
	bl Object_GetById
	mov r3, r11
	adds r7, r0, #0
	adds r3, #102
	movs r2, #0
	ldrsh r6, [r3, r2]
	ldr r3, [r7, #80]
	mov r2, r11
	mov r9, r3
	ldr r3, [r2, #8]
	mov r5, sp
	str r3, [r5]
	movs r0, #128
	ldr r3, [r2, #12]
	ldr r2, .L_0200b054
	adds r1, r6, #0
	adds r3, r3, r2
	str r3, [r5, #4]
	mov r2, r11
	ldr r3, [r2, #16]
	lsls r0, r0, #14
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Vector_AddPolarOffsetFar
	ldr r3, .L_0200b050
	movs r2, #0
	mov r10, r3
	adds r3, r7, #0
	mov r8, r2
	adds r3, #85
	mov r2, r10
	strh r6, [r7, #6]
	strb r2, [r3]
	adds r0, r7, #0
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	bl Object_SetPositionAndResetMotion
	ldr r3, .L_0200b058
	mov r2, r10
	str r3, [r7, #108]
	adds r3, r7, #0
	adds r3, #90
	strb r2, [r3]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r7, #52]
	ldr r3, .L_0200b05c
	mov r2, r9
	adds r6, r6, r3
	b .L_0200b060
	.2byte 0x0000
.L_0200b050:
	.4byte 0x00000000
.L_0200b054:
	.4byte 0xfff40000
.L_0200b058:
	.4byte Func_02002c40
.L_0200b05c:
	.4byte 0xffffc000
.L_0200b060:
	mov r3, r8
	strh r6, [r2, #18]
	str r3, [r7, #24]
	str r3, [r7, #28]
	adds r3, r7, #0
	mov r2, r8
	adds r3, #100
	strh r2, [r3]
	mov r2, r11
	ldr r3, [r2, #104]
	movs r0, #104
	str r3, [r7, #104]
	bl Func_02004de0
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200b08c,"ax",%progbits
	.global Func_0200308c
	.thumb_func
Func_0200308c:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r7, r5, #0
	adds r7, #100
	movs r3, #0
	ldrsh r6, [r7, r3]
	cmp r6, #0
	bne .L_0200b0c4
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r3, #128
	lsls r3, r3, #13
	adds r0, r0, r3
	movs r3, #128
	lsls r3, r3, #9
	str r0, [r5, #12]
	movs r1, #1
	adds r0, r5, #0
	str r6, [r5, #24]
	str r6, [r5, #28]
	str r3, [r5, #48]
	str r3, [r5, #52]
	bl Func_02004c68
	b .L_0200b12c
.L_0200b0c4:
	cmp r6, #30
	bgt .L_0200b0dc
	cmp r6, #30
	bne .L_0200b12c
	movs r0, #136
	bl Func_02004de0
	ldr r0, [r5, #104]
	movs r1, #2
	bl Func_02004bc0
	b .L_0200b12c
.L_0200b0dc:
	cmp r6, #60
	bgt .L_0200b104
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldrh r3, [r7]
	ldr r0, [r5, #104]
	lsls r3, r3, #16
	asrs r2, r3, #16
	lsrs r3, r3, #31
	adds r2, r2, r3
	ldr r3, .L_0200b128
	asrs r2, r2, #1
	ands r2, r3
	lsls r1, r2, #3
	subs r1, r1, r2
	bl Animation_ApplyChildValues
	b .L_0200b12c
.L_0200b104:
	movs r1, #1
	ldr r0, [r5, #104]
	bl Func_02004bc0
	movs r0, #184
	bl Func_02004de0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r3, #0
	str r0, [r5, #12]
	str r3, [r5, #104]
	movs r0, #0
	b .L_0200b134
	.2byte 0x0000
.L_0200b128:
	.4byte 0x00000001
.L_0200b12c:
	ldrh r3, [r7]
	movs r0, #1
	adds r3, #1
	strh r3, [r7]
.L_0200b134:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200b138,"ax",%progbits
	.global Func_02003138
	.thumb_func
Func_02003138:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r7, r5, #0
	adds r7, #100
	movs r3, #0
	ldrsh r6, [r7, r3]
	cmp r6, #0
	bne .L_0200b182
	movs r0, #136
	bl Func_02004de0
	ldr r0, [r5, #104]
	movs r1, #2
	bl Func_02004bc0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r3, #128
	lsls r3, r3, #13
	adds r0, r0, r3
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #48]
	movs r3, #128
	str r0, [r5, #12]
	lsls r3, r3, #8
	adds r0, r5, #0
	movs r1, #1
	str r6, [r5, #24]
	str r6, [r5, #28]
	str r3, [r5, #52]
	bl Func_02004c68
	b .L_0200b1d0
.L_0200b182:
	cmp r6, #32
	bgt .L_0200b1aa
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldrh r3, [r7]
	ldr r0, [r5, #104]
	lsls r3, r3, #16
	asrs r2, r3, #16
	lsrs r3, r3, #31
	adds r2, r2, r3
	ldr r3, .L_0200b1cc
	asrs r2, r2, #1
	ands r2, r3
	lsls r1, r2, #3
	subs r1, r1, r2
	bl Animation_ApplyChildValues
	b .L_0200b1d0
.L_0200b1aa:
	movs r1, #1
	ldr r0, [r5, #104]
	bl Func_02004bc0
	movs r0, #184
	bl Func_02004de0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r3, #0
	str r0, [r5, #12]
	str r3, [r5, #104]
	movs r0, #0
	b .L_0200b1d8
.L_0200b1cc:
	.4byte 0x00000001
.L_0200b1d0:
	ldrh r3, [r7]
	movs r0, #1
	adds r3, #1
	strh r3, [r7]
.L_0200b1d8:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200b1dc,"ax",%progbits
	.global Func_020031dc
	.thumb_func
Func_020031dc:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_02002fc4
	movs r3, #0
	str r3, [r5, #8]
	str r3, [r5, #12]
	str r3, [r5, #16]
	str r3, [r5, #36]
	str r3, [r5, #40]
	str r3, [r5, #44]
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200b1f8,"ax",%progbits
	.global Func_020031f8
	.thumb_func
Func_020031f8:
	push {r5, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #100
	movs r0, #0
	ldrsh r1, [r2, r0]
	ldrh r3, [r2]
	cmp r1, #0
	beq .L_0200b210
	subs r3, #1
	strh r3, [r2]
	b .L_0200b276
.L_0200b210:
	adds r3, r5, #0
	adds r3, #90
	movs r0, #131
	strb r1, [r3]
	lsls r0, r0, #1
	bl GameFlag_Test
	movs r3, #1
	negs r3, r3
	cmp r0, #0
	bne .L_0200b236
	ldr r3, .L_0200b278
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_0200b27c
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r3, [r1, r3]
.L_0200b236:
	movs r0, #1
	negs r0, r0
	cmp r3, r0
	bne .L_0200b248
	adds r0, r5, #0
	movs r1, #9
	bl Func_02004bc0
	b .L_0200b276
.L_0200b248:
	ldrh r1, [r5, #6]
	movs r2, #128
	subs r3, r3, r1
	lsls r3, r3, #16
	asrs r3, r3, #16
	lsls r2, r2, #5
	cmp r3, r2
	ble .L_0200b25a
	adds r3, r2, #0
.L_0200b25a:
	ldr r2, .L_0200b280
	cmp r3, r2
	bge .L_0200b262
	adds r3, r2, #0
.L_0200b262:
	adds r3, r1, r3
	adds r0, r5, #0
	movs r1, #2
	strh r3, [r5, #6]
	bl Func_02004bc0
	adds r0, r5, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
.L_0200b276:
	pop {r5, pc}
.L_0200b278:
	.4byte gInput
.L_0200b27c:
	.4byte Data_02005758
.L_0200b280:
	.4byte 0xfffff000
	.section .text.x0200b284,"ax",%progbits
	.global Func_02003284
	.thumb_func
Func_02003284:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #162
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200b2b4
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_SetBit
	bl Func_02004d48
	bl Func_02004d98
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
.L_0200b2b4:
	pop {pc}
	.2byte 0x0000
	.section .text.x0200b2b8,"ax",%progbits
	.global Func_020032b8
	.thumb_func
Func_020032b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200b378
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #40
	bl ObjectTable_Get
	adds r7, r0, #0
.L_0200b2d8:
	bl Func_02003284
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_0200b37c
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, r10
	str r3, [sp, #12]
	str r3, [r0]
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3]
	ldr r2, [sp, #16]
	bl Func_02004c10
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_0200b380
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_0200b390
	ldr r3, .L_0200b384
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_0200b388
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_0200b38c
	cmp r3, r2
	bne .L_0200b3bc
	b .L_0200b552
.L_0200b378:
	.4byte gPartyState
.L_0200b37c:
	.4byte 0xfff00000
.L_0200b380:
	.4byte IwramMulQ16
.L_0200b384:
	.4byte gInput
.L_0200b388:
	.4byte Data_02005798
.L_0200b38c:
	.4byte 0xffff0000
.L_0200b390:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_0200b3b8
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_0200b3bc
.L_0200b3b8:
	.4byte 0xffffc000
.L_0200b3bc:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, r10
	ldr r1, [r2]
	ldrb r0, [r3]
	ldr r2, [r2, #8]
	bl Func_02004c10
	mov r11, r0
	cmp r0, #255
	beq .L_0200b43e
	ldr r3, [sp, #4]
	mov r2, r10
	ldrb r0, [r3]
	ldr r1, [r2]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200b43e
	ldr r0, [sp, #12]
	mov r2, r10
	str r0, [r2]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	ldr r1, [sp, #12]
	ldr r3, [sp, #8]
	ldr r2, [r7, #12]
	bl Func_02004bf8
	adds r0, r7, #0
	movs r1, #2
	bl Func_02004bc0
	adds r0, r7, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
	adds r0, r7, #0
	bl Func_02004c00
	ldr r3, .L_0200b560
	str r3, [r7, #108]
	b .L_0200b4e8
.L_0200b43e:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_0200b534
.L_0200b452:
	ldr r2, [sp, #4]
	ldr r1, [r6]
	ldrb r0, [r2]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200b508
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov r10, r0
	mov r8, r2
.L_0200b480:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200b4aa
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200b4aa
	cmp r5, r7
	beq .L_0200b4aa
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02004c78
	cmp r0, #0
	bge .L_0200b508
.L_0200b4aa:
	movs r0, #1
	add r10, r0
	movs r2, #128
	mov r3, r10
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_0200b480
	ldr r0, [r6]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	ldr r3, [r6, #8]
	ldr r1, [r6]
	ldr r2, [r6, #4]
	bl Func_02004bf8
	adds r0, r7, #0
	bl Func_02004c00
	ldr r3, [sp, #24]
	cmp r11, r3
	bne .L_0200b52e
.L_0200b4e8:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Func_02004c10
	mov r11, r0
	cmp r0, #255
	bne .L_0200b452
.L_0200b508:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_02004bf8
	adds r0, r7, #0
	bl Func_02004c00
	movs r0, #2
	bl WaitFrames
	b .L_0200b2d8
.L_0200b52e:
	movs r0, #10
	bl WaitFrames
.L_0200b534:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_02004bc0
.L_0200b552:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200b560:
	.4byte Func_020031f8
	.section .text.x0200b564,"ax",%progbits
	.global Func_02003564
	.thumb_func
Func_02003564:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200b5c4
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #40
	bl ObjectTable_Get
	ldr r2, .L_0200b5c8
	ldr r3, .L_0200b5c0
	adds r7, r0, #0
	strh r3, [r2]
.L_0200b58a:
	bl Func_02003284
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_0200b5cc
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, r10
	str r3, [sp, #12]
	str r3, [r0]
	b .L_0200b5d0
.L_0200b5c0:
	.4byte 0x00000000
.L_0200b5c4:
	.4byte gPartyState
.L_0200b5c8:
	.4byte Data_020068b0
.L_0200b5cc:
	.4byte 0xfff00000
.L_0200b5d0:
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3]
	ldr r2, [sp, #16]
	bl Func_02004c10
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_0200b63c
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_0200b64c
	ldr r3, .L_0200b640
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_0200b644
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_0200b648
	cmp r3, r2
	bne .L_0200b678
	b .L_0200b842
.L_0200b63c:
	.4byte IwramMulQ16
.L_0200b640:
	.4byte gInput
.L_0200b644:
	.4byte Data_02005798
.L_0200b648:
	.4byte 0xffff0000
.L_0200b64c:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_0200b674
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_0200b678
.L_0200b674:
	.4byte 0xffffc000
.L_0200b678:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, r10
	ldr r1, [r2]
	ldrb r0, [r3]
	ldr r2, [r2, #8]
	bl Func_02004c10
	mov r11, r0
	cmp r0, #255
	beq .L_0200b6f2
	ldr r3, [sp, #4]
	mov r2, r10
	ldrb r0, [r3]
	ldr r1, [r2]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200b6f2
	ldr r0, [sp, #12]
	mov r2, r10
	str r0, [r2]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	ldr r1, [sp, #12]
	ldr r2, [r7, #12]
	ldr r3, [sp, #8]
	bl Func_02004bf8
	adds r0, r7, #0
	movs r1, #2
	bl Func_02004bc0
	adds r0, r7, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
	movs r5, #0
	b .L_0200b71a
.L_0200b6f2:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_0200b842
.L_0200b706:
	ldr r3, .L_0200b870
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_0200b712
	b .L_0200b842
.L_0200b712:
	movs r0, #1
	bl WaitFrames
	adds r5, #1
.L_0200b71a:
	cmp r5, #179
	bgt .L_0200b728
	adds r0, r7, #0
	bl Func_02004c70
	cmp r0, #0
	beq .L_0200b706
.L_0200b728:
	ldr r3, .L_0200b874
	str r3, [r7, #108]
	b .L_0200b7f6
.L_0200b72e:
	ldr r2, [sp, #4]
	ldr r1, [r6]
	ldrb r0, [r2]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200b816
	ldr r3, .L_0200b870
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200b842
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov r10, r0
	mov r8, r2
.L_0200b766:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200b790
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200b790
	cmp r5, r7
	beq .L_0200b790
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02004c78
	cmp r0, #0
	bge .L_0200b816
.L_0200b790:
	movs r0, #1
	add r10, r0
	movs r2, #128
	mov r3, r10
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_0200b766
	ldr r0, [r6]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	movs r5, #0
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl Func_02004bf8
	b .L_0200b7ce
.L_0200b7c6:
	movs r0, #1
	bl WaitFrames
	adds r5, #1
.L_0200b7ce:
	cmp r5, #179
	bgt .L_0200b7e6
	adds r0, r7, #0
	bl Func_02004c70
	cmp r0, #0
	bne .L_0200b7e6
	ldr r3, .L_0200b870
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_0200b7c6
.L_0200b7e6:
	ldr r3, .L_0200b870
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200b842
	ldr r3, [sp, #24]
	cmp r11, r3
	bne .L_0200b83c
.L_0200b7f6:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Func_02004c10
	mov r11, r0
	cmp r0, #255
	bne .L_0200b72e
.L_0200b816:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_02004bf8
	adds r0, r7, #0
	bl Func_02004c00
	movs r0, #2
	bl WaitFrames
	b .L_0200b58a
.L_0200b83c:
	movs r0, #10
	bl WaitFrames
.L_0200b842:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_02004bc0
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b870:
	.4byte Data_020068b0
.L_0200b874:
	.4byte Func_020031f8
	.section .text.x0200b878,"ax",%progbits
	.global Func_02003878
	.thumb_func
Func_02003878:
	ldr r3, .L_0200b880
	strh r0, [r3]
	bx lr
	.2byte 0x0000
.L_0200b880:
	.4byte Data_020068b0
	.section .text.x0200b884,"ax",%progbits
	.global Func_02003884
	.thumb_func
Func_02003884:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200b8f0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	sub sp, #32
	bl ObjectTable_Get
	adds r5, r0, #0
	ldrh r3, [r5, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_0200b8ec
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	str r3, [sp, #16]
.L_0200b8b6:
	bl Func_02003284
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #56]
	str r3, [r5, #64]
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	ldr r2, .L_0200b8f4
	ldr r3, [r5, #8]
	movs r1, #128
	lsls r1, r1, #12
	ands r3, r2
	mov r9, r1
	add r6, sp, #20
	add r3, r9
	str r3, [r6]
	mov r8, r3
	b .L_0200b8f8
	.2byte 0x0000
.L_0200b8ec:
	.4byte 0xffffc000
.L_0200b8f0:
	.4byte gPartyState
.L_0200b8f4:
	.4byte 0xfff00000
.L_0200b8f8:
	ldr r3, [r5, #12]
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	ands r3, r2
	adds r7, r3, r1
	mov r2, r8
	str r7, [r6, #8]
	str r2, [sp, #8]
	str r7, [sp, #4]
	movs r3, #34
	adds r3, r3, r5
	ldrb r0, [r3]
	adds r1, r2, #0
	adds r2, r7, #0
	mov r11, r3
	bl Func_02004c10
	str r0, [sp, #12]
	movs r0, #128
	ldr r1, [sp, #16]
	lsls r0, r0, #13
	adds r2, r6, #0
	bl Vector_AddPolarOffsetFar
	mov r1, r11
	ldrb r0, [r1]
	ldr r2, [r6, #8]
	ldr r1, [r6]
	bl Func_02004c10
	mov r10, r0
	cmp r0, #255
	beq .L_0200b98c
	mov r2, r11
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #12]
	subs r0, r0, r3
	cmp r0, r9
	bgt .L_0200b98c
	ldr r3, [sp, #8]
	ldr r2, .L_0200b984
	str r3, [r6]
	ldr r1, [sp, #4]
	movs r3, #128
	lsls r3, r3, #10
	str r1, [r6, #8]
	str r3, [r5, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r5, #52]
	adds r3, r5, #0
	adds r3, #100
	strh r2, [r3]
	adds r0, r5, #0
	movs r1, #2
	bl Func_02004bc0
	adds r0, r5, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
	ldr r3, .L_0200b988
	str r3, [r5, #108]
	b .L_0200ba36
	.2byte 0x0000
.L_0200b984:
	.4byte 0x00000000
.L_0200b988:
	.4byte Func_020031f8
.L_0200b98c:
	add r1, sp, #16
	ldrh r1, [r1]
	movs r3, #0
	mov r2, r8
	strh r1, [r5, #6]
	str r3, [r5, #36]
	str r3, [r5, #44]
	str r2, [r5, #8]
	str r7, [r5, #16]
	b .L_0200ba82
.L_0200b9a0:
	mov r3, r11
	ldrb r0, [r3]
	ldr r1, [r7]
	ldr r2, [r7, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #12]
	movs r1, #128
	subs r0, r0, r3
	lsls r1, r1, #12
	cmp r0, r1
	bgt .L_0200ba56
	ldrh r3, [r5, #32]
	movs r2, #0
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #20]
	movs r3, #89
	adds r3, r3, r6
	mov r9, r2
	mov r8, r3
.L_0200b9ce:
	ldr r3, [r6]
	cmp r3, #0
	beq .L_0200b9f8
	mov r1, r8
	ldrb r2, [r1]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200b9f8
	cmp r6, r5
	beq .L_0200b9f8
	ldrh r3, [r6, #32]
	adds r0, r6, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #20
	bl Func_02004c78
	cmp r0, #0
	bge .L_0200ba56
.L_0200b9f8:
	movs r2, #1
	add r9, r2
	movs r3, #128
	mov r1, r9
	add r8, r3
	adds r6, #128
	cmp r1, #63
	ble .L_0200b9ce
	ldr r2, [r7]
	adds r0, r5, #0
	str r2, [sp, #8]
	ldr r3, [r7, #8]
	str r3, [sp, #4]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r5, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl Func_02004bf8
	adds r0, r5, #0
	bl Func_02004c00
	ldr r1, [sp, #12]
	cmp r10, r1
	bne .L_0200ba7c
.L_0200ba36:
	movs r0, #128
	ldr r1, [sp, #16]
	add r2, sp, #20
	lsls r0, r0, #13
	bl Vector_AddPolarOffsetFar
	mov r2, r11
	add r7, sp, #20
	ldrb r0, [r2]
	ldr r1, [r7]
	ldr r2, [r7, #8]
	bl Func_02004c10
	mov r10, r0
	cmp r0, #255
	bne .L_0200b9a0
.L_0200ba56:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #52]
	ldr r2, [r5, #12]
	adds r0, r5, #0
	ldr r1, [sp, #8]
	ldr r3, [sp, #4]
	bl Func_02004bf8
	adds r0, r5, #0
	bl Func_02004c00
	movs r0, #2
	bl WaitFrames
	b .L_0200b8b6
.L_0200ba7c:
	movs r0, #10
	bl WaitFrames
.L_0200ba82:
	movs r3, #0
	str r3, [r5, #108]
	adds r1, r5, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r5, #52]
	adds r0, r5, #0
	movs r1, #1
	bl Func_02004bc0
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200bab0,"ax",%progbits
	.global Func_02003ab0
	.thumb_func
Func_02003ab0:
	push {r5, lr}
	adds r5, r0, #0
	adds r4, r1, #0
	cmp r5, #0
	beq .L_0200baf4
	adds r3, r5, #0
	adds r3, #84
	ldrb r2, [r3]
	movs r3, #15
	ands r3, r2
	cmp r3, #0
	beq .L_0200baf4
	ldr r1, [r5, #80]
	movs r2, #13
	ldrb r0, [r1, #9]
	movs r3, #3
	negs r2, r2
	ands r4, r3
	adds r3, r2, #0
	lsls r4, r4, #2
	ands r3, r0
	orrs r3, r4
	strb r3, [r1, #9]
	adds r1, #37
	ldrb r3, [r1]
	ands r2, r3
	orrs r2, r4
	strb r2, [r1]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
.L_0200baf4:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200baf8,"ax",%progbits
	.global Func_02003af8
	.thumb_func
Func_02003af8:
	ldr r3, [r0, #8]
	ldr r2, [r0, #68]
	ldr r1, [r0, #80]
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r2, [r0, #72]
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	ldr r2, [r0, #76]
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r0, #16]
	ldr r2, [r0, #48]
	ldr r3, [r0, #24]
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, [r0, #52]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	adds r0, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r0]
	adds r3, r3, r2
	strh r3, [r1, #18]
	bx lr
	.2byte 0x0000
	.section .text.x0200bb30,"ax",%progbits
	.global Func_02003b30
	.thumb_func
Func_02003b30:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_0200bce8
	sub sp, #4
	mov r10, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r1, #0
	ldr r1, [sp, #44]
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	mov r8, r1
	ldr r7, [sp, #48]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #13
	mov r1, r8
	ands r3, r1
	mov r9, r0
	cmp r3, #0
	beq .L_0200bb78
	cmp r7, #0
	beq .L_0200bb78
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_0200bb80
.L_0200bb78:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_0200bb80:
	mov r3, r10
	bl Func_02004bd8
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200bb8e
	b .L_0200bcda
.L_0200bb8e:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_02004bc0
	ldr r2, .L_0200bcec
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02004bd0
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_0200bcf0
	mov r1, r9
	str r3, [r6, #108]
	mov r3, r11
	str r3, [r6, #68]
	ldr r3, [sp, #36]
	adds r0, r6, #0
	str r3, [r6, #72]
	ldr r3, [sp, #40]
	str r3, [r6, #76]
	ldr r3, [r1, #80]
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl Func_02003ab0
	movs r2, #100
	adds r2, r2, r6
	mov r9, r2
	mov r3, r9
	str r5, [r6, #48]
	str r5, [r6, #52]
	strh r5, [r3]
	ldr r3, .L_0200bcf4
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200bcda
	cmp r7, #0
	beq .L_0200bcda
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_0200bc10
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_0200bc10:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200bc30
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Func_02003ab0
.L_0200bc30:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_0200bc44
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_0200bc44:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200bc8a
	ldr r3, .L_0200bcec
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_0200bc72
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200bc84
.L_0200bc72:
	ldr r2, .L_0200bcf4
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_0200bcf4
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200bc84:
	bl __divsi3
	str r0, [r6, #52]
.L_0200bc8a:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200bca6
	adds r0, r6, #0
	movs r1, #1
	bl Func_02004bc0
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02004bd0
.L_0200bca6:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200bcb8
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_0200bcb8:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200bcca
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_0200bcca:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200bcda
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_0200bcda:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200bce8:
	.4byte gPartyState
.L_0200bcec:
	.4byte Data_0200686c
.L_0200bcf0:
	.4byte Func_02003af8
.L_0200bcf4:
	.4byte 0xffff0000
	.section .text.x0200bcf8,"ax",%progbits
	.global Func_02003cf8
	.thumb_func
Func_02003cf8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r4, .L_0200be10
	movs r1, #1
	movs r0, #12
	ldrsh r3, [r4, r0]
	negs r1, r1
	sub sp, #4
	cmp r3, r1
	beq .L_0200be04
	lsls r3, r3, #3
	adds r3, r3, r4
	adds r3, #32
	mov r8, r3
	ldr r3, .L_0200be14
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	str r4, [sp, #0]
	bl Object_GetById
	mov r1, r8
	ldr r3, [r0, #8]
	movs r5, #0
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	ldr r4, [sp, #0]
	cmp r3, r2
	bne .L_0200bd44
	ldr r3, [r0, #16]
	movs r5, #2
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	cmp r3, r2
	beq .L_0200bd4c
.L_0200bd44:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r4, #12]
.L_0200bd4c:
	movs r0, #12
	ldrsh r3, [r4, r0]
	movs r2, #1
	negs r2, r2
	ldr r1, .L_0200be18
	cmp r3, r2
	beq .L_0200be04
	movs r5, #14
	ldrsh r3, [r4, r5]
	cmp r3, #0
	beq .L_0200be04
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	str r4, [sp, #0]
	adds r3, r2, #0
	adds r3, #228
	ldr r0, [r3]
	ldr r5, [r3, #4]
	ldr r3, [r2]
	ands r0, r1
	ands r5, r1
	ldr r6, [r3, #4]
	movs r1, #16
	ldrsh r3, [r4, r1]
	ldr r2, .L_0200be1c
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	lsrs r3, r3, #5
	mov r10, r3
	mov r3, r8
	movs r2, #0
	ldrsh r1, [r3, r2]
	lsls r1, r1, #20
	subs r7, r1, r0
	movs r0, #2
	ldrsh r2, [r3, r0]
	movs r0, #0
	lsls r2, r2, #20
	bl Map_GetTerrainHeight
	mov r2, r8
	movs r1, #2
	ldrsh r3, [r2, r1]
	subs r0, r0, r6
	lsls r3, r3, #20
	subs r3, r3, r5
	subs r3, r3, r6
	subs r2, r3, r0
	asrs r7, r7, #16
	adds r0, r0, r3
	asrs r0, r0, #16
	adds r3, r7, #0
	movs r5, #167
	asrs r2, r2, #16
	adds r1, r0, #0
	adds r3, #15
	lsls r5, r5, #1
	adds r2, #14
	adds r1, #58
	ldr r4, [sp, #0]
	cmp r3, r5
	bhi .L_0200be04
	movs r0, #15
	negs r0, r0
	cmp r2, r0
	blt .L_0200be04
	cmp r2, #239
	bgt .L_0200be04
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r7, r3
	movs r3, #255
	ands r2, r3
	movs r3, #0
	str r3, [r4, #20]
	lsls r3, r7, #16
	orrs r2, r3
	ldr r3, .L_0200be20
	adds r0, r4, #0
	orrs r2, r3
	movs r3, #128
	str r2, [r4, #24]
	lsls r3, r3, #3
	mov r2, r10
	orrs r2, r3
	str r2, [r4, #28]
	adds r0, #20
	bl Func_02004b80
.L_0200be04:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200be10:
	.4byte Data_020068b4
.L_0200be14:
	.4byte gPartyState
.L_0200be18:
	.4byte 0xffff0000
.L_0200be1c:
	.4byte ResourceTableEntries
.L_0200be20:
	.4byte 0x80008800
	.section .text.x0200be24,"ax",%progbits
	.global Func_02003e24
	.thumb_func
Func_02003e24:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	str r0, [sp, #44]
	ldr r0, .L_0200c054
	str r1, [sp, #40]
	mov r8, r0
	movs r1, #32
	add r1, r8
	mov r9, r1
	mov r12, r9
	adds r5, r2, #0
	mov r2, r12
	adds r6, r3, #0
	str r2, [sp, #8]
	ldr r3, .L_0200c058
	movs r1, #4
	ldr r7, [sp, #80]
	mov lr, r3
	.2byte 0xf800
	add r0, sp, #44
	ldrh r0, [r0]
	mov r1, r8
	strh r0, [r1, #4]
	add r1, sp, #40
	ldrh r1, [r1]
	mov r3, r8
	strh r1, [r3]
	strh r5, [r3, #2]
	movs r3, #255
	lsls r3, r3, #8
	mov r5, r8
	mov r0, r8
	adds r3, #255
	mov r1, r8
	strh r6, [r5, #6]
	movs r2, #0
	strh r7, [r0, #8]
	strh r3, [r1, #12]
	mov r3, r8
	strh r2, [r3, #10]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #132
	mov r12, r3
	lsls r2, r2, #1
	mov r1, r12
	add r2, r12
	adds r1, #236
	ldr r0, [r1]
	ldr r3, [r2, #8]
	ldr r5, [r2, #48]
	adds r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #32]
	adds r1, #4
	ldr r3, [r2, #12]
	ldr r2, [r1]
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [sp, #28]
	mov r3, r12
	adds r3, #244
	ldr r3, [r3]
	subs r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #24]
	mov r3, r12
	adds r3, #248
	ldr r3, [r3]
	asrs r0, r0, #20
	subs r3, r3, r2
	asrs r2, r2, #20
	lsls r2, r2, #7
	adds r2, r2, r0
	lsls r2, r2, #2
	asrs r3, r3, #20
	adds r5, r5, r2
	movs r0, #0
	str r3, [sp, #20]
	str r5, [sp, #36]
	str r0, [sp, #12]
	cmp r0, r3
	bge .L_0200bfa8
.L_0200bed8:
	ldr r1, [sp, #12]
	ldr r2, [sp, #36]
	ldr r5, [sp, #24]
	lsls r3, r1, #9
	adds r2, r2, r3
	movs r3, #0
	mov r11, r2
	str r3, [sp, #16]
	cmp r3, r5
	bge .L_0200bf9c
.L_0200beec:
	mov r0, r11
	ldrb r5, [r0, #2]
	cmp r5, #0
	beq .L_0200bf8c
	ldr r1, [sp, #44]
	cmp r5, r1
	bcc .L_0200bf8c
	adds r1, #1
	mov r10, r1
	cmp r5, r10
	bhi .L_0200bf8c
	ldr r2, [sp, #16]
	ldr r3, [sp, #32]
	mov r0, r9
	adds r7, r2, r3
	strh r7, [r0]
	ldr r1, [sp, #12]
	ldr r2, [sp, #28]
	add r0, sp, #40
	ldrh r0, [r0]
	adds r6, r1, r2
	mov r3, r9
	mov r1, r9
	strh r6, [r3, #2]
	strh r0, [r1, #4]
	ldr r1, [sp, #40]
	movs r0, #10
	adds r1, #1
	adds r0, #255
	str r1, [sp, #40]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200bf40
	cmp r5, r10
	bne .L_0200bf7e
	mov r3, r9
	movs r2, #4
	ldrsh r0, [r3, r2]
	bl GameFlag_SetBit
	b .L_0200bf7e
.L_0200bf40:
	mov r1, r9
	movs r5, #4
	ldrsh r0, [r1, r5]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bf7e
	mov r2, r8
	ldrh r4, [r2, #6]
	ldrh r5, [r2, #8]
	movs r3, #8
	ldrsh r1, [r2, r3]
	movs r3, #6
	ldrsh r0, [r2, r3]
	movs r2, #64
	adds r3, r2, #0
	ands r3, r4
	ands r2, r5
	lsls r3, r3, #16
	lsls r2, r2, #16
	asrs r3, r3, #16
	asrs r2, r2, #16
	orrs r7, r3
	orrs r6, r2
	adds r1, #1
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004c20
.L_0200bf7e:
	mov r0, r8
	ldrh r3, [r0, #10]
	mov r1, r8
	adds r3, #1
	strh r3, [r1, #10]
	movs r5, #8
	add r9, r5
.L_0200bf8c:
	ldr r2, [sp, #16]
	ldr r5, [sp, #24]
	adds r2, #1
	movs r3, #4
	str r2, [sp, #16]
	add r11, r3
	cmp r2, r5
	blt .L_0200beec
.L_0200bf9c:
	ldr r0, [sp, #12]
	ldr r1, [sp, #20]
	adds r0, #1
	str r0, [sp, #12]
	cmp r0, r1
	blt .L_0200bed8
.L_0200bfa8:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c000
	ldr r3, .L_0200c05c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r2, #0
	asrs r4, r3, #20
	ldr r3, [r0, #16]
	mov r0, r8
	asrs r1, r3, #20
	ldr r3, [sp, #8]
	mov r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	cmp r2, r3
	bge .L_0200c000
.L_0200bfda:
	mov r0, r9
	movs r5, #0
	ldrsh r3, [r0, r5]
	cmp r3, r4
	bne .L_0200bff0
	movs r5, #2
	ldrsh r3, [r0, r5]
	cmp r3, r1
	bne .L_0200bff0
	mov r0, r8
	strh r2, [r0, #12]
.L_0200bff0:
	movs r3, #8
	mov r0, r8
	add r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	adds r2, #1
	cmp r2, r3
	blt .L_0200bfda
.L_0200c000:
	movs r0, #128
	lsls r0, r0, #1
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	adds r1, r5, #0
	movs r2, #63
.L_0200c00e:
	ldr r3, .L_0200c060
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bge .L_0200c00e
	bl Resource_FindFreeEntry
	mov r1, r8
	strh r0, [r1, #16]
	lsls r0, r0, #16
	movs r1, #128
	adds r2, r5, #0
	lsls r1, r1, #1
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r5, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200c064
	bl Scheduler_AddOrUpdateCallback
	mov r3, r8
	movs r2, #10
	ldrsh r0, [r3, r2]
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c054:
	.4byte Data_020068b4
.L_0200c058:
	.4byte IwramClearWords
.L_0200c05c:
	.4byte gPartyState
.L_0200c060:
	.4byte 0x11111111
.L_0200c064:
	.4byte Func_02003cf8
	.section .text.x0200c068,"ax",%progbits
	.global Func_02004068
	.thumb_func
Func_02004068:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r1, .L_0200c0e8
	movs r2, #133
	mov r8, r1
	lsls r2, r2, #2
	add r8, r2
	mov r3, r8
	ldr r0, [r3]
	bl Object_GetById
	mov r1, r8
	ldr r5, [r0, #8]
	ldr r6, [r0, #16]
	mov r10, r0
	movs r2, #128
	ldr r0, [r1]
	movs r1, #128
	lsls r1, r1, #11
	lsls r2, r2, #10
	bl ObjectMotion_SetSpeedParameters
	asrs r5, r5, #20
	mov r2, r8
	asrs r6, r6, #20
	ldr r0, [r2]
	lsls r1, r5, #4
	lsls r2, r6, #4
	adds r1, #8
	adds r2, #8
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #1
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #12
	lsls r5, r5, #20
	lsls r6, r6, #20
	adds r5, r5, r3
	mov r1, r10
	adds r6, r6, r3
	ldr r2, [r1, #12]
	adds r3, r6, #0
	adds r1, r5, #0
	mov r0, r10
	bl Object_SetPositionAndResetMotion
	movs r0, #4
	bl Battle_WaitMode0
	bl Func_02004d28
	ldr r2, .L_0200c0ec
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200c0e8:
	.4byte gPartyState
.L_0200c0ec:
	.4byte 0xfff80000
	.section .text.x0200c0f0,"ax",%progbits
	.global Func_020040f0
	.thumb_func
Func_020040f0:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r3, .L_0200c15c
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	mov r8, r0
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	mov r2, r8
	ldrh r1, [r2, #6]
	movs r2, #64
	ldr r6, [r0, #8]
	adds r3, r2, #0
	ands r3, r1
	lsls r3, r3, #16
	mov r1, r8
	asrs r3, r3, #16
	asrs r6, r6, #20
	orrs r6, r3
	ldrh r3, [r1, #8]
	ldr r5, [r0, #16]
	ands r2, r3
	lsls r2, r2, #16
	asrs r2, r2, #16
	asrs r5, r5, #20
	orrs r5, r2
	bl Func_02004068
	movs r0, #161
	bl Func_02004de0
	mov r3, r8
	movs r2, #8
	ldrsh r1, [r3, r2]
	movs r2, #6
	ldrsh r0, [r3, r2]
	adds r1, #1
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02004c20
	movs r0, #12
	bl Battle_WaitMode0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_0200c15c:
	.4byte gPartyState
	.section .text.x0200c160,"ax",%progbits
	.global Func_02004160
	.thumb_func
Func_02004160:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_0200c210
	movs r2, #133
	lsls r2, r2, #2
	adds r1, r1, r2
	mov r8, r0
	ldr r0, [r1]
	sub sp, #8
	mov r10, r1
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	movs r2, #64
	asrs r7, r3, #20
	mov r3, r8
	ldrh r1, [r3, #6]
	adds r3, r2, #0
	ands r3, r1
	lsls r3, r3, #16
	mov r1, r8
	asrs r3, r3, #16
	orrs r7, r3
	ldrh r3, [r1, #8]
	ldr r5, [r6, #16]
	ands r2, r3
	lsls r2, r2, #16
	asrs r2, r2, #16
	asrs r5, r5, #20
	orrs r5, r2
	bl Func_02004068
	movs r0, #229
	bl Func_02004de0
	mov r3, r8
	movs r2, #8
	ldrsh r1, [r3, r2]
	movs r2, #6
	ldrsh r0, [r3, r2]
	adds r1, #2
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl Func_02004c20
	movs r0, #12
	bl Battle_WaitMode0
	movs r3, #128
	ldr r2, .L_0200c208
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	mov r3, r10
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r2, #226
	movs r3, #128
	lsls r2, r2, #4
	lsls r3, r3, #19
	adds r2, #255
	adds r3, #74
	strh r2, [r3]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0200c20c
	movs r7, #0
	orrs r3, r2
	strh r3, [r1]
	mov r2, r10
	b .L_0200c214
	.2byte 0x0000
.L_0200c208:
	.4byte 0x00000000
.L_0200c20c:
	.4byte 0x00008000
.L_0200c210:
	.4byte gPartyState
.L_0200c214:
	movs r3, #1
	mov r1, r8
	strh r3, [r1, #14]
	ldr r0, [r2]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl Battle_WaitMode0
.L_0200c228:
	cmp r7, #5
	bne .L_0200c232
	movs r0, #204
	bl Func_02004de0
.L_0200c232:
	ldr r3, [r6, #24]
	ldr r1, .L_0200c290
	ldr r2, .L_0200c294
	adds r3, r3, r1
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	ldr r1, .L_0200c298
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r7, #1
	bl WaitFrames
	cmp r7, #39
	ble .L_0200c228
	ldr r3, .L_0200c29c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #0
	adds r0, #84
	strb r3, [r0]
	mov r1, r8
	strh r3, [r1, #14]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200c290:
	.4byte 0xfffffc00
.L_0200c294:
	.4byte 0xfffffd00
.L_0200c298:
	.4byte 0xffff6667
.L_0200c29c:
	.4byte gPartyState
	.section .text.x0200c2a0,"ax",%progbits
	.global Func_020042a0
	.thumb_func
Func_020042a0:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r5, [r6, #68]
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	adds r0, r5, #0
	adds r3, r3, r7
	movs r1, #18
	str r3, [r6, #16]
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_0200c2d0
	adds r3, #15
.L_0200c2d0:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	ldr r1, [r6, #80]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #18]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200c2f8,"ax",%progbits
	.global Func_020042f8
	.thumb_func
Func_020042f8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200c47c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02004bf0
	movs r0, #1
	bl WaitFrames
	movs r3, #130
	lsls r3, r3, #16
	str r3, [r7, #12]
	movs r3, #128
	lsls r3, r3, #8
	adds r5, r7, #0
	str r3, [r7, #72]
	adds r5, #85
	movs r3, #0
	str r3, [r7, #68]
	strb r3, [r5]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r4, #214
	lsls r4, r4, #1
	movs r2, #128
	adds r3, r3, r4
	lsls r2, r2, #1
	str r2, [r3]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #204
	bl Func_02004de0
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_0200c480
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_0200c392:
	mov r4, r10
	lsls r5, r4, #12
	adds r0, r5, #0
	bl Math_Cosine
	add r6, sp, #16
	movs r3, #0
	str r0, [r6]
	adds r0, r5, #0
	str r3, [r6, #4]
	bl Math_Sine
	ldr r3, [r6]
	str r0, [r6, #8]
	asrs r2, r3, #1
	adds r3, r3, r2
	str r3, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r6]
	adds r3, r3, r0
	lsls r3, r3, #14
	lsrs r3, r3, #16
	adds r2, r2, r3
	ldr r3, .L_0200c484
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_0200c488
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r5, r5, r3
	adds r5, r5, r4
	ldr r4, [r6, #4]
	str r5, [r6, #8]
	ldr r2, [r7, #16]
	ldr r3, [r6]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	str r4, [sp, #0]
	ldr r4, .L_0200c48c
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_02003b30
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200c392
	movs r0, #188
	bl Func_02004de0
	ldr r5, .L_0200c47c
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02004d08
	ldr r0, [r5]
	movs r1, #49
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02004c50
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02004c50
	bl Func_02004c58
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02004d08
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #68]
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
	bl Func_02004cb0
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200c47c:
	.4byte gPartyState
.L_0200c480:
	.4byte Func_020042a0
.L_0200c484:
	.4byte 0xffffa000
.L_0200c488:
	.4byte 0xffffd000
.L_0200c48c:
	.4byte 0x01090001
	.section .text.x0200c490,"ax",%progbits
	.global Func_02004490
	.thumb_func
Func_02004490:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200c538
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_0200c53c
	asrs r3, r3, #20
	mov r8, r3
	ldr r3, [r0, #16]
	adds r5, r6, #0
	asrs r3, r3, #20
	mov r10, r3
	movs r1, #10
	ldrsh r3, [r6, r1]
	movs r7, #0
	adds r5, #32
	ldrh r2, [r6, #10]
	cmp r7, r3
	bge .L_0200c52c
.L_0200c4c4:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_0200c520
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_0200c520
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c4f4
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_020040f0
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_0200c52c
.L_0200c4f4:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_0200c52c
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_02004160
	movs r2, #2
	ldrsh r0, [r6, r2]
	mov r1, r8
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r6, r3]
	mov r1, r10
	adds r0, #8
	bl GameFlag_SetByte
	movs r0, #1
	b .L_0200c52e
.L_0200c520:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_0200c4c4
.L_0200c52c:
	movs r0, #0
.L_0200c52e:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c538:
	.4byte gPartyState
.L_0200c53c:
	.4byte Data_020068b4
	.section .text.x0200c540,"ax",%progbits
	.global Func_02004540
	.thumb_func
Func_02004540:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	ldr r3, .L_0200c5f0
	str r2, [sp, #0]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r9, r0
	ldr r0, [r3]
	mov r11, r1
	bl Object_GetById
	movs r3, #192
	ldr r5, .L_0200c5f4
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r6, r0, #0
	movs r2, #2
	ldrsh r0, [r5, r2]
	mov r10, r3
	bl GameFlag_GetByte
	adds r7, r0, #0
	movs r3, #2
	ldrsh r0, [r5, r3]
	adds r0, #8
	bl GameFlag_GetByte
	mov r8, r0
	cmp r7, #0
	bne .L_0200c58e
	cmp r0, #0
	beq .L_0200c5e2
.L_0200c58e:
	movs r2, #2
	ldrsh r0, [r5, r2]
	movs r1, #0
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r5, r3]
	movs r1, #0
	adds r0, #8
	bl GameFlag_SetByte
	mov r3, r9
	adds r2, r7, r3
	mov r3, r8
	movs r1, #128
	add r3, r11
	lsls r1, r1, #12
	lsls r3, r3, #20
	adds r3, r3, r1
	str r3, [r6, #16]
	movs r3, #230
	lsls r3, r3, #1
	add r3, r10
	lsls r2, r2, #20
	adds r2, r2, r1
	ldr r1, [r3]
	str r2, [r6, #8]
	str r2, [r1, #8]
	ldr r3, [r6, #16]
	str r3, [r1, #16]
	bl Func_02004bf0
	bl Func_020042f8
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r2, [sp, #0]
	str r2, [r3]
.L_0200c5e2:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200c5f0:
	.4byte gPartyState
.L_0200c5f4:
	.4byte Data_020068b4
	.section .text.x0200c5f8,"ax",%progbits
	.global Func_020045f8
	.thumb_func
Func_020045f8:
	push {r5, r6, lr}
	adds r4, r1, #0
	movs r1, #192
	lsls r1, r1, #18
	adds r1, #128
	ldr r6, [r1]
	ldr r1, .L_0200c69c
	adds r5, r0, #0
	str r5, [r1]
	ldr r1, .L_0200c6a0
	str r4, [r1]
	ldr r1, .L_0200c6a4
	str r2, [r1]
	ldr r2, .L_0200c6a8
	str r3, [r2]
	movs r2, #255
	ldrh r3, [r5]
	b .L_0200c642
.L_0200c61c:
	ldrh r0, [r4]
	adds r4, #2
	ldrh r2, [r4]
	adds r4, #2
	ldrh r1, [r5]
	ldrh r3, [r4]
	adds r5, #2
	adds r4, #2
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	movs r2, #160
	lsls r2, r2, #19
	lsls r1, r1, #1
	orrs r3, r0
	adds r1, r1, r2
	strh r3, [r1]
	ldrh r3, [r5]
	movs r2, #255
.L_0200c642:
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_0200c650
	ldrh r3, [r4]
	cmp r3, r2
	bne .L_0200c61c
.L_0200c650:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r0, #160
	lsls r2, r2, #24
	adds r3, #212
	lsls r0, r0, #19
	adds r1, r6, #0
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #224
	lsls r2, r2, #1
	adds r1, r6, r2
	movs r2, #132
	lsls r2, r2, #24
	ldr r0, .L_0200c6ac
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02004d58
	ldr r3, .L_0200c6b0
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #2
	bne .L_0200c69a
	bl Func_020048e8
.L_0200c69a:
	pop {r5, r6, pc}
.L_0200c69c:
	.4byte Data_0200e88c
.L_0200c6a0:
	.4byte Data_02006890
.L_0200c6a4:
	.4byte Data_02006894
.L_0200c6a8:
	.4byte Data_02006880
.L_0200c6ac:
	.4byte 0x05000200
.L_0200c6b0:
	.4byte gPartyState
	.section .text.x0200c6b4,"ax",%progbits
	.global Func_020046b4
	.thumb_func
Func_020046b4:
	push {r5, lr}
	ldr r2, .L_0200c6f8
	ldr r3, .L_0200c6e8
	ldr r5, .L_0200c6fc
	strh r3, [r2]
	ldr r3, .L_0200c700
	ldr r0, [r3]
	bl Func_02004794
	ldr r2, .L_0200c704
	ldr r3, .L_0200c6ec
	strh r0, [r5]
	strh r3, [r2]
	ldr r2, .L_0200c708
	ldr r3, .L_0200c6f0
	movs r1, #144
	strh r3, [r2]
	ldr r2, .L_0200c70c
	ldr r3, .L_0200c6f4
	lsls r1, r1, #3
	strh r3, [r2]
	ldr r0, .L_0200c710
	bl Scheduler_AddOrUpdateCallback
	b .L_0200c714
	.2byte 0x0000
.L_0200c6e8:
	.4byte 0x00000000
.L_0200c6ec:
	.4byte 0x0000000f
.L_0200c6f0:
	.4byte 0x00000010
.L_0200c6f4:
	.4byte 0x00000001
.L_0200c6f8:
	.4byte Data_0200689c
.L_0200c6fc:
	.4byte Data_02006898
.L_0200c700:
	.4byte Data_0200e88c
.L_0200c704:
	.4byte Data_0200e888
.L_0200c708:
	.4byte Data_02006884
.L_0200c70c:
	.4byte Data_0200687c
.L_0200c710:
	.4byte Func_020047b8
.L_0200c714:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200c718,"ax",%progbits
	.global Func_02004718
	.thumb_func
Func_02004718:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #172
	str r2, [r3]
	ldr r0, .L_0200c734
	bl Scheduler_RemoveCallbackFar
	pop {pc}
	.2byte 0x0000
.L_0200c734:
	.4byte Func_020047b8
	.section .text.x0200c738,"ax",%progbits
	.global Func_02004738
	.thumb_func
Func_02004738:
	push {r5, lr}
	ldr r2, .L_0200c774
	ldr r3, .L_0200c768
	ldr r5, .L_0200c778
	strh r3, [r2]
	ldr r3, .L_0200c77c
	ldr r0, [r3]
	bl Func_02004794
	ldr r2, .L_0200c76c
	ldr r3, .L_0200c780
	strh r0, [r5]
	strh r2, [r3]
	ldr r3, .L_0200c784
	movs r1, #144
	strh r2, [r3]
	ldr r2, .L_0200c788
	ldr r3, .L_0200c770
	lsls r1, r1, #3
	strh r3, [r2]
	ldr r0, .L_0200c78c
	bl Scheduler_AddOrUpdateCallback
	b .L_0200c790
.L_0200c768:
	.4byte 0x00000000
.L_0200c76c:
	.4byte 0x00000002
.L_0200c770:
	.4byte 0x00000001
.L_0200c774:
	.4byte Data_0200689c
.L_0200c778:
	.4byte Data_02006898
.L_0200c77c:
	.4byte Data_0200e88c
.L_0200c780:
	.4byte Data_0200e888
.L_0200c784:
	.4byte Data_02006884
.L_0200c788:
	.4byte Data_0200687c
.L_0200c78c:
	.4byte Func_020047b8
.L_0200c790:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200c794,"ax",%progbits
	.global Func_02004794
	.thumb_func
Func_02004794:
	push {lr}
	ldr r1, .L_0200c7ac
	ldrh r3, [r0]
	movs r2, #0
	cmp r3, r1
	beq .L_0200c7b0
.L_0200c7a0:
	adds r0, #2
	ldrh r3, [r0]
	adds r2, #1
	cmp r3, r1
	bne .L_0200c7a0
	b .L_0200c7b0
.L_0200c7ac:
	.4byte 0x0000ffff
.L_0200c7b0:
	subs r2, #1
	adds r0, r2, #0
	pop {pc}
	.2byte 0x0000
	.section .text.x0200c7b8,"ax",%progbits
	.global Func_020047b8
	.thumb_func
Func_020047b8:
	push {r5, r6, r7, lr}
	ldr r1, .L_0200c868
	movs r4, #0
	ldrh r2, [r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0200c7f2
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r2, r0
	strh r3, [r1]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200c7f2
	ldr r0, .L_0200c86c
	movs r4, #1
	ldrh r2, [r0]
	strh r2, [r1]
	movs r1, #128
	lsls r3, r2, #16
	lsls r1, r1, #10
	cmp r3, r1
	bls .L_0200c7f2
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r2, r1
	strh r3, [r0]
.L_0200c7f2:
	cmp r4, #0
	bne .L_0200c7f8
	b .L_0200c8e4
.L_0200c7f8:
	ldr r3, .L_0200c870
	ldr r6, .L_0200c874
	ldr r1, [r3]
	ldrh r3, [r6]
	movs r5, #0
	cmp r5, r3
	bcs .L_0200c846
	ldr r3, .L_0200c878
	ldr r2, .L_0200c87c
	ldr r7, [r3]
	mov lr, r2
	mov r12, r6
.L_0200c810:
	mov r3, lr
	ldrh r2, [r3]
	ldrh r3, [r6]
	movs r0, #160
	muls r3, r2
	adds r3, r3, r5
	lsls r3, r3, #1
	ldrh r3, [r3, r7]
	lsls r0, r0, #19
	lsls r3, r3, #1
	adds r4, r3, r0
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	adds r1, #2
	ldrh r3, [r1]
	adds r1, #2
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	adds r5, #1
	mov r2, r12
	ldrh r3, [r2]
	cmp r5, r3
	bcc .L_0200c810
.L_0200c846:
	ldr r3, .L_0200c874
	movs r0, #160
	ldrh r1, [r3]
	ldr r3, .L_0200c880
	lsls r2, r1, #1
	ldr r3, [r3]
	lsls r0, r0, #19
	ldrh r3, [r2, r3]
	adds r2, r2, r1
	lsls r3, r3, #1
	adds r4, r3, r0
	ldr r3, .L_0200c884
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_0200c88c
	ldr r3, .L_0200c888
	b .L_0200c88e
.L_0200c868:
	.4byte Data_02006884
.L_0200c86c:
	.4byte Data_0200e888
.L_0200c870:
	.4byte Data_02006894
.L_0200c874:
	.4byte Data_02006898
.L_0200c878:
	.4byte Data_02006880
.L_0200c87c:
	.4byte Data_0200689c
.L_0200c880:
	.4byte Data_0200e88c
.L_0200c884:
	.4byte Data_0200687c
.L_0200c888:
	.4byte Data_02006890
.L_0200c88c:
	ldr r3, .L_0200c8d4
.L_0200c88e:
	lsls r2, r2, #1
	ldr r3, [r3]
	adds r1, r3, r2
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	ldrh r3, [r1, #2]
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	ldr r1, .L_0200c8d8
	ldr r2, .L_0200c8cc
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
	ldr r1, .L_0200c8dc
	ldr r2, .L_0200c8e0
	ldrh r3, [r1]
	adds r3, #1
	strh r3, [r1]
	lsls r3, r3, #16
	ldrh r2, [r2]
	lsrs r3, r3, #16
	cmp r3, r2
	bne .L_0200c8e4
	ldr r3, .L_0200c8d0
	strh r3, [r1]
	b .L_0200c8e4
	.2byte 0x0000
.L_0200c8cc:
	.4byte 0x00000001
.L_0200c8d0:
	.4byte 0x00000000
.L_0200c8d4:
	.4byte Data_02006894
.L_0200c8d8:
	.4byte Data_0200687c
.L_0200c8dc:
	.4byte Data_0200689c
.L_0200c8e0:
	.4byte Data_02006898
.L_0200c8e4:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200c8e8,"ax",%progbits
	.global Func_020048e8
	.thumb_func
Func_020048e8:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	movs r6, #192
	lsls r6, r6, #18
	ldr r5, [r6, #108]
	movs r1, #214
	lsls r1, r1, #1
	mov r8, r1
	add r5, r8
	ldr r2, [r5]
	ldr r0, .L_0200c968
	movs r1, #1
	mov r10, r2
	bl Func_02004d58
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #2
	bl Func_02004d58
	ldr r2, [r6, #108]
	movs r3, #128
	lsls r3, r3, #1
	mov r1, r8
	str r3, [r2, r1]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02004738
	bl Func_02004dc8
	movs r0, #40
	bl WaitFrames
	bl Func_02004718
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl Func_02004d58
	movs r0, #16
	bl Func_02004d60
	movs r0, #16
	bl WaitFrames
	ldr r3, .L_0200c96c
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	movs r2, #0
	strb r2, [r3]
	mov r3, r10
	str r3, [r5]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
.L_0200c968:
	.4byte 0x00202108
.L_0200c96c:
	.4byte gPartyState
	.section .text.x0200c970,"ax",%progbits
	.global Func_02004970
	.thumb_func
Func_02004970:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	sub sp, #4
	cmp r3, r2
	beq .L_0200c9d4
	adds r7, r0, #0
.L_0200c986:
	ldrh r3, [r7]
	mov r8, r3
	mov r0, r8
	bl Object_GetById
	adds r5, r0, #0
	adds r1, r5, #0
	adds r1, #89
	movs r2, #2
	ldrsh r6, [r7, r2]
	ldrb r2, [r1]
	movs r3, #4
	ldrsh r4, [r7, r3]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r1, #0
	str r4, [sp, #0]
	bl ObjectDispatch_SetSingleChildField26
	ldr r4, [sp, #0]
	lsls r6, r6, #16
	lsls r4, r4, #16
	lsrs r4, r4, #16
	lsrs r6, r6, #16
	adds r5, #34
	ldrb r3, [r5]
	adds r2, r4, #0
	mov r0, r8
	adds r1, r6, #0
	adds r7, #6
	bl Func_02004a60
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200c986
.L_0200c9d4:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x0200c9dc,"ax",%progbits
	.global Func_020049dc
	.thumb_func
Func_020049dc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	adds r5, r0, #0
	mov r9, r1
	mov r10, r2
	movs r1, #255
	ldr r2, [r3]
	b .L_0200ca48
.L_0200c9f8:
	ldrh r3, [r5]
	movs r1, #26
	ldrsh r7, [r2, r1]
	cmp r7, r3
	bne .L_0200ca44
	adds r0, r7, #0
	bl Object_GetById
	adds r5, #2
	ldrh r2, [r5]
	mov r3, r10
	adds r6, r0, #0
	mov r8, r2
	ldrh r5, [r5, #2]
	cmp r3, #7
	bgt .L_0200ca20
	ldr r3, [r6, #28]
	ldr r1, .L_0200ca5c
	adds r3, r3, r1
	str r3, [r6, #28]
.L_0200ca20:
	mov r2, r9
	cmp r2, #1
	bne .L_0200ca52
	adds r0, r5, #0
	bl GameFlag_SetBit
	adds r3, r6, #0
	adds r3, #34
	ldrb r3, [r3]
	adds r0, r7, #0
	mov r1, r8
	adds r2, r5, #0
	bl Func_02004a60
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #28]
	b .L_0200ca52
.L_0200ca44:
	adds r5, #6
	movs r1, #255
.L_0200ca48:
	ldrh r3, [r5]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	bne .L_0200c9f8
.L_0200ca52:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200ca5c:
	.4byte 0xffffe100
	.section .text.x0200ca60,"ax",%progbits
	.global Func_02004a60
	.thumb_func
Func_02004a60:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r3, #0
	mov r8, r2
	adds r6, r1, #0
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	lsls r3, r5, #3
	subs r3, r3, r5
	movs r1, #156
	lsls r1, r1, #1
	lsls r3, r3, #3
	adds r3, r3, r1
	ldr r5, [r2, r3]
	adds r7, r0, #0
	bl Func_02004db8
	lsls r0, r0, #2
	adds r5, r5, r0
	mov r0, r8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200cad4
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #0
	strb r3, [r5, #2]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	cmp r6, #1
	beq .L_0200cac0
	cmp r6, #1
	bcc .L_0200cab6
	cmp r6, #2
	beq .L_0200caca
	b .L_0200cb02
.L_0200cab6:
	adds r0, r7, #0
	movs r1, #2
	bl Func_02004bc0
	b .L_0200cb02
.L_0200cac0:
	adds r0, r7, #0
	movs r1, #4
	bl Func_02004bc0
	b .L_0200cb02
.L_0200caca:
	adds r0, r7, #0
	movs r1, #6
	bl Func_02004bc0
	b .L_0200cb02
.L_0200cad4:
	movs r3, #255
	strb r3, [r5, #2]
	cmp r6, #1
	beq .L_0200caf0
	cmp r6, #1
	bcc .L_0200cae6
	cmp r6, #2
	beq .L_0200cafa
	b .L_0200cb02
.L_0200cae6:
	adds r0, r7, #0
	movs r1, #1
	bl Func_02004bc0
	b .L_0200cb02
.L_0200caf0:
	adds r0, r7, #0
	movs r1, #3
	bl Func_02004bc0
	b .L_0200cb02
.L_0200cafa:
	adds r0, r7, #0
	movs r1, #5
	bl Func_02004bc0
.L_0200cb02:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .rodata.x0200cde8,"a",%progbits
.L_0200cde8:
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000004
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
.L_0200ce50:
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000004
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
.L_0200ceb8:
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00300000
	.4byte 0x00000016
	.4byte 0x00000016
	.4byte 0x00300000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000004
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x02100000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02100000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
.L_0200cf44:
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00300000
	.4byte 0x00000016
	.4byte 0x00000016
	.4byte 0x00300000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02b00000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x80010000
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02b00000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02b00000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
.L_0200cff0:
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00200000
	.4byte 0x00000016
	.4byte 0x00000016
	.4byte 0x00200000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000011
	.global Data_0200503c
Data_0200503c:
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00200000
	.4byte 0x00000016
	.4byte 0x00000016
	.4byte 0x00200000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000004
	.4byte 0x03280000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x03280000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02d80000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02d80000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02c80000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02c80000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02680000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02680000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02280000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02280000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02080000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02080000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01280000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01280000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00380000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000002e
	.4byte Func_02000070
	.4byte 0x00000011
	.global Data_02005314
Data_02005314:
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000005
	.4byte 0x00380000
	.4byte 0x00000016
	.4byte 0x00000007
	.4byte 0x00800000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00300000
	.4byte 0x00000016
	.4byte 0x00000016
	.4byte 0x00300000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000004
	.4byte 0x00380000
	.4byte 0x00300000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000011
	.global Data_02005394
Data_02005394:
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.global Data_020053d8
Data_020053d8:
	.4byte 0x00070008
	.4byte 0x00090200
	.4byte 0x02010007
	.2byte 0xffff
	.global Data_020053e6
Data_020053e6:
	.2byte 0x0009
	.4byte 0x000a0003
	.4byte 0x000b0003
	.4byte 0x000c0003
	.4byte 0x00000003
	.global Data_020053f8
Data_020053f8:
	.4byte 0x00000008
	.2byte 0x0000
	.global Data_020053fe
Data_020053fe:
	.2byte 0x0010
	.4byte 0x02020007
	.4byte 0x00070011
	.4byte 0xffff0203
	.global Data_0200540c
Data_0200540c:
	.4byte 0x00030008
	.2byte 0x0000
	.global Data_02005412
Data_02005412:
	.2byte 0x0009
	.4byte 0x00000001
	.global Data_02005418
Data_02005418:
	.4byte 0x0007000c
	.4byte 0x000d0204
	.4byte 0x02050007
	.4byte 0x0007000e
	.4byte 0x000f0206
	.4byte 0x02070007
	.4byte 0x00070010
	.4byte 0xffff0208
	.global Data_02005438
Data_02005438:
	.4byte 0x00030009
	.2byte 0x0000
	.global Data_0200543e
Data_0200543e:
	.2byte 0x0008
	.4byte 0x000a0000
	.4byte 0x00000001
	.global Data_02005448
Data_02005448:
	.4byte 0x0043000b
	.2byte 0x0000
	.global Data_0200544e
Data_0200544e:
	.2byte 0x000c
	.4byte 0x00000001
	.global Data_02005454
Data_02005454:
	.4byte 0x0007000d
	.4byte 0x000e0210
	.4byte 0x02110007
	.4byte 0x0007000f
	.4byte 0x00100212
	.4byte 0x02130007
	.4byte 0x00070011
	.4byte 0x00120214
	.4byte 0x02150007
	.4byte 0x0000ffff
	.global Data_0200547c
Data_0200547c:
	.4byte 0x0000002e
	.4byte Func_0200308c
	.4byte 0x00000004
	.4byte 0x00700000
	.4byte 0x00200000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00700000
	.4byte 0x00160000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00800000
	.4byte 0x00160000
	.4byte 0x02d80000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_020031dc
	.4byte 0x00000011
	.global Data_020054cc
Data_020054cc:
	.4byte 0x0000002e
	.4byte Func_0200308c
	.4byte 0x00000004
	.4byte 0x00500000
	.4byte 0x00200000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00500000
	.4byte 0x00160000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00600000
	.4byte 0x00160000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000025
	.4byte Func_02000c68
	.4byte 0x0000002e
	.4byte Func_020031dc
	.4byte 0x00000011
	.global Data_02005528
Data_02005528:
	.4byte 0x0000002e
	.4byte Func_0200308c
	.4byte 0x00000004
	.4byte 0x03680000
	.4byte 0x00200000
	.4byte 0x02400000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x03680000
	.4byte 0x001e0000
	.4byte 0x02400000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x03680000
	.4byte 0x001e0000
	.4byte 0x02480000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_020031dc
	.4byte 0x00000011
	.global Data_02005578
Data_02005578:
	.4byte 0x0000002e
	.4byte Func_02003138
	.4byte 0x00000004
	.4byte 0x00600000
	.4byte 0x00200000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00600000
	.4byte 0x00160000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00700000
	.4byte 0x00160000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000025
	.4byte Func_02000a24
	.4byte 0x0000002e
	.4byte Func_020031dc
	.4byte 0x00000011
	.global Data_020055d4
Data_020055d4:
	.4byte 0x0000002e
	.4byte Func_02003138
	.4byte 0x00000004
	.4byte 0x03580000
	.4byte 0x00200000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x03680000
	.4byte 0x00200000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x03680000
	.4byte 0x00160000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x03680000
	.4byte 0x00160000
	.4byte 0x00980000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000025
	.4byte Func_02000c1c
	.4byte 0x0000002e
	.4byte Func_020031dc
	.4byte 0x00000011
	.global Data_02005644
Data_02005644:
	.4byte 0x0000002e
	.4byte Func_02003138
	.4byte 0x00000004
	.4byte 0x00700000
	.4byte 0x00200000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00700000
	.4byte 0x00160000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00800000
	.4byte 0x00160000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000025
	.4byte Func_02000808
	.4byte 0x0000002e
	.4byte Func_020031dc
	.4byte 0x00000011
	.global Data_020056a0
Data_020056a0:
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_020056d0
Data_020056d0:
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.global Data_02005714
Data_02005714:
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.global Data_02005758
Data_02005758:
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xe000c000
	.4byte 0xc000a000
	.4byte 0x20004000
	.4byte 0x40006000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xffffc000
	.4byte 0xc000ffff
	.4byte 0xffff4000
	.4byte 0x4000ffff
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.global Data_02005798
Data_02005798:
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000c000
	.4byte 0xc0008000
	.4byte 0x00004000
	.4byte 0x40008000
	.4byte 0x0000ffff
	.4byte 0xffff8000
.L_0200d7b8:
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
.L_0200d7f4:
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
.L_0200d830:
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
	.global Data_0200586c
Data_0200586c:
	.4byte 0x008a0089
	.4byte 0x008c008b
	.4byte 0x008e008d
	.4byte 0xffff008f
	.global Data_0200587c
Data_0200587c:
	.4byte 0x00190017
	.4byte 0x00170016
	.4byte 0x00160019
	.4byte 0x00190017
	.4byte 0x00170016
	.4byte 0x00160019
	.4byte 0x00190017
	.4byte 0x00170016
	.4byte 0x00160019
	.4byte 0x00190017
	.4byte 0xffff0016
	.global Data_020058a8
Data_020058a8:
	.4byte 0x001b001e
	.4byte 0x0016001f
	.4byte 0x001f0015
	.4byte 0x00120014
	.4byte 0x000d001f
	.4byte 0x001b000b
	.4byte 0x0008000b
	.4byte 0x00080018
	.4byte 0x00150005
	.4byte 0x001f001f
	.4byte 0xffff001f
	.global Data_020058d4
Data_020058d4:
	.4byte 0x0089008e
	.4byte 0x008b008a
	.4byte 0x008d008c
	.4byte 0x008e008d
	.4byte 0x008a0089
	.4byte 0x008c008b
	.4byte 0x008d008c
	.4byte 0x0089008e
	.4byte 0x008b008a
	.4byte 0x008c008b
	.4byte 0x008e008d
	.4byte 0x008a0089
	.4byte 0x008b008a
	.4byte 0x008d008c
	.4byte 0x0089008e
	.4byte 0x008a0089
	.4byte 0x008c008b
	.4byte 0x008e008d
.L_0200d91c:
	.4byte 0x0000002e
	.4byte Func_02000038
	.4byte 0x00000011
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000c0
	.4byte 0x800000c0
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
	.4byte 0x00000117
	.4byte 0x00154002
	.4byte 0x00202119
	.4byte 0x00000118
	.4byte 0x00101119
	.4byte 0x0020211f
	.4byte 0x00304118
	.4byte 0x00403118
	.4byte 0x0050511a
	.4byte 0x0060611a
	.4byte 0x00708118
	.4byte 0x00807118
	.4byte 0x0090a118
	.4byte 0x00a09118
	.4byte 0x01414119
	.4byte 0x00000119
	.4byte 0x00101118
	.4byte 0x00202117
	.4byte 0x0030311a
	.4byte 0x0040411a
	.4byte 0x0050511b
	.4byte 0x00607119
	.4byte 0x00706119
	.4byte 0x00809119
	.4byte 0x00908119
	.4byte 0x00a0b119
	.4byte 0x00b0a119
	.4byte 0x00c0d119
	.4byte 0x00d0c119
	.4byte 0x00e0f119
	.4byte 0x00f0e119
	.4byte 0x0101011a
	.4byte 0x01112119
	.4byte 0x01211119
	.4byte 0x01403118
	.4byte 0x0000011a
	.4byte 0x0010111b
	.4byte 0x0020211b
	.4byte 0x00303119
	.4byte 0x00404119
	.4byte 0x00505118
	.4byte 0x00606118
	.4byte 0x0070711c
	.4byte 0x0080911a
	.4byte 0x0090811a
	.4byte 0x00a0b11a
	.4byte 0x00b0a11a
	.4byte 0x00e0e11b
	.4byte 0x01010119
	.4byte 0x0000011b
	.4byte 0x0010111a
	.4byte 0x0020211a
	.4byte 0x0030411b
	.4byte 0x0040311b
	.4byte 0x00505119
	.4byte 0x0060711b
	.4byte 0x0070611b
	.4byte 0x0080911b
	.4byte 0x0090811b
	.4byte 0x00a0b11b
	.4byte 0x00b0a11b
	.4byte 0x00c0d11b
	.4byte 0x00d0c11b
	.4byte 0x00e0e11a
	.4byte 0x0000011c
	.4byte 0x0010111d
	.4byte 0x0020311c
	.4byte 0x0030211c
	.4byte 0x0040511c
	.4byte 0x0050411c
	.4byte 0x0060611e
	.4byte 0x0070711a
	.4byte 0x0080811e
	.4byte 0x0090911e
	.4byte 0x00a0b11c
	.4byte 0x00b0a11c
	.4byte 0x00c0d11c
	.4byte 0x00d0c11c
	.4byte 0x00e0f11c
	.4byte 0x00f0e11c
	.4byte 0x0100a11e
	.4byte 0x0000011d
	.4byte 0x0010111c
	.4byte 0x0020311d
	.4byte 0x0030211d
	.4byte 0x0040611d
	.4byte 0x0050511e
	.4byte 0x0060411d
	.4byte 0x0070711e
	.4byte 0x00b0b11e
	.4byte 0x0000011e
	.4byte 0x0010211e
	.4byte 0x0020111e
	.4byte 0x0030411e
	.4byte 0x0040311e
	.4byte 0x0050511d
	.4byte 0x0060611c
	.4byte 0x0070711d
	.4byte 0x0080811c
	.4byte 0x000001ff
.L_0200dae8:
	.4byte 0x00000027
	.4byte 0x00000007
	.4byte 0x00000011
	.global Data_02005af4
Data_02005af4:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005b0c
Data_02005b0c:
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00020000
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005b54
Data_02005b54:
	.4byte 0xffff0196
	.4byte .L_0200dae8
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte .L_0200dae8
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0x032001e9
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0x006900f5
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x0000a000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005bcc
Data_02005bcc:
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte .L_0200cde8
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte .L_0200ce50
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte .L_0200d91c
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte .L_0200d91c
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00024000
	.4byte 0xffff01a2
	.4byte .L_0200d91c
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte .L_0200dae8
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte .L_0200dae8
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00024000
	.4byte 0xffff0123
	.4byte 0x00000007
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte .L_0200d91c
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00024000
	.4byte 0xffff0123
	.4byte 0x00000007
	.4byte 0x01e00000
	.4byte 0x00000000
	.4byte 0x02c00000
	.4byte 0x00024000
	.4byte 0xffff02a1
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019c
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
	.global Data_02005d4c
Data_02005d4c:
	.4byte 0xffff019e
	.4byte .L_0200ceb8
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02100000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte .L_0200d91c
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00024000
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00020000
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00028000
	.4byte 0xffff0196
	.4byte .L_0200dae8
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte .L_0200dae8
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x01024000
	.4byte 0xffff0196
	.4byte .L_0200dae8
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x01024000
	.4byte 0xffff0196
	.4byte .L_0200dae8
	.4byte 0x03a80000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x01024000
	.4byte 0xffff0196
	.4byte .L_0200dae8
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x01024000
	.4byte 0x005400f4
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00004000
	.4byte 0xffff02a1
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff02a1
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019c
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019c
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
	.global Data_02005eb4
Data_02005eb4:
	.4byte 0xffff01a2
	.4byte .L_0200d91c
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte .L_0200d91c
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00024000
	.4byte 0xffff01a2
	.4byte .L_0200d91c
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte .L_0200d91c
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff0123
	.4byte 0x00000007
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x02b00000
	.4byte 0x00024000
	.4byte 0xffff02a1
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019c
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
	.global Data_02005f74
Data_02005f74:
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte .L_0200cf44
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02b00000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte .L_0200d91c
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte .L_0200cff0
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte .L_0200d91c
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte .L_0200dae8
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte .L_0200dae8
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte .L_0200dae8
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte .L_0200dae8
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte .L_0200dae8
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte .L_0200dae8
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff02a1
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019c
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
	.global Data_020060c4
Data_020060c4:
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200610c
Data_0200610c:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006118
Data_02006118:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200613c
Data_0200613c:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000051
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000051
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000051
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00008400
	.4byte 0xffff000b
	.4byte Func_02000044
	.4byte 0x0000c400
	.4byte 0xffff000b
	.4byte Func_02000044
	.4byte 0x50008615
	.4byte 0x02000008
	.4byte Func_0200040c
	.4byte 0x50008615
	.4byte 0x02010009
	.4byte Func_0200040c
	.4byte 0x00009815
	.4byte 0x0a3e000a
	.4byte Func_0200051c
	.4byte 0x00000000
	.4byte 0x0a3e000a
	.4byte Func_02000564
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006250
Data_02006250:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000051
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000051
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000051
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000051
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000051
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000031
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000002
	.4byte 0xffff0058
	.4byte Func_0200263c
	.4byte 0x50008615
	.4byte 0x02020010
	.4byte Func_0200040c
	.4byte 0x50008615
	.4byte 0x02030011
	.4byte Func_0200040c
	.4byte 0x00009c05
	.4byte 0xffff0011
	.4byte Func_02000648
	.4byte 0x00009c05
	.4byte 0xffff0012
	.4byte Func_02000590
	.4byte 0x00008c15
	.4byte 0x0a4f0008
	.4byte Func_020008f4
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_020008f4
	.4byte 0x50009705
	.4byte 0x12300050
	.4byte Func_02000678
	.4byte 0x50009705
	.4byte 0x12310051
	.4byte Func_0200085c
	.4byte 0x10009a15
	.4byte 0xffff000f
	.4byte 0x00000000
	.4byte 0x60009a15
	.4byte 0xffff000d
	.4byte Func_02000710
	.4byte 0x20009a15
	.4byte 0xffff000d
	.4byte Func_02000718
	.4byte 0x00000000
	.4byte 0xffff0040
	.4byte Func_02000930
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020063b8
Data_020063b8:
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
	.4byte 0x00000021
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000021
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000051
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000051
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000021
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte Func_020032b8
	.4byte 0x00000002
	.4byte 0xffff005a
	.4byte Func_02002860
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte Func_02000050
	.4byte 0x50008c15
	.4byte 0xffff000a
	.4byte Func_0200096c
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x00000000
	.4byte 0x50008615
	.4byte 0x0204000c
	.4byte Func_0200040c
	.4byte 0x50008615
	.4byte 0x0205000d
	.4byte Func_0200040c
	.4byte 0x50008615
	.4byte 0x0206000e
	.4byte Func_0200040c
	.4byte 0x50008615
	.4byte 0x0207000f
	.4byte Func_0200040c
	.4byte 0x50008615
	.4byte 0x02080010
	.4byte Func_0200040c
	.4byte 0x00009c05
	.4byte 0xffff000e
	.4byte Func_0200093c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020064fc
Data_020064fc:
	.4byte 0x00000051
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000051
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000051
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000021
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000051
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000051
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000051
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte Func_020032b8
	.4byte 0x10009a15
	.4byte 0xffff0008
	.4byte 0x00000000
	.4byte 0x60009a15
	.4byte 0xffff0009
	.4byte Func_020009c0
	.4byte 0x20009a15
	.4byte 0xffff0009
	.4byte Func_020009e8
	.4byte 0x10009a15
	.4byte 0xffff000a
	.4byte 0x00000000
	.4byte 0x60009a15
	.4byte 0xffff000b
	.4byte Func_02000a74
	.4byte 0x20009a15
	.4byte 0xffff000b
	.4byte Func_02000a7c
	.4byte 0x50009705
	.4byte 0x12320050
	.4byte Func_02000b84
	.4byte 0x00009c05
	.4byte 0xffff000e
	.4byte Func_02000af4
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_02000b24
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02000b54
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006628
Data_02006628:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000051
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000051
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000031
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000051
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000051
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte Func_02000e3c
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte Func_02000e3c
	.4byte 0x00000002
	.4byte 0x02340023
	.4byte Func_02000fa4
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte Func_02000fd4
	.4byte 0x00000002
	.4byte 0xffff005a
	.4byte Func_02002860
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte Func_020029a0
	.4byte 0x50008615
	.4byte 0x0210000d
	.4byte Func_0200040c
	.4byte 0x50008615
	.4byte 0x0211000e
	.4byte Func_0200040c
	.4byte 0x50008615
	.4byte 0x0212000f
	.4byte Func_0200040c
	.4byte 0x50008615
	.4byte 0x02130010
	.4byte Func_0200040c
	.4byte 0x50008615
	.4byte 0x02140011
	.4byte Func_0200040c
	.4byte 0x50008615
	.4byte 0x02150012
	.4byte Func_0200040c
	.4byte 0x00009c05
	.4byte 0xffff000c
	.4byte Func_02000c84
	.4byte 0x00009c05
	.4byte 0xffff000d
	.4byte Func_02000cb4
	.4byte 0x00008c15
	.4byte 0x0a4e0008
	.4byte Func_02000ce4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006778
Data_02006778:
	.4byte 0x00000051
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000051
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000051
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte Func_020032b8
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte Func_02001138
	.4byte 0x00000202
	.4byte 0xffff0023
	.4byte Func_0200112c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006868
Data_02006868:
	.4byte 0x00000000
	.global Data_0200686c
Data_0200686c:
	.4byte .L_0200d7b8
	.4byte .L_0200d7f4
	.4byte .L_0200d830
	.section .bss,"aw",%nobits
	.global Data_02006878
Data_02006878:
	.space 0x00000004
	.global Data_0200687c
Data_0200687c:
	.space 0x00000004
	.global Data_02006880
Data_02006880:
	.space 0x00000004
	.global Data_02006884
Data_02006884:
	.space 0x00000004
	.global Data_0200e888
Data_0200e888:
	.space 0x00000004
	.global Data_0200e88c
Data_0200e88c:
	.space 0x00000004
	.global Data_02006890
Data_02006890:
	.space 0x00000004
	.global Data_02006894
Data_02006894:
	.space 0x00000004
	.global Data_02006898
Data_02006898:
	.space 0x00000004
	.global Data_0200689c
Data_0200689c:
	.space 0x00000004
	.global Data_020068a0
Data_020068a0:
	.space 0x00000002
	.global Data_020068a2
Data_020068a2:
	.space 0x00000002
	.global Data_020068a4
Data_020068a4:
	.space 0x0000000c
	.global Data_020068b0
Data_020068b0:
	.space 0x00000004
	.global Data_020068b4
Data_020068b4:
