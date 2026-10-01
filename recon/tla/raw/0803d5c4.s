.syntax unified
	.thumb
	.global Func_0803d5c4
	.thumb_func
Func_0803d5c4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r7, r1, #0
	movs r1, #0
	mov r10, r1
	movs r1, #193
	adds r6, r0, #0
	lsls r1, r1, #3
	movs r0, #68
	mov r8, r2
	mov r9, r3
	bl Runtime_AllocateHeapBlock
	adds r5, r0, #0
	bl Ui_CountIconTableEntries
	cmp r6, r0
	bcc .L_0803d5f0
	movs r6, #0
.L_0803d5f0:
	cmp r7, #0
	beq .L_0803d61e
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #4
	adds r2, r5, r3
	ldr r3, .L_0803d678
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
	mov r10, r2
.L_0803d61e:
	movs r3, #192
	ldr r2, .L_0803d67c
	lsls r3, r3, #3
	adds r3, #4
	adds r1, r5, r3
	lsls r3, r6, #2
	ldr r3, [r2, r3]
	movs r2, #2
	str r3, [r1]
	movs r1, #192
	lsls r1, r1, #3
	adds r3, r5, r1
	adds r1, #2
	strh r2, [r3]
	adds r3, r5, r1
	strh r2, [r3]
	adds r0, r5, #0
	mov r1, r10
	bl UiGlyph_DecodeWithHeapRoutines
	ldr r2, [sp, #28]
	cmp r2, #0
	bne .L_0803d654
	bl Resource_FindFreeEntry
	mov r3, r8
	str r0, [r3]
.L_0803d654:
	movs r3, #128
	mov r1, r8
	lsls r3, r3, #3
	ldr r0, [r1]
	adds r2, r5, r3
	movs r1, #128
	bl VramBlock_LoadCached
	mov r1, r9
	str r0, [r1]
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0803d678:
	.4byte UiIcon_BaseGlyphPointers
.L_0803d67c:
	.4byte Data_0804eb58
