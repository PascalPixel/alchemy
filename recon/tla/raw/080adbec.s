.syntax unified
	.thumb
	.global Func_080adbec
	.thumb_func
Func_080adbec:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #104
	adds r0, #255
	sub sp, #8
	bl GameFlag_ClearBit
	bl Party_CountActiveOwners
	mov r10, r0
	movs r0, #0
	mov r8, r0
	cmp r8, r10
	bge .L_080adc7e
	ldr r3, .L_080adc8c
	movs r2, #134
	lsls r2, r2, #2
	adds r2, r2, r3
	movs r3, #128
	lsls r3, r3, #2
	mov r9, r2
	mov r11, r3
.L_080adc22:
	mov r2, r9
	ldrb r0, [r2]
	bl Owner_GetState
	movs r2, #216
	adds r7, r0, #0
	movs r1, #14
.L_080adc30:
	ldrh r3, [r2, r7]
	mov r0, r11
	ands r3, r0
	cmp r3, #0
	beq .L_080adc6c
	ldrh r0, [r2, r7]
	str r1, [sp, #4]
	str r2, [sp, #0]
	bl Item_GetDirect
	ldr r2, [sp, #0]
	ldr r1, [sp, #4]
	adds r5, r0, #0
	adds r5, #24
	movs r6, #3
.L_080adc4e:
	ldrb r3, [r5]
	adds r5, #4
	cmp r3, #27
	bne .L_080adc66
	movs r0, #104
	adds r0, #255
	str r1, [sp, #4]
	str r2, [sp, #0]
	bl GameFlag_SetBit
	ldr r2, [sp, #0]
	ldr r1, [sp, #4]
.L_080adc66:
	subs r6, #1
	cmp r6, #0
	bge .L_080adc4e
.L_080adc6c:
	subs r1, #1
	adds r2, #2
	cmp r1, #0
	bge .L_080adc30
	movs r2, #1
	add r8, r2
	add r9, r2
	cmp r8, r10
	blt .L_080adc22
.L_080adc7e:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080adc8c:
	.4byte gPartyState
