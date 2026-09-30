.syntax unified
	.thumb
	.global Func_081c03d8
	.thumb_func
Func_081c03d8:
	movs r2, #0
	strb r2, [r1, #22]
	strb r2, [r1, #26]
	ldrb r2, [r1, #24]
	cmp r2, #0
	bne .L_081c03e8
	movs r2, #12
	b .L_081c03ea
.L_081c03e8:
	movs r2, #3
.L_081c03ea:
	ldrb r3, [r1]
	orrs r3, r2
	strb r3, [r1]
	bx lr
	.2byte 0x0000
