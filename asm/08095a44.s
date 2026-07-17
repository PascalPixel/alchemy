@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08095a44
	.thumb_func
Func_08095a44:
	push	{r5, r6, r7, lr}
	sub	sp, #12
	adds	r6, r0, #0
	bl	Func_08092054
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L0
	b.n	.L1
.L0:
	bl	Func_080958a8
	ldr	r3, [pc, #284]
	ldr	r0, [pc, #284]
	ldr	r5, [r3, #0]
	bl	Func_080b0048
	movs	r0, #173
	bl	Func_080f9010
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_080925cc
	movs	r0, #174
	bl	Func_080f9010
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_080925cc
	movs	r0, #175
	bl	Func_080f9010
	movs	r1, #1
	adds	r0, r6, #0
	bl	Func_080925cc
	movs	r0, #20
	bl	Func_080030f8
	movs	r0, #140
	bl	Func_080f9010
	ldr	r3, [pc, #228]
	movs	r0, #40
	str	r3, [r7, #108]
	bl	Func_080030f8
	movs	r0, #153
	bl	Func_080f9010
	movs	r1, #12
	movs	r2, #22
	adds	r0, r6, #0
	bl	Func_08092560
	ldr	r3, [r7, #8]
	mov	r6, sp
	str	r3, [r6, #0]
	ldr	r3, [r7, #12]
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	adds	r0, r6, #0
	str	r3, [r6, #8]
	bl	Func_080974d8
	adds	r0, r7, #0
	bl	Func_080090d0
	movs	r0, #164
	bl	Func_080f9010
	adds	r5, #88
	movs	r7, #23
.L2:
	movs	r1, #142
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #8]
	adds	r0, r5, #0
	lsls	r1, r1, #1
	bl	Func_0809ba90
	adds	r0, r5, #0
	ldr	r1, [pc, #152]
	bl	Func_0809ba7c
	adds	r0, r5, #0
	movs	r1, #7
	bl	Func_0809ba70
	bl	Func_08004458
	lsls	r1, r0, #3
	subs	r1, r1, r0
	lsrs	r1, r1, #16
	ldr	r0, [r5, #0]
	bl	Func_08009248
	bl	Func_08004458
	movs	r1, #3
	bl	Func_080022f4
	movs	r3, #128
	lsls	r3, r3, #9
	adds	r0, r0, r3
	str	r0, [r5, #44]
	str	r0, [r5, #40]
	subs	r7, #1
	movs	r0, #1
	bl	Func_080030f8
	adds	r5, #72
	cmp	r7, #0
	bge.n	.L2
	movs	r0, #60
	bl	Func_080030f8
	ldr	r5, [pc, #88]
	movs	r3, #250
	lsls	r3, r3, #1
	adds	r5, r5, r3
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	ldr	r0, [r5, #0]
	bl	Func_08092adc
	movs	r0, #20
	bl	Func_080030f8
	ldr	r0, [r5, #0]
	bl	Func_08092054
	movs	r1, #28
	bl	Func_08009080
	movs	r0, #40
	bl	Func_080030f8
	movs	r0, #164
	bl	Func_080f9010
	movs	r0, #100
	bl	Func_080030f8
	bl	Func_080b0050
	bl	Func_080958e4
.L1:
	add	sp, #12
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f30
	.4byte 0x0020118c
	.4byte 0x0809592d
	.4byte 0x08095939
	.4byte 0x02000240
