@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080d2d98
	.thumb_func
Func_080d2d98:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #100]
	ldr	r1, [r3, #0]
	sub	sp, #60
	str	r1, [sp, #48]
	subs	r2, r3, #4
	ldr	r2, [r2, #0]
	str	r2, [sp, #44]
	ldr	r7, [pc, #88]
	ldr	r3, [r3, #4]
	str	r3, [sp, #40]
	adds	r3, r2, r7
	str	r0, [r3, #0]
	movs	r0, #0
	bl	Func_080cd594
	ldr	r2, [pc, #76]
	mov	r0, sp
	ldr	r3, [pc, #60]
	adds	r0, #52
	strh	r3, [r2, #0]
	str	r0, [sp, #36]
	ldr	r1, [sp, #36]
	movs	r0, #0
	bl	Func_080cef64
	ldr	r0, [pc, #60]
	bl	Func_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #56]
	movs	r2, #128
	adds	r1, r5, #0
	lsls	r0, r0, #19
	bl	Func_080072f0
	adds	r5, #128
	ldr	r1, [sp, #44]
	adds	r0, r5, #0
	bl	Func_08005340
	ldr	r0, [pc, #36]
	bl	Func_08002f40
	ldr	r3, [pc, #36]
	ldr	r2, [sp, #44]
	adds	r5, r0, #0
	b.n	.L0
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x03001ef0
	.4byte 0x00007828
	.4byte 0x04000052
	.4byte 0x0000006e
	.4byte 0x03001388
	.4byte 0x00000085
	.4byte 0x000006e4
.L0:
	adds	r5, #128
	adds	r1, r2, r3
	adds	r0, r5, #0
	bl	Func_08005340
	ldr	r0, [pc, #888]
	bl	Func_08002f40
	ldr	r1, [sp, #40]
	bl	Func_08005340
	movs	r0, #239
	ldr	r7, [sp, #44]
	lsls	r0, r0, #7
	ldr	r1, [pc, #872]
	adds	r2, r7, r0
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r7, r1
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #860]
	bl	Func_080041d8
	movs	r7, #176
	ldr	r0, [pc, #856]
	lsls	r7, r7, #15
	ldr	r1, [pc, #856]
	movs	r3, #128
	str	r7, [sp, #32]
	str	r0, [sp, #20]
	ldr	r7, [sp, #44]
	ldr	r0, [pc, #848]
	movs	r2, #0
	lsls	r3, r3, #17
	str	r1, [sp, #24]
	str	r2, [sp, #16]
	str	r3, [sp, #28]
	mov	r8, r2
	adds	r3, r7, r0
	subs	r2, #1
.L1:
	movs	r1, #1
	add	r8, r1
	mov	r7, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r7, #64
	bne.n	.L1
	ldr	r1, [sp, #44]
	ldr	r2, [pc, #820]
	movs	r0, #0
	mov	r8, r0
	adds	r5, r1, r2
.L2:
	bl	Func_08004458
	movs	r3, #127
	ands	r3, r0
	str	r3, [r5, #0]
	bl	Func_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #56
	str	r3, [r5, #4]
	bl	Func_08004458
	movs	r3, #15
	ands	r3, r0
	negs	r3, r3
	str	r3, [r5, #24]
	movs	r3, #1
	add	r8, r3
	mov	r7, r8
	adds	r5, #28
	cmp	r7, #16
	bne.n	.L2
	movs	r0, #0
	movs	r1, #1
	movs	r2, #128
	ldr	r3, [pc, #764]
	mov	r8, r0
	negs	r1, r1
	lsls	r2, r2, #3
.L3:
	movs	r7, #1
	add	r8, r7
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L3
	ldr	r0, [sp, #44]
	ldr	r1, [pc, #744]
	adds	r3, r0, r1
	ldr	r0, [r3, #0]
	bl	Func_080d6750
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #190
	movs	r2, #2
	lsls	r1, r1, #1
	movs	r0, #12
	bl	Func_080dbb24
	movs	r2, #0
	mov	r9, r2
.L44:
	ldr	r3, [pc, #716]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L4
	mov	r3, r9
	cmp	r3, #32
	ble.n	.L4
	cmp	r3, #97
	bgt.n	.L4
	movs	r7, #98
	mov	r9, r7
.L4:
	mov	r0, r9
	cmp	r0, #120
	bne.n	.L5
	movs	r0, #134
	bl	Func_080b50e8
.L5:
	mov	r1, r9
	cmp	r1, #15
	bgt.n	.L6
	ldr	r2, [sp, #16]
	adds	r2, #2
	str	r2, [sp, #16]
.L6:
	mov	r3, r9
	cmp	r3, #99
	bgt.n	.L7
	ldr	r0, [sp, #28]
	ldr	r7, [sp, #20]
	adds	r7, r7, r0
	str	r7, [sp, #28]
	ldr	r7, [sp, #20]
	movs	r3, #58
	muls	r3, r7
	ldr	r2, [sp, #32]
	ldr	r1, [sp, #24]
	adds	r1, r1, r2
	str	r1, [sp, #32]
	cmp	r3, #0
	bge.n	.L8
	adds	r3, #63
.L8:
	ldr	r0, [sp, #24]
	asrs	r3, r3, #6
	str	r3, [sp, #20]
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L9
	adds	r3, #63
.L9:
	ldr	r1, [sp, #28]
	ldr	r2, [pc, #616]
	asrs	r3, r3, #6
	str	r3, [sp, #24]
	cmp	r1, r2
	bgt.n	.L7
	ldr	r3, [sp, #20]
	movs	r7, #128
	lsls	r7, r7, #8
	adds	r7, r3, r7
	str	r7, [sp, #20]
.L7:
	movs	r0, #1
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #32]
	bl	Func_080e6d3c
	mov	r0, r9
	cmp	r0, #28
	bne.n	.L10
	movs	r1, #0
	movs	r2, #63
	ldr	r7, [pc, #580]
	mov	r8, r1
	mov	sl, r2
.L12:
	movs	r0, #1
	ldr	r3, [r7, #24]
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L11
	bl	Func_08004458
	mov	r1, sl
	adds	r6, r0, #0
	ands	r6, r1
	bl	Func_08004458
	ldr	r3, [pc, #556]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	Func_08002322
	adds	r3, r6, #0
	muls	r3, r0
	movs	r2, #128
	lsls	r2, r2, #14
	asrs	r3, r3, #3
	adds	r3, r3, r2
	str	r3, [r7, #0]
	adds	r0, r5, #0
	bl	Func_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	movs	r0, #192
	lsls	r0, r0, #15
	asrs	r3, r3, #2
	adds	r3, r3, r0
	str	r3, [r7, #4]
	bl	Func_08004458
	mov	r1, sl
	ands	r0, r1
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r7, #12]
	bl	Func_08004458
	mov	r2, sl
	ands	r0, r2
	negs	r0, r0
	subs	r0, #8
	lsls	r0, r0, #13
	movs	r3, #0
	str	r0, [r7, #16]
	str	r3, [r7, #24]
.L11:
	movs	r3, #1
	movs	r0, #128
	add	r8, r3
	lsls	r0, r0, #1
	adds	r7, #28
	cmp	r8, r0
	bne.n	.L12
.L10:
	mov	r1, r9
	subs	r1, #32
	str	r1, [sp, #12]
	cmp	r1, #47
	bhi.n	.L13
	movs	r2, #0
	movs	r3, #63
	ldr	r7, [pc, #444]
	mov	fp, r2
	mov	r8, r2
	mov	sl, r3
.L15:
	movs	r0, #1
	ldr	r3, [r7, #24]
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L14
	bl	Func_08004458
	mov	r1, sl
	adds	r6, r0, #0
	ands	r6, r1
	bl	Func_08004458
	ldr	r3, [pc, #416]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	Func_08002322
	adds	r3, r6, #0
	muls	r3, r0
	movs	r2, #128
	lsls	r2, r2, #14
	asrs	r3, r3, #3
	adds	r3, r3, r2
	str	r3, [r7, #0]
	adds	r0, r5, #0
	bl	Func_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	movs	r0, #192
	lsls	r0, r0, #15
	asrs	r3, r3, #2
	adds	r3, r3, r0
	str	r3, [r7, #4]
	bl	Func_08004458
	mov	r1, sl
	ands	r0, r1
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r7, #12]
	bl	Func_08004458
	mov	r2, sl
	ands	r0, r2
	movs	r3, #0
	negs	r0, r0
	subs	r0, #8
	str	r3, [r7, #24]
	movs	r3, #1
	lsls	r0, r0, #13
	add	fp, r3
	str	r0, [r7, #16]
	mov	r0, fp
	cmp	r0, #16
	beq.n	.L13
.L14:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #3
	adds	r7, #28
	cmp	r8, r2
	bne.n	.L15
.L13:
	mov	r3, r9
	cmp	r3, #0
	bne.n	.L16
	movs	r0, #164
	bl	Func_080f9010
.L16:
	mov	r7, r9
	cmp	r7, #32
	bne.n	.L17
	movs	r0, #145
	bl	Func_080f9010
.L17:
	mov	r0, r9
	cmp	r0, #80
	bne.n	.L18
	movs	r0, #144
	bl	Func_080f9010
.L18:
	ldr	r1, [sp, #12]
	cmp	r1, #47
	bhi.n	.L19
	ldr	r0, [sp, #44]
	ldr	r1, [pc, #272]
	adds	r0, r0, r1
	movs	r2, #0
	mov	r7, r9
	mov	sl, r0
	ldr	r0, [pc, #264]
	mov	r8, r2
	lsls	r3, r7, #4
	movs	r2, #34
	ldr	r6, [pc, #260]
	mov	fp, r2
	adds	r7, r3, r0
.L20:
	adds	r0, r7, #0
	movs	r1, #104
	bl	Func_080022fc
	ldrb	r3, [r6, #1]
	ldrb	r2, [r6, #0]
	adds	r5, r0, #0
	mov	r1, fp
	movs	r0, #104
	subs	r3, r3, r5
	str	r1, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #52]
	subs	r2, #17
	subs	r3, #104
	ldr	r0, [sp, #48]
	mov	r1, sl
	bl	Func_080072f4
	ldrb	r2, [r6, #0]
	ldrb	r3, [r6, #1]
	mov	r1, fp
	subs	r2, #17
	subs	r3, r3, r5
	str	r1, [sp, #0]
	str	r5, [sp, #4]
	ldr	r4, [sp, #52]
	ldr	r0, [sp, #48]
	mov	r1, sl
	bl	Func_080072f4
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r6, #2
	adds	r7, #25
	cmp	r3, #3
	bne.n	.L20
.L19:
	mov	r7, r9
	cmp	r7, #95
	bgt.n	.L21
	movs	r0, #0
	mov	r8, r0
	movs	r5, #32
	movs	r6, #120
.L23:
	mov	r2, r8
	lsls	r1, r2, #5
	mov	r2, r9
	cmp	r2, #0
	bge.n	.L22
	adds	r2, #3
.L22:
	movs	r3, #31
	asrs	r2, r2, #2
	ands	r2, r3
	ldr	r7, [sp, #16]
	adds	r2, r1, r2
	subs	r2, #32
	ldr	r1, [sp, #44]
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	ldr	r4, [sp, #52]
	ldr	r0, [sp, #48]
	subs	r3, r6, r7
	bl	Func_080072f4
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	cmp	r1, #5
	bne.n	.L23
.L21:
	movs	r2, #0
	ldr	r5, [pc, #96]
	mov	r8, r2
.L38:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bge.n	.L24
	b.n	.L25
.L24:
	mov	r0, r8
	movs	r1, #3
	bl	Func_080022fc
	ldr	r3, [r5, #16]
	adds	r4, r0, #2
	cmp	r3, #0
	ble.n	.L26
	adds	r4, #2
.L26:
	mov	r7, r9
	cmp	r7, #68
	ble.n	.L27
	cmp	r4, #5
	bgt.n	.L27
	movs	r4, #6
.L27:
	mov	r0, r9
	cmp	r0, #70
	ble.n	.L28
	cmp	r4, #6
	bgt.n	.L28
	movs	r4, #7
	b.n	.L28
	.4byte 0x00000073
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0xfff00000
	.4byte 0xfffc0000
	.4byte 0x00007098
	.4byte 0x00007320
	.4byte 0x02010018
	.4byte 0x00007828
	.4byte 0x03001b04
	.4byte 0x0077ffff
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0x000006e4
	.4byte 0xffffff00
	.4byte 0x080ee1ac
.L28:
	mov	r1, r9
	cmp	r1, #72
	ble.n	.L29
	cmp	r4, #7
	bgt.n	.L29
	movs	r4, #8
.L29:
	mov	r2, r9
	cmp	r2, #74
	ble.n	.L30
	cmp	r4, #8
	bgt.n	.L30
	movs	r4, #9
.L30:
	mov	r7, r9
	cmp	r7, #76
	ble.n	.L31
	movs	r4, #10
.L31:
	movs	r6, #4
	cmp	r3, #0
	bgt.n	.L32
	movs	r6, #0
.L32:
	lsls	r0, r4, #1
	ldr	r2, [pc, #396]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #40]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #4]
	str	r4, [sp, #0]
	ldr	r0, [sp, #36]
	subs	r3, r3, r4
	ldr	r4, [r6, r0]
	ldr	r0, [sp, #48]
	bl	Func_080072f4
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r1, [r5, #16]
	ldr	r3, [r5, #4]
	mov	r2, r9
	adds	r3, r3, r1
	str	r3, [r5, #4]
	cmp	r2, #80
	ble.n	.L33
	ldr	r7, [pc, #336]
	adds	r3, r1, r7
	b.n	.L34
.L33:
	movs	r2, #3
	mov	r0, r8
	ldr	r3, [pc, #328]
	ands	r2, r0
	lsls	r2, r2, #2
	ldr	r3, [r3, r2]
	adds	r3, r1, r3
.L34:
	str	r3, [r5, #16]
	ldr	r2, [r5, #12]
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r3, r3, #1
	cmp	r3, #0
	bge.n	.L35
	adds	r3, #63
.L35:
	ldr	r2, [r5, #16]
	asrs	r3, r3, #6
	str	r3, [r5, #12]
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r2, r3, #1
	cmp	r2, #0
	bge.n	.L36
	adds	r2, #63
.L36:
	ldr	r3, [r5, #24]
	asrs	r2, r2, #6
	adds	r3, #1
	str	r2, [r5, #16]
	str	r3, [r5, #24]
	cmp	r2, #0
	ble.n	.L25
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	cmp	r3, #104
	ble.n	.L25
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L25:
	movs	r2, #1
	movs	r3, #128
	add	r8, r2
	lsls	r3, r3, #3
	adds	r5, #28
	cmp	r8, r3
	beq.n	.L37
	b.n	.L38
.L37:
	mov	r7, r9
	cmp	r7, #79
	bgt.n	.L39
	ldr	r3, [pc, #240]
	ldr	r1, [sp, #44]
	adds	r2, r1, r3
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	movs	r0, #0
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L39
	adds	r7, r2, #0
	movs	r4, #36
.L42:
	mov	r0, r9
	cmp	r0, #29
	ble.n	.L40
	movs	r1, #12
	str	r4, [sp, #8]
	bl	Func_080022fc
	adds	r6, r0, #0
	ldr	r4, [sp, #8]
	cmp	r6, #0
	bne.n	.L41
	ldr	r3, [r7, #0]
	ldrsh	r0, [r3, r4]
	bl	Func_080b5098
	ldr	r3, [r7, #0]
	ldr	r4, [sp, #8]
	ldr	r5, [r0, #0]
	ldrsh	r0, [r3, r4]
	movs	r3, #1
	negs	r3, r3
	movs	r1, #7
	movs	r2, #5
	str	r6, [sp, #0]
	bl	Func_080d6888
	movs	r3, #144
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	ldr	r3, [pc, #164]
	ldr	r4, [sp, #8]
	str	r3, [r5, #72]
.L41:
	cmp	r6, #6
	bne.n	.L40
	ldr	r3, [r7, #0]
	ldrsh	r0, [r3, r4]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #5
	subs	r3, #1
	str	r4, [sp, #8]
	bl	Func_080d6888
	ldr	r4, [sp, #8]
.L40:
	ldr	r3, [r7, #0]
	movs	r2, #1
	ldr	r3, [r3, #20]
	add	r8, r2
	adds	r4, #2
	cmp	r8, r3
	bne.n	.L42
.L39:
	ldr	r3, [sp, #44]
	ldr	r7, [pc, #120]
	adds	r2, r3, r7
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	Func_080030f8
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	cmp	r1, #124
	beq.n	.L43
	b.n	.L44
.L43:
	ldr	r0, [pc, #96]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	ldr	r2, [sp, #32]
	movs	r0, #1
	ldr	r1, [sp, #28]
	bl	Func_080e6eac
	ldr	r3, [sp, #44]
	movs	r2, #0
	subs	r7, #76
	mov	r8, r2
	adds	r5, r3, r7
.L45:
	ldmia	r5!, {r0}
	bl	Func_08009038
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	cmp	r1, #12
	bne.n	.L45
	bl	Func_080cdbc0
	add	sp, #60
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x080ede48
	.4byte 0xffff8000
	.4byte 0x080ee1b4
	.4byte 0x00007828
	.4byte 0x0000ab85
	.4byte 0x00007824
	.4byte 0x080cd261
