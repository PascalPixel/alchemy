.syntax unified
	.thumb
	.global Func_080ae410
	.thumb_func
Func_080ae410:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #164
	lsls r3, r3, #3
	sub sp, #24
	movs r2, #144
	adds r3, r0, r3
	str r1, [sp, #4]
	lsls r2, r2, #2
	movs r1, #64
	str r3, [sp, #0]
	adds r1, r1, r0
	adds r2, r2, r0
	movs r4, #0
	mov r9, r1
	mov r11, r2
	movs r7, #0
	mov r8, r4
.L_080ae43e:
	lsls r3, r7, #2
	movs r5, #0
	adds r3, r3, r7
	mov r10, r5
	lsls r3, r3, #2
	movs r5, #128
	adds r6, r3, #0
	lsls r5, r5, #4
	adds r6, #48
	add r5, r8
.L_080ae452:
	mov r0, r9
	adds r1, r6, #0
	bl Func_080ae3fc
	cmp r0, #0
	beq .L_080ae466
	adds r0, r5, #0
	bl GameFlag_SetBit
	b .L_080ae46c
.L_080ae466:
	adds r0, r5, #0
	bl GameFlag_ClearBit
.L_080ae46c:
	movs r0, #1
	add r10, r0
	mov r1, r10
	adds r6, #1
	adds r5, #1
	cmp r1, #6
	ble .L_080ae452
	movs r2, #7
	adds r7, #1
	add r8, r2
	cmp r7, #3
	ble .L_080ae43e
	ldr r5, .L_080ae5ec
	movs r7, #0
.L_080ae488:
	ldrh r1, [r5]
	mov r0, r9
	adds r5, #2
	bl Func_080ae3fc
	cmp r0, #0
	beq .L_080ae4a2
	movs r3, #130
	lsls r3, r3, #4
	adds r0, r7, r3
	bl GameFlag_SetBit
	b .L_080ae4ac
.L_080ae4a2:
	movs r4, #130
	lsls r4, r4, #4
	adds r0, r7, r4
	bl GameFlag_ClearBit
.L_080ae4ac:
	adds r7, #1
	cmp r7, #5
	ble .L_080ae488
	movs r5, #128
	lsls r5, r5, #4
	adds r5, #34
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ae4ca
	adds r0, r5, #0
	bl GameFlag_ClearBit
	b .L_080ae4d0
.L_080ae4ca:
	adds r0, r5, #0
	bl GameFlag_SetBit
.L_080ae4d0:
	ldr r4, .L_080ae5f0
	mov r0, r11
	movs r5, #163
	ldr r3, [r0, #16]
	lsls r5, r5, #2
	adds r2, r4, r5
	str r3, [r2]
	ldr r1, [sp, #4]
	cmp r1, #0
	bne .L_080ae51c
	movs r0, #147
	lsls r0, r0, #1
	mov r2, r11
	adds r0, #255
	ldrb r3, [r2, r0]
	adds r0, r4, r0
	strb r3, [r0]
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #38
	ldrb r3, [r2, r1]
	adds r1, r4, r1
	strb r3, [r1]
	movs r2, #139
	lsls r2, r2, #2
	mov r5, r11
	ldrb r3, [r5, r2]
	strb r3, [r4, r2]
	subs r2, #2
	ldrb r3, [r5, r2]
	strb r3, [r4, r2]
	adds r2, #32
	ldrb r3, [r5, r2]
	strb r3, [r4, r2]
	ldrb r0, [r0]
	ldrb r1, [r1]
	bl Func_08038348
.L_080ae51c:
	ldr r2, [sp, #0]
	ldr r3, .L_080ae5f4
	movs r1, #8
	movs r0, #4
	add r1, sp
	mov r10, r0
	movs r7, #0
	mov r9, r1
	mov r8, r2
	mov r11, r3
.L_080ae530:
	adds r0, r7, #0
	bl Owner_GetState
	mov r2, r9
	mov r3, r8
	adds r5, r0, #0
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	ldrh r1, [r3]
	adds r0, r5, #0
	strh r1, [r2]
	mov r1, r8
	ldrb r3, [r3, #2]
	strb r3, [r2, #2]
	movs r2, #166
	lsls r2, r2, #1
	mov lr, r11
	.2byte 0xf800
	adds r2, r5, #0
	mov r3, r9
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	ldrh r1, [r3]
	strh r1, [r2]
	ldrb r3, [r3, #2]
	strb r3, [r2, #2]
	ldr r2, [sp, #4]
	cmp r2, #0
	bne .L_080ae57a
	movs r4, #148
	lsls r4, r4, #1
	adds r3, r5, r4
	ldrb r3, [r3]
	movs r0, #165
	lsls r0, r0, #1
	adds r2, r5, r0
	strh r3, [r2]
.L_080ae57a:
	movs r6, #14
	adds r5, #244
.L_080ae57e:
	ldrh r0, [r5]
	subs r5, #2
	cmp r0, #0
	beq .L_080ae5a0
	bl Item_GetDirect
	ldrb r2, [r0, #3]
	movs r3, #32
	ands r3, r2
	cmp r3, #0
	beq .L_080ae5a0
.L_080ae594:
	adds r0, r7, #0
	adds r1, r6, #0
	bl Inventory_Remove
	cmp r0, #1
	beq .L_080ae594
.L_080ae5a0:
	subs r6, #1
	cmp r6, #0
	bge .L_080ae57e
	adds r0, r7, #0
	bl Owner_RecalculateStats
	movs r1, #166
	lsls r1, r1, #1
	adds r7, #1
	add r8, r1
	cmp r7, r10
	blt .L_080ae530
	bl Func_080b1004
	movs r0, #1
	bl Func_080ae16c
	ldr r2, [sp, #4]
	movs r0, #1
	ands r0, r2
	adds r0, #44
	bl GameFlag_SetBit
	movs r0, #47
	bl GameFlag_SetBit
	ldr r3, .L_080ae5f0
	ldr r2, .L_080ae5f8
	ldr r3, [r3, #4]
	add sp, #24
	str r3, [r2]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ae5ec:
	.4byte Data_080b1284
.L_080ae5f0:
	.4byte gPartyState
.L_080ae5f4:
	.4byte IwramCopyWords
.L_080ae5f8:
	.4byte Data_0300117c
