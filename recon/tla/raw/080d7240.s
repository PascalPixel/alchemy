.syntax unified
	.thumb
	.global Func_080d7240
	.thumb_func
Func_080d7240:
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
	ldr r2, .L_080d72ec
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #3
	movs r0, #56
	bl Runtime_AllocateBlock
	adds r6, r0, #0
	adds r1, r6, #0
	ldr r0, .L_080d72f0
	bl Func_0801591c
	bl Resource_FindFreeEntry
	movs r1, #128
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
	movs r6, #0
.L_080d72a4:
	mov r2, r8
	ldr r3, [r2, #32]
	adds r1, r7, #0
	stmia r1!, {r6}
	ldr r2, [r3]
	ldr r3, .L_080d72f4
	movs r0, #0
	stmia r1!, {r3}
	movs r3, #212
	lsls r3, r3, #8
	str r3, [r1]
	ldr r1, [r2]
	ldr r2, [r2, #8]
	str r6, [r7, #12]
	str r6, [r7, #20]
	bl Map_GetTerrainHeightFar
	ldr r2, .L_080d72e8
	adds r3, r5, #0
	ands r3, r2
	adds r3, #1
	adds r5, #1
	str r0, [r7, #16]
	strh r3, [r7, #28]
	adds r7, #32
	cmp r5, #63
	bls .L_080d72a4
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080d72f8
	bl Scheduler_AddOrUpdateCallback
	add sp, #4
	b .L_080d72fc
.L_080d72e8:
	.4byte 0x0000000f
.L_080d72ec:
	.4byte 0x85000205
.L_080d72f0:
	.4byte Data_080f38b2
.L_080d72f4:
	.4byte 0x40000400
.L_080d72f8:
	.4byte Func_080d7034
.L_080d72fc:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
