.syntax unified
	.thumb
	.global Func_081b21ec
	.thumb_func
Func_081b21ec:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r1, #160
	lsls r1, r1, #19
	sub sp, #128
	mov r8, r1
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	mov r1, sp
	adds r2, #32
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r7, .L_081b2224
	movs r2, #0
	movs r3, #31
	mov lr, r2
	mov r10, r3
	mov r12, sp
	movs r6, #0
	b .L_081b2228
	.2byte 0x0000
.L_081b2224:
	.4byte 0x0000001f
.L_081b2228:
	mov r1, r8
	ldrh r3, [r1]
	mov r5, r10
	mov r1, r12
	ands r5, r3
	lsls r3, r3, #16
	lsrs r2, r3, #21
	lsrs r0, r3, #26
	ldrh r3, [r6, r1]
	mov r1, r10
	ands r1, r3
	lsls r3, r3, #16
	lsrs r4, r3, #21
	lsrs r3, r3, #26
	ands r2, r7
	ands r0, r7
	ands r4, r7
	ands r3, r7
	cmp r5, r1
	bge .L_081b2254
	adds r5, #1
	b .L_081b225a
.L_081b2254:
	cmp r5, r1
	ble .L_081b225a
	subs r5, #1
.L_081b225a:
	cmp r2, r4
	bge .L_081b2262
	adds r2, #1
	b .L_081b2268
.L_081b2262:
	cmp r2, r4
	ble .L_081b2268
	subs r2, #1
.L_081b2268:
	cmp r0, r3
	bge .L_081b2270
	adds r0, #1
	b .L_081b2276
.L_081b2270:
	cmp r0, r3
	ble .L_081b2276
	subs r0, #1
.L_081b2276:
	lsls r2, r2, #5
	lsls r3, r0, #10
	orrs r3, r2
	orrs r3, r5
	mov r2, r12
	strh r3, [r6, r2]
	movs r3, #1
	add lr, r3
	movs r1, #2
	mov r2, lr
	adds r6, #2
	add r8, r1
	cmp r2, #64
	bne .L_081b2228
	movs r3, #128
	movs r1, #160
	movs r2, #128
	mov r0, sp
	lsls r3, r3, #19
	lsls r1, r1, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, #2
	adds r1, #2
	adds r2, #63
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	add sp, #128
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
