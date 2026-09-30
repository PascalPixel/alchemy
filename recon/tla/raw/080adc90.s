.syntax unified
	.thumb
	.global Func_080adc90
	.thumb_func
Func_080adc90:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #0
	mov r8, r2
	movs r6, #0
	bl Party_CountActiveOwners
	cmp r0, #0
	bne .L_080adca8
	movs r0, #0
	b .L_080adce0
.L_080adca8:
	cmp r6, r0
	bge .L_080adcd0
	ldr r3, .L_080adce8
	movs r2, #134
	lsls r2, r2, #2
	adds r7, r3, r2
	adds r5, r0, #0
.L_080adcb6:
	ldrb r0, [r7]
	bl Owner_GetState
	movs r2, #58
	ldrsh r3, [r0, r2]
	subs r5, #1
	add r8, r3
	movs r2, #54
	ldrsh r3, [r0, r2]
	adds r7, #1
	adds r6, r6, r3
	cmp r5, #0
	bne .L_080adcb6
.L_080adcd0:
	movs r0, #0
	cmp r6, #0
	beq .L_080adce0
	mov r3, r8
	lsls r0, r3, #16
	adds r1, r6, #0
	bl Math_Div
.L_080adce0:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080adce8:
	.4byte gPartyState
