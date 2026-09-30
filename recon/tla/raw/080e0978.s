.syntax unified
	.thumb
	.global Func_080e0978
	.thumb_func
Func_080e0978:
	push {r5, r6, r7, lr}
	movs r3, #192
	sub sp, #8
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	add r1, sp, #4
	adds r3, #224
	mov r2, sp
	adds r6, r0, #0
	ldr r7, [r3]
	bl Func_080d9a74
	cmp r0, #0
	beq .L_080e099e
	adds r2, r7, #0
	adds r2, #32
	movs r3, #1
	strb r3, [r2]
	b .L_080e09b8
.L_080e099e:
	movs r1, #208
	lsls r1, r1, #4
	adds r1, #50
	adds r3, r5, r1
	strb r6, [r3]
	ldr r2, [sp, #4]
	adds r1, #1
	adds r3, r5, r1
	strb r2, [r3]
	ldr r2, [sp, #0]
	subs r1, #2
	adds r3, r5, r1
	strb r2, [r3]
.L_080e09b8:
	bl Func_080e09c0
	add sp, #8
	pop {r5, r6, r7, pc}
