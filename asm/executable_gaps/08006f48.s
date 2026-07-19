.syntax unified
.text
	.thumb
	.global Func_08006f48
	.thumb_func
Func_08006f48:
	adds	r2, r0, #0
	ldr	r0, [pc, #8]
	ldr	r0, [r0, #0]
	ldr	r1, [r0, #4]
	b .L_08006f5a
	.inst.n 0x0000
	.4byte 0x02004c08
.L_08006f58:
	subs	r1, #1
.L_08006f5a:
	cmp	r1, #0
	beq .L_08006f66
	ldrb	r0, [r2, #0]
	adds	r2, #1
	cmp	r0, #255
	beq .L_08006f58
.L_08006f66:
	adds	r0, r1, #0
	bx	lr
	.inst.n 0x0000
