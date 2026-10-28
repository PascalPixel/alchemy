.syntax unified
	.thumb
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
	.4byte gMapShapeGrid
.L_020082f4:
	.4byte gAdventure
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
	.4byte gAdventure
	.section .text.x02008584,"ax",%progbits
	.balign 4
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
	.4byte gAdventure
.L_020086e4:
	.4byte 0xfdff0000
.L_020086e8:
	.4byte gMapShapeGrid
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
	.4byte gAdventure
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
	bl ObjectDispatch_SetSingleChildFlags
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
	.4byte gAdventure
.L_02008a84:
	.4byte 0xfff80000
.L_02008a88:
	.4byte 0xfffe0000
.L_02008a8c:
	.4byte gData_02003e00
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
	.4byte Effect_AdvanceMotionAtLeaderPriority
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
	bl ObjectDispatch_SetSingleChildFlags
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
	.4byte gAdventure
.L_02008bac:
	.4byte gData_02003e30
.L_02008bb0:
	.4byte 0xffffa000
.L_02008bb4:
	.4byte 0xffffd000
.L_02008bb8:
	.4byte 0xfffff800
.L_02008bbc:
	.4byte Effect_AdvanceMotionAtLeaderPriority
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
	.4byte gMapShapeGrid
.L_02008c48:
	.4byte 0x000fffff
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
	bl ObjectDispatch_SetSingleChildFlags
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
	.4byte gMapShapeGrid
.L_02008d74:
	.4byte gAdventure
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
	.4byte gAdventure
.L_02008e94:
	.4byte gMapShapeGrid
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
	.4byte gMapShapeGrid
.L_02008f40:
	.4byte 0xfdff0000
	.section .text.x02008f8a,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008fc2,"ax",%progbits
	.2byte 0x0000
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
	bl FieldEvent_WaitFrames
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
	.4byte gAdventure
.L_02009c94:
	.4byte 0x00000022
.L_02009c98:
	.4byte gData_02004068
.L_02009c9c:
	.4byte 0x00000023
.L_02009ca0:
	.4byte gData_02004110
.L_02009ca4:
	.4byte 0x00000024
.L_02009ca8:
	.4byte gData_02004170
.L_02009cac:
	.4byte 0x00000026
.L_02009cb0:
	.4byte gData_020041b8
.L_02009cb4:
	.4byte 0xfff80000
.L_02009cb8:
	.4byte gData_02003fd2
.L_02009cbc:
	.4byte gData_02003fda
.L_02009cc0:
	.4byte gData_02003fe2
.L_02009cc4:
	.4byte gData_02003fea
.L_02009cc8:
	.4byte gData_02003ff2
.L_02009ccc:
	.4byte gData_02004012
.L_02009cd0:
	.4byte gData_02004032
.L_02009cd4:
	.4byte gData_02004052
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
	bl FieldEvent_WaitFrames
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
	bl ObjectDispatch_SetSingleChildFlags
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
	.4byte gData_02004052
.L_02009d38:
	.4byte gAdventure
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
	bl FieldEvent_WaitFrames
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
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
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
	bl FieldEvent_WaitFrames
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
	.4byte gData_02004420
.L_02009ebc:
	.4byte gAdventure
.L_02009ec0:
	.4byte 0xfffffc00
.L_02009ec4:
	.4byte 0xfffffd00
.L_02009ec8:
	.4byte 0xffff6667
.L_02009ecc:
	.4byte 0x00000022
.L_02009ed0:
	.4byte gData_02003fd2
.L_02009ed4:
	.4byte gData_02003fda
.L_02009ed8:
	.4byte gData_02003fe2
.L_02009edc:
	.4byte gData_02003fea
.L_02009ee0:
	.4byte gData_0200567c
.L_02009ee4:
	.4byte gData_02005678
	.section .text.x0200a696,"ax",%progbits
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
	bl ObjectDispatch_SetSingleChildFlags
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
	bl FieldEvent_WaitFrames
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
	.4byte gData_02003f14
.L_0200a7d0:
	.4byte 0xffffff00
.L_0200a7d4:
	.4byte 0xfffff800
.L_0200a7d8:
	.4byte 0xfffffa00
.L_0200a7dc:
	.4byte Effect_AdvanceMotion
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
	bl FieldEvent_WaitFrames
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
	bl FieldEvent_WaitFrames
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
	.4byte gAdventure
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
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
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
	bl Camera_MoveWithinMapBounds
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
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #204
	bl Func_02003df8
	movs r3, #3
	strb r3, [r6]
	movs r0, #24
	bl FieldEvent_WaitFrames
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
	bl FieldEvent_WaitFrames
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
	bl FieldEvent_WaitFrames
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
	.4byte gAdventure
.L_0200adcc:
	.4byte gData_02004420
.L_0200add0:
	.4byte 0x00019999
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
	bl Task_Add
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b51c:
	.4byte gAdventure
.L_0200b520:
	.4byte gData_02003fc4
.L_0200b524:
	.4byte 0xfff80000
.L_0200b528:
	.4byte gData_02003fc8
.L_0200b52c:
	.4byte gData_02003f80
.L_0200b530:
	.4byte gData_0200567c
.L_0200b534:
	.4byte gData_02005678
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
	.4byte gData_02004276
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
	.4byte gData_02004276
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
	.4byte gAdventure
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
	bl Camera_MoveWithinMapBounds
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
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #204
	bl Func_02003df8
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl FieldEvent_WaitFrames
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
	bl FieldEvent_WaitFrames
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
	.4byte gAdventure
.L_0200b8fc:
	.4byte Effect_AdvanceMotion
.L_0200b900:
	.4byte 0xffffa000
.L_0200b904:
	.4byte 0xffffd000
.L_0200b908:
	.4byte 0x01090001
	.section .bss,"aw",%nobits
	.global gData_02005678
gData_02005678:
	.space 0x00000004
	.global gData_0200567c
gData_0200567c:
