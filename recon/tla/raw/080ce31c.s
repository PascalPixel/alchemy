.syntax unified
	.thumb
	.global Func_080ce31c
	.thumb_func
Func_080ce31c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	sub sp, #20
	ldr r5, [r3, #16]
	mov r9, r0
	bl EventRuntime_GetControlledOwner
	bl ObjectTable_Get
	ldrh r0, [r0, #6]
	str r0, [sp, #16]
	bl Func_080cb09c
	str r0, [sp, #12]
	bl Func_080cb144
	movs r2, #156
	str r0, [sp, #8]
	lsls r2, r2, #6
	movs r0, #1
	negs r0, r0
	adds r2, #15
	str r2, [sp, #4]
	str r0, [sp, #0]
	mov r11, r0
	ldr r1, [r5]
	mov r10, r0
	cmp r1, r0
	beq .L_080ce434
.L_080ce366:
	movs r3, #4
	ldrsh r6, [r5, r3]
	movs r3, #240
	lsls r3, r3, #8
	ldrh r2, [r5, #4]
	ands r6, r3
	ldr r3, .L_080ce39c
	lsls r0, r1, #16
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	movs r7, #255
	lsrs r0, r0, #24
	mov r8, r3
	ands r7, r2
	bl BattleAction_Get
	ldr r3, [r5]
	movs r2, #15
	ands r3, r2
	ldrb r1, [r0, #6]
	cmp r3, #5
	bne .L_080ce426
	cmp r1, r9
	bne .L_080ce426
	b .L_080ce3a0
	.2byte 0x0000
.L_080ce39c:
	.4byte 0x00000800
.L_080ce3a0:
	movs r2, #6
	ldrsh r0, [r5, r2]
	bl GameFlag_IsConditionActive
	cmp r0, #0
	beq .L_080ce426
	mov r3, r8
	cmp r3, #0
	beq .L_080ce3ca
	ldr r2, [sp, #16]
	subs r3, r6, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r3, #0
	bge .L_080ce3c0
	negs r3, r3
.L_080ce3c0:
	movs r2, #184
	lsls r2, r2, #5
	adds r2, #255
	cmp r3, r2
	bgt .L_080ce426
.L_080ce3ca:
	ldr r2, [r5]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	beq .L_080ce3dc
	movs r3, #128
	lsls r3, r3, #2
	str r3, [sp, #0]
	b .L_080ce426
.L_080ce3dc:
	movs r3, #16
	ands r3, r2
	cmp r3, #0
	beq .L_080ce410
	bl EventRuntime_GetControlledOwner
	adds r2, r7, #0
	adds r1, r0, #0
	mov r0, r9
	bl Func_080cdac0
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	beq .L_080ce426
	ldr r3, [sp, #4]
	cmp r3, r0
	ble .L_080ce426
	movs r2, #128
	lsls r2, r2, #1
	mov r10, r2
	mov r3, r10
	orrs r3, r7
	mov r10, r3
	str r0, [sp, #4]
	b .L_080ce426
.L_080ce410:
	mov r2, r9
	cmp r2, #30
	bne .L_080ce420
	ldr r3, [sp, #8]
	cmp r7, r3
	bne .L_080ce426
.L_080ce41c:
	mov r11, r7
	b .L_080ce434
.L_080ce420:
	ldr r2, [sp, #12]
	cmp r7, r2
	beq .L_080ce41c
.L_080ce426:
	adds r5, #12
	ldr r3, [r5]
	movs r2, #1
	negs r2, r2
	adds r1, r3, #0
	cmp r3, r2
	bne .L_080ce366
.L_080ce434:
	movs r3, #1
	negs r3, r3
	cmp r11, r3
	beq .L_080ce440
	mov r0, r11
	b .L_080ce448
.L_080ce440:
	mov r0, r10
	cmp r10, r11
	bne .L_080ce448
	ldr r0, [sp, #0]
.L_080ce448:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
