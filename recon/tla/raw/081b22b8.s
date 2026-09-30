.syntax unified
	.thumb
	.global Func_081b22b8
	.thumb_func
Func_081b22b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r4, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #96]
	adds r7, r1, #0
	subs r6, r2, r0
	movs r1, #128
	mov r10, r2
	sub sp, #4
	mov r8, r0
	subs r5, r4, r7
	mov r9, r1
	mov r11, r3
	adds r2, r6, #0
	cmp r6, #0
	bge .L_081b22e8
	negs r2, r6
.L_081b22e8:
	adds r3, r5, #0
	cmp r5, #0
	bge .L_081b22f0
	negs r3, r5
.L_081b22f0:
	cmp r2, r3
	bge .L_081b2398
	cmp r5, #0
	bge .L_081b230c
	mov r12, r8
	mov r8, r10
	mov r10, r12
	mov r12, r7
	mov r2, r10
	adds r7, r4, #0
	mov r3, r8
	mov r4, r12
	subs r6, r2, r3
	subs r5, r4, r7
.L_081b230c:
	lsls r0, r6, #8
	cmp r6, #0
	bge .L_081b231a
	mov r1, r8
	mov r2, r10
	subs r3, r1, r2
	lsls r0, r3, #8
.L_081b231a:
	cmp r5, #0
	blt .L_081b232c
	adds r1, r5, #0
	str r4, [sp, #0]
	bl Math_Div
	mov r12, r0
	ldr r4, [sp, #0]
	b .L_081b2338
.L_081b232c:
	subs r1, r7, r4
	str r4, [sp, #0]
	bl Math_Div
	ldr r4, [sp, #0]
	mov r12, r0
.L_081b2338:
	adds r0, r7, #0
	mov r1, r8
	cmp r0, r4
	beq .L_081b2428
	ldr r7, .L_081b2438
	movs r3, #128
	lsls r3, r3, #1
	movs r5, #7
	mov lr, r3
	mov r8, r7
.L_081b234c:
	lsrs r2, r0, #3
	lsrs r3, r1, #3
	lsls r2, r2, #5
	adds r2, r2, r3
	adds r3, r0, #0
	ands r3, r5
	lsls r2, r2, #3
	adds r2, r2, r3
	adds r3, r1, #0
	ands r3, r5
	lsls r2, r2, #3
	mov r7, r11
	adds r2, r2, r3
	ldrb r3, [r7, r2]
	ldr r7, [sp, #36]
	cmp r3, r7
	bge .L_081b2372
	mov r3, r11
	strb r7, [r3, r2]
.L_081b2372:
	add r9, r12
	mov r3, r9
	mov r7, lr
	ands r3, r7
	cmp r3, #0
	beq .L_081b2390
	cmp r6, #0
	ble .L_081b2386
	adds r1, #1
	b .L_081b2388
.L_081b2386:
	subs r1, #1
.L_081b2388:
	mov r2, r9
	mov r3, r8
	ands r2, r3
	mov r9, r2
.L_081b2390:
	adds r0, #1
	cmp r0, r4
	bne .L_081b234c
	b .L_081b2428
.L_081b2398:
	cmp r6, #0
	bge .L_081b23b0
	mov r12, r8
	mov r8, r10
	mov r10, r12
	mov r12, r7
	mov r1, r10
	adds r7, r4, #0
	mov r2, r8
	mov r4, r12
	subs r6, r1, r2
	subs r5, r4, r7
.L_081b23b0:
	lsls r0, r5, #8
	cmp r5, #0
	bge .L_081b23ba
	subs r3, r7, r4
	lsls r0, r3, #8
.L_081b23ba:
	cmp r6, #0
	blt .L_081b23c2
	adds r1, r6, #0
	b .L_081b23c8
.L_081b23c2:
	mov r3, r8
	mov r6, r10
	subs r1, r3, r6
.L_081b23c8:
	bl Math_Div
	mov r12, r0
	mov r0, r8
	adds r1, r7, #0
	cmp r0, r10
	beq .L_081b2428
	ldr r2, .L_081b2438
	movs r7, #128
	movs r4, #7
	lsls r7, r7, #1
	mov lr, r2
.L_081b23e0:
	lsrs r2, r1, #3
	lsrs r3, r0, #3
	lsls r2, r2, #5
	adds r2, r2, r3
	adds r3, r1, #0
	ands r3, r4
	lsls r2, r2, #3
	adds r2, r2, r3
	adds r3, r0, #0
	ands r3, r4
	lsls r2, r2, #3
	mov r6, r11
	adds r2, r2, r3
	ldrb r3, [r6, r2]
	ldr r6, [sp, #36]
	cmp r3, r6
	bge .L_081b2406
	mov r3, r11
	strb r6, [r3, r2]
.L_081b2406:
	add r9, r12
	mov r3, r9
	ands r3, r7
	cmp r3, #0
	beq .L_081b2422
	cmp r5, #0
	ble .L_081b2418
	adds r1, #1
	b .L_081b241a
.L_081b2418:
	subs r1, #1
.L_081b241a:
	mov r6, r9
	mov r2, lr
	ands r6, r2
	mov r9, r6
.L_081b2422:
	adds r0, #1
	cmp r0, r10
	bne .L_081b23e0
.L_081b2428:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081b2438:
	.4byte 0xfffffeff
