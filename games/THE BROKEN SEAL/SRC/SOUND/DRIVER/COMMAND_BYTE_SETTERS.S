@ Two track-byte setters (offsets 25 and 23 of the track in r1): each reads
@ the next command byte, stores it and calls 0x080fa1ac when it is zero. They
@ keep their return address in ip (mov ip, lr ... bx ip) and take the byte
@ from r3 after bl; both approved compilers push lr and read results from r0.
@ Every TBS and TLA edition has these bytes.
@ The zero halfwords of alignment at 0x080fa1e6 and 0x080fa1fa never run and
@ carry no credit.
@ credit: library — TBS/TLA common sound command-byte reader and setters
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
AlchemyUncredited_080fa1e6:
	.balign 4, 0

.global Func_080fa1e8
	.thumb_func
Func_080fa1e8:
AlchemyUncreditedEnd_080fa1e6:
	mov	ip, lr
	bl	Func_080fa1c8
	strb	r3, [r1, #23]
	cmp	r3, #0
	bne.n	.L_080fa1e8_0
	bl	Func_080fa1ac
.L_080fa1e8_0:
	bx	ip
AlchemyUncredited_080fa1fa:
	.2byte 0
AlchemyUncreditedEnd_080fa1fa:
