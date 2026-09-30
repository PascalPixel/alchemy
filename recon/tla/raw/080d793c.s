.syntax unified
	.thumb
	.global Func_080d793c
	.thumb_func
Func_080d793c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r1, .L_080d7a50
	movs r2, #149
	lsls r2, r2, #2
	adds r7, r1, r2
	movs r3, #0
	ldrsh r5, [r7, r3]
	ldrh r2, [r7]
	movs r6, #240
	movs r3, #240
	lsls r6, r6, #4
	lsls r3, r3, #8
	adds r6, #255
	sub sp, #4
	ands r5, r3
	ands r6, r2
	cmp r0, #0
	bne .L_080d79cc
	cmp r5, #0
	bne .L_080d799e
	movs r3, #224
	lsls r3, r3, #3
	ldr r0, .L_080d7a54
	adds r3, #255
	ands r6, r3
	adds r3, r6, r0
	cmp r3, #80
	bhi .L_080d7a48
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #86
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	cmp r2, #0
	ble .L_080d7992
	movs r1, #186
	lsls r1, r1, #2
	adds r1, #255
	cmp r2, r1
	bne .L_080d7a48
.L_080d7992:
	adds r0, r6, #0
	subs r0, #172
	bl GameFlag_SetBit
	strh r5, [r7]
	b .L_080d7a48
.L_080d799e:
	movs r2, #128
	lsls r2, r2, #5
	cmp r5, r2
	bne .L_080d7a48
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #86
	adds r3, r1, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	ble .L_080d7a3e
	movs r0, #186
	lsls r0, r0, #2
	adds r0, #255
	cmp r3, r0
	beq .L_080d7a3e
	adds r0, r6, #0
	str r1, [sp, #0]
	bl GameFlag_SetBit
	ldr r1, [sp, #0]
	b .L_080d7a3e
.L_080d79cc:
	cmp r5, #0
	bne .L_080d7a3e
	movs r2, #224
	lsls r2, r2, #3
	ldr r0, .L_080d7a54
	adds r2, #255
	ands r6, r2
	adds r3, r6, r0
	cmp r3, #80
	bhi .L_080d7a3e
	ands r6, r2
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #86
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	ble .L_080d7a3e
	ldr r1, .L_080d7a54
	adds r5, r6, r1
	adds r0, r5, #0
	movs r1, #20
	bl Math_Div
	movs r1, #20
	mov r8, r0
	adds r0, r5, #0
	bl __modsi3
	movs r5, #8
	adds r7, r0, #0
	b .L_080d7a10
.L_080d7a0e:
	adds r5, #1
.L_080d7a10:
	cmp r5, #79
	bgt .L_080d7a3c
	adds r0, r5, #0
	bl BattleAction_FindDescriptor
	cmp r0, #0
	beq .L_080d7a0e
	movs r2, #2
	ldrsh r3, [r0, r2]
	ldr r0, .L_080d7a54
	subs r3, #48
	adds r2, r6, r0
	cmp r3, r2
	bne .L_080d7a0e
	movs r0, #40
	bl WaitFrames
	mov r1, r8
	adds r0, r5, #0
	adds r2, r7, #0
	bl Func_080d7788
.L_080d7a3c:
	ldr r1, .L_080d7a50
.L_080d7a3e:
	movs r3, #149
	lsls r3, r3, #2
	adds r2, r1, r3
	movs r3, #0
	strh r3, [r2]
.L_080d7a48:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080d7a50:
	.4byte gPartyState
.L_080d7a54:
	.4byte 0xfffffed4
