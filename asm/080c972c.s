@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c972c
	.thumb_func
Func_080c972c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #96]
	adds	r3, r5, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #32
	str	r3, [sp, #28]
	ldr	r3, [pc, #88]
	mov	sl, r1
	add	r3, sl
	str	r0, [r3, #0]
	ldr	r0, [pc, #84]
	bl	Func_080cd594
	ldr	r2, [pc, #80]
	ldr	r3, [pc, #56]
	ldr	r1, [pc, #80]
	strh	r3, [r2, #0]
	ldr	r0, [pc, #80]
	add	r1, sl
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	ldr	r0, [pc, #72]
	mov	r1, sl
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	bl	Func_080c9048
	ldr	r2, [pc, #60]
	ldr	r3, [pc, #24]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #24]
	subs	r2, #8
	strh	r3, [r2, #0]
	movs	r1, #7
	movs	r3, #2
	movs	r2, #7
	movs	r0, #46
	str	r3, [sp, #0]
	b.n	.L0
	.4byte 0x00000100
	.4byte 0x00003f44
	.4byte 0x00003337
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x00002001
	.4byte 0x04000020
	.4byte 0x00000604
	.4byte 0x000000cc
	.4byte 0x00000076
	.4byte 0x04000050
.L0:
	bl	Func_080ed408
	ldr	r2, [r5, #28]
	movs	r3, #3
	str	r2, [sp, #16]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #2
	movs	r0, #47
	bl	Func_080ed408
	ldr	r5, [r5, #32]
	movs	r3, #0
	mov	r8, r3
	movs	r1, #1
	movs	r2, #128
	str	r5, [sp, #20]
	ldr	r3, [pc, #180]
	negs	r1, r1
	lsls	r2, r2, #2
.L1:
	movs	r4, #1
	add	r8, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L1
	movs	r5, #225
	movs	r7, #0
	lsls	r5, r5, #7
	ldr	r6, [pc, #160]
	mov	r8, r7
	add	r5, sl
.L4:
	bl	Func_08004458
	movs	r3, #63
	ands	r0, r3
	ldr	r3, [pc, #152]
	add	r3, sl
	ldr	r2, [r3, #0]
	ldr	r3, [r2, #24]
	lsls	r3, r3, #2
	adds	r3, #2
	ldrb	r3, [r6, r3]
	mov	r1, r8
	muls	r1, r3
	adds	r3, r1, #0
	adds	r3, #16
	negs	r1, r3
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L2
	lsrs	r3, r1, #31
	adds	r3, r1, r3
	asrs	r3, r3, #1
	adds	r3, r0, r3
	adds	r0, r3, #0
	subs	r0, #48
	b.n	.L3
.L2:
	lsrs	r3, r1, #31
	adds	r3, r1, r3
	asrs	r3, r3, #1
	subs	r3, r0, r3
	adds	r0, r3, #0
	adds	r0, #72
.L3:
	lsls	r3, r0, #3
	str	r3, [r5, #0]
	lsls	r3, r1, #3
	str	r3, [r5, #4]
	movs	r2, #1
	movs	r3, #1
	negs	r3, r3
	add	r8, r2
	str	r3, [r5, #24]
	mov	r3, r8
	adds	r5, #28
	cmp	r3, #64
	bne.n	.L4
	ldr	r3, [pc, #68]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L5
	movs	r4, #0
	mov	r8, r4
	ldr	r5, [pc, #32]
	ldr	r1, [pc, #56]
	ldr	r4, [pc, #32]
	ldr	r0, [pc, #36]
.L9:
	mov	r3, r8
	subs	r3, #8
	cmp	r3, #95
	bhi.n	.L6
	mov	r7, r8
	lsrs	r3, r7, #31
	add	r3, r8
	asrs	r3, r3, #1
	subs	r2, r5, r3
	lsls	r2, r2, #8
	subs	r3, r4, r3
	orrs	r2, r3
	strh	r2, [r1, #0]
	b.n	.L7
	.4byte 0x00000034
	.4byte 0x000000b4
	.4byte 0x00000080
	.4byte 0x02010158
	.4byte 0x080ededc
	.4byte 0x00007828
	.4byte 0x02010000
.L6:
	mov	r2, r8
	cmp	r2, #135
	bgt.n	.L8
	strh	r0, [r1, #0]
	b.n	.L7
.L8:
	ldr	r3, [pc, #16]
	strh	r3, [r1, #0]
.L7:
	movs	r3, #1
	add	r8, r3
	mov	r7, r8
	adds	r1, #2
	cmp	r7, #160
	bne.n	.L9
	b.n	.L10
	.4byte 0x00000100
.L5:
	movs	r0, #0
	mov	r8, r0
	ldr	r4, [pc, #44]
	ldr	r0, [pc, #48]
	ldr	r1, [pc, #48]
.L14:
	mov	r3, r8
	subs	r3, #8
	cmp	r3, #95
	bhi.n	.L11
	mov	r2, r8
	lsrs	r3, r2, #31
	add	r3, r8
	asrs	r3, r3, #1
	adds	r2, r3, #0
	adds	r2, #60
	lsls	r2, r2, #8
	adds	r3, #188
	orrs	r2, r3
	strh	r2, [r1, #0]
	b.n	.L12
.L11:
	mov	r3, r8
	cmp	r3, #135
	bgt.n	.L13
	strh	r4, [r1, #0]
	b.n	.L12
	movs	r0, r0
	.4byte 0x000070f0
	.4byte 0x00000100
	.4byte 0x02010000
.L13:
	strh	r0, [r1, #0]
.L12:
	movs	r7, #1
	add	r8, r7
	mov	r2, r8
	adds	r1, #2
	cmp	r2, #160
	bne.n	.L14
.L10:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #780]
	bl	Func_080041d8
	ldr	r3, [pc, #776]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r1, [r3, #24]
	cmp	r1, #0
	bne.n	.L15
	movs	r3, #239
	lsls	r3, r3, #7
	add	r3, sl
	movs	r2, #1
	str	r2, [r3, #0]
	ldr	r3, [pc, #760]
	add	r3, sl
	str	r1, [r3, #0]
	b.n	.L16
.L15:
	cmp	r1, #1
	bne.n	.L17
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #736]
	movs	r3, #50
	b.n	.L18
.L17:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #720]
	movs	r3, #75
.L18:
	add	r2, sl
	str	r3, [r2, #0]
.L16:
	movs	r1, #144
	ldr	r0, [pc, #716]
	lsls	r1, r1, #3
	bl	Func_080041d8
	movs	r3, #0
	str	r3, [sp, #24]
	ldr	r2, [pc, #696]
	mov	r4, sl
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #24]
	ldr	r6, [pc, #700]
	lsls	r3, r3, #2
	adds	r3, #3
	ldrb	r3, [r6, r3]
	cmp	r3, #0
	bne.n	.L19
	b.n	.L20
.L19:
	mov	r7, sl
	adds	r5, r7, r2
	ldr	r2, [r5, #0]
	ldr	r3, [r2, #24]
	lsls	r3, r3, #2
	adds	r3, #3
	ldrb	r3, [r6, r3]
	ldr	r0, [sp, #24]
	subs	r3, #64
	cmp	r0, r3
	bne.n	.L21
	movs	r0, #132
	bl	Func_080b50e8
	ldr	r2, [r5, #0]
.L21:
	ldr	r3, [r2, #24]
	lsls	r3, r3, #2
	ldrb	r3, [r6, r3]
	movs	r1, #0
	mov	r8, r1
	cmp	r3, #0
	bne.n	.L22
	b.n	.L23
.L22:
	movs	r7, #225
	lsls	r7, r7, #7
	add	r7, sl
.L41:
	ldr	r2, [r7, #0]
	cmp	r2, #0
	bge.n	.L24
	adds	r2, #7
.L24:
	ldr	r3, [r7, #4]
	asrs	r6, r2, #3
	cmp	r3, #0
	bge.n	.L25
	adds	r3, #7
.L25:
	asrs	r5, r3, #3
	movs	r4, #1
	ldr	r3, [r7, #24]
	negs	r4, r4
	adds	r2, r3, #0
	cmp	r3, r4
	beq.n	.L26
	b.n	.L27
.L26:
	ldr	r1, [pc, #604]
	movs	r3, #24
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	ldr	r0, [sp, #28]
	adds	r3, r5, #0
	add	r1, sl
	adds	r2, r6, #0
	ldr	r4, [sp, #16]
	bl	Func_080072f4
	ldr	r3, [r7, #4]
	ldr	r0, [pc, #584]
	cmp	r3, r0
	bgt.n	.L28
	ldr	r3, [pc, #556]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L29
	ldr	r3, [r7, #0]
	subs	r3, #32
	b.n	.L30
.L29:
	ldr	r3, [r7, #0]
	adds	r3, #32
.L30:
	str	r3, [r7, #0]
	ldr	r3, [r7, #4]
	adds	r3, #64
	str	r3, [r7, #4]
	b.n	.L31
.L28:
	ldr	r1, [pc, #524]
	movs	r3, #0
	mov	r0, sl
	str	r3, [r7, #24]
	ldr	r3, [r0, r1]
	ldr	r3, [r3, #24]
	ldr	r2, [pc, #524]
	lsls	r3, r3, #2
	adds	r3, #1
	ldrb	r3, [r2, r3]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L32
	adds	r6, #12
	lsls	r5, r5, #16
	movs	r3, #255
	str	r6, [sp, #12]
	mov	fp, r5
	mov	r9, r3
.L35:
	mov	r0, sl
	adds	r5, r0, r1
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	lsls	r3, r3, #2
	adds	r3, #1
	ldrb	r3, [r2, r3]
	mov	r2, r8
	muls	r2, r3
	adds	r2, r2, r4
	lsls	r3, r2, #3
	ldr	r1, [pc, #488]
	subs	r3, r3, r2
	ldr	r2, [sp, #12]
	lsls	r3, r3, #2
	adds	r6, r3, r1
	lsls	r3, r2, #16
	str	r3, [r6, #0]
	mov	r3, fp
	str	r3, [r6, #4]
	str	r4, [sp, #8]
	bl	Func_08004458
	mov	r1, r9
	ands	r0, r1
	subs	r0, #128
	lsls	r0, r0, #9
	str	r0, [r6, #12]
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	ldr	r4, [sp, #8]
	cmp	r3, #2
	bne.n	.L33
	bl	Func_08004458
	ldr	r3, [pc, #444]
	ldr	r2, [pc, #448]
	ands	r3, r0
	adds	r3, r3, r2
	lsls	r3, r3, #10
	str	r3, [r6, #16]
	b.n	.L34
.L33:
	str	r4, [sp, #8]
	bl	Func_08004458
	mov	r3, r9
	ands	r0, r3
	subs	r0, #255
	lsls	r0, r0, #10
	str	r0, [r6, #16]
.L34:
	ldr	r4, [sp, #8]
	str	r4, [sp, #8]
	bl	Func_08004458
	movs	r3, #15
	ands	r3, r0
	ldr	r1, [pc, #376]
	adds	r3, #16
	mov	r0, sl
	str	r3, [r6, #24]
	ldr	r3, [r0, r1]
	ldr	r3, [r3, #24]
	ldr	r2, [pc, #376]
	lsls	r3, r3, #2
	ldr	r4, [sp, #8]
	adds	r3, #1
	ldrb	r3, [r2, r3]
	adds	r4, #1
	cmp	r4, r3
	bne.n	.L35
.L32:
	movs	r3, #3
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L36
	movs	r0, #132
	bl	Func_080f9010
.L36:
	ldr	r2, [pc, #332]
	add	r2, sl
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L31
	adds	r5, r2, #0
	movs	r6, #36
.L37:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	adds	r3, r4, #0
	movs	r2, #5
	str	r4, [sp, #8]
	bl	Func_080d6888
	ldr	r3, [r5, #0]
	ldr	r4, [sp, #8]
	ldr	r3, [r3, #20]
	adds	r4, #1
	adds	r6, #2
	cmp	r4, r3
	bne.n	.L37
	b.n	.L31
.L27:
	cmp	r2, #3
	bhi.n	.L38
	ldr	r1, [pc, #308]
	movs	r3, #24
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	ldr	r0, [sp, #28]
	adds	r3, r5, #0
	add	r1, sl
	adds	r2, r6, #0
	b.n	.L39
.L38:
	cmp	r2, #7
	bgt.n	.L40
	movs	r1, #42
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	ldr	r1, [pc, #284]
	adds	r2, r6, #0
	adds	r3, r5, #0
	subs	r3, #9
	subs	r2, #9
	ldr	r0, [sp, #28]
	add	r1, sl
.L39:
	ldr	r4, [sp, #16]
	bl	Func_080072f4
	ldr	r3, [r7, #24]
.L40:
	cmp	r3, #14
	bgt.n	.L31
	adds	r3, #1
	str	r3, [r7, #24]
.L31:
	ldr	r3, [pc, #216]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #24]
	ldr	r1, [pc, #220]
	lsls	r3, r3, #2
	movs	r0, #1
	ldrb	r3, [r1, r3]
	add	r8, r0
	adds	r7, #28
	cmp	r8, r3
	beq.n	.L23
	b.n	.L41
.L23:
	movs	r2, #0
	ldr	r6, [pc, #232]
	ldr	r5, [pc, #208]
	mov	r8, r2
.L44:
	movs	r3, #1
	ldr	r0, [r5, #24]
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L42
	adds	r4, r0, #1
	cmp	r4, #6
	ble.n	.L43
	movs	r4, #6
.L43:
	lsls	r0, r4, #1
	subs	r3, r0, #2
	ldrh	r1, [r6, r3]
	movs	r7, #2
	ldrsh	r2, [r5, r7]
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	subs	r2, r2, r4
	subs	r3, r3, r4
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	add	r1, sl
	ldr	r0, [sp, #28]
	ldr	r4, [sp, #20]
	bl	Func_080072f4
	movs	r2, #128
	adds	r0, r5, #0
	movs	r1, #60
	lsls	r2, r2, #6
	bl	Func_080e3908
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L42:
	movs	r7, #1
	movs	r0, #128
	add	r8, r7
	lsls	r0, r0, #2
	adds	r5, #28
	cmp	r8, r0
	bne.n	.L44
	bl	Func_080cd52c
	ldr	r2, [pc, #144]
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [sp, #24]
	adds	r1, #1
	str	r1, [sp, #24]
	ldr	r2, [pc, #76]
	mov	r4, sl
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #24]
	ldr	r6, [pc, #80]
	lsls	r3, r3, #2
	adds	r3, #3
	ldrb	r3, [r6, r3]
	cmp	r1, r3
	beq.n	.L20
	b.n	.L19
.L20:
	ldr	r0, [pc, #48]
	bl	Func_08004278
	ldr	r0, [pc, #56]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	bl	Func_080c9048
	add	sp, #32
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
	.4byte 0x080ededc
	.4byte 0x00000604
	.4byte 0x0000027f
	.4byte 0x02010140
	.4byte 0x000001ff
	.4byte 0xfffffe80
	.4byte 0x00000844
	.4byte 0x00000a84
	.4byte 0x080ede5c
	.4byte 0x00007824
