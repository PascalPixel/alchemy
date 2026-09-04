@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x080fa514 (72 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_080fa514
	.thumb_func
Func_080fa514:
	push	{r4, r5, r6, r7, lr}
	ldrb	r5, [r0, #8]
	ldr	r4, [r0, #44]	@ 0x2c
	cmp	r5, #0
	ble.n	.L_080fa556
	movs	r7, #128	@ 0x80
.L_080fa520:
	ldrb	r1, [r4, #0]
	adds	r0, r7, #0
	ands	r0, r1
	cmp	r0, #0
	beq.n	.L_080fa54e
	movs	r6, #64	@ 0x40
	adds	r0, r6, #0
	ands	r0, r1
	cmp	r0, #0
	beq.n	.L_080fa54e
	adds	r0, r4, #0
	bl	Func_080fa68c
	strb	r7, [r4, #0]
	movs	r0, #2
	strb	r0, [r4, #15]
	strb	r6, [r4, #19]
	movs	r0, #22
	strb	r0, [r4, #25]
	adds	r1, r4, #0
	adds	r1, #36	@ 0x24
	movs	r0, #1
	strb	r0, [r1, #0]
.L_080fa54e:
	subs	r5, #1
	adds	r4, #80	@ 0x50
	cmp	r5, #0
	bgt.n	.L_080fa520
.L_080fa556:
	pop	{r4, r5, r6, r7}
	pop	{r0}
	bx	r0
