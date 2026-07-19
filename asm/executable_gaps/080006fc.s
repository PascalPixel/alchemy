.syntax unified
.text
	.thumb
	.set sub_0800070c, 0x0800070c
	.global Func_080006fc
	.thumb_func
Func_080006fc:
	ldr	r0, [pc, #108]
	ldr	r0, [r0, #0]
	ldr	r2, [pc, #100]
	ldr	r3, [r0, #0]
	cmp	r2, r3
	beq sub_0800070c
	bx	lr
	.inst.n 0x4718
