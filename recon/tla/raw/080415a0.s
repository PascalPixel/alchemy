.syntax unified
	.thumb
	.global Func_080415a0
	.thumb_func
Func_080415a0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #132
	str r1, [sp, #0]
	adds r7, r3, #0
	movs r3, #192
	mov r11, r0
	lsls r3, r3, #18
	ldr r0, .L_080416a0
	mov r9, r2
	ldr r6, [r3, #60]
	bl Resource_GetTableEntry
	mov r10, r0
	mov r0, r11
	ldrb r5, [r0]
	movs r1, #4
	lsls r2, r5, #5
	add r1, sp
	mov r8, r2
	movs r2, #192
	mov r12, r1
	lsls r2, r2, #19
	mov lr, r12
	add r2, r8
	movs r1, #0
.L_080415de:
	ldrb r4, [r2]
	movs r0, #15
	adds r3, r4, #0
	ands r3, r0
	mov r0, r12
	strb r3, [r0]
	lsrs r3, r4, #4
	strb r3, [r0, #1]
	adds r1, #1
	movs r3, #2
	adds r2, #1
	add r12, r3
	cmp r1, #31
	bls .L_080415de
	mov r4, r9
	lsls r3, r4, #5
	mov r0, r10
	adds r2, r0, r3
	movs r3, #15
	mov r12, lr
	movs r1, #0
	mov r10, r3
.L_0804160a:
	ldrb r0, [r2]
	mov r4, r10
	adds r3, r0, #0
	ands r3, r4
	ldrb r3, [r7, r3]
	adds r2, #1
	cmp r3, #0
	beq .L_0804161e
	mov r4, r12
	strb r3, [r4]
.L_0804161e:
	movs r3, #1
	add r12, r3
	lsrs r3, r0, #4
	ldrb r3, [r7, r3]
	cmp r3, #0
	beq .L_0804162e
	mov r4, r12
	strb r3, [r4]
.L_0804162e:
	movs r0, #1
	adds r1, #1
	add r12, r0
	cmp r1, #31
	bls .L_0804160a
	mov r12, lr
	movs r1, #0
	mov r0, r12
.L_0804163e:
	ldrb r3, [r0, #1]
	ldrb r2, [r0]
	lsls r3, r3, #4
	orrs r2, r3
	movs r4, #1
	mov r3, r12
	adds r1, #1
	adds r0, #2
	strb r2, [r3]
	add r12, r4
	cmp r1, #31
	bls .L_0804163e
	lsls r3, r5, #24
	cmp r3, #0
	blt .L_080416a4
	movs r1, #224
	lsls r1, r1, #4
	movs r4, #0
	movs r0, #127
	adds r1, #56
.L_08041666:
	ldrh r3, [r6]
	adds r2, r3, #1
	ands r2, r0
	lsls r3, r3, #24
	strh r2, [r6]
	lsrs r5, r3, #24
	adds r2, r5, r1
	ldrb r3, [r6, r2]
	cmp r3, #0
	beq .L_08041680
	adds r4, #1
	cmp r4, #127
	bls .L_08041666
.L_08041680:
	movs r3, #1
	strb r3, [r6, r2]
	movs r3, #128
	ldr r2, .L_0804169c
	orrs r5, r3
	adds r3, r5, #0
	orrs r3, r2
	mov r0, r11
	strh r3, [r0]
	ldr r1, [sp, #0]
	lsls r5, r5, #5
	strh r3, [r1]
	mov r8, r5
	b .L_080416a4
.L_0804169c:
	.4byte 0x0000f000
.L_080416a0:
	.4byte 0x00000013
.L_080416a4:
	movs r3, #128
	movs r1, #192
	movs r2, #132
	lsls r3, r3, #19
	lsls r1, r1, #19
	lsls r2, r2, #24
	adds r3, #212
	add r0, sp, #4
	add r1, r8
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	add sp, #132
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
