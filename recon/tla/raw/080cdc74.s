.syntax unified
	.thumb
	.global Func_080cdc74
	.thumb_func
Func_080cdc74:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r6, .L_080cdd78
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r0, [r3]
	sub sp, #24
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #12
	lsrs r5, r3, #12
	adds r3, r5, #2
	ands r3, r2
	lsls r5, r3, #12
	add r3, sp, #12
	mov r8, r3
	ldr r3, [r0, #8]
	mov r2, r8
	str r3, [r2]
	movs r2, #128
	ldr r3, [r0, #12]
	lsls r2, r2, #11
	adds r3, r3, r2
	ldr r2, .L_080cdd7c
	adds r1, r5, #0
	ands r3, r2
	mov r2, r8
	str r3, [r2, #4]
	ldr r3, [r0, #16]
	movs r0, #128
	str r3, [r2, #8]
	lsls r0, r0, #13
	bl Vector_AddPolarOffset
	mov r0, r8
	movs r1, #1
	bl Func_080eaf28
	adds r7, r0, #0
	movs r0, #0
	cmp r7, #0
	beq .L_080cdd70
	adds r3, r7, #0
	adds r3, #98
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080cdd70
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #106
	adds r3, r6, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	cmp r0, #1
	beq .L_080cdcf0
	bl Object_GetById
	cmp r7, r0
	beq .L_080cdd6a
.L_080cdcf0:
	ldr r3, [r7, #8]
	mov r2, r8
	str r3, [r2]
	ldr r3, [r7, #12]
	movs r0, #128
	str r3, [r2, #4]
	ldr r3, [r7, #16]
	lsls r0, r0, #13
	adds r1, r5, #0
	str r3, [r2, #8]
	bl Vector_AddPolarOffset
	mov r0, r8
	movs r1, #1
	bl Func_080eaf28
	cmp r0, #0
	beq .L_080cdd24
	adds r3, r0, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	movs r0, #0
	cmp r3, #0
	bne .L_080cdd70
.L_080cdd24:
	ldr r3, [r7, #8]
	mov r0, sp
	str r3, [r0]
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	movs r1, #1
	str r3, [r0, #8]
	bl Func_080eaf28
	cmp r0, #0
	beq .L_080cdd52
	adds r3, r0, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	movs r0, #0
	cmp r3, #0
	bne .L_080cdd70
.L_080cdd52:
	adds r6, r7, #0
	adds r6, #34
	movs r3, #2
	ldrb r5, [r6]
	adds r0, r7, #0
	strb r3, [r6]
	mov r1, r8
	bl Func_08020210
	strb r5, [r6]
	cmp r0, #0
	ble .L_080cdd6e
.L_080cdd6a:
	movs r0, #0
	b .L_080cdd70
.L_080cdd6e:
	movs r0, #1
.L_080cdd70:
	add sp, #24
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080cdd78:
	.4byte gPartyState
.L_080cdd7c:
	.4byte 0xfff80000
