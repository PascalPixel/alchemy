.syntax unified
	.thumb
	.global Unnamed_080b5534
	.thumb_func
Unnamed_080b5534:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r2, #0
	movs r0, #0
	sub sp, #128
	mov r10, r2
	bl Owner_GetStateFar
	mov r5, sp
	adds r6, r0, #0
	adds r1, r5, #0
	ldr r0, .L_080b56b4
	bl Ui_AdjustValueWithoutLimitFar
	mov r2, r10
	ldrh r3, [r5, r2]
	strb r3, [r6]
	lsls r3, r3, #16
	movs r0, #0
	cmp r3, #0
	beq .L_080b557a
	adds r2, r6, #0
	adds r1, r5, #0
.L_080b5566:
	adds r0, #1
	cmp r0, #13
	bgt .L_080b557a
	adds r1, #2
	ldrh r3, [r1]
	adds r2, #1
	strb r3, [r2]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_080b5566
.L_080b557a:
	movs r3, #0
	strb r3, [r6, #14]
	bl FarCall_WindowTable
	movs r0, #71
	bl AudioCommand_PlayFar
	movs r5, #0
	ldr r2, .L_080b56b8
	movs r3, #128
	lsls r3, r3, #19
	strh r2, [r3]
	ldr r3, .L_080b56bc
	movs r2, #131
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #2
.L_080b559c:
	strb r2, [r3]
	bl UiWork_ClearValueNameTablesFar
	ldr r0, .L_080b56c0
	movs r1, #5
	bl UiText_DrawQuantity
	movs r0, #0
	movs r1, #3
	bl UiText_DrawQuantity
	movs r0, #1
	movs r1, #1
	bl UiText_DrawQuantity
	movs r0, #1
	movs r1, #2
	bl UiText_DrawQuantity
	movs r0, #2
	movs r1, #4
	bl UiText_DrawQuantity
	mov r3, r10
	cmp r3, #0
	bne .L_080b55da
	ldr r3, .L_080b56c4
	movs r1, #2
	adds r0, r5, r3
	movs r2, #10
	b .L_080b55e2
.L_080b55da:
	ldr r3, .L_080b56c8
	movs r1, #2
	adds r0, r5, r3
	movs r2, #2
.L_080b55e2:
	movs r3, #4
	bl UiText_OpenMessageWindowFar
	adds r7, r0, #0
	movs r0, #10
	bl WaitFrames
	ldr r2, .L_080b56cc
	ldr r3, .L_080b56c4
	ldr r6, .L_080b56d0
	subs r2, r2, r3
	mov r8, r2
	b .L_080b5614
.L_080b55fc:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_080b560e
	ldr r3, [r6]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_080b569a
.L_080b560e:
	movs r0, #1
	bl WaitFrames
.L_080b5614:
	ldr r3, [r6]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080b5630
	mov r2, r10
	cmp r2, #0
	beq .L_080b562a
	movs r3, #0
	mov r10, r3
	b .L_080b5630
.L_080b562a:
	movs r2, #1
	adds r5, #1
	mov r10, r2
.L_080b5630:
	ldr r3, [r6]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_080b563c
	adds r5, #1
.L_080b563c:
	ldr r3, [r6]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_080b5648
	subs r5, #2
.L_080b5648:
	ldr r3, [r6]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_080b5656
	movs r3, #1
	mov r10, r3
.L_080b5656:
	ldr r3, [r6]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_080b5664
	movs r2, #0
	mov r10, r2
.L_080b5664:
	ldr r3, [r6]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080b5672
	adds r5, #10
.L_080b5672:
	ldr r3, [r6]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080b5680
	subs r5, #10
.L_080b5680:
	cmp r5, #0
	bge .L_080b5686
	movs r5, #0
.L_080b5686:
	mov r3, r8
	adds r3, #5
	cmp r5, r3
	bcc .L_080b5690
	adds r5, r3, #0
.L_080b5690:
	ldr r3, [r6]
	ldr r2, .L_080b56d4
	ands r3, r2
	cmp r3, #0
	beq .L_080b55fc
.L_080b569a:
	movs r0, #1
	bl UiWork_DrainPendingFar
	adds r0, r7, #0
	movs r1, #1
	bl UiWork_FinalizeFar
	ldr r3, .L_080b56d8
	ldr r2, .L_080b56dc
	ldr r3, [r3]
	adds r3, r3, r2
	movs r2, #0
	b .L_080b559c
.L_080b56b4:
	.4byte 0x00000903
.L_080b56b8:
	.4byte 0x00001341
.L_080b56bc:
	.4byte gCell
.L_080b56c0:
	.4byte 0x000003e7
.L_080b56c4:
	.4byte 0x000026fa
.L_080b56c8:
	.4byte 0x00000ad0
.L_080b56cc:
	.4byte 0x00002850
.L_080b56d0:
	.4byte gKeysRepeat
.L_080b56d4:
	.4byte 0x000003f2
.L_080b56d8:
	.4byte gWindowWork
.L_080b56dc:
	.4byte 0x000012f8
