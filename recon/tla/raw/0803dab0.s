.syntax unified
	.thumb
	.global Func_0803dab0
	.thumb_func
Func_0803dab0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r1, #193
	adds r5, r0, #0
	lsls r1, r1, #3
	movs r0, #68
	mov r8, r3
	adds r7, r2, #0
	bl Runtime_AllocateHeapBlock
	adds r6, r0, #0
	ldr r0, .L_0803db48
	bl Resource_GetTableEntry
	adds r3, r5, #0
	cmp r5, #127
	bls .L_0803dad6
	subs r3, #112
.L_0803dad6:
	lsls r3, r3, #1
	ldrh r3, [r3, r0]
	movs r1, #192
	adds r5, r0, r3
	lsls r1, r1, #3
	adds r1, #4
	adds r3, r5, #0
	adds r2, r6, r1
	adds r3, #32
	str r3, [r2]
	movs r2, #192
	lsls r2, r2, #3
	adds r3, r6, r2
	subs r1, #2
	movs r2, #4
	strh r2, [r3]
	adds r3, r6, r1
	strh r2, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl UiGlyph_DecodeWithHeapRoutines
	ldr r2, [sp, #24]
	cmp r2, #0
	bne .L_0803db0e
	bl Resource_FindFreeEntry
	str r0, [r7]
.L_0803db0e:
	movs r3, #128
	lsls r3, r3, #3
	movs r1, #128
	adds r2, r6, r3
	ldr r0, [r7]
	lsls r1, r1, #2
	bl VramBlock_LoadCached
	mov r1, r8
	str r0, [r1]
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
	ldr r1, [sp, #20]
	ldr r2, .L_0803db4c
	lsls r1, r1, #5
	adds r1, r1, r2
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r5, #0
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0803db48:
	.4byte 0x000001d6
.L_0803db4c:
	.4byte 0x05000200
	.4byte 0x00004770
