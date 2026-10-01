.syntax unified
	.thumb
	.global Func_08118eb0
	.thumb_func
Func_08118eb0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #20
	mov r5, sp
	adds r0, r5, #0
	bl BattleParty_PrepareActiveOwners
	movs r2, #0
	mov r10, r0
	mov r8, r2
	cmp r8, r10
	bge .L_08118f5e
	mov r9, r5
.L_08118ed0:
	mov r3, r9
	ldrh r7, [r3]
	movs r6, #0
.L_08118ed6:
	movs r5, #0
.L_08118ed8:
	adds r0, r7, #0
	adds r1, r6, #0
	adds r2, r5, #0
	bl Trade_CanOfferDjinnFar
	cmp r0, #0
	beq .L_08118f46
	movs r0, #0
	cmp r7, #7
	bls .L_08118eee
	movs r0, #1
.L_08118eee:
	bl Trade_GetOfferStateFar
	movs r2, #148
	adds r3, r0, #0
	lsls r2, r2, #1
	adds r1, r3, #0
	adds r3, r3, r2
	ldr r3, [r3]
	movs r0, #0
	adds r1, #8
	cmp r0, r3
	bge .L_08118f30
	ldrb r3, [r1]
	cmp r6, r3
	bne .L_08118f12
	ldrb r3, [r1, #1]
	cmp r5, r3
	beq .L_08118f30
.L_08118f12:
	movs r2, #144
	lsls r2, r2, #1
	adds r3, r1, r2
	ldr r3, [r3]
	adds r0, #1
	cmp r0, r3
	bge .L_08118f30
	lsls r2, r0, #2
	ldrb r3, [r1, r2]
	cmp r6, r3
	bne .L_08118f12
	adds r3, r1, r2
	ldrb r3, [r3, #1]
	cmp r5, r3
	bne .L_08118f12
.L_08118f30:
	movs r2, #144
	lsls r2, r2, #1
	adds r3, r1, r2
	ldr r3, [r3]
	cmp r0, r3
	bne .L_08118f46
	adds r0, r7, #0
	adds r1, r6, #0
	adds r2, r5, #0
	bl Trade_AddOfferFar
.L_08118f46:
	adds r5, #1
	cmp r5, #19
	ble .L_08118ed8
	adds r6, #1
	cmp r6, #3
	ble .L_08118ed6
	movs r2, #1
	movs r3, #2
	add r8, r2
	add r9, r3
	cmp r8, r10
	blt .L_08118ed0
.L_08118f5e:
	add sp, #20
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
