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
	ldr r0, .L_0801a284
	mov r11, r1
	ands r0, r2
	movs r1, #0
	str r1, [sp, #4]
	mov r10, r1
	bl Item_Get
	ldr r3, .L_0801a288
	str r0, [sp, #0]
	ldr r5, [r3]
	cmp r5, #0
	bne .L_0801a0bc
	movs r0, #1
	negs r0, r0
	b .L_0801a272
.L_0801a0bc:
	movs r3, #1
	mov r1, r11
	ands r3, r1
	cmp r3, #0
	beq .L_0801a0ec
	ldr r3, .L_0801a28c
	adds r2, r5, r3
	ldr r3, .L_0801a290
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
.L_0801a0ec:
	ldr r3, .L_0801a28c
	ldr r1, [sp, #0]
	adds r3, r3, r5
	mov r9, r3
	ldrh r3, [r1, #6]
	ldr r2, .L_0801a294
	lsls r3, r3, #2
	ldr r3, [r2, r3]
	mov r2, r9
	str r3, [r2]
	movs r3, #192
	lsls r3, r3, #3
	adds r3, r3, r5
	ldr r2, .L_0801a298
	mov r8, r3
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
	beq .L_0801a148
	movs r3, #128
	ldr r2, [sp, #8]
	lsls r3, r3, #3
	ands r3, r2
	cmp r3, #0
	beq .L_0801a148
	ldr r3, .L_0801a29c
	ldr r3, [r3, #4]
	mov r1, r9
	mov r2, r8
	str r3, [r1]
	adds r0, r5, #0
	strh r6, [r2]
	movs r1, #1
	strh r6, [r7]
	bl UiGlyph_DecodeWithHeapRoutines
.L_0801a148:
	movs r3, #16
	mov r1, r11
	ands r3, r1
	cmp r3, #0
	beq .L_0801a180
	movs r3, #128
	ldr r2, [sp, #8]
	lsls r3, r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0801a180
	ldr r3, .L_0801a28c
	adds r2, r5, r3
	ldr r3, .L_0801a29c
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
.L_0801a180:
	movs r3, #32
	mov r2, r11
	ands r3, r2
	cmp r3, #0
	beq .L_0801a1cc
	movs r3, #128
	ldr r1, [sp, #8]
	lsls r3, r3, #2
	ands r3, r1
	cmp r3, #0
	beq .L_0801a1cc
	ldr r2, [sp, #0]
	ldrb r0, [r2, #3]
	movs r3, #1
	ands r3, r0
	cmp r3, #0
	beq .L_0801a1cc
	movs r1, #2
	adds r3, r1, #0
	ands r3, r0
	cmp r3, #0
	beq .L_0801a1cc
	ldr r3, .L_0801a28c
	adds r2, r5, r3
	ldr r3, .L_0801a29c
	ldr r3, [r3, #8]
	str r3, [r2]
	movs r2, #192
	lsls r2, r2, #3
	adds r3, r5, r2
	adds r2, #2
	strh r1, [r3]
	adds r3, r5, r2
	strh r1, [r3]
	adds r0, r5, #0
	movs r1, #1
	bl UiGlyph_DecodeWithHeapRoutines
.L_0801a1cc:
	movs r3, #2
	mov r1, r11
	ands r3, r1
	cmp r3, #0
	beq .L_0801a1f0
	movs r3, #248
	ldr r2, [sp, #8]
	lsls r3, r3, #8
	ands r3, r2
	lsrs r3, r3, #11
	mov r10, r3
	movs r3, #1
	add r10, r3
	mov r1, r10
	cmp r1, #1
	bgt .L_0801a1f0
	movs r2, #0
	mov r10, r2
.L_0801a1f0:
	movs r3, #4
	mov r1, r11
	ands r3, r1
	cmp r3, #0
	beq .L_0801a20a
	movs r3, #248
	ldr r2, [sp, #8]
	lsls r3, r3, #8
	ands r3, r2
	lsrs r3, r3, #11
	mov r10, r3
	movs r3, #1
	add r10, r3
.L_0801a20a:
	mov r1, r10
	cmp r1, #0
	beq .L_0801a26e
	cmp r1, #30
	bgt .L_0801a26e
	movs r1, #10
	mov r0, r10
	bl Func_080022fc
	ldr r3, .L_0801a2a0
	lsls r0, r0, #2
	ldr r2, .L_0801a28c
	mov r11, r3
	movs r1, #192
	ldr r3, [r3, r0]
	adds r2, r2, r5
	lsls r1, r1, #3
	str r3, [r2]
	adds r1, r1, r5
	ldr r3, .L_0801a298
	mov r8, r1
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
	bl FixedPoint_Ratio
	cmp r0, #0
	beq .L_0801a26e
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
.L_0801a26e:
	movs r0, #128
	lsls r0, r0, #1
.L_0801a272:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_0801a284:
	.4byte 0x000001ff
.L_0801a288:
	.4byte gGlyphWork
.L_0801a28c:
	.4byte 0x00000604
.L_0801a290:
	.4byte RomBytes_08029a10
.L_0801a294:
	.4byte UiIcon_ItemIconPointers
.L_0801a298:
	.4byte 0x00000602
.L_0801a29c:
	.4byte Data_08029acc
.L_0801a2a0:
	.4byte Data_08029b68
