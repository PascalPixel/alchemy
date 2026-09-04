@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x08006910 (148 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_08006910
	.thumb_func
Func_08006910:
	push	{r4, lr}
	ldr	r2, [pc, #28]	@ (0x8006930)
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #28]	@ (0x8006934)
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
	bl	Func_08006878
	lsls	r0, r0, #16
	lsrs	r3, r0, #16
	ldr	r2, [pc, #12]	@ (0x8006938)
	movs	r4, #1
	b.n	.L_0800693e
	movs	r0, r0
	.4byte 0x04000204
	.4byte 0x0000fffc
	.2byte 0x7a0c
	.2byte 0x0800
.L_0800693c:
	adds	r2, #4
.L_0800693e:
	ldr	r1, [r2, #0]
	adds	r0, r1, #0
	adds	r0, #40	@ 0x28
	ldrb	r0, [r0, #0]
	cmp	r0, #0
	beq.n	.L_08006952
	ldrh	r0, [r1, #40]	@ 0x28
	cmp	r3, r0
	bne.n	.L_0800693c
	movs	r4, #0
.L_08006952:
	ldr	r1, [pc, #56]	@ (0x800698c)
	ldr	r0, [r2, #0]
	ldr	r0, [r0, #0]
	str	r0, [r1, #0]
	ldr	r1, [pc, #52]	@ (0x8006990)
	ldr	r0, [r2, #0]
	ldr	r0, [r0, #4]
	str	r0, [r1, #0]
	ldr	r1, [pc, #48]	@ (0x8006994)
	ldr	r0, [r2, #0]
	ldr	r0, [r0, #8]
	str	r0, [r1, #0]
	ldr	r1, [pc, #44]	@ (0x8006998)
	ldr	r0, [r2, #0]
	ldr	r0, [r0, #12]
	str	r0, [r1, #0]
	ldr	r1, [pc, #40]	@ (0x800699c)
	ldr	r0, [r2, #0]
	ldr	r0, [r0, #16]
	str	r0, [r1, #0]
	ldr	r1, [pc, #36]	@ (0x80069a0)
	ldr	r0, [r2, #0]
	adds	r0, #20
	str	r0, [r1, #0]
	adds	r0, r4, #0
	pop	{r4}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02004c04
	.4byte 0x02004c10
	.4byte 0x02004c14
	.4byte 0x02004c00
	.4byte 0x02004c18
	.4byte 0x02004c08
