@ Why Render_DecodePicture is assembly:
@ Computed add pc dispatch into code; a later block saves r5-r9 without lr.
@ Why TileMap_DrawRows is assembly:
@ r4 (0xffa00038) is at once two packed loop counters and an AND mask.
@ subs r4, r4, #0x01000000 / bpl counts the inner loop in its top byte, reset
@ by add r4, r4, #0x1e000000; sub r4, r4, #0x10000 with tst r4, #0x70000 and
@ tst r4, #0xff0000 / bne counts the rows in bits 16-23; and every
@ and sl, r4, r8, lsr #20 uses the same register as the mask, of which only
@ the low 0x38 can meet the 12 shifted bits. A compiler keeps two counters
@ and a constant mask as separate values.
@ Why assembly: r11/fp is a tile-data address (add fp, r3, r7, lsr #26), not a frame pointer.
.syntax unified
	.arm
	.global Render_DecodePicture
Render_DecodePicture:
	cmp	r2, #0
	bne	.Lmode
	mov	r4, #255
	ands	r2, r0, #3
	ldm	r0!, {r3}
	add	r2, r2, r2, lsl #2
	add	pc, pc, r2, lsl #2
AlchemyUncredited_render_decode_1:
	nop
.Lcopy_checked:
AlchemyUncreditedEnd_render_decode_1:
	ands	r2, r4, r3
	bxeq	lr
	subs	ip, r2, #223
	addhi	r1, r1, ip
	strbls	r2, [r1], #1
	ands	r2, r4, r3, lsr #8
	bxeq	lr
	subs	ip, r2, #223
	addhi	r1, r1, ip
	strbls	r2, [r1], #1
	ands	r2, r4, r3, lsr #16
	bxeq	lr
	subs	ip, r2, #223
	addhi	r1, r1, ip
	strbls	r2, [r1], #1
	ands	r2, r4, r3, lsr #24
	bxeq	lr
	subs	ip, r2, #223
	addhi	r1, r1, ip
	strbls	r2, [r1], #1
	ldm	r0!, {r3}
	and	r2, r3, r3, lsr #16
	ands	r2, r2, r2, lsr #8
	beq	.Lcopy_checked
.Lcopy_unchecked:
	ands	r2, r4, r3
	subs	ip, r2, #223
	addhi	r1, r1, ip
	strbls	r2, [r1], #1
	ands	r2, r4, r3, lsr #8
	subs	ip, r2, #223
	addhi	r1, r1, ip
	strbls	r2, [r1], #1
	ands	r2, r4, r3, lsr #16
	subs	ip, r2, #223
	addhi	r1, r1, ip
	strbls	r2, [r1], #1
	ands	r2, r4, r3, lsr #24
	subs	ip, r2, #223
	addhi	r1, r1, ip
	strbls	r2, [r1], #1
	ldm	r0!, {r3}
	and	r2, r3, r3, lsr #16
	ands	r2, r2, r2, lsr #8
	bne	.Lcopy_unchecked
	b	.Lcopy_checked
	.4byte gDecodeFillByte
	ldr	r2, [pc, #-12]
	ldrb	r2, [r2]
	b	.Lfill_next
	mov	r2, #0
	b	.Lfill_next
	mov	r2, #1
	b	.Lfill_next
	mov	r2, #15
	b	.Lfill_next
.Lfill_put:
	subs	r3, r3, #223
	addhi	r1, r1, r3
	strbls	r2, [r1], #1
.Lfill_next:
	ldrb	r3, [r0], #1
	cmp	r3, #0
	bne	.Lfill_put
	bx	lr
.Lmode:
	add	r4, r2, r2
	ldrsh	r4, [pc, r4]
	add	pc, pc, r4
	.2byte -288, 32, 32, 32
	.2byte 32, 32, 32, -52
	.2byte -52, 28, 28, 28
	.2byte 28, -80, -60, -68
	sub	r2, r2, #2
	ldr	r4, [pc, #136]
	cmp	r4, r2
	beq	.Lmap_ready
	str	r2, [pc, #124]
	ldr	r3, [pc, #128]
	add	r2, r3, r2, lsl #8
	add	r3, pc, #124
	push	{r5, r6, r7, r8, r9}
	ldm	r2!, {r4, r5, r6, r7, r8, r9, ip}
	stmia	r3!, {r4, r5, r6, r7, r8, r9, ip}
	ldm	r2!, {r4, r5, r6, r7, r8, r9, ip}
	stmia	r3!, {r4, r5, r6, r7, r8, r9, ip}
	ldm	r2!, {r4, r5, r6, r7, r8, r9, ip}
	stmia	r3!, {r4, r5, r6, r7, r8, r9, ip}
	ldm	r2!, {r4, r5, r6, r7, r8, r9, ip}
	stmia	r3!, {r4, r5, r6, r7, r8, r9, ip}
	ldm	r2!, {r4, r5, r6, r7, r8, r9, ip}
	stmia	r3!, {r4, r5, r6, r7, r8, r9, ip}
	ldm	r2!, {r4, r5, r6, r7, r8, r9, ip}
	stmia	r3!, {r4, r5, r6, r7, r8, r9, ip}
	ldm	r2!, {r4, r5, r6, r7, r8, r9, ip}
	stmia	r3!, {r4, r5, r6, r7, r8, r9, ip}
	ldm	r2!, {r4, r5, r6, r7, r8, r9, ip}
	stmia	r3!, {r4, r5, r6, r7, r8, r9, ip}
	pop	{r5, r6, r7, r8, r9}
.Lmap_ready:
	add	r2, pc, #48
	b	.Lmap_next
.Lmap_skip:
	add	r1, r1, r4
.Lmap_next:
	ldrb	r3, [r0], #1
	subs	r4, r3, #223
	bhi	.Lmap_skip
	cmp	r3, #0
	ldrbne	r3, [r2, r3]
	strbne	r3, [r1], #1
	bne	.Lmap_next
	bx	lr
.syntax unified
	.section .data,"aw",%progbits
	.arm
	@ The table number last loaded, then the colour tables' address less
	@ 224 bytes (unused) and less 256 bytes: table n is the 256 bytes at
	@ that base + n * 256.
	.4byte 0
	.4byte gSpriteColorTables - 224
	.4byte gSpriteColorTables - 256
	.global gRender_DecodePictureEnd
gRender_DecodePictureEnd:
	.space 224, 0
.syntax unified
	.section .text.draw_map,"ax",%progbits
	.arm
	.global TileMap_DrawRows
TileMap_DrawRows:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	mov	r4, #10485760
	orr	r4, r4, #56
	orr	r4, r4, #-16777216
.Ldraw_row:
	ldrsh	r6, [r0, #4]
	ldr	r7, [r0, #8]
	ldr	r8, [r0, #12]
	ldrsh	r5, [r0], #32
	lsl	r7, r7, #15
	lsl	r8, r8, #15
	add	r4, r4, #503316480
.Ldraw_cell:
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	ip, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	ip, #0
	ldrbne	ip, [sl, ip, lsl #6]
	cmp	ip, #0
	subeq	fp, fp, #4096
	ldrbeq	ip, [fp]
	subeq	sl, sl, #16384
	ldrbeq	ip, [sl, ip, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	r9, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	r9, #0
	ldrbne	r9, [sl, r9, lsl #6]
	cmp	r9, #0
	subeq	fp, fp, #4096
	ldrbeq	r9, [fp]
	subeq	sl, sl, #16384
	ldrbeq	r9, [sl, r9, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	orr	r9, ip, r9, lsl #8
	strh	r9, [r1], #2
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	ip, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	ip, #0
	ldrbne	ip, [sl, ip, lsl #6]
	cmp	ip, #0
	subeq	fp, fp, #4096
	ldrbeq	ip, [fp]
	subeq	sl, sl, #16384
	ldrbeq	ip, [sl, ip, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	r9, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	r9, #0
	ldrbne	r9, [sl, r9, lsl #6]
	cmp	r9, #0
	subeq	fp, fp, #4096
	ldrbeq	r9, [fp]
	subeq	sl, sl, #16384
	ldrbeq	r9, [sl, r9, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	orr	r9, ip, r9, lsl #8
	strh	r9, [r1], #2
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	ip, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	ip, #0
	ldrbne	ip, [sl, ip, lsl #6]
	cmp	ip, #0
	subeq	fp, fp, #4096
	ldrbeq	ip, [fp]
	subeq	sl, sl, #16384
	ldrbeq	ip, [sl, ip, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	r9, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	r9, #0
	ldrbne	r9, [sl, r9, lsl #6]
	cmp	r9, #0
	subeq	fp, fp, #4096
	ldrbeq	r9, [fp]
	subeq	sl, sl, #16384
	ldrbeq	r9, [sl, r9, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	orr	r9, ip, r9, lsl #8
	strh	r9, [r1], #2
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	ip, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	ip, #0
	ldrbne	ip, [sl, ip, lsl #6]
	cmp	ip, #0
	subeq	fp, fp, #4096
	ldrbeq	ip, [fp]
	subeq	sl, sl, #16384
	ldrbeq	ip, [sl, ip, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	lsr	sl, r8, #26
	add	fp, r3, r7, lsr #26
	ldrb	r9, [fp, sl, lsl #6]!
	and	sl, r4, r8, lsr #20
	lsl	lr, r7, #6
	add	sl, sl, lr, lsr #29
	add	sl, sl, r2
	cmp	r9, #0
	ldrbne	r9, [sl, r9, lsl #6]
	cmp	r9, #0
	subeq	fp, fp, #4096
	ldrbeq	r9, [fp]
	subeq	sl, sl, #16384
	ldrbeq	r9, [sl, r9, lsl #6]
	add	r7, r7, r5, lsl #15
	add	r8, r8, r6, lsl #15
	orr	r9, ip, r9, lsl #8
	strh	r9, [r1], #2
	add	r1, r1, #56
	subs	r4, r4, #16777216
	bpl	.Ldraw_cell
	sub	r1, r1, #1920
	add	r1, r1, #8
	sub	r4, r4, #65536
	tst	r4, #458752
	addeq	r1, r1, #1856
	tst	r4, #16711680
	bne	.Ldraw_row
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
	.global TileMap_DrawRowsCodeSize
	.set TileMap_DrawRowsCodeSize, . - TileMap_DrawRows
