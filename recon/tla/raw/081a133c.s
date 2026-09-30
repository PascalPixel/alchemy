.syntax unified
	.thumb
	.global Func_081a133c
	.thumb_func
Func_081a133c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_081a14e4
	ldr r2, .L_081a14e8
	ldr r5, [r3]
	movs r0, #0
	ldrsh r3, [r2, r0]
	ldrh r1, [r2]
	cmp r3, #63
	bgt .L_081a138c
	adds r3, r1, #1
	strh r3, [r2]
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r3, #0
	bge .L_081a1360
	adds r3, #3
.L_081a1360:
	asrs r1, r3, #2
	ldr r3, .L_081a14ec
	ldrh r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_081a137c
	movs r3, #16
	subs r3, r3, r1
	movs r2, #128
	lsls r3, r3, #8
	lsls r2, r2, #19
	orrs r3, r1
	b .L_081a1388
.L_081a137c:
	movs r2, #16
	subs r2, r2, r1
	lsls r3, r1, #8
	orrs r3, r2
	movs r2, #128
	lsls r2, r2, #19
.L_081a1388:
	adds r2, #82
	strh r3, [r2]
.L_081a138c:
	ldr r3, .L_081a14f0
	ldr r1, .L_081a14f4
	movs r4, #0
	ldrsh r2, [r3, r4]
	movs r6, #0
	ldrsh r3, [r1, r6]
	adds r0, r1, #0
	cmp r2, r3
	beq .L_081a13c4
	subs r1, r2, r3
	adds r2, r1, #0
	cmp r1, #0
	bge .L_081a13a8
	negs r2, r1
.L_081a13a8:
	ldr r3, .L_081a14f8
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r2, r3
	ble .L_081a13bc
	cmp r1, #0
	ble .L_081a13ba
	adds r1, r3, #0
	b .L_081a13bc
.L_081a13ba:
	negs r1, r3
.L_081a13bc:
	ldrh r3, [r0]
	adds r3, r3, r1
	strh r3, [r0]
	adds r1, r0, #0
.L_081a13c4:
	ldr r0, .L_081a14e4
	movs r6, #0
	ldrsh r3, [r1, r6]
	adds r4, r0, #0
	lsls r2, r3, #16
	ldr r3, [r0]
	cmp r3, r2
	beq .L_081a1420
	subs r1, r2, r3
	adds r2, r1, #0
	cmp r1, #0
	bge .L_081a13de
	negs r2, r1
.L_081a13de:
	ldr r0, .L_081a14f8
	movs r6, #0
	ldrsh r3, [r0, r6]
	lsls r3, r3, #14
	cmp r2, r3
	ble .L_081a13f4
	adds r3, r1, #0
	cmp r1, #0
	bge .L_081a13f2
	adds r3, r1, #7
.L_081a13f2:
	asrs r1, r3, #3
.L_081a13f4:
	adds r2, r1, #0
	cmp r1, #0
	bge .L_081a13fc
	negs r2, r1
.L_081a13fc:
	movs r6, #0
	ldrsh r3, [r0, r6]
	lsls r3, r3, #16
	cmp r2, r3
	ble .L_081a1410
	cmp r1, #0
	ble .L_081a140e
	adds r1, r3, #0
	b .L_081a1410
.L_081a140e:
	negs r1, r3
.L_081a1410:
	ldr r3, [r4]
	ldr r2, .L_081a14fc
	adds r3, r3, r1
	str r3, [r4]
	adds r0, r4, #0
	ldr r3, [r2]
	adds r3, r3, r1
	str r3, [r2]
.L_081a1420:
	ldr r3, [r0]
	movs r2, #128
	eors r3, r5
	lsls r2, r2, #12
	ands r3, r2
	cmp r3, #0
	beq .L_081a14d4
	ldr r3, .L_081a1500
	movs r7, #192
	ldr r6, [r3]
	ldr r3, .L_081a14fc
	lsls r7, r7, #19
	ldr r3, [r3]
	lsrs r3, r3, #19
	lsls r1, r3, #6
	ldr r3, .L_081a14ec
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_081a144a
	ldr r7, .L_081a1504
.L_081a144a:
	ldr r3, [r0]
	cmp r3, r5
	ble .L_081a1496
	lsrs r3, r3, #19
	movs r2, #240
	lsls r2, r2, #3
	lsls r3, r3, #6
	adds r3, r3, r2
	mov r12, r3
	movs r3, #224
	lsls r3, r3, #3
	adds r3, #255
	adds r4, r1, r2
	ands r4, r3
	movs r0, #128
	movs r3, #128
	lsls r3, r3, #5
	lsls r0, r0, #4
	movs r5, #0
	mov r8, r3
	mov lr, r0
.L_081a1474:
	mov r2, r12
	adds r0, r6, r2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r1, r7, r4
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r5, #1
	add r6, r8
	add r7, lr
	cmp r5, #14
	ble .L_081a1474
	b .L_081a14d4
.L_081a1496:
	ldr r3, [r0]
	movs r4, #224
	lsrs r3, r3, #19
	lsls r3, r3, #6
	mov r12, r3
	lsls r4, r4, #3
	movs r3, #128
	movs r0, #128
	adds r4, #255
	lsls r3, r3, #5
	lsls r0, r0, #4
	ands r4, r1
	mov r8, r3
	mov lr, r0
	movs r5, #14
.L_081a14b4:
	mov r2, r12
	adds r0, r6, r2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r1, r7, r4
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	subs r5, #1
	add r6, r8
	add r7, lr
	cmp r5, #0
	bge .L_081a14b4
.L_081a14d4:
	ldr r3, .L_081a14fc
	ldrh r1, [r3, #2]
	ldr r3, .L_081a1508
	strh r1, [r3, #8]
	strh r1, [r3, #12]
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_081a14e4:
	.4byte Data_0200751c
.L_081a14e8:
	.4byte Data_02007516
.L_081a14ec:
	.4byte Data_02007514
.L_081a14f0:
	.4byte gScrollTarget
.L_081a14f4:
	.4byte Data_02007520
.L_081a14f8:
	.4byte Data_02007524
.L_081a14fc:
	.4byte Data_02007518
.L_081a1500:
	.4byte Data_02007528
.L_081a1504:
	.4byte 0x06008000
.L_081a1508:
	.4byte Data_03001120
