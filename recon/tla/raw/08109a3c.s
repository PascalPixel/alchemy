.syntax unified
	.thumb
	.global Func_08109a3c
	.thumb_func
Func_08109a3c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	adds r6, r1, #0
	bl Owner_GetState
	movs r3, #1
	negs r3, r3
	mov r8, r3
	cmp r6, r8
	bne .L_08109a58
	movs r0, #0
	b .L_08109a92
.L_08109a58:
	lsls r3, r6, #1
	adds r3, #216
	ldrh r3, [r0, r3]
	movs r5, #128
	lsls r5, r5, #1
	adds r5, #255
	ands r5, r3
	adds r0, r5, #0
	bl Item_Get
	ldrb r3, [r0, #2]
	movs r0, #0
	cmp r3, #6
	beq .L_08109a92
	adds r0, r5, #0
	bl Item_Get
	ldrb r2, [r0, #3]
	movs r3, #8
	ands r3, r2
	movs r0, #0
	cmp r3, #0
	bne .L_08109a92
	adds r0, r7, #0
	adds r1, r6, #0
	mov r2, r8
	bl Func_0810a108
	movs r0, #1
.L_08109a92:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
