@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b63c8
	.thumb_func
Func_080b63c8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #68
	str	r0, [sp, #12]
	movs	r1, #76
	movs	r0, #12
	bl	Func_080048f4
	ldr	r1, [pc, #556]
	mov	sl, r0
	movs	r0, #9
	bl	Func_080048f4
	movs	r5, #249
	lsls	r5, r5, #3
	adds	r1, r5, #0
	mov	r8, r0
	movs	r0, #54
	bl	Func_080048f4
	movs	r1, #32
	adds	r6, r0, #0
	movs	r0, #44
	bl	Func_080048f4
	movs	r1, #160
	str	r0, [sp, #8]
	lsls	r1, r1, #2
	movs	r0, #11
	bl	Func_080048f4
	movs	r1, #12
	add	r1, sl
	ldr	r3, [pc, #508]
	mov	r9, r1
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	Func_080072f0
	bl	Func_080040e8
	movs	r3, #128
	ldr	r2, [sp, #8]
	movs	r7, #0
	lsls	r3, r3, #6
	str	r7, [r2, #4]
	str	r3, [r2, #0]
	ldr	r3, [sp, #8]
	movs	r2, #1
	str	r2, [r3, #20]
	str	r7, [r3, #24]
	str	r7, [r3, #28]
	movs	r3, #128
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	ldr	r0, [pc, #468]
	bl	Func_080770c8
	ldr	r0, [pc, #468]
	bl	Func_080770c8
	bl	Func_080049ac
	add	r5, sp, #16
	str	r7, [r5, #0]
	ldr	r3, [pc, #456]
	adds	r0, r5, #0
	mov	r1, sl
	ldr	r2, [pc, #456]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	str	r7, [r5, #0]
	adds	r0, r5, #0
	mov	r1, r8
	ldr	r2, [pc, #448]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #1
	negs	r3, r3
	str	r3, [r1, #84]
	ldr	r2, [sp, #12]
	movs	r0, #37
	str	r2, [r1, #0]
	movs	r1, #12
	bl	Func_080048f4
	str	r7, [r5, #0]
	adds	r1, r0, #0
	ldr	r3, [pc, #412]
	adds	r0, r5, #0
	ldr	r2, [pc, #420]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	Func_0808a4a0
	movs	r3, #201
	lsls	r3, r3, #3
	add	r3, r8
	movs	r1, #224
	strh	r0, [r3, #0]
	lsls	r1, r1, #4
	movs	r0, #4
	bl	Func_080048f4
	movs	r1, #192
	lsls	r1, r1, #3
	movs	r0, #3
	bl	Func_080048f4
	movs	r0, #4
	bl	Func_08009078
	movs	r0, #183
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L0
	movs	r0, #1
	bl	Func_08015008
	b.n	.L1
.L0:
	movs	r0, #0
	bl	Func_08015008
.L1:
	movs	r2, #128
	mov	r3, r9
	movs	r5, #0
	lsls	r2, r2, #15
	str	r2, [r3, #4]
	str	r5, [r3, #0]
	str	r5, [r3, #8]
	movs	r3, #180
	mov	r1, sl
	lsls	r3, r3, #16
	str	r3, [r1, #4]
	movs	r3, #160
	str	r2, [r1, #8]
	lsls	r3, r3, #6
	mov	r2, sl
	str	r5, [r1, #0]
	strh	r3, [r2, #54]
	movs	r3, #160
	lsls	r3, r3, #7
	strh	r3, [r1, #52]
	movs	r3, #128
	lsls	r3, r3, #17
	str	r3, [r1, #32]
	mov	r2, r8
	ldr	r0, [r2, #0]
	bl	Func_080c1ffc
	adds	r6, r0, #0
	movs	r0, #182
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L2
	mov	r3, r8
	adds	r3, #68
	str	r3, [sp, #4]
	ldr	r1, [sp, #4]
	movs	r3, #1
	strb	r3, [r1, #0]
	ldr	r3, [pc, #272]
	ldr	r2, [pc, #276]
	adds	r3, r3, r2
	movs	r2, #4
	strb	r2, [r3, #0]
	b.n	.L3
.L2:
	mov	r3, r8
	adds	r3, #68
	str	r3, [sp, #4]
.L3:
	ldr	r1, [sp, #4]
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L4
	ldr	r3, [pc, #256]
	str	r5, [r3, #0]
	movs	r5, #0
	ldr	r2, [pc, #252]
	mov	r6, r8
	movs	r3, #1
	mov	sl, r2
	adds	r6, #82
	movs	r7, #3
	mov	r9, r3
.L6:
	mov	r1, sl
	ldrh	r2, [r1, #0]
	adds	r3, r7, #0
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L5
	movs	r0, #1
	adds	r5, #1
	bl	Func_080030f8
	cmp	r5, #24
	ble.n	.L6
	mov	r2, r9
	strb	r2, [r6, #0]
.L5:
	ldr	r3, [pc, #216]
	ldr	r3, [r3, #0]
	mov	r2, r8
	lsls	r3, r3, #26
	lsrs	r3, r3, #30
	adds	r2, #80
	strb	r3, [r2, #0]
	ldr	r3, [pc, #204]
	ldr	r4, [pc, #208]
	ldr	r2, [r3, #0]
	ldr	r1, [pc, #208]
	movs	r0, #0
.L7:
	ldrb	r3, [r1, #0]
	adds	r0, #1
	strb	r3, [r2, #0]
	adds	r1, #1
	adds	r2, #1
	cmp	r0, r4
	bls.n	.L7
	movs	r0, #252
	lsls	r0, r0, #2
	bl	Func_080770e0
	adds	r6, r0, #0
	bl	Func_080b6378
	mov	r2, r8
	adds	r2, #66
	movs	r3, #0
	strb	r3, [r2, #0]
.L4:
	ldr	r1, [pc, #172]
	ldr	r0, [pc, #172]
	bl	Func_080041d8
	ldr	r3, [pc, #132]
	movs	r1, #247
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	cmp	r0, #0
	beq.n	.L8
	bl	Func_080f9010
	movs	r0, #182
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L9
	movs	r0, #55
	bl	Func_080f9010
	movs	r0, #4
	bl	Func_080037d4
	b.n	.L9
.L8:
	movs	r0, #51
	bl	Func_080f9010
	movs	r0, #76
	bl	Func_080f9010
.L9:
	bl	Func_080b5a0c
	bl	Func_080b75dc
	bl	Func_080b5c08
	bl	Func_080b5d3c
	movs	r0, #0
	bl	Func_08077000
	ldr	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L10
	movs	r3, #65
	add	r3, r8
	mov	r9, r3
	mov	r1, r9
	movs	r3, #3
	strb	r3, [r1, #0]
	b.n	.L11
	movs	r0, r0
	.4byte 0x0000082c
	.4byte 0x03000164
	.4byte 0x00000103
	.4byte 0x00000169
	.4byte 0x040000d4
	.4byte 0x85000013
	.4byte 0x8500020b
	.4byte 0x85000003
	.4byte 0x02000240
	.4byte 0x0000022b
	.4byte 0x020023a8
	.4byte 0x03001f64
	.4byte 0x04000128
	.4byte 0x03001f28
	.4byte 0x000007c7
	.4byte 0x02018000
	.4byte 0x00000c7f
	.4byte 0x080b5865
.L10:
	movs	r2, #65
	add	r2, r8
	movs	r3, #1
	strb	r3, [r2, #0]
	mov	r9, r2
.L11:
	movs	r0, #9
	bl	Func_08015128
	bl	Func_080b7f9c
	bl	Func_080b6c90
	bl	Func_080c08a8
	movs	r3, #201
	lsls	r3, r3, #3
	add	r3, r8
	ldrh	r1, [r3, #0]
	movs	r0, #1
	movs	r2, #0
	bl	Func_080c08ec
	movs	r3, #128
	lsls	r3, r3, #10
	movs	r0, #160
	movs	r1, #160
	str	r3, [sp, #0]
	lsls	r0, r0, #16
	lsls	r1, r1, #15
	movs	r2, #0
	movs	r3, #0
	bl	Func_080c0a24
	movs	r1, #0
	movs	r2, #0
	movs	r3, #190
	movs	r0, #0
	bl	Func_080c0cec
	movs	r0, #1
	bl	Func_080b5b14
	ldr	r5, [pc, #52]
	ldr	r3, [pc, #56]
	strh	r5, [r3, #0]
	bl	Func_080c24b0
	movs	r0, #128
	bl	Func_080040b4
	mov	r3, r8
	movs	r1, #69
	str	r0, [r3, #84]
	add	r1, r8
	movs	r0, #183
	strb	r5, [r1, #0]
	lsls	r0, r0, #1
	mov	fp, r1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L12
	ldr	r3, [pc, #20]
	ldr	r1, [pc, #24]
	adds	r3, r3, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L13
	b.n	.L14
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x04000050
	.4byte 0x02000240
	.4byte 0x0000022b
.L14:
	bl	Func_080771a0
	movs	r3, #15
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L15
.L12:
	movs	r3, #1
	mov	r2, fp
	strb	r3, [r2, #0]
	b.n	.L13
.L15:
	bl	Func_080771a0
	movs	r3, #31
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L13
	movs	r3, #2
	mov	r1, fp
	strb	r3, [r1, #0]
.L13:
	adds	r0, r6, #0
	ldr	r1, [sp, #12]
	bl	Func_080c02a4
	ldr	r3, [sp, #8]
	movs	r2, #0
	str	r2, [r3, #20]
	ldr	r3, [pc, #620]
	movs	r1, #200
	strb	r2, [r3, #0]
	ldr	r0, [pc, #620]
	lsls	r1, r1, #4
	bl	Func_080041d8
.L36:
	bl	Func_080b9b2c
	bl	Func_080b5d3c
	movs	r0, #0
	bl	Func_08077000
	ldr	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L16
	movs	r3, #3
	mov	r1, r9
	strb	r3, [r1, #0]
	b.n	.L17
.L16:
	movs	r3, #1
	mov	r2, r9
	strb	r3, [r2, #0]
.L17:
	ldr	r1, [sp, #8]
	movs	r3, #160
	lsls	r3, r3, #6
	str	r3, [r1, #0]
	movs	r3, #60
	str	r3, [r1, #4]
	mov	r2, r9
	movs	r5, #187
	ldrb	r0, [r2, #0]
	lsls	r5, r5, #2
	bl	Func_08015130
	add	r5, r8
	movs	r1, #160
	ldr	r3, [pc, #552]
	lsls	r1, r1, #1
	adds	r0, r5, #0
	bl	Func_080072f0
	mov	r3, r8
	ldr	r0, [r3, #84]
	bl	Func_08003f3c
	movs	r0, #181
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L18
	bl	Func_0800488c
	bl	Func_080048a0
	adds	r0, r5, #0
	bl	Func_080b9934
	adds	r5, r0, #0
	bl	Func_0800488c
	bl	Func_080048a0
	b.n	.L19
.L18:
	adds	r0, r5, #0
	bl	Func_080b8574
	adds	r5, r0, #0
.L19:
	movs	r0, #128
	bl	Func_080040b4
	mov	r1, r8
	str	r0, [r1, #84]
	mov	r2, r9
	ldrb	r0, [r2, #0]
	bl	Func_08015130
	cmp	r5, #0
	bge.n	.L20
	b.n	.L21
.L20:
	movs	r7, #0
	cmp	r7, r5
	bge.n	.L22
	movs	r6, #187
	lsls	r6, r6, #2
.L32:
	mov	r3, r8
	ldrsh	r3, [r6, r3]
	mov	sl, r3
	bl	Func_0800488c
	bl	Func_080048a0
	movs	r0, #181
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L23
	mov	r2, r8
	adds	r0, r6, r2
	movs	r1, #10
	cmp	r7, #0
	beq.n	.L24
	movs	r1, #0
.L24:
	bl	Func_080b9b30
	cmp	r0, #1
	bne.n	.L25
	b.n	.L26
.L23:
	mov	r3, r8
	adds	r0, r6, r3
	bl	Func_080b874c
	cmp	r0, #1
	bne.n	.L25
	b.n	.L26
.L25:
	bl	Func_0800488c
	bl	Func_080048a0
	movs	r0, #1
	movs	r1, #0
	bl	Func_080b6b40
	cmp	r0, #0
	bne.n	.L27
	b.n	.L28
.L27:
	movs	r0, #2
	movs	r1, #0
	bl	Func_080b6b40
	cmp	r0, #0
	bne.n	.L29
	mov	r1, sl
	cmp	r1, #7
	bhi.n	.L30
	movs	r3, #167
	lsls	r3, r3, #3
	add	r3, r8
	ldr	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L30
	movs	r3, #3
	mov	r2, r8
	strh	r3, [r2, #62]
	b.n	.L30
.L29:
	bl	Func_080b6148
	cmp	r0, #0
	bge.n	.L31
	b.n	.L21
.L31:
	adds	r7, #1
	adds	r6, #16
	cmp	r7, r5
	blt.n	.L32
.L22:
	movs	r3, #0
	mov	r1, fp
	strb	r3, [r1, #0]
	bl	Func_080bf674
	bl	Func_080bf678
	bl	Func_080b7e7c
	ldr	r2, [sp, #4]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L33
	bl	Func_080b6148
	cmp	r0, #0
	bge.n	.L34
	b.n	.L21
.L33:
	movs	r0, #20
	bl	Func_080030f8
.L34:
	movs	r0, #183
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L35
	b.n	.L36
.L35:
	ldr	r0, [pc, #264]
	movs	r1, #0
	movs	r2, #4
	movs	r3, #1
	bl	Func_08015038
	adds	r5, r0, #0
	b.n	.L37
.L38:
	movs	r0, #1
	bl	Func_080030f8
.L37:
	bl	Func_08015048
	cmp	r0, #0
	beq.n	.L38
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015018
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #4
	movs	r3, #1
	movs	r1, #10
	ldr	r0, [pc, #216]
	bl	Func_08015038
	movs	r1, #24
	adds	r5, r0, #0
	movs	r0, #92
	bl	Func_080bb7c0
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015018
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L36
.L30:
	bl	Func_080b63b0
	movs	r0, #183
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L39
	ldr	r1, [sp, #4]
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L40
	movs	r0, #58
	bl	Func_080f9010
.L40:
	movs	r3, #167
	lsls	r3, r3, #3
	add	r3, r8
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L41
	movs	r0, #58
	bl	Func_080f9010
	mov	r2, r8
	ldrh	r3, [r2, #62]
	cmp	r3, #1
	bhi.n	.L41
	ldrh	r3, [r2, #60]
	lsls	r3, r3, #1
	adds	r3, #16
	ldrh	r1, [r2, r3]
	movs	r0, #128
	movs	r2, #26
	bl	Func_08077140
	bl	Func_08015118
	movs	r0, #128
	movs	r1, #1
	bl	Func_08015120
	mov	r3, r8
	ldrh	r0, [r3, #62]
	ldr	r3, [pc, #100]
	adds	r0, r0, r3
	bl	Func_080151c8
	bl	Func_080bb65c
.L41:
	bl	Func_080c2724
.L39:
	movs	r0, #17
	bl	Func_080f9010
	movs	r0, #30
	bl	Func_08003b70
	bl	Func_08003ce0
	movs	r3, #167
	lsls	r3, r3, #3
	add	r3, r8
	ldr	r7, [r3, #0]
	b.n	.L42
.L21:
	bl	Func_080b63b0
	movs	r0, #0
	bl	Func_080042c8
	ldr	r3, [pc, #24]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r3, #167
	lsls	r3, r3, #3
	movs	r0, #250
	add	r3, r8
	lsls	r0, r0, #2
	ldr	r7, [r3, #0]
	bl	Func_080770c8
	b.n	.L42
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x03001f58
	.4byte 0x080b7739
	.4byte 0x03000164
	.4byte 0x00000c47
	.4byte 0x00000c48
	.4byte 0x00000838
.L28:
	bl	Func_080b63b0
	movs	r0, #59
	bl	Func_080f9010
	bl	Func_08015118
	ldr	r3, [pc, #136]
	movs	r1, #252
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldrb	r0, [r3, #0]
	movs	r1, #1
	bl	Func_08015120
	movs	r0, #0
	bl	Func_080b6a60
	cmp	r0, #1
	bne.n	.L43
	ldr	r0, [pc, #112]
	bl	Func_080151c8
	b.n	.L44
.L43:
	ldr	r0, [pc, #108]
	bl	Func_080151c8
.L44:
	bl	Func_080bb65c
	movs	r0, #17
	bl	Func_080f9010
	movs	r7, #1
	movs	r0, #30
	bl	Func_08003b70
	negs	r7, r7
	bl	Func_08003ce0
	b.n	.L42
.L26:
	movs	r0, #17
	bl	Func_080f9010
	movs	r0, #30
	bl	Func_08003b70
	bl	Func_08003ce0
	ldr	r7, [pc, #64]
.L42:
	bl	Func_080b5b18
	bl	Func_080bf674
	bl	Func_080bf5a8
	ldr	r3, [pc, #40]
	ldr	r2, [pc, #52]
	adds	r3, r3, r2
	movs	r2, #0
	strb	r2, [r3, #0]
	ldr	r0, [pc, #48]
	bl	Func_08004278
	bl	Func_080c08e0
	adds	r0, r7, #0
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x0000083d
	.4byte 0x00000837
	.4byte 0x000003e7
	.4byte 0x0000022b
	.4byte 0x080b7739
