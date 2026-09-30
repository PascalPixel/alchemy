.syntax unified
	.thumb
	.global Func_080d690c
	.thumb_func
Func_080d690c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r1, #128
	lsls r1, r1, #4
	adds r1, #20
	movs r0, #116
	sub sp, #8
	bl Runtime_AllocateBlock
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	adds r5, r0, #0
	ldr r3, [r3]
	movs r0, #170
	mov r8, r3
	bl Func_080d2c64
	movs r3, #128
	movs r6, #0
	adds r7, r5, #0
	add r0, sp, #4
	lsls r3, r3, #19
	adds r7, #8
	str r6, [r0]
	adds r3, #212
	adds r1, r5, #0
	ldr r2, .L_080d69d8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #3
	movs r0, #56
	bl Runtime_AllocateBlock
	adds r6, r0, #0
	adds r1, r6, #0
	ldr r0, .L_080d69dc
	bl Func_0801591c
	bl Resource_FindFreeEntry
	movs r1, #192
	str r0, [r5]
	lsls r1, r1, #2
	adds r2, r6, #0
	bl VramBlock_LoadCached
	str r0, [r5, #4]
	movs r0, #56
	bl Runtime_ReleaseHeapBlock
	movs r5, #0
.L_080d6978:
	movs r4, #0
	adds r6, r7, #0
	stmia r6!, {r4}
	ldr r3, .L_080d69e0
	movs r0, #0
	stmia r6!, {r3}
	movs r3, #212
	lsls r3, r3, #8
	str r3, [r6]
	mov r3, r8
	ldr r2, [r3, #8]
	ldr r1, [r3]
	str r2, [r7, #20]
	str r1, [r7, #12]
	str r4, [sp, #0]
	bl Func_080201c0
	ldr r2, .L_080d69d4
	adds r3, r5, #0
	ands r3, r2
	adds r3, #1
	adds r5, #1
	str r0, [r7, #16]
	strh r3, [r7, #28]
	ldr r4, [sp, #0]
	adds r7, #32
	cmp r5, #63
	bls .L_080d6978
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
	strh r4, [r3]
	ldr r0, .L_080d69e4
	movs r1, #144
	lsls r1, r1, #3
	b .L_080d69e8
	.2byte 0x0000
.L_080d69d4:
	.4byte 0x0000000f
.L_080d69d8:
	.4byte 0x85000205
.L_080d69dc:
	.4byte Data_080f3798
.L_080d69e0:
	.4byte 0x40000400
.L_080d69e4:
	.4byte Func_080d64f0
.L_080d69e8:
	bl Func_080145a8
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
