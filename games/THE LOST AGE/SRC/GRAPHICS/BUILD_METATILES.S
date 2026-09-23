@ Metatile builder, copied and run as one 536-byte unit (size and source
@ {0x218, 0x080388fc} in its loader's literal pool at 0x080425f4). It
@ decodes the commands 0xf007 to 0xf01b of halfword command lists through two
@ jump tables. It is kept as assembly without credit because after its
@ 4-byte Thumb entry (bx pc) the body has the shape of GCC 2.96 ARM-mode
@ output (each switch as cmp; ldrls pc, [pc, r3, lsl #2]; b default,
@ halfword stores post-incremented by their size with the rest of the step
@ as a separate add, lr saved with r5-sl while r4 stays free), and no
@ approved route compiles ARM state. After that entry its 532-byte body
@ equals TBS GRAPHICS/TILE/BUILD_METATILES.S (0x080158e8), which is likewise
@ uncredited, apart from the 41 relocated case addresses of its tables.
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x080388fc
	.global Tile_BuildMetatiles
	.thumb_func
Tile_BuildMetatiles:
	bx	pc
	.2byte	0x0200
	.arm
	push	{r5, r6, r7, r8, r9, sl, lr}
	mov	sl, r1
	mov	r1, #0
.L_0803890c:
	mov	r7, #0
	mov	lr, r7
	mov	r8, r7
	mov	r2, r7
	mov	ip, r2
	mov	r4, r2
	mov	r5, r2
	mov	r6, r2
	add	r0, r0, #2
	add	r9, r1, #1
	mov	r1, r2
.L_08038938:
	ldrh	r3, [sl], #2
	cmp	r3, #0
	beq	.L_08038aa4
	cmp	r5, #0
	bne	.L_08038a10
	sub	r3, r3, #0xf000
	sub	r3, r3, #7
	cmp	r3, #19
	ldrls	pc, [pc, r3, lsl #2]
	b	.L_08038a0c
	.4byte	.L_080389f0
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_080389b0
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_080389d0
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_080389f0
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_080389f0
.L_080389b0:
	lsl	lr, r6, #3
	add	ip, lr, #8
	add	r8, lr, #1
	mov	r7, lr
	add	r4, r7, #7
	mov	r2, ip
	add	r5, r5, #1
	b	.L_08038aa4
.L_080389d0:
	lsl	lr, r6, #3
	add	ip, lr, #8
	mov	r8, lr
	add	r7, lr, #1
	mov	r4, ip
	add	r2, lr, #7
	add	r5, r5, #1
	b	.L_08038aa4
.L_080389f0:
	lsl	lr, r6, #3
	add	ip, lr, #8
	mov	r7, lr
	mov	r8, r7
	mov	r2, ip
	mov	r4, r2
	add	r5, r5, #1
.L_08038a0c:
	b	.L_08038aa4
.L_08038a10:
	sub	r3, r3, #0xf000
	sub	r3, r3, #7
	cmp	r3, #20
	ldrls	pc, [pc, r3, lsl #2]
	b	.L_08038aa4
	.4byte	.L_08038a98
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_08038a78
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_08038a88
	.4byte	.L_08038aa4
	.4byte	.L_08038a98
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_08038aa4
	.4byte	.L_08038a98
.L_08038a78:
	add	ip, r1, #8
	add	r4, r1, #7
	mov	r2, ip
	b	.L_08038aa4
.L_08038a88:
	add	ip, r1, #8
	mov	r4, ip
	add	r2, r1, #7
	b	.L_08038aa4
.L_08038a98:
	add	r2, r1, #8
	mov	ip, r2
	mov	r4, r2
.L_08038aa4:
	add	r1, r1, #8
	add	r6, r6, #1
	cmp	r6, #29
	bls	.L_08038938
	add	r3, r4, r8, lsl #8
	strh	r3, [r0], #2
	add	r3, ip, lr, lsl #8
	add	r0, r0, #2
	strh	r3, [r0], #2
	add	r0, r0, #2
	strh	r3, [r0], #2
	add	r0, r0, #2
	strh	r3, [r0], #2
	add	sl, sl, #4
	add	r0, r0, #2
	strh	r3, [r0], #2
	mov	r1, r9
	add	r0, r0, #2
	strh	r3, [r0], #2
	cmp	r1, #19
	add	r0, r0, #2
	strh	r3, [r0], #2
	add	r3, r2, r7, lsl #8
	add	r0, r0, #2
	strh	r3, [r0], #2
	bls	.L_0803890c
	pop	{r5, r6, r7, r8, r9, sl, lr}
	bx	lr
