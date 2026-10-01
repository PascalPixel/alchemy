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
	bl Func_02003db0
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
	bl Func_02003ba0
	b .L_020081ca
.L_02008188:
	adds r0, r7, #0
	movs r1, #4
	bl Func_02003ba0
	b .L_020081ca
.L_02008192:
	adds r0, r7, #0
	movs r1, #6
	bl Func_02003ba0
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
	bl Func_02003ba0
	b .L_020081ca
.L_020081b8:
	adds r0, r7, #0
	movs r1, #3
	bl Func_02003ba0
	b .L_020081ca
.L_020081c2:
	adds r0, r7, #0
	movs r1, #5
	bl Func_02003ba0
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
	bl Func_02003c68
	b .L_020082a4
.L_02008244:
	adds r0, r7, #0
	bl Func_02003db0
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r5, r5, r3
	str r5, [sp, #4]
	mov r2, r9
	ldrb r0, [r2]
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	bl Func_02003c10
	mov r3, r9
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	mov r10, r0
	ldrb r0, [r3]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	ldr r0, [sp, #12]
	bl Func_02003cc0
	ldr r2, [sp, #4]
	movs r3, #128
	asrs r5, r5, #19
	strb r3, [r2, #3]
	adds r5, #4
	mov r3, r9
	adds r2, r5, #0
	ldrb r0, [r3]
	mov r1, r10
	bl Func_02003dd0
	add r8, r6
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020082a4
	adds r0, r7, #0
	movs r1, #0
	bl Func_02003ba0
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
	bl Func_02003d20
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
	bl Func_02003dd8
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
	bl Func_02003c28
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020083ba
	movs r0, #125
	bl Func_02003df8
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #2
	bl WaitFrames
	movs r1, #0
	mov r0, r8
	bl Func_02003ba0
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
	bl Func_02003df8
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #9
	bl Func_02003c00
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
	bl Func_02003ba0
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
	bl Func_02003c00
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
	bl Func_02003df8
	bl Func_02000aa8
	movs r0, #20
	bl WaitFrames
	mov r0, r10
	bl GameFlag_SetBit
.L_0200846a:
	bl Func_02003c30
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
	bl Func_02003c28
	adds r3, r5, #1
	mov r8, r3
	mov r0, r8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008554
	movs r0, #185
	bl Func_02003df8
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #9
	bl Func_02003c00
	movs r0, #0
	bl Func_02000974
	movs r0, #8
	bl WaitFrames
	adds r0, r7, #0
	movs r1, #2
	bl Func_02003ba0
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
	bl Func_02003c00
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
	bl Func_02003df8
	bl Func_02000aa8
	movs r0, #20
	bl WaitFrames
	adds r0, r5, #0
	bl GameFlag_SetBit
	mov r0, r8
	bl GameFlag_SetBit
.L_02008554:
	bl Func_02003c30
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
	bl Func_02003cc0
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
	bl Func_02003c78
	ldr r1, [r5]
	movs r0, #9
	bl Func_02003c78
	ldr r1, [r5]
	movs r0, #10
	bl Func_02003c78
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
	bl Func_02003c68
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02003c68
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_02003c68
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
	bl Func_02003c68
	b .L_020086ba
.L_02008660:
	adds r0, r5, #0
	bl Func_02003db0
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r6, r6, r3
	str r6, [sp, #4]
	add r8, r10
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldrb r0, [r7]
	bl Func_02003c10
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	mov r10, r0
	ldrb r0, [r7]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	mov r0, r9
	bl Func_02003cc0
	ldr r6, [sp, #4]
	asrs r5, r5, #19
	movs r3, #128
	adds r5, #4
	adds r2, r5, #0
	strb r3, [r6, #3]
	ldrb r0, [r7]
	mov r1, r10
	bl Func_02003dd0
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
	bl Func_02003d20
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
	bl Func_02003dd8
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
	bl Func_02003c28
	adds r0, r6, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008802
	adds r0, r7, #0
	movs r1, #1
	bl Func_02003ba0
	mov r1, r10
	adds r0, r7, #0
	bl Func_020006ec
	movs r0, #1
	bl WaitFrames
	movs r0, #125
	bl Func_02003df8
	movs r0, #8
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #2
	bl WaitFrames
	adds r0, r7, #0
	movs r1, #0
	bl Func_02003ba0
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
	bl Func_02003ba0
	mov r1, r10
	adds r0, r7, #0
	bl Func_020006ec
	adds r5, #85
	movs r0, #1
	bl WaitFrames
	strb r6, [r5]
	movs r0, #185
	bl Func_02003df8
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #9
	bl Func_02003c00
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
	bl Func_02003ba0
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
	bl Func_02003c00
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
	bl Func_02003df8
	bl Func_02000aa8
	movs r0, #20
	bl WaitFrames
	mov r0, r9
	bl GameFlag_SetBit
.L_020088ce:
	bl Func_02003c30
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
	bl Func_02003bb0
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02008a6a
	mov r1, r9
	ldr r0, [r6, #80]
	bl Func_02003d80
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
	bl Func_02003ba0
	adds r0, r6, #0
	ldr r1, .L_02008a8c
	bl Func_02003ba8
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
	.4byte Data_02003e00
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
	bl Func_02003bb0
	adds r7, r0, #0
	cmp r7, #0
	beq .L_02008b92
	mov r1, r9
	ldr r0, [r7, #80]
	bl Func_02003d80
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
	bl Func_02003ba0
	ldr r1, .L_02008bac
	adds r0, r7, #0
	bl Func_02003ba8
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
	.4byte Data_02003e30
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
	bl Func_02003c10
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
	bl Func_02003dd0
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
	.section .text.x02008c64,"ax",%progbits
	.global Func_02000c64
	.thumb_func
Func_02000c64:
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
	sub sp, #8
	str r3, [sp, #4]
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	mov r11, r0
	cmp r3, r2
	beq .L_02008d34
.L_02008c8a:
	mov r3, r11
	ldrh r0, [r3]
	bl Object_GetById
	mov r8, r0
	mov r7, r8
	adds r7, #34
	mov r2, r8
	ldr r1, [r2, #8]
	ldrb r0, [r7]
	ldr r2, [r2, #16]
	bl Func_02003c10
	str r0, [sp, #0]
	mov r3, r8
	ldr r1, [r3, #8]
	ldr r2, [r3, #16]
	ldrb r0, [r7]
	bl Map_GetTerrainHeight
	mov r10, r0
	mov r2, r10
	asrs r2, r2, #19
	mov r0, r8
	mov r10, r2
	bl Func_02003db0
	ldrb r2, [r7]
	mov r9, r0
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [sp, #4]
	mov r0, r8
	ldr r6, [r2, r3]
	ldr r3, .L_02008d6c
	ldr r2, .L_02008d70
	adds r5, r6, r3
	asrs r5, r5, #2
	adds r5, r5, r2
	mov r2, r8
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	adds r2, #4
	strb r3, [r2]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #4
	add r10, r3
	mov r2, r10
	ldrb r0, [r7]
	ldr r1, [sp, #0]
	bl Func_02003dd0
	mov r2, r9
	lsls r2, r2, #2
	add r5, r9
	mov r9, r2
	add r6, r9
	ldrb r3, [r6, #3]
	movs r2, #192
	orrs r3, r2
	strb r3, [r6, #3]
	movs r3, #128
	lsls r3, r3, #2
	adds r6, r6, r3
	ldrb r2, [r6, #3]
	movs r3, #64
	orrs r3, r2
	movs r2, #2
	add r11, r2
	mov r2, r11
	strb r3, [r6, #3]
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	strb r0, [r5]
	cmp r3, r2
	bne .L_02008c8a
.L_02008d34:
	ldr r3, .L_02008d74
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
	bge .L_02008d5c
	str r0, [r5, #20]
	str r0, [r5, #12]
.L_02008d5c:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008d6c:
	.4byte 0xfdff0000
.L_02008d70:
	.4byte Data_02024000
.L_02008d74:
	.4byte gPartyState
	.section .text.x02008d78,"ax",%progbits
	.global Func_02000d78
	.thumb_func
Func_02000d78:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	str r0, [sp, #4]
	bl Func_02003df0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r7, r0, #0
	str r3, [sp, #0]
	cmp r7, #0
	bne .L_02008dba
	ldr r3, .L_02008e90
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	movs r1, #1
	bl Func_02003d18
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	beq .L_02008dba
	bl Object_GetById
	adds r7, r0, #0
.L_02008dba:
	ldr r1, [sp, #0]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #188
	adds r3, r1, r2
	movs r2, #0
	str r2, [r3]
	ldr r1, [sp, #4]
	movs r2, #255
	ldrh r3, [r1]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_02008e80
.L_02008dd6:
	ldr r3, [sp, #4]
	ldrh r0, [r3]
	bl Object_GetById
	cmp r7, r0
	bne .L_02008e6e
	movs r1, #34
	adds r1, r1, r7
	ldrb r0, [r1]
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	lsls r3, r0, #3
	mov r11, r1
	subs r3, r3, r0
	movs r1, #156
	lsls r1, r1, #1
	lsls r3, r3, #3
	adds r3, r3, r1
	ldr r2, [r2, r3]
	ldr r3, .L_02008e94
	mov r10, r2
	ldr r2, .L_02008e98
	ldr r1, [r7, #8]
	add r2, r10
	asrs r2, r2, #2
	mov r8, r2
	ldr r2, [r7, #16]
	add r8, r3
	bl Func_02003c10
	mov r1, r11
	ldr r2, [r7, #16]
	mov r9, r0
	ldrb r0, [r1]
	ldr r1, [r7, #8]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_02003db0
	asrs r5, r5, #19
	mov r2, r11
	subs r5, #4
	adds r6, r0, #0
	mov r1, r9
	ldrb r0, [r2]
	adds r2, r5, #0
	bl Func_02003dd0
	add r8, r6
	lsls r6, r6, #2
	add r10, r6
	mov r1, r10
	ldrb r2, [r1, #3]
	mov r3, r8
	strb r0, [r3]
	movs r3, #63
	ands r3, r2
	movs r2, #128
	strb r3, [r1, #3]
	lsls r2, r2, #2
	add r10, r2
	mov r3, r10
	ldrb r2, [r3, #3]
	movs r3, #191
	ands r3, r2
	mov r1, r10
	strb r3, [r1, #3]
	ldr r2, [sp, #0]
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #188
	adds r3, r2, r1
	str r7, [r3]
.L_02008e6e:
	ldr r2, [sp, #4]
	movs r1, #255
	adds r2, #2
	str r2, [sp, #4]
	lsls r1, r1, #8
	ldrh r3, [r2]
	adds r1, #255
	cmp r3, r1
	bne .L_02008dd6
.L_02008e80:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008e90:
	.4byte gPartyState
.L_02008e94:
	.4byte Data_02024000
.L_02008e98:
	.4byte 0xfdff0000
	.section .text.x02008e9c,"ax",%progbits
	.global Func_02000e9c
	.thumb_func
Func_02000e9c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #188
	adds r3, r3, r1
	ldr r6, [r3]
	cmp r6, #0
	beq .L_02008f32
	adds r7, r6, #0
	adds r7, #34
	ldrb r0, [r7]
	ldr r2, [r2, #32]
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r1, #156
	lsls r1, r1, #1
	lsls r3, r3, #3
	adds r3, r3, r1
	ldr r2, [r2, r3]
	ldr r3, .L_02008f3c
	mov r10, r2
	ldr r2, .L_02008f40
	ldr r1, [r6, #8]
	add r2, r10
	asrs r2, r2, #2
	mov r8, r2
	ldr r2, [r6, #16]
	add r8, r3
	bl Func_02003c10
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	mov r9, r0
	ldrb r0, [r7]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	adds r0, r6, #0
	bl Func_02003db0
	asrs r5, r5, #19
	adds r5, #4
	adds r6, r0, #0
	mov r1, r9
	adds r2, r5, #0
	ldrb r0, [r7]
	bl Func_02003dd0
	add r8, r6
	lsls r6, r6, #2
	add r10, r6
	mov r3, r10
	ldrb r2, [r3, #3]
	mov r1, r8
	movs r3, #192
	orrs r3, r2
	strb r0, [r1]
	movs r2, #128
	mov r1, r10
	strb r3, [r1, #3]
	lsls r2, r2, #2
	add r10, r2
	mov r3, r10
	ldrb r2, [r3, #3]
	movs r3, #64
	orrs r3, r2
	mov r1, r10
	strb r3, [r1, #3]
.L_02008f32:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008f3c:
	.4byte Data_02024000
.L_02008f40:
	.4byte 0xfdff0000
	.section .text.x02008f8a,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008f8c,"ax",%progbits
	.global Func_02000f8c
	.thumb_func
Func_02000f8c:
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
	.section .text.x02008fc4,"ax",%progbits
	.global Func_02000fc4
	.thumb_func
Func_02000fc4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_0200917c
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
	beq .L_0200900c
	cmp r7, #0
	beq .L_0200900c
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_02009014
.L_0200900c:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_02009014:
	mov r3, r10
	bl Func_02003bb0
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02009022
	b .L_0200916e
.L_02009022:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_02003ba0
	ldr r2, .L_02009180
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02003ba8
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_02009184
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
	ldr r3, .L_02009188
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200916e
	cmp r7, #0
	beq .L_0200916e
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_020090a4
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_020090a4:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020090c4
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriority
.L_020090c4:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_020090d8
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_020090d8:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200911e
	ldr r3, .L_02009180
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_02009106
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_02009118
.L_02009106:
	ldr r2, .L_02009188
	adds r0, r3, r2
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_02009188
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_02009118:
	bl Engine_MathDivide
	str r0, [r6, #52]
.L_0200911e:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200913a
	adds r0, r6, #0
	movs r1, #1
	bl Func_02003ba0
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02003ba8
.L_0200913a:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200914c
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_0200914c:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200915e
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_0200915e:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200916e
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_0200916e:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200917c:
	.4byte gPartyState
.L_02009180:
	.4byte Data_02003f38
.L_02009184:
	.4byte Func_02000f8c
.L_02009188:
	.4byte 0xffff0000
	.section .text.x0200918c,"ax",%progbits
	.global Func_0200118c
	.thumb_func
Func_0200118c:
	push {lr}
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #16
	movs r1, #47
	bl Func_02003d08
	pop {pc}
	.2byte 0x0000
	.section .text.x020091b8,"ax",%progbits
	.global Func_020011b8
	.thumb_func
Func_020011b8:
	push {lr}
	ldr r3, .L_02009208
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200920c
	cmp r2, r3
	bne .L_020091d0
	ldr r0, .L_02009210
	b .L_02009204
.L_020091d0:
	ldr r3, .L_02009214
	cmp r2, r3
	bne .L_020091da
	ldr r0, .L_02009218
	b .L_02009204
.L_020091da:
	ldr r3, .L_0200921c
	cmp r2, r3
	bne .L_020091e4
	ldr r0, .L_02009220
	b .L_02009204
.L_020091e4:
	ldr r3, .L_02009224
	cmp r2, r3
	bne .L_020091ee
	ldr r0, .L_02009228
	b .L_02009204
.L_020091ee:
	ldr r3, .L_0200922c
	cmp r2, r3
	bne .L_020091f8
	ldr r0, .L_02009230
	b .L_02009204
.L_020091f8:
	ldr r3, .L_02009234
	cmp r2, r3
	bne .L_02009202
	ldr r0, .L_02009238
	b .L_02009204
.L_02009202:
	ldr r0, .L_0200923c
.L_02009204:
	pop {pc}
	.2byte 0x0000
.L_02009208:
	.4byte gPartyState
.L_0200920c:
	.4byte 0x00000022
.L_02009210:
	.4byte Data_020045f8
.L_02009214:
	.4byte 0x00000023
.L_02009218:
	.4byte Data_020047d8
.L_0200921c:
	.4byte 0x00000024
.L_02009220:
	.4byte Data_02004ac0
.L_02009224:
	.4byte 0x00000025
.L_02009228:
	.4byte Data_02004d78
.L_0200922c:
	.4byte 0x00000026
.L_02009230:
	.4byte Data_02004dd8
.L_02009234:
	.4byte 0x00000027
.L_02009238:
	.4byte Data_02004e20
.L_0200923c:
	.4byte Data_020045e0
	.section .text.x02009240,"ax",%progbits
	.global Func_02001240
	.thumb_func
Func_02001240:
	push {lr}
	ldr r3, .L_02009290
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02009294
	cmp r2, r3
	bne .L_02009258
	ldr r0, .L_02009298
	b .L_0200928c
.L_02009258:
	ldr r3, .L_0200929c
	cmp r2, r3
	bne .L_02009262
	ldr r0, .L_020092a0
	b .L_0200928c
.L_02009262:
	ldr r3, .L_020092a4
	cmp r2, r3
	bne .L_0200926c
	ldr r0, .L_020092a8
	b .L_0200928c
.L_0200926c:
	ldr r3, .L_020092ac
	cmp r2, r3
	bne .L_02009276
	ldr r0, .L_020092b0
	b .L_0200928c
.L_02009276:
	ldr r3, .L_020092b4
	cmp r2, r3
	bne .L_02009280
	ldr r0, .L_020092b8
	b .L_0200928c
.L_02009280:
	ldr r3, .L_020092bc
	cmp r2, r3
	bne .L_0200928a
	ldr r0, .L_020092c0
	b .L_0200928c
.L_0200928a:
	ldr r0, .L_020092c4
.L_0200928c:
	pop {pc}
	.2byte 0x0000
.L_02009290:
	.4byte gPartyState
.L_02009294:
	.4byte 0x00000022
.L_02009298:
	.4byte Data_02004f1c
.L_0200929c:
	.4byte 0x00000023
.L_020092a0:
	.4byte Data_02005078
.L_020092a4:
	.4byte 0x00000024
.L_020092a8:
	.4byte Data_020051bc
.L_020092ac:
	.4byte 0x00000025
.L_020092b0:
	.4byte Data_020053b4
.L_020092b4:
	.4byte 0x00000026
.L_020092b8:
	.4byte Data_02005450
.L_020092bc:
	.4byte 0x00000027
.L_020092c0:
	.4byte Data_020055b8
.L_020092c4:
	.4byte Data_02004f10
	.section .text.x020092c8,"ax",%progbits
	.global Func_020012c8
	.thumb_func
Func_020012c8:
	push {lr}
	ldr r0, .L_020092d4
	bl Func_020002f8
	pop {pc}
	.2byte 0x0000
.L_020092d4:
	.4byte Data_02003f44
	.section .text.x020092d8,"ax",%progbits
	.global Func_020012d8
	.thumb_func
Func_020012d8:
	push {lr}
	movs r0, #138
	lsls r0, r0, #1
	adds r0, #255
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020092fe
	movs r3, #9
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02003bf0
.L_020092fe:
	movs r0, #139
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009320
	movs r3, #11
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02003bf0
.L_02009320:
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009342
	movs r3, #14
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02003bf0
.L_02009342:
	movs r0, #146
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009364
	movs r3, #16
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02003bf0
.L_02009364:
	add sp, #8
	pop {pc}
	.section .text.x02009368,"ax",%progbits
	.global Func_02001368
	.thumb_func
Func_02001368:
	push {r5, lr}
	ldr r0, .L_020093a8
	bl Func_02000708
	bl Func_020012d8
	ldr r3, .L_020093ac
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r2, #128
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_020093a4
	ldr r0, [r5]
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
.L_020093a4:
	pop {r5, pc}
	.2byte 0x0000
.L_020093a8:
	.4byte Data_02003f52
.L_020093ac:
	.4byte gPartyState
	.section .text.x020093b0,"ax",%progbits
	.global Func_020013b0
	.thumb_func
Func_020013b0:
	push {lr}
	movs r0, #138
	lsls r0, r0, #1
	adds r0, #255
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020093d6
	movs r3, #50
	movs r2, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #63
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02003bf0
.L_020093d6:
	movs r0, #139
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020093f8
	movs r3, #52
	movs r2, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #63
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02003bf0
.L_020093f8:
	add sp, #8
	pop {pc}
	.section .text.x020093fc,"ax",%progbits
	.global Func_020013fc
	.thumb_func
Func_020013fc:
	push {r5, lr}
	ldr r0, .L_02009438
	bl Func_020002f8
	bl Func_020013b0
	ldr r3, .L_0200943c
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	ldr r2, .L_02009440
	ldr r3, [r0, #12]
	cmp r3, r2
	bge .L_02009436
	ldr r0, [r5]
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
.L_02009436:
	pop {r5, pc}
.L_02009438:
	.4byte Data_02003f80
.L_0200943c:
	.4byte gPartyState
.L_02009440:
	.4byte 0xffe80000
	.section .text.x02009444,"ax",%progbits
	.global Func_02001444
	.thumb_func
Func_02001444:
	push {lr}
	ldr r0, .L_0200946c
	bl Func_020002f8
	ldr r3, .L_02009470
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r2, #192
	lsls r2, r2, #13
	cmp r3, r2
	bge .L_02009468
	bl Func_02002180
.L_02009468:
	pop {pc}
	.2byte 0x0000
.L_0200946c:
	.4byte Data_02003f92
.L_02009470:
	.4byte gPartyState
	.section .text.x02009474,"ax",%progbits
	.global Func_02001474
	.thumb_func
Func_02001474:
	push {lr}
	ldr r3, .L_020094d4
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020094d8
	cmp r2, r3
	bne .L_02009492
	movs r1, #13
	movs r2, #14
	bl Func_02001514
	b .L_020094d0
.L_02009492:
	ldr r3, .L_020094dc
	cmp r2, r3
	bne .L_020094a2
	movs r1, #25
	movs r2, #13
	bl Func_02001514
	b .L_020094d0
.L_020094a2:
	ldr r3, .L_020094e0
	cmp r2, r3
	bne .L_020094b2
	movs r1, #26
	movs r2, #10
	bl Func_02001514
	b .L_020094d0
.L_020094b2:
	ldr r3, .L_020094e4
	cmp r2, r3
	bne .L_020094c2
	movs r1, #8
	movs r2, #3
	bl Func_02001514
	b .L_020094d0
.L_020094c2:
	ldr r3, .L_020094e8
	cmp r2, r3
	bne .L_020094d0
	movs r1, #10
	movs r2, #6
	bl Func_02001514
.L_020094d0:
	pop {pc}
	.2byte 0x0000
.L_020094d4:
	.4byte gPartyState
.L_020094d8:
	.4byte 0x00000022
.L_020094dc:
	.4byte 0x00000023
.L_020094e0:
	.4byte 0x00000024
.L_020094e4:
	.4byte 0x00000025
.L_020094e8:
	.4byte 0x00000027
	.section .text.x020094ec,"ax",%progbits
	.global Func_020014ec
	.thumb_func
Func_020014ec:
	push {lr}
	movs r0, #1
	bl Func_02001474
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	pop {pc}
	.2byte 0x0000
	.section .text.x02009500,"ax",%progbits
	.global Func_02001500
	.thumb_func
Func_02001500:
	push {lr}
	movs r0, #0
	bl Func_02001474
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	pop {pc}
	.2byte 0x0000
	.section .text.x02009514,"ax",%progbits
	.global Func_02001514
	.thumb_func
Func_02001514:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r2, #0
	movs r5, #0
	mov r8, r0
	adds r7, r1, #0
	cmp r5, r6
	bcs .L_0200954c
.L_02009526:
	adds r0, r7, r5
	bl Object_GetById
	mov r3, r8
	adds r0, #35
	adds r1, r5, #1
	cmp r3, #0
	beq .L_0200953e
	ldrb r2, [r0]
	movs r3, #239
	ands r3, r2
	b .L_02009544
.L_0200953e:
	ldrb r2, [r0]
	movs r3, #16
	orrs r3, r2
.L_02009544:
	strb r3, [r0]
	adds r5, r1, #0
	cmp r5, r6
	bcc .L_02009526
.L_0200954c:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02009554,"ax",%progbits
	.global Func_02001554
	.thumb_func
Func_02001554:
	push {lr}
	adds r3, r0, #0
	adds r2, r1, #0
	ldr r0, .L_02009564
	adds r1, r3, #0
	bl Func_020000a4
	pop {pc}
.L_02009564:
	.4byte Data_02003f98
	.section .text.x02009568,"ax",%progbits
	.global Func_02001568
	.thumb_func
Func_02001568:
	push {lr}
	adds r3, r0, #0
	adds r2, r1, #0
	ldr r0, .L_02009578
	adds r1, r3, #0
	bl Func_020000a4
	pop {pc}
.L_02009578:
	.4byte Data_02003fa0
	.section .text.x0200957c,"ax",%progbits
	.global Func_0200157c
	.thumb_func
Func_0200157c:
	push {lr}
	adds r3, r0, #0
	adds r2, r1, #0
	ldr r0, .L_0200958c
	adds r1, r3, #0
	bl Func_020000a4
	pop {pc}
.L_0200958c:
	.4byte Data_02003fa8
	.section .text.x02009590,"ax",%progbits
	.global Func_02001590
	.thumb_func
Func_02001590:
	push {lr}
	adds r3, r0, #0
	adds r2, r1, #0
	ldr r0, .L_020095a0
	adds r1, r3, #0
	bl Func_020000a4
	pop {pc}
.L_020095a0:
	.4byte Data_02003fb0
	.section .text.x020095a4,"ax",%progbits
	.global Func_020015a4
	.thumb_func
Func_020015a4:
	push {lr}
	adds r3, r0, #0
	adds r2, r1, #0
	ldr r0, .L_020095b4
	adds r1, r3, #0
	bl Func_020000a4
	pop {pc}
.L_020095b4:
	.4byte Data_02003fb8
	.section .text.x020095b8,"ax",%progbits
	.global Func_020015b8
	.thumb_func
Func_020015b8:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	adds r7, r6, #0
	adds r7, #85
	movs r3, #3
	strb r3, [r7]
.L_020095c4:
	movs r0, #1
	bl WaitFrames
	ldr r5, [r6, #40]
	cmp r5, #0
	bne .L_020095c4
	movs r0, #188
	bl Func_02003df8
	movs r0, #10
	bl WaitFrames
	strb r5, [r7]
	pop {r5, r6, r7, pc}
	.section .text.x020095e0,"ax",%progbits
	.global Func_020015e0
	.thumb_func
Func_020015e0:
	push {r5, r6, lr}
	movs r0, #12
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r6, r3, #20
	cmp r6, #56
	bne .L_0200962c
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #32
	bne .L_0200962c
	bl Func_02003c28
	movs r0, #0
	bl Func_02003d78
	adds r0, r5, #0
	bl Func_020015b8
	movs r3, #64
	str r3, [sp, #4]
	movs r0, #54
	movs r1, #32
	movs r2, #2
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02003bf0
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_SetBit
	bl Func_02003c30
.L_0200962c:
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x02009630,"ax",%progbits
	.global Func_02001630
	.thumb_func
Func_02001630:
	push {lr}
	ldr r0, .L_0200963c
	bl Func_02003dc0
	pop {pc}
	.2byte 0x0000
.L_0200963c:
	.4byte Data_02003fc0
	.section .text.x02009640,"ax",%progbits
	.global Func_02001640
	.thumb_func
Func_02001640:
	push {r5, r6, r7, lr}
	movs r0, #23
	sub sp, #8
	bl Object_GetById
	adds r7, r0, #0
	ldr r3, [r7, #8]
	asrs r6, r3, #20
	bl Func_02003dc8
	cmp r6, #52
	bne .L_0200969c
	bl Func_02003c28
	movs r0, #0
	bl Func_02003d78
	movs r5, #7
	movs r1, #7
	movs r2, #1
	movs r3, #1
	movs r0, #51
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003bf0
	movs r0, #1
	bl WaitFrames
	adds r0, r7, #0
	bl Func_020015b8
	movs r0, #50
	movs r1, #7
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003bf0
	movs r0, #132
	lsls r0, r0, #4
	bl GameFlag_SetBit
	bl Func_02003c30
.L_0200969c:
	add sp, #8
	pop {r5, r6, r7, pc}
	.section .text.x020096a0,"ax",%progbits
	.global Func_020016a0
	.thumb_func
Func_020016a0:
	push {lr}
	ldr r0, .L_020096ac
	bl Func_02003dc0
	pop {pc}
	.2byte 0x0000
.L_020096ac:
	.4byte Data_02003fc4
	.section .text.x020096b0,"ax",%progbits
	.global Func_020016b0
	.thumb_func
Func_020016b0:
	push {r5, r6, r7, lr}
	movs r0, #19
	sub sp, #8
	bl Object_GetById
	adds r7, r0, #0
	ldr r3, [r7, #8]
	asrs r6, r3, #20
	bl Func_02003dc8
	cmp r6, #47
	bne .L_0200970e
	bl Func_02003c28
	movs r0, #0
	bl Func_02003d78
	movs r5, #13
	movs r1, #12
	movs r2, #1
	movs r3, #1
	movs r0, #47
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003bf0
	movs r0, #1
	bl WaitFrames
	adds r0, r7, #0
	bl Func_020015b8
	movs r0, #50
	movs r1, #16
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003bf0
	movs r0, #19
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	bl Func_02003c30
	b .L_02009726
.L_0200970e:
	cmp r6, #46
	bne .L_02009726
	movs r3, #48
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #44
	movs r1, #13
	movs r2, #1
	movs r3, #1
	bl Func_02003bf0
.L_02009726:
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200972c,"ax",%progbits
	.global Func_0200172c
	.thumb_func
Func_0200172c:
	push {lr}
	movs r1, #150
	movs r2, #134
	movs r0, #20
	lsls r1, r1, #18
	lsls r2, r2, #18
	sub sp, #8
	bl Func_02003c68
	movs r3, #37
	movs r2, #33
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #32
	movs r2, #1
	movs r3, #1
	bl Func_02003bf0
	movs r3, #38
	movs r2, #32
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #30
	movs r1, #32
	movs r2, #1
	movs r3, #2
	bl Func_02003bf0
	movs r3, #36
	movs r2, #64
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #64
	movs r2, #4
	movs r3, #3
	movs r0, #40
	bl Func_02003bf0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #65
	bl GameFlag_SetBit
	add sp, #8
	pop {pc}
	.section .text.x02009788,"ax",%progbits
	.global Func_02001788
	.thumb_func
Func_02001788:
	push {lr}
	movs r0, #20
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #37
	bne .L_0200979c
	bl Func_0200172c
.L_0200979c:
	pop {pc}
	.2byte 0x0000
	.section .text.x020097a0,"ax",%progbits
	.global Func_020017a0
	.thumb_func
Func_020017a0:
	push {r5, lr}
	movs r0, #20
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #38
	bne .L_020097ec
	ldr r3, .L_020097f0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02003c28
	movs r0, #0
	bl Func_02003d78
	adds r5, #85
	movs r3, #0
	strb r3, [r5]
	movs r0, #1
	bl WaitFrames
	bl Func_02003de8
	bl Func_0200172c
	movs r0, #1
	bl WaitFrames
	movs r3, #3
	strb r3, [r5]
	bl Func_02003c30
.L_020097ec:
	pop {r5, pc}
	.2byte 0x0000
.L_020097f0:
	.4byte gPartyState
	.section .text.x020097f4,"ax",%progbits
	.global Func_020017f4
	.thumb_func
Func_020017f4:
	push {lr}
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	pop {pc}
	.2byte 0x0000
	.section .text.x02009804,"ax",%progbits
	.global Func_02001804
	.thumb_func
Func_02001804:
	push {r5, lr}
	movs r5, #200
	lsls r5, r5, #2
.L_0200980a:
	adds r0, r5, #0
	bl GameFlag_ClearBit
	movs r3, #152
	lsls r3, r3, #2
	adds r3, #255
	cmp r5, r3
	beq .L_0200981e
	adds r5, #1
	b .L_0200980a
.L_0200981e:
	pop {r5, pc}
	.section .text.x02009820,"ax",%progbits
	.global Func_02001820
	.thumb_func
Func_02001820:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r5, .L_0200989c
	movs r6, #144
	movs r3, #0
	sub sp, #8
	lsls r6, r6, #2
	mov r8, r3
	movs r7, #0
.L_02009834:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009868
	adds r0, r6, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009876
	ldr r3, .L_020098a0
	ldr r1, .L_020098a4
	adds r5, r7, r3
	ldrh r2, [r5]
	adds r5, #2
	ldrh r3, [r5]
	ldrh r0, [r1]
	movs r4, #1
	ldrh r1, [r1, #2]
	adds r5, #2
	str r4, [sp, #0]
	str r4, [sp, #4]
	bl Func_02003be0
	b .L_02009876
.L_02009868:
	ldrh r3, [r5]
	cmp r3, #0
	beq .L_02009874
	adds r0, r6, #0
	bl GameFlag_SetBit
.L_02009874:
	adds r5, #12
.L_02009876:
	movs r3, #1
	add r8, r3
	mov r3, r8
	adds r6, #1
	adds r7, #12
	cmp r3, #13
	bls .L_02009834
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009894
	bl Func_02001804
.L_02009894:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200989c:
	.4byte Data_02004066
.L_020098a0:
	.4byte Data_02004068
.L_020098a4:
	.4byte Data_02003fd2
	.section .text.x020098a8,"ax",%progbits
	.global Func_020018a8
	.thumb_func
Func_020018a8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r5, .L_02009924
	movs r6, #144
	movs r3, #0
	sub sp, #8
	lsls r6, r6, #2
	mov r8, r3
	movs r7, #0
.L_020098bc:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020098f0
	adds r0, r6, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020098fe
	ldr r3, .L_02009928
	ldr r1, .L_0200992c
	adds r5, r7, r3
	ldrh r2, [r5]
	adds r5, #2
	ldrh r3, [r5]
	ldrh r0, [r1]
	movs r4, #1
	ldrh r1, [r1, #2]
	adds r5, #2
	str r4, [sp, #0]
	str r4, [sp, #4]
	bl Func_02003be0
	b .L_020098fe
.L_020098f0:
	ldrh r3, [r5]
	cmp r3, #0
	beq .L_020098fc
	adds r0, r6, #0
	bl GameFlag_SetBit
.L_020098fc:
	adds r5, #12
.L_020098fe:
	movs r3, #1
	add r8, r3
	mov r3, r8
	adds r6, #1
	adds r7, #12
	cmp r3, #7
	bls .L_020098bc
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200991c
	bl Func_02001804
.L_0200991c:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02009924:
	.4byte Data_0200410e
.L_02009928:
	.4byte Data_02004110
.L_0200992c:
	.4byte Data_02003fda
	.section .text.x02009930,"ax",%progbits
	.global Func_02001930
	.thumb_func
Func_02001930:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r5, .L_020099ac
	movs r6, #144
	movs r3, #0
	sub sp, #8
	lsls r6, r6, #2
	mov r8, r3
	movs r7, #0
.L_02009944:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009978
	adds r0, r6, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009986
	ldr r3, .L_020099b0
	ldr r1, .L_020099b4
	adds r5, r7, r3
	ldrh r2, [r5]
	adds r5, #2
	ldrh r3, [r5]
	ldrh r0, [r1]
	movs r4, #1
	ldrh r1, [r1, #2]
	adds r5, #2
	str r4, [sp, #0]
	str r4, [sp, #4]
	bl Func_02003be0
	b .L_02009986
.L_02009978:
	ldrh r3, [r5]
	cmp r3, #0
	beq .L_02009984
	adds r0, r6, #0
	bl GameFlag_SetBit
.L_02009984:
	adds r5, #12
.L_02009986:
	movs r3, #1
	add r8, r3
	mov r3, r8
	adds r6, #1
	adds r7, #12
	cmp r3, #5
	bls .L_02009944
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020099a4
	bl Func_02001804
.L_020099a4:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_020099ac:
	.4byte Data_0200416e
.L_020099b0:
	.4byte Data_02004170
.L_020099b4:
	.4byte Data_02003fe2
	.section .text.x020099b8,"ax",%progbits
	.global Func_020019b8
	.thumb_func
Func_020019b8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r5, .L_02009a24
	movs r6, #144
	movs r3, #0
	sub sp, #8
	lsls r6, r6, #2
	mov r8, r3
	movs r7, #0
.L_020099cc:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009a00
	adds r0, r6, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009a0e
	ldr r3, .L_02009a28
	ldr r1, .L_02009a2c
	adds r5, r7, r3
	ldrh r2, [r5]
	adds r5, #2
	ldrh r3, [r5]
	ldrh r0, [r1]
	movs r4, #1
	ldrh r1, [r1, #2]
	adds r5, #2
	str r4, [sp, #0]
	str r4, [sp, #4]
	bl Func_02003be0
	b .L_02009a0e
.L_02009a00:
	ldrh r3, [r5]
	cmp r3, #0
	beq .L_02009a0c
	adds r0, r6, #0
	bl GameFlag_SetBit
.L_02009a0c:
	adds r5, #12
.L_02009a0e:
	movs r3, #1
	add r8, r3
	mov r3, r8
	adds r6, #1
	adds r7, #12
	cmp r3, #15
	bls .L_020099cc
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02009a24:
	.4byte Data_020041b6
.L_02009a28:
	.4byte Data_020041b8
.L_02009a2c:
	.4byte Data_02003fea
	.section .text.x02009a30,"ax",%progbits
	.global Func_02001a30
	.thumb_func
Func_02001a30:
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
	sub sp, #16
	ldr r5, .L_02009c90
	str r3, [sp, #12]
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r5, r0
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r1, #240
	asrs r3, r3, #20
	lsls r3, r3, #1
	adds r3, #1
	mov r8, r3
	ldr r3, [r0, #16]
	lsls r1, r1, #1
	asrs r3, r3, #20
	lsls r3, r3, #1
	adds r3, #1
	str r3, [sp, #8]
	adds r3, r5, r1
	mov r9, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009c94
	cmp r2, r3
	bne .L_02009aa2
	ldr r1, [sp, #12]
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	movs r1, #155
	adds r7, r3, #0
	lsls r1, r1, #1
	subs r7, #11
	adds r1, #255
	adds r1, r1, r3
	ldr r2, .L_02009c98
	lsls r3, r7, #1
	adds r3, r3, r7
	lsls r3, r3, #2
	mov r11, r1
	adds r6, r3, r2
	b .L_02009b36
.L_02009aa2:
	ldr r3, .L_02009c9c
	cmp r2, r3
	bne .L_02009ace
	ldr r2, [sp, #12]
	movs r0, #170
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	movs r2, #128
	lsls r2, r2, #2
	adds r7, r3, #0
	adds r2, #50
	adds r2, r2, r3
	subs r7, #14
	mov r11, r2
	lsls r3, r7, #1
	ldr r2, .L_02009ca0
	adds r3, r3, r7
	lsls r3, r3, #2
	adds r6, r3, r2
	b .L_02009b36
.L_02009ace:
	ldr r3, .L_02009ca4
	cmp r2, r3
	bne .L_02009af8
	ldr r0, [sp, #12]
	movs r1, #170
	lsls r1, r1, #1
	adds r3, r0, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	movs r0, #141
	adds r7, r3, #0
	subs r7, #12
	lsls r0, r0, #2
	adds r0, r0, r3
	ldr r2, .L_02009ca8
	lsls r3, r7, #1
	adds r3, r3, r7
	lsls r3, r3, #2
	mov r11, r0
	adds r6, r3, r2
	b .L_02009b36
.L_02009af8:
	ldr r3, .L_02009cac
	cmp r2, r3
	beq .L_02009b00
	b .L_02009e9e
.L_02009b00:
	ldr r1, [sp, #12]
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	movs r1, #128
	adds r7, r3, #0
	lsls r1, r1, #2
	subs r7, #18
	adds r1, #46
	adds r1, r1, r3
	ldr r5, .L_02009cb0
	lsls r3, r7, #1
	movs r0, #192
	adds r3, r3, r7
	lsls r0, r0, #2
	lsls r3, r3, #2
	adds r0, #58
	mov r11, r1
	adds r6, r3, r5
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009b36
	movs r3, #3
	strh r3, [r5, #52]
.L_02009b36:
	bl Func_02003c28
	movs r0, #0
	bl Func_02003d78
	ldr r2, .L_02009c90
	movs r5, #133
	mov r10, r2
	lsls r5, r5, #2
	add r5, r10
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #11
	lsls r2, r2, #10
	bl ObjectMotion_SetSpeedParameters
	mov r3, r8
	lsls r1, r3, #3
	ldr r3, [sp, #8]
	ldr r0, [r5]
	lsls r2, r3, #3
	bl ObjectMotion_SetPositionAndCommit
	mov r0, r8
	lsls r1, r0, #19
	ldr r0, [sp, #8]
	mov r3, r9
	ldr r2, [r3, #12]
	lsls r3, r0, #19
	mov r0, r9
	bl Object_SetPositionAndResetMotion
	movs r0, #4
	bl Battle_WaitMode0
	bl Func_02003cf8
	ldr r1, .L_02009cb4
	ldr r3, [r0, #12]
	adds r5, r6, #2
	adds r3, r3, r1
	str r3, [r0, #12]
	mov r0, r11
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009b98
	b .L_02009e00
.L_02009b98:
	movs r0, #229
	bl Func_02003df8
	movs r3, #240
	lsls r3, r3, #1
	add r3, r10
	ldrh r2, [r6]
	adds r6, r5, #0
	movs r0, #0
	ldrsh r5, [r3, r0]
	ldr r3, .L_02009c94
	ldrh r4, [r6]
	adds r6, #2
	cmp r5, r3
	bne .L_02009bba
	ldr r3, .L_02009cb8
	b .L_02009bcc
.L_02009bba:
	ldr r3, .L_02009c9c
	cmp r5, r3
	bne .L_02009bc4
	ldr r3, .L_02009cbc
	b .L_02009bcc
.L_02009bc4:
	ldr r3, .L_02009ca4
	cmp r5, r3
	bne .L_02009bde
	ldr r3, .L_02009cc0
.L_02009bcc:
	ldrh r0, [r3, #4]
	ldrh r1, [r3, #6]
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	adds r3, r4, #0
	bl Func_02003be0
	b .L_02009bf6
.L_02009bde:
	ldr r3, .L_02009cac
	cmp r5, r3
	bne .L_02009bf6
	ldr r3, .L_02009cc4
	ldrh r0, [r3, #4]
	ldrh r1, [r3, #6]
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	adds r3, r4, #0
	bl Func_02003be0
.L_02009bf6:
	ldrh r1, [r6]
	ldr r3, .L_02009c90
	movs r0, #240
	lsls r0, r0, #1
	lsls r2, r1, #1
	adds r3, r3, r0
	mov r8, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009c94
	adds r6, #2
	ldrh r5, [r6]
	ldrh r6, [r6, #2]
	cmp r2, r3
	bne .L_02009c24
	ldr r2, .L_02009cc8
	lsls r3, r1, #2
	ldrh r0, [r2, r3]
	adds r3, #2
	ldrh r1, [r2, r3]
	movs r3, #1
	movs r2, #2
	b .L_02009c82
.L_02009c24:
	ldr r3, .L_02009c9c
	cmp r2, r3
	bne .L_02009c3a
	ldr r2, .L_02009ccc
	lsls r3, r1, #2
	ldrh r0, [r2, r3]
	adds r3, #2
	ldrh r1, [r2, r3]
	movs r3, #1
	movs r2, #2
	b .L_02009c82
.L_02009c3a:
	ldr r3, .L_02009ca4
	cmp r2, r3
	bne .L_02009c50
	ldr r2, .L_02009cd0
	lsls r3, r1, #2
	ldrh r0, [r2, r3]
	adds r3, #2
	ldrh r1, [r2, r3]
	movs r3, #1
	movs r2, #2
	b .L_02009c82
.L_02009c50:
	ldr r3, .L_02009cac
	cmp r2, r3
	bne .L_02009cf4
	cmp r7, #0
	bne .L_02009c68
	movs r0, #161
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009c78
.L_02009c68:
	cmp r7, #11
	bne .L_02009cd8
	movs r0, #147
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009cd8
.L_02009c78:
	ldr r3, .L_02009cd4
	movs r2, #2
	ldrh r0, [r3, #16]
	ldrh r1, [r3, #18]
	movs r3, #1
.L_02009c82:
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r3, r6, #0
	adds r2, r5, #0
	bl Func_02003be0
	b .L_02009cf4
.L_02009c90:
	.4byte gPartyState
.L_02009c94:
	.4byte 0x00000022
.L_02009c98:
	.4byte Data_02004068
.L_02009c9c:
	.4byte 0x00000023
.L_02009ca0:
	.4byte Data_02004110
.L_02009ca4:
	.4byte 0x00000024
.L_02009ca8:
	.4byte Data_02004170
.L_02009cac:
	.4byte 0x00000026
.L_02009cb0:
	.4byte Data_020041b8
.L_02009cb4:
	.4byte 0xfff80000
.L_02009cb8:
	.4byte Data_02003fd2
.L_02009cbc:
	.4byte Data_02003fda
.L_02009cc0:
	.4byte Data_02003fe2
.L_02009cc4:
	.4byte Data_02003fea
.L_02009cc8:
	.4byte Data_02003ff2
.L_02009ccc:
	.4byte Data_02004012
.L_02009cd0:
	.4byte Data_02004032
.L_02009cd4:
	.4byte Data_02004052
.L_02009cd8:
	ldr r2, .L_02009d34
	mov r1, r8
	lsls r3, r1, #1
	ldrh r0, [r2, r3]
	adds r3, #2
	ldrh r1, [r2, r3]
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r3, r6, #0
	adds r2, r5, #0
	bl Func_02003be0
.L_02009cf4:
	movs r0, #12
	bl Battle_WaitMode0
	movs r3, #128
	ldr r2, .L_02009d30
	lsls r3, r3, #7
	mov r0, r9
	strh r3, [r0, #6]
	mov r3, r9
	adds r3, #85
	ldr r5, .L_02009d38
	strb r2, [r3]
	movs r1, #133
	lsls r1, r1, #2
	adds r6, r5, r1
	ldr r0, [r6]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009d3c
	cmp r2, r3
	bne .L_02009d4a
	b .L_02009d40
.L_02009d30:
	.4byte 0x00000000
.L_02009d34:
	.4byte Data_02004052
.L_02009d38:
	.4byte gPartyState
.L_02009d3c:
	.4byte 0x00000022
.L_02009d40:
	ldr r0, [r6]
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	b .L_02009d80
.L_02009d4a:
	ldr r3, .L_02009eac
	cmp r2, r3
	beq .L_02009d56
	ldr r3, .L_02009eb0
	cmp r2, r3
	bne .L_02009d60
.L_02009d56:
	ldr r0, [r6]
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	b .L_02009d80
.L_02009d60:
	ldr r3, .L_02009eb4
	cmp r2, r3
	bne .L_02009d80
	ldr r3, .L_02009eb8
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02009d78
	ldr r3, [r3, #80]
	movs r1, #12
	ldrb r2, [r3, #9]
	orrs r2, r1
	strb r2, [r3, #9]
.L_02009d78:
	ldr r0, [r6]
	movs r1, #3
	bl ObjectMotion_SetActionVariant
.L_02009d80:
	ldr r3, .L_02009ebc
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl Battle_WaitMode0
	movs r2, #0
	mov r8, r2
.L_02009d9a:
	mov r3, r8
	cmp r3, #5
	bne .L_02009da6
	movs r0, #204
	bl Func_02003df8
.L_02009da6:
	mov r0, r9
	ldr r3, [r0, #24]
	ldr r1, .L_02009ec0
	ldr r2, .L_02009ec4
	adds r3, r3, r1
	str r3, [r0, #24]
	ldr r3, [r0, #28]
	mov r1, r9
	adds r3, r3, r2
	str r3, [r0, #28]
	ldr r3, [r0, #12]
	ldr r0, .L_02009ec8
	adds r3, r3, r0
	str r3, [r1, #12]
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #39
	bls .L_02009d9a
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #214
	movs r2, #133
	lsls r0, r0, #1
	lsls r2, r2, #1
	adds r3, r3, r0
	adds r2, #255
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	ldr r1, [sp, #12]
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl Func_02003d00
	b .L_02009e9e
.L_02009e00:
	movs r0, #161
	bl Func_02003df8
	movs r3, #240
	lsls r3, r3, #1
	add r3, r10
	ldrh r4, [r5]
	movs r0, #0
	ldrsh r5, [r3, r0]
	ldr r3, .L_02009ecc
	ldrh r2, [r6]
	cmp r5, r3
	bne .L_02009e1e
	ldr r3, .L_02009ed0
	b .L_02009e30
.L_02009e1e:
	ldr r3, .L_02009eac
	cmp r5, r3
	bne .L_02009e28
	ldr r3, .L_02009ed4
	b .L_02009e30
.L_02009e28:
	ldr r3, .L_02009eb0
	cmp r5, r3
	bne .L_02009e42
	ldr r3, .L_02009ed8
.L_02009e30:
	ldrh r0, [r3]
	ldrh r1, [r3, #2]
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	adds r3, r4, #0
	bl Func_02003be0
	b .L_02009e5a
.L_02009e42:
	ldr r3, .L_02009eb4
	cmp r5, r3
	bne .L_02009e5a
	ldr r3, .L_02009edc
	ldrh r0, [r3]
	ldrh r1, [r3, #2]
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	adds r3, r4, #0
	bl Func_02003be0
.L_02009e5a:
	movs r0, #12
	bl Battle_WaitMode0
	mov r0, r11
	bl GameFlag_SetBit
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	ldr r3, .L_02009ebc
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009eb4
	cmp r2, r3
	beq .L_02009e88
	ldr r3, .L_02009eb0
	cmp r2, r3
	bne .L_02009e9a
.L_02009e88:
	mov r1, r9
	ldr r3, [r1, #8]
	ldr r2, .L_02009ee0
	asrs r3, r3, #20
	str r3, [r2]
	ldr r2, .L_02009ee4
	ldr r3, [r1, #16]
	asrs r3, r3, #20
	str r3, [r2]
.L_02009e9a:
	bl Func_02003c30
.L_02009e9e:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009eac:
	.4byte 0x00000023
.L_02009eb0:
	.4byte 0x00000024
.L_02009eb4:
	.4byte 0x00000026
.L_02009eb8:
	.4byte Data_02004420
.L_02009ebc:
	.4byte gPartyState
.L_02009ec0:
	.4byte 0xfffffc00
.L_02009ec4:
	.4byte 0xfffffd00
.L_02009ec8:
	.4byte 0xffff6667
.L_02009ecc:
	.4byte 0x00000022
.L_02009ed0:
	.4byte Data_02003fd2
.L_02009ed4:
	.4byte Data_02003fda
.L_02009ed8:
	.4byte Data_02003fe2
.L_02009edc:
	.4byte Data_02003fea
.L_02009ee0:
	.4byte Data_0200567c
.L_02009ee4:
	.4byte Data_02005678
	.section .text.x02009ee8,"ax",%progbits
	.global Func_02001ee8
	.thumb_func
Func_02001ee8:
	push {lr}
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009f28
	ldr r3, .L_02009f2c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r2, .L_02009f30
	ldr r3, [r0, #8]
	ldr r2, [r2]
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02009f1e
	ldr r3, .L_02009f34
	ldr r2, [r0, #16]
	ldr r3, [r3]
	asrs r2, r2, #20
	cmp r3, r2
	beq .L_02009f28
.L_02009f1e:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
.L_02009f28:
	pop {pc}
	.2byte 0x0000
.L_02009f2c:
	.4byte gPartyState
.L_02009f30:
	.4byte Data_0200567c
.L_02009f34:
	.4byte Data_02005678
	.section .text.x02009f38,"ax",%progbits
	.global Func_02001f38
	.thumb_func
Func_02001f38:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_02003cc0
	adds r0, r5, #0
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	adds r0, r5, #0
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02009f68,"ax",%progbits
	.global Func_02001f68
	.thumb_func
Func_02001f68:
	push {lr}
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009f88
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009f90
	bl Func_02001500
	b .L_02009f90
.L_02009f88:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_02009f90:
	pop {pc}
	.2byte 0x0000
	.section .text.x02009f94,"ax",%progbits
	.global Func_02001f94
	.thumb_func
Func_02001f94:
	push {r5, lr}
	movs r5, #0
.L_02009f98:
	adds r0, r5, #0
	adds r0, #13
	adds r5, #1
	bl Func_02001f38
	cmp r5, #13
	bls .L_02009f98
	bl Func_02001f68
	pop {r5, pc}
	.section .text.x02009fac,"ax",%progbits
	.global Func_02001fac
	.thumb_func
Func_02001fac:
	push {r5, lr}
	movs r5, #0
.L_02009fb0:
	adds r0, r5, #0
	adds r0, #25
	adds r5, #1
	bl Func_02001f38
	cmp r5, #12
	bls .L_02009fb0
	bl Func_02001f68
	pop {r5, pc}
	.section .text.x02009fc4,"ax",%progbits
	.global Func_02001fc4
	.thumb_func
Func_02001fc4:
	push {r5, lr}
	movs r5, #0
.L_02009fc8:
	adds r0, r5, #0
	adds r0, #26
	adds r5, #1
	bl Func_02001f38
	cmp r5, #9
	bls .L_02009fc8
	bl Func_02001f68
	pop {r5, pc}
	.section .text.x02009fdc,"ax",%progbits
	.global Func_02001fdc
	.thumb_func
Func_02001fdc:
	push {r5, lr}
	movs r5, #0
.L_02009fe0:
	adds r0, r5, #0
	adds r0, #8
	adds r5, #1
	bl Func_02001f38
	cmp r5, #2
	bls .L_02009fe0
	bl Func_02001f68
	pop {r5, pc}
	.section .text.x02009ff4,"ax",%progbits
	.global Func_02001ff4
	.thumb_func
Func_02001ff4:
	push {r5, lr}
	movs r5, #0
.L_02009ff8:
	adds r0, r5, #0
	adds r0, #10
	adds r5, #1
	bl Func_02001f38
	cmp r5, #5
	bls .L_02009ff8
	bl Func_02001f68
	pop {r5, pc}
	.section .text.x0200a00c,"ax",%progbits
	.global Func_0200200c
	.thumb_func
Func_0200200c:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	sub sp, #8
	bl Func_02000e9c
	ldr r3, [r7, #8]
	asrs r6, r3, #20
	cmp r6, #12
	bne .L_0200a05c
	bl Func_02003c28
	movs r0, #0
	bl Func_02003d78
	movs r5, #11
	movs r1, #32
	movs r2, #1
	movs r3, #1
	movs r0, #0
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003bf0
	movs r0, #1
	bl WaitFrames
	adds r0, r7, #0
	bl Func_020015b8
	movs r0, #1
	movs r1, #32
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003bf0
	bl Func_02003c30
	b .L_0200a0d6
.L_0200a05c:
	cmp r6, #18
	bne .L_0200a082
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #156
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r0, r7, #0
	ldr r5, [r3]
	bl Func_02003db0
	lsls r0, r0, #2
	adds r5, r5, r0
	ldrb r2, [r5, #3]
	movs r3, #127
	ands r3, r2
	strb r3, [r5, #3]
	b .L_0200a0d6
.L_0200a082:
	cmp r6, #54
	bne .L_0200a0ac
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #156
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r0, r7, #0
	ldr r5, [r3]
	bl Func_02003db0
	lsls r0, r0, #2
	adds r5, r5, r0
	ldrb r2, [r5, #3]
	movs r3, #127
	ands r3, r2
	strb r3, [r5, #3]
	movs r3, #11
	strb r3, [r5, #2]
	b .L_0200a0d6
.L_0200a0ac:
	cmp r6, #55
	bne .L_0200a0d6
	movs r3, #54
	movs r5, #20
	str r3, [sp, #0]
	movs r0, #53
	movs r1, #20
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02003bf0
	movs r3, #56
	str r3, [sp, #0]
	movs r0, #57
	movs r1, #20
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02003bf0
.L_0200a0d6:
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a0dc,"ax",%progbits
	.global Func_020020dc
	.thumb_func
Func_020020dc:
	push {r5, lr}
	bl Func_02003de0
	cmp r0, #0
	beq .L_0200a10a
	ldr r3, .L_0200a10c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Func_02003d10
	bl Object_GetById
	adds r5, r0, #0
	ldr r0, .L_0200a110
	bl Func_02000d78
	bl Func_02003de8
	adds r0, r5, #0
	bl Func_0200200c
.L_0200a10a:
	pop {r5, pc}
.L_0200a10c:
	.4byte gPartyState
.L_0200a110:
	.4byte Data_02003fc8
	.section .text.x0200a114,"ax",%progbits
	.global Func_02002114
	.thumb_func
Func_02002114:
	push {lr}
	ldr r0, .L_0200a120
	bl Func_02000d78
	pop {pc}
	.2byte 0x0000
.L_0200a120:
	.4byte Data_02003fc8
	.section .text.x0200a124,"ax",%progbits
	.global Func_02002124
	.thumb_func
Func_02002124:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #188
	adds r3, r3, r2
	ldr r0, [r3]
	cmp r0, #0
	beq .L_0200a13e
	bl Func_0200200c
.L_0200a13e:
	pop {pc}
	.section .text.x0200a140,"ax",%progbits
	.global Func_02002140
	.thumb_func
Func_02002140:
	push {r5, lr}
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	sub sp, #8
	bl GameFlag_Test
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0200a17c
	movs r0, #8
	bl Object_GetById
	movs r3, #47
	adds r0, #85
	movs r2, #12
	strb r5, [r0]
	movs r1, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #51
	movs r2, #1
	movs r3, #1
	bl Func_02003bf0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
.L_0200a17c:
	add sp, #8
	pop {r5, pc}
	.section .text.x0200a180,"ax",%progbits
	.global Func_02002180
	.thumb_func
Func_02002180:
	push {lr}
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a1b0
	movs r3, #47
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #48
	movs r1, #11
	movs r2, #1
	movs r3, #1
	bl Func_02003bf0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_ClearBit
.L_0200a1b0:
	add sp, #8
	pop {pc}
	.section .text.x0200a1b4,"ax",%progbits
	.global Func_020021b4
	.thumb_func
Func_020021b4:
	push {r5, r6, lr}
	bl Func_02003cf8
	adds r5, r0, #0
	ldr r3, [r5, #12]
	ldr r2, .L_0200a1e4
	movs r0, #1
	adds r3, r3, r2
	str r3, [r5, #12]
	bl WaitFrames
	movs r6, #0
.L_0200a1cc:
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r5, #12]
	movs r0, #1
	adds r6, #1
	bl WaitFrames
	cmp r6, #7
	bls .L_0200a1cc
	pop {r5, r6, pc}
.L_0200a1e4:
	.4byte 0xfff80000
	.section .text.x0200a1e8,"ax",%progbits
	.global Func_020021e8
	.thumb_func
Func_020021e8:
	push {r5, lr}
	sub sp, #8
	bl Func_02003c28
	movs r0, #0
	bl Func_02003d78
	bl Func_02003cf8
	movs r3, #0
	adds r0, #85
	movs r1, #1
	movs r2, #164
	strb r3, [r0]
	negs r1, r1
	ldr r0, .L_0200a2d4
	lsls r2, r2, #16
	movs r3, #1
	bl Motion_CamBounds
	bl Func_02003cf0
	ldr r3, .L_0200a2d8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r0, #24
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #24
	bl Func_02003cc8
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #24
	ldr r1, .L_0200a2dc
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #166
	movs r0, #24
	lsls r1, r1, #2
	movs r2, #136
	bl ObjectMotion_SetPositionAndReset
	movs r5, #1
	movs r3, #6
	movs r1, #33
	movs r2, #105
	movs r0, #68
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003be0
	movs r0, #161
	bl Func_02003df8
	bl Func_020021b4
	movs r0, #164
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r1, #166
	movs r0, #24
	lsls r1, r1, #2
	movs r2, #104
	bl ObjectMotion_SetPositionAndReset
	movs r3, #4
	movs r1, #33
	movs r2, #105
	movs r0, #69
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003be0
	movs r0, #229
	bl Func_02003df8
	bl Func_020021b4
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #24
	movs r1, #6
	movs r2, #16
	bl ObjectMotion_Launch
	movs r3, #128
	movs r1, #222
	movs r2, #168
	lsls r3, r3, #7
	lsls r1, r1, #18
	lsls r2, r2, #16
	movs r0, #24
	bl Func_02003c70
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #108
	bl GameFlag_SetBit
	bl Func_02003c30
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
.L_0200a2d4:
	.4byte 0x02920000
.L_0200a2d8:
	.4byte gPartyState
.L_0200a2dc:
	.4byte 0x00019999
	.section .text.x0200a2e0,"ax",%progbits
	.global Func_020022e0
	.thumb_func
Func_020022e0:
	push {lr}
	bl Func_02003c28
	movs r0, #0
	bl Func_02003d78
	movs r0, #245
	movs r1, #1
	movs r2, #168
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl Motion_CamBounds
	bl Func_02003cf0
	ldr r3, .L_0200a388
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r0, #24
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #24
	bl Func_02003cc8
	movs r0, #24
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #1
	movs r0, #24
	bl ObjectMotion_SetActionVariant
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #24
	ldr r1, .L_0200a38c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #235
	movs r0, #24
	lsls r1, r1, #2
	movs r2, #168
	bl ObjectMotion_SetPositionAndReset
	movs r1, #246
	movs r0, #24
	lsls r1, r1, #2
	movs r2, #152
	bl ObjectMotion_SetPositionAndReset
	movs r1, #130
	movs r0, #24
	lsls r1, r1, #3
	movs r2, #152
	bl ObjectMotion_SetPositionAndReset
	movs r1, #0
	movs r2, #0
	movs r0, #24
	bl Func_02003c68
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #109
	bl GameFlag_SetBit
	bl Func_02003c30
	pop {pc}
	.2byte 0x0000
.L_0200a388:
	.4byte gPartyState
.L_0200a38c:
	.4byte 0x00019999
	.section .text.x0200a390,"ax",%progbits
	.global Func_02002390
	.thumb_func
Func_02002390:
	push {lr}
	bl Func_02003c28
	movs r0, #0
	bl Func_02003d78
	movs r0, #144
	movs r1, #1
	movs r2, #214
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #15
	movs r3, #1
	bl Motion_CamBounds
	bl Func_02003cf0
	ldr r3, .L_0200a450
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r0, #25
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #25
	bl Func_02003cc8
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #25
	ldr r1, .L_0200a454
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #104
	movs r1, #168
	movs r0, #25
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #153
	bl Func_02003df8
	movs r0, #25
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #25
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r2, #104
	movs r1, #200
	movs r0, #25
	bl ObjectMotion_SetPositionAndReset
	movs r0, #25
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #147
	movs r0, #25
	lsls r1, r1, #1
	movs r2, #104
	bl ObjectMotion_SetPositionAndReset
	movs r3, #128
	movs r1, #212
	movs r2, #208
	lsls r3, r3, #7
	lsls r1, r1, #17
	lsls r2, r2, #15
	movs r0, #25
	bl Func_02003c70
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #110
	bl GameFlag_SetBit
	bl Func_02003c30
	pop {pc}
.L_0200a450:
	.4byte gPartyState
.L_0200a454:
	.4byte 0x00019999
	.section .text.x0200a458,"ax",%progbits
	.global Func_02002458
	.thumb_func
Func_02002458:
	push {r5, lr}
	sub sp, #8
	bl Func_02003c28
	movs r0, #0
	bl Func_02003d78
	ldr r5, .L_0200a56c
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #25
	bl Object_LinkObjectAndSetCallback
	bl Func_02003cf8
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #1
	movs r0, #212
	movs r2, #208
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #15
	bl Motion_CamBounds
	bl Func_02003cf0
	ldr r1, [r5]
	movs r0, #25
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #25
	bl Func_02003cc8
	movs r0, #212
	movs r1, #1
	movs r2, #152
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #25
	ldr r1, .L_0200a570
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #212
	movs r0, #25
	lsls r1, r1, #1
	movs r2, #152
	bl ObjectMotion_SetPositionAndReset
	movs r0, #25
	movs r1, #0
	movs r2, #20
	bl ObjectMotion_ArmCallback
	bl Func_02002604
	movs r1, #220
	movs r0, #25
	lsls r1, r1, #1
	movs r2, #152
	bl ObjectMotion_SetPositionAndReset
	movs r1, #228
	movs r0, #25
	lsls r1, r1, #1
	movs r2, #168
	bl ObjectMotion_SetPositionAndReset
	movs r1, #228
	lsls r1, r1, #1
	movs r2, #184
	movs r0, #25
	bl ObjectMotion_SetPositionAndReset
	movs r0, #161
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r1, #51
	movs r3, #11
	movs r2, #92
	movs r0, #69
	bl Func_02003be0
	movs r0, #161
	bl Func_02003df8
	bl Func_020021b4
	movs r1, #228
	movs r0, #25
	lsls r1, r1, #1
	movs r2, #216
	bl ObjectMotion_SetPositionAndReset
	movs r1, #146
	movs r0, #25
	lsls r1, r1, #2
	movs r2, #216
	bl ObjectMotion_SetPositionAndReset
	movs r3, #128
	movs r1, #154
	movs r2, #140
	lsls r3, r3, #7
	lsls r1, r1, #18
	lsls r2, r2, #17
	movs r0, #25
	bl Func_02003c70
	movs r0, #238
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_02003c30
	add sp, #8
	pop {r5, pc}
.L_0200a56c:
	.4byte gPartyState
.L_0200a570:
	.4byte 0x00019999
	.section .text.x0200a574,"ax",%progbits
	.global Func_02002574
	.thumb_func
Func_02002574:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	movs r0, #18
	ldr r6, [r3]
	sub sp, #8
	bl Object_GetById
	movs r2, #224
	lsls r2, r2, #3
	adds r2, #198
	adds r3, r6, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	movs r2, #1
	negs r2, r2
	adds r5, r0, #0
	cmp r3, r2
	beq .L_0200a5a8
	cmp r3, #7
	bgt .L_0200a5a8
	ldr r3, [r5, #28]
	ldr r2, .L_0200a600
	adds r3, r3, r2
	str r3, [r5, #28]
.L_0200a5a8:
	movs r2, #224
	lsls r2, r2, #3
	adds r2, #196
	adds r3, r6, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a5fa
	movs r3, #27
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #1
	movs r0, #27
	movs r1, #8
	bl Func_02003bf0
	movs r0, #18
	movs r1, #4
	bl Object_SetModeById
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	adds r1, #54
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r0, #131
	movs r3, #128
	lsls r3, r3, #9
	lsls r0, r0, #1
	str r3, [r5, #28]
	adds r0, #255
	bl GameFlag_SetBit
.L_0200a5fa:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a600:
	.4byte 0xffffe100
	.section .text.x0200a604,"ax",%progbits
	.global Func_02002604
	.thumb_func
Func_02002604:
	push {r5, lr}
	movs r0, #134
	movs r1, #1
	bl Func_02003d50
	ldr r5, .L_0200a63c
	movs r1, #144
	adds r0, r5, #0
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r1, #18
	movs r0, #25
	bl Func_02003d58
	bl Func_02003d70
	movs r0, #1
	bl Field_DispatchTypeHandler
	bl Func_02003d60
	adds r0, r5, #0
	bl Scheduler_RemoveCallbackFar
	bl Func_02003d68
	pop {r5, pc}
.L_0200a63c:
	.4byte Func_02002574
	.section .text.x0200a640,"ax",%progbits
	.global Func_02002640
	.thumb_func
Func_02002640:
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
	bge .L_0200a670
	adds r3, #15
.L_0200a670:
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
	.section .text.x0200a698,"ax",%progbits
	.global Func_02002698
	.thumb_func
Func_02002698:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r5, r0, #0
	movs r0, #125
	mov r9, r2
	adds r6, r1, #0
	bl Func_02003df8
	adds r0, r5, #0
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #2
	bl WaitFrames
	movs r1, #0
	adds r0, r5, #0
	bl Object_SetModeById
	adds r0, r5, #0
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #4
	bl WaitFrames
	adds r0, r5, #0
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	adds r0, r6, #0
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #0
	mov r10, r0
	mov r8, r0
.L_0200a6f4:
	bl Random16Far
	lsls r3, r0, #3
	subs r3, r3, r0
	ldr r2, [r7, #12]
	lsls r3, r3, #1
	lsrs r3, r3, #16
	lsls r3, r3, #16
	subs r2, r2, r3
	mov r3, r8
	lsls r1, r3, #17
	ldr r3, [r7, #8]
	ldr r0, .L_0200a7c4
	adds r1, r1, r3
	ldr r3, .L_0200a7c8
	adds r1, r1, r0
	movs r0, #30
	adds r2, r2, r3
	adds r0, #255
	ldr r3, [r7, #16]
	bl Func_02003bb0
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200a7a4
	mov r1, r10
	ldr r0, [r6, #80]
	bl Func_02003d80
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r3, #4
	strb r5, [r3]
	movs r1, #0
	mov r10, r0
	adds r0, r6, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r6, #0
	movs r1, #2
	bl Func_02003ba0
	adds r0, r6, #0
	ldr r1, .L_0200a7cc
	bl Func_02003ba8
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	ldr r1, [r6, #80]
	str r3, [r6, #24]
	str r3, [r6, #28]
	movs r0, #13
	ldrb r3, [r1, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	str r5, [r6, #68]
	str r5, [r6, #76]
	bl Random16Far
	movs r3, #192
	lsls r0, r0, #14
	lsls r3, r3, #7
	lsrs r0, r0, #16
	adds r0, r0, r3
	negs r0, r0
	str r0, [r6, #72]
	bl Random16Far
	ldr r3, .L_0200a7d0
	lsls r0, r0, #9
	lsrs r0, r0, #16
	adds r0, r0, r3
	adds r3, r6, #0
	adds r3, #100
	strh r0, [r3]
	ldr r3, .L_0200a7d4
	str r3, [r6, #48]
	ldr r3, .L_0200a7d8
	str r3, [r6, #52]
	ldr r3, .L_0200a7dc
	str r3, [r6, #108]
.L_0200a7a4:
	movs r0, #1
	add r8, r0
	mov r3, r8
	cmp r3, #7
	bls .L_0200a6f4
	mov r0, r9
	bl GameFlag_SetBit
	movs r0, #20
	bl Battle_WaitMode0
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200a7c4:
	.4byte 0xfff80000
.L_0200a7c8:
	.4byte 0xfffe0000
.L_0200a7cc:
	.4byte Data_02003f14
.L_0200a7d0:
	.4byte 0xffffff00
.L_0200a7d4:
	.4byte 0xfffff800
.L_0200a7d8:
	.4byte 0xfffffa00
.L_0200a7dc:
	.4byte Func_02002640
	.section .text.x0200a7e0,"ax",%progbits
	.global Func_020027e0
	.thumb_func
Func_020027e0:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	movs r0, #25
	sub sp, #8
	bl Object_GetById
	mov r10, r0
	bl Func_02003c28
	movs r0, #0
	bl Func_02003d78
	ldr r6, .L_0200aa50
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r6, r2
	ldr r0, [r6]
	movs r1, #25
	bl Object_LinkObjectAndSetCallback
	bl Func_02003cf8
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #1
	movs r0, #154
	movs r2, #140
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	bl Func_02003cf0
	ldr r1, [r6]
	movs r0, #25
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #25
	bl Func_02003cc8
	movs r0, #25
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #25
	ldr r1, .L_0200aa54
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #158
	movs r2, #140
	movs r0, #25
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #25
	bl ObjectMotion_SetSpeedParameters
	movs r0, #153
	bl Func_02003df8
	movs r3, #128
	lsls r3, r3, #12
	mov r2, r10
	str r3, [r2, #40]
	movs r0, #25
	mov r8, r3
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #166
	movs r2, #140
	lsls r2, r2, #1
	lsls r1, r1, #2
	movs r0, #25
	bl ObjectMotion_SetPositionAndReset
	movs r0, #25
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r2, #132
	movs r1, #25
	lsls r2, r2, #2
	movs r0, #21
	bl Func_02002698
	movs r0, #153
	bl Func_02003df8
	mov r3, r8
	mov r2, r10
	str r3, [r2, #40]
	movs r0, #25
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #174
	movs r2, #140
	lsls r2, r2, #1
	lsls r1, r1, #2
	movs r0, #25
	bl ObjectMotion_SetPositionAndReset
	movs r0, #25
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #25
	ldr r1, .L_0200aa54
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #178
	movs r2, #140
	lsls r2, r2, #1
	movs r0, #25
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #152
	lsls r1, r1, #7
	ldr r0, .L_0200aa58
	adds r1, #204
	bl Func_02003ce0
	movs r0, #178
	movs r1, #1
	movs r2, #136
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl Motion_CamBounds
	movs r5, #1
	movs r3, #4
	movs r2, #110
	movs r0, #112
	movs r1, #6
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003be0
	movs r0, #25
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r1, #178
	movs r0, #25
	lsls r1, r1, #2
	movs r2, #200
	bl ObjectMotion_SetPositionAndReset
	movs r1, #178
	movs r2, #168
	lsls r1, r1, #2
	movs r0, #25
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r6]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #25
	movs r1, #0
	bl Func_02003cb0
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #25
	bl ObjectMotion_SetSpeedParameters
	movs r0, #153
	bl Func_02003df8
	mov r3, r8
	mov r2, r10
	str r3, [r2, #40]
	movs r0, #25
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #186
	movs r2, #168
	lsls r1, r1, #2
	movs r0, #25
	bl ObjectMotion_SetPositionAndReset
	movs r0, #25
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #22
	movs r0, #24
	movs r1, #25
	bl Func_02002698
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #25
	bl Func_02003cb0
	movs r0, #153
	bl Func_02003df8
	mov r3, r8
	mov r2, r10
	str r3, [r2, #40]
	movs r0, #25
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #186
	movs r2, #136
	lsls r1, r1, #2
	movs r0, #25
	bl ObjectMotion_SetPositionAndReset
	movs r0, #25
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #186
	movs r0, #25
	lsls r1, r1, #2
	movs r2, #72
	bl ObjectMotion_SetPositionAndReset
	movs r3, #4
	movs r1, #51
	movs r2, #110
	movs r0, #66
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003be0
	movs r0, #229
	bl Func_02003df8
	bl Func_020021b4
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #6
	movs r2, #16
	bl ObjectMotion_Launch
	movs r1, #0
	movs r2, #0
	movs r0, #25
	bl Func_02003c68
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #135
	lsls r0, r0, #4
	bl GameFlag_SetBit
	bl Func_02003c30
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
.L_0200aa50:
	.4byte gPartyState
.L_0200aa54:
	.4byte 0x00019999
.L_0200aa58:
	.4byte 0x00026666
	.section .text.x0200aa5c,"ax",%progbits
	.global Func_02002a5c
	.thumb_func
Func_02002a5c:
	push {r5, r6, lr}
	ldr r5, .L_0200ab10
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	sub sp, #8
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02003c28
	movs r0, #0
	bl Func_02003d78
	movs r3, #192
	movs r1, #248
	movs r2, #136
	ldr r0, [r5]
	lsls r3, r3, #8
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02003c70
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r1, #64
	movs r3, #8
	movs r2, #15
	movs r0, #65
	bl Func_02003be0
	movs r0, #163
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #161
	bl Func_02003df8
	bl Func_020021b4
	ldr r1, [r5]
	movs r0, #9
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #9
	bl Func_02003cc8
	bl Func_02002bcc
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #128
	ldr r2, .L_0200ab0c
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	movs r0, #9
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	ldr r0, [r5]
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	ldr r0, [r5]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl Battle_WaitMode0
	movs r5, #0
.L_0200ab00:
	cmp r5, #5
	bne .L_0200ab14
	movs r0, #204
	bl Func_02003df8
	b .L_0200ab14
.L_0200ab0c:
	.4byte 0x00000000
.L_0200ab10:
	.4byte gPartyState
.L_0200ab14:
	ldr r3, [r6, #24]
	ldr r2, .L_0200ab5c
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, .L_0200ab60
	ldr r3, [r6, #28]
	adds r5, #1
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r2, .L_0200ab64
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	bl WaitFrames
	cmp r5, #39
	bls .L_0200ab00
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
	movs r0, #23
	bl Func_02003d00
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200ab5c:
	.4byte 0xfffffc00
.L_0200ab60:
	.4byte 0xfffffd00
.L_0200ab64:
	.4byte 0xffff6667
	.section .text.x0200ab68,"ax",%progbits
	.global Func_02002b68
	.thumb_func
Func_02002b68:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r2, #224
	lsls r2, r2, #3
	adds r2, #196
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	sub sp, #8
	cmp r3, #0
	beq .L_0200abc2
	movs r0, #229
	bl Func_02003df8
	movs r5, #1
	movs r0, #64
	movs r1, #64
	movs r2, #15
	movs r3, #8
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003be0
	movs r3, #2
	str r3, [sp, #4]
	movs r1, #66
	movs r0, #64
	movs r2, #15
	movs r3, #72
	str r5, [sp, #0]
	bl Func_02003be0
	ldr r3, .L_0200abc8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
.L_0200abc2:
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
.L_0200abc8:
	.4byte gPartyState
	.section .text.x0200abcc,"ax",%progbits
	.global Func_02002bcc
	.thumb_func
Func_02002bcc:
	push {r5, lr}
	movs r0, #134
	movs r1, #1
	bl Func_02003d50
	ldr r5, .L_0200ac0c
	movs r1, #144
	adds r0, r5, #0
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_0200ac10
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r0, #9
	bl Func_02003d58
	bl Func_02003d70
	movs r0, #1
	bl Field_DispatchTypeHandler
	bl Func_02003d60
	adds r0, r5, #0
	bl Scheduler_RemoveCallbackFar
	bl Func_02003d68
	pop {r5, pc}
.L_0200ac0c:
	.4byte Func_02002b68
.L_0200ac10:
	.4byte gPartyState
	.section .text.x0200ac14,"ax",%progbits
	.global Func_02002c14
	.thumb_func
Func_02002c14:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_0200adc8
	movs r2, #133
	lsls r2, r2, #2
	adds r2, r2, r3
	ldr r0, [r2]
	sub sp, #8
	mov r8, r2
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #9
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02003c28
	movs r0, #0
	bl Func_02003d78
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02003bc8
	movs r0, #1
	bl WaitFrames
	movs r3, #130
	lsls r3, r3, #16
	str r3, [r5, #12]
	movs r2, #0
	movs r3, #128
	lsls r3, r3, #8
	mov r10, r2
	adds r6, r5, #0
	str r3, [r5, #72]
	adds r6, #85
	mov r9, r3
	mov r3, r10
	str r2, [r5, #68]
	strb r3, [r6]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #172
	str r2, [r3]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #204
	bl Func_02003df8
	movs r3, #3
	strb r3, [r6]
	movs r0, #24
	bl Battle_WaitMode0
	movs r0, #176
	bl Func_02003df8
	bl Func_020021b4
	ldr r6, .L_0200adcc
	ldr r0, [r6]
	cmp r0, #0
	beq .L_0200acd8
	movs r1, #248
	movs r3, #240
	movs r2, #0
	lsls r3, r3, #15
	lsls r1, r1, #16
	bl Object_SetPositionAndResetMotion
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6]
	ldr r2, .L_0200add0
	str r2, [r3, #48]
	str r2, [r3, #52]
	movs r2, #128
	lsls r2, r2, #12
	str r2, [r3, #40]
.L_0200acd8:
	movs r2, #204
	mov r3, r8
	lsls r2, r2, #8
	ldr r0, [r3]
	ldr r1, .L_0200add0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #9
	ldr r1, .L_0200add0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	movs r1, #216
	str r3, [r7, #40]
	movs r3, #240
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	lsls r3, r3, #15
	bl Func_02003bd0
	movs r1, #140
	movs r3, #240
	adds r0, r7, #0
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #15
	bl Func_02003bd0
	ldr r0, [r6]
	cmp r0, #0
	beq .L_0200ad32
	movs r1, #248
	movs r3, #176
	lsls r1, r1, #16
	movs r2, #0
	lsls r3, r3, #15
	bl Func_02003bd0
.L_0200ad32:
	adds r0, r5, #0
	bl Func_02003bd8
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #4
	adds r1, #255
	movs r2, #0
	movs r0, #9
	bl Func_02003cc8
	mov r2, r8
	ldr r0, [r2]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #9
	mov r1, r9
	movs r2, #60
	bl ObjectMotion_ArmCallback
	movs r1, #148
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #104
	bl ObjectMotion_SetPositionAndReset
	movs r1, #148
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #80
	bl ObjectMotion_SetPositionAndReset
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02003c68
	movs r0, #20
	bl Battle_WaitMode0
	ldr r3, [r6]
	cmp r3, #0
	beq .L_0200adae
	adds r3, #85
	mov r2, r10
	strb r2, [r3]
	movs r3, #15
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02003bf0
	movs r0, #1
	bl WaitFrames
.L_0200adae:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #113
	bl GameFlag_SetBit
	bl Func_02003c30
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200adc8:
	.4byte gPartyState
.L_0200adcc:
	.4byte Data_02004420
.L_0200add0:
	.4byte 0x00019999
	.section .text.x0200add4,"ax",%progbits
	.global Func_02002dd4
	.thumb_func
Func_02002dd4:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #16
	bl Object_GetById
	mov r8, r0
	bl Func_02003c28
	movs r0, #0
	bl Func_02003d78
	ldr r6, .L_0200af04
	movs r3, #133
	lsls r3, r3, #2
	adds r6, r6, r3
	ldr r0, [r6]
	movs r1, #16
	bl Object_LinkObjectAndSetCallback
	bl Func_02003cf8
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #128
	movs r0, #190
	movs r2, #204
	movs r3, #1
	lsls r0, r0, #18
	lsls r1, r1, #14
	lsls r2, r2, #16
	bl Motion_CamBounds
	bl Func_02003cf0
	ldr r1, [r6]
	movs r0, #16
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #16
	bl Func_02003cc8
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #10
	movs r0, #16
	bl ObjectMotion_SetSpeedParameters
	movs r5, #128
	movs r0, #153
	bl Func_02003df8
	lsls r5, r5, #12
	mov r3, r8
	str r5, [r3, #40]
	movs r0, #16
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #190
	movs r2, #200
	lsls r1, r1, #2
	movs r0, #16
	bl ObjectMotion_SetPositionAndReset
	movs r0, #16
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #153
	bl Func_02003df8
	mov r3, r8
	str r5, [r3, #40]
	movs r0, #16
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #198
	movs r2, #200
	lsls r1, r1, #2
	movs r0, #16
	bl ObjectMotion_SetPositionAndReset
	movs r0, #16
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r2, #132
	lsls r2, r2, #2
	movs r1, #16
	movs r0, #9
	bl Func_02002698
	movs r0, #153
	bl Func_02003df8
	mov r3, r8
	str r5, [r3, #40]
	movs r0, #16
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #206
	movs r2, #200
	lsls r1, r1, #2
	movs r0, #16
	bl ObjectMotion_SetPositionAndReset
	movs r0, #16
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #20
	bl Battle_WaitMode0
	ldr r1, [r6]
	movs r0, #16
	bl Object_LinkObjectAndSetCallback
	ldr r0, [r6]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_02003c30
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_0200af04:
	.4byte gPartyState
	.section .text.x0200af08,"ax",%progbits
	.global Func_02002f08
	.thumb_func
Func_02002f08:
	push {r5, lr}
	bl Func_02003c28
	movs r0, #0
	bl Func_02003d78
	ldr r5, .L_0200af70
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r1, [r5]
	movs r0, #16
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #16
	bl Func_02003cc8
	movs r1, #0
	movs r0, #16
	bl Func_02003cb0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #16
	movs r1, #3
	bl Motion_SetVarCbAndRefresh
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #8
	bl Func_02003cb0
	ldr r1, [r5]
	movs r0, #16
	bl Object_LinkObjectAndSetCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #16
	bl Func_02003cd0
	movs r0, #40
	bl Battle_WaitMode0
	bl Func_02003c30
	pop {r5, pc}
.L_0200af70:
	.4byte gPartyState
	.section .text.x0200af74,"ax",%progbits
	.global Func_02002f74
	.thumb_func
Func_02002f74:
	push {lr}
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200af8a
	bl Func_02002f08
	b .L_0200af8e
.L_0200af8a:
	bl Func_02002dd4
.L_0200af8e:
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	pop {pc}
	.2byte 0x0000
	.section .text.x0200af9c,"ax",%progbits
	.global Func_02002f9c
	.thumb_func
Func_02002f9c:
	push {r5, lr}
	sub sp, #8
	bl Func_02003c28
	movs r0, #0
	bl Func_02003d78
	ldr r5, .L_0200aff0
	ldr r0, [r5]
	cmp r0, #0
	beq .L_0200afb8
	movs r1, #3
	bl Func_02003d40
.L_0200afb8:
	movs r0, #199
	movs r1, #0
	bl PartyInventory_GiveItem
	ldr r0, [r5]
	cmp r0, #0
	beq .L_0200afca
	bl Func_02003bb8
.L_0200afca:
	movs r3, #15
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #1
	bl Func_02003bf0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #120
	bl GameFlag_SetBit
	bl Func_02003c30
	add sp, #8
	pop {r5, pc}
.L_0200aff0:
	.4byte Data_02004420
	.section .text.x0200aff4,"ax",%progbits
	.global Func_02002ff4
	.thumb_func
Func_02002ff4:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	bl Func_02003c28
	movs r0, #0
	bl Func_02003d78
	ldr r1, .L_0200b044
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_0200b048
	cmp r2, r3
	bne .L_0200b026
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r1, r2
	ldr r0, [r3]
	movs r1, #3
	bl ObjectMotion_SetActionVariant
.L_0200b026:
	movs r0, #123
	bl Func_02003df8
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #170
	lsls r0, r0, #1
	adds r3, r5, r0
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_02003d00
	pop {r5, pc}
.L_0200b044:
	.4byte gPartyState
.L_0200b048:
	.4byte 0x00000022
	.section .text.x0200b04c,"ax",%progbits
	.global Func_0200304c
	.thumb_func
Func_0200304c:
	push {lr}
	ldr r3, .L_0200b0a4
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200b0a8
	cmp r2, r3
	bne .L_0200b066
	bl Func_020030e4
	b .L_0200b0a0
.L_0200b066:
	ldr r3, .L_0200b0ac
	cmp r2, r3
	bne .L_0200b072
	bl Func_02003164
	b .L_0200b0a0
.L_0200b072:
	ldr r3, .L_0200b0b0
	cmp r2, r3
	bne .L_0200b07e
	bl Func_02003284
	b .L_0200b0a0
.L_0200b07e:
	ldr r3, .L_0200b0b4
	cmp r2, r3
	bne .L_0200b08a
	bl Func_0200369c
	b .L_0200b0a0
.L_0200b08a:
	ldr r3, .L_0200b0b8
	cmp r2, r3
	bne .L_0200b096
	bl Func_0200390c
	b .L_0200b0a0
.L_0200b096:
	ldr r3, .L_0200b0bc
	cmp r2, r3
	bne .L_0200b0a0
	bl Func_02003aa0
.L_0200b0a0:
	movs r0, #0
	pop {pc}
.L_0200b0a4:
	.4byte gPartyState
.L_0200b0a8:
	.4byte 0x00000022
.L_0200b0ac:
	.4byte 0x00000023
.L_0200b0b0:
	.4byte 0x00000024
.L_0200b0b4:
	.4byte 0x00000025
.L_0200b0b8:
	.4byte 0x00000026
.L_0200b0bc:
	.4byte 0x00000027
	.section .text.x0200b0c0,"ax",%progbits
	.global Func_020030c0
	.thumb_func
Func_020030c0:
	push {lr}
	ldr r3, .L_0200b0e0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #80]
	movs r0, #64
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl ObjectMotion_SetActionVariant
	pop {pc}
.L_0200b0e0:
	.4byte gPartyState
	.section .text.x0200b0e4,"ax",%progbits
	.global Func_020030e4
	.thumb_func
Func_020030e4:
	push {lr}
	ldr r0, .L_0200b158
	sub sp, #8
	bl Func_020001d0
	movs r1, #3
	movs r0, #8
	bl ObjectMotion_SetActionVariant
	ldr r0, .L_0200b15c
	bl Func_02000038
	movs r0, #12
	bl Func_02003cc0
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b140
	movs r3, #56
	movs r2, #64
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #32
	movs r2, #2
	movs r3, #1
	movs r0, #54
	bl Func_02003bf0
	movs r0, #12
	bl Object_GetById
	movs r1, #226
	movs r2, #128
	movs r3, #130
	lsls r1, r1, #18
	lsls r2, r2, #15
	lsls r3, r3, #18
	bl Object_SetPositionAndResetMotion
	movs r0, #10
	bl WaitFrames
.L_0200b140:
	bl Func_02001820
	bl Func_02001f94
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200b160
	bl Scheduler_AddOrUpdateCallback
	add sp, #8
	pop {pc}
	.2byte 0x0000
.L_0200b158:
	.4byte Data_02003f44
.L_0200b15c:
	.4byte Data_02003f98
.L_0200b160:
	.4byte Func_020030c0
	.section .text.x0200b164,"ax",%progbits
	.global Func_02003164
	.thumb_func
Func_02003164:
	push {r5, r6, lr}
	movs r0, #10
	adds r0, #255
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b1b2
	ldr r5, .L_0200b274
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r5, r2
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	movs r2, #241
	lsls r2, r2, #1
	strb r3, [r0]
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #5
	bne .L_0200b1b2
	ldr r0, [r6]
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
.L_0200b1b2:
	ldr r0, .L_0200b278
	bl Func_02000584
	bl Func_020012d8
	ldr r0, .L_0200b27c
	bl Func_02000038
	movs r0, #132
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b1fe
	movs r0, #23
	bl Func_02003cc0
	movs r3, #52
	movs r2, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #7
	movs r2, #1
	movs r3, #1
	movs r0, #50
	bl Func_02003bf0
	movs r0, #23
	bl Object_GetById
	movs r1, #210
	movs r3, #240
	lsls r1, r1, #18
	movs r2, #0
	lsls r3, r3, #15
	bl Object_SetPositionAndResetMotion
	b .L_0200b204
.L_0200b1fe:
	ldr r0, .L_0200b280
	bl Func_02003db8
.L_0200b204:
	bl Func_020018a8
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #108
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b254
	movs r3, #128
	movs r1, #222
	movs r2, #168
	lsls r3, r3, #7
	movs r0, #24
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02003c70
	movs r5, #1
	movs r0, #68
	movs r1, #33
	movs r2, #105
	movs r3, #6
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003be0
	movs r0, #69
	movs r1, #33
	movs r2, #105
	movs r3, #4
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003be0
	movs r0, #164
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
.L_0200b254:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #109
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b26c
	movs r0, #24
	movs r1, #0
	movs r2, #0
	bl Func_02003c68
.L_0200b26c:
	bl Func_02001fac
	add sp, #8
	pop {r5, r6, pc}
.L_0200b274:
	.4byte gPartyState
.L_0200b278:
	.4byte Data_02003f52
.L_0200b27c:
	.4byte Data_02003fa0
.L_0200b280:
	.4byte Data_02003fc0
	.section .text.x0200b284,"ax",%progbits
	.global Func_02003284
	.thumb_func
Func_02003284:
	push {r5, r6, r7, lr}
	movs r0, #10
	adds r0, #255
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b2c4
	ldr r5, .L_0200b51c
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #2
	ldr r0, [r5]
	bl ObjectMotion_SetActionVariant
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
.L_0200b2c4:
	movs r0, #8
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #9
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #10
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #11
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	bl Func_02003d98
	movs r1, #8
	movs r2, #9
	movs r0, #0
	bl Func_02003da0
	movs r2, #11
	movs r1, #10
	movs r0, #1
	bl Func_02003da0
	movs r0, #12
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #13
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	bl Func_02003d90
	movs r1, #130
	movs r2, #12
	movs r3, #13
	lsls r1, r1, #2
	movs r0, #0
	bl Func_02003da8
	ldr r0, .L_0200b520
	bl Func_02003db8
	movs r0, #19
	bl Object_GetById
	ldr r2, .L_0200b524
	ldr r3, [r0, #12]
	cmp r3, r2
	ble .L_0200b33c
	movs r0, #19
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	b .L_0200b344
.L_0200b33c:
	movs r0, #19
	movs r1, #2
	bl ObjectMotion_SetActionVariant
.L_0200b344:
	movs r0, #20
	bl Func_02003cc0
	movs r0, #20
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #20
	bl ObjectMotion_SetActionVariant
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #65
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b376
	bl Func_0200172c
	b .L_0200b38a
.L_0200b376:
	movs r3, #38
	movs r2, #33
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #32
	movs r2, #1
	movs r3, #1
	bl Func_02003bf0
.L_0200b38a:
	movs r0, #14
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r6, #254
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #15
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #16
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #17
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	ldr r0, .L_0200b528
	bl Func_02000c64
	movs r0, #17
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r7, #0
	asrs r3, r3, #20
	cmp r3, #54
	bne .L_0200b404
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #156
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r5, [r3]
	bl Func_02003db0
	lsls r0, r0, #2
	adds r5, r5, r0
	ldrb r2, [r5, #3]
	movs r3, #127
	ands r3, r2
	strb r3, [r5, #3]
	movs r3, #11
	strb r3, [r5, #2]
.L_0200b404:
	movs r0, #14
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #15
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	bl Func_02001930
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #110
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b438
	movs r3, #128
	movs r1, #212
	movs r2, #208
	lsls r3, r3, #7
	movs r0, #25
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl Func_02003c70
.L_0200b438:
	movs r0, #18
	movs r1, #3
	bl Object_SetModeById
	movs r0, #238
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b4c0
	movs r0, #18
	bl Object_GetById
	movs r3, #27
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r5, r0, #0
	movs r2, #1
	movs r3, #1
	movs r0, #27
	movs r1, #8
	bl Func_02003bf0
	movs r0, #18
	movs r1, #4
	bl Object_SetModeById
	adds r1, r5, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #2
	orrs r3, r2
	strb r3, [r1]
	adds r5, #89
	ldrb r2, [r5]
	adds r3, r6, #0
	movs r0, #131
	ands r3, r2
	lsls r0, r0, #1
	strb r3, [r5]
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #161
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #69
	movs r1, #51
	movs r2, #92
	movs r3, #11
	bl Func_02003be0
	movs r3, #128
	movs r1, #154
	movs r2, #140
	lsls r3, r3, #7
	movs r0, #25
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02003c70
.L_0200b4c0:
	movs r0, #135
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b4e8
	movs r0, #132
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #22
	bl GameFlag_SetBit
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl Func_02003c68
.L_0200b4e8:
	ldr r0, .L_0200b52c
	bl Func_020001d0
	bl Func_020013b0
	bl Func_02001fc4
	movs r0, #27
	bl Object_GetById
	adds r3, r0, #0
	adds r3, #85
	strb r7, [r3]
	ldr r3, .L_0200b530
	str r7, [r0, #12]
	str r7, [r3]
	ldr r3, .L_0200b534
	movs r1, #144
	str r7, [r3]
	lsls r1, r1, #3
	ldr r0, .L_0200b538
	bl Scheduler_AddOrUpdateCallback
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b51c:
	.4byte gPartyState
.L_0200b520:
	.4byte Data_02003fc4
.L_0200b524:
	.4byte 0xfff80000
.L_0200b528:
	.4byte Data_02003fc8
.L_0200b52c:
	.4byte Data_02003f80
.L_0200b530:
	.4byte Data_0200567c
.L_0200b534:
	.4byte Data_02005678
.L_0200b538:
	.4byte Func_02001ee8
	.section .text.x0200b53c,"ax",%progbits
	.global Func_0200353c
	.thumb_func
Func_0200353c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r5, #200
	sub sp, #8
	lsls r5, r5, #2
	movs r7, #0
.L_0200b54c:
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b5ca
	ldr r6, .L_0200b5e4
	ldr r2, .L_0200b5e8
	adds r6, r7, r6
	ldrh r3, [r6]
	adds r6, #2
	ldrh r7, [r6]
	mov r8, r3
	adds r5, r5, r2
	movs r2, #1
	adds r3, r7, #0
	mov r10, r2
	str r2, [sp, #0]
	str r2, [sp, #4]
	movs r0, #3
	mov r2, r8
	movs r1, #8
	bl Func_02003be0
	adds r6, #2
	ldrh r3, [r6]
	ldrh r7, [r6, #2]
	mov r8, r3
	mov r2, r10
	movs r3, #5
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #67
	movs r1, #3
	mov r2, r8
	adds r3, r7, #0
	bl Func_02003be0
	cmp r5, #1
	bne .L_0200b5ac
	mov r3, r10
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #67
	movs r1, #4
	movs r2, #69
	movs r3, #3
	bl Func_02003be0
.L_0200b5ac:
	movs r2, #64
	negs r2, r2
	add r8, r2
	movs r3, #4
	mov r2, r10
	adds r7, #66
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #3
	movs r1, #69
	mov r2, r8
	adds r3, r7, #0
	bl Func_02003be0
	b .L_0200b5da
.L_0200b5ca:
	movs r3, #152
	lsls r3, r3, #2
	adds r3, #255
	cmp r5, r3
	beq .L_0200b5da
	adds r7, #8
	adds r5, #1
	b .L_0200b54c
.L_0200b5da:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200b5e4:
	.4byte Data_02004276
.L_0200b5e8:
	.4byte 0xfffffce0
	.section .text.x0200b5ec,"ax",%progbits
	.global Func_020035ec
	.thumb_func
Func_020035ec:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r5, #200
	sub sp, #8
	lsls r5, r5, #2
	movs r7, #0
.L_0200b5fc:
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b67a
	ldr r6, .L_0200b694
	ldr r2, .L_0200b698
	adds r6, r7, r6
	ldrh r3, [r6]
	adds r6, #2
	ldrh r7, [r6]
	mov r8, r3
	adds r5, r5, r2
	movs r2, #1
	adds r3, r7, #0
	mov r10, r2
	str r2, [sp, #0]
	str r2, [sp, #4]
	movs r0, #13
	mov r2, r8
	movs r1, #5
	bl Func_02003be0
	adds r6, #2
	ldrh r3, [r6]
	ldrh r7, [r6, #2]
	mov r8, r3
	mov r2, r10
	movs r3, #5
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #77
	movs r1, #0
	mov r2, r8
	adds r3, r7, #0
	bl Func_02003be0
	cmp r5, #1
	bne .L_0200b65c
	mov r3, r10
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #67
	movs r1, #4
	movs r2, #69
	movs r3, #3
	bl Func_02003be0
.L_0200b65c:
	movs r2, #64
	negs r2, r2
	add r8, r2
	movs r3, #4
	mov r2, r10
	adds r7, #65
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #13
	movs r1, #65
	mov r2, r8
	adds r3, r7, #0
	bl Func_02003be0
	b .L_0200b68a
.L_0200b67a:
	movs r3, #152
	lsls r3, r3, #2
	adds r3, #255
	cmp r5, r3
	beq .L_0200b68a
	adds r7, #8
	adds r5, #1
	b .L_0200b5fc
.L_0200b68a:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200b694:
	.4byte Data_02004276
.L_0200b698:
	.4byte 0xfffffce0
	.section .text.x0200b69c,"ax",%progbits
	.global Func_0200369c
	.thumb_func
Func_0200369c:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r6, .L_0200b760
	adds r2, #88
	str r2, [r3]
	adds r2, #16
	adds r3, r6, r2
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	bl Func_02001fdc
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #108
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b70e
	movs r5, #1
	movs r0, #3
	movs r1, #8
	movs r2, #30
	movs r3, #33
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003be0
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #67
	movs r1, #3
	movs r2, #94
	movs r3, #28
	str r5, [sp, #0]
	bl Func_02003be0
	movs r3, #4
	str r3, [sp, #4]
	movs r0, #3
	movs r1, #69
	movs r2, #30
	movs r3, #93
	str r5, [sp, #0]
	bl Func_02003be0
.L_0200b70e:
	movs r3, #241
	lsls r3, r3, #1
	adds r6, r6, r3
	ldrh r5, [r6]
	movs r2, #224
	adds r3, r5, #0
	subs r3, #11
	lsls r3, r3, #16
	lsls r2, r2, #13
	cmp r3, r2
	bhi .L_0200b73a
	bl Func_02001804
	movs r3, #192
	lsls r5, r5, #16
	lsls r3, r3, #2
	asrs r5, r5, #16
	adds r3, #21
	adds r5, r5, r3
	adds r0, r5, #0
	bl GameFlag_SetBit
.L_0200b73a:
	bl Func_0200353c
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b75a
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #10
	ble .L_0200b75a
	cmp r3, #39
	bgt .L_0200b75a
	bl Func_02003764
.L_0200b75a:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b760:
	.4byte gPartyState
	.section .text.x0200b764,"ax",%progbits
	.global Func_02003764
	.thumb_func
Func_02003764:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200b8f8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02003c28
	movs r0, #0
	bl Func_02003d78
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02003bc8
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
	bl Func_02003df8
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_0200b8fc
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_0200b7fe:
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
	ldr r3, .L_0200b900
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_0200b904
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
	ldr r4, .L_0200b908
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_02000fc4
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200b7fe
	movs r0, #188
	bl Func_02003df8
	ldr r5, .L_0200b8f8
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02003cd0
	ldr r0, [r5]
	movs r1, #49
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02003c00
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02003c00
	bl Func_02003c08
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02003cd0
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
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	bl Func_02003c30
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200b8f8:
	.4byte gPartyState
.L_0200b8fc:
	.4byte Func_02002640
.L_0200b900:
	.4byte 0xffffa000
.L_0200b904:
	.4byte 0xffffd000
.L_0200b908:
	.4byte 0x01090001
	.section .text.x0200b90c,"ax",%progbits
	.global Func_0200390c
	.thumb_func
Func_0200390c:
	push {r5, r6, lr}
	ldr r3, .L_0200ba18
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #120
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b988
	bl Func_02003a30
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #113
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b988
	movs r3, #15
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r3, #1
	movs r1, #0
	movs r2, #1
	bl Func_02003bf0
	ldr r3, .L_0200ba1c
	ldr r0, [r3]
	cmp r0, #0
	beq .L_0200b988
	movs r1, #248
	movs r3, #176
	lsls r1, r1, #16
	movs r2, #0
	lsls r3, r3, #15
	bl Object_SetPositionAndResetMotion
	movs r0, #1
	bl WaitFrames
.L_0200b988:
	ldr r0, .L_0200ba20
	bl Func_02000038
	bl Func_020019b8
	ldr r3, .L_0200ba18
	movs r2, #241
	lsls r2, r2, #1
	adds r6, r3, r2
	movs r3, #0
	ldrsh r5, [r6, r3]
	cmp r5, #10
	ble .L_0200b9b2
	bl Func_02001804
	movs r2, #192
	lsls r2, r2, #2
	adds r2, #21
	adds r0, r5, r2
	bl GameFlag_SetBit
.L_0200b9b2:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #113
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b9ca
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02003c68
.L_0200b9ca:
	bl Func_020035ec
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b9fe
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #16
	ble .L_0200b9fe
	cmp r3, #37
	bne .L_0200b9fa
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #113
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b9fa
	bl Func_02002c14
	b .L_0200b9fe
.L_0200b9fa:
	bl Func_02003764
.L_0200b9fe:
	ldr r3, .L_0200ba24
	movs r2, #0
	str r2, [r3]
	ldr r3, .L_0200ba28
	movs r1, #144
	str r2, [r3]
	lsls r1, r1, #3
	ldr r0, .L_0200ba2c
	bl Scheduler_AddOrUpdateCallback
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200ba18:
	.4byte gPartyState
.L_0200ba1c:
	.4byte Data_02004420
.L_0200ba20:
	.4byte Data_02003fb0
.L_0200ba24:
	.4byte Data_0200567c
.L_0200ba28:
	.4byte Data_02005678
.L_0200ba2c:
	.4byte Func_02001ee8
	.section .text.x0200ba30,"ax",%progbits
	.global Func_02003a30
	.thumb_func
Func_02003a30:
	push {r5, r6, r7, lr}
	movs r0, #234
	adds r0, #255
	movs r1, #0
	movs r2, #0
	movs r3, #0
	ldr r5, .L_0200ba9c
	bl Func_02003bb0
	movs r7, #0
	str r0, [r5]
	cmp r0, #0
	beq .L_0200ba98
	ldr r6, [r0, #80]
	movs r3, #33
	ldrb r2, [r6, #5]
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	strb r3, [r6, #9]
	adds r3, r0, #0
	adds r3, #85
	adds r2, r0, #0
	adds r2, #92
	strb r7, [r3]
	movs r1, #193
	movs r3, #1
	strb r3, [r2]
	lsls r1, r1, #3
	strb r7, [r6, #26]
	strb r7, [r6, #27]
	movs r0, #68
	bl Runtime_AllocateHeapBlockFar
	adds r5, r0, #0
	movs r0, #199
	bl Func_02003c18
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	ldrb r0, [r6, #16]
	movs r1, #128
	adds r2, r5, #0
	bl VramBlock_LoadCached
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
.L_0200ba98:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ba9c:
	.4byte Data_02004420
	.section .text.x0200baa0,"ax",%progbits
	.global Func_02003aa0
	.thumb_func
Func_02003aa0:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r5, .L_0200bb28
	adds r2, #88
	str r2, [r3]
	adds r2, #16
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r0, #95
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200badc
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
.L_0200badc:
	ldr r0, .L_0200bb2c
	bl Func_02000038
	ldr r0, .L_0200bb30
	bl Func_020001d0
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200bb00
	bl Func_02002140
	movs r0, #10
	bl WaitFrames
.L_0200bb00:
	bl Func_02001ff4
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200bb26
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #5
	ble .L_0200bb26
	cmp r3, #39
	bgt .L_0200bb26
	bl Func_02003764
.L_0200bb26:
	pop {r5, pc}
.L_0200bb28:
	.4byte gPartyState
.L_0200bb2c:
	.4byte Data_02003fb8
.L_0200bb30:
	.4byte Data_02003f92
	.section .rodata.x0200be00,"a",%progbits
	.global Data_02003e00
Data_02003e00:
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
	.global Data_02003e30
Data_02003e30:
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
.L_0200be60:
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
.L_0200be9c:
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
.L_0200bed8:
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
	.global Data_02003f14
Data_02003f14:
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_02003f38
Data_02003f38:
	.4byte .L_0200be60
	.4byte .L_0200be9c
	.4byte .L_0200bed8
	.global Data_02003f44
Data_02003f44:
	.4byte 0x02100008
	.4byte 0x02120009
	.4byte 0x0214000a
	.2byte 0xffff
	.global Data_02003f52
Data_02003f52:
	.2byte 0x000b
	.4byte 0x000c0210
	.4byte 0x000d0212
	.4byte 0x000e0214
	.4byte 0x000f0216
	.4byte 0x00100218
	.4byte 0x0011021a
	.4byte 0x0012021c
	.4byte 0x0013021e
	.4byte 0x00140220
	.4byte 0x00150222
	.4byte 0xffff0224
	.global Data_02003f80
Data_02003f80:
	.4byte 0x02100015
	.4byte 0x02120016
	.4byte 0x02140017
	.4byte 0x02160018
	.2byte 0xffff
	.global Data_02003f92
Data_02003f92:
	.2byte 0x0009
	.4byte 0xffff0210
	.global Data_02003f98
Data_02003f98:
	.4byte 0x0001000b
	.4byte 0xffff0203
	.global Data_02003fa0
Data_02003fa0:
	.4byte 0x00010016
	.4byte 0xffff0204
	.global Data_02003fa8
Data_02003fa8:
	.4byte 0x00010012
	.4byte 0xffff0205
	.global Data_02003fb0
Data_02003fb0:
	.4byte 0x00010008
	.4byte 0xffff0206
	.global Data_02003fb8
Data_02003fb8:
	.4byte 0x00010008
	.4byte 0xffff0207
	.global Data_02003fc0
Data_02003fc0:
	.4byte 0xffff0017
	.global Data_02003fc4
Data_02003fc4:
	.4byte 0xffff0013
	.global Data_02003fc8
Data_02003fc8:
	.4byte 0x000f000e
	.4byte 0x00110010
	.2byte 0xffff
	.global Data_02003fd2
Data_02003fd2:
	.2byte 0x0004
	.4byte 0x00050021
	.2byte 0x0021
	.global Data_02003fda
Data_02003fda:
	.2byte 0x0044
	.4byte 0x00450021
	.2byte 0x0021
	.global Data_02003fe2
Data_02003fe2:
	.2byte 0x0045
	.4byte 0x00460033
	.2byte 0x0033
	.global Data_02003fea
Data_02003fea:
	.2byte 0x0041
	.4byte 0x00400040
	.2byte 0x0040
	.global Data_02003ff2
Data_02003ff2:
	.2byte 0x0001
	.4byte 0x00020024
	.4byte 0x00030024
	.4byte 0x00040024
	.4byte 0x00050024
	.4byte 0x00060024
	.4byte 0x00070024
	.4byte 0x00080024
	.2byte 0x0024
	.global Data_02004012
Data_02004012:
	.2byte 0x0041
	.4byte 0x00420024
	.4byte 0x00430024
	.4byte 0x00440024
	.4byte 0x00450024
	.4byte 0x00460024
	.4byte 0x00470024
	.4byte 0x00480024
	.2byte 0x0024
	.global Data_02004032
Data_02004032:
	.2byte 0x0042
	.4byte 0x00430036
	.4byte 0x00440036
	.4byte 0x00450036
	.4byte 0x00460036
	.4byte 0x00470036
	.4byte 0x00480036
	.4byte 0x00490036
	.2byte 0x0036
	.global Data_02004052
Data_02004052:
	.2byte 0x0040
	.4byte 0x00410042
	.4byte 0x00420042
	.4byte 0x00430042
	.4byte 0x00440042
	.2byte 0x0042
	.global Data_02004066
Data_02004066:
	.2byte 0x0000
	.global Data_02004068
Data_02004068:
	.4byte 0x000c004c
	.4byte 0x004c0000
	.4byte 0x00000030
	.4byte 0x000d004d
	.4byte 0x004d0000
	.4byte 0x00010031
	.4byte 0x000c004f
	.4byte 0x004f0000
	.4byte 0x00000030
	.4byte 0x000e004f
	.4byte 0x004f0001
	.4byte 0x00000032
	.4byte 0x000c0051
	.4byte 0x00510000
	.4byte 0x00000030
	.4byte 0x00110076
	.4byte 0x00760004
	.4byte 0x00010035
	.4byte 0x00100077
	.4byte 0x00770000
	.4byte 0x00000034
	.4byte 0x00120077
	.4byte 0x00770007
	.4byte 0x00010036
	.4byte 0x000c0078
	.4byte 0x00780007
	.4byte 0x00000030
	.4byte 0x000e0078
	.4byte 0x00780007
	.4byte 0x00000032
	.4byte 0x00110078
	.4byte 0x00780002
	.4byte 0x00000035
	.4byte 0x000c007a
	.4byte 0x007a0007
	.4byte 0x00010030
	.4byte 0x0010007a
	.4byte 0x007a0000
	.4byte 0x00000034
	.4byte 0x0012007a
	.4byte 0x007a0005
	.2byte 0x0036
	.global Data_0200410e
Data_0200410e:
	.2byte 0x0001
	.global Data_02004110
Data_02004110:
	.4byte 0x00040069
	.4byte 0x00290000
	.4byte 0x00000024
	.4byte 0x0011006d
	.4byte 0x002d0000
	.4byte 0x00000031
	.4byte 0x0013006e
	.4byte 0x002e0003
	.4byte 0x00000033
	.4byte 0x0011006f
	.4byte 0x002f0003
	.4byte 0x00000031
	.4byte 0x00150070
	.4byte 0x00300000
	.4byte 0x00000035
	.4byte 0x00140071
	.4byte 0x00310003
	.4byte 0x00000034
	.4byte 0x00120072
	.4byte 0x00320000
	.4byte 0x00000032
	.4byte 0x00060069
	.4byte 0x00290000
	.2byte 0x0026
	.global Data_0200416e
Data_0200416e:
	.2byte 0x0001
	.global Data_02004170
Data_02004170:
	.4byte 0x000b005b
	.4byte 0x001b0000
	.4byte 0x0000002b
	.4byte 0x000b005c
	.4byte 0x001c0000
	.4byte 0x0001002b
	.4byte 0x0009005d
	.4byte 0x001d0000
	.4byte 0x00010029
	.4byte 0x000c005d
	.4byte 0x001d0000
	.4byte 0x0001002c
	.4byte 0x0007006d
	.4byte 0x002d0000
	.4byte 0x00010027
	.4byte 0x00060070
	.4byte 0x00300000
	.2byte 0x0026
	.global Data_020041b6
Data_020041b6:
	.2byte 0x0001
	.global Data_020041b8
Data_020041b8:
	.4byte 0x0008000d
	.4byte 0x000d0002
	.4byte 0x00000048
	.4byte 0x0009000d
	.4byte 0x000d0000
	.4byte 0x00000049
	.4byte 0x000b000d
	.4byte 0x000d0001
	.4byte 0x0000004b
	.4byte 0x0006000d
	.4byte 0x000d0000
	.4byte 0x00010046
	.4byte 0x0006000f
	.4byte 0x000f0000
	.4byte 0x00000046
	.4byte 0x0008000f
	.4byte 0x000f0000
	.4byte 0x00000048
	.4byte 0x000a000f
	.4byte 0x000f0001
	.4byte 0x0001004a
	.4byte 0x000a000e
	.4byte 0x000e0000
	.4byte 0x0000004a
	.4byte 0x000c000e
	.4byte 0x000e0000
	.4byte 0x0001004c
	.4byte 0x000c000f
	.4byte 0x000f0000
	.4byte 0x0000004c
	.4byte 0x00060010
	.4byte 0x00100000
	.4byte 0x00010046
	.4byte 0x00080010
	.4byte 0x00100002
	.4byte 0x00000048
	.4byte 0x00090010
	.4byte 0x00100000
	.4byte 0x00000049
	.4byte 0x000b0010
	.4byte 0x00100000
	.4byte 0x0001004b
	.4byte 0x00090012
	.4byte 0x00120000
	.4byte 0x00000049
	.4byte 0x000b0012
	.4byte 0x00120000
	.2byte 0x004b
	.global Data_02004276
Data_02004276:
	.2byte 0x0004
	.4byte 0x00440007
	.4byte 0x00050002
	.4byte 0x00450008
	.4byte 0x00070003
	.4byte 0x00470007
	.4byte 0x00070002
	.4byte 0x00470009
	.4byte 0x00090004
	.4byte 0x00490007
	.4byte 0x00280002
	.4byte 0x0068000c
	.4byte 0x00290007
	.4byte 0x0069000b
	.4byte 0x00290006
	.4byte 0x0069000d
	.4byte 0x002a0008
	.4byte 0x006a0005
	.4byte 0x002a0000
	.4byte 0x006a0009
	.4byte 0x002a0004
	.4byte 0x006a000c
	.4byte 0x002c0007
	.4byte 0x006c0007
	.4byte 0x002c0002
	.4byte 0x006c000b
	.4byte 0x002c0006
	.4byte 0x006c000d
	.4byte 0x000b0008
	.4byte 0x004b0017
	.4byte 0x000c0012
	.4byte 0x004c0019
	.4byte 0x000d0014
	.4byte 0x004d0017
	.4byte 0x000e0012
	.4byte 0x004e001b
	.4byte 0x000f0016
	.4byte 0x004f001a
	.4byte 0x00100015
	.4byte 0x00500018
	.4byte 0x001e0013
	.4byte 0x005e0023
	.4byte 0x001a001e
	.4byte 0x005a0009
	.4byte 0x001b0004
	.4byte 0x005b0009
	.4byte 0x001c0004
	.4byte 0x005c0007
	.4byte 0x001c0002
	.4byte 0x005c000a
	.4byte 0x000c0005
	.4byte 0x004c0008
	.4byte 0x000f0003
	.4byte 0x004f0007
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x001e0000
	.4byte 0x005e0021
	.4byte 0x0000001c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004420
Data_02004420:
	.4byte 0x00000000
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000002f0
	.4byte 0x4000015c
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
	.4byte 0x00000022
	.4byte 0x00106002
	.4byte 0x00201025
	.4byte 0x00302025
	.4byte 0x00404025
	.4byte 0x00501023
	.4byte 0x00607022
	.4byte 0x00706022
	.4byte 0x00b0b025
	.4byte 0x00c0c025
	.4byte 0x00d0d025
	.4byte 0x00e0e025
	.4byte 0x00f0f025
	.4byte 0x01010025
	.4byte 0x01111025
	.4byte 0x01212025
	.4byte 0x01313025
	.4byte 0x01414025
	.4byte 0x01515025
	.4byte 0x01616025
	.4byte 0x01717025
	.4byte 0x01818025
	.4byte 0x00000023
	.4byte 0x00105022
	.4byte 0x00206025
	.4byte 0x00305025
	.4byte 0x00401024
	.4byte 0x00506023
	.4byte 0x00605023
	.4byte 0x00f19025
	.4byte 0x0101a025
	.4byte 0x0111b025
	.4byte 0x0121c025
	.4byte 0x0131d025
	.4byte 0x0141e025
	.4byte 0x0151f025
	.4byte 0x00e3c025
	.4byte 0x00000024
	.4byte 0x00104023
	.4byte 0x00203025
	.4byte 0x00301026
	.4byte 0x00404026
	.4byte 0x00505026
	.4byte 0x00607002
	.4byte 0x00708024
	.4byte 0x00807024
	.4byte 0x00c20025
	.4byte 0x00d21025
	.4byte 0x00e22025
	.4byte 0x00f23025
	.4byte 0x01024026
	.4byte 0x01125026
	.4byte 0x00000025
	.4byte 0x00102022
	.4byte 0x00203022
	.4byte 0x00302024
	.4byte 0x00404022
	.4byte 0x00503023
	.4byte 0x00602023
	.4byte 0x0282a025
	.4byte 0x0292b025
	.4byte 0x02a28025
	.4byte 0x02b29025
	.4byte 0x00000026
	.4byte 0x00103024
	.4byte 0x00201027
	.4byte 0x00302027
	.4byte 0x00404024
	.4byte 0x00505024
	.4byte 0x00603027
	.4byte 0x0070a026
	.4byte 0x00809026
	.4byte 0x00908026
	.4byte 0x00a07026
	.4byte 0x01208027
	.4byte 0x01309027
	.4byte 0x0140a027
	.4byte 0x0150b027
	.4byte 0x0160c027
	.4byte 0x0170d027
	.4byte 0x0180e027
	.4byte 0x0190f027
	.4byte 0x01a10027
	.4byte 0x01b11027
	.4byte 0x01c12027
	.4byte 0x01d13027
	.4byte 0x01e14027
	.4byte 0x01f15027
	.4byte 0x02016027
	.4byte 0x02117027
	.4byte 0x00000027
	.4byte 0x00102026
	.4byte 0x00203026
	.4byte 0x00306026
	.4byte 0x02829027
	.4byte 0x02928027
	.4byte 0x02a2b027
	.4byte 0x02b2a027
	.4byte 0x000001ff
	.global Data_020045e0
Data_020045e0:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020045f8
Data_020045f8:
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x03980000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02900000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020047d8
Data_020047d8:
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x000a0000
	.4byte 0x00000000
	.4byte 0x000a0000
	.4byte 0x00024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x000a0000
	.4byte 0x00000000
	.4byte 0x000a0000
	.4byte 0x00024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x000a0000
	.4byte 0x00000000
	.4byte 0x000a0000
	.4byte 0x00024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff00f5
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004ac0
Data_02004ac0:
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff00fa
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff00fa
	.4byte 0x00000001
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff00fa
	.4byte 0x00000001
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff00fa
	.4byte 0x00000001
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x0000000e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x0000000f
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff00f5
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03980000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004d78
Data_02004d78:
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004dd8
Data_02004dd8:
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00024000
	.4byte 0xffff00f5
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004e20
Data_02004e20:
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff0199
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01024000
	.4byte 0x005f00f5
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004f10
Data_02004f10:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004f1c
Data_02004f1c:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte Func_02002ff4
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff001e
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff001f
	.4byte 0x00000007
	.4byte 0x00000202
	.4byte 0xffff0009
	.4byte Func_020012c8
	.4byte 0x50008615
	.4byte 0x0203000b
	.4byte Func_02001554
	.4byte 0x00000009
	.4byte 0x03010000
	.4byte Func_020015e0
	.4byte 0x00008c15
	.4byte 0x0301000c
	.4byte Func_020015e0
	.4byte 0x00000002
	.4byte 0x02000006
	.4byte Func_020014ec
	.4byte 0x00000002
	.4byte 0x12000007
	.4byte Func_02001500
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte Func_020017f4
	.4byte 0x00000002
	.4byte 0x0201000b
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x0201000c
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x0201000d
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x0201000e
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x0201000f
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010010
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010011
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010012
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010013
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010014
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010015
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010016
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010017
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010018
	.4byte Func_02001a30
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005078
Data_02005078:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte Func_02002ff4
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x00000006
	.4byte 0x00000002
	.4byte 0x086c001e
	.4byte Func_020021e8
	.4byte 0x00000002
	.4byte 0x086d001f
	.4byte Func_020022e0
	.4byte 0x00008602
	.4byte 0xffff0008
	.4byte Func_02001368
	.4byte 0x00000202
	.4byte 0xffff0009
	.4byte Func_02001368
	.4byte 0x50008615
	.4byte 0x02040016
	.4byte Func_02001568
	.4byte 0x00000008
	.4byte 0x08400000
	.4byte Func_02001630
	.4byte 0x00000009
	.4byte 0x08400000
	.4byte Func_02001640
	.4byte 0x10008c15
	.4byte 0x08400017
	.4byte Func_02001630
	.4byte 0x00008c15
	.4byte 0x08400017
	.4byte Func_02001640
	.4byte 0x00000002
	.4byte 0x02000006
	.4byte Func_020014ec
	.4byte 0x00000002
	.4byte 0x12000007
	.4byte Func_02001500
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte Func_020017f4
	.4byte 0x00000002
	.4byte 0x0201000e
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x0201000f
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010010
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010011
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010012
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010013
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010014
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010015
	.4byte Func_02001a30
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020051bc
Data_020051bc:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0018
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0019
	.4byte 0x00000008
	.4byte 0x00000002
	.4byte 0x086e001e
	.4byte Func_02002390
	.4byte 0x00000002
	.4byte 0x086f001f
	.4byte Func_02002458
	.4byte 0x00000002
	.4byte 0x08700020
	.4byte Func_020027e0
	.4byte 0x00008515
	.4byte 0x0208000c
	.4byte 0x00000000
	.4byte 0x50008615
	.4byte 0x02050012
	.4byte Func_0200157c
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_020016a0
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_020016b0
	.4byte 0x10008c15
	.4byte 0xffff0013
	.4byte Func_020016a0
	.4byte 0x00008c15
	.4byte 0xffff0013
	.4byte Func_020016b0
	.4byte 0x00008c15
	.4byte 0x08410014
	.4byte Func_02001788
	.4byte 0x00008602
	.4byte 0x08410009
	.4byte Func_020017a0
	.4byte 0x00000602
	.4byte 0xffff0014
	.4byte Func_020020dc
	.4byte 0x00008602
	.4byte 0xffff0015
	.4byte Func_020020dc
	.4byte 0x10008c15
	.4byte 0xffff000e
	.4byte Func_02002114
	.4byte 0x10008c15
	.4byte 0xffff000f
	.4byte Func_02002114
	.4byte 0x10008c15
	.4byte 0xffff0010
	.4byte Func_02002114
	.4byte 0x10008c15
	.4byte 0xffff0011
	.4byte Func_02002114
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte Func_02002124
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte Func_02002124
	.4byte 0x00008c15
	.4byte 0xffff0010
	.4byte Func_02002124
	.4byte 0x00008c15
	.4byte 0xffff0011
	.4byte Func_02002124
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte Func_020013fc
	.4byte 0x00008602
	.4byte 0xffff000b
	.4byte Func_020013fc
	.4byte 0x00000602
	.4byte 0xffff001a
	.4byte Func_020013fc
	.4byte 0x00004602
	.4byte 0xffff001b
	.4byte Func_020013fc
	.4byte 0x00000002
	.4byte 0x02000007
	.4byte Func_020014ec
	.4byte 0x00000002
	.4byte 0x12000008
	.4byte Func_02001500
	.4byte 0x00000002
	.4byte 0x0201000c
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x0201000d
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x0201000e
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x0201000f
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010010
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010011
	.4byte Func_02001a30
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020053b4
Data_020053b4:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000021
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000021
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0028
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0xffff0029
	.4byte 0x00000029
	.4byte 0x00000001
	.4byte 0xffff002a
	.4byte 0x0000002a
	.4byte 0x00000001
	.4byte 0xffff002b
	.4byte 0x0000002b
	.4byte 0x00000002
	.4byte 0x02000008
	.4byte Func_020014ec
	.4byte 0x00000002
	.4byte 0x12000009
	.4byte Func_02001500
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005450
Data_02005450:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000021
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000021
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000031
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
	.4byte 0x00000002
	.4byte 0x08710017
	.4byte Func_02002a5c
	.4byte 0x50008615
	.4byte 0x02060008
	.4byte Func_02001590
	.4byte 0x00000003
	.4byte 0x0878000c
	.4byte Func_02002f9c
	.4byte 0x00000002
	.4byte 0x02010012
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010013
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010014
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010015
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010016
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010017
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010018
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010019
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x0201001a
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x0201001b
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x0201001c
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x0201001d
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x0201001e
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x0201001f
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010020
	.4byte Func_02001a30
	.4byte 0x00000002
	.4byte 0x02010021
	.4byte Func_02001a30
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020055b8
Data_020055b8:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0028
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0xffff0029
	.4byte 0x00000029
	.4byte 0x00000001
	.4byte 0xffff002a
	.4byte 0x0000002a
	.4byte 0x00000001
	.4byte 0xffff002b
	.4byte 0x0000002b
	.4byte 0x00000202
	.4byte 0xffff0008
	.4byte Func_02001444
	.4byte 0x50008615
	.4byte 0x02070008
	.4byte Func_020015a4
	.4byte 0x00000002
	.4byte 0x02020009
	.4byte Func_02002140
	.4byte 0x00000002
	.4byte 0x1202000a
	.4byte Func_02002180
	.4byte 0x00000002
	.4byte 0x02000005
	.4byte Func_020014ec
	.4byte 0x00000002
	.4byte 0x12000006
	.4byte Func_02001500
	.4byte 0x00000002
	.4byte 0x0209000b
	.4byte Func_02002f74
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte Func_0200118c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.section .bss,"aw",%nobits
	.global Data_02005678
Data_02005678:
	.space 0x00000004
	.global Data_0200567c
Data_0200567c:
