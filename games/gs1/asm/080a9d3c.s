@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x080a9d3c (72 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_080a9d3c
	.thumb_func
Func_080a9d3c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #60]	@ (0x80a9d80)
	mov	r8, r0
	ldr	r5, [r3, #0]
	bl	Func_080a9d84
	movs	r6, #0
	adds	r5, #200	@ 0xc8
	movs	r7, #88	@ 0x58
.L_080a9d52:
	ldmia	r5!, {r0}
	cmp	r0, #0
	beq.n	.L_080a9d70
	mov	r2, r8
	ldrb	r3, [r2, r6]
	cmp	r3, #0
	beq.n	.L_080a9d70
	movs	r3, #8
	strh	r3, [r0, #6]
	movs	r3, #240	@ 0xf0
	strh	r7, [r0, #8]
	strb	r3, [r0, #15]
	bl	Func_080a17c4
	adds	r7, #16
.L_080a9d70:
	adds	r6, #1
	cmp	r6, #4
	ble.n	.L_080a9d52
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
