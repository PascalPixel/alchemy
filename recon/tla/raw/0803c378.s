.syntax unified
	.thumb
	.global Func_0803c378
	.thumb_func
Func_0803c378:
	push {r5, r6, r7, lr}
	adds r4, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	adds r5, r2, #0
	adds r6, r3, #0
	ldrh r3, [r0, #10]
	adds r4, #1
	subs r3, #1
	adds r7, r1, #0
	ldr r2, [sp, #16]
	adds r6, #8
	adds r5, #1
	cmp r4, r3
	bhi .L_0803c408
	ldrh r3, [r0, #8]
	subs r3, #1
	cmp r5, r3
	bhi .L_0803c408
	movs r1, #240
	lsls r1, r1, #8
	cmp r2, #3
	beq .L_0803c3c0
	cmp r2, #3
	bhi .L_0803c3b6
	movs r1, #224
	lsls r1, r1, #8
	cmp r2, #2
	beq .L_0803c3c0
	b .L_0803c3be
.L_0803c3b6:
	movs r1, #128
	lsls r1, r1, #5
	cmp r2, #4
	beq .L_0803c3c0
.L_0803c3be:
	movs r1, #0
.L_0803c3c0:
	cmp r2, #1
	beq .L_0803c408
	cmp r2, #1
	bcc .L_0803c3ec
	cmp r2, #4
	bhi .L_0803c3ec
	movs r3, #14
	ldrsh r2, [r0, r3]
	adds r2, r2, r4
	movs r4, #12
	ldrsh r3, [r0, r4]
	lsls r2, r2, #5
	adds r3, r3, r5
	adds r0, r2, r3
	movs r3, #160
	lsls r3, r3, #2
	cmp r0, r3
	bcs .L_0803c408
	lsls r3, r0, #1
	orrs r1, r7
	strh r1, [r6, r3]
	b .L_0803c408
.L_0803c3ec:
	movs r1, #14
	ldrsh r2, [r0, r1]
	movs r1, #160
	adds r2, r2, r4
	movs r4, #12
	ldrsh r3, [r0, r4]
	lsls r2, r2, #5
	adds r3, r3, r5
	adds r0, r2, r3
	lsls r1, r1, #2
	cmp r0, r1
	bcs .L_0803c408
	lsls r3, r0, #1
	strh r7, [r6, r3]
.L_0803c408:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
