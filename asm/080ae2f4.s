@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080ae2f4
	.thumb_func
Func_080ae2f4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #52]
	sub	sp, #84
	movs	r1, #1
	movs	r2, #0
	movs	r0, #0
	ldr	r3, [r3, #0]
	str	r1, [sp, #52]
	str	r0, [sp, #56]
	str	r2, [sp, #48]
	ldr	r2, [r3, #20]
	mov	r9, r3
	movs	r3, #13
	strb	r3, [r2, #5]
	mov	r3, sp
	adds	r3, #76
	movs	r7, #0
	str	r3, [sp, #20]
	str	r7, [sp, #76]
	str	r7, [r3, #4]
	movs	r3, #165
	lsls	r3, r3, #1
	ldr	r1, [pc, #4]
	movs	r2, #3
	add	r3, r9
	b.n	.L0
	.4byte 0x000000c8
	.4byte 0x03001f2c
.L0:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L0
	mov	r1, r9
	ldr	r0, [r1, #48]
	bl	Func_08015278
	movs	r0, #1
	bl	Func_080030f8
	add	r2, sp, #68
	movs	r7, #1
	str	r7, [sp, #68]
	movs	r0, #96
	str	r7, [r2, #4]
	mov	fp, r2
	bl	Func_08004970
	adds	r5, r0, #0
	movs	r0, #166
	lsls	r0, r0, #1
	bl	Func_08004970
	ldr	r3, [pc, #856]
	add	r3, r9
	adds	r6, r0, #0
	ldrb	r0, [r3, #0]
	bl	Func_08077008
	adds	r1, r0, #0
	add	r2, sp, #60
	adds	r1, #88
	add	r3, sp, #64
	str	r2, [sp, #0]
	adds	r0, r1, #0
	adds	r2, r5, #0
	bl	Func_080aae14
	mov	r3, fp
	str	r0, [sp, #68]
	str	r0, [r3, #4]
	adds	r0, r6, #0
	bl	Func_08002df0
	adds	r0, r5, #0
	bl	Func_08002df0
	ldr	r0, [sp, #68]
	movs	r1, #6
	subs	r0, #1
	bl	Func_080022ec
	adds	r0, #1
	str	r0, [sp, #68]
	cmp	r0, #0
	bne.n	.L1
	str	r7, [sp, #68]
.L1:
	mov	r1, fp
	ldr	r0, [r1, #4]
	movs	r1, #6
	subs	r0, #1
	bl	Func_080022ec
	mov	r2, fp
	adds	r0, #1
	str	r0, [r2, #4]
	cmp	r0, #0
	bne.n	.L2
	str	r7, [r2, #4]
.L2:
	mov	r3, r9
	adds	r3, #36
	str	r3, [sp, #44]
	movs	r5, #2
	movs	r6, #15
	adds	r0, r3, #0
	movs	r1, #0
	movs	r2, #5
	movs	r3, #15
	str	r5, [sp, #4]
	str	r6, [sp, #0]
	bl	Func_080a10d0
	mov	r0, r9
	movs	r2, #5
	movs	r1, #15
	adds	r0, #52
	movs	r3, #15
	str	r5, [sp, #4]
	str	r0, [sp, #40]
	str	r6, [sp, #0]
	bl	Func_080a10d0
	movs	r3, #134
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r0, [r3, #0]
	bl	Func_08015270
	mov	r1, r9
	ldr	r0, [r1, #16]
	bl	Func_08015270
	ldr	r5, [pc, #700]
	mov	r2, r9
	ldr	r1, [r2, #16]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
	adds	r5, #2
	mov	r3, r9
	ldr	r1, [r3, #16]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #16
	bl	Func_08015080
	movs	r2, #150
	lsls	r2, r2, #2
	ldr	r1, [sp, #20]
	add	r2, r9
	movs	r0, #0
	str	r2, [sp, #24]
	mov	r8, r0
	mov	sl, r1
.L18:
	ldr	r3, [pc, #656]
	ldr	r3, [r3, #0]
	str	r3, [sp, #36]
	ldr	r3, [pc, #656]
	ldr	r3, [r3, #0]
	str	r3, [sp, #32]
	ldr	r3, [pc, #652]
	ldr	r3, [r3, #0]
	str	r3, [sp, #28]
	ldr	r3, [sp, #52]
	cmp	r3, #0
	beq.n	.L3
	ldr	r0, [sp, #36]
	ldr	r1, [pc, #644]
	movs	r2, #1
	adds	r5, r0, r1
	strb	r2, [r5, #0]
	mov	r3, r9
	ldr	r0, [r3, #36]
	bl	Func_08015060
	mov	r1, r9
	ldr	r0, [r1, #52]
	bl	Func_08015060
	mov	r2, r9
	ldr	r1, [sp, #24]
	ldr	r0, [r2, #36]
	mov	r2, r8
	ldrb	r3, [r1, #0]
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	str	r2, [sp, #12]
	movs	r1, #3
	movs	r2, #1
	str	r1, [sp, #8]
	str	r2, [sp, #16]
	movs	r1, #0
	movs	r2, #0
	bl	Func_080acab8
	ldr	r1, [sp, #24]
	mov	r2, r8
	mov	r3, r9
	ldr	r0, [r3, #52]
	ldrb	r3, [r1, #0]
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	ldr	r2, [sp, #76]
	adds	r2, #1
	movs	r1, #3
	str	r2, [sp, #12]
	movs	r2, #1
	str	r1, [sp, #8]
	str	r2, [sp, #16]
	movs	r1, #0
	movs	r2, #0
	bl	Func_080acab8
	mov	r3, r8
	strb	r3, [r5, #0]
.L3:
	movs	r0, #0
	mov	r1, fp
	ldr	r3, [r0, r1]
	cmp	r3, #1
	ble.n	.L4
	mov	r2, r9
	movs	r5, #0
	ldr	r6, [r2, #52]
	cmp	r5, r3
	bge.n	.L5
	movs	r7, #0
	add	r7, fp
.L8:
	ldr	r3, [pc, #528]
	adds	r1, r5, r3
	cmp	r5, #9
	ble.n	.L6
	ldr	r1, [pc, #524]
.L6:
	ldr	r2, [sp, #20]
	movs	r0, #0
	ldr	r3, [r0, r2]
	cmp	r5, r3
	bne.n	.L7
	ldr	r3, [pc, #516]
	adds	r1, r1, r3
.L7:
	ldr	r3, [r7, #0]
	ldrh	r2, [r6, #8]
	subs	r2, r2, r3
	adds	r2, r2, r5
	mov	r0, r8
	movs	r3, #1
	str	r0, [sp, #0]
	negs	r3, r3
	subs	r2, #2
	adds	r0, r6, #0
	bl	Func_08015280
	ldr	r3, [r7, #0]
	adds	r5, #1
	cmp	r5, r3
	blt.n	.L8
.L5:
	mov	r0, fp
	movs	r1, #0
	ldr	r3, [r1, r0]
	ldrh	r2, [r6, #8]
	movs	r5, #1
	negs	r5, r5
	mov	r1, r8
	subs	r2, r2, r3
	str	r1, [sp, #0]
	adds	r0, r6, #0
	adds	r3, r5, #0
	subs	r2, #3
	ldr	r1, [pc, #460]
	bl	Func_08015280
	ldrh	r2, [r6, #8]
	mov	r3, r8
	str	r3, [sp, #0]
	subs	r2, #2
	adds	r0, r6, #0
	ldr	r1, [pc, #448]
	adds	r3, r5, #0
	bl	Func_08015280
	ldr	r0, [sp, #36]
	ldr	r2, [pc, #440]
	adds	r1, r0, r2
	ldrh	r2, [r6, #14]
	movs	r3, #2
	lsrs	r2, r2, #2
	lsls	r3, r2
	ldrb	r2, [r1, #0]
	orrs	r3, r2
	strb	r3, [r1, #0]
.L4:
	ldr	r3, [sp, #48]
	adds	r3, #1
	adds	r0, r3, #0
	movs	r1, #60
	str	r3, [sp, #48]
	bl	Func_080022fc
	subs	r0, #5
	movs	r0, #0
	movs	r1, #32
	movs	r2, #200
	movs	r3, #0
	bl	Func_080ad5b4
	ldr	r0, [sp, #52]
	cmp	r0, #0
	beq.n	.L9
	movs	r1, #0
	str	r1, [sp, #52]
	ldr	r0, [sp, #56]
	movs	r1, #2
	bl	Func_080aa538
	str	r0, [sp, #56]
.L9:
	ldr	r3, [sp, #48]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L10
	ldr	r0, [sp, #48]
	movs	r3, #4
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L11
	ldr	r3, [pc, #360]
	ldr	r0, [pc, #360]
	ldr	r1, [pc, #364]
	movs	r2, #32
	bl	Func_080072f0
	b.n	.L10
.L11:
	ldr	r3, [pc, #356]
	ldr	r0, [pc, #348]
	movs	r1, #32
	ldr	r2, [pc, #356]
	bl	Func_080072f0
.L10:
	ldr	r1, [sp, #32]
	movs	r3, #8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L12
	movs	r0, #113
	movs	r7, #2
	b.n	.L13
.L12:
	ldr	r3, [pc, #336]
	ldr	r2, [sp, #32]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L14
	movs	r0, #113
	movs	r7, #1
.L13:
	bl	Func_080f9010
	negs	r7, r7
	b.n	.L15
.L14:
	ldr	r0, [sp, #28]
	movs	r3, #32
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L16
	mov	r1, sl
	ldr	r0, [r1, #0]
	subs	r0, #1
	str	r0, [r1, #0]
	mov	r2, r8
	mov	r3, fp
	ldr	r1, [r2, r3]
	bl	Func_080aa538
	mov	r1, sl
	str	r0, [r1, #0]
	movs	r0, #111
	bl	Func_080f9010
	bl	Func_0800352c
	movs	r2, #1
	str	r2, [sp, #52]
	b.n	.L17
.L16:
	ldr	r0, [sp, #28]
	movs	r3, #16
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L17
	mov	r1, sl
	ldr	r3, [r1, #0]
	adds	r3, #1
	str	r3, [r1, #0]
	movs	r0, #111
	bl	Func_080f9010
	bl	Func_0800352c
	movs	r2, #1
	str	r2, [sp, #52]
	mov	r3, sl
	ldr	r0, [r3, #0]
	mov	r2, r8
	mov	r3, fp
	ldr	r1, [r2, r3]
	bl	Func_080aa538
	mov	r1, sl
	str	r0, [r1, #0]
.L17:
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L18
.L15:
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #208]
	bl	Func_080041d8
	ldr	r2, [pc, #140]
	movs	r5, #134
	mov	r8, r2
	ldr	r6, [pc, #144]
	ldr	r2, [r2, #0]
	movs	r3, #0
	lsls	r5, r5, #1
	mov	sl, r3
	add	r5, r9
	movs	r3, #1
	strb	r3, [r2, r6]
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_080a1114
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #5
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r2, #0
	movs	r3, #17
	adds	r0, r5, #0
	movs	r1, #13
	bl	Func_080a10d0
	movs	r1, #1
	ldr	r0, [sp, #44]
	bl	Func_080a1114
	movs	r1, #1
	ldr	r0, [sp, #40]
	bl	Func_080a1114
	mov	r1, r9
	ldr	r0, [r1, #48]
	bl	Func_08015270
	mov	r2, r9
	ldr	r0, [r2, #40]
	bl	Func_08015270
	mov	r3, r9
	ldr	r0, [r3, #16]
	bl	Func_08015270
	mov	r0, r8
	ldr	r3, [r0, #0]
	mov	r1, sl
	adds	r3, r3, r6
	strb	r1, [r3, #0]
	movs	r0, #1
	bl	Func_080030f8
	adds	r0, r7, #0
	add	sp, #84
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x0000021a
	.4byte 0x00000baa
	.4byte 0x03001e8c
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x00000ea6
	.4byte 0x0000f031
	.4byte 0x0000f030
	.4byte 0xfffff000
	.4byte 0x0000f128
	.4byte 0x0000f129
	.4byte 0x00000ea3
	.4byte 0x03001388
	.4byte 0x060052c0
	.4byte 0x080af26c
	.4byte 0x03000168
	.4byte 0x44444444
	.4byte 0x00000303
	.4byte 0x080a19a1
