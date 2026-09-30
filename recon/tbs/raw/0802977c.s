.syntax unified
	.thumb
	.global DebugMenu_BrowseEntryGlyphs
	.thumb_func
DebugMenu_BrowseEntryGlyphs:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_080298f8
	ldr r3, [r3]
	sub sp, #24
	movs r0, #1
	str r0, [sp, #8]
	movs r1, #0
	movs r2, #10
	mov r11, r3
	movs r0, #0
	movs r3, #5
	bl UiWindow_CreateWithSideObject
	movs r3, #2
	movs r2, #14
	str r0, [sp, #12]
	str r3, [sp, #0]
	movs r1, #10
	movs r3, #3
	movs r0, #10
	bl UiWindow_Create
	adds r7, r0, #0
	ldr r0, .L_080298fc
	movs r5, #0
	ldrsh r3, [r0, r5]
	movs r2, #1
	negs r2, r2
	movs r1, #0
	cmp r3, r2
	beq .L_080297d6
	mov r12, r2
	adds r2, r0, #0
.L_080297ca:
	adds r2, #4
	movs r0, #0
	ldrsh r3, [r2, r0]
	adds r1, #1
	cmp r3, r12
	bne .L_080297ca
.L_080297d6:
	ldr r0, .L_08029900
	mov r8, r1
	movs r1, #0
	ldrsh r3, [r0, r1]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	beq .L_080297f6
	mov r12, r2
	adds r2, r0, #0
.L_080297ea:
	adds r2, #4
	movs r0, #0
	ldrsh r3, [r2, r0]
	adds r1, #1
	cmp r3, r12
	bne .L_080297ea
.L_080297f6:
	add r1, r8
	mov r10, r1
	ldr r6, .L_08029904
	movs r1, #2
	mov r9, r1
.L_08029800:
	ldr r3, [r6]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_08029810
	movs r2, #1
	str r2, [sp, #8]
	subs r5, #1
.L_08029810:
	ldr r3, [r6]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_08029820
	movs r3, #1
	str r3, [sp, #8]
	adds r5, #1
.L_08029820:
	ldr r3, [r6]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_08029832
	movs r0, #1
	str r0, [sp, #8]
	subs r5, #10
.L_08029832:
	ldr r3, [r6]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08029844
	movs r1, #1
	str r1, [sp, #8]
	adds r5, #10
.L_08029844:
	ldr r3, [r6]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_080298ce
	ldr r3, [r6]
	mov r2, r9
	ands r3, r2
	cmp r3, #0
	bne .L_080298ce
	ldr r3, [sp, #8]
	cmp r3, #0
	beq .L_080298c6
	movs r0, #0
	mov r1, r10
	str r0, [sp, #8]
	adds r0, r5, r1
	bl Func_080022fc
	adds r5, r0, #0
	adds r0, r7, #0
	bl RenderOutput_PrepareForRedraw
	cmp r5, r8
	bge .L_08029880
	ldr r2, .L_080298fc
	lsls r3, r5, #2
	adds r3, #2
	ldrsh r0, [r2, r3]
	b .L_08029890
.L_08029880:
	mov r0, r8
	subs r2, r5, r0
	ldr r3, .L_08029900
	lsls r2, r2, #2
	adds r2, #2
	ldrsh r3, [r3, r2]
	adds r0, r3, #0
	adds r0, #128
.L_08029890:
	ldr r1, .L_08029908
	mov r2, r11
	ldrh r3, [r2, r1]
	movs r2, #15
	str r3, [sp, #20]
	movs r3, #1
	str r2, [sp, #0]
	str r3, [sp, #4]
	add r2, sp, #20
	add r3, sp, #16
	movs r1, #0
	bl UiGlyph_LoadEntryWithPalette
	movs r3, #0
	adds r0, r5, #0
	movs r1, #2
	adds r2, r7, #0
	str r3, [sp, #0]
	bl UiText_DrawNumberInWindow
	ldr r0, .L_0802990c
	adds r1, r7, #0
	adds r0, r5, r0
	movs r2, #24
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
.L_080298c6:
	movs r0, #1
	bl WaitFrames
	b .L_08029800
.L_080298ce:
	adds r0, r7, #0
	movs r1, #2
	bl UiWork_Finalize
	movs r1, #2
	ldr r0, [sp, #12]
	bl UiWork_Finalize
	movs r0, #1
	bl WaitFrames
	movs r0, #0
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_080298f8:
	.4byte Data_03001e8c
.L_080298fc:
	.4byte SideObject_CharacterIdMap
.L_08029900:
	.4byte SideObject_ActorKindIdMap
.L_08029904:
	.4byte gKeysRepeat
.L_08029908:
	.4byte 0x000012f2
.L_0802990c:
	.4byte 0x00000dd2
