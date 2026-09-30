.syntax unified
	.thumb
	.global Resource_LoadByMode
	.thumb_func
Resource_LoadByMode:
	push {r5, r6, lr}
	movs r6, #1
	sub sp, #12
	negs r6, r6
	adds r5, r1, #0
	str r6, [sp, #8]
	cmp r0, #2
	beq .L_0803f206
	cmp r0, #2
	bhi .L_0803f1ee
	cmp r0, #1
	beq .L_0803f1f6
	b .L_0803f22e
.L_0803f1ee:
	cmp r0, #4
	beq .L_0803f21e
	cmp r0, #6
	bne .L_0803f22e
.L_0803f1f6:
	movs r1, #0
	add r2, sp, #8
	add r3, sp, #4
	adds r0, r5, #0
	str r1, [sp, #0]
	bl Ui_BuildPairedPatternsToSlot
	b .L_0803f22e
.L_0803f206:
	bl Resource_FindFreeEntry
	adds r2, r0, #0
	str r2, [sp, #8]
	adds r0, r6, #0
	cmp r2, #96
	beq .L_0803f230
	adds r0, r5, #0
	movs r1, #26
	bl UiIcon_CopyResourceToSlot
	b .L_0803f22e
.L_0803f21e:
	movs r1, #0
	str r1, [sp, #0]
	add r2, sp, #8
	add r3, sp, #4
	adds r0, r5, #0
	movs r1, #1
	bl Ability_LoadGlyph
.L_0803f22e:
	ldr r0, [sp, #8]
.L_0803f230:
	add sp, #12
	pop {r5, r6, pc}
