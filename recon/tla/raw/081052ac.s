.syntax unified
	.thumb
	.global Func_081052ac
	.thumb_func
Func_081052ac:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	movs r3, #136
	lsls r0, r0, #2
	lsls r3, r3, #2
	adds r6, r0, r3
	ldr r0, [r7, r6]
	adds r5, r1, #0
	mov r8, r2
	cmp r0, #0
	beq .L_081052d4
	bl Func_08020040 + 0x8
	movs r3, #0
	str r3, [r7, r6]
.L_081052d4:
	ldr r3, .L_081052fc
	lsls r2, r5, #2
	ldr r0, [r3, r2]
	bl Func_08020040
	adds r5, r0, #0
	cmp r5, #0
	beq .L_081052f4
	mov r1, r8
	bl Animation_ApplyChildArgumentFar
	ldrb r2, [r5, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	strb r3, [r5, #9]
.L_081052f4:
	str r5, [r7, r6]
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_081052fc:
	.4byte Data_08105a40
