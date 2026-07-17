@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08077f70
	.thumb_func
Func_08077f70:
	push	{r5, r6, lr}
	movs	r0, #32
	bl	Func_08079374
	movs	r0, #33
	bl	Func_08079374
	ldr	r0, [pc, #156]
	bl	Func_08079358
	movs	r0, #5
	bl	Func_08079ae8
	movs	r0, #5
	bl	Func_08077428
	ldr	r0, [pc, #140]
	bl	Func_08079374
	movs	r0, #141
	lsls	r0, r0, #1
	bl	Func_08079358
	movs	r6, #0
.L7:
	adds	r0, r6, #0
	bl	Func_08077394
	adds	r5, r0, #0
	ldrh	r1, [r5, #52]
	ldrh	r3, [r5, #54]
	strh	r1, [r5, #56]
	strh	r3, [r5, #58]
	lsls	r1, r1, #16
	asrs	r1, r1, #16
	lsls	r0, r1, #14
	bl	Func_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L0
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L0
	adds	r3, r0, #0
.L0:
	strh	r3, [r5, #20]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L1
	movs	r1, #56
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	beq.n	.L1
	movs	r3, #1
	strh	r3, [r5, #20]
.L1:
	movs	r2, #58
	ldrsh	r0, [r5, r2]
	movs	r3, #54
	ldrsh	r1, [r5, r3]
	lsls	r0, r0, #14
	bl	Func_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L2
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L2
	adds	r3, r0, #0
.L2:
	strh	r3, [r5, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L3
	movs	r1, #58
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	beq.n	.L3
	movs	r3, #1
	strh	r3, [r5, #22]
.L3:
	ldr	r0, [pc, #4]
	movs	r1, #0
	movs	r2, #216
	b.n	.L4
	.4byte 0x000001ff
	.4byte 0x00000901
	.4byte 0x0000011b
.L6:
	adds	r2, #2
	adds	r1, #1
.L4:
	cmp	r1, #14
	bgt.n	.L5
	ldrh	r3, [r2, r5]
	ands	r3, r0
	cmp	r3, #15
	bne.n	.L6
	ldr	r3, [pc, #8]
	adds	r0, r6, #0
	strh	r3, [r2, r5]
	bl	Func_08078708
	b.n	.L5
	.4byte 0x00000010
.L5:
	adds	r0, r6, #0
	bl	Func_08079ae8
	adds	r0, r6, #0
	adds	r6, #1
	bl	Func_08077428
	cmp	r6, #1
	ble.n	.L7
	movs	r1, #140
	movs	r0, #0
	bl	Func_08078e28
	movs	r1, #149
	movs	r0, #0
	bl	Func_08078e28
	movs	r1, #140
	movs	r0, #1
	bl	Func_08078e28
	movs	r1, #141
	movs	r0, #2
	bl	Func_08078e28
	ldr	r2, [pc, #16]
	movs	r1, #150
	ldr	r3, [r2, #16]
	lsls	r1, r1, #1
	adds	r3, r3, r1
	str	r3, [r2, #16]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x02000240
