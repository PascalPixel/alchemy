@ Sound command-byte reader using the driver's private r3 result contract.
.syntax unified
	.thumb

.global Func_080fa1c8
.thumb_func
Func_080fa1c8:
	ldr	r2, [r1, #64]
	adds	r3, r2, #1
	str	r3, [r1, #64]
	ldrb	r3, [r2, #0]
	bx	lr
