.syntax unified
	.thumb
	.global Func_080adcec
	.thumb_func
Func_080adcec:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #1
	negs r2, r2
	movs r7, #0
	mov r8, r2
	bl Party_CountActiveOwners
	cmp r0, #0
	bne .L_080add06
	movs r0, #0
	b .L_080add52
.L_080add06:
	cmp r0, #0
	ble .L_080add42
	ldr r3, .L_080add58
	movs r2, #134
	lsls r2, r2, #2
	adds r6, r3, r2
	adds r5, r0, #0
.L_080add14:
	ldrb r0, [r6]
	bl Owner_GetState
	adds r3, r0, #0
	movs r2, #54
	ldrsh r1, [r3, r2]
	movs r0, #0
	cmp r1, #0
	ble .L_080add30
	movs r2, #58
	ldrsh r0, [r3, r2]
	lsls r0, r0, #16
	bl Math_Div
.L_080add30:
	cmp r0, r7
	ble .L_080add3a
	ldrb r3, [r6]
	adds r7, r0, #0
	mov r8, r3
.L_080add3a:
	subs r5, #1
	adds r6, #1
	cmp r5, #0
	bne .L_080add14
.L_080add42:
	movs r1, #1
	negs r1, r1
	movs r0, #0
	cmp r8, r1
	beq .L_080add52
	mov r0, r8
	bl Owner_AdjustSecondValue
.L_080add52:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080add58:
	.4byte gPartyState
