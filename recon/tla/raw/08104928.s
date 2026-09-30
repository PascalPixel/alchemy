.syntax unified
	.thumb
	.global Func_08104928
	.thumb_func
Func_08104928:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r2, #0
	mov r8, r3
	movs r3, #139
	lsls r3, r3, #1
	adds r3, #255
	add r3, r8
	ldrb r3, [r3]
	sub sp, #4
	adds r7, r1, #0
	movs r6, #0
	cmp r2, r3
	bge .L_08104980
	adds r5, r0, #0
.L_08104950:
	movs r3, #0
	strb r3, [r5]
	cmp r6, r7
	beq .L_0810496e
	adds r0, r7, #0
	adds r1, r6, #0
	str r2, [sp, #0]
	bl Func_0810498c
	ldr r2, [sp, #0]
	cmp r0, #0
	bne .L_0810496e
	movs r3, #1
	strb r3, [r5]
	adds r2, #1
.L_0810496e:
	movs r3, #139
	lsls r3, r3, #1
	adds r3, #255
	add r3, r8
	ldrb r3, [r3]
	adds r6, #1
	adds r5, #1
	cmp r6, r3
	blt .L_08104950
.L_08104980:
	adds r0, r2, #0
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
