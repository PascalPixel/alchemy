.syntax unified
	.thumb
	.global Func_080fa458
	.thumb_func
Func_080fa458:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r0, #13
	adds r3, #76
	movs r1, #31
.L_080fa468:
	ldmia r3!, {r2}
	cmp r2, #0
	beq .L_080fa470
	strb r0, [r2, #5]
.L_080fa470:
	subs r1, #1
	cmp r1, #0
	bge .L_080fa468
	pop {pc}
