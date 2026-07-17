@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800d340
	.thumb_func
Func_0800d340:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #548]
	ldr	r5, [r3, #0]
	adds	r2, r5, #0
	sub	sp, #16
	movs	r0, #13
	adds	r2, #85
	str	r0, [sp, #12]
	str	r2, [sp, #0]
.L30:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L0
	b.n	.L1
.L0:
	ldr	r4, [r5, #12]
	ldr	r3, [r5, #8]
	ldr	r2, [sp, #0]
	str	r4, [sp, #8]
	mov	fp, r3
	ldr	r0, [r5, #16]
	ldrb	r3, [r2, #12]
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L2
	b.n	.L3
.L2:
	movs	r3, #0
	str	r3, [sp, #4]
	movs	r4, #128
	ldr	r3, [r5, #56]
	lsls	r4, r4, #24
	cmp	r3, r4
	bne.n	.L4
	b.n	.L5
.L4:
	mov	r2, fp
	subs	r0, r3, r2
	cmp	r0, #0
	bge.n	.L6
	ldr	r3, [pc, #480]
	adds	r0, r0, r3
.L6:
	ldr	r3, [r5, #64]
	mov	r4, r9
	asrs	r6, r0, #16
	subs	r0, r3, r4
	cmp	r0, #0
	bge.n	.L7
	ldr	r2, [pc, #464]
	adds	r0, r0, r2
.L7:
	asrs	r7, r0, #16
	adds	r3, r7, #0
	muls	r3, r7
	adds	r0, r6, #0
	muls	r0, r6
	adds	r0, r0, r3
	ldr	r3, [pc, #452]
	bl	Func_080072f0
	ldr	r4, [pc, #452]
	lsls	r0, r0, #16
	cmp	r0, r4
	bgt.n	.L8
	ldr	r3, [r5, #56]
	mov	r0, fp
	subs	r6, r3, r0
	ldr	r3, [r5, #64]
	mov	r2, r9
	subs	r7, r3, r2
	adds	r0, r6, #0
	adds	r1, r6, #0
	ldr	r3, [pc, #432]
	mov	ip, pc
	bx	r3
	adds	r3, r0, #0
	adds	r1, r7, #0
	adds	r0, r7, #0
	ldr	r4, [pc, #420]
	mov	ip, pc
	bx	r4
	adds	r3, r3, r0
	adds	r0, r3, #0
	ldr	r2, [pc, #400]
	bl	Func_080072ec
	lsls	r0, r0, #8
.L8:
	cmp	r0, #0
	bne.n	.L9
	ldr	r3, [r5, #56]
	ldr	r4, [r5, #64]
	mov	fp, r3
	mov	r9, r4
	b.n	.L10
.L9:
	ldr	r1, [r5, #52]
	ldr	r2, [pc, #388]
	bl	Func_080072ec
	ldr	r3, [pc, #380]
	adds	r4, r0, #0
	mov	r8, r3
	adds	r0, r6, #0
	adds	r1, r4, #0
	mov	ip, pc
	bx	r8
	ldr	r3, [r5, #36]
	adds	r3, r3, r0
	mov	sl, r3
	str	r3, [r5, #36]
	adds	r0, r7, #0
	adds	r1, r4, #0
	mov	ip, pc
	bx	r8
	ldr	r3, [r5, #44]
	adds	r6, r3, r0
	str	r6, [r5, #44]
	mov	r0, sl
	mov	r1, sl
	movs	r0, r0
	mov	ip, pc
	bx	r8
	adds	r3, r0, #0
	adds	r1, r6, #0
	adds	r0, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	r8
	adds	r3, r3, r0
	adds	r0, r3, #0
	ldr	r4, [pc, #308]
	bl	Func_080072f4
	ldr	r1, [r5, #48]
	lsls	r0, r0, #8
	cmp	r0, r1
	ble.n	.L10
	ldr	r2, [pc, #308]
	bl	Func_080072ec
	adds	r4, r0, #0
	adds	r1, r4, #0
	mov	r0, sl
	movs	r0, r0
	mov	ip, pc
	bx	r8
	str	r0, [r5, #36]
	adds	r1, r4, #0
	adds	r0, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	r8
	b.n	.L11
.L5:
	ldr	r6, [r5, #36]
	ldr	r7, [r5, #44]
	adds	r3, r6, #0
	orrs	r3, r7
	cmp	r3, #0
	beq.n	.L10
	ldr	r3, [pc, #260]
	adds	r0, r6, #0
	mov	r8, r3
	adds	r1, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	r8
	adds	r3, r0, #0
	adds	r1, r7, #0
	adds	r0, r7, #0
	movs	r0, r0
	mov	ip, pc
	bx	r8
	adds	r3, r3, r0
	ldr	r2, [pc, #224]
	adds	r0, r3, #0
	bl	Func_080072ec
	lsls	r0, r0, #8
	cmp	r0, #0
	beq.n	.L12
	ldr	r3, [r5, #52]
	subs	r1, r0, r3
	cmp	r1, #0
	bge.n	.L13
	ldr	r4, [sp, #4]
	str	r4, [r5, #36]
	str	r4, [r5, #44]
	b.n	.L10
.L13:
	ldr	r3, [pc, #204]
	bl	Func_080072f0
	adds	r4, r0, #0
	adds	r1, r4, #0
	adds	r0, r6, #0
	mov	ip, pc
	bx	r8
	str	r0, [r5, #36]
	adds	r1, r4, #0
	adds	r0, r7, #0
	movs	r0, r0
	mov	ip, pc
	bx	r8
	b.n	.L11
.L12:
	ldr	r0, [sp, #4]
	str	r0, [r5, #36]
.L11:
	str	r0, [r5, #44]
.L10:
	ldr	r3, [sp, #0]
	ldrb	r2, [r3, #0]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	ldr	r3, [r5, #20]
	ldr	r4, [sp, #8]
	cmp	r4, r3
	ble.n	.L14
	ldr	r2, [r5, #40]
	ldr	r3, [r5, #72]
	subs	r2, r2, r3
	str	r2, [r5, #40]
	adds	r0, r2, #0
	b.n	.L15
.L14:
	ldr	r0, [r5, #40]
	cmp	r0, #0
	bge.n	.L15
	str	r3, [sp, #8]
	ldr	r3, [pc, #124]
	ldr	r1, [r5, #68]
	mov	ip, pc
	bx	r3
	adds	r3, r0, #0
	negs	r0, r3
	adds	r1, r0, #0
	str	r0, [r5, #40]
	cmp	r1, #0
	bge.n	.L16
	adds	r1, r3, #0
.L16:
	ldr	r3, [r5, #72]
	cmp	r1, r3
	bgt.n	.L15
	movs	r3, #0
	str	r3, [r5, #40]
	movs	r0, #0
	b.n	.L15
.L3:
	ldr	r0, [r5, #40]
.L15:
	ldr	r4, [sp, #8]
	adds	r4, r4, r0
	ldr	r3, [r5, #36]
	str	r4, [sp, #8]
	add	fp, r3
	ldr	r0, [sp, #0]
	ldr	r3, [r5, #44]
	add	r9, r3
	ldrb	r3, [r0, #1]
	adds	r1, r5, #0
	adds	r1, #86
	cmp	r3, #0
	beq.n	.L17
	cmp	r3, #17
	beq.n	.L18
	cmp	r3, #17
	bgt.n	.L19
	cmp	r3, #16
	beq.n	.L20
	b.n	.L17
.L19:
	cmp	r3, #18
	beq.n	.L21
	b.n	.L17
.L20:
	ldr	r2, [r5, #56]
	cmp	fp, r2
	beq.n	.L22
	ldr	r3, [r5, #8]
	mov	r4, fp
	b.n	.L23
.L18:
	ldr	r2, [r5, #60]
	ldr	r3, [sp, #8]
	cmp	r3, r2
	beq.n	.L22
	ldr	r3, [r5, #12]
	ldr	r4, [sp, #8]
	b.n	.L23
	movs	r0, r0
	.4byte 0x03001e64
	.4byte 0x0000ffff
	.4byte 0x030001d8
	.4byte 0x00ffffff
	.4byte 0x03000118
	.4byte 0x0300013c
.L21:
	ldr	r2, [r5, #64]
	cmp	r9, r2
	beq.n	.L22
	ldr	r3, [r5, #16]
	mov	r4, r9
.L23:
	subs	r3, r3, r2
	subs	r2, r4, r2
	eors	r3, r2
	cmp	r3, #0
	bge.n	.L17
.L22:
	movs	r0, #1
	str	r0, [sp, #4]
.L17:
	ldr	r2, [sp, #4]
	cmp	r2, #0
	beq.n	.L24
	ldr	r4, [sp, #0]
	ldrb	r3, [r4, #3]
	cmp	r3, #0
	beq.n	.L25
	movs	r3, #0
	str	r3, [r5, #36]
	str	r3, [r5, #44]
	ldr	r0, [r5, #56]
	ldr	r2, [r5, #64]
	ldrb	r3, [r4, #0]
	mov	fp, r0
	mov	r9, r2
	cmp	r3, #0
	bne.n	.L25
	ldr	r4, [r5, #60]
	str	r4, [sp, #8]
	str	r3, [r5, #40]
.L25:
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r5, #56]
	str	r3, [r5, #60]
	str	r3, [r5, #64]
	movs	r3, #0
	strb	r3, [r1, #0]
.L24:
	mov	r0, fp
	str	r0, [r5, #8]
	ldr	r2, [sp, #8]
	mov	r3, r9
	str	r3, [r5, #16]
	str	r2, [r5, #12]
	ldr	r4, [sp, #0]
	ldrb	r2, [r4, #5]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	ldr	r0, [r5, #36]
	ldr	r2, [r5, #44]
	mov	fp, r0
	mov	r9, r2
	cmp	r0, #0
	bne.n	.L26
	cmp	r2, #0
	beq.n	.L1
.L26:
	mov	r0, r9
	mov	r1, fp
	bl	Func_080044d0
	ldrh	r3, [r5, #6]
	subs	r0, r0, r3
	lsls	r0, r0, #16
	movs	r4, #128
	asrs	r0, r0, #16
	lsls	r4, r4, #5
	cmp	r0, r4
	ble.n	.L27
	movs	r0, #128
	lsls	r0, r0, #5
.L27:
	ldr	r2, [pc, #48]
	cmp	r0, r2
	bge.n	.L28
	ldr	r0, [pc, #40]
.L28:
	adds	r3, r3, r0
	strh	r3, [r5, #6]
.L1:
	ldr	r3, [sp, #12]
	ldr	r4, [sp, #0]
	subs	r3, #1
	adds	r4, #112
	str	r3, [sp, #12]
	str	r4, [sp, #0]
	adds	r5, #112
	cmp	r3, #0
	blt.n	.L29
	b.n	.L30
.L29:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0xfffff000
