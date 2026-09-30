.syntax unified
	.thumb
	.global Func_080af7ac
	.thumb_func
Func_080af7ac:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r1, .L_080af858
	sub sp, #32
	mov r10, sp
	mov r8, r1
	movs r6, #0
	mov r9, r10
.L_080af7c2:
	adds r0, r6, #0
	bl Owner_GetState
	adds r7, r0, #0
	ldr r0, .L_080af85c
	mov r1, r9
	adds r0, r6, r0
	bl Ui_AdjustValueWithoutLimitFar
	mov r2, r9
	ldrh r3, [r2]
	movs r5, #0
	strb r3, [r7]
	ldrh r3, [r2]
	cmp r3, #0
	beq .L_080af7fc
	mov r1, r10
	adds r2, r7, #0
	movs r0, #0
.L_080af7e8:
	adds r5, #1
	adds r0, #2
	cmp r5, #13
	bgt .L_080af7fc
	ldrh r3, [r0, r1]
	adds r2, #1
	strb r3, [r2]
	ldrh r3, [r0, r1]
	cmp r3, #0
	bne .L_080af7e8
.L_080af7fc:
	movs r3, #0
	adds r6, #1
	strb r3, [r7, #14]
	cmp r6, #7
	ble .L_080af7c2
	mov r3, r8
	ldr r0, [r3]
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	beq .L_080af8be
.L_080af812:
	bl Owner_GetState
	adds r7, r0, #0
	cmp r7, #0
	beq .L_080af8ae
	mov r2, r8
	ldr r3, [r2]
	movs r1, #165
	lsls r1, r1, #1
	adds r2, r7, r1
	strh r3, [r2]
	movs r5, #14
	ldrh r0, [r2]
	bl Owner_GetRecordStride180
	ldr r2, .L_080af854
	adds r3, r7, #0
	mov r10, r0
	adds r3, #244
.L_080af838:
	subs r5, #1
	strh r2, [r3]
	subs r3, #2
	cmp r5, #0
	bge .L_080af838
	movs r2, #128
	lsls r2, r2, #1
	mov r6, r10
	adds r2, #255
	movs r5, #0
	adds r6, #152
	mov r9, r2
	b .L_080af860
	.2byte 0x0000
.L_080af854:
	.4byte 0x00000000
.L_080af858:
	.4byte Data_080b2340
.L_080af85c:
	.4byte 0x00000083
.L_080af860:
	mov r3, r8
	ldr r0, [r3]
	ldrh r3, [r6]
	mov r1, r9
	ands r1, r3
	bl Inventory_AddItem
	mov r2, r8
	adds r1, r0, #0
	adds r5, #1
	ldr r0, [r2]
	adds r6, #2
	bl Func_080aef34
	cmp r5, #12
	bls .L_080af860
	mov r3, r8
	ldr r0, [r3]
	bl Func_080b0298
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r7, #22]
	strh r3, [r7, #20]
	mov r3, r10
	adds r3, #150
	mov r1, r8
	ldr r0, [r1]
	ldrb r1, [r3]
	bl Party_AdvanceOwnerCountToTarget
	mov r2, r8
	ldr r0, [r2]
	bl Func_080b0298
	mov r3, r8
	ldr r0, [r3]
	bl Owner_RecalculateStats
.L_080af8ae:
	movs r1, #4
	add r8, r1
	mov r2, r8
	ldr r3, [r2]
	subs r1, #5
	adds r0, r3, #0
	cmp r3, r1
	bne .L_080af812
.L_080af8be:
	add sp, #32
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
