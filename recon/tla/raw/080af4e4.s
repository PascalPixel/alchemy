.syntax unified
	.thumb
	.global Func_080af4e4
	.thumb_func
Func_080af4e4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	bl Owner_GetState
	movs r5, #42
	mov r9, r0
	adds r5, #255
	movs r0, #88
	add r0, r9
	add r5, r9
	mov r8, r0
	ldrb r0, [r5]
	bl Owner_GetRecordStride84
	ldrb r3, [r5]
	movs r4, #192
	mov r11, r0
	lsls r4, r4, #8
	ldr r1, .L_080af524
	mov r2, r8
	movs r6, #31
	movs r0, #0
	cmp r3, #0
	bne .L_080af522
	b .L_080af6e6
.L_080af522:
	b .L_080af528
.L_080af524:
	.4byte 0x00000000
.L_080af528:
	ldrh r3, [r2]
	ands r3, r4
	cmp r3, #0
	beq .L_080af532
	strh r1, [r2]
.L_080af532:
	subs r6, #1
	adds r2, #4
	cmp r6, #0
	bge .L_080af528
	mov r1, r8
	movs r4, #31
	movs r6, #31
	adds r1, #124
.L_080af542:
	lsls r3, r4, #2
	mov r0, r8
	ldrh r2, [r3, r0]
	adds r3, r2, #0
	cmp r3, #0
	bne .L_080af552
	subs r4, #1
	b .L_080af55a
.L_080af552:
	strh r2, [r1]
	subs r4, #1
	subs r1, #4
	subs r6, #1
.L_080af55a:
	cmp r4, #0
	bge .L_080af542
	cmp r6, #0
	blt .L_080af578
	ldr r2, .L_080af574
	lsls r3, r6, #2
	add r3, r8
.L_080af568:
	subs r6, #1
	strh r2, [r3]
	subs r3, #4
	cmp r6, #0
	bge .L_080af568
	b .L_080af578
.L_080af574:
	.4byte 0x00000000
.L_080af578:
	movs r1, #16
	add r1, r11
	mov lr, r1
	movs r2, #16
	movs r6, #0
	mov r10, r2
	mov r7, lr
.L_080af586:
	ldrh r3, [r7]
	cmp r3, #0
	beq .L_080af5f4
	mov r3, r9
	mov r0, lr
	ldrb r2, [r3, #15]
	ldrb r3, [r0, #2]
	cmp r2, r3
	bcc .L_080af5f4
	mov r1, r8
	ldrh r5, [r1]
	ldrh r3, [r0]
	mov r12, r5
	movs r4, #0
	cmp r12, r3
	beq .L_080af5bc
	mov r12, r10
.L_080af5a8:
	adds r4, #1
	cmp r4, #31
	bgt .L_080af5bc
	adds r1, #4
	mov r3, r11
	mov r0, r12
	ldrh r2, [r1]
	ldrh r3, [r3, r0]
	cmp r2, r3
	bne .L_080af5a8
.L_080af5bc:
	cmp r4, #32
	bne .L_080af5f4
	adds r3, r5, #0
	movs r4, #0
	cmp r3, #0
	bne .L_080af5d4
	ldrh r3, [r7]
	movs r1, #128
	lsls r1, r1, #8
	orrs r3, r1
	mov r2, r8
	b .L_080af5ee
.L_080af5d4:
	adds r4, #1
	cmp r4, #31
	bgt .L_080af5f0
	lsls r3, r4, #2
	mov r0, r8
	adds r2, r3, r0
	ldrh r3, [r2]
	cmp r3, #0
	bne .L_080af5d4
	ldrh r3, [r7]
	movs r1, #128
	lsls r1, r1, #8
	orrs r3, r1
.L_080af5ee:
	strh r3, [r2]
.L_080af5f0:
	cmp r4, #32
	beq .L_080af602
.L_080af5f4:
	movs r2, #4
	adds r6, #1
	add lr, r2
	adds r7, #4
	add r10, r2
	cmp r6, #15
	ble .L_080af586
.L_080af602:
	movs r3, #216
	movs r6, #0
	mov r10, r3
.L_080af608:
	mov r0, r10
	mov r1, r9
	ldrh r2, [r0, r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_080af694
	ldr r3, .L_080af63c
	ands r3, r2
	cmp r3, #0
	beq .L_080af694
	ldrh r0, [r0, r1]
	bl Item_GetDirect
	ldrb r3, [r0, #12]
	cmp r3, #3
	bne .L_080af694
	mov r2, r8
	ldrh r5, [r2]
	ldr r3, .L_080af640
	ldrh r0, [r0, #40]
	ands r3, r5
	mov r12, r0
	movs r4, #0
	cmp r3, r12
	beq .L_080af65e
	b .L_080af644
.L_080af63c:
	.4byte 0x00000200
.L_080af640:
	.4byte 0x00003fff
.L_080af644:
	movs r7, #252
	lsls r7, r7, #6
	adds r7, #255
	mov r1, r8
.L_080af64c:
	adds r4, #1
	cmp r4, #31
	bgt .L_080af65e
	adds r1, #4
	ldrh r2, [r1]
	adds r3, r7, #0
	ands r3, r2
	cmp r3, r12
	bne .L_080af64c
.L_080af65e:
	cmp r4, #32
	bne .L_080af694
	adds r3, r5, #0
	movs r4, #0
	cmp r3, #0
	bne .L_080af678
	ldr r3, .L_080af674
	orrs r3, r0
	mov r0, r8
	strh r3, [r0]
	b .L_080af690
.L_080af674:
	.4byte 0x00004000
.L_080af678:
	adds r4, #1
	cmp r4, #31
	bgt .L_080af690
	lsls r3, r4, #2
	mov r1, r8
	adds r2, r3, r1
	ldrh r3, [r2]
	cmp r3, #0
	bne .L_080af678
	ldr r3, .L_080af6b4
	orrs r3, r0
	strh r3, [r2]
.L_080af690:
	cmp r4, #32
	beq .L_080af69e
.L_080af694:
	movs r2, #2
	adds r6, #1
	add r10, r2
	cmp r6, #14
	ble .L_080af608
.L_080af69e:
	movs r4, #0
	movs r6, #0
	mov r1, r8
.L_080af6a4:
	lsls r3, r4, #2
	mov r0, r8
	ldrh r2, [r3, r0]
	adds r3, r2, #0
	cmp r3, #0
	bne .L_080af6b8
	adds r4, #1
	b .L_080af6c0
.L_080af6b4:
	.4byte 0x00004000
.L_080af6b8:
	strh r2, [r1]
	adds r4, #1
	adds r1, #4
	adds r6, #1
.L_080af6c0:
	cmp r4, #31
	ble .L_080af6a4
	cmp r6, #31
	bgt .L_080af6e4
	lsls r3, r6, #2
	mov r0, r8
	ldr r1, .L_080af6e0
	adds r2, r3, r0
	movs r3, #32
	subs r6, r3, r6
.L_080af6d4:
	subs r6, #1
	strh r1, [r2]
	adds r2, #4
	cmp r6, #0
	bne .L_080af6d4
	b .L_080af6e4
.L_080af6e0:
	.4byte 0x00000000
.L_080af6e4:
	movs r0, #0
.L_080af6e6:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
