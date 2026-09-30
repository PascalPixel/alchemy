.syntax unified
	.thumb
	.global Func_0812824c
	.thumb_func
Func_0812824c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	mov r11, r1
	bl Owner_GetState
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #36]
	mov r10, r0
	movs r0, #170
	lsls r0, r0, #3
	adds r7, r6, r0
	cmp r5, #7
	bhi .L_0812827a
	movs r0, #1
	negs r0, r0
	b .L_081284b4
.L_0812827a:
	movs r3, #42
	adds r3, #255
	add r3, r10
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0812828c
	movs r0, #2
	negs r0, r0
	b .L_081284b4
.L_0812828c:
	movs r5, #26
	ldrsb r5, [r7, r5]
	movs r1, #165
	adds r5, #1
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	lsls r3, r3, #1
	subs r5, r5, r3
	lsls r1, r1, #1
	strb r5, [r7, #26]
	add r1, r10
	lsls r5, r5, #24
	ldrh r2, [r1]
	asrs r5, r5, #24
	lsls r3, r5, #1
	adds r3, #20
	strh r2, [r7, r3]
	mov r0, r10
	mov r8, r1
	bl Func_08128228
	adds r5, #24
	strb r0, [r7, r5]
	ldrb r2, [r7, #27]
	movs r3, #27
	ldrsb r3, [r7, r3]
	cmp r3, #1
	bgt .L_081282ca
	adds r3, r2, #1
	strb r3, [r7, #27]
.L_081282ca:
	mov r5, r8
	ldrh r2, [r6, #16]
	ldrh r3, [r5]
	movs r4, #0
	movs r0, #0
	cmp r2, r3
	beq .L_081282f0
	adds r1, r6, #0
	mov r12, r8
	adds r1, #16
.L_081282de:
	adds r0, #1
	cmp r0, #5
	bgt .L_081282f0
	adds r1, #2
	mov r5, r12
	ldrh r2, [r1]
	ldrh r3, [r5]
	cmp r2, r3
	bne .L_081282de
.L_081282f0:
	cmp r0, #6
	beq .L_081282f6
	adds r4, r0, #0
.L_081282f6:
	ldrh r3, [r6, #62]
	cmp r3, #2
	beq .L_0812830e
	ldrh r3, [r6, #60]
	cmp r4, r3
	bge .L_08128304
	strh r4, [r6, #60]
.L_08128304:
	ldr r3, [r7, #8]
	cmp r3, #0
	beq .L_08128310
	movs r3, #1
	strh r3, [r6, #62]
.L_0812830e:
	ldr r3, [r7, #8]
.L_08128310:
	adds r3, #1
	movs r0, #116
	str r3, [r7, #8]
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_08128322
	b .L_081284b2
.L_08128322:
	movs r5, #165
	lsls r5, r5, #1
	add r5, r10
	ldrh r3, [r5]
	movs r0, #128
	lsls r0, r0, #2
	cmp r3, r0
	bcs .L_0812833e
	movs r1, #192
	adds r0, r3, #0
	lsls r1, r1, #3
	adds r0, r0, r1
	bl GameFlag_SetBit
.L_0812833e:
	ldrh r0, [r5]
	bl Func_080ad140
	mov r2, r11
	mov r8, r0
	cmp r2, #0
	beq .L_081283ee
	movs r3, #66
	add r3, r8
	mov r9, r3
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_081283a0
	movs r6, #0
	movs r5, #0
	b .L_08128370
.L_0812835e:
	bl Random16
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #1
	lsrs r3, r3, #16
	adds r3, r6, r3
	adds r6, r3, #1
	adds r5, #1
.L_08128370:
	mov r4, r10
	ldrb r0, [r4, #15]
	movs r1, #10
	bl Math_DivU
	lsls r0, r0, #24
	lsrs r0, r0, #24
	adds r0, #1
	cmp r5, r0
	blt .L_0812835e
	mov r0, r9
	ldrh r5, [r0]
	movs r1, #10
	lsls r0, r5, #1
	adds r0, r0, r5
	bl Math_Div
	cmp r6, r0
	bge .L_08128398
	adds r6, r0, #0
.L_08128398:
	ldr r3, [r7]
	adds r2, r6, r5
	adds r3, r3, r2
	str r3, [r7]
.L_081283a0:
	movs r1, #72
	add r1, r8
	ldrh r3, [r1]
	mov r9, r1
	cmp r3, #0
	beq .L_08128406
	movs r6, #0
	movs r5, #0
	b .L_081283c0
.L_081283b2:
	bl Random16
	lsls r0, r0, #2
	lsrs r0, r0, #16
	adds r0, r6, r0
	adds r6, r0, #1
	adds r5, #1
.L_081283c0:
	mov r2, r10
	ldrb r0, [r2, #15]
	movs r1, #10
	bl Math_DivU
	lsls r0, r0, #24
	lsrs r0, r0, #24
	adds r0, #1
	cmp r5, r0
	blt .L_081283b2
	mov r3, r9
	ldrh r5, [r3]
	movs r1, #10
	lsls r0, r5, #1
	adds r0, r0, r5
	bl Math_Div
	cmp r6, r0
	bge .L_081283e8
	adds r6, r0, #0
.L_081283e8:
	ldr r3, [r7, #4]
	adds r2, r6, r5
	b .L_08128402
.L_081283ee:
	mov r3, r8
	adds r3, #66
	ldrh r2, [r3]
	ldr r3, [r7]
	adds r3, r3, r2
	str r3, [r7]
	mov r3, r8
	adds r3, #72
	ldrh r2, [r3]
	ldr r3, [r7, #4]
.L_08128402:
	adds r3, r3, r2
	str r3, [r7, #4]
.L_08128406:
	movs r4, #68
	add r4, r8
	movs r5, #0
	ldrsh r2, [r4, r5]
	mov r9, r4
	cmp r2, #0
	beq .L_081284b2
	movs r0, #70
	add r0, r8
	movs r1, #0
	ldrsh r3, [r0, r1]
	mov r12, r0
	cmp r3, #0
	beq .L_081284b2
	ldrh r3, [r7, #12]
	movs r5, #0
	cmp r3, r2
	beq .L_08128442
	adds r1, r7, #0
	mov r0, r9
	adds r1, #12
.L_08128430:
	adds r5, #1
	cmp r5, #3
	bgt .L_08128442
	adds r1, #2
	ldrh r2, [r1]
	movs r4, #0
	ldrsh r3, [r0, r4]
	cmp r2, r3
	bne .L_08128430
.L_08128442:
	cmp r5, #4
	bne .L_081284b2
	mov r1, r12
	mov r2, r11
	movs r5, #0
	ldrsh r0, [r1, r5]
	cmp r2, #0
	beq .L_08128454
	subs r0, #2
.L_08128454:
	cmp r0, #0
	bge .L_0812845a
	movs r0, #0
.L_0812845a:
	movs r5, #128
	lsls r5, r5, #10
	asrs r5, r0
	bl BattleRandom16Far
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	ands r0, r3
	cmp r5, r0
	ble .L_081284b2
	movs r3, #1
	movs r4, #128
	negs r3, r3
	lsls r4, r4, #23
	adds r6, r7, #0
	mov r10, r3
	mov r8, r4
	movs r5, #0
	adds r6, #12
.L_08128482:
	ldrh r0, [r6]
	adds r6, #2
	bl Item_EncodeBankedId
	cmp r0, r8
	bge .L_08128492
	mov r8, r0
	mov r10, r5
.L_08128492:
	adds r5, #1
	cmp r5, #3
	ble .L_08128482
	mov r1, r9
	movs r5, #0
	ldrsh r0, [r1, r5]
	bl Item_EncodeBankedId
	cmp r0, r8
	ble .L_081284b2
	mov r2, r10
	mov r4, r9
	lsls r3, r2, #1
	ldrh r2, [r4]
	adds r3, #12
	strh r2, [r7, r3]
.L_081284b2:
	movs r0, #0
.L_081284b4:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
