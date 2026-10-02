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
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
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
	movs r1, #20
	lsls r0, r0, #4
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
	ble .L_0811d4a2
	mov r2, r11
	ldr r1, [r2]
	mov r6, r9
	movs r0, #1
	adds r6, #80
	adds r4, r3, #0
.L_0811d472:
	movs r2, #0
	ldrsh r3, [r1, r2]
	mov r2, r9
	adds r3, #72
	ldrb r3, [r2, r3]
	strh r3, [r1, #2]
	ldrb r3, [r6]
	cmp r3, #0
	bne .L_0811d492
	ldrh r2, [r1, #4]
	adds r3, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_0811d49a
	adds r3, r2, #1
	b .L_0811d498
.L_0811d492:
	ldrh r2, [r1, #4]
	adds r3, r0, #0
	orrs r3, r2
.L_0811d498:
	strh r3, [r1, #4]
.L_0811d49a:
	subs r4, #1
	adds r1, #16
	cmp r4, #0
	bne .L_0811d472
.L_0811d4a2:
	mov r3, r9
	adds r3, #82
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_0811d580
	mov r3, r9
	adds r3, #80
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_0811d500
	mov r3, r10
	ldr r2, [r3]
	ldr r3, [r7]
	str r3, [r2]
	bl BattleRandom16Far
	mov r1, r10
	ldr r3, [r1]
	str r0, [r3, #4]
	ldr r2, .L_0811d574
	ldrh r1, [r2]
	strh r2, [r2]
	ldr r4, .L_0811d578
	ldr r3, .L_0811d57c
	add r5, sp, #16
	ldr r3, [r3]
	ldr r0, [r5]
	str r3, [r0, #8]
	str r3, [r4]
	strh r1, [r2]
	add r2, sp, #20
	mov r9, r2
	bl Func_0811d24c
	cmp r0, #0
	blt .L_0811d580
	add r3, sp, #20
	mov r9, r3
	bl Func_0811d2f8
	cmp r0, #0
	blt .L_0811d580
	ldr r3, [r5]
	mov r0, r8
	ldr r3, [r3]
	str r3, [r0]
	b .L_0811d53a
.L_0811d500:
	add r1, sp, #20
	mov r9, r1
	bl Func_0811d2f8
	cmp r0, #0
	blt .L_0811d580
	mov r3, r10
	ldr r2, [r3]
	mov r0, r8
	ldr r3, [r2]
	add r1, sp, #20
	str r3, [r0]
	ldr r3, [r7]
	mov r9, r1
	str r3, [r2]
	bl Func_0811d24c
	cmp r0, #0
	blt .L_0811d580
	bl BattleRandom16Far
	mov r2, r10
	ldr r1, [r2]
	ldr r3, [r1, #4]
	cmp r0, r3
	bne .L_0811d580
	ldr r2, .L_0811d578
	ldr r3, [r1, #8]
	str r3, [r2]
.L_0811d53a:
	mov r3, r8
	ldr r1, [r3]
	cmp r1, #0
	ble .L_0811d562
	mov r0, r11
	ldr r3, [r7]
	ldr r2, [r0]
	ldr r6, .L_0811d570
	lsls r3, r3, #4
	adds r0, r3, r2
	adds r4, r1, #0
.L_0811d550:
	ldrh r3, [r0, #2]
	subs r4, #1
	strh r3, [r0]
	ldrh r3, [r0, #10]
	eors r3, r6
	strh r3, [r0, #10]
	adds r0, #16
	cmp r4, #0
	bne .L_0811d550
.L_0811d562:
	ldr r0, [r5]
	bl Sys_Free
	mov r1, r8
	ldr r0, [r1]
	b .L_0811d592
	.2byte 0x0000
.L_0811d570:
	.4byte 0x00000080
.L_0811d574:
	.4byte 0x04000208
.L_0811d578:
	.4byte Data_020054c8
.L_0811d57c:
	.4byte gRandomState
.L_0811d580:
	bl Func_08016950
	bl SerialRuntime_RemoveIrqHandlers
	ldr r0, [r5]
	bl Sys_Free
	movs r0, #1
	negs r0, r0
.L_0811d592:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
