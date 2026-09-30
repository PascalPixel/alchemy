.syntax unified
	.thumb
	.global Func_080ce574
	.thumb_func
Func_080ce574:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r0
	mov r8, r3
	lsrs r7, r0, #10
	movs r3, #15
	mov r0, r8
	ands r7, r3
	bl BattleAction_Get
	ldrb r5, [r0, #6]
	adds r0, r5, #0
	bl Func_080ce31c
	adds r6, r0, #0
	movs r0, #192
	lsls r0, r0, #22
	adds r1, r5, #0
	adds r2, r6, #0
	adds r0, #5
	bl Func_080ce458
	mov r10, r0
	movs r0, #128
	lsls r0, r0, #22
	adds r1, r5, #0
	adds r0, #5
	adds r2, r6, #0
	bl Func_080ce458
	movs r5, #1
	mov r3, r10
	mov r9, r0
	negs r5, r5
	cmp r3, #0
	bne .L_080ce5cc
	cmp r0, #0
	beq .L_080ce5de
.L_080ce5cc:
	cmp r6, r5
	beq .L_080ce5de
	movs r3, #128
	lsls r3, r3, #1
	ands r3, r6
	cmp r3, #0
	beq .L_080ce5de
	movs r5, #255
	ands r5, r6
.L_080ce5de:
	mov r0, r8
	movs r1, #0
	bl Func_080dc410
	bl Func_080cdf5c
	adds r1, r5, #0
	bl Func_080dc62c
	adds r1, r7, #0
	adds r2, r5, #0
	mov r0, r10
	bl Func_080ceafc
	bl FieldEvent_RunTypeHandler
	bl Func_080dc7cc
	adds r1, r7, #0
	adds r2, r5, #0
	mov r0, r9
	bl Func_080ceafc
	bl Func_080dc7e8
	movs r0, #0
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
