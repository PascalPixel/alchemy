.syntax unified
	.thumb
	.global sub_08124af8
	.thumb_func
sub_08124af8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r10, r0
	bl Owner_GetState
	movs r7, #159
	mov r8, r0
	lsls r7, r7, #1
	add r7, r8
	ldrb r3, [r7]
	cmp r3, #0
	beq .L_08124b92
	bl Func_081234a4
	mov r2, r8
	movs r3, #52
	ldrsh r6, [r2, r3]
	ldrb r3, [r7]
	movs r1, #56
	ldrsh r5, [r2, r1]
	adds r3, #1
	adds r0, r3, #0
	muls r0, r6
	movs r1, #10
	bl Math_Div
	mov r9, r5
	adds r5, r5, r0
	cmp r5, r6
	ble .L_08124b3c
	adds r5, r6, #0
.L_08124b3c:
	mov r1, r9
	subs r6, r5, r1
	cmp r6, #0
	beq .L_08124b7a
	mov r1, r10
	movs r0, #0
	bl BattleEv_Push
	mov r1, r8
	movs r2, #52
	ldrsh r3, [r1, r2]
	cmp r5, r3
	bne .L_08124b60
	ldr r1, .L_08124ba0
	movs r0, #4
	bl BattleEv_Push
	b .L_08124b70
.L_08124b60:
	movs r0, #1
	adds r1, r6, #0
	bl BattleEv_Push
	ldr r1, .L_08124ba4
	movs r0, #4
	bl BattleEv_Push
.L_08124b70:
	mov r2, r8
	strh r5, [r2, #56]
	mov r0, r10
	bl Owner_RecalculateRatiosFar
.L_08124b7a:
	bl BattleEv_DispatchQueued
	movs r2, #159
	lsls r2, r2, #1
	add r2, r8
	ldrb r3, [r2]
	movs r0, #1
	adds r3, #255
	strb r3, [r2]
	lsls r3, r3, #24
	cmp r3, #0
	beq .L_08124b94
.L_08124b92:
	movs r0, #0
.L_08124b94:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08124ba0:
	.4byte 0x00000c6c
.L_08124ba4:
	.4byte 0x00000c69
