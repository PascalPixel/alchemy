.syntax unified
	.thumb
	.global Func_080d6e58
	.thumb_func
Func_080d6e58:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r1, #128
	lsls r1, r1, #4
	adds r1, #20
	movs r0, #116
	sub sp, #4
	bl Runtime_AllocateBlock
	movs r3, #128
	adds r5, r0, #0
	adds r7, r5, #0
	movs r1, #0
	mov r0, sp
	lsls r3, r3, #19
	str r1, [r0]
	adds r7, #8
	adds r3, #212
	adds r1, r5, #0
	ldr r2, .L_080d6f1c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #3
	movs r0, #56
	bl Runtime_AllocateBlock
	adds r6, r0, #0
	adds r1, r6, #0
	ldr r0, .L_080d6f20
	bl Resource_DecodeByteLzInRam
	bl Resource_FindFreeEntry
	movs r1, #192
	adds r2, r6, #0
	str r0, [r5]
	lsls r1, r1, #2
	bl VramBlock_LoadCached
	str r0, [r5, #4]
	movs r0, #56
	bl Runtime_ReleaseHeapBlock
	movs r2, #192
	lsls r2, r2, #18
	movs r5, #0
	mov r8, r2
.L_080d6eba:
	mov r2, r8
	ldr r3, [r2, #32]
	adds r1, r7, #0
	movs r6, #0
	stmia r1!, {r6}
	ldr r2, [r3]
	ldr r3, .L_080d6f24
	movs r0, #0
	stmia r1!, {r3}
	movs r3, #212
	lsls r3, r3, #8
	str r3, [r1]
	ldr r1, [r2]
	ldr r2, [r2, #8]
	str r1, [r7, #12]
	str r2, [r7, #20]
	bl Map_GetTerrainHeightFar
	ldr r2, .L_080d6f18
	adds r3, r5, #0
	ands r3, r2
	adds r3, #1
	adds r5, #1
	str r0, [r7, #16]
	strh r3, [r7, #28]
	adds r7, #32
	cmp r5, #63
	bls .L_080d6eba
	movs r3, #128
	movs r2, #252
	lsls r3, r3, #19
	lsls r2, r2, #6
	adds r3, #80
	strh r2, [r3]
	movs r2, #128
	lsls r2, r2, #5
	adds r2, #8
	adds r3, #2
	strh r2, [r3]
	adds r3, #2
	strh r6, [r3]
	ldr r0, .L_080d6f28
	movs r1, #144
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	b .L_080d6f2c
.L_080d6f18:
	.4byte 0x0000000f
.L_080d6f1c:
	.4byte 0x85000205
.L_080d6f20:
	.4byte Data_080f385e
.L_080d6f24:
	.4byte 0x40000400
.L_080d6f28:
	.4byte Func_080d6c60
.L_080d6f2c:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
