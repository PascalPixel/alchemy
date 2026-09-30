.syntax unified
	.thumb
	.global Func_080fd6b0
	.thumb_func
Func_080fd6b0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	mov r8, r0
	adds r6, r3, #0
	adds r6, #76
	mov r5, r8
	movs r7, #31
.L_080fd6c8:
	ldrh r1, [r5]
	adds r5, #2
	cmp r1, #0
	beq .L_080fd6dc
	ldr r3, [r6]
	movs r0, #4
	ldrb r2, [r3, #14]
	movs r3, #0
	bl Func_08038288
.L_080fd6dc:
	subs r7, #1
	adds r6, #4
	cmp r7, #0
	bge .L_080fd6c8
	mov r0, r8
	bl Func_080facd8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
