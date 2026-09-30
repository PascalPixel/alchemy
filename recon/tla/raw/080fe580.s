.syntax unified
	.thumb
	.global Func_080fe580
	.thumb_func
Func_080fe580:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	movs r2, #28
	adds r2, r2, r0
	adds r3, #220
	lsls r0, r0, #2
	ldr r5, [r3]
	mov r10, r0
	mov r3, r10
	adds r3, #20
	ldr r0, [r5, r3]
	movs r6, #0
	movs r3, #1
	strb r3, [r0, #5]
	strh r6, [r0, #12]
	ldr r0, [r5, #16]
	sub sp, #4
	mov r8, r2
	ldrsb r7, [r5, r2]
	bl RenderOutput_RedrawSavedRectFar
	movs r0, #185
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080fe5ce
	movs r3, #3
	ldr r0, [r5, #16]
	movs r1, #9
	str r3, [sp, #0]
	movs r2, #1
	movs r3, #9
	bl UiWindow_DrawDividerLineFar
.L_080fe5ce:
	movs r3, #3
	ldr r0, [r5, #16]
	movs r1, #0
	str r3, [sp, #0]
	movs r2, #3
	movs r3, #12
	bl UiWindow_DrawDividerLineFar
	movs r3, #1
	negs r3, r3
	cmp r7, r3
	bne .L_080fe5ee
	ldr r3, .L_080fe610
	mov r2, r8
	strb r3, [r5, r2]
	b .L_080fe5fc
.L_080fe5ee:
	lsls r0, r7, #1
	adds r0, r0, r7
	lsls r0, r0, #3
	subs r0, #10
	movs r1, #16
	bl Func_080f8ab4
.L_080fe5fc:
	movs r2, #135
	lsls r2, r2, #2
	adds r3, r5, r2
	ldrh r3, [r3]
	cmp r3, #3
	bne .L_080fe614
	bl Func_080feb58
	b .L_080fe618
	.2byte 0x0000
.L_080fe610:
	.4byte 0x00000000
.L_080fe614:
	bl Func_080fe894
.L_080fe618:
	adds r6, r0, #0
	mov r3, r10
	adds r3, #20
	ldr r0, [r5, r3]
	bl UiIcon_PrepareObject
	movs r0, #1
	bl WaitFrames
	adds r0, r6, #0
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
