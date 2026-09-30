.syntax unified
	.thumb
	.global Func_08128194
	.thumb_func
Func_08128194:
	push {lr}
	movs r3, #193
	lsls r3, r3, #1
	cmp r0, r3
	bls .L_081281a2
	movs r0, #0
	b .L_081281aa
.L_081281a2:
	ldr r3, .L_081281ac
	lsls r2, r0, #3
	adds r2, #4
	ldrb r0, [r3, r2]
.L_081281aa:
	pop {pc}
.L_081281ac:
	.4byte Data_08130d0c
