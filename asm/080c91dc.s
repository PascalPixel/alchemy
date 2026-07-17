@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c91dc
	.thumb_func
Func_080c91dc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #100]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #36
	str	r3, [sp, #24]
	ldr	r3, [pc, #92]
	mov	sl, r1
	ldr	r2, [r2, #8]
	add	r3, sl
	str	r2, [sp, #16]
	str	r0, [r3, #0]
	ldr	r0, [pc, #84]
	bl	Func_080cd594
	ldr	r2, [pc, #80]
	ldr	r3, [pc, #56]
	ldr	r0, [pc, #80]
	strh	r3, [r2, #0]
	mov	r1, sl
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #68]
	ldr	r1, [sp, #16]
	bl	Func_080e0524
	bl	Func_080c9048
	ldr	r2, [pc, #60]
	ldr	r3, [pc, #24]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #24]
	subs	r2, #8
	strh	r3, [r2, #0]
	movs	r6, #225
	movs	r3, #128
	movs	r2, #0
	negs	r3, r3
	movs	r7, #16
	lsls	r6, r6, #7
	b.n	.L0
	.4byte 0x00000100
	.4byte 0x00003f44
	.4byte 0x00003337
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x00002001
	.4byte 0x04000020
	.4byte 0x000000b3
	.4byte 0x000000ba
	.4byte 0x04000050
.L0:
	mov	r8, r2
	mov	r9, r3
	negs	r7, r7
	add	r6, sl
.L3:
	bl	Func_08004458
	adds	r5, r0, #0
	bl	Func_08004458
	movs	r2, #63
	movs	r3, #7
	ands	r3, r0
	ands	r2, r5
	adds	r2, r2, r3
	ldr	r3, [pc, #248]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	adds	r2, #24
	cmp	r3, #1
	bne.n	.L1
	adds	r3, r2, r7
	adds	r2, r3, #0
	adds	r2, #24
	b.n	.L2
.L1:
	subs	r3, r2, r7
	adds	r2, r3, #0
	adds	r2, #80
.L2:
	mov	r1, r9
	lsls	r3, r2, #3
	str	r1, [r6, #4]
	movs	r2, #64
	movs	r1, #1
	str	r3, [r6, #0]
	negs	r2, r2
	movs	r3, #1
	add	r8, r1
	negs	r3, r3
	add	r9, r2
	mov	r2, r8
	str	r3, [r6, #24]
	subs	r7, #8
	adds	r6, #28
	cmp	r2, #32
	bne.n	.L3
	adds	r2, r3, #0
	ldr	r3, [pc, #188]
	movs	r7, #0
	mov	r8, r7
	add	r3, sl
.L4:
	movs	r1, #1
	add	r8, r1
	mov	r7, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r7, #32
	bne.n	.L4
	ldr	r3, [pc, #164]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L5
	movs	r3, #2
	movs	r1, #7
	movs	r2, #7
	movs	r0, #46
	str	r3, [sp, #0]
	bl	Func_080ed408
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #2
	bl	Func_080ed408
	b.n	.L6
.L5:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #6
	movs	r0, #46
	bl	Func_080ed408
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #6
	bl	Func_080ed408
.L6:
	ldr	r3, [pc, #96]
	adds	r2, r3, #0
	adds	r2, #184
	ldr	r2, [r2, #0]
	str	r2, [sp, #28]
	adds	r3, #188
	ldr	r3, [r3, #0]
	add	r1, sp, #28
	str	r3, [r1, #4]
	ldr	r3, [pc, #68]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	mov	fp, r1
	cmp	r3, #0
	bne.n	.L7
	movs	r2, #0
	mov	r8, r2
	movs	r1, #224
	ldr	r6, [pc, #36]
	ldr	r2, [pc, #60]
	ldr	r5, [pc, #36]
	ldr	r4, [pc, #40]
	ldr	r0, [pc, #56]
	lsls	r1, r1, #7
.L11:
	mov	r3, r8
	subs	r3, #8
	cmp	r3, #95
	bhi.n	.L8
	mov	r7, r8
	subs	r3, r6, r7
	orrs	r3, r1
	strh	r3, [r2, #0]
	b.n	.L9
.L8:
	mov	r3, r8
	cmp	r3, #135
	bgt.n	.L10
	strh	r5, [r2, #0]
	b.n	.L9
	.4byte 0x000000f0
	.4byte 0x00000888
	.4byte 0x00000100
	.4byte 0x00007828
	.4byte 0x00007418
	.4byte 0x03001e50
	.4byte 0x02010000
	.4byte 0xffffff00
.L10:
	strh	r4, [r2, #0]
.L9:
	movs	r7, #1
	add	r8, r7
	mov	r3, r8
	adds	r2, #2
	adds	r1, r1, r0
	cmp	r3, #160
	bne.n	.L11
	b.n	.L12
.L7:
	movs	r7, #0
	movs	r1, #192
	movs	r0, #128
	ldr	r5, [pc, #36]
	ldr	r4, [pc, #40]
	ldr	r2, [pc, #40]
	mov	r8, r7
	lsls	r1, r1, #5
	lsls	r0, r0, #1
.L16:
	mov	r3, r8
	subs	r3, #8
	cmp	r3, #87
	bhi.n	.L13
	adds	r3, #160
	orrs	r3, r1
	strh	r3, [r2, #0]
	b.n	.L14
.L13:
	mov	r3, r8
	cmp	r3, #135
	bgt.n	.L15
	strh	r5, [r2, #0]
	b.n	.L14
	movs	r0, r0
	.4byte 0x000078f8
	.4byte 0x00000100
	.4byte 0x02010000
.L15:
	strh	r4, [r2, #0]
.L14:
	movs	r7, #1
	add	r8, r7
	mov	r3, r8
	adds	r2, #2
	adds	r1, r1, r0
	cmp	r3, #160
	bne.n	.L16
.L12:
	movs	r1, #144
	ldr	r0, [pc, #780]
	lsls	r1, r1, #3
	bl	Func_080041d8
	movs	r2, #239
	lsls	r2, r2, #7
	movs	r3, #2
	add	r2, sl
	str	r3, [r2, #0]
	ldr	r3, [pc, #764]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #24]
	cmp	r3, #1
	bne.n	.L17
	ldr	r2, [pc, #756]
	movs	r3, #75
	b.n	.L18
.L17:
	ldr	r2, [pc, #752]
	movs	r3, #50
.L18:
	add	r2, sl
	str	r3, [r2, #0]
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #744]
	bl	Func_080041d8
	movs	r7, #0
	str	r7, [sp, #20]
	ldr	r2, [pc, #724]
	mov	r1, sl
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #24]
	ldr	r6, [pc, #728]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r6, r3]
	cmp	r3, #0
	bne.n	.L19
	b.n	.L20
.L19:
	mov	r3, sl
	adds	r5, r3, r2
	ldr	r2, [r5, #0]
	ldr	r3, [r2, #24]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r6, r3]
	ldr	r7, [sp, #20]
	subs	r3, #16
	cmp	r7, r3
	bne.n	.L21
	movs	r0, #132
	bl	Func_080b50e8
	ldr	r2, [r5, #0]
.L21:
	ldr	r3, [r2, #24]
	lsls	r3, r3, #1
	ldrb	r3, [r6, r3]
	movs	r1, #0
	mov	r8, r1
	cmp	r3, #0
	bne.n	.L22
	b.n	.L23
.L22:
	ldr	r2, [pc, #656]
	movs	r7, #225
	add	r2, sl
	lsls	r7, r7, #7
	mov	r9, r2
	add	r7, sl
.L44:
	movs	r3, #1
	ldr	r0, [r7, #24]
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L24
	ldr	r2, [r7, #0]
	cmp	r2, #0
	bge.n	.L25
	adds	r2, #7
.L25:
	ldr	r3, [r7, #4]
	asrs	r2, r2, #3
	cmp	r3, #0
	bge.n	.L26
	adds	r3, #7
.L26:
	mov	r1, r9
	asrs	r5, r3, #3
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #24]
	movs	r1, #4
	cmp	r3, #2
	beq.n	.L27
	movs	r1, #0
.L27:
	movs	r3, #32
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	mov	r3, fp
	ldr	r4, [r1, r3]
	ldr	r0, [sp, #24]
	mov	r1, sl
	adds	r3, r5, #0
	bl	Func_080072f4
	ldr	r3, [r7, #4]
	ldr	r1, [pc, #596]
	cmp	r3, r1
	bgt.n	.L28
	mov	r2, r9
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L29
	ldr	r3, [r7, #0]
	subs	r3, #64
	b.n	.L30
.L29:
	ldr	r3, [r7, #0]
	adds	r3, #64
.L30:
	str	r3, [r7, #0]
	ldr	r3, [r7, #4]
	adds	r3, #64
	str	r3, [r7, #4]
	b.n	.L31
.L28:
	movs	r3, #3
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L32
	movs	r0, #115
	bl	Func_080f9010
.L32:
	ldr	r2, [pc, #548]
	movs	r3, #2
	add	r2, sl
	str	r3, [r2, #0]
	movs	r3, #0
	str	r3, [r7, #24]
	ldr	r3, [pc, #516]
	mov	r2, sl
	ldr	r3, [r2, r3]
	ldr	r3, [r3, #20]
	movs	r6, #0
	cmp	r3, #0
	beq.n	.L31
	ldr	r5, [pc, #500]
	movs	r4, #36
	add	r5, sl
.L33:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r4]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #9
	adds	r3, r6, #0
	movs	r2, #5
	str	r4, [sp, #8]
	bl	Func_080d6888
	ldr	r3, [r5, #0]
	ldr	r4, [sp, #8]
	ldr	r3, [r3, #20]
	adds	r6, #1
	adds	r4, #2
	cmp	r6, r3
	bne.n	.L33
.L31:
	movs	r2, #1
	ldr	r0, [r7, #24]
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L34
.L24:
	ldr	r2, [r7, #0]
	cmp	r2, #0
	bge.n	.L35
	adds	r2, #7
.L35:
	ldr	r3, [r7, #4]
	asrs	r2, r2, #3
	cmp	r3, #0
	bge.n	.L36
	adds	r3, #7
.L36:
	asrs	r6, r3, #3
	subs	r3, r0, #1
	cmp	r3, #13
	bhi.n	.L37
	mov	r1, r9
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #24]
	movs	r5, #4
	cmp	r3, #2
	beq.n	.L38
	movs	r5, #0
.L38:
	movs	r1, #3
	str	r2, [sp, #12]
	bl	Func_080022ec
	adds	r1, r0, #0
	lsls	r1, r1, #10
	movs	r3, #128
	lsls	r3, r3, #3
	add	r1, sl
	adds	r1, r1, r3
	movs	r3, #32
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	add	r5, fp
	ldr	r4, [r5, #0]
	ldr	r0, [sp, #24]
	ldr	r2, [sp, #12]
	adds	r3, r6, #0
	bl	Func_080072f4
	ldr	r0, [r7, #24]
.L37:
	adds	r3, r0, #0
	subs	r3, #9
	cmp	r3, #2
	bhi.n	.L39
	movs	r5, #232
	lsls	r5, r5, #7
	movs	r6, #0
	add	r5, sl
.L43:
	movs	r1, #1
	ldr	r3, [r5, #24]
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L40
	movs	r3, #18
	str	r3, [r5, #24]
	bl	Func_08004458
	movs	r3, #31
	ands	r0, r3
	ldr	r3, [r7, #0]
	cmp	r3, #0
	bge.n	.L41
	adds	r3, #7
.L41:
	asrs	r3, r3, #3
	adds	r3, r0, r3
	lsls	r3, r3, #3
	adds	r3, #8
	str	r3, [r5, #0]
	bl	Func_08004458
	movs	r3, #15
	ands	r0, r3
	ldr	r3, [r7, #4]
	cmp	r3, #0
	bge.n	.L42
	adds	r3, #7
.L42:
	asrs	r3, r3, #3
	adds	r3, r0, r3
	subs	r3, #15
	lsls	r3, r3, #3
	str	r3, [r5, #4]
	ldr	r0, [r7, #24]
	b.n	.L39
.L40:
	adds	r6, #1
	adds	r5, #28
	cmp	r6, #32
	bne.n	.L43
.L39:
	cmp	r0, #14
	bgt.n	.L34
	adds	r3, r0, #1
	str	r3, [r7, #24]
.L34:
	mov	r1, r9
	ldr	r3, [r1, #0]
	movs	r2, #1
	ldr	r3, [r3, #24]
	add	r8, r2
	ldr	r2, [pc, #268]
	lsls	r3, r3, #1
	ldrb	r3, [r2, r3]
	adds	r7, #28
	cmp	r8, r3
	beq.n	.L23
	b.n	.L44
.L23:
	movs	r5, #232
	movs	r2, #0
	lsls	r5, r5, #7
	mov	r8, r2
	add	r5, sl
.L50:
	movs	r3, #1
	ldr	r2, [r5, #24]
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L45
	cmp	r2, #17
	bgt.n	.L46
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	ldr	r2, [r5, #0]
	asrs	r0, r3, #1
	cmp	r2, #0
	bge.n	.L47
	adds	r2, #7
.L47:
	ldr	r6, [pc, #228]
	ldrb	r3, [r6, r0]
	asrs	r2, r2, #3
	lsrs	r1, r3, #1
	ldr	r3, [r5, #4]
	subs	r2, r2, r1
	mov	ip, r2
	cmp	r3, #0
	bge.n	.L48
	adds	r3, #7
.L48:
	asrs	r3, r3, #3
	subs	r7, r3, r1
	ldr	r3, [pc, #180]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #24]
	movs	r4, #4
	cmp	r3, #2
	beq.n	.L49
	movs	r4, #0
.L49:
	ldr	r2, [pc, #192]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldrb	r3, [r6, r0]
	ldr	r2, [sp, #16]
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	mov	r3, fp
	adds	r1, r2, r1
	ldr	r4, [r4, r3]
	mov	r2, ip
	ldr	r0, [sp, #24]
	adds	r3, r7, #0
	bl	Func_080072f4
	ldr	r2, [r5, #24]
.L46:
	movs	r7, #1
	negs	r7, r7
	cmp	r2, r7
	ble.n	.L45
	subs	r3, r2, #1
	str	r3, [r5, #24]
.L45:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r5, #28
	cmp	r2, #32
	bne.n	.L50
	bl	Func_080cd52c
	movs	r1, #4
	movs	r0, #4
	bl	Func_080e155c
	ldr	r2, [pc, #128]
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [sp, #20]
	ldr	r2, [pc, #80]
	adds	r3, #1
	str	r3, [sp, #20]
	mov	r7, sl
	ldr	r3, [r7, r2]
	ldr	r3, [r3, #24]
	ldr	r6, [pc, #80]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r6, r3]
	ldr	r1, [sp, #20]
	cmp	r1, r3
	beq.n	.L20
	b.n	.L19
.L20:
	ldr	r0, [pc, #60]
	bl	Func_08004278
	ldr	r0, [pc, #44]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	bl	Func_080c9048
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080c91a5
	.4byte 0x00007828
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x080eded6
	.4byte 0x0000027f
	.4byte 0x000077a8
	.4byte 0x080ede96
	.4byte 0x080ede84
	.4byte 0x00007824
