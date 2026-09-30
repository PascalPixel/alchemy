.syntax unified
	.thumb
	.global Func_0811b4d8
	.thumb_func
Func_0811b4d8:
	push {r5, r6, lr}
	adds r6, r1, #0
	adds r3, r6, #0
	adds r3, #43
	ldrb r3, [r3]
	movs r5, #0
	cmp r3, #0
	bne .L_0811b596
	bl Owner_GetState
	movs r1, #50
	adds r2, r0, #0
	adds r1, #255
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #1
	bne .L_0811b500
	movs r5, #1
.L_0811b500:
	cmp r3, #2
	bne .L_0811b506
	orrs r5, r3
.L_0811b506:
	movs r1, #156
	lsls r1, r1, #1
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811b516
	movs r3, #32
	orrs r5, r3
.L_0811b516:
	movs r1, #60
	adds r1, #255
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811b54e
	movs r3, #4
	adds r1, #15
	orrs r5, r3
	adds r3, r2, r1
	ldrh r3, [r3]
	adds r1, #18
	cmp r3, r1
	beq .L_0811b548
	cmp r3, r1
	bgt .L_0811b540
	cmp r3, #97
	bgt .L_0811b54e
	cmp r3, #91
	blt .L_0811b54e
	b .L_0811b548
.L_0811b540:
	movs r1, #102
	adds r1, #255
	cmp r3, r1
	bne .L_0811b54e
.L_0811b548:
	movs r3, #5
	negs r3, r3
	ands r5, r3
.L_0811b54e:
	movs r1, #62
	adds r1, #255
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811b55e
	movs r3, #8
	orrs r5, r3
.L_0811b55e:
	movs r1, #160
	lsls r1, r1, #1
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811b56e
	movs r3, #64
	orrs r5, r3
.L_0811b56e:
	movs r1, #158
	lsls r1, r1, #1
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811b57e
	movs r3, #16
	orrs r5, r3
.L_0811b57e:
	movs r3, #66
	adds r3, #255
	adds r2, r2, r3
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_0811b594
	adds r2, r3, #0
	adds r2, #6
	movs r3, #1
	lsls r3, r2
	orrs r5, r3
.L_0811b594:
	strh r5, [r6, #28]
.L_0811b596:
	pop {r5, r6, pc}
