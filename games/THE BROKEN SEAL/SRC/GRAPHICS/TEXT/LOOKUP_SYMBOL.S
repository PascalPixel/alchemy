@ Finds the start of a message for DECODE_SYMBOL.S: it indexes the block table
@ at 0x080736b8 by the message number's high byte, adds the lengths of the
@ preceding messages in the block (a 0xff byte continues a length), and stores
@ the reader state {0, word pointer, bit word}, putting a sentinel bit on top
@ of the bit word with rrx from the carry of rsbs r4, r3, r4, lsl #3. It runs
@ from a RAM copy of exactly these 96 bytes. GCC never emits rrx. Every TBS
@ and TLA edition has it, differing only in the table address.
@ credit: library — TBS/TLA common ARM text and tile kernel bank
.syntax unified
.text
	.arm
	.global Func_08015570
Func_08015570:
	lsr	r3, r1, #8
	ldr	ip, [pc, #80]
	add	ip, ip, r3, lsl #3
	ldm	ip, {r2, r4}
	ands	r1, r1, #255
	beq .L_080155a0
.L_08015588:
	ldrb	r3, [r4], #1
	add	r2, r2, r3
	cmp	r3, #255
	beq .L_08015588
	subs	r1, r1, #1
	bne .L_08015588
.L_080155a0:
	mov	r3, #1
	ands	r4, r2, #3
	beq .L_080155c0
	rsbs	r4, r3, r4, lsl #3
	bic	r2, r2, #3
	ldr	r3, [r2], #4
	rrx	r3, r3
	lsr	r3, r3, r4
.L_080155c0:
	mov	r1, #0
	stm	r0, {r1, r2, r3}
	bx	lr
	.4byte 0x080736b8
