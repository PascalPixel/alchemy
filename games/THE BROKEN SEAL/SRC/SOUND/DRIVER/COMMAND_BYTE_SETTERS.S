@ Sound command-byte setters using the driver's private r3 result contract.
.syntax unified
	.thumb

.global Func_080fa1d4
	.thumb_func
Func_080fa1d4:
	mov	ip, lr
	bl	Func_080fa1c8
	strb	r3, [r1, #25]
	cmp	r3, #0
	bne.n	.L_080fa1d4_0
	bl	Func_080fa1ac
.L_080fa1d4_0:
	bx	ip
	.balign 4, 0

.global Func_080fa1e8
	.thumb_func
Func_080fa1e8:
	mov	ip, lr
	bl	Func_080fa1c8
	strb	r3, [r1, #23]
	cmp	r3, #0
	bne.n	.L_080fa1e8_0
	bl	Func_080fa1ac
.L_080fa1e8_0:
	bx	ip
	.2byte 0
