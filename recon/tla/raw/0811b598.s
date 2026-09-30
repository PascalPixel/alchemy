.syntax unified
	.thumb
	.global Func_0811b598
	.thumb_func
Func_0811b598:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r3, #0
	mov r8, r3
	movs r3, #31
	ldrsb r3, [r5, r3]
	ldrb r2, [r5, #31]
	cmp r3, #0
	blt .L_0811b5b2
	subs r3, r2, #1
	strb r3, [r5, #31]
.L_0811b5b2:
	movs r3, #28
	ldrsh r2, [r5, r3]
	ldrh r4, [r5, #28]
	cmp r2, #0
	beq .L_0811b5c4
	ldr r1, [r5, #32]
	cmp r1, #0
	beq .L_0811b5e0
	b .L_0811b5c6
.L_0811b5c4:
	ldr r1, [r5, #32]
.L_0811b5c6:
	cmp r1, #0
	beq .L_0811b5d8
	ldrb r3, [r5, #30]
	asrs r2, r3
	adds r3, r2, #0
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0811b5e0
.L_0811b5d8:
	movs r3, #31
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_0811b69c
.L_0811b5e0:
	lsls r3, r4, #16
	movs r6, #1
	asrs r3, r3, #16
	negs r6, r6
	ldr r0, [r5]
	cmp r3, #0
	beq .L_0811b61e
	ldrb r2, [r5, #30]
	mov r12, r3
	adds r6, r2, #1
	movs r4, #1
	b .L_0811b5fa
.L_0811b5f8:
	adds r6, #1
.L_0811b5fa:
	cmp r6, #13
	ble .L_0811b600
	movs r6, #0
.L_0811b600:
	mov r3, r12
	asrs r3, r6
	ands r3, r4
	cmp r3, #0
	beq .L_0811b5f8
	cmp r2, r6
	bne .L_0811b612
	cmp r1, #0
	bne .L_0811b618
.L_0811b612:
	strb r6, [r5, #30]
	movs r3, #1
	mov r8, r3
.L_0811b618:
	movs r3, #80
	strb r3, [r5, #31]
	b .L_0811b622
.L_0811b61e:
	movs r3, #1
	mov r8, r3
.L_0811b622:
	movs r1, #0
	bl GetMotionRecord
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0811b69c
	cmp r6, #0
	blt .L_0811b646
	ldrb r3, [r7, #20]
	cmp r3, #32
	bne .L_0811b640
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #190
	b .L_0811b644
.L_0811b640:
	movs r3, #172
	lsls r3, r3, #2
.L_0811b644:
	adds r6, r6, r3
.L_0811b646:
	ldr r1, [r5, #32]
	cmp r1, #0
	beq .L_0811b65c
	mov r3, r8
	cmp r3, #0
	beq .L_0811b65c
	adds r0, r7, #0
	bl Func_08020060
	movs r3, #0
	str r3, [r5, #32]
.L_0811b65c:
	cmp r6, #0
	blt .L_0811b68c
	mov r3, r8
	cmp r3, #0
	beq .L_0811b68c
	adds r0, r7, #0
	adds r1, r6, #0
	bl ResourceMetadata_RegisterFar
	movs r3, #1
	negs r3, r3
	str r0, [r5, #32]
	cmp r0, r3
	bne .L_0811b67c
	movs r3, #0
	str r3, [r5, #32]
.L_0811b67c:
	ldr r0, [r5, #32]
	cmp r0, #0
	beq .L_0811b68c
	movs r3, #3
	strb r3, [r0, #6]
	movs r1, #0
	bl Func_08020080
.L_0811b68c:
	movs r3, #1
	strb r3, [r7, #25]
	cmp r6, #0
	blt .L_0811b698
	strh r6, [r5, #8]
	b .L_0811b69c
.L_0811b698:
	movs r3, #0
	strh r3, [r5, #8]
.L_0811b69c:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
