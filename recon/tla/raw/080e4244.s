.syntax unified
	.thumb
	.global Func_080e4244
	.thumb_func
Func_080e4244:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	movs r2, #0
	ldr r3, [r3, #108]
	mov r8, r2
	movs r2, #197
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	sub sp, #4
	adds r6, r0, #0
	cmp r3, #3
	beq .L_080e427c
	ldr r0, [r6, #8]
	ldr r1, [r6, #16]
	ldr r3, [r6, #20]
	movs r2, #0
	bl Func_08020308
	cmp r0, #0
	beq .L_080e427c
	movs r3, #1
	mov r8, r3
.L_080e427c:
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r4, #63
	adds r7, r5, #0
	adds r7, #89
.L_080e4288:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_080e42c0
	ldrb r2, [r7]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080e42c0
	cmp r5, r6
	beq .L_080e42c0
	ldrh r1, [r6, #32]
	ldrh r3, [r5, #32]
	adds r2, r5, #0
	adds r0, r6, #0
	subs r1, #2
	adds r2, #8
	subs r3, #2
	adds r0, #8
	str r4, [sp, #0]
	bl Func_08020348
	ldr r4, [sp, #0]
	cmp r0, #0
	blt .L_080e42c0
	mov r2, r8
	movs r3, #2
	orrs r2, r3
	mov r8, r2
.L_080e42c0:
	subs r4, #1
	adds r7, #128
	adds r5, #128
	cmp r4, #0
	bge .L_080e4288
	mov r0, r8
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
