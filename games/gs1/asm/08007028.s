@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x08007028 (112 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_08007028
	.thumb_func
Func_08007028:
	push	{r4, r5, r6, lr}
	sub	sp, #64	@ 0x40
	mov	r0, sp
	bl	Func_08006ac0
	ldr	r5, [pc, #76]	@ (0x8007080)
	ldrh	r0, [r5, #0]
	ldr	r6, [pc, #76]	@ (0x8007084)
	ands	r0, r6
	ldr	r1, [pc, #76]	@ (0x8007088)
	ldrh	r1, [r1, #36]	@ 0x24
	orrs	r0, r1
	strh	r0, [r5, #0]
	ldr	r1, [pc, #72]	@ (0x800708c)
	movs	r4, #170	@ 0xaa
	strb	r4, [r1, #0]
	ldr	r3, [pc, #68]	@ (0x8007090)
	movs	r2, #85	@ 0x55
	strb	r2, [r3, #0]
	movs	r0, #128	@ 0x80
	strb	r0, [r1, #0]
	strb	r4, [r1, #0]
	strb	r2, [r3, #0]
	movs	r0, #16
	strb	r0, [r1, #0]
	ldr	r0, [pc, #56]	@ (0x8007094)
	movs	r1, #224	@ 0xe0
	lsls	r1, r1, #20
	ldr	r3, [r0, #0]
	movs	r0, #3
	movs	r2, #255	@ 0xff
	bl	Func_080072f0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	ldrh	r1, [r5, #0]
	ands	r1, r6
	movs	r2, #3
	orrs	r1, r2
	strh	r1, [r5, #0]
	add	sp, #64	@ 0x40
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x08007c10
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.4byte 0x02004c00
