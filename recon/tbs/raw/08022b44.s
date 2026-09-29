@ DjinnMenu_ShowChangePreview matches as C, but it contains the nested arrow
@ renderer (recon/tbs/raw/08022a7c.s), which does not yet; it links as the
@ compiler's own assembly while its source is a draft (recon/tbs/en/main/08022b44.c).
	.code	16
.text
	.align	2, 0
	.global	DjinnMenu_ShowChangePreview
	.thumb_func
	.type	 DjinnMenu_ShowChangePreview,function
DjinnMenu_ShowChangePreview:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, sp, #76
	add	r4, sp, #72
	mov	r7, r1
	str	r0, [r4]
	mov	r0, r7
	mov	sl, r4
	mov	r8, r2
	str	r3, [sp, #60]
	bl	Runtime_GetObject
	str	r0, [sp, #56]
	mov	r1, sl
	ldr	r0, [r1]
	cmp	r0, #0
	beq	.L12
	mov	r1, #1
	bl	UiWork_Finalize
.L12:
	ldr	r2, [sp, #60]
	cmp	r2, #0
	bne	.L13
	mov	r3, #6
	str	r3, [sp]
	mov	r0, #0
	mov	r3, #11
	mov	r1, #8
	mov	r2, #21
	bl	UiWindow_Create
	mov	r3, sl
	str	r0, [r3]
	b	.L14
.L13:
	mov	r3, #6
	str	r3, [sp]
	mov	r0, #0
	mov	r1, #5
	mov	r2, #21
	mov	r3, #14
	bl	UiWindow_Create
	mov	r4, sl
	str	r0, [r4]
.L14:
	mov	r1, sl
	ldr	r3, [r1]
	mov	r0, #0
	cmp	r3, #0
	bne	.LCB169
	b	.L2	@long jump
.LCB169:
	mov	r0, #128
	bl	Runtime_BumpAllocate
	mov	r5, #166
	lsl	r5, r5, #1
	str	r0, [sp, #48]
	mov	r0, r5
	bl	Runtime_BumpAllocate
	str	r0, [sp, #52]
	mov	r0, #96
	bl	Runtime_BumpAllocateAlternatePool
	mov	r2, r5
	str	r0, [sp, #24]
	ldr	r3, .L56+8
	ldr	r1, [sp, #56]
	ldr	r0, [sp, #52]
	bl	_call_via_r3
	mov	r2, r8
	asr	r5, r2, #8
	mov	r3, #15
	mov	r6, #255
	and	r5, r5, r3
	and	r6, r6, r2
	mov	r0, r7
	mov	r1, r5
	mov	r2, r6
	bl	Func_08077208
	cmp	r0, #0
	beq	.L16
	mov	r0, r7
	mov	r1, r5
	mov	r2, r6
	bl	Func_080771b8
	b	.L17
.L16:
	mov	r0, r7
	mov	r1, r5
	mov	r2, r6
	bl	Func_080771b0
.L17:
	mov	r0, r7
	bl	BattleUnit_Recalculate
	ldr	r0, [sp, #52]
	ldr	r1, [sp, #56]
	add	r2, sp, #64
	add	r3, sp, #68
	str	r2, [sp]
	add	r1, r1, #88
	ldr	r2, [sp, #24]
	add	r0, r0, #88
	bl	DjinnMenu_ListChangedDjinn
	mov	r1, #5
	str	r0, [sp, #20]
	sub	r0, r0, #1
	bl	FixedPoint_Ratio
	ldr	r3, [sp, #108]
	add	r0, r0, #1
	str	r0, [r3]
	ldr	r4, [sp, #60]
	lsl	r3, r4, #2
	add	r3, r3, r4
	ldr	r1, [sp, #20]
	sub	r3, r3, #5
	cmp	r3, r1
	blt	.L18
	str	r0, [sp, #60]
.L18:
	ldr	r2, [sp, #60]
	cmp	r2, #0
	beq	.LCB251
	b	.L19	@long jump
.LCB251:
	ldr	r5, .L56+12
	mov	r3, sl
	ldr	r1, [r3]
	mov	r0, r5
	mov	r2, #0
	mov	r3, #8
	bl	UiText_DrawCharacterAtOffset
	mov	r4, sl
	ldr	r1, [r4]
	add	r0, r5, #1
	mov	r2, #0
	mov	r3, #16
	bl	UiText_DrawCharacterAtOffset
	mov	r2, sl
	add	r0, r5, #2
	ldr	r1, [r2]
	mov	r3, #24
	mov	r2, #0
	bl	UiText_DrawCharacterAtOffset
	mov	r3, sl
	add	r0, r5, #3
	ldr	r1, [r3]
	mov	r2, #0
	mov	r3, #32
	bl	UiText_DrawCharacterAtOffset
	mov	r4, sl
	ldr	r1, [r4]
	add	r0, r5, #4
	mov	r2, #0
	mov	r3, #40
	bl	UiText_DrawCharacterAtOffset
	add	r5, r5, #5
	mov	r2, sl
	ldr	r1, [r2]
	mov	r0, r5
	mov	r2, #0
	mov	r3, #48
	bl	UiText_DrawCharacterAtOffset
	ldr	r3, [sp, #52]
	mov	r4, #56
	ldrsh	r3, [r3, r4]
	ldr	r0, [sp, #48]
	mov	r1, r3
	str	r3, [sp, #44]
	bl	UiText_FormatNumberToHalfwords
	ldr	r5, [sp, #48]
	mov	r4, sl
	add	r5, r5, #14
	ldr	r1, [r4]
	mov	r2, #5
	mov	r3, #1
	mov	r0, r5
	bl	UiText_RenderWideStringInWindow
	ldr	r1, [sp, #52]
	mov	r2, #58
	ldrsh	r1, [r1, r2]
	ldr	r0, [sp, #48]
	str	r1, [sp, #40]
	bl	UiText_FormatNumberToHalfwords
	mov	r2, sl
	ldr	r1, [r2]
	mov	r0, r5
	mov	r2, #5
	mov	r3, #2
	bl	UiText_RenderWideStringInWindow
	ldr	r3, [sp, #52]
	ldrh	r3, [r3, #60]
	ldr	r0, [sp, #48]
	mov	r1, r3
	str	r3, [sp, #36]
	bl	UiText_FormatNumberToHalfwords
	ldr	r5, [sp, #48]
	mov	r4, sl
	add	r5, r5, #16
	ldr	r1, [r4]
	mov	r2, #6
	mov	r3, #3
	mov	r0, r5
	bl	UiText_RenderWideStringInWindow
	ldr	r1, [sp, #52]
	ldrh	r1, [r1, #62]
	ldr	r0, [sp, #48]
	str	r1, [sp, #32]
	bl	UiText_FormatNumberToHalfwords
	mov	r2, sl
	ldr	r1, [r2]
	mov	r3, #4
	mov	r2, #6
	mov	r0, r5
	bl	UiText_RenderWideStringInWindow
	ldr	r3, [sp, #52]
	add	r3, r3, #64
	ldrh	r3, [r3]
	ldr	r0, [sp, #48]
	mov	r1, r3
	str	r3, [sp, #28]
	bl	UiText_FormatNumberToHalfwords
	mov	r3, sl
	ldr	r1, [r3]
	mov	r2, #6
	mov	r3, #5
	mov	r0, r5
	bl	UiText_RenderWideStringInWindow
	ldr	r3, [sp, #52]
	add	r3, r3, #66
	ldrb	r1, [r3]
	ldr	r0, [sp, #48]
	bl	UiText_FormatNumberToHalfwords
	mov	r4, sl
	ldr	r1, [r4]
	mov	r2, #5
	mov	r3, #6
	mov	r0, r5
	bl	UiText_RenderWideStringInWindow
	mov	r1, sl
	mov	r3, #8
	ldr	r0, [r1]
	mov	r2, #8
	str	r3, [sp]
	mov	r1, #0
	mov	r3, #19
	bl	UiWindow_DrawDividerLine
	ldr	r3, [sp, #68]
	cmp	r3, #0
	bne	.L21
	ldr	r3, [sp, #64]
	cmp	r3, #0
	beq	.L20
.L21:
	mov	r0, #2
	bl	UiWork_SetParamNibble
.L20:
	mov	r2, sl
	ldr	r0, .L56+16
	ldr	r1, [r2]
	mov	r3, #64
	mov	r2, #24
	bl	UiText_DrawCharacterAtOffset
	mov	r0, #15
	bl	UiWork_SetParamNibble
.L19:
	ldr	r3, [sp, #60]
	cmp	r3, #0
	bgt	.LCB423
	b	.L22	@long jump
.LCB423:
	ldr	r1, [sp, #20]
	mov	r4, #0
	str	r4, [sp, #12]
	str	r1, [sp, #8]
	cmp	r1, #4
	ble	.L23
	mov	r2, #5
	str	r2, [sp, #8]
.L23:
	ldr	r4, [sp, #60]
	lsl	r3, r4, #2
	add	r3, r3, r4
	sub	r3, r3, #5
	mov	r8, r3
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #8]
	mov	r1, #0
	str	r1, [sp, #16]
	cmp	r2, r3
	blt	.LCB444
	b	.L25	@long jump
.LCB444:
	ldr	r4, [sp, #20]
	cmp	r8, r4
	blt	.LCB447
	b	.L25	@long jump
.LCB447:
	mov	r1, r8
	ldr	r2, [sp, #24]
	mov	r4, #4
	lsl	r3, r1, #1
	neg	r4, r4
	add	r6, r3, r2
	str	r4, [sp, #4]
	mov	r3, #0
	mov	r7, sl
	mov	r9, r3
	mov	fp, r3
.L27:
	ldrh	r0, [r6]
	bl	Ability_GetData
	mov	r5, r0
	ldrb	r3, [r5, #2]
	cmp	r3, #4
	beq	.L29
	mov	r1, sl
	ldr	r2, .L56+20
	ldr	r0, [r1]
	mov	r1, r3
	mov	r3, #0
	add	r1, r1, r2
	str	r3, [sp]
	mov	r2, #15
	mov	r3, r9
	bl	UiWindow_SetTilemapEntry
.L29:
	ldrb	r3, [r5, #8]
	cmp	r3, #255
	bne	.L30
	mov	r3, #11
	b	.L31
.L30:
	sub	r3, r3, #1
.L31:
	mov	r4, #0
	ldr	r0, [r7]
	mov	r1, #16
	mov	r2, r9
	str	r4, [sp]
	bl	UiWindow_DrawThreeTileColumn
	ldrh	r3, [r6]
	ldr	r1, .L56+24
	ldr	r2, [sp, #4]
	and	r3, r3, r1
	ldr	r0, [r7]
	mov	r1, #0
	bl	Ui_CreateOutputFromResourceSlot
	ldrh	r2, [r6]
	ldrh	r3, .L56
	and	r3, r3, r2
	cmp	r3, #0
	beq	.L32
	mov	r0, #4
	bl	UiWork_SetParamNibble
	b	.L33
.L32:
	ldrh	r3, .L56+4
	and	r3, r3, r2
	cmp	r3, #0
	beq	.L34
	mov	r0, #2
	bl	UiWork_SetParamNibble
	b	.L33
.L57:
	.align	2, 0
.L56:
	.word	32768
	.word	16384
	.word	50336648
	.word	2222
	.word	2221
	.word	20481
	.word	16383
.L34:
	mov	r0, #15
	bl	UiWork_SetParamNibble
.L33:
	ldrh	r3, [r6]
	ldr	r0, .L58
	and	r0, r0, r3
	ldr	r3, .L58+4
	ldr	r1, [r7]
	add	r0, r0, r3
	mov	r2, #16
	mov	r3, fp
	bl	UiText_DrawCharacterAtOffset
	mov	r2, #0
	ldr	r0, [r7]
	mov	r3, r9
	str	r2, [sp]
	ldr	r1, .L58+8
	mov	r2, #11
	bl	UiWindow_SetTilemapEntry
	mov	r3, #0
	ldr	r0, [r7]
	ldr	r1, .L58+12
	str	r3, [sp]
	mov	r2, #12
	mov	r3, r9
	bl	UiWindow_SetTilemapEntry
	ldrh	r0, [r6]
	bl	Ability_GetData
	mov	r4, fp
	ldr	r2, [r7]
	ldrb	r0, [r0, #9]
	mov	r1, #2
	mov	r3, #104
	str	r4, [sp]
	bl	UiText_DrawNumberInWindow
	mov	r2, #16
	ldr	r3, [sp, #4]
	ldr	r4, [sp, #16]
	mov	r1, #2
	add	fp, fp, r2
	ldr	r2, [sp, #8]
	add	r9, r9, r1
	add	r3, r3, #16
	add	r4, r4, #1
	mov	r1, #1
	str	r3, [sp, #4]
	str	r4, [sp, #16]
	add	r6, r6, #2
	add	r8, r8, r1
	cmp	r4, r2
	bge	.L25
	ldr	r3, [sp, #20]
	cmp	r8, r3
	bge	.LCB629
	b	.L27	@long jump
.LCB629:
.L25:
	ldr	r3, [sp, #68]
	cmp	r3, #0
	beq	.L37
	mov	r0, #4
	bl	UiWork_SetParamNibble
	mov	r4, sl
	ldr	r1, [r4]
	ldr	r0, .L58+16
	mov	r2, #32
	mov	r3, #80
	bl	UiText_DrawCharacterAtOffset
	mov	r1, #1
	str	r1, [sp, #12]
.L37:
	ldr	r3, [sp, #64]
	cmp	r3, #0
	beq	.L38
	mov	r0, #2
	bl	UiWork_SetParamNibble
	ldr	r4, [sp, #12]
	mov	r2, sl
	lsl	r3, r4, #3
	ldr	r1, [r2]
	ldr	r0, .L58+20
	add	r3, r3, #80
	mov	r2, #32
	bl	UiText_DrawCharacterAtOffset
	ldr	r1, [sp, #12]
	add	r1, r1, #1
	str	r1, [sp, #12]
.L38:
	ldr	r2, [sp, #12]
	cmp	r2, #0
	bne	.L39
	mov	r3, sl
	ldr	r1, [r3]
	ldr	r0, .L58+24
	mov	r2, #32
	mov	r3, #80
	bl	UiText_DrawCharacterAtOffset
.L39:
	mov	r0, #15
	bl	UiWork_SetParamNibble
	mov	r0, #15
	bl	UiWork_SetParamNibble
	mov	r3, #10
	mov	r4, sl
	ldr	r0, [r4]
	mov	r1, #0
	str	r3, [sp]
	mov	r2, #10
	mov	r3, #19
	bl	UiWindow_DrawDividerLine
.L22:
	ldr	r1, [sp, #60]
	cmp	r1, #0
	beq	.LCB702
	b	.L40	@long jump
.LCB702:
	ldr	r2, [sp, #52]
	ldr	r5, .L58+28
	add	r2, r2, r5
	ldrb	r0, [r2]
	ldr	r6, .L58+32
	mov	r3, sl
	ldr	r1, [r3]
	mov	r8, r2
	add	r0, r0, r6
	mov	r2, #0
	mov	r3, #0
	bl	UiText_DrawCharacterAtOffset
	ldr	r4, [sp, #56]
	add	r5, r4, r5
	ldrb	r0, [r5]
	mov	r2, sl
	ldr	r1, [r2]
	mov	r3, #0
	mov	r2, #80
	add	r0, r0, r6
	bl	UiText_DrawCharacterAtOffset
	mov	r3, r8
	ldrb	r2, [r3]
	ldrb	r3, [r5]
	cmp	r2, r3
	beq	.L41
	ldr	r2, [sp, #60]
	mov	r4, sl
	ldr	r0, [r4]
	ldr	r1, .L58+36
	str	r2, [sp]
	mov	r3, #0
	mov	r2, #9
	bl	UiWindow_SetTilemapEntry
	b	.L42
.L41:
	mov	r3, sl
	ldr	r4, [sp, #60]
	ldr	r0, [r3]
	ldr	r1, .L58+40
	mov	r2, #9
	mov	r3, #0
	str	r4, [sp]
	bl	UiWindow_SetTilemapEntry
.L42:
	ldr	r3, [sp, #56]
	ldr	r0, [sp, #48]
	mov	r2, #56
	ldrsh	r1, [r3, r2]
	bl	UiText_FormatNumberToHalfwords
	ldr	r6, [sp, #48]
	mov	r4, sl
	add	r6, r6, #14
	ldr	r1, [r4]
	mov	r2, #11
	mov	r3, #1
	mov	r0, r6
	bl	UiText_RenderWideStringInWindow
	ldr	r2, [sp, #56]
	ldr	r4, [sp, #44]
	mov	r1, #56
	ldrsh	r3, [r2, r1]
	cmp	r3, r4
	beq	.L43
	mov	r2, #0
	cmp	r3, r4
	ble	.L44
	mov	r2, #1
.L44:
	add	r1, sp, #76
	mov	r9, r1
	mov	r0, #80
	mov	r1, #14
	bl	Func_08022a7c
.L43:
	ldr	r3, [sp, #56]
	ldr	r0, [sp, #48]
	mov	r2, #58
	ldrsh	r1, [r3, r2]
	bl	UiText_FormatNumberToHalfwords
	mov	r4, sl
	ldr	r1, [r4]
	mov	r2, #11
	mov	r3, #2
	mov	r0, r6
	bl	UiText_RenderWideStringInWindow
	ldr	r2, [sp, #56]
	ldr	r4, [sp, #40]
	mov	r1, #58
	ldrsh	r3, [r2, r1]
	cmp	r3, r4
	beq	.L45
	mov	r2, #0
	cmp	r3, r4
	ble	.L46
	mov	r2, #1
.L46:
	add	r1, sp, #76
	mov	r9, r1
	mov	r0, #80
	mov	r1, #22
	bl	Func_08022a7c
.L45:
	ldr	r2, [sp, #56]
	ldr	r0, [sp, #48]
	ldrh	r1, [r2, #60]
	bl	UiText_FormatNumberToHalfwords
	mov	r3, sl
	ldr	r1, [r3]
	mov	r0, r6
	mov	r3, #3
	mov	r2, #11
	bl	UiText_RenderWideStringInWindow
	ldr	r4, [sp, #56]
	ldr	r1, [sp, #36]
	ldrh	r3, [r4, #60]
	cmp	r3, r1
	beq	.L47
	mov	r2, #0
	cmp	r3, r1
	ble	.L48
	mov	r2, #1
.L48:
	add	r3, sp, #76
	mov	r9, r3
	mov	r0, #80
	mov	r1, #30
	bl	Func_08022a7c
.L47:
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #48]
	ldrh	r1, [r4, #62]
	bl	UiText_FormatNumberToHalfwords
	mov	r2, sl
	ldr	r1, [r2]
	mov	r3, #4
	mov	r0, r6
	mov	r2, #11
	bl	UiText_RenderWideStringInWindow
	ldr	r4, [sp, #56]
	ldr	r1, [sp, #32]
	ldrh	r3, [r4, #62]
	cmp	r3, r1
	beq	.L49
	mov	r2, #0
	cmp	r3, r1
	ble	.L50
	mov	r2, #1
.L50:
	add	r3, sp, #76
	mov	r9, r3
	mov	r0, #80
	mov	r1, #38
	bl	Func_08022a7c
.L49:
	ldr	r5, [sp, #56]
	add	r5, r5, #64
	ldrh	r1, [r5]
	ldr	r0, [sp, #48]
	bl	UiText_FormatNumberToHalfwords
	mov	r4, sl
	ldr	r1, [r4]
	mov	r3, #5
	mov	r0, r6
	mov	r2, #11
	bl	UiText_RenderWideStringInWindow
	ldrh	r3, [r5]
	ldr	r1, [sp, #28]
	cmp	r3, r1
	beq	.L51
	mov	r2, #0
	cmp	r3, r1
	ble	.L52
	mov	r2, #1
.L52:
	add	r3, sp, #76
	mov	r9, r3
	mov	r0, #80
	mov	r1, #46
	bl	Func_08022a7c
.L51:
	ldr	r5, [sp, #56]
	add	r5, r5, #66
	ldrb	r1, [r5]
	ldr	r0, [sp, #48]
	bl	UiText_FormatNumberToHalfwords
	ldr	r0, [sp, #48]
	mov	r4, sl
	ldr	r1, [r4]
	mov	r3, #6
	add	r0, r0, #16
	mov	r2, #12
	bl	UiText_RenderWideStringInWindow
	ldr	r3, [sp, #52]
	add	r3, r3, #66
	ldrb	r1, [r5]
	ldrb	r3, [r3]
	cmp	r1, r3
	beq	.L40
	mov	r2, #0
	cmp	r1, r3
	bls	.L54
	mov	r2, #1
.L54:
	add	r1, sp, #76
	mov	r9, r1
	mov	r0, #80
	mov	r1, #54
	bl	Func_08022a7c
.L40:
	mov	r2, #166
	lsl	r2, r2, #1
	ldr	r3, .L58+44
	ldr	r0, [sp, #56]
	ldr	r1, [sp, #52]
	bl	_call_via_r3
	ldr	r0, [sp, #24]
	bl	Sys_Free
	ldr	r0, [sp, #52]
	bl	Sys_Free
	ldr	r0, [sp, #48]
	bl	Sys_Free
	mov	r2, sl
	ldr	r0, [r2]
.L2:
	add	sp, sp, #76
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
.L59:
	.align	2, 0
.L58:
	.word	16383
	.word	Value_00000333
	.word	61471
	.word	61470
	.word	2978
	.word	2979
	.word	2984
	.word	297
	.word	1857
	.word	63272
	.word	63273
	.word	50336648
.Lfe2:
	.size	 DjinnMenu_ShowChangePreview,.Lfe2-DjinnMenu_ShowChangePreview
