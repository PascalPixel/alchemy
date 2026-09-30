.syntax unified
	.thumb
	.global Func_08041abc
	.thumb_func
Func_08041abc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #60]
	lsls r1, r1, #6
	lsls r0, r0, #1
	adds r1, r5, r1
	ldrb r3, [r5, #2]
	adds r1, r1, r0
	adds r0, r1, #0
	movs r6, #0
	sub sp, #4
	adds r7, r2, #0
	adds r0, #8
	mov r8, r3
	cmp r6, r11
	bcs .L_08041b58
	movs r3, #32
	subs r3, r3, r7
	lsls r3, r3, #1
	str r3, [sp, #0]
.L_08041af4:
	movs r4, #0
	cmp r4, r7
	bcs .L_08041b4e
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	mov r9, r3
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	mov r10, r3
	adds r3, #128
	mov lr, r3
	movs r3, #255
	mov r12, r3
.L_08041b12:
	ldrh r3, [r0]
	mov r2, r9
	ands r2, r3
	adds r3, r2, #0
	subs r3, #128
	adds r0, #2
	cmp r3, #127
	bls .L_08041b30
	mov r3, r8
	cmp r3, #0
	beq .L_08041b48
	cmp r2, r10
	bls .L_08041b48
	cmp r2, lr
	bhi .L_08041b48
.L_08041b30:
	mov r3, r12
	ands r2, r3
	movs r3, #128
	eors r2, r3
	movs r3, #224
	lsls r3, r3, #4
	adds r3, #56
	adds r2, r2, r3
	ldrb r1, [r5, r2]
	movs r3, #252
	ands r3, r1
	strb r3, [r5, r2]
.L_08041b48:
	adds r4, #1
	cmp r4, r7
	bcc .L_08041b12
.L_08041b4e:
	ldr r3, [sp, #0]
	adds r6, #1
	adds r0, r0, r3
	cmp r6, r11
	bcc .L_08041af4
.L_08041b58:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
