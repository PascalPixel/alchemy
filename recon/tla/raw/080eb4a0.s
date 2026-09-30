.syntax unified
	.thumb
	.global Func_080eb4a0
	.thumb_func
Func_080eb4a0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r4, r2, #0
	ldr r2, [sp, #28]
	mov r9, r3
	mov lr, r2
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #180
	ldr r3, [r3]
	ldr r2, [r2, #96]
	mov r8, r3
	mov r12, r2
	cmp r1, #0
	blt .L_080eb512
	mov r7, r9
	adds r3, r1, r7
	cmp r3, #63
	bgt .L_080eb512
	cmp r4, #0
	blt .L_080eb512
	mov r2, lr
	adds r3, r4, r2
	cmp r3, #63
	bgt .L_080eb512
	movs r6, #0
	cmp r6, lr
	bge .L_080eb512
	lsls r3, r4, #6
	adds r3, r3, r1
	lsls r5, r3, #1
.L_080eb4e6:
	mov r3, r9
	cmp r3, #0
	ble .L_080eb50a
	mov r7, r8
	adds r1, r5, r7
	mov r2, r9
.L_080eb4f2:
	ldrb r4, [r0]
	adds r3, r4, #0
	cmp r3, #0
	beq .L_080eb500
	ldrh r3, [r1]
	mov r7, r12
	strb r4, [r7, r3]
.L_080eb500:
	subs r2, #1
	adds r0, #1
	adds r1, #2
	cmp r2, #0
	bne .L_080eb4f2
.L_080eb50a:
	adds r6, #1
	adds r5, #128
	cmp r6, lr
	blt .L_080eb4e6
.L_080eb512:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
