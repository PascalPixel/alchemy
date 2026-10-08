@ C2: a load or store between a compare and its branch. Every edition
@ puts an unconditional ldr, ldrb or str between a compare and the
@ conditional branch that reads it, while the compare's operands are
@ written by nothing after it: in all six, str rN, [sp, #8] after
@ cmp r3, #31; lsl ip, ip, #16 and before bls (the row step of the four
@ passes); in Japanese also cmp r1, #255; str r0, [sp, #20]; bls,
@ cmp r3, #1; str r2, [sp, #16]; bhi, cmp r7, #31; ldrb ip, [lr, r3]; bhi
@ and cmp r3, #31; ldrb ip, [r9], #1; bls; in English cmp r7, #31;
@ str r3, [sp, #12]; bhi; in German, Spanish, French and Italian
@ cmp r3, #0; str r0, [sp, #28]; beq and cmp r8, #31; str r2, [sp, #12];
@ bhi. agbcc_arm (K1) cannot emit any of them. sched2 (gcc_arm/sched.c,
@ schedule_block) orders each block last, backwards from its branch, one
@ insn a cycle, and links every insn nothing else in the block consumes
@ to the branch at its result_ready_cost (insn_cost; arm_adjust_cost in
@ arm.c changes only a load after a store). Tuned for the arm7tdmi
@ (ldsched no), arm.md gives load and store1 2 cycles and a compare 1, so
@ the slot before the branch goes to the compare, or another one-cycle
@ insn, whenever one is free; a load or store reaches it only when none
@ is, as when the compare's register is reused or it is movsi_compare0's
@ cmp rX, #0 that also copies rX. After sched2, jump2 (jump.c, its two
@ "x = a" hoists) moves only a register set whose source has no side
@ effect and cannot trap: a pool constant or sp-based load, never a
@ store, an ldrb through two registers or a post-increment. At every site
@ above the compare is free. Measured through the game ARM route (alchemy
@ diff --arm), 658 of the other 837 C files of The Broken Seal and COMMON
@ compile and put a load or store before a conditional branch 44 times,
@ each one of those two cases (38 with no one-cycle insn free, 6 pool
@ constants hoisted by jump2); this function's 5, 2 and 3 sites are
@ neither. The C with every device removed differs in 51 of the Japanese
@ listing's 193 instructions, 146 of the English 196 and 112 of the
@ other four's 187 (0 with the devices).
@ No marker of C2's list is present: lr is saved, r11 is never used and
@ the one multiply is mla.
	.include "games/common/include/game/ed_asm.h"
	.syntax unified
	.arm

	.set	WIN_WIDTH, 0x0a
	.set	WIN_FLAGS, 0x16
	.set	WIN_OUTLINE, 64

	.set	SAVED, 7 * 4

	.ifdef TBS_EDITION_JA
	.set	FRAME, 100
	.set	SP_PASS, 0
	.set	SP_ROWS1, 4
	.set	SP_NEXT, 8
	.set	SP_W, 12
	.set	SP_COLOR, 16
	.set	SP_WIDTH, 20
	.set	SP_RET, 24
	.set	SP_ROWS, 28
	.set	SP_WIN, 32
	.else
	.set	FRAME, 44
	.set	SP_PASS, 0
	.set	SP_ROWS1, 4
	.set	SP_NEXT, 8
	.set	SP_W, 12
	.set	SP_C3, 16
	.set	SP_C1, 20
	.set	SP_SRC, 24
	.set	SP_WIDTH, 28
	.set	SP_RET, 32
	.set	SP_ROWS, 36
	.set	SP_WIN, 40
	.endif
	.set	SP_FONT, FRAME + SAVED

@ 4回の描画で使うレジスタ
	.ifdef TBS_EDITION_JA
xpos	.req	r8
ypos	.req	r7
rows	.req	r0
win	.req	r2
pass	.req	r1
next	.req	r1
color	.req	r4
row	.req	r2
rownext	.req	lr
col	.req	r5
tile	.req	r9
line	.req	sl
	.else
	.ifdef TBS_EDITION_EN
ypos	.req	r7
rows	.req	r2
pass	.req	r1
next	.req	r1
row	.req	r2
tile	.req	sl
line	.req	r8
fontrow	.req	r2
fonttile .req	r8
	.else
ypos	.req	r8
rows	.req	r1
pass	.req	r2
next	.req	r2
row	.req	r1
tile	.req	sl
line	.req	r7
fontrow	.req	r1
fonttile .req	r7
	.endif
xpos	.req	r9
win	.req	r0
color	.req	r5
rownext	.req	r4
col	.req	lr
	.endif

	.text
	.global	UiText_DrawGlyphArm
	.type	UiText_DrawGlyphArm, %function
UiText_DrawGlyphArm:
	push	{r5, r6, r7, r8, r9, sl, lr}
	.ifdef TBS_EDITION_JA
	sub	sp, sp, #FRAME
	ldr	lr, [sp, #SP_FONT]
	mov	xpos, r2
	str	r0, [sp, #SP_WIN]
	mov	ypos, r3
	ldrh	r0, [r0, #WIN_WIDTH]
	cmp	r1, #255
	str	r0, [sp, #SP_WIDTH]
	bls	.L_half
	cmp	lr, #0
	sub	ip, r1, #222
	beq	.L_kanji
	cmp	ip, #1
	bhi	.L_kanji
.L_half:
	cmp	lr, #0
	beq	.L_glyph

@ 1bpp、1文字10バイト
	mov	r2, #0x1100				@ 色1
	add	r2, r2, #0x11
	mov	r3, #8
	add	xpos, xpos, #1
	str	r3, [sp, #SP_W]
	sub	r3, r1, #222
	cmp	r3, #1
	str	r2, [sp, #SP_COLOR]
	bhi	.L_font_code
	add	r1, r1, #32
	sub	xpos, xpos, #2
	sub	ypos, ypos, #1
	mov	r0, #2
	str	r0, [sp, #SP_W]
.L_font_code:
	sub	r1, r1, #32
	add	r3, r1, r1, lsl #2
	lsl	r3, r3, #1
	add	r9, lr, r3
	mov	r2, #0
	add	r9, r9, #1
	cmp	ypos, #31
	ldrb	ip, [lr, r3]
	bhi	.L_font_done
.L_font_row:
	mov	r5, #0
	cmp	ip, r5
	add	lr, r2, #1
	beq	.L_font_next
	add	r3, ypos, r2
	lsr	sl, r3, #3
	mov	r4, #0x2500				@ VRAM
	add	r4, r4, #0x06000000
	and	r3, r3, #7
	lsl	r6, r3, #1
.L_font_dot:
	tst	ip, #1
	beq	.L_font_skip
	add	r1, xpos, r5
	ldr	r0, [sp, #SP_WIDTH]
	lsr	r2, r1, #3
	sub	r3, r0, #2
	mla	r0, r3, r2, sl
	and	r3, r1, #7
	orr	r3, r6, r3, lsr #2
	and	r1, r1, #3
	lsl	r1, r1, #2
	add	r0, r4, r0, lsl #5
	orr	r0, r0, r3, lsl #1
	mov	r3, #15
	ldrh	r2, [r0]
	lsl	r3, r3, r1
	ldr	r1, [sp, #SP_COLOR]
	bic	r2, r2, r3
	and	r3, r3, r1
	orr	r2, r2, r3
	strh	r2, [r0]
.L_font_skip:
	add	r5, r5, #1
	lsrs	ip, ip, #1
	bne	.L_font_dot
.L_font_next:
	mov	r2, lr
	cmp	lr, #9
	bhi	.L_font_done
	add	r3, ypos, lr
	cmp	r3, #31
	ldrb	ip, [r9], #1
	bls	.L_font_row
.L_font_done:
	ldr	r0, [sp, #SP_W]
	b	.L_return

@ 1文字26バイト
.L_glyph:
	sub	ip, r1, #222
	sub	r3, r1, #32
	add	r2, r3, r3, lsl #1
	add	r3, r3, r2, lsl #2
	ldr	r1, .L_UiText_Glyphs
	lsl	r3, r3, #1
	add	r2, r3, r1
	add	r2, r2, #2
	str	r2, [sp, #SP_ROWS]
	ldrh	r0, [r3, r1]
	b	.L_shift

@ 漢字、1文字24バイト
.L_kanji:
	mov	r0, #10
	sub	r3, r1, #256
	ldr	r2, .L_UiText_KanjiGlyphs
	add	r3, r3, r3, lsl #1
	add	r3, r2, r3, lsl #3
	str	r3, [sp, #SP_ROWS]
.L_shift:
	cmp	ip, #1
	bhi	.L_ret_width
	sub	xpos, xpos, #3
	sub	ypos, ypos, #2
	mov	r3, #0
	str	r3, [sp, #SP_RET]
	b	.L_passes
.L_ret_width:
	str	r0, [sp, #SP_RET]

	.else
	.ifdef TBS_EDITION_EN
	mov	ypos, r3
	sub	sp, sp, #FRAME
	ldr	r3, [sp, #SP_FONT]
	mov	xpos, r2
	str	r0, [sp, #SP_WIN]
	sub	ip, r1, #32
	ldrh	r0, [r0, #WIN_WIDTH]
	cmp	ip, #111
	str	r0, [sp, #SP_WIDTH]
	movhi	r0, #1
	bhi	.L_return
	cmp	r1, #255
	bls	.L_half
	cmp	r3, #0
	sub	lr, r1, #222
	beq	.L_wide
	cmp	lr, #1
	bhi	.L_wide
.L_half:
	cmp	r3, #0
	beq	.L_glyph

@ 2bpp、1文字32バイト
	add	r1, r3, r1, lsl #5
	str	r1, [sp, #SP_SRC]
	mov	r1, #0x1100				@ 色1
	add	r1, r1, #0x11
	mov	r2, #0x3300				@ 色3
	str	r1, [sp, #SP_C1]
	add	r2, r2, #0x33
	str	r2, [sp, #SP_C3]
	ldr	r3, .L_UiText_GlyphWidths
	mov	fontrow, #0
	ldrb	r3, [r3, ip]
	cmp	ypos, #31
	str	r3, [sp, #SP_W]
	bhi	.L_font_done
	.else
	mov	xpos, r2
	mov	ypos, r3
	sub	sp, sp, #FRAME
	ldr	r2, [sp, #SP_FONT]
	mov	ip, r1
	str	r0, [sp, #SP_WIN]
	subs	r3, r2, #0
	movne	r3, #1
	cmp	ip, #255
	movls	r3, #1
	ldrh	r0, [r0, #WIN_WIDTH]
	cmp	r3, #0
	str	r0, [sp, #SP_WIDTH]
	beq	.L_wide
	cmp	r2, #0
	beq	.L_glyph

@ 2bpp、1文字32バイト
	add	r2, r2, ip, lsl #5
	mov	r1, #0x1100				@ 色1
	add	r1, r1, #0x11
	str	r2, [sp, #SP_SRC]
	mov	r2, #0x3300				@ 色3
	str	r1, [sp, #SP_C1]
	add	r2, r2, #0x33
	str	r2, [sp, #SP_C3]
	mov	fontrow, #0
	ldr	r2, .L_UiText_GlyphWidths
	sub	r3, ip, #32
	ldrb	r2, [r2, r3]
	cmp	ypos, #31
	str	r2, [sp, #SP_W]
	bhi	.L_font_done
	.endif
	mov	sl, #0x2500				@ VRAM
	add	sl, sl, #0x06000000
.L_font_row:
	mov	lr, #0
	add	r4, fontrow, #1
	add	r3, ypos, fontrow
	lsr	fonttile, r3, #3
	and	r3, r3, #7
	lsl	r6, r3, #1
.L_font_dot:
	tst	lr, #1
	lsr	r5, r5, #4
	ldreq	r3, [sp, #SP_SRC]
	ldrbeq	r5, [r3], #1
	streq	r3, [sp, #SP_SRC]
	and	r3, r5, #13
	cmp	r3, #1
	bne	.L_font_skip
	and	r3, r5, #15
	cmp	r3, #1
	ldr	ip, [sp, #SP_C3]
	add	r1, xpos, lr
	ldr	r0, [sp, #SP_C1]
	lsr	r2, r1, #3
	moveq	ip, r0
	cmp	r2, #29
	bhi	.L_font_skip
	ldr	r0, [sp, #SP_WIDTH]
	sub	r3, r0, #2
	mla	r0, r3, r2, fonttile
	and	r3, r1, #7
	orr	r3, r6, r3, lsr #2
	and	r1, r1, #3
	lsl	r1, r1, #2
	add	r0, sl, r0, lsl #5
	orr	r0, r0, r3, lsl #1
	mov	r3, #15
	ldrh	r2, [r0]
	lsl	r3, r3, r1
	bic	r2, r2, r3
	and	r3, r3, ip
	orr	r2, r2, r3
	strh	r2, [r0]
.L_font_skip:
	add	lr, lr, #1
	cmp	lr, #7
	bls	.L_font_dot
	mov	fontrow, r4
	cmp	r4, #7
	bhi	.L_font_done
	add	r3, ypos, r4
	cmp	r3, #31
	bls	.L_font_row
.L_font_done:
	ldr	r0, [sp, #SP_W]
	b	.L_return

	.ifdef TBS_EDITION_EN
@ 1文字26バイト
.L_glyph:
	sub	lr, r1, #222
	add	r3, ip, ip, lsl #1
	add	r3, ip, r3, lsl #2
	ldr	r2, .L_UiText_Glyphs
	lsl	r3, r3, #1
	add	r1, r3, r2
	add	r1, r1, #2
	str	r1, [sp, #SP_ROWS]
	b	.L_width

@ 1文字32バイト
.L_wide:
	sub	r3, r1, #32
	ldr	r2, .L_UiText_Glyphs
	lsl	r3, r3, #5
	add	r0, r3, r2
	add	r0, r0, #8
	str	r0, [sp, #SP_ROWS]
.L_width:
	ldrh	r0, [r3, r2]
	cmp	lr, #1
	bhi	.L_ret_width
	sub	xpos, xpos, #3
	sub	ypos, ypos, #2
	mov	r1, #0
	str	r1, [sp, #SP_RET]
	b	.L_passes
.L_ret_width:
	str	r0, [sp, #SP_RET]
	.else
@ 1文字26バイト
.L_glyph:
	sub	r3, ip, #32
	add	r2, r3, r3, lsl #1
	add	r3, r3, r2, lsl #2
	ldr	r1, .L_UiText_Glyphs
	lsl	r3, r3, #1
	add	r2, r3, r1
	add	r2, r2, #2
	str	r2, [sp, #SP_ROWS]
	ldrh	r3, [r3, r1]
	b	.L_ret_width

@ 1文字32バイト
.L_wide:
	sub	r3, ip, #32
	ldr	r2, .L_UiText_Glyphs
	lsl	r3, r3, #5
	add	r0, r3, r2
	add	r0, r0, #8
	str	r0, [sp, #SP_ROWS]
	ldrh	r3, [r3, r2]
.L_ret_width:
	str	r3, [sp, #SP_RET]
	.endif
	.endif

@ 4回描く、最後は色1
.L_passes:
	ldr	rows, [sp, #SP_ROWS]
	mov	pass, #0
	add	rows, rows, #2
	str	rows, [sp, #SP_ROWS1]
.L_pass:
	cmp	pass, #3
	moveq	color, #0x1100				@ 色1
	addeq	color, color, #0x11
	movne	color, #0x5500				@ 色5
	addne	color, color, #0x55
	ldr	win, [sp, #SP_WIN]
	ldrh	r3, [win, #WIN_FLAGS]
	tst	r3, #WIN_OUTLINE
	beq	.L_draw
	cmp	pass, #1
	bhi	.L_pass_2
	addeq	xpos, xpos, #2				@ 右
	beq	.L_draw
	sub	xpos, xpos, #1				@ 左
	b	.L_draw
.L_pass_2:
	cmp	pass, #2
	bne	.L_pass_3
	sub	xpos, xpos, #1				@ 下
	add	ypos, ypos, #1
	b	.L_draw

	.ifdef TBS_EDITION_JA
.L_UiText_Glyphs:
	.4byte	UiText_Glyphs
.L_UiText_KanjiGlyphs:
	.4byte	UiText_KanjiGlyphs
	.else
.L_UiText_GlyphWidths:
	.4byte	UiText_GlyphWidths
.L_UiText_Glyphs:
	.4byte	UiText_Glyphs
	.endif

.L_pass_3:
	sub	ypos, ypos, #1				@ 元の位置
.L_draw:
	ldr	r3, [sp, #SP_ROWS1]
	ldr	r0, [sp, #SP_ROWS]
	mov	row, #0
	str	r3, [sp, #SP_NEXT]
	cmp	ypos, #31
	ldrh	ip, [r0]
	add	pass, pass, #1
	str	pass, [sp, #SP_PASS]
	lsl	ip, ip, #16
	bhi	.L_pass_next
	mov	r6, #0x2500				@ VRAM
	add	r6, r6, #0x06000000
.L_row:
	mov	col, #0
	add	rownext, row, #1
	add	r3, ypos, row
	lsr	tile, r3, #3
	and	r3, r3, #7
	lsl	line, r3, #1
.L_dot:
	cmp	ip, #0
	bge	.L_dot_next
	add	r1, xpos, col
	ldr	r0, [sp, #SP_WIDTH]
	lsr	r2, r1, #3
	sub	r3, r0, #2
	mla	r0, r3, r2, tile
	and	r3, r1, #7
	orr	r3, line, r3, lsr #2
	and	r1, r1, #3
	lsl	r1, r1, #2
	add	r0, r6, r0, lsl #5
	orr	r0, r0, r3, lsl #1
	mov	r3, #15
	ldrh	r2, [r0]
	lsl	r3, r3, r1
	bic	r2, r2, r3
	and	r3, r3, color
	orr	r2, r2, r3
	strh	r2, [r0]
.L_dot_next:
	lsls	ip, ip, #1
	beq	.L_row_next
	add	col, col, #1
	cmp	col, #13
	bls	.L_dot
.L_row_next:
	mov	row, rownext
	cmp	rownext, #11
	bhi	.L_pass_next
	ldr	next, [sp, #SP_NEXT]
	add	r3, ypos, rownext
	ldrh	ip, [next], #2
	cmp	r3, #31
	lsl	ip, ip, #16
	str	next, [sp, #SP_NEXT]
	bls	.L_row
.L_pass_next:
	ldr	pass, [sp, #SP_PASS]
	cmp	pass, #3
	bls	.L_pass
	ldr	r0, [sp, #SP_RET]
.L_return:
	add	sp, sp, #FRAME
	pop	{r5, r6, r7, r8, r9, sl, lr}
	bx	lr
	.size	UiText_DrawGlyphArm, . - UiText_DrawGlyphArm
