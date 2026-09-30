.syntax unified
	.thumb
	.global Func_080228bc
	.thumb_func
Func_080228bc:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #24]
	movs r1, #0
	adds r2, r3, #0
	adds r2, #28
.L_080228ca:
	ldr r3, [r2]
	cmp r3, r0
	bne .L_080228d4
	ldr r0, [r2, #4]
	b .L_080228de
.L_080228d4:
	adds r1, #1
	adds r2, #8
	cmp r1, #7
	bls .L_080228ca
	movs r0, #0
.L_080228de:
	pop {pc}
