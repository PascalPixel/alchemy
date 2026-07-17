@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080db264
	.thumb_func
Func_080db264:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #48]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #44
	str	r3, [sp, #32]
	ldr	r5, [pc, #40]
	mov	sl, r1
	ldr	r2, [r2, #8]
	add	r5, sl
	str	r2, [sp, #24]
	str	r0, [r5, #0]
	movs	r0, #1
	bl	Func_080cdb24
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	cmp	r3, #2
	bne.n	.L0
	ldr	r2, [pc, #16]
	ldr	r3, [pc, #4]
	b.n	.L1
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
.L0:
	ldr	r2, [pc, #60]
	ldr	r3, [pc, #56]
.L1:
	strh	r3, [r2, #0]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	Func_080ed408
	ldr	r5, [pc, #44]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	str	r3, [sp, #36]
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	bl	Func_080ed408
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r2, sp
	ldr	r1, [pc, #16]
	adds	r2, #36
	b.n	.L2
	.4byte 0x00000100
	.4byte 0x04000020
	.4byte 0x03001e50
	.4byte 0x0000060e
.L2:
	str	r2, [sp, #12]
	ldr	r0, [pc, #880]
	str	r3, [r2, #4]
	add	r1, sl
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #864]
	ldr	r1, [sp, #24]
	bl	Func_080e0524
	ldr	r3, [pc, #860]
	add	r3, sl
	ldr	r2, [r3, #0]
	ldr	r3, [r2, #24]
	cmp	r3, #2
	bne.n	.L3
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L4
	ldr	r2, [pc, #848]
	ldr	r3, [pc, #848]
	b.n	.L5
.L4:
	ldr	r2, [pc, #840]
	movs	r3, #128
	lsls	r3, r3, #5
	b.n	.L5
.L3:
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L6
	ldr	r2, [pc, #828]
	ldr	r3, [pc, #832]
.L5:
	str	r3, [r2, #0]
.L6:
	movs	r3, #0
	ldr	r7, [pc, #832]
	mov	r8, r3
.L7:
	bl	Func_08004458
	ldr	r6, [pc, #828]
	movs	r1, #128
	lsls	r1, r1, #1
	ands	r6, r0
	adds	r6, r6, r1
	bl	Func_08004458
	ldr	r5, [pc, #816]
	movs	r3, #128
	ldr	r2, [pc, #816]
	lsls	r3, r3, #7
	str	r3, [r7, #0]
	ands	r5, r0
	movs	r3, #224
	adds	r5, r5, r2
	lsls	r3, r3, #7
	str	r3, [r7, #4]
	adds	r0, r5, #0
	bl	Func_08002322
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #16
	str	r3, [r7, #8]
	adds	r0, r5, #0
	bl	Func_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #16
	str	r3, [r7, #20]
	movs	r3, #0
	str	r3, [r7, #24]
	movs	r1, #128
	movs	r3, #1
	add	r8, r3
	lsls	r1, r1, #3
	adds	r7, #28
	cmp	r8, r1
	bne.n	.L7
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #744]
	movs	r3, #75
	add	r2, sl
	str	r3, [r2, #0]
	adds	r1, #128
	ldr	r0, [pc, #740]
	bl	Func_080041d8
	movs	r0, #138
	bl	Func_080f9010
	movs	r2, #0
	str	r2, [sp, #28]
.L27:
	ldr	r3, [sp, #28]
	cmp	r3, #20
	bne.n	.L8
	movs	r0, #133
	bl	Func_080b50e8
.L8:
	ldr	r1, [sp, #28]
	cmp	r1, #15
	ble.n	.L9
	b.n	.L10
.L9:
	adds	r0, r1, #0
	movs	r1, #5
	bl	Func_080022fc
	cmp	r0, #2
	bne.n	.L11
	movs	r1, #128
	ldr	r3, [pc, #692]
	ldr	r0, [sp, #32]
	lsls	r1, r1, #7
	ldr	r2, [pc, #692]
	bl	Func_080072f0
.L11:
	ldr	r3, [sp, #28]
	movs	r2, #0
	lsls	r3, r3, #11
	str	r2, [sp, #20]
	str	r3, [sp, #8]
.L18:
	movs	r1, #0
	ldr	r2, [sp, #8]
	str	r1, [sp, #16]
	movs	r1, #128
	lsls	r1, r1, #7
	adds	r3, r2, r1
	ldr	r2, [sp, #20]
	adds	r5, r2, #0
	muls	r5, r3
	adds	r0, r5, #0
	bl	Func_08002322
	ldr	r1, [sp, #28]
	movs	r3, #32
	subs	r3, r3, r1
	muls	r3, r0
	asrs	r3, r3, #16
	adds	r3, #64
	adds	r0, r5, #0
	mov	r9, r3
	bl	Func_0800231c
	ldr	r3, [pc, #588]
	add	r3, sl
	lsls	r0, r0, #3
	ldr	r3, [r3, #0]
	asrs	r0, r0, #16
	negs	r0, r0
	ldr	r3, [r3, #24]
	adds	r6, r0, #0
	subs	r6, #8
	cmp	r3, #0
	bne.n	.L12
	bl	Func_08004458
	movs	r3, #3
	ands	r0, r3
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r5, r3, #4
	subs	r5, r5, r3
	ldr	r2, [pc, #596]
	lsls	r5, r5, #6
	add	r5, sl
	adds	r5, r5, r2
	bl	Func_08004458
	movs	r2, #7
	ands	r2, r0
	movs	r3, #24
	add	r2, r9
	str	r3, [sp, #0]
	movs	r3, #120
	str	r3, [sp, #4]
	subs	r2, #16
	ldr	r4, [sp, #36]
	ldr	r0, [sp, #32]
	adds	r1, r5, #0
	adds	r3, r6, #0
	bl	Func_080072f4
	b.n	.L13
.L12:
	bl	Func_08004458
	movs	r3, #3
	ands	r0, r3
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r5, r3, #4
	subs	r5, r5, r3
	lsls	r5, r5, #6
	ldr	r3, [pc, #536]
	add	r5, sl
	adds	r5, r5, r3
	bl	Func_08004458
	ldr	r3, [sp, #20]
	movs	r1, #1
	ands	r1, r3
	movs	r3, #24
	str	r3, [sp, #0]
	movs	r2, #7
	movs	r3, #120
	ands	r2, r0
	str	r3, [sp, #4]
	ldr	r3, [sp, #12]
	lsls	r1, r1, #2
	add	r2, r9
	ldr	r4, [r1, r3]
	subs	r2, #16
	ldr	r0, [sp, #32]
	adds	r1, r5, #0
	adds	r3, r6, #0
	bl	Func_080072f4
.L13:
	adds	r3, r6, #0
	adds	r3, #112
	movs	r1, #0
	lsls	r3, r3, #16
	ldr	r7, [pc, #448]
	mov	r8, r1
	mov	fp, r3
.L16:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	bne.n	.L14
	bl	Func_08004458
	ldr	r6, [pc, #468]
	ands	r6, r0
	bl	Func_08004458
	ldr	r5, [pc, #432]
	ldr	r2, [pc, #436]
	mov	r1, r9
	ands	r5, r0
	lsls	r3, r1, #16
	adds	r5, r5, r2
	mov	r2, fp
	str	r2, [r7, #4]
	str	r3, [r7, #0]
	adds	r0, r5, #0
	bl	Func_08002322
	adds	r6, #128
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #9
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	Func_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #7
	str	r3, [r7, #16]
	bl	Func_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #32
	str	r3, [r7, #24]
	ldr	r3, [sp, #16]
	adds	r3, #1
	str	r3, [sp, #16]
	ldr	r3, [pc, #340]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #24]
	ldr	r1, [pc, #388]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r1, r3]
	ldr	r2, [sp, #16]
	cmp	r2, r3
	beq.n	.L15
.L14:
	movs	r3, #1
	movs	r1, #128
	add	r8, r3
	lsls	r1, r1, #3
	adds	r7, #28
	cmp	r8, r1
	bne.n	.L16
.L15:
	ldr	r2, [sp, #20]
	adds	r2, #1
	str	r2, [sp, #20]
	cmp	r2, #4
	beq.n	.L17
	b.n	.L18
.L17:
	ldr	r2, [pc, #352]
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
.L10:
	movs	r3, #0
	ldr	r5, [pc, #296]
	mov	r8, r3
.L22:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	ble.n	.L19
	subs	r3, #1
	str	r3, [r5, #24]
	movs	r1, #60
	adds	r0, r5, #0
	ldr	r2, [pc, #328]
	bl	Func_080e3908
	movs	r1, #240
	ldr	r3, [r5, #4]
	lsls	r1, r1, #15
	cmp	r3, r1
	ble.n	.L20
	ldr	r3, [r5, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r5, #16]
	b.n	.L19
.L20:
	ldr	r2, [r5, #0]
	ldr	r1, [pc, #300]
	cmp	r2, r1
	bhi.n	.L19
	cmp	r3, #0
	blt.n	.L19
	ldr	r0, [r5, #24]
	asrs	r6, r2, #16
	asrs	r7, r3, #16
	cmp	r0, #0
	bge.n	.L21
	adds	r0, #7
.L21:
	asrs	r0, r0, #3
	adds	r0, #1
	lsls	r4, r0, #1
	ldr	r2, [pc, #276]
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #24]
	adds	r1, r2, r1
	lsrs	r2, r0, #31
	adds	r2, r0, r2
	asrs	r2, r2, #1
	subs	r3, r7, r0
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	subs	r2, r6, r2
	ldr	r4, [sp, #36]
	ldr	r0, [sp, #32]
	bl	Func_080072f4
.L19:
	movs	r3, #1
	movs	r1, #128
	add	r8, r3
	lsls	r1, r1, #3
	adds	r5, #28
	cmp	r8, r1
	bne.n	.L22
	ldr	r3, [sp, #28]
	subs	r3, #4
	cmp	r3, #91
	bhi.n	.L23
	movs	r2, #0
	mov	r8, r2
	ldr	r2, [pc, #144]
	mov	r1, sl
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L23
	movs	r6, #36
	movs	r5, #4
.L25:
	ldr	r3, [sp, #28]
	cmp	r3, r5
	bne.n	.L24
	mov	r1, sl
	ldr	r3, [r1, r2]
	ldrsh	r0, [r3, r6]
	movs	r3, #10
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r3, r8
	bl	Func_080d6888
.L24:
	ldr	r2, [pc, #104]
	movs	r3, #1
	mov	r1, sl
	add	r8, r3
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	adds	r6, #2
	adds	r5, #4
	cmp	r8, r3
	bne.n	.L25
.L23:
	movs	r0, #2
	movs	r1, #4
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r2, [pc, #148]
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r2, [sp, #28]
	adds	r2, #1
	str	r2, [sp, #28]
	cmp	r2, #64
	beq.n	.L26
	b.n	.L27
.L26:
	ldr	r0, [pc, #80]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x000000c4
	.4byte 0x00000073
	.4byte 0x00007828
	.4byte 0x04000028
	.4byte 0xfffff000
	.4byte 0xffff8000
	.4byte 0x02010000
	.4byte 0x000003ff
	.4byte 0x00007fff
	.4byte 0xffffc000
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x03000168
	.4byte 0x10101010
	.4byte 0x0000060e
	.4byte 0x000001ff
	.4byte 0x080eeadc
	.4byte 0x000077a8
	.4byte 0xfffff800
	.4byte 0x007effff
	.4byte 0x080ede48
	.4byte 0x00007824
