.syntax unified
	.thumb
	.global Func_080b06c8
	.thumb_func
Func_080b06c8:
	push {lr}
	ldr r2, .L_080b06e4
	movs r1, #0
.L_080b06ce:
	ldrh r3, [r2]
	adds r2, #2
	cmp r0, r3
	bne .L_080b06da
	movs r0, #1
	b .L_080b06e2
.L_080b06da:
	adds r1, #1
	cmp r1, #28
	bls .L_080b06ce
	movs r0, #0
.L_080b06e2:
	pop {pc}
.L_080b06e4:
	.4byte Data_080c6b76
