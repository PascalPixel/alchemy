@ 合成ルーチンへ書き込まれる命令の雛形。関数ではない：r1 が読み出し元、
@ r9 が書き込み先、r5/r8/sl は作業用で、呼び出し規約を持たない。
.syntax unified
.text
	.arm
	.global SentouKouka_YomiGyaku
	.global SentouKouka_YomiJun
	.global SentouKouka_Yomi8Jun
	.global SentouKouka_Yomi8Gyaku
	.global SentouKouka_NuriJun
	.global SentouKouka_NuriGyaku
	.global SentouKouka_Nuri8Jun
	.global SentouKouka_Nuri8Gyaku
	.global SentouKouka_HikakuJun
	.global SentouKouka_HikakuGyaku
	.global SentouKouka_Hikaku4Jun
	.global SentouKouka_Hikaku4Gyaku
	.global SentouKouka_KasanJun
	.global SentouKouka_KasanGyaku
	.global SentouKouka_Mask
SentouKouka_YomiGyaku:
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
SentouKouka_YomiJun:
	ldrb	r5, [r1], #1
	strb	r5, [r9], #1
SentouKouka_Yomi8Jun:
	ldrb	r5, [r1], #1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #1
	strb	r5, [r9], #1
SentouKouka_Yomi8Gyaku:
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
	ldrb	r5, [r1], #-1
	strb	r5, [r9], #1
SentouKouka_NuriJun:
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9]
	add	r9, r9, #1
SentouKouka_NuriGyaku:
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9]
	add	r9, r9, #1
SentouKouka_Nuri8Jun:
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9]
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9, #1]
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9, #2]
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9, #3]
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9, #4]
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9, #5]
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9, #6]
	ldrb	r5, [r1], #1
	cmp	r5, #0
	strbne	r5, [r9, #7]
	add	r9, r9, #8
SentouKouka_Nuri8Gyaku:
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9]
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9, #1]
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9, #2]
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9, #3]
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9, #4]
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9, #5]
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9, #6]
	ldrb	r5, [r1], #-1
	cmp	r5, #0
	strbne	r5, [r9, #7]
	add	r9, r9, #8
SentouKouka_HikakuJun:
	ldrb	r5, [r1], #1
	ldrb	r8, [r9], #1
	cmp	r5, r8
	strbgt	r5, [r9, #-1]
SentouKouka_HikakuGyaku:
	ldrb	r5, [r1], #-1
	ldrb	r8, [r9], #1
	cmp	r5, r8
	strbgt	r5, [r9, #-1]
SentouKouka_Hikaku4Jun:
	ldrb	r5, [r1], #1
	ldr	r8, [r9], #4
	and	sl, r8, #255
	cmp	r5, sl
	strbgt	r5, [r9, #-4]
	ldrb	r5, [r1], #1
	and	sl, r8, #65280
	cmp	r5, sl, lsr #8
	strbgt	r5, [r9, #-3]
	ldrb	r5, [r1], #1
	and	sl, r8, #16711680
	cmp	r5, sl, lsr #16
	strbgt	r5, [r9, #-2]
	ldrb	r5, [r1], #1
	cmp	r5, r8, lsr #24
	strbgt	r5, [r9, #-1]
SentouKouka_Hikaku4Gyaku:
	ldrb	r5, [r1], #-1
	ldr	r8, [r9], #4
	and	sl, r8, #255
	cmp	r5, sl
	strbgt	r5, [r9, #-4]
	ldrb	r5, [r1], #-1
	and	sl, r8, #65280
	cmp	r5, sl, lsr #8
	strbgt	r5, [r9, #-3]
	ldrb	r5, [r1], #-1
	and	sl, r8, #16711680
	cmp	r5, sl, lsr #16
	strbgt	r5, [r9, #-2]
	ldrb	r5, [r1], #-1
	cmp	r5, r8, lsr #24
	strbgt	r5, [r9, #-1]
SentouKouka_KasanJun:
	ldrb	r5, [r1], #1
	ldrb	r8, [r9], #1
	add	r5, r5, r8
	cmp	r5, #63
	movcs	r5, #63
	strb	r5, [r9, #-1]
SentouKouka_KasanGyaku:
	ldrb	r5, [r1], #-1
	ldrb	r8, [r9], #1
	add	r5, r5, r8
	cmp	r5, #63
	movcs	r5, #63
	strb	r5, [r9, #-1]
SentouKouka_Mask:
	mov	sl, #64
	orr	sl, sl, sl, lsl #8
	orr	sl, sl, sl, lsl #16
