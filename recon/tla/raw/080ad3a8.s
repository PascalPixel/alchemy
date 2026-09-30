.syntax unified
	.thumb
	.global Func_080ad3a8
	.thumb_func
Func_080ad3a8:
	push {lr}
	movs r3, #250
	subs r0, #8
	lsls r3, r3, #2
	cmp r0, r3
	bcc .L_080ad3b6
	movs r0, #0
.L_080ad3b6:
	movs r3, #76
	muls r0, r3
	ldr r3, .L_080ad3c0
	adds r0, r0, r3
	pop {pc}
.L_080ad3c0:
	.4byte Data_080b9e7c
