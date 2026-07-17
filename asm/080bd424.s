@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080bd424
	.thumb_func
Func_080bd424:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	str	r1, [sp, #24]
	adds	r7, r0, #0
	movs	r1, #0
	ldrsh	r0, [r7, r1]
	bl	Func_08077008
	movs	r3, #0
	movs	r2, #1
	str	r3, [sp, #12]
	ldr	r3, [pc, #560]
	mov	fp, r0
	str	r2, [sp, #16]
	str	r2, [sp, #8]
	add	r3, fp
	movs	r0, #1
	ldrb	r3, [r3, #0]
	negs	r0, r0
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L0
	b.n	.L1
.L0:
	ldr	r1, [sp, #24]
	cmp	r1, #0
	beq.n	.L2
	movs	r2, #6
	ldrsh	r3, [r7, r2]
	cmp	r3, #4
	beq.n	.L2
	b.n	.L1
.L2:
	movs	r3, #148
	lsls	r3, r3, #1
	add	r3, fp
	ldrb	r0, [r3, #0]
	bl	Func_08077198
	str	r0, [sp, #20]
	ldr	r1, [sp, #20]
	movs	r3, #144
	lsls	r3, r3, #1
	adds	r0, #54
	adds	r1, #55
	add	r3, fp
	str	r0, [sp, #4]
	str	r1, [sp, #0]
	mov	r8, r3
.L42:
	ldr	r2, [sp, #4]
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #6
	bhi.n	.L3
	ldr	r2, [pc, #480]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080bd4bc
	.4byte 0x080bd4c4
	.4byte 0x080bd4cc
	.4byte 0x080bd4da
	.4byte 0x080bd504
	.4byte 0x080bd530
	.4byte 0x080bd534
	add	r3, sp, #28
	ldr	r0, [pc, #448]
	mov	r9, r3
	b.n	.L4
	add	r1, sp, #28
	ldr	r0, [pc, #444]
	mov	r9, r1
	b.n	.L4
	add	r2, sp, #28
	ldr	r0, [pc, #440]
	mov	r9, r2
.L4:
	bl	Func_080bd3e4
	mov	sl, r0
	b.n	.L3
	mov	r3, r8
	ldr	r2, [r3, #0]
	lsls	r3, r2, #31
	cmp	r3, #0
	bne.n	.L5
	bl	Func_080771a0
	mov	r1, r8
	movs	r3, #7
	ldrb	r2, [r1, #0]
	ands	r0, r3
	movs	r3, #15
	negs	r3, r3
	ands	r3, r2
	lsls	r0, r0, #1
	movs	r2, #1
	orrs	r3, r0
	orrs	r3, r2
	strb	r3, [r1, #0]
	ldr	r2, [r1, #0]
	b.n	.L5
	mov	r3, r8
	ldr	r2, [r3, #0]
.L5:
	lsls	r3, r2, #28
	ldr	r0, [sp, #24]
	lsrs	r3, r3, #29
	mov	sl, r3
	cmp	r0, #0
	beq.n	.L3
	mov	r2, sl
	movs	r3, #7
	adds	r2, #1
	ands	r2, r3
	mov	r3, r8
	ldrb	r1, [r3, #0]
	movs	r3, #15
	negs	r3, r3
	lsls	r2, r2, #1
	ands	r3, r1
	orrs	r3, r2
	mov	r0, r8
	strb	r3, [r0, #0]
	b.n	.L3
	movs	r1, #1
	add	sl, r1
.L3:
	ldr	r2, [sp, #0]
	ldrb	r6, [r2, #0]
	mov	r3, sl
	asrs	r6, r3
	mov	r1, sl
	movs	r3, #1
	ands	r6, r3
	ldr	r2, [sp, #20]
	lsls	r3, r1, #1
	adds	r3, #56
	ldr	r0, [sp, #16]
	ldrh	r3, [r2, r3]
	ands	r6, r0
	mov	r9, r3
	movs	r3, #4
	strh	r3, [r7, #6]
	cmp	r6, #0
	beq.n	.L6
	ldr	r3, [sp, #24]
	cmp	r3, #0
	beq.n	.L6
	mov	r1, fp
	adds	r1, #216
	ldrh	r2, [r1, #0]
	ldr	r3, [pc, #292]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L7
	ldr	r3, [sp, #20]
	adds	r3, #53
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r6, #0
	cmp	r3, #0
	bne.n	.L7
	movs	r3, #2
	strh	r3, [r7, #6]
	ldr	r3, [pc, #268]
	strh	r3, [r7, #8]
	b.n	.L1
.L7:
	cmp	r6, #0
	beq.n	.L8
	ldrh	r0, [r1, #0]
	bl	Func_08077018
	adds	r5, r0, #0
	ldrb	r3, [r5, #12]
	cmp	r3, #1
	bne.n	.L9
	ldrh	r0, [r5, #40]
	bl	Func_08077080
	movs	r3, #2
	ldrh	r5, [r5, #40]
	strh	r3, [r7, #6]
	ldrb	r3, [r0, #1]
	movs	r2, #0
	mov	r9, r5
	strh	r2, [r7, #8]
	cmp	r3, #2
	bgt.n	.L9
	cmp	r3, #1
	bge.n	.L10
.L9:
	movs	r6, #0
.L10:
	cmp	r6, #0
	bne.n	.L6
.L8:
	movs	r0, #0
	str	r0, [sp, #16]
.L6:
	ldr	r1, [sp, #8]
	cmp	r1, #0
	bne.n	.L11
	b.n	.L12
.L11:
	mov	r0, r9
	bl	Func_08077080
	adds	r5, r0, #0
	ldrb	r3, [r5, #3]
	cmp	r3, #47
	beq.n	.L13
	cmp	r3, #47
	bgt.n	.L14
	cmp	r3, #46
	beq.n	.L15
	b.n	.L16
.L14:
	cmp	r3, #49
	beq.n	.L17
	b.n	.L16
.L15:
	movs	r3, #3
	strh	r3, [r7, #6]
	movs	r2, #0
	ldrsh	r0, [r7, r2]
	b.n	.L18
.L13:
	movs	r3, #7
	strh	r3, [r7, #6]
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	b.n	.L18
.L17:
	movs	r3, #99
	strh	r3, [r7, #6]
	movs	r1, #0
	ldrsh	r0, [r7, r1]
.L18:
	bl	Func_080b9a70
	strh	r0, [r7, #10]
.L16:
	ldr	r2, [sp, #24]
	cmp	r2, #0
	bne.n	.L19
	movs	r0, #6
	ldrsh	r3, [r7, r0]
	ldrh	r2, [r7, #6]
	cmp	r3, #3
	beq.n	.L20
	cmp	r3, #7
	beq.n	.L21
	b.n	.L1
.L21:
	b.n	.L20
.L19:
	ldrh	r2, [r7, #6]
.L20:
	movs	r1, #128
	lsls	r3, r2, #16
	lsls	r1, r1, #10
	cmp	r3, r1
	beq.n	.L22
	mov	r0, r9
	bl	Func_080bd3c8
	cmp	r0, #0
	beq.n	.L23
	movs	r3, #1
	mov	r2, r9
	mov	r1, fp
	strh	r3, [r7, #6]
	strh	r2, [r7, #8]
	movs	r0, #58
	ldrsh	r3, [r1, r0]
	ldrb	r2, [r5, #9]
	cmp	r2, r3
	ble.n	.L24
	ldr	r3, [sp, #20]
	adds	r3, #53
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L24
	b.n	.L25
.L24:
	ldr	r3, [pc, #56]
	add	r3, fp
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L26
	ldr	r3, [sp, #20]
	adds	r3, #53
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #2
	bne.n	.L27
	b.n	.L25
.L27:
	b.n	.L26
.L23:
	ldrh	r2, [r7, #6]
	b.n	.L22
	.4byte 0x00000129
	.4byte 0x080bd4a0
	.4byte 0x080c2b80
	.4byte 0x080c2b88
	.4byte 0x080c2b90
	.4byte 0x000001ff
	.4byte 0x000001fd
	.4byte 0x0000013d
.L26:
	ldr	r2, [pc, #36]
.L22:
	lsls	r3, r2, #16
	asrs	r2, r3, #16
	cmp	r2, #99
	bne.n	.L28
	movs	r3, #164
	lsls	r3, r3, #1
	add	r3, fp
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L25
.L28:
	ldr	r3, [sp, #24]
	cmp	r3, #0
	beq.n	.L29
	cmp	r2, #3
	beq.n	.L25
	cmp	r2, #7
	beq.n	.L25
	b.n	.L29
	movs	r0, r0
	.4byte 0x00000001
.L29:
	cmp	r2, #4
	bne.n	.L30
	mov	r0, r9
	mov	r1, r9
	movs	r3, #0
	strh	r0, [r7, #8]
	cmp	r1, #1
	bne.n	.L30
	strh	r3, [r7, #6]
.L30:
	ldrb	r3, [r5, #8]
	strh	r3, [r7, #12]
	ldrb	r3, [r5, #0]
	cmp	r3, #2
	beq.n	.L31
	cmp	r3, #2
	bgt.n	.L32
	cmp	r3, #1
	beq.n	.L33
	b.n	.L34
.L32:
	cmp	r3, #3
	beq.n	.L35
	cmp	r3, #4
	bne.n	.L34
.L31:
	movs	r2, #0
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	bl	Func_080bae40
	movs	r3, #2
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L36
	ldrh	r3, [r7, #0]
	movs	r0, #0
	cmp	r3, #7
	bhi.n	.L37
	movs	r0, #1
.L37:
	bl	Func_080bad7c
.L36:
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L12
	movs	r2, #0
	strh	r0, [r7, #10]
	str	r2, [sp, #8]
	b.n	.L12
.L33:
	adds	r1, r5, #0
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	bl	Func_080bae40
	movs	r1, #2
	negs	r1, r1
	cmp	r0, r1
	bne.n	.L38
	ldrh	r3, [r7, #0]
	movs	r0, #0
	cmp	r3, #7
	bhi.n	.L39
	movs	r0, #1
.L39:
	bl	Func_080bad7c
.L38:
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L12
	b.n	.L40
.L35:
	movs	r1, #0
	ldrsh	r0, [r7, r1]
	bl	Func_080b9a70
	strh	r0, [r7, #10]
	b.n	.L12
.L34:
	movs	r2, #0
	ldrsh	r0, [r7, r2]
	bl	Func_080b9a70
.L40:
	movs	r3, #0
	strh	r0, [r7, #10]
	str	r3, [sp, #8]
.L12:
	ldr	r0, [sp, #24]
	cmp	r0, #0
	bne.n	.L25
	movs	r1, #0
	str	r1, [sp, #8]
.L25:
	ldr	r2, [sp, #8]
	cmp	r2, #0
	beq.n	.L41
	ldr	r3, [sp, #12]
	cmp	r3, #16
	ble.n	.L41
	movs	r3, #3
	movs	r0, #0
	strh	r3, [r7, #6]
	str	r0, [sp, #8]
.L41:
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #8]
	adds	r1, #1
	str	r1, [sp, #12]
	cmp	r2, #0
	beq.n	.L1
	b.n	.L42
.L1:
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
