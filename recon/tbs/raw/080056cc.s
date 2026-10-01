.syntax unified
	.thumb
	.global SaveState_InitializeWorkspace
	.thumb_func
SaveState_InitializeWorkspace:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #136
	lsls r1, r1, #5
	movs r0, #51
	sub sp, #24
	bl Runtime_AllocateBlock
	movs r3, #0
	mov r11, r0
	add r0, sp, #4
	str r3, [r0]
	mov r1, r11
	ldr r3, .L_08005720
	ldr r2, .L_08005724
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r1, .L_08005728
	movs r0, #2
	bl SetFlashTimerIntr
	movs r7, #0
	b .L_0800570c
.L_08005704:
	movs r0, #1
	bl WaitFrames
	adds r7, #1
.L_0800570c:
	cmp r7, #7
	bhi .L_0800571c
	bl IdentifyFlash
	lsls r0, r0, #16
	cmp r0, #0
	bne .L_08005704
	b .L_0800572c
.L_0800571c:
	movs r0, #1
	b .L_080057fc
.L_08005720:
	.4byte 0x040000d4
.L_08005724:
	.4byte 0x85000440
.L_08005728:
	.4byte Data_030000e0 + 0x14
.L_0800572c:
	mov r2, r11
	movs r3, #8
	adds r2, #64
	add r3, sp
	mov r6, r11
	str r2, [sp, #0]
	mov r8, r3
	movs r2, #32
	movs r3, #16
	adds r2, r2, r6
	adds r3, r3, r6
	movs r7, #0
	mov r9, r2
	mov r10, r3
.L_08005748:
	movs r3, #0
	strb r3, [r6]
	mov r2, r10
	movs r3, #16
	strb r3, [r2]
	ldr r3, .L_08005788
	mov r2, r9
	strh r3, [r2]
	adds r0, r7, #0
	bl SaveState_ReadSlotAndCheckChecksum
	ldr r3, .L_0800578c
	adds r5, r0, #0
	add r1, sp, #8
	ldr r0, [sp, #0]
	ldr r2, .L_08005790
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_0800576c:
	ldr r2, .L_0800578c
	ldr r3, [r2, #8]
	movs r2, #128
	lsls r2, r2, #24
	ands r3, r2
	cmp r3, #0
	bne .L_0800576c
	mov r0, r8
	ldr r1, .L_08005794
	movs r2, #7
	bl SaveState_CompareBytes
	b .L_08005798
	.2byte 0x0000
.L_08005788:
	.4byte 0x00000000
.L_0800578c:
	.4byte 0x040000d4
.L_08005790:
	.4byte 0x84000004
.L_08005794:
	.4byte Save_Signature
.L_08005798:
	cmp r0, #0
	bne .L_080057ea
	mov r2, r8
	ldrh r3, [r2, #10]
	mov r2, r9
	strh r3, [r2]
	mov r3, r8
	ldrb r2, [r3, #7]
	adds r1, r2, #0
	cmp r1, #15
	bhi .L_080057ea
	cmp r5, #0
	bne .L_080057ea
	movs r3, #1
	strb r3, [r6]
	mov r3, r10
	strb r2, [r3]
	cmp r5, r7
	bcs .L_080057ea
	mov r12, r1
	mov r1, r11
	adds r0, r1, #0
	movs r4, #0
	adds r0, #32
.L_080057c8:
	ldrb r3, [r1, #16]
	cmp r3, r12
	bne .L_080057e0
	mov r3, r8
	ldrh r3, [r3, #10]
	ldrh r2, [r0]
	mov lr, r3
	cmp r2, lr
	bcs .L_080057de
	strb r4, [r1]
	b .L_080057e0
.L_080057de:
	strb r4, [r6]
.L_080057e0:
	adds r5, #1
	adds r1, #1
	adds r0, #2
	cmp r5, r7
	bcc .L_080057c8
.L_080057ea:
	movs r2, #2
	movs r3, #1
	adds r7, #1
	adds r6, #1
	add r9, r2
	add r10, r3
	cmp r7, #15
	bls .L_08005748
	movs r0, #0
.L_080057fc:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
