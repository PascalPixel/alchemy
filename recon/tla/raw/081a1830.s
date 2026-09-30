.syntax unified
	.thumb
	.global Func_081a1830
	.thumb_func
Func_081a1830:
	push {r5, r6, r7, lr}
	sub sp, #4
	bl Resource_FindFreeEntry
	adds r5, r0, #0
	movs r0, #144
	lsls r0, r0, #1
	bl Runtime_BumpAllocateAlternatePool
	ldr r6, .L_081a18dc
	movs r1, #128
	str r0, [r6]
	lsls r1, r1, #2
	adds r0, r5, #0
	movs r2, #0
	bl VramBlock_LoadCached
	ldr r3, .L_081a18e0
	ldr r2, .L_081a18e4
	adds r4, r0, #0
	lsls r1, r4, #5
	mov r0, sp
	str r3, [r0]
	adds r1, r1, r2
	movs r3, #128
	movs r2, #133
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r2, #128
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r6, [r6]
	ldr r0, .L_081a18e8
	movs r1, #0
	movs r5, #0
	adds r2, r6, #0
.L_081a187a:
	lsls r3, r1, #21
	orrs r3, r0
	adds r1, #1
	str r5, [r2]
	str r3, [r2, #4]
	str r4, [r2, #8]
	adds r6, #12
	adds r2, #12
	cmp r1, #7
	bls .L_081a187a
	movs r0, #128
	ldr r5, .L_081a18ec
	lsls r0, r0, #4
	movs r1, #0
	movs r7, #0
	orrs r0, r4
	adds r2, r6, #0
.L_081a189c:
	lsls r3, r1, #21
	orrs r3, r5
	adds r1, #1
	str r7, [r2]
	str r3, [r2, #4]
	str r0, [r2, #8]
	adds r6, #12
	adds r2, #12
	cmp r1, #7
	bls .L_081a189c
	movs r2, #128
	ldr r0, .L_081a18f0
	lsls r2, r2, #4
	movs r1, #0
	movs r5, #0
	orrs r2, r4
.L_081a18bc:
	lsls r3, r1, #21
	orrs r3, r0
	adds r1, #1
	str r5, [r6]
	str r3, [r6, #4]
	str r2, [r6, #8]
	adds r6, #12
	cmp r1, #7
	bls .L_081a18bc
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_081a18f4
	bl Func_080145a8
	add sp, #4
	pop {r5, r6, r7, pc}
.L_081a18dc:
	.4byte Data_0200752c
.L_081a18e0:
	.4byte 0x11111111
.L_081a18e4:
	.4byte 0x06010000
.L_081a18e8:
	.4byte 0x80004078
.L_081a18ec:
	.4byte 0x80004088
.L_081a18f0:
	.4byte 0x40004098
.L_081a18f4:
	.4byte Func_081a1810
