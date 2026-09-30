.syntax unified
	.thumb
	.global Func_08023d80
	.thumb_func
Func_08023d80:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	movs r3, #24
	movs r0, #108
	adds r3, r3, r5
	adds r0, #255
	sub sp, #4
	mov r10, r1
	mov r8, r3
	bl GameFlag_Test
	cmp r0, #0
	beq .L_08023da4
	ldr r3, .L_08023e14
	mov r8, r3
.L_08023da4:
	adds r3, r5, #0
	adds r3, #84
	ldrb r3, [r3]
	movs r2, #15
	ands r2, r3
	cmp r2, #1
	beq .L_08023dbc
	cmp r2, #1
	ble .L_08023e0a
	cmp r2, #2
	beq .L_08023de4
	b .L_08023e0a
.L_08023dbc:
	adds r3, r5, #0
	adds r3, #35
	ldrb r2, [r3]
	movs r3, #16
	adds r1, r5, #0
	ands r3, r2
	adds r1, #8
	ldr r0, [r5, #80]
	cmp r3, #0
	bne .L_08023e0a
	adds r2, r5, #0
	adds r2, #34
	ldrb r2, [r2]
	ldrh r3, [r5, #6]
	str r2, [sp, #0]
	add r3, r10
	mov r2, r8
	bl Func_0802254c
	b .L_08023e0a
.L_08023de4:
	ldr r6, [r5, #80]
	movs r7, #3
.L_08023de8:
	ldmia r6!, {r0}
	cmp r0, #0
	beq .L_08023e04
	adds r2, r5, #0
	adds r2, #34
	ldrb r2, [r2]
	ldrh r3, [r5, #6]
	adds r1, r5, #0
	str r2, [sp, #0]
	add r3, r10
	adds r1, #8
	mov r2, r8
	bl Func_0802254c
.L_08023e04:
	subs r7, #1
	cmp r7, #0
	bge .L_08023de8
.L_08023e0a:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_08023e14:
	.4byte Data_0802eb90
