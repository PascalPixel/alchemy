.syntax unified
	.thumb
	.global Func_080e7338
	.thumb_func
Func_080e7338:
	push {r5, r6, r7, lr}
	mov r7, r9
	push {r7}
	sub sp, #4
	mov r4, r9
	mov r3, sp
	str r4, [r3]
	adds r3, r4, #0
	adds r5, r1, #0
	adds r1, r3, #0
	subs r1, #136
	ldr r3, [r1]
	movs r7, #232
	lsls r7, r7, #7
	adds r6, r2, #0
	adds r2, r3, r7
	ldr r3, [r2, #24]
	movs r7, #1
	negs r7, r7
	movs r4, #0
	cmp r3, r7
	bne .L_080e7368
	str r4, [r2, #24]
	b .L_080e738c
.L_080e7368:
	adds r4, #1
	cmp r4, #16
	beq .L_080e7392
	lsls r3, r4, #3
	subs r3, r3, r4
	ldr r2, [r1]
	lsls r3, r3, #2
	adds r2, r2, r3
	movs r3, #232
	lsls r3, r3, #7
	adds r2, r2, r3
	movs r7, #1
	ldr r3, [r2, #24]
	negs r7, r7
	cmp r3, r7
	bne .L_080e7368
	movs r3, #0
	str r3, [r2, #24]
.L_080e738c:
	str r0, [r2]
	str r5, [r2, #4]
	str r6, [r2, #12]
.L_080e7392:
	add sp, #4
	pop {r3}
	mov r9, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
