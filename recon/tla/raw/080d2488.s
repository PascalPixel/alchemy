.syntax unified
	.thumb
	.global Party_RemoveOwnerRestored
	.thumb_func
Party_RemoveOwnerRestored:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	bl Func_080ad110
	bl Event_ClearValidPackedIds
	adds r0, r5, #0
	bl Owner_GetState
	adds r6, r0, #0
	ldrh r1, [r6, #52]
	ldrh r3, [r6, #54]
	strh r1, [r6, #56]
	strh r3, [r6, #58]
	lsls r1, r1, #16
	asrs r1, r1, #16
	lsls r0, r1, #14
	bl Math_Div
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_080d24c2
	movs r3, #0
	cmp r0, #0
	blt .L_080d24c2
	adds r3, r0, #0
.L_080d24c2:
	strh r3, [r6, #20]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_080d24d6
	movs r2, #56
	ldrsh r3, [r6, r2]
	cmp r3, #0
	beq .L_080d24d6
	movs r3, #1
	strh r3, [r6, #20]
.L_080d24d6:
	movs r3, #58
	ldrsh r0, [r6, r3]
	movs r2, #54
	ldrsh r1, [r6, r2]
	lsls r0, r0, #14
	bl Math_Div
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_080d24f4
	movs r3, #0
	cmp r0, #0
	blt .L_080d24f4
	adds r3, r0, #0
.L_080d24f4:
	strh r3, [r6, #22]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_080d2508
	movs r2, #58
	ldrsh r3, [r6, r2]
	cmp r3, #0
	beq .L_080d2508
	movs r3, #1
	strh r3, [r6, #22]
.L_080d2508:
	movs r3, #50
	adds r3, #255
	adds r2, r6, r3
	movs r3, #0
	strb r3, [r2]
	mov r8, r3
	bl Party_CountActiveOwnersFar
	cmp r8, r0
	bge .L_080d2542
	ldr r3, .L_080d25c4
	movs r2, #134
	lsls r2, r2, #2
	adds r7, r3, r2
	adds r5, r0, #0
.L_080d2526:
	ldrb r0, [r7]
	bl Owner_GetState
	adds r6, r0, #0
	movs r2, #56
	ldrsh r3, [r6, r2]
	adds r7, #1
	cmp r3, #0
	beq .L_080d253c
	movs r3, #1
	add r8, r3
.L_080d253c:
	subs r5, #1
	cmp r5, #0
	bne .L_080d2526
.L_080d2542:
	mov r2, r8
	cmp r2, #0
	bne .L_080d25be
	ldr r3, .L_080d25c4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Owner_GetState
	movs r5, #1
	adds r6, r0, #0
	strh r5, [r6, #56]
	lsls r5, r5, #14
	movs r3, #52
	ldrsh r1, [r6, r3]
	adds r0, r5, #0
	bl Math_Div
	movs r2, #128
	lsls r2, r2, #7
	cmp r0, r2
	bgt .L_080d2578
	movs r5, #0
	cmp r0, #0
	blt .L_080d2578
	adds r5, r0, #0
.L_080d2578:
	lsls r3, r5, #16
	strh r5, [r6, #20]
	cmp r3, #0
	bne .L_080d258c
	movs r2, #56
	ldrsh r3, [r6, r2]
	cmp r3, #0
	beq .L_080d258c
	movs r3, #1
	strh r3, [r6, #20]
.L_080d258c:
	movs r3, #58
	ldrsh r0, [r6, r3]
	movs r2, #54
	ldrsh r1, [r6, r2]
	lsls r0, r0, #14
	bl Math_Div
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_080d25aa
	movs r3, #0
	cmp r0, #0
	blt .L_080d25aa
	adds r3, r0, #0
.L_080d25aa:
	strh r3, [r6, #22]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_080d25be
	movs r2, #58
	ldrsh r3, [r6, r2]
	cmp r3, #0
	beq .L_080d25be
	movs r3, #1
	strh r3, [r6, #22]
.L_080d25be:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080d25c4:
	.4byte gPartyState
