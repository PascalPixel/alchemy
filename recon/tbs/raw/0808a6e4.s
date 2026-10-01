.syntax unified
	.thumb
	.global Party_SetReturnPoint
	.thumb_func
Party_SetReturnPoint:
	push {r5, r6, lr}
	ldr r1, .L_0808a8c8
	ldr r2, .L_0808a8cc
	movs r4, #1
	adds r3, r1, r2
	negs r4, r4
	strh r0, [r3]
	cmp r0, r4
	beq .L_0808a6f8
	b .L_0808a846
.L_0808a6f8:
	movs r4, #250
	lsls r4, r4, #1
	adds r3, r1, r4
	ldr r0, [r3]
	bl Owner_GetStateFar
	adds r6, r0, #0
	movs r5, #56
	ldrsh r3, [r6, r5]
	cmp r3, #0
	bne .L_0808a774
	movs r5, #1
	strh r5, [r6, #56]
	lsls r5, r5, #14
	movs r0, #52
	ldrsh r1, [r6, r0]
	adds r0, r5, #0
	bl FixedPoint_Ratio
	movs r1, #128
	lsls r1, r1, #7
	cmp r0, r1
	bgt .L_0808a72e
	movs r5, #0
	cmp r0, #0
	blt .L_0808a72e
	adds r5, r0, #0
.L_0808a72e:
	lsls r3, r5, #16
	strh r5, [r6, #20]
	cmp r3, #0
	bne .L_0808a742
	movs r2, #56
	ldrsh r3, [r6, r2]
	cmp r3, #0
	beq .L_0808a742
	movs r3, #1
	strh r3, [r6, #20]
.L_0808a742:
	movs r3, #58
	ldrsh r0, [r6, r3]
	movs r4, #54
	ldrsh r1, [r6, r4]
	lsls r0, r0, #14
	bl FixedPoint_Ratio
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_0808a760
	movs r3, #0
	cmp r0, #0
	blt .L_0808a760
	adds r3, r0, #0
.L_0808a760:
	strh r3, [r6, #22]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0808a774
	movs r5, #58
	ldrsh r3, [r6, r5]
	cmp r3, #0
	beq .L_0808a774
	movs r3, #1
	strh r3, [r6, #22]
.L_0808a774:
	movs r0, #32
	bl GameFlag_IsSet
	cmp r0, #0
	beq .L_0808a7f6
	movs r5, #0
.L_0808a780:
	adds r0, r5, #0
	bl Owner_GetStateFar
	adds r6, r0, #0
	ldrh r1, [r6, #52]
	ldrh r3, [r6, #54]
	strh r1, [r6, #56]
	strh r3, [r6, #58]
	lsls r1, r1, #16
	asrs r1, r1, #16
	lsls r0, r1, #14
	bl FixedPoint_Ratio
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_0808a7aa
	movs r3, #0
	cmp r0, #0
	blt .L_0808a7aa
	adds r3, r0, #0
.L_0808a7aa:
	strh r3, [r6, #20]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0808a7be
	movs r0, #56
	ldrsh r3, [r6, r0]
	cmp r3, #0
	beq .L_0808a7be
	movs r3, #1
	strh r3, [r6, #20]
.L_0808a7be:
	movs r1, #58
	ldrsh r0, [r6, r1]
	movs r2, #54
	ldrsh r1, [r6, r2]
	lsls r0, r0, #14
	bl FixedPoint_Ratio
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_0808a7dc
	movs r3, #0
	cmp r0, #0
	blt .L_0808a7dc
	adds r3, r0, #0
.L_0808a7dc:
	strh r3, [r6, #22]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0808a7f0
	movs r4, #58
	ldrsh r3, [r6, r4]
	cmp r3, #0
	beq .L_0808a7f0
	movs r3, #1
	strh r3, [r6, #22]
.L_0808a7f0:
	adds r5, #1
	cmp r5, #1
	ble .L_0808a780
.L_0808a7f6:
	ldr r1, .L_0808a8c8
	movs r5, #233
	lsls r5, r5, #1
	movs r4, #234
	adds r3, r1, r5
	lsls r4, r4, #1
	movs r0, #0
	ldrsh r2, [r3, r0]
	adds r3, r1, r4
	movs r5, #0
	ldrsh r0, [r3, r5]
	movs r3, #1
	negs r3, r3
	cmp r2, r3
	bne .L_0808a836
	cmp r0, r2
	bne .L_0808a83e
	subs r4, #16
	adds r3, r1, r4
	movs r5, #224
	ldrh r2, [r3]
	lsls r5, r5, #1
	adds r3, r1, r5
	movs r0, #227
	strh r2, [r3]
	lsls r0, r0, #1
	adds r3, r1, r0
	ldrh r3, [r3]
	subs r4, #2
	adds r2, r1, r4
	strh r3, [r2]
	b .L_0808a8c2
.L_0808a836:
	movs r5, #224
	lsls r5, r5, #1
	adds r3, r1, r5
	b .L_0808a874
.L_0808a83e:
	movs r2, #228
	lsls r2, r2, #1
	adds r3, r1, r2
	b .L_0808a86c
.L_0808a846:
	movs r0, #231
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r5, #0
	ldrsh r2, [r3, r5]
	adds r0, #2
	adds r3, r1, r0
	movs r5, #0
	ldrsh r0, [r3, r5]
	cmp r2, r4
	bne .L_0808a862
	cmp r0, r2
	beq .L_0808a89e
	b .L_0808a866
.L_0808a862:
	movs r4, #224
	b .L_0808a870
.L_0808a866:
	movs r5, #228
	lsls r5, r5, #1
	adds r3, r1, r5
.L_0808a86c:
	movs r4, #224
	ldrh r2, [r3]
.L_0808a870:
	lsls r4, r4, #1
	adds r3, r1, r4
.L_0808a874:
	strh r2, [r3]
	movs r5, #1
	negs r5, r5
	cmp r0, r5
	beq .L_0808a88a
	ldr r3, .L_0808a8c8
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r3, r1
	strh r0, [r3]
	b .L_0808a8c2
.L_0808a88a:
	ldr r2, .L_0808a8c8
	movs r4, #229
	lsls r4, r4, #1
	adds r3, r2, r4
	movs r5, #225
	ldrh r3, [r3]
	lsls r5, r5, #1
	adds r2, r2, r5
	strh r3, [r2]
	b .L_0808a8c2
.L_0808a89e:
	movs r0, #228
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r4, #224
	ldrh r2, [r3]
	lsls r4, r4, #1
	adds r3, r1, r4
	movs r5, #229
	strh r2, [r3]
	lsls r5, r5, #1
	adds r3, r1, r5
	subs r0, #6
	ldrh r3, [r3]
	adds r2, r1, r0
	strh r3, [r2]
	subs r0, #185
	bl GameFlag_SetBitFar
.L_0808a8c2:
	pop {r5, r6}
	pop {r0}
	bx r0
.L_0808a8c8:
	.4byte gCell
.L_0808a8cc:
	.4byte 0x00000236
