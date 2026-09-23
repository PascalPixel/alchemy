@ Runs a list of 12-byte transfer records from a stack copy of exactly these
@ 104 bytes. A record without size bits goes straight to the DMA3 registers
@ with stm; otherwise its value is stored, ORed or cleared at the destination
@ with a byte, halfword or word access chosen by one compare
@ (ldrbcc/ldrheq/ldrgt). It saves r5 alone with stmfd sp!, {r5}; GCC saves lr
@ with any register. Every TBS and TLA edition has these bytes.
@ credit: library — TBS/TLA common ARM graphics and runtime kernel bank
.syntax unified
	.arm
	.global Func_08002cf4
Func_08002cf4:
	stmfd	sp!, {r5}
	add	r0, r0, #4
.L_08002cfc:
	ldm	r0!, {r2, r3, r4}
	ands	ip, r4, #196608
	beq	.L_08002d4c
	tst	r4, #786432
	beq	.L_08002d2c
	cmp	ip, #131072
	ldrbcc	r5, [r3]
	ldrheq	r5, [r3]
	ldrgt	r5, [r3]
	tst	r4, #262144
	orrne	r2, r5, r2
	biceq	r2, r5, r2
.L_08002d2c:
	cmp	ip, #131072
	strbcc	r2, [r3]
	strheq	r2, [r3]
	strgt	r2, [r3]
.L_08002d3c:
	subs	r1, r1, #1
	bgt	.L_08002cfc
	ldmfd	sp!, {r5}
	bx	lr
.L_08002d4c:
	mov	ip, #67108864
	add	ip, ip, #212
	stm	ip, {r2, r3, r4}
	b	.L_08002d3c
