.syntax unified
	.thumb
	.global Func_0811c074
	.thumb_func
Func_0811c074:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	mov r8, r1
	bl GetBattleObjectSlot
	adds r7, r0, #0
	adds r0, r5, #0
	ldr r6, [r7]
	bl Owner_GetState
	movs r2, #165
	lsls r2, r2, #1
	adds r3, r0, r2
	ldrh r0, [r3]
	movs r3, #102
	adds r3, #255
	cmp r0, r3
	beq .L_0811c0f0
	adds r2, #39
	cmp r0, r2
	beq .L_0811c0f0
	adds r3, #16
	cmp r0, r3
	beq .L_0811c0f0
	adds r2, #7
	cmp r0, r2
	beq .L_0811c0f0
	ldr r3, .L_0811c110
	mov r2, r8
	lsls r5, r2, #2
	ldr r3, [r3, r5]
	str r3, [r6, #52]
	ldr r3, .L_0811c114
	ldr r3, [r3, r5]
	str r3, [r6, #48]
	ldr r3, [r6, #12]
	cmp r3, #0
	beq .L_0811c0c8
	cmp r2, #4
	ble .L_0811c0ce
.L_0811c0c8:
	ldr r3, .L_0811c118
	ldr r3, [r3, r5]
	str r3, [r6, #40]
.L_0811c0ce:
	adds r0, r6, #0
	bl Object_ResetMotion
	ldr r3, .L_0811c11c
	ldr r2, [r7, #12]
	ldr r3, [r3, r5]
	movs r1, #100
	adds r0, r3, #0
	muls r0, r2
	bl Math_Div
	ldr r3, [r7, #16]
	adds r1, r0, #0
	movs r2, #0
	adds r0, r6, #0
	bl Object_SetPosition
.L_0811c0f0:
	movs r3, #153
	lsls r3, r3, #8
	adds r3, #153
	adds r2, r6, #0
	str r3, [r6, #72]
	adds r2, #90
	movs r3, #0
	str r3, [r6, #68]
	adds r0, r6, #0
	strb r3, [r2]
	movs r1, #5
	bl Object_SetMode
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0811c110:
	.4byte Data_0812cbb0
.L_0811c114:
	.4byte Data_0812cbd8
.L_0811c118:
	.4byte Data_0812cc00
.L_0811c11c:
	.4byte Data_0812cc28
