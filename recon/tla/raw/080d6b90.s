.syntax unified
	.thumb
	.global Func_080d6b90
	.thumb_func
Func_080d6b90:
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
	movs r2, #0
	mov r0, sp
	lsls r3, r3, #19
	str r2, [r0]
	adds r7, #8
	adds r3, #212
	adds r1, r5, #0
	ldr r2, .L_080d6c50
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #3
	movs r0, #56
	bl Runtime_AllocateBlock
	adds r6, r0, #0
	adds r1, r6, #0
	ldr r0, .L_080d6c54
	bl Func_0801591c
	bl Resource_FindFreeEntry
	movs r1, #192
	adds r2, r6, #0
	lsls r1, r1, #2
	str r0, [r5]
	bl VramBlock_LoadCached
	str r0, [r5, #4]
	movs r0, #56
	bl Runtime_ReleaseHeapBlock
	movs r6, #0
	mov r8, r6
.L_080d6bee:
	adds r2, r7, #0
	mov r3, r8
	stmia r2!, {r3}
	ldr r3, .L_080d6c58
	stmia r2!, {r3}
	movs r3, #212
	lsls r3, r3, #8
	str r3, [r2]
	bl Random16
	movs r3, #200
	adds r5, r0, #0
	muls r5, r3
	movs r3, #144
	lsls r3, r3, #17
	adds r5, r5, r3
	bl Random16
	lsls r3, r0, #2
	adds r3, r3, r0
	movs r2, #160
	lsls r3, r3, #2
	lsls r2, r2, #15
	subs r2, r2, r3
	str r5, [r7, #12]
	str r2, [r7, #20]
	movs r0, #0
	adds r1, r5, #0
	bl Func_080201c0
	mov r3, r8
	str r3, [r7, #24]
	lsls r3, r6, #1
	adds r6, #1
	str r0, [r7, #16]
	strh r3, [r7, #28]
	adds r7, #32
	cmp r6, #63
	bls .L_080d6bee
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080d6c5c
	bl Func_080145a8
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d6c50:
	.4byte 0x85000205
.L_080d6c54:
	.4byte Data_080f385e
.L_080d6c58:
	.4byte 0x40000400
.L_080d6c5c:
	.4byte Func_080d69f4
