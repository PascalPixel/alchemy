.syntax unified
	.thumb
	.global ItemIcon_Compose
	.thumb_func
ItemIcon_Compose:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	str r0, [sp, #8]
	ldr r2, [sp, #8]
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	mov r11, r1
	ands r0, r2
	movs r1, #0
	str r1, [sp, #4]
	mov r10, r1
	bl Item_Get
	str r0, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #68]
	cmp r5, #0
	bne .L_0803d6ba
	movs r0, #1
	negs r0, r0
	b .L_0803d88c
.L_0803d6ba:
	movs r3, #1
	mov r1, r11
	ands r3, r1
	cmp r3, #0
	beq .L_0803d6ee
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #4
	adds r2, r5, r3
	ldr r3, .L_0803d89c
	movs r1, #192
	ldr r3, [r3, #8]
	lsls r1, r1, #3
	str r3, [r2]
	movs r2, #2
	adds r3, r5, r1
	adds r1, #2
	strh r2, [r3]
	adds r3, r5, r1
	strh r2, [r3]
	adds r0, r5, #0
	movs r1, #0
	bl UiGlyph_DecodeWithHeapRoutines
	movs r2, #1
	str r2, [sp, #4]
.L_0803d6ee:
	movs r3, #192
	ldr r1, [sp, #0]
	lsls r3, r3, #3
	adds r3, #4
	adds r3, r3, r5
	mov r9, r3
	ldrh r3, [r1, #6]
	ldr r2, .L_0803d8a0
	lsls r3, r3, #2
	ldr r3, [r2, r3]
	mov r2, r9
	str r3, [r2]
	movs r3, #192
	lsls r3, r3, #3
	movs r2, #192
	adds r3, r3, r5
	lsls r2, r2, #3
	mov r8, r3
	adds r2, #2
	movs r6, #2
	mov r1, r8
	adds r7, r5, r2
	strh r6, [r1]
	strh r6, [r7]
	ldr r1, [sp, #4]
	adds r0, r5, #0
	bl UiGlyph_DecodeWithHeapRoutines
	movs r3, #8
	mov r1, r11
	ands r3, r1
	cmp r3, #0
	beq .L_0803d752
	ldr r2, [sp, #8]
	movs r3, #128
	lsls r3, r3, #3
	ands r3, r2
	cmp r3, #0
	beq .L_0803d752
	ldr r3, .L_0803d8a4
	mov r1, r9
	ldr r3, [r3, #4]
	mov r2, r8
	str r3, [r1]
	adds r0, r5, #0
	strh r6, [r2]
	movs r1, #1
	strh r6, [r7]
	bl UiGlyph_DecodeWithHeapRoutines
.L_0803d752:
	movs r3, #16
	mov r1, r11
	ands r3, r1
	cmp r3, #0
	beq .L_0803d78e
	ldr r2, [sp, #8]
	movs r3, #128
	lsls r3, r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0803d78e
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #4
	adds r2, r5, r3
	ldr r3, .L_0803d8a4
	movs r1, #192
	ldr r3, [r3]
	lsls r1, r1, #3
	str r3, [r2]
	adds r3, r5, r1
	movs r2, #2
	adds r1, #2
	strh r2, [r3]
	adds r3, r5, r1
	strh r2, [r3]
	adds r0, r5, #0
	movs r1, #1
	bl UiGlyph_DecodeWithHeapRoutines
.L_0803d78e:
	movs r3, #32
	mov r2, r11
	ands r3, r2
	cmp r3, #0
	beq .L_0803d7de
	ldr r1, [sp, #8]
	movs r3, #128
	lsls r3, r3, #2
	ands r3, r1
	cmp r3, #0
	beq .L_0803d7de
	ldr r2, [sp, #0]
	movs r3, #1
	ldrb r0, [r2, #3]
	ands r3, r0
	cmp r3, #0
	beq .L_0803d7de
	movs r1, #2
	adds r3, r1, #0
	ands r3, r0
	cmp r3, #0
	beq .L_0803d7de
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #4
	adds r2, r5, r3
	ldr r3, .L_0803d8a4
	adds r0, r5, #0
	ldr r3, [r3, #8]
	str r3, [r2]
	movs r2, #192
	lsls r2, r2, #3
	adds r3, r5, r2
	adds r2, #2
	strh r1, [r3]
	adds r3, r5, r2
	strh r1, [r3]
	movs r1, #1
	bl UiGlyph_DecodeWithHeapRoutines
.L_0803d7de:
	movs r3, #2
	mov r1, r11
	ands r3, r1
	cmp r3, #0
	beq .L_0803d802
	ldr r2, [sp, #8]
	movs r3, #248
	lsls r3, r3, #8
	ands r3, r2
	lsrs r3, r3, #11
	mov r10, r3
	movs r3, #1
	add r10, r3
	mov r1, r10
	cmp r1, #1
	bgt .L_0803d802
	movs r2, #0
	mov r10, r2
.L_0803d802:
	movs r3, #4
	mov r1, r11
	ands r3, r1
	cmp r3, #0
	beq .L_0803d81c
	ldr r2, [sp, #8]
	movs r3, #248
	lsls r3, r3, #8
	ands r3, r2
	lsrs r3, r3, #11
	mov r10, r3
	movs r3, #1
	add r10, r3
.L_0803d81c:
	mov r1, r10
	cmp r1, #0
	beq .L_0803d888
	cmp r1, #30
	bgt .L_0803d888
	movs r1, #10
	mov r0, r10
	bl __modsi3
	ldr r3, .L_0803d8a8
	lsls r0, r0, #2
	movs r2, #192
	lsls r2, r2, #3
	mov r11, r3
	ldr r3, [r3, r0]
	adds r2, #4
	adds r2, r2, r5
	movs r1, #192
	str r3, [r2]
	lsls r1, r1, #3
	movs r3, #192
	adds r1, r1, r5
	lsls r3, r3, #3
	mov r8, r1
	adds r3, #2
	movs r6, #2
	adds r7, r5, r3
	mov r9, r2
	mov r2, r8
	strh r6, [r2]
	adds r0, r5, #0
	movs r1, #1
	strh r6, [r7]
	bl UiGlyph_DecodeWithHeapRoutines
	mov r0, r10
	movs r1, #10
	bl Math_Div
	cmp r0, #0
	beq .L_0803d888
	lsls r3, r0, #2
	mov r1, r11
	adds r3, #36
	ldr r3, [r1, r3]
	mov r2, r9
	str r3, [r2]
	mov r3, r8
	strh r6, [r3]
	adds r0, r5, #0
	strh r6, [r7]
	movs r1, #1
	bl UiGlyph_DecodeWithHeapRoutines
.L_0803d888:
	movs r0, #128
	lsls r0, r0, #1
.L_0803d88c:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0803d89c:
	.4byte Data_0804e684
.L_0803d8a0:
	.4byte Data_0804eb58
.L_0803d8a4:
	.4byte Data_0804e740
.L_0803d8a8:
	.4byte Data_0804e7dc
