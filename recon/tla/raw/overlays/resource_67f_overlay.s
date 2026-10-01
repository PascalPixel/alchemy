.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	sub sp, #4
	cmp r3, r2
	beq .L_0200809c
	adds r7, r0, #0
.L_0200804e:
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
	bl Func_02000128
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200804e
.L_0200809c:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x020080a4,"ax",%progbits
	.global Func_020000a4
	.thumb_func
Func_020000a4:
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
	b .L_02008110
.L_020080c0:
	ldrh r3, [r5]
	movs r1, #26
	ldrsh r7, [r2, r1]
	cmp r7, r3
	bne .L_0200810c
	adds r0, r7, #0
	bl Object_GetById
	adds r5, #2
	ldrh r2, [r5]
	mov r3, r10
	adds r6, r0, #0
	mov r8, r2
	ldrh r5, [r5, #2]
	cmp r3, #7
	bgt .L_020080e8
	ldr r3, [r6, #28]
	ldr r1, .L_02008124
	adds r3, r3, r1
	str r3, [r6, #28]
.L_020080e8:
	mov r2, r9
	cmp r2, #1
	bne .L_0200811a
	adds r0, r5, #0
	bl GameFlag_SetBit
	adds r3, r6, #0
	adds r3, #34
	ldrb r3, [r3]
	adds r0, r7, #0
	mov r1, r8
	adds r2, r5, #0
	bl Func_02000128
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #28]
	b .L_0200811a
.L_0200810c:
	adds r5, #6
	movs r1, #255
.L_02008110:
	ldrh r3, [r5]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	bne .L_020080c0
.L_0200811a:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008124:
	.4byte 0xffffe100
	.section .text.x02008128,"ax",%progbits
	.global Func_02000128
	.thumb_func
Func_02000128:
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
	bl Func_02001508
	lsls r0, r0, #2
	adds r5, r5, r0
	mov r0, r8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200819c
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #0
	strb r3, [r5, #2]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	cmp r6, #1
	beq .L_02008188
	cmp r6, #1
	bcc .L_0200817e
	cmp r6, #2
	beq .L_02008192
	b .L_020081ca
.L_0200817e:
	adds r0, r7, #0
	movs r1, #2
	bl Func_02001428
	b .L_020081ca
.L_02008188:
	adds r0, r7, #0
	movs r1, #4
	bl Func_02001428
	b .L_020081ca
.L_02008192:
	adds r0, r7, #0
	movs r1, #6
	bl Func_02001428
	b .L_020081ca
.L_0200819c:
	movs r3, #255
	strb r3, [r5, #2]
	cmp r6, #1
	beq .L_020081b8
	cmp r6, #1
	bcc .L_020081ae
	cmp r6, #2
	beq .L_020081c2
	b .L_020081ca
.L_020081ae:
	adds r0, r7, #0
	movs r1, #1
	bl Func_02001428
	b .L_020081ca
.L_020081b8:
	adds r0, r7, #0
	movs r1, #3
	bl Func_02001428
	b .L_020081ca
.L_020081c2:
	adds r0, r7, #0
	movs r1, #5
	bl Func_02001428
.L_020081ca:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x020081d0,"ax",%progbits
	.global Func_020001d0
	.thumb_func
Func_020001d0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #20
	str r3, [sp, #16]
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	mov r11, r0
	cmp r3, r2
	beq .L_020082b6
.L_020081f6:
	mov r3, r11
	ldrh r3, [r3]
	adds r0, r3, #0
	str r3, [sp, #12]
	bl Object_GetById
	mov r2, r11
	ldrh r2, [r2, #2]
	adds r7, r0, #0
	str r2, [sp, #8]
	movs r3, #34
	adds r3, r3, r7
	adds r0, r2, #0
	ldrb r2, [r3]
	mov r9, r3
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [sp, #16]
	adds r0, #1
	ldr r5, [r2, r3]
	ldr r2, .L_020082ec
	adds r3, r5, r2
	ldr r2, .L_020082f0
	asrs r3, r3, #2
	adds r6, r3, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008244
	ldr r0, [sp, #12]
	movs r1, #0
	movs r2, #0
	bl Func_020014a0
	b .L_020082a4
.L_02008244:
	adds r0, r7, #0
	bl Func_02001508
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r5, r5, r3
	str r5, [sp, #4]
	mov r2, r9
	ldrb r0, [r2]
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	bl Func_02001478
	mov r3, r9
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	mov r10, r0
	ldrb r0, [r3]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	ldr r0, [sp, #12]
	bl Func_020014c0
	ldr r2, [sp, #4]
	movs r3, #128
	asrs r5, r5, #19
	strb r3, [r2, #3]
	adds r5, #4
	mov r3, r9
	adds r2, r5, #0
	ldrb r0, [r3]
	mov r1, r10
	bl Func_02001528
	add r8, r6
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020082a4
	adds r0, r7, #0
	movs r1, #0
	bl Func_02001428
.L_020082a4:
	movs r3, #4
	add r11, r3
	mov r2, r11
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_020081f6
.L_020082b6:
	ldr r3, .L_020082f4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #12]
	cmp r3, r0
	bge .L_020082de
	str r0, [r5, #20]
	str r0, [r5, #12]
.L_020082de:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020082ec:
	.4byte 0xfdff0000
.L_020082f0:
	.4byte Data_02024000
.L_020082f4:
	.4byte gPartyState
	.section .text.x020082f8,"ax",%progbits
	.global Func_020002f8
	.thumb_func
Func_020002f8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #12
	adds r5, r0, #0
	bl Func_020014d8
	cmp r0, #0
	beq .L_0200830e
	b .L_0200847e
.L_0200830e:
	ldr r3, .L_02008488
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	mov r0, sp
	str r3, [r0]
	movs r1, #0
	ldr r3, [r6, #12]
	str r3, [r0, #4]
	ldr r3, [r6, #16]
	str r3, [r0, #8]
	bl Func_02001530
	mov r8, r0
	cmp r0, #0
	bne .L_0200833a
	b .L_0200847e
.L_0200833a:
	b .L_02008470
.L_0200833c:
	ldrh r7, [r5]
	adds r0, r7, #0
	bl Object_GetById
	cmp r0, r8
	beq .L_0200834c
	adds r5, #4
	b .L_02008470
.L_0200834c:
	ldrh r5, [r5, #2]
	bl Func_02001488
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020083ba
	movs r0, #125
	bl Func_02001538
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #2
	bl WaitFrames
	movs r1, #0
	mov r0, r8
	bl Func_02001428
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #2
	bl WaitFrames
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #4
	bl WaitFrames
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #0
	bl Func_02000974
	adds r0, r5, #0
	bl GameFlag_SetBit
	b .L_0200846a
.L_020083ba:
	adds r5, #1
	mov r10, r5
	mov r0, r10
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200846a
	adds r6, #85
	strb r0, [r6]
	movs r0, #185
	bl Func_02001538
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #9
	bl Func_02001470
	movs r0, #0
	bl Func_02000974
	movs r5, #2
	movs r0, #8
	mov r7, r8
	bl WaitFrames
	negs r5, r5
	mov r0, r8
	movs r1, #2
	adds r7, #34
	bl Func_02001428
	ldrb r1, [r7]
	adds r0, r5, #0
	bl Func_02000c4c
	movs r0, #1
	bl Func_02000974
	movs r0, #16
	bl WaitFrames
	ldrb r1, [r7]
	adds r0, r5, #0
	bl Func_02000c4c
	movs r0, #4
	bl WaitFrames
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02001470
	movs r0, #8
	bl WaitFrames
	movs r3, #3
	strb r3, [r6]
	movs r0, #5
	bl WaitFrames
	movs r1, #0
	movs r2, #0
	movs r3, #0
	mov r0, r8
	bl Object_SetPositionAndResetMotion
	movs r0, #2
	bl WaitFrames
	movs r0, #188
	bl Func_02001538
	bl Func_02000aa8
	movs r0, #20
	bl WaitFrames
	mov r0, r10
	bl GameFlag_SetBit
.L_0200846a:
	bl Func_02001490
	b .L_0200847e
.L_02008470:
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_0200847e
	b .L_0200833c
.L_0200847e:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008488:
	.4byte gPartyState
	.section .text.x0200848c,"ax",%progbits
	.global Func_0200048c
	.thumb_func
Func_0200048c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r1, #0
	adds r5, r0, #0
	adds r0, r6, #0
	bl Object_GetById
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
	adds r7, r0, #0
	cmp r3, r2
	beq .L_02008566
.L_020084aa:
	ldrh r3, [r5]
	cmp r3, r6
	beq .L_020084b4
	adds r5, #4
	b .L_0200855a
.L_020084b4:
	ldrh r5, [r5, #2]
	bl Func_02001488
	adds r3, r5, #1
	mov r8, r3
	mov r0, r8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008554
	movs r0, #185
	bl Func_02001538
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #9
	bl Func_02001470
	movs r0, #0
	bl Func_02000974
	movs r0, #8
	bl WaitFrames
	adds r0, r7, #0
	movs r1, #2
	bl Func_02001428
	adds r3, r7, #0
	adds r3, #34
	movs r0, #4
	ldrb r1, [r3]
	adds r2, r6, #0
	negs r0, r0
	bl Func_02000bc0
	movs r0, #1
	bl Func_02000974
	movs r0, #16
	bl WaitFrames
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02001470
	movs r0, #8
	bl WaitFrames
	movs r1, #0
	movs r2, #0
	movs r3, #0
	adds r0, r7, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #2
	bl WaitFrames
	movs r0, #188
	bl Func_02001538
	bl Func_02000aa8
	movs r0, #20
	bl WaitFrames
	adds r0, r5, #0
	bl GameFlag_SetBit
	mov r0, r8
	bl GameFlag_SetBit
.L_02008554:
	bl Func_02001490
	b .L_02008566
.L_0200855a:
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_020084aa
.L_02008566:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x0200856c,"ax",%progbits
	.global Func_0200056c
	.thumb_func
Func_0200056c:
	push {r5, lr}
	adds r5, r0, #0
	bl Object_GetById
	movs r3, #3
	adds r0, #92
	strb r3, [r0]
	adds r0, r5, #0
	bl Func_020014c0
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008584,"ax",%progbits
	.global Func_02000584
	.thumb_func
Func_02000584:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #16
	ldr r5, .L_020086e0
	str r3, [sp, #12]
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	mov r11, r0
	ldr r1, [r5]
	movs r0, #8
	bl Func_020014a8
	ldr r1, [r5]
	movs r0, #9
	bl Func_020014a8
	ldr r1, [r5]
	movs r0, #10
	bl Func_020014a8
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	bl Func_0200056c
	movs r0, #9
	bl Func_0200056c
	movs r0, #10
	bl Func_0200056c
	movs r1, #0
	movs r0, #9
	bl Object_SetModeById
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_020014a0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_020014a0
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_020014a0
	movs r0, #1
	bl WaitFrames
	b .L_020086be
.L_0200860a:
	mov r3, r11
	ldrh r3, [r3]
	mov r9, r3
	mov r0, r9
	bl Object_GetById
	mov r2, r11
	ldrh r2, [r2, #2]
	adds r5, r0, #0
	str r2, [sp, #8]
	adds r7, r5, #0
	adds r7, #34
	adds r0, r2, #0
	ldrb r2, [r7]
	adds r0, #1
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [sp, #12]
	ldr r6, [r2, r3]
	ldr r2, .L_020086e4
	adds r3, r6, r2
	ldr r2, .L_020086e8
	asrs r3, r3, #2
	adds r2, r2, r3
	mov r10, r2
	adds r2, r5, #0
	adds r2, #92
	movs r3, #3
	strb r3, [r2]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008660
	mov r0, r9
	movs r1, #0
	movs r2, #0
	bl Func_020014a0
	b .L_020086ba
.L_02008660:
	adds r0, r5, #0
	bl Func_02001508
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r6, r6, r3
	str r6, [sp, #4]
	add r8, r10
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldrb r0, [r7]
	bl Func_02001478
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	mov r10, r0
	ldrb r0, [r7]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	mov r0, r9
	bl Func_020014c0
	ldr r6, [sp, #4]
	asrs r5, r5, #19
	movs r3, #128
	adds r5, #4
	adds r2, r5, #0
	strb r3, [r6, #3]
	ldrb r0, [r7]
	mov r1, r10
	bl Func_02001528
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086ba
	mov r0, r9
	movs r1, #9
	bl ObjectVisual_CopyAttributes
.L_020086ba:
	movs r3, #4
	add r11, r3
.L_020086be:
	mov r2, r11
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200860a
	movs r0, #10
	bl WaitFrames
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020086e0:
	.4byte gPartyState
.L_020086e4:
	.4byte 0xfdff0000
.L_020086e8:
	.4byte Data_02024000
	.section .text.x020086ec,"ax",%progbits
	.global Func_020006ec
	.thumb_func
Func_020006ec:
	push {r5, lr}
	adds r5, r1, #0
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	bl Object_SetPositionAndResetMotion
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl Object_SetPositionAndResetMotion
	pop {r5, pc}
	.section .text.x02008708,"ax",%progbits
	.global Func_02000708
	.thumb_func
Func_02000708:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #12
	adds r6, r0, #0
	bl Func_020014d8
	cmp r0, #0
	beq .L_02008720
	b .L_020088e2
.L_02008720:
	ldr r3, .L_020088f0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #8
	bl Object_GetById
	ldr r3, [r5, #8]
	adds r7, r0, #0
	mov r0, sp
	str r3, [r0]
	movs r1, #0
	ldr r3, [r5, #12]
	str r3, [r0, #4]
	ldr r3, [r5, #16]
	str r3, [r0, #8]
	bl Func_02001530
	mov r10, r0
	cmp r0, #0
	bne .L_02008754
	b .L_020088e2
.L_02008754:
	b .L_020088d4
.L_02008756:
	ldrh r3, [r6]
	mov r8, r3
	mov r0, r8
	bl Object_GetById
	cmp r0, r10
	beq .L_02008768
	adds r6, #4
	b .L_020088d4
.L_02008768:
	ldrh r6, [r6, #2]
	bl Func_02001488
	adds r0, r6, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008802
	adds r0, r7, #0
	movs r1, #1
	bl Func_02001428
	mov r1, r10
	adds r0, r7, #0
	bl Func_020006ec
	movs r0, #1
	bl WaitFrames
	movs r0, #125
	bl Func_02001538
	movs r0, #8
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #2
	bl WaitFrames
	adds r0, r7, #0
	movs r1, #0
	bl Func_02001428
	movs r1, #9
	mov r0, r8
	bl ObjectVisual_CopyAttributes
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #2
	bl WaitFrames
	movs r0, #8
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #4
	bl WaitFrames
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #0
	bl Func_02000974
	mov r0, r10
	adds r1, r7, #0
	bl Func_020006ec
	movs r0, #1
	bl WaitFrames
	adds r0, r6, #0
	bl GameFlag_SetBit
	b .L_020088ce
.L_02008802:
	adds r6, #1
	mov r9, r6
	mov r0, r9
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020088ce
	adds r0, r7, #0
	movs r1, #0
	bl Func_02001428
	mov r1, r10
	adds r0, r7, #0
	bl Func_020006ec
	adds r5, #85
	movs r0, #1
	bl WaitFrames
	strb r6, [r5]
	movs r0, #185
	bl Func_02001538
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #9
	bl Func_02001470
	movs r0, #0
	bl Func_02000974
	mov r8, r5
	movs r0, #8
	movs r6, #2
	mov r5, r10
	bl WaitFrames
	negs r6, r6
	adds r0, r7, #0
	movs r1, #2
	adds r5, #34
	bl Func_02001428
	ldrb r1, [r5]
	adds r0, r6, #0
	bl Func_02000c4c
	movs r0, #1
	bl Func_02000974
	movs r0, #16
	bl WaitFrames
	ldrb r1, [r5]
	adds r0, r6, #0
	bl Func_02000c4c
	movs r0, #4
	bl WaitFrames
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02001470
	movs r0, #8
	bl WaitFrames
	movs r3, #3
	mov r2, r8
	strb r3, [r2]
	movs r0, #5
	bl WaitFrames
	movs r1, #0
	movs r2, #0
	movs r3, #0
	adds r0, r7, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #2
	bl WaitFrames
	movs r0, #188
	bl Func_02001538
	bl Func_02000aa8
	movs r0, #20
	bl WaitFrames
	mov r0, r9
	bl GameFlag_SetBit
.L_020088ce:
	bl Func_02001490
	b .L_020088e2
.L_020088d4:
	movs r2, #255
	ldrh r3, [r6]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_020088e2
	b .L_02008756
.L_020088e2:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020088f0:
	.4byte gPartyState
	.section .text.x020088f4,"ax",%progbits
	.global Func_020008f4
	.thumb_func
Func_020008f4:
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
	bl Engine_MathDivide
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_02008924
	adds r3, #15
.L_02008924:
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
	ldr r3, .L_02008970
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #80]
	ldr r4, [r6, #80]
	ldrb r3, [r3, #9]
	ldrb r1, [r4, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r4, #9]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008970:
	.4byte gPartyState
	.section .text.x02008974,"ax",%progbits
	.global Func_02000974
	.thumb_func
Func_02000974:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02008a80
	mov r8, r0
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Object_GetById
	movs r2, #0
	adds r7, r0, #0
	mov r9, r2
	mov r10, r2
.L_02008996:
	bl Random16Far
	lsls r3, r0, #3
	subs r3, r3, r0
	ldr r2, [r7, #12]
	lsls r3, r3, #1
	lsrs r3, r3, #16
	lsls r3, r3, #16
	subs r2, r2, r3
	mov r3, r10
	lsls r1, r3, #17
	ldr r3, [r7, #8]
	ldr r0, .L_02008a84
	adds r1, r1, r3
	ldr r3, .L_02008a88
	adds r1, r1, r0
	movs r0, #30
	adds r2, r2, r3
	adds r0, #255
	ldr r3, [r7, #16]
	bl Func_02001438
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02008a6a
	mov r1, r9
	ldr r0, [r6, #80]
	bl Func_020014f0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	adds r2, #4
	strb r3, [r2]
	movs r1, #0
	mov r9, r0
	adds r0, r6, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r6, #0
	movs r1, #2
	bl Func_02001428
	adds r0, r6, #0
	ldr r1, .L_02008a8c
	bl Func_02001430
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	str r3, [r6, #24]
	str r3, [r6, #28]
	ldr r1, [r6, #80]
	movs r0, #13
	ldrb r3, [r1, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	mov r2, r8
	strb r3, [r1, #9]
	cmp r2, #0
	beq .L_02008a34
	mov r3, r10
	lsls r5, r3, #13
	adds r0, r5, #0
	bl Math_Cosine
	ldr r3, .L_02008a90
	ldr r1, .L_02008a94
	mov lr, r3
	.2byte 0xf800
	str r0, [r6, #68]
	adds r0, r5, #0
	bl Math_Sine
	b .L_02008a38
.L_02008a34:
	mov r0, r8
	str r0, [r6, #68]
.L_02008a38:
	str r0, [r6, #76]
	bl Random16Far
	movs r2, #192
	lsls r0, r0, #14
	lsls r2, r2, #7
	lsrs r0, r0, #16
	adds r0, r0, r2
	negs r0, r0
	str r0, [r6, #72]
	bl Random16Far
	ldr r3, .L_02008a98
	lsls r0, r0, #9
	lsrs r0, r0, #16
	adds r0, r0, r3
	adds r3, r6, #0
	adds r3, #100
	strh r0, [r3]
	ldr r3, .L_02008a9c
	str r3, [r6, #48]
	ldr r3, .L_02008aa0
	str r3, [r6, #52]
	ldr r3, .L_02008aa4
	str r3, [r6, #108]
.L_02008a6a:
	movs r0, #1
	add r10, r0
	mov r2, r10
	cmp r2, #7
	bls .L_02008996
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008a80:
	.4byte gPartyState
.L_02008a84:
	.4byte 0xfff80000
.L_02008a88:
	.4byte 0xfffe0000
.L_02008a8c:
	.4byte Data_02001540
.L_02008a90:
	.4byte IwramMulQ16
.L_02008a94:
	.4byte 0x00013333
.L_02008a98:
	.4byte 0xffffff00
.L_02008a9c:
	.4byte 0xfffff800
.L_02008aa0:
	.4byte 0xfffffa00
.L_02008aa4:
	.4byte Func_020008f4
	.section .text.x02008aa8,"ax",%progbits
	.global Func_02000aa8
	.thumb_func
Func_02000aa8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02008ba8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02008b9c
	movs r3, #0
	mov r9, r3
	mov r10, r3
.L_02008acc:
	movs r0, #30
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	adds r0, #255
	bl Func_02001438
	adds r7, r0, #0
	cmp r7, #0
	beq .L_02008b92
	mov r1, r9
	ldr r0, [r7, #80]
	bl Func_020014f0
	movs r4, #0
	mov r8, r4
	adds r3, r7, #0
	mov r2, r8
	adds r3, #85
	strb r2, [r3]
	adds r3, #4
	strb r2, [r3]
	movs r1, #0
	mov r9, r0
	adds r0, r7, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r7, #0
	movs r1, #2
	bl Func_02001428
	ldr r1, .L_02008bac
	adds r0, r7, #0
	bl Func_02001430
	mov r3, r10
	lsls r5, r3, #12
	adds r0, r5, #0
	bl Math_Cosine
	mov r4, r8
	str r4, [r7, #72]
	str r0, [r7, #68]
	adds r0, r5, #0
	bl Math_Sine
	ldr r3, [r7, #68]
	str r0, [r7, #76]
	asrs r2, r3, #1
	adds r3, r3, r2
	str r3, [r7, #68]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r7, #68]
	adds r3, r3, r0
	lsls r3, r3, #14
	lsrs r3, r3, #16
	adds r2, r2, r3
	ldr r3, .L_02008bb0
	adds r2, r2, r3
	str r2, [r7, #68]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r7, #76]
	adds r3, r3, r0
	ldr r4, .L_02008bb4
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r2, r2, r3
	adds r2, r2, r4
	str r2, [r7, #76]
	bl Random16Far
	ldr r2, .L_02008bb8
	lsls r0, r0, #12
	lsrs r0, r0, #16
	adds r3, r7, #0
	adds r0, r0, r2
	adds r3, #100
	strh r0, [r3]
	mov r3, r8
	str r3, [r7, #48]
	str r3, [r7, #52]
	ldr r3, .L_02008bbc
	ldr r0, [r7, #80]
	str r3, [r7, #108]
	ldr r3, [r6, #80]
	movs r1, #12
	ldrb r3, [r3, #9]
	movs r4, #13
	ands r1, r3
	ldrb r3, [r0, #9]
	negs r4, r4
	adds r2, r4, #0
	ands r3, r2
	orrs r3, r1
	strb r3, [r0, #9]
.L_02008b92:
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_02008acc
.L_02008b9c:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008ba8:
	.4byte gPartyState
.L_02008bac:
	.4byte Data_02001570
.L_02008bb0:
	.4byte 0xffffa000
.L_02008bb4:
	.4byte 0xffffd000
.L_02008bb8:
	.4byte 0xfffff800
.L_02008bbc:
	.4byte Func_020008f4
	.section .text.x02008bc0,"ax",%progbits
	.global Func_02000bc0
	.thumb_func
Func_02000bc0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	mov r10, r0
	adds r0, r2, #0
	adds r5, r1, #0
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
	ldr r3, [r2, r3]
	ldr r2, .L_02008c40
	adds r7, r0, #0
	ldr r1, .L_02008c44
	adds r3, r3, r2
	adds r5, r7, #0
	asrs r3, r3, #2
	adds r5, #34
	adds r6, r3, r1
	ldr r2, [r7, #16]
	ldr r1, [r7, #8]
	ldrb r0, [r5]
	bl Func_02001478
	ldr r2, [r7, #16]
	mov r8, r0
	ldr r1, [r7, #8]
	ldrb r0, [r5]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #8]
	asrs r2, r0, #19
	add r2, r10
	cmp r3, #0
	bge .L_02008c1a
	ldr r1, .L_02008c48
	adds r3, r3, r1
.L_02008c1a:
	ldr r0, [r7, #16]
	asrs r1, r3, #20
	cmp r0, #0
	bge .L_02008c26
	ldr r3, .L_02008c48
	adds r0, r0, r3
.L_02008c26:
	asrs r3, r0, #20
	lsls r3, r3, #7
	adds r3, r1, r3
	ldrb r0, [r5]
	mov r1, r8
	adds r6, r6, r3
	bl Func_02001528
	strb r0, [r6]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008c40:
	.4byte 0xfdff0000
.L_02008c44:
	.4byte Data_02024000
.L_02008c48:
	.4byte 0x000fffff
	.section .text.x02008c4c,"ax",%progbits
	.global Func_02000c4c
	.thumb_func
Func_02000c4c:
	push {lr}
	ldr r3, .L_02008c60
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r2, [r3]
	bl Func_02000bc0
	pop {pc}
	.2byte 0x0000
.L_02008c60:
	.4byte gPartyState
	.section .text.x02008caa,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008cac,"ax",%progbits
	.global Func_02000cac
	.thumb_func
Func_02000cac:
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
	.section .text.x02008ce4,"ax",%progbits
	.global Func_02000ce4
	.thumb_func
Func_02000ce4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_02008e9c
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
	beq .L_02008d2c
	cmp r7, #0
	beq .L_02008d2c
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_02008d34
.L_02008d2c:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_02008d34:
	mov r3, r10
	bl Func_02001438
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02008d42
	b .L_02008e8e
.L_02008d42:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_02001428
	ldr r2, .L_02008ea0
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02001430
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_02008ea4
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
	bl Object_SetSpritePriority
	movs r2, #100
	adds r2, r2, r6
	mov r9, r2
	mov r3, r9
	str r5, [r6, #48]
	str r5, [r6, #52]
	strh r5, [r3]
	ldr r3, .L_02008ea8
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02008e8e
	cmp r7, #0
	beq .L_02008e8e
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_02008dc4
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_02008dc4:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02008de4
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriority
.L_02008de4:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_02008df8
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_02008df8:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02008e3e
	ldr r3, .L_02008ea0
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_02008e26
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_02008e38
.L_02008e26:
	ldr r2, .L_02008ea8
	adds r0, r3, r2
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_02008ea8
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_02008e38:
	bl Engine_MathDivide
	str r0, [r6, #52]
.L_02008e3e:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02008e5a
	adds r0, r6, #0
	movs r1, #1
	bl Func_02001428
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02001430
.L_02008e5a:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02008e6c
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_02008e6c:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02008e7e
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_02008e7e:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02008e8e
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_02008e8e:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008e9c:
	.4byte gPartyState
.L_02008ea0:
	.4byte Data_02001668
.L_02008ea4:
	.4byte Func_02000cac
.L_02008ea8:
	.4byte 0xffff0000
	.section .text.x02008eac,"ax",%progbits
	.global Func_02000eac
	.thumb_func
Func_02000eac:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	cmp r4, #0
	bne .L_02008ebc
	movs r0, #0
	b .L_02008ee2
.L_02008ebc:
	cmp r0, #2
	bhi .L_02008ed0
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r3, r3, #3
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r4, [r4, r3]
	b .L_02008ed2
.L_02008ed0:
	ldr r4, .L_02008ee4
.L_02008ed2:
	lsls r3, r2, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r4, r4, r3
	ldrb r0, [r4, #2]
	ldrb r3, [r4, #3]
	lsls r0, r0, #8
	orrs r0, r3
.L_02008ee2:
	pop {pc}
.L_02008ee4:
	.4byte gMapCellBuffer
	.section .text.x02008ee8,"ax",%progbits
	.global Func_02000ee8
	.thumb_func
Func_02000ee8:
	push {r5, lr}
	adds r5, r1, #0
	adds r1, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	cmp r4, #0
	bne .L_02008efc
	movs r0, #0
	b .L_02008f28
.L_02008efc:
	cmp r0, #2
	bhi .L_02008f10
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r3, r3, #3
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r4, [r4, r3]
	b .L_02008f12
.L_02008f10:
	ldr r4, .L_02008f2c
.L_02008f12:
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
.L_02008f28:
	pop {r5, pc}
	.2byte 0x0000
.L_02008f2c:
	.4byte gMapCellBuffer
	.section .text.x02008f30,"ax",%progbits
	.global Func_02000f30
	.thumb_func
Func_02000f30:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	ldr r3, [r6, #8]
	movs r0, #0
	asrs r3, r3, #20
	mov r9, r3
	ldr r3, [r6, #16]
	mov r1, r9
	asrs r3, r3, #20
	mov r10, r3
	mov r2, r10
	bl Func_02000eac
	mov r1, r9
	mov r2, r10
	mov r8, r0
	movs r0, #2
	bl Func_02000eac
	movs r2, #34
	adds r2, r2, r6
	adds r5, r0, #0
	mov r11, r2
	ldrb r0, [r2]
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Map_GetTerrainHeight
	adds r3, r6, #0
	adds r3, #100
	asrs r7, r0, #19
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008faa
	movs r3, #255
	lsls r3, r3, #8
	orrs r5, r3
	movs r3, #129
	negs r3, r3
	mov r2, r8
	ands r2, r3
	ldr r3, [r6, #20]
	mov r8, r2
	asrs r3, r3, #19
	cmp r3, r7
	beq .L_02008fa0
	subs r7, #4
.L_02008fa0:
	adds r0, r6, #0
	movs r1, #3
	bl Object_SetSpritePriority
	b .L_02008fd4
.L_02008faa:
	mov r2, r8
	asrs r3, r2, #8
	cmp r3, #232
	beq .L_02008fb8
	movs r3, #255
	ands r5, r3
	b .L_02008fbe
.L_02008fb8:
	movs r3, #255
	ands r5, r3
	movs r3, #232
.L_02008fbe:
	lsls r3, r3, #8
	orrs r5, r3
	mov r2, r8
	movs r3, #128
	orrs r2, r3
	adds r0, r6, #0
	movs r1, #2
	mov r8, r2
	adds r7, #4
	bl Object_SetSpritePriority
.L_02008fd4:
	mov r1, r9
	mov r2, r10
	mov r3, r8
	movs r0, #0
	bl Func_02000ee8
	mov r1, r9
	mov r2, r10
	adds r3, r5, #0
	movs r0, #2
	bl Func_02000ee8
	mov r3, r9
	mov r2, r10
	lsls r0, r3, #20
	mov r3, r11
	lsls r1, r2, #20
	ldrb r2, [r3]
	adds r3, r7, #0
	bl Func_02001480
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200900c,"ax",%progbits
	.global Func_0200100c
	.thumb_func
Func_0200100c:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	b .L_02009058
.L_02009012:
	ldrh r0, [r7]
	bl Object_GetById
	movs r3, #4
	ldrsh r6, [r7, r3]
	movs r1, #0
	adds r5, r0, #0
	bl ObjectDispatch_SetSingleChildField26
	lsls r0, r6, #16
	lsrs r0, r0, #16
	bl GameFlag_Test
	adds r1, r0, #0
	adds r1, #1
	adds r0, r5, #0
	bl Func_02001428
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r1, r5, #0
	adds r1, #89
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	adds r3, #100
	strh r6, [r3]
	adds r0, r5, #0
	adds r7, #6
	bl Func_02000f30
.L_02009058:
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_02009012
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02009070,"ax",%progbits
	.global Func_02001070
	.thumb_func
Func_02001070:
	push {lr}
	ldr r3, .L_02009094
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02009098
	cmp r2, r3
	bne .L_02009088
	ldr r0, .L_0200909c
	b .L_02009092
.L_02009088:
	ldr r3, .L_020090a0
	movs r0, #0
	cmp r2, r3
	bne .L_02009092
	ldr r0, .L_020090a4
.L_02009092:
	pop {pc}
.L_02009094:
	.4byte gPartyState
.L_02009098:
	.4byte 0x000000ac
.L_0200909c:
	.4byte Data_020016d0
.L_020090a0:
	.4byte 0x000000ad
.L_020090a4:
	.4byte Data_020016f0
	.section .text.x020090b0,"ax",%progbits
	.global Func_020010b0
	.thumb_func
Func_020010b0:
	push {lr}
	ldr r3, .L_020090d8
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020090dc
	cmp r2, r3
	bne .L_020090c8
	ldr r0, .L_020090e0
	b .L_020090d4
.L_020090c8:
	ldr r3, .L_020090e4
	cmp r2, r3
	bne .L_020090d2
	ldr r0, .L_020090e8
	b .L_020090d4
.L_020090d2:
	ldr r0, .L_020090ec
.L_020090d4:
	pop {pc}
	.2byte 0x0000
.L_020090d8:
	.4byte gPartyState
.L_020090dc:
	.4byte 0x000000ac
.L_020090e0:
	.4byte Data_0200176c
.L_020090e4:
	.4byte 0x000000ad
.L_020090e8:
	.4byte Data_02001814
.L_020090ec:
	.4byte Data_02001754
	.section .text.x020090f0,"ax",%progbits
	.global Func_020010f0
	.thumb_func
Func_020010f0:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r6, [r3]
	ldr r3, .L_02009138
	adds r4, r1, #0
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200913c
	adds r5, r0, #0
	cmp r2, r3
	bne .L_0200911c
	ldr r0, .L_02009140
	adds r1, r5, #0
	adds r2, r4, #0
	bl Func_020000a4
	b .L_02009126
.L_0200911c:
	ldr r0, .L_02009144
	adds r1, r5, #0
	adds r2, r4, #0
	bl Func_020000a4
.L_02009126:
	cmp r5, #1
	bne .L_02009136
	movs r2, #26
	ldrsh r0, [r6, r2]
	bl Object_GetById
	bl Func_02000f30
.L_02009136:
	pop {r5, r6, pc}
.L_02009138:
	.4byte gPartyState
.L_0200913c:
	.4byte 0x000000ac
.L_02009140:
	.4byte Data_02001674
.L_02009144:
	.4byte Data_02001688
	.section .text.x02009148,"ax",%progbits
	.global Func_02001148
	.thumb_func
Func_02001148:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	bl Func_02001488
	movs r0, #0
	bl Func_020014e8
	ldr r0, .L_02009180
	bl Func_02001500
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #188
	adds r5, r5, r3
	ldr r1, [r5]
	movs r3, #1
	adds r1, #35
	ldrb r2, [r1]
	orrs r3, r2
	movs r2, #253
	ands r3, r2
	strb r3, [r1]
	bl Func_02001490
	pop {r5, pc}
	.2byte 0x0000
.L_02009180:
	.4byte Data_02001696
	.section .text.x02009184,"ax",%progbits
	.global Func_02001184
	.thumb_func
Func_02001184:
	push {r5, lr}
	ldr r3, .L_020091b4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #2
	bl Func_02000eac
	asrs r0, r0, #8
	cmp r0, #232
	bne .L_020091b2
	adds r2, r5, #0
	adds r2, #34
	movs r3, #2
	strb r3, [r2]
.L_020091b2:
	pop {r5, pc}
.L_020091b4:
	.4byte gPartyState
	.section .text.x020091b8,"ax",%progbits
	.global Func_020011b8
	.thumb_func
Func_020011b8:
	push {lr}
	ldr r3, .L_020091d0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #0
	adds r0, #34
	strb r3, [r0]
	pop {pc}
.L_020091d0:
	.4byte gPartyState
	.section .text.x020091d4,"ax",%progbits
	.global Func_020011d4
	.thumb_func
Func_020011d4:
	push {lr}
	ldr r3, .L_02009218
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200921c
	cmp r2, r3
	bne .L_020091f0
	ldr r0, .L_02009220
	bl Func_02001518
	b .L_02009214
.L_020091f0:
	movs r1, #50
	movs r2, #13
	movs r0, #2
	bl Func_02000eac
	movs r3, #255
	movs r2, #255
	ands r3, r0
	lsls r2, r2, #8
	orrs r3, r2
	movs r0, #2
	movs r1, #50
	movs r2, #13
	bl Func_02000ee8
	ldr r0, .L_02009224
	bl Func_02001518
.L_02009214:
	pop {pc}
	.2byte 0x0000
.L_02009218:
	.4byte gPartyState
.L_0200921c:
	.4byte 0x000000ac
.L_02009220:
	.4byte Data_02001654
.L_02009224:
	.4byte Data_02001658
	.section .text.x02009228,"ax",%progbits
	.global Func_02001228
	.thumb_func
Func_02001228:
	push {lr}
	bl Func_02001488
	movs r0, #0
	bl Func_020014e8
	bl Func_02001520
	ldr r3, .L_02009268
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200926c
	cmp r2, r3
	bne .L_02009262
	movs r1, #50
	movs r2, #13
	movs r0, #2
	bl Func_02000eac
	movs r3, #255
	ands r3, r0
	movs r1, #50
	movs r0, #2
	movs r2, #13
	bl Func_02000ee8
.L_02009262:
	bl Func_02001490
	pop {pc}
.L_02009268:
	.4byte gPartyState
.L_0200926c:
	.4byte 0x000000ad
	.section .text.x02009270,"ax",%progbits
	.global Func_02001270
	.thumb_func
Func_02001270:
	push {lr}
	sub sp, #8
	bl Func_02001488
	movs r2, #12
	str r2, [sp, #4]
	movs r3, #7
	movs r0, #7
	movs r1, #2
	movs r2, #8
	str r3, [sp, #0]
	bl Func_02001460
	movs r3, #8
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #66
	movs r2, #7
	movs r3, #76
	movs r0, #7
	bl Func_02001450
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #65
	bl GameFlag_SetBit
	bl Func_02001490
	add sp, #8
	pop {pc}
	.section .text.x020092b0,"ax",%progbits
	.global Func_020012b0
	.thumb_func
Func_020012b0:
	push {lr}
	ldr r3, .L_020092cc
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020092d0
	cmp r2, r3
	bne .L_020092c8
	ldr r0, .L_020092d4
	b .L_020092ca
.L_020092c8:
	ldr r0, .L_020092d8
.L_020092ca:
	pop {pc}
.L_020092cc:
	.4byte gPartyState
.L_020092d0:
	.4byte 0x000000ac
.L_020092d4:
	.4byte Data_020018ec
.L_020092d8:
	.4byte Data_020019ac
	.section .text.x020092dc,"ax",%progbits
	.global Func_020012dc
	.thumb_func
Func_020012dc:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	ldr r5, .L_020093d0
	movs r2, #129
	adds r3, r3, r1
	lsls r2, r2, #2
	str r2, [r3]
	adds r2, #16
	adds r7, r5, r2
	ldr r0, [r7]
	sub sp, #8
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r6, #32
	orrs r3, r6
	strb r3, [r0]
	movs r0, #0
	bl Func_020014e0
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020093d4
	cmp r2, r3
	bne .L_02009332
	ldr r0, .L_020093d8
	bl Func_02001510
	ldr r0, .L_020093dc
	bl Func_0200100c
	ldr r0, .L_020093e0
	bl Func_020014f8
	b .L_020093ca
.L_02009332:
	movs r0, #10
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	movs r0, #11
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	ldr r0, .L_020093e4
	bl Func_02001510
	ldr r0, .L_020093e8
	bl Func_0200100c
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #65
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020093ca
	movs r2, #12
	str r2, [sp, #4]
	movs r3, #7
	movs r0, #7
	movs r1, #2
	movs r2, #8
	str r3, [sp, #0]
	bl Func_02001460
	movs r3, #8
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #7
	movs r1, #66
	movs r2, #7
	movs r3, #76
	bl Func_02001450
	bl Func_02001448
	movs r0, #1
	bl WaitFrames
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020093ca
	ldr r0, [r7]
	bl Object_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r2, [r5, #16]
	ldr r3, [r5, #12]
	ldr r1, [r5, #8]
	subs r2, r2, r3
	bl Map_GetTerrainHeight
	str r0, [r5, #20]
	str r0, [r5, #12]
	ldr r0, [r7]
	movs r1, #0
	bl Object_AttachWorkTargetToObject
.L_020093ca:
	movs r0, #0
	add sp, #8
	pop {r5, r6, r7, pc}
.L_020093d0:
	.4byte gPartyState
.L_020093d4:
	.4byte 0x000000ac
.L_020093d8:
	.4byte Data_02001654
.L_020093dc:
	.4byte Data_02001674
.L_020093e0:
	.4byte Data_02001696
.L_020093e4:
	.4byte Data_02001658
.L_020093e8:
	.4byte Data_02001688
	.section .rodata.x02009540,"a",%progbits
	.global Data_02001540
Data_02001540:
	.4byte 0x00000000
	.4byte 0x00000021
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
	.global Data_02001570
Data_02001570:
	.4byte 0x00000000
	.4byte 0x0000002c
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
.L_020095a0:
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
.L_020095dc:
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
.L_02009618:
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
	.global Data_02001654
Data_02001654:
	.4byte 0xffff000d
	.global Data_02001658
Data_02001658:
	.4byte 0x000b000a
	.4byte 0x000d000c
	.4byte 0x000f000e
	.4byte 0x0000ffff
	.global Data_02001668
Data_02001668:
	.4byte .L_020095a0
	.4byte .L_020095dc
	.4byte .L_02009618
	.global Data_02001674
Data_02001674:
	.4byte 0x00000008
	.4byte 0x00090200
	.4byte 0x02010000
	.4byte 0x0000000c
	.4byte 0xffff0202
	.global Data_02001688
Data_02001688:
	.4byte 0x00000008
	.4byte 0x00090200
	.4byte 0x02010000
	.2byte 0xffff
	.global Data_02001696
Data_02001696:
	.2byte 0x000a
	.4byte 0x000b0203
	.4byte 0xffff0204
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
	.global Data_020016d0
Data_020016d0:
	.4byte 0x002c00d0
	.4byte 0x00e002f0
	.4byte 0x0300003c
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020016f0
Data_020016f0:
	.4byte 0x00100190
	.4byte 0x01a00160
	.4byte 0x01700020
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x000000ac
	.4byte 0x1010a0ab
	.4byte 0xffffffff
	.4byte 0x102030ac
	.4byte 0xffffffff
	.4byte 0x103020ac
	.4byte 0xffffffff
	.4byte 0x104010ad
	.4byte 0xffffffff
	.4byte 0x000000ad
	.4byte 0x101040ac
	.4byte 0xffffffff
	.4byte 0x102030ad
	.4byte 0xffffffff
	.4byte 0x103020ad
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02001754
Data_02001754:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200176c
Data_0200176c:
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x0002c000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x0002c000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x0002c000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x0002c000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001814
Data_02001814:
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x0002c000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020018ec
Data_020018ec:
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
	.4byte 0x50008615
	.4byte 0x02000008
	.4byte Func_020010f0
	.4byte 0x50008615
	.4byte 0x02010009
	.4byte Func_020010f0
	.4byte 0x50008615
	.4byte 0x0202000c
	.4byte Func_020010f0
	.4byte 0x00001815
	.4byte 0x0203000a
	.4byte Func_02001148
	.4byte 0x00001815
	.4byte 0x0204000b
	.4byte Func_02001148
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte Func_02001184
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte Func_020011b8
	.4byte 0x10008c15
	.4byte 0xffff000d
	.4byte Func_020011d4
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte Func_02001228
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_020011d4
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02001228
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020019ac
Data_020019ac:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x50008615
	.4byte 0x02000008
	.4byte Func_020010f0
	.4byte 0x50008615
	.4byte 0x02010009
	.4byte Func_020010f0
	.4byte 0x50008a05
	.4byte 0xffff0032
	.4byte Func_02001270
	.4byte 0x10008c15
	.4byte 0xffff000a
	.4byte Func_020011d4
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte Func_02001228
	.4byte 0x10008c15
	.4byte 0xffff000b
	.4byte Func_020011d4
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte Func_02001228
	.4byte 0x10008c15
	.4byte 0xffff000c
	.4byte Func_020011d4
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte Func_02001228
	.4byte 0x10008c15
	.4byte 0xffff000d
	.4byte Func_020011d4
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte Func_02001228
	.4byte 0x10008c15
	.4byte 0xffff000e
	.4byte Func_020011d4
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte Func_02001228
	.4byte 0x10008c15
	.4byte 0xffff000f
	.4byte Func_020011d4
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte Func_02001228
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
