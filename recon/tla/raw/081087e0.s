.syntax unified
	.thumb
	.global Func_081087e0
	.thumb_func
Func_081087e0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r1
	movs r1, #128
	adds r5, r0, #0
	lsls r1, r1, #3
	movs r0, #56
	mov r10, r2
	adds r7, r3, #0
	sub sp, #4
	bl Runtime_AllocateBlock
	movs r3, #0
	mov r9, r3
	movs r2, #132
	movs r3, #128
	adds r6, r0, #0
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_081088d0
	adds r1, r6, #0
	adds r2, #64
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r5, #0
	movs r1, #10
	bl __modsi3
	adds r1, r6, #0
	movs r2, #0
	bl Shop_CopyGlyphs
	adds r0, r5, #0
	movs r1, #10
	bl __divsi3
	adds r5, r0, #0
	cmp r5, #0
	beq .L_08108896
	movs r1, #10
	bl __modsi3
	adds r1, r6, #0
	movs r2, #1
	bl Shop_CopyGlyphs
	adds r0, r5, #0
	movs r1, #10
	bl __divsi3
	adds r5, r0, #0
	cmp r5, #0
	beq .L_08108896
	movs r1, #10
	bl __modsi3
	adds r1, r6, #0
	movs r2, #2
	bl Shop_CopyGlyphs
	adds r0, r5, #0
	movs r1, #10
	bl __divsi3
	adds r5, r0, #0
	cmp r5, #0
	beq .L_08108896
	movs r1, #10
	bl __modsi3
	adds r1, r6, #0
	movs r2, #3
	bl Shop_CopyGlyphs
	adds r0, r5, #0
	movs r1, #10
	bl __divsi3
	cmp r0, #0
	beq .L_08108896
	movs r1, #10
	bl __modsi3
	adds r1, r6, #0
	movs r2, #4
	bl Shop_CopyGlyphs
.L_08108896:
	bl Resource_FindFreeEntry
	adds r5, r0, #0
	cmp r5, #96
	beq .L_081088ba
	movs r1, #128
	lsls r1, r1, #1
	adds r2, r6, #0
	bl VramBlock_LoadCached
	ldr r1, .L_081088d4
	adds r0, r5, #0
	mov r2, r8
	mov r3, r10
	str r7, [sp, #0]
	bl RenderOutput_CreateFar
	mov r9, r0
.L_081088ba:
	movs r0, #56
	bl Runtime_ReleaseHeapBlock
	mov r0, r9
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081088d0:
	.4byte Data_0810c148
.L_081088d4:
	.4byte 0x80008000
