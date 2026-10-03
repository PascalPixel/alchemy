.syntax unified
	.thumb
	.global BattleQueue_SortByPriority
	.thumb_func
BattleQueue_SortByPriority:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r7, r1, #0
	sub sp, #16
	mov r11, r0
	cmp r7, #0
	ble .L_0811d1e0
	mov r5, r11
	adds r6, r7, #0
.L_0811d17c:
	movs r1, #6
	ldrsh r3, [r5, r1]
	cmp r3, #5
	bne .L_0811d1d8
	movs r2, #0
	ldrsh r0, [r5, r2]
	bl Owner_GetState
	ldrh r2, [r5, #8]
	ldr r3, .L_0811d1b8
	lsls r0, r2, #16
	asrs r0, r0, #24
	movs r1, #255
	ands r0, r3
	ands r1, r2
	bl Djinn_GetDefinitionHeaderFar
	bl BattleAction_Get
	ldrb r3, [r0, #3]
	cmp r3, #72
	beq .L_0811d1cc
	cmp r3, #76
	beq .L_0811d1cc
	cmp r3, #79
	beq .L_0811d1cc
	cmp r3, #74
	beq .L_0811d1cc
	b .L_0811d1bc
	.2byte 0x0000
.L_0811d1b8:
	.4byte 0x0000000f
.L_0811d1bc:
	cmp r3, #46
	beq .L_0811d1cc
	cmp r3, #47
	beq .L_0811d1cc
	cmp r3, #88
	beq .L_0811d1cc
	cmp r3, #53
	bne .L_0811d1d8
.L_0811d1cc:
	ldrh r3, [r5, #4]
	movs r1, #156
	lsls r1, r1, #6
	adds r1, #16
	adds r3, r3, r1
	strh r3, [r5, #4]
.L_0811d1d8:
	subs r6, #1
	adds r5, #16
	cmp r6, #0
	bne .L_0811d17c
.L_0811d1e0:
	subs r7, #1
	mov r9, r7
.L_0811d1e4:
	movs r2, #0
	mov r7, r9
	mov r10, r2
	cmp r7, #0
	ble .L_0811d234
	ldr r3, .L_0811d248
	mov r8, r3
	lsls r3, r7, #4
	add r3, r11
	adds r5, r3, #0
	subs r5, #16
	adds r6, r3, #0
.L_0811d1fc:
	movs r1, #20
	ldrsh r2, [r5, r1]
	movs r1, #4
	ldrsh r3, [r5, r1]
	cmp r2, r3
	ble .L_0811d22a
	mov r0, sp
	adds r1, r6, #0
	movs r2, #16
	mov lr, r8
	.2byte 0xf800
	adds r1, r5, #0
	movs r2, #16
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	movs r2, #16
	adds r0, r5, #0
	mov r1, sp
	mov lr, r8
	.2byte 0xf800
	movs r2, #1
	add r10, r2
.L_0811d22a:
	subs r7, #1
	subs r5, #16
	subs r6, #16
	cmp r7, #0
	bgt .L_0811d1fc
.L_0811d234:
	mov r3, r10
	cmp r3, #0
	bne .L_0811d1e4
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0811d248:
	.4byte IwramCopyWords
