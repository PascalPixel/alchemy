@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080e3a3c
	.thumb_func
Func_080e3a3c:
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r1, [pc, #84]
	movs	r0, #41
	bl	Func_080048f4
	ldr	r1, [pc, #80]
	movs	r0, #39
	bl	Func_080048b0
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r0, #40
	bl	Func_080048b0
	ldr	r3, [r5, #0]
	adds	r1, r3, #0
	subs	r1, #100
	cmp	r1, #35
	bhi.n	.L0
	adds	r0, r5, #0
	bl	Func_080e47b8
	b.n	.L1
.L0:
	cmp	r3, #199
	ble.n	.L2
	adds	r0, r5, #0
	bl	Func_080e40a4
	b.n	.L1
.L2:
	adds	r0, r5, #0
	bl	Func_080e3aa0
.L1:
	movs	r0, #40
	bl	Func_08002dd8
	movs	r0, #39
	bl	Func_08002dd8
	movs	r0, #41
	bl	Func_08002dd8
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000060e
	.4byte 0x0000782c
