.syntax unified
	.thumb
	.global Func_08101638
	.thumb_func
Func_08101638:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	sub sp, #12
	mov r10, r3
	movs r3, #192
	lsls r3, r3, #1
	add r3, r10
	ldr r3, [r3]
	movs r1, #0
	mov r8, r3
	mov r3, r10
	adds r3, #52
	str r3, [sp, #8]
	movs r3, #15
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	ldr r0, [sp, #8]
	movs r2, #5
	movs r3, #30
	bl UiWindow_UpdateOrCreate
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_081017c4
	mov r0, r8
	movs r2, #128
	mov r9, r3
	ldr r1, .L_081017c8
	lsls r2, r2, #6
	adds r0, #168
	mov lr, r9
	.2byte 0xf800
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #160
	adds r0, #168
	lsls r1, r1, #19
	add r0, r8
	adds r1, #128
	movs r2, #128
	mov lr, r9
	.2byte 0xf800
	movs r1, #128
	ldr r7, .L_081017cc
	lsls r1, r1, #6
	ldr r2, .L_081017d0
	ldr r0, .L_081017c8
	mov lr, r7
	.2byte 0xf800
	movs r0, #160
	lsls r0, r0, #19
	movs r1, #128
	ldr r2, .L_081017d4
	adds r0, #128
	mov lr, r7
	.2byte 0xf800
	ldr r0, .L_081017d8
	bl Func_080383b8
	ldr r1, .L_081017dc
	movs r2, #32
	ldr r0, .L_081017e0
	mov lr, r9
	.2byte 0xf800
	bl Func_080149f0
	movs r3, #128
	movs r1, #160
	movs r2, #128
	lsls r3, r3, #19
	lsls r1, r1, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r1, #160
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r6, .L_081017e4
	movs r2, #160
	ldrh r3, [r6]
	lsls r2, r2, #19
	adds r2, #188
	strh r3, [r2]
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_081017e8
	adds r1, #64
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #8
	bl Graphics_AdjustPaletteBank
	ldrh r3, [r6]
	movs r2, #160
	lsls r2, r2, #19
	adds r2, #232
	strh r3, [r2]
	subs r2, #32
	ldrh r3, [r6]
	mov r0, r8
	strh r3, [r2]
	bl Func_081019a4
	mov r3, r10
	ldr r6, [r3, #16]
	adds r0, r6, #0
	bl RenderOutput_ClearListFar
	movs r1, #8
	negs r1, r1
	movs r2, #11
	adds r0, r6, #0
	bl UiIcon_CreateWithResourceVariant
	movs r3, #0
	mov r11, r3
	movs r3, #13
	mov r8, r3
	adds r7, r0, #0
	mov r3, r8
	strb r3, [r7, #5]
	mov r3, r10
	str r7, [r3, #20]
	bl Resource_FindFreeEntry
	adds r5, r0, #0
	cmp r5, #95
	bgt .L_081017b2
	ldr r2, .L_081017ec
	movs r1, #128
	bl VramBlock_LoadResourceFar
	movs r3, #128
	lsls r3, r3, #23
	mov r9, r3
	mov r1, r9
	mov r3, r11
	adds r2, r6, #0
	adds r0, r5, #0
	str r3, [sp, #0]
	bl RenderOutput_CreateFar
	mov r3, r8
	adds r7, r0, #0
	strb r3, [r7, #5]
	movs r3, #188
	lsls r3, r3, #1
	add r3, r10
	str r7, [r3]
	ldrb r1, [r7, #23]
	ldr r2, .L_081017c0
	lsls r3, r1, #26
	lsrs r3, r3, #27
	orrs r2, r3
	movs r3, #63
	negs r3, r3
	lsls r2, r2, #1
	ands r3, r1
	orrs r3, r2
	strb r3, [r7, #23]
	adds r0, r5, #0
	mov r3, r11
	mov r1, r9
	adds r2, r6, #0
	str r3, [sp, #0]
	bl RenderOutput_CreateFar
	mov r3, r8
	adds r7, r0, #0
	strb r3, [r7, #5]
	movs r3, #190
	lsls r3, r3, #1
	add r3, r10
	str r7, [r3]
.L_081017b2:
	ldr r3, [sp, #8]
	ldr r0, .L_081017f0
	ldr r6, [r3]
	movs r3, #188
	lsls r3, r3, #1
	add r3, r10
	b .L_081017f4
.L_081017c0:
	.4byte 0x00000008
.L_081017c4:
	.4byte IwramCopyWords
.L_081017c8:
	.4byte 0x06004000
.L_081017cc:
	.4byte IwramFillWords
.L_081017d0:
	.4byte 0x33333333
.L_081017d4:
	.4byte 0x55555555
.L_081017d8:
	.4byte 0x06005000
.L_081017dc:
	.4byte Data_081059b4
.L_081017e0:
	.4byte 0x060052c0
.L_081017e4:
	.4byte 0x050001e8
.L_081017e8:
	.4byte 0x050001e0
.L_081017ec:
	.4byte 0x000001fb
.L_081017f0:
	.4byte 0xfffffe00
.L_081017f4:
	ldr r7, [r3]
	ldrh r1, [r6, #12]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	mov r12, r3
	lsls r1, r1, #3
	mov r3, r12
	adds r2, r1, #0
	ands r2, r3
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ldrh r4, [r7, #22]
	mov lr, r3
	mov r3, lr
	ands r2, r3
	adds r3, r0, #0
	ands r3, r4
	orrs r3, r2
	ldrh r2, [r6, #14]
	strh r3, [r7, #22]
	lsls r2, r2, #3
	adds r2, #4
	mov r3, r12
	adds r4, r2, #0
	ands r4, r3
	strh r1, [r7, #6]
	strh r2, [r7, #8]
	strb r4, [r7, #20]
	movs r3, #190
	lsls r3, r3, #1
	add r3, r10
	ldr r7, [r3]
	adds r1, #224
	mov r3, r12
	strh r1, [r7, #6]
	ands r1, r3
	mov r3, lr
	ands r1, r3
	ldrh r3, [r7, #22]
	strh r2, [r7, #8]
	ands r0, r3
	orrs r0, r1
	strh r0, [r7, #22]
	strb r4, [r7, #20]
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
