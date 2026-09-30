.syntax unified
	.thumb
	.global Func_0803f234
	.thumb_func
Func_0803f234:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r3
	movs r3, #1
	sub sp, #12
	adds r4, r2, #0
	negs r3, r3
	adds r7, r0, #0
	adds r5, r1, #0
	str r2, [sp, #8]
	adds r6, r4, #0
	cmp r4, r3
	bne .L_0803f25e
	bl Resource_FindFreeEntry
	adds r4, r0, #0
	str r0, [sp, #8]
	adds r0, r6, #0
	cmp r4, #96
	beq .L_0803f2d6
.L_0803f25e:
	subs r0, r7, #1
	cmp r0, #7
	bhi .L_0803f2d4
	ldr r2, .L_0803f2e0
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0803f26c:
	.4byte .L_0803f28c
	.4byte .L_0803f29e
	.4byte .L_0803f2d4
	.4byte .L_0803f2b6
	.4byte .L_0803f2d4
	.4byte .L_0803f28c
	.4byte .L_0803f2aa
	.4byte .L_0803f2c8
.L_0803f28c:
	movs r1, #1
	str r1, [sp, #0]
	add r2, sp, #8
	add r3, sp, #4
	adds r0, r5, #0
	mov r1, r8
	bl Ui_BuildPairedPatternsToSlot
	b .L_0803f2d2
.L_0803f29e:
	adds r2, r4, #0
	adds r0, r5, #0
	movs r1, #58
	bl UiIcon_CopyResourceToSlot
	b .L_0803f2d2
.L_0803f2aa:
	adds r2, r4, #0
	adds r0, r5, #0
	movs r1, #42
	bl UiIcon_CopyResourceToSlot
	b .L_0803f2d2
.L_0803f2b6:
	movs r1, #1
	str r1, [sp, #0]
	add r2, sp, #8
	add r3, sp, #4
	adds r0, r5, #0
	mov r1, r8
	bl Ability_LoadGlyph
	b .L_0803f2d2
.L_0803f2c8:
	adds r2, r4, #0
	adds r0, r5, #0
	movs r1, #0
	bl Ui_BuildPatternToSlot
.L_0803f2d2:
	ldr r4, [sp, #8]
.L_0803f2d4:
	adds r0, r4, #0
.L_0803f2d6:
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0803f2e0:
	.4byte .L_0803f26c
