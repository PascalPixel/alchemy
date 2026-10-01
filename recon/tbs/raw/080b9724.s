.syntax unified
	.thumb
	.global BattlePresentation_AppendLinkedActions
	.thumb_func
BattlePresentation_AppendLinkedActions:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_080b9880
	ldr r3, [r3]
	sub sp, #20
	mov r9, r3
	add r3, sp, #4
	add r2, sp, #8
	mov r8, r3
	mov r7, sp
	str r0, [r2]
	movs r3, #0
	mov r0, r8
	str r1, [r7]
	str r3, [r0]
	ldr r0, [r7]
	lsls r0, r0, #4
	movs r1, #20
	adds r0, #19
	mov r11, r2
	bl __udivsi3
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	movs r0, #40
	str r3, [sp, #12]
	bl Runtime_BumpAllocateAlternatePool
	ldr r3, [r7]
	add r5, sp, #16
	mov r10, r5
	str r0, [r5]
	cmp r3, #0
	ble .L_080b97b0
	mov r2, r11
	mov r6, r9
	ldr r1, [r2]
	movs r0, #1
	adds r6, #80
	adds r4, r3, #0
.L_080b9780:
	movs r2, #0
	ldrsh r3, [r1, r2]
	mov r2, r9
	adds r3, #72
	ldrb r3, [r2, r3]
	strh r3, [r1, #2]
	ldrb r3, [r6]
	cmp r3, #0
	bne .L_080b97a0
	ldrh r2, [r1, #4]
	adds r3, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_080b97a8
	adds r3, r2, #1
	b .L_080b97a6
.L_080b97a0:
	ldrh r2, [r1, #4]
	adds r3, r0, #0
	orrs r3, r2
.L_080b97a6:
	strh r3, [r1, #4]
.L_080b97a8:
	subs r4, #1
	adds r1, #16
	cmp r4, #0
	bne .L_080b9780
.L_080b97b0:
	mov r3, r9
	adds r3, #82
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080b9890
	mov r3, r9
	adds r3, #80
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080b980e
	mov r3, r10
	ldr r2, [r3]
	ldr r3, [r7]
	str r3, [r2]
	bl BattleRandom16Far
	mov r1, r10
	ldr r3, [r1]
	str r0, [r3, #4]
	ldr r2, .L_080b9884
	ldrh r1, [r2]
	strh r2, [r2]
	ldr r4, .L_080b9888
	ldr r3, .L_080b988c
	add r5, sp, #16
	ldr r3, [r3]
	ldr r0, [r5]
	str r3, [r0, #8]
	str r3, [r4]
	strh r1, [r2]
	add r2, sp, #20
	mov r9, r2
	bl Func_080b9554
	cmp r0, #0
	blt .L_080b9890
	add r3, sp, #20
	mov r9, r3
	bl Func_080b9604
	cmp r0, #0
	blt .L_080b9890
	ldr r3, [r5]
	ldr r3, [r3]
	mov r0, r8
	str r3, [r0]
	b .L_080b9848
.L_080b980e:
	add r1, sp, #20
	mov r9, r1
	bl Func_080b9604
	cmp r0, #0
	blt .L_080b9890
	mov r3, r10
	ldr r2, [r3]
	ldr r3, [r2]
	mov r0, r8
	str r3, [r0]
	ldr r3, [r7]
	add r1, sp, #20
	str r3, [r2]
	mov r9, r1
	bl Func_080b9554
	cmp r0, #0
	blt .L_080b9890
	bl BattleRandom16Far
	mov r2, r10
	ldr r1, [r2]
	ldr r3, [r1, #4]
	cmp r0, r3
	bne .L_080b9890
	ldr r2, .L_080b9888
	ldr r3, [r1, #8]
	str r3, [r2]
.L_080b9848:
	mov r3, r8
	ldr r1, [r3]
	cmp r1, #0
	ble .L_080b9870
	mov r0, r11
	ldr r3, [r7]
	ldr r2, [r0]
	lsls r3, r3, #4
	ldr r6, .L_080b987c
	adds r0, r3, r2
	adds r4, r1, #0
.L_080b985e:
	ldrh r3, [r0, #2]
	strh r3, [r0]
	ldrh r3, [r0, #10]
	subs r4, #1
	eors r3, r6
	strh r3, [r0, #10]
	adds r0, #16
	cmp r4, #0
	bne .L_080b985e
.L_080b9870:
	ldr r0, [r5]
	bl Runtime_BumpFree
	mov r1, r8
	ldr r0, [r1]
	b .L_080b98a2
.L_080b987c:
	.4byte 0x00000080
.L_080b9880:
	.4byte gBattleWork
.L_080b9884:
	.4byte 0x04000208
.L_080b9888:
	.4byte gBattleRandomSeed
.L_080b988c:
	.4byte Data_03001cb4
.L_080b9890:
	bl BattleLink_ResetTransferState
	bl SerialRuntime_RemoveIrqHandlers
	ldr r0, [r5]
	bl Runtime_BumpFree
	movs r0, #1
	negs r0, r0
.L_080b98a2:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
