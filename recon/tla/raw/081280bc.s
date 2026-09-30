.syntax unified
	.thumb
	.global Func_081280bc
	.thumb_func
Func_081280bc:
	push {lr}
	movs r3, #193
	lsls r3, r3, #1
	cmp r0, r3
	bls .L_081280cc
	ldr r3, .L_081280d4
	ldrh r0, [r3]
	b .L_081280d2
.L_081280cc:
	ldr r3, .L_081280d4
	lsls r2, r0, #3
	ldrh r0, [r3, r2]
.L_081280d2:
	pop {pc}
.L_081280d4:
	.4byte Data_08130d0c
