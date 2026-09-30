.syntax unified
	.thumb
	.global Func_080b06a8
	.thumb_func
Func_080b06a8:
	push {lr}
	ldr r2, .L_080b06c4
	movs r1, #0
.L_080b06ae:
	ldrh r3, [r2]
	adds r2, #2
	cmp r0, r3
	bne .L_080b06ba
	movs r0, #1
	b .L_080b06c2
.L_080b06ba:
	adds r1, #1
	cmp r1, #46
	bls .L_080b06ae
	movs r0, #0
.L_080b06c2:
	pop {pc}
.L_080b06c4:
	.4byte Data_080c6b18
