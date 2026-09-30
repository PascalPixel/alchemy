.syntax unified
	.thumb
	.global Func_080d99b8
	.thumb_func
Func_080d99b8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_080d9a70
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	mov r8, r1
	bl Object_GetById
	adds r5, r0, #0
	adds r0, r6, #0
	bl Object_GetById
	adds r6, r0, #0
	mov r0, r8
	bl Object_GetById
	ldr r3, [r5, #8]
	ldr r2, [r6, #12]
	asrs r4, r3, #20
	ldr r3, [r5, #12]
	asrs r2, r2, #20
	asrs r3, r3, #20
	mov lr, r3
	ldr r3, [r5, #16]
	asrs r1, r3, #20
	ldr r3, [r6, #8]
	asrs r7, r3, #20
	ldr r3, [r6, #16]
	asrs r5, r3, #20
	ldr r3, [r0, #8]
	asrs r6, r3, #20
	ldr r3, [r0, #12]
	asrs r3, r3, #20
	mov r12, r3
	ldr r3, [r0, #16]
	asrs r0, r3, #20
	cmp lr, r2
	bne .L_080d9a36
	cmp r4, r7
	bne .L_080d9a22
	subs r3, r5, #1
	cmp r1, r3
	beq .L_080d9a64
	adds r3, r5, #1
	cmp r1, r3
	beq .L_080d9a64
	cmp r1, r5
	bne .L_080d9a36
	b .L_080d9a64
.L_080d9a22:
	cmp r1, r5
	bne .L_080d9a36
	subs r3, r7, #1
	cmp r4, r3
	beq .L_080d9a64
	adds r3, r7, #1
	cmp r4, r3
	beq .L_080d9a64
	cmp r4, r7
	beq .L_080d9a64
.L_080d9a36:
	cmp lr, r12
	bne .L_080d9a68
	cmp r4, r6
	bne .L_080d9a50
	subs r3, r0, #1
	cmp r1, r3
	beq .L_080d9a64
	adds r3, r0, #1
	cmp r1, r3
	beq .L_080d9a64
	cmp r1, r0
	bne .L_080d9a68
	b .L_080d9a64
.L_080d9a50:
	cmp r1, r0
	bne .L_080d9a68
	subs r3, r6, #1
	cmp r4, r3
	beq .L_080d9a64
	adds r3, r6, #1
	cmp r4, r3
	beq .L_080d9a64
	cmp r4, r6
	bne .L_080d9a68
.L_080d9a64:
	movs r0, #1
	b .L_080d9a6a
.L_080d9a68:
	movs r0, #0
.L_080d9a6a:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080d9a70:
	.4byte gPartyState
