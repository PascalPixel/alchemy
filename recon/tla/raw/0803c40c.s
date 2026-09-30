.syntax unified
	.thumb
	.global Func_0803c40c
	.thumb_func
Func_0803c40c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	ldrb r3, [r6, #5]
	movs r7, #128
	adds r5, r6, #0
	sub sp, #8
	lsls r7, r7, #1
	adds r5, #16
	cmp r3, #9
	beq .L_0803c436
	cmp r3, #9
	blt .L_0803c486
	cmp r3, #10
	beq .L_0803c448
	cmp r3, #11
	beq .L_0803c45a
	cmp r3, #12
	beq .L_0803c470
	b .L_0803c486
.L_0803c436:
	ldrh r2, [r6, #12]
	ldr r1, .L_0803c508
	adds r3, r2, #1
	strh r3, [r6, #12]
	movs r3, #31
	ands r3, r2
	lsls r3, r3, #1
	ldrh r7, [r1, r3]
	b .L_0803c486
.L_0803c448:
	ldrh r2, [r6, #12]
	ldr r1, .L_0803c508
	adds r3, r2, #1
	strh r3, [r6, #12]
	movs r3, #31
	ands r3, r2
	lsls r3, r3, #1
	ldrh r3, [r1, r3]
	b .L_0803c484
.L_0803c45a:
	ldrh r3, [r6, #12]
	adds r0, r3, #0
	cmp r0, #7
	bhi .L_0803c486
	adds r3, #1
	ldr r2, .L_0803c508
	strh r3, [r6, #12]
	lsls r3, r0, #2
	adds r3, #32
	ldrh r7, [r2, r3]
	b .L_0803c486
.L_0803c470:
	ldrh r3, [r6, #12]
	adds r1, r3, #0
	cmp r1, #7
	bhi .L_0803c486
	adds r3, #1
	ldr r2, .L_0803c508
	strh r3, [r6, #12]
	lsls r3, r1, #2
	adds r3, #32
	ldrh r3, [r2, r3]
.L_0803c484:
	lsrs r7, r3, #1
.L_0803c486:
	movs r3, #128
	lsls r3, r3, #1
	mov r8, r3
	cmp r7, r8
	bne .L_0803c4a4
	ldrb r2, [r5, #7]
	movs r3, #63
	negs r3, r3
	ands r3, r2
	ldrb r2, [r5, #5]
	strb r3, [r5, #7]
	movs r3, #4
	negs r3, r3
	ands r3, r2
	b .L_0803c520
.L_0803c4a4:
	ldr r3, [sp, #0]
	ldr r4, .L_0803c50c
	movs r2, #255
	adds r1, r7, #0
	ands r3, r4
	lsls r2, r2, #8
	adds r2, #255
	orrs r3, r1
	ands r3, r2
	lsls r1, r1, #16
	orrs r3, r1
	str r3, [sp, #0]
	mov r0, sp
	ldr r3, [r0, #4]
	ands r3, r4
	str r3, [r0, #4]
	bl Func_0801401c
	movs r3, #31
	ldrb r2, [r5, #7]
	ands r0, r3
	movs r3, #63
	negs r3, r3
	lsls r0, r0, #1
	ands r3, r2
	orrs r3, r0
	strb r3, [r5, #7]
	cmp r7, r8
	ble .L_0803c514
	ldrb r3, [r5, #5]
	movs r2, #3
	orrs r3, r2
	strb r3, [r5, #5]
	movs r3, #255
	ldrh r2, [r6, #6]
	lsls r3, r3, #8
	adds r3, #248
	adds r2, r2, r3
	ldr r3, .L_0803c504
	ldrh r1, [r5, #6]
	ands r2, r3
	ldr r3, .L_0803c510
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #6]
	ldrb r3, [r6, #8]
	adds r3, #248
	b .L_0803c538
.L_0803c504:
	.4byte 0x000001ff
.L_0803c508:
	.4byte Data_0805e9c4
.L_0803c50c:
	.4byte 0xffff0000
.L_0803c510:
	.4byte 0xfffffe00
.L_0803c514:
	ldrb r2, [r5, #5]
	movs r3, #4
	negs r3, r3
	ands r3, r2
	movs r2, #1
	orrs r3, r2
.L_0803c520:
	strb r3, [r5, #5]
	movs r2, #128
	ldrh r3, [r6, #6]
	lsls r2, r2, #1
	adds r2, #255
	ands r2, r3
	ldrh r1, [r5, #6]
	ldr r3, .L_0803c544
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #6]
	ldrh r3, [r6, #8]
.L_0803c538:
	strb r3, [r5, #4]
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0803c544:
	.4byte 0xfffffe00
