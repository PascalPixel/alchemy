@ Why assembly: four stmia r0! of zero registers have no paired load/copy;
@ r4 is saved through ip.
@ Why MPlayJumpTableCopy to ply_patt are assembly:
@ MPlayJumpTableCopy keeps the return address with mov ip, lr across a
@ bl and returns with bx ip, never saving lr; the helper after it returns
@ its byte in r3 and preserves r0 with push {r0} / pop {r0}, and ply_goto
@ calls it there (bl .L_081c1734), a linkage no procedure-call standard
@ produces; ply_patt branches into ply_goto's body (b.n .L_081c175c) and
@ tail-branches to ply_fine.
@ Why ply_rept to ply_port are assembly:
@ each calls the helper that returns its byte in r3 (bl .L_081c1750,
@ .L_081c1736, or .L_081c1752 with the address already in r2); ply_prio to
@ ply_port keep
@ the return address with mov ip, lr across that bl and return with bx ip,
@ never saving lr, and ply_rept branches into ply_goto's body
@ (b.n .L_081c175e).
@ Why m4aSoundVSync to call_r3 are assembly:
@ DMA timer branches on carry from lsls: each bcc consumes the carry of
@ lsls r1, r1, #7, a DMA channel's repeat bit. The player after it saves
@ with push {r4, r5, r6, r7} without lr, after pop {r0} has taken back the r0
@ pushed with lr; r11 (fp) is an ordinary register, the mask of the tracks
@ still playing (mov fp, r4), not a frame pointer; it returns through
@ pop {r3} into call_r3's bx r3, which its callback call also
@ enters with bl.
@ Why assembly: caller supplies the channel in r4 and track in r5; the routine
@ reads them (ldrb r1, [r4, #18]; ldrb r0, [r5, #16]) without an argument move
@ or save, a linkage no procedure-call standard produces.
@ Why assembly: push saves r4/r5 without lr; MusicPlayer2000 library ply_endtie.
@ Why assembly: MusicPlayer2000's ld_r3_tp_adr_i_unchecked takes track in r1
@ and returns the decoded byte in r3 while updating r2/r3, rather than a
@ standard r0 return.
@ Why ply_lfos to ply_mod are assembly:
@ Both nested-call entries return through ip saved from lr rather than a
@ procedure-standard return: mov ip, lr before the bl, bx ip after it.
	.section .text,"ax",%progbits
	.balign 2
.syntax unified
	.thumb
	.global SoundMainBTM
	.thumb_func
SoundMainBTM:
	mov	ip, r4
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	mov	r4, ip
	bx	lr
	.2byte	0x0000

	.section .text,"ax",%progbits
	.balign 2
.syntax unified
	.thumb
	.global RealClearChain
	.thumb_func
RealClearChain:
	ldr	r3, [r0, #44]
	cmp	r3, #0
	beq.n	.L_081c16ea
	ldr	r1, [r0, #52]
	ldr	r2, [r0, #48]
	cmp	r2, #0
	beq.n	.L_081c16de
	str	r1, [r2, #52]
	b.n	.L_081c16e0
.L_081c16de:
	str	r1, [r3, #32]
.L_081c16e0:
	cmp	r1, #0
	beq.n	.L_081c16e6
	str	r2, [r1, #48]
.L_081c16e6:
	movs	r1, #0
	str	r1, [r0, #44]
.L_081c16ea:
	bx	lr
	.global ply_fine
	.thumb_func
ply_fine:
	push	{r4, r5, lr}
	adds	r5, r1, #0
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L_081c1710
.L_081c16f6:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L_081c1704
	movs	r0, #64
	orrs	r1, r0
	strb	r1, [r4, #0]
.L_081c1704:
	adds	r0, r4, #0
	bl	RealClearChain
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L_081c16f6
.L_081c1710:
	movs	r0, #0
	strb	r0, [r5, #0]
	pop	{r4, r5}
	pop	{r0}
	bx	r0
AlchemyUncredited_081c171a:
	.2byte	0x0000
AlchemyUncreditedEnd_081c171a:

	.section .text,"ax",%progbits
	.balign 2
.syntax unified
	.thumb
	.global MPlayJumpTableCopy
	.thumb_func
MPlayJumpTableCopy:
	mov	ip, lr
	movs	r1, #36
	ldr	r2, [pc, #40]
.L_081c1722:
	ldr	r3, [r2, #0]
	bl	.L_081c1736
	stmia	r0!, {r3}
	adds	r2, #4
	subs	r1, #1
	bgt.n	.L_081c1722
	bx	ip
AlchemyUncredited_081c1732:
	.2byte	0x0000
.L_081c1734:
AlchemyUncreditedEnd_081c1732:
	ldrb	r3, [r2, #0]
.L_081c1736:
	push	{r0}
	lsrs	r0, r2, #25
	bne.n	.L_081c1748
	ldr	r0, [pc, #12]
	cmp	r2, r0
	bcc.n	.L_081c1746
	lsrs	r0, r2, #14
	beq.n	.L_081c1748
.L_081c1746:
	movs	r3, #0
.L_081c1748:
	pop	{r0}
	bx	lr
	.4byte	gMPlayJumpTableTemplate
.L_081c1750:
	ldr	r2, [r1, #64]
.L_081c1752:
	adds	r3, r2, #1
	str	r3, [r1, #64]
	ldrb	r3, [r2, #0]
	b.n	.L_081c1736
AlchemyUncredited_081c175a:
	.2byte	0x0000
.L_081c175c:
AlchemyUncreditedEnd_081c175a:
	.global ply_goto
	.thumb_func
ply_goto:
	push	{lr}
.L_081c175e:
	ldr	r2, [r1, #64]
	ldrb	r0, [r2, #3]
	lsls	r0, r0, #8
	ldrb	r3, [r2, #2]
	orrs	r0, r3
	lsls	r0, r0, #8
	ldrb	r3, [r2, #1]
	orrs	r0, r3
	lsls	r0, r0, #8
	bl	.L_081c1734
	orrs	r0, r3
	str	r0, [r1, #64]
	pop	{r0}
	bx	r0
	.global ply_patt
	.thumb_func
ply_patt:
	ldrb	r2, [r1, #2]
	cmp	r2, #3
	bcs.n	.L_081c1794
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldr	r2, [r1, #64]
	adds	r2, #4
	str	r2, [r3, #68]
	ldrb	r2, [r1, #2]
	adds	r2, #1
	strb	r2, [r1, #2]
	b.n	.L_081c175c
.L_081c1794:
	b.n	ply_fine
AlchemyUncredited_081c1796:
	.2byte	0x0000
AlchemyUncreditedEnd_081c1796:
	.global ply_pend
	.thumb_func
ply_pend:
	ldrb	r2, [r1, #2]
	cmp	r2, #0
	beq.n	.L_081c17aa
	subs	r2, #1
	strb	r2, [r1, #2]
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldr	r2, [r3, #68]
	str	r2, [r1, #64]
.L_081c17aa:
	bx	lr
	.global ply_rept
	.thumb_func
ply_rept:
	push	{lr}
	ldr	r2, [r1, #64]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_081c17bc
	adds	r2, #1
	str	r2, [r1, #64]
	b.n	.L_081c175e
.L_081c17bc:
	ldrb	r3, [r1, #3]
	adds	r3, #1
	strb	r3, [r1, #3]
	mov	ip, r3
	bl	.L_081c1750
	cmp	ip, r3
	bcs.n	.L_081c17ce
	b.n	.L_081c175e
.L_081c17ce:
	movs	r3, #0
	strb	r3, [r1, #3]
	adds	r2, #5
	str	r2, [r1, #64]
	pop	{r0}
	bx	r0
AlchemyUncredited_081c17da:
	.2byte	0x0000
AlchemyUncreditedEnd_081c17da:
	.global ply_prio
	.thumb_func
ply_prio:
	mov	ip, lr
	bl	.L_081c1750
	strb	r3, [r1, #29]
	bx	ip
AlchemyUncredited_081c17e6:
	.2byte	0x0000
AlchemyUncreditedEnd_081c17e6:
	.global ply_tempo
	.thumb_func
ply_tempo:
	mov	ip, lr
	bl	.L_081c1750
	lsls	r3, r3, #1
	strh	r3, [r0, #28]
	ldrh	r2, [r0, #30]
	muls	r3, r2
	lsrs	r3, r3, #8
	strh	r3, [r0, #32]
	bx	ip
	.global ply_keysh
	.thumb_func
ply_keysh:
	mov	ip, lr
	bl	.L_081c1750
	strb	r3, [r1, #10]
	ldrb	r3, [r1, #0]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip
AlchemyUncredited_081c180e:
	.2byte	0x0000
AlchemyUncreditedEnd_081c180e:
	.global ply_voice
	.thumb_func
ply_voice:
	mov	ip, lr
	ldr	r2, [r1, #64]
	ldrb	r3, [r2, #0]
	adds	r2, #1
	str	r2, [r1, #64]
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsls	r2, r2, #2
	ldr	r3, [r0, #48]
	adds	r2, r2, r3
	ldr	r3, [r2, #0]
	bl	.L_081c1736
	str	r3, [r1, #36]
	ldr	r3, [r2, #4]
	bl	.L_081c1736
	str	r3, [r1, #40]
	ldr	r3, [r2, #8]
	bl	.L_081c1736
	str	r3, [r1, #44]
	bx	ip
AlchemyUncredited_081c183e:
	.2byte	0x0000
AlchemyUncreditedEnd_081c183e:
	.global ply_vol
	.thumb_func
ply_vol:
	mov	ip, lr
	bl	.L_081c1750
	strb	r3, [r1, #18]
	ldrb	r3, [r1, #0]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip
AlchemyUncredited_081c1852:
	.2byte	0x0000
AlchemyUncreditedEnd_081c1852:
	.global ply_pan
	.thumb_func
ply_pan:
	mov	ip, lr
	bl	.L_081c1750
	subs	r3, #64
	strb	r3, [r1, #20]
	ldrb	r3, [r1, #0]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip
	.global ply_bend
	.thumb_func
ply_bend:
	mov	ip, lr
	bl	.L_081c1750
	subs	r3, #64
	strb	r3, [r1, #14]
	ldrb	r3, [r1, #0]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip
	.global ply_bendr
	.thumb_func
ply_bendr:
	mov	ip, lr
	bl	.L_081c1750
	strb	r3, [r1, #15]
	ldrb	r3, [r1, #0]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip
AlchemyUncredited_081c188e:
	.2byte	0x0000
AlchemyUncreditedEnd_081c188e:
	.global ply_lfodl
	.thumb_func
ply_lfodl:
	mov	ip, lr
	bl	.L_081c1750
	strb	r3, [r1, #27]
	bx	ip
AlchemyUncredited_081c189a:
	.2byte	0x0000
AlchemyUncreditedEnd_081c189a:
	.global ply_modt
	.thumb_func
ply_modt:
	mov	ip, lr
	bl	.L_081c1750
	ldrb	r0, [r1, #24]
	cmp	r0, r3
	beq.n	.L_081c18b2
	strb	r3, [r1, #24]
	ldrb	r3, [r1, #0]
	movs	r2, #15
	orrs	r3, r2
	strb	r3, [r1, #0]
.L_081c18b2:
	bx	ip
	.global ply_tune
	.thumb_func
ply_tune:
	mov	ip, lr
	bl	.L_081c1750
	subs	r3, #64
	strb	r3, [r1, #12]
	ldrb	r3, [r1, #0]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip
	.global ply_port
	.thumb_func
ply_port:
	mov	ip, lr
	ldr	r2, [r1, #64]
	ldrb	r3, [r2, #0]
	adds	r2, #1
	ldr	r0, [pc, #8]
	adds	r0, r0, r3
	bl	.L_081c1752
	strb	r3, [r0, #0]
	bx	ip
	.4byte	0x04000060

	.section .text,"ax",%progbits
	.balign 2
.syntax unified
	.thumb
	.global m4aSoundVSync
	.thumb_func
m4aSoundVSync:
	ldr	r0, [pc, #680]
	ldr	r0, [r0, #0]
	ldr	r2, [pc, #680]
	ldr	r3, [r0, #0]
	subs	r3, r3, r2
	cmp	r3, #1
	bhi.n	.L_081c1920
	ldrb	r1, [r0, #4]
	subs	r1, #1
	strb	r1, [r0, #4]
	bgt.n	.L_081c1920
	ldrb	r1, [r0, #11]
	strb	r1, [r0, #4]
	ldr	r2, [pc, #40]
	ldr	r1, [r2, #8]
	lsls	r1, r1, #7
	bcc.n	.L_081c1906
	ldr	r1, [pc, #36]
	str	r1, [r2, #8]
.L_081c1906:
	ldr	r1, [r2, #20]
	lsls	r1, r1, #7
	bcc.n	.L_081c1910
	ldr	r1, [pc, #24]
	str	r1, [r2, #20]
.L_081c1910:
	movs	r1, #4
	lsls	r1, r1, #8
	strh	r1, [r2, #10]
	strh	r1, [r2, #22]
	movs	r1, #182
	lsls	r1, r1, #8
	strh	r1, [r2, #10]
	strh	r1, [r2, #22]
.L_081c1920:
	bx	lr
AlchemyUncredited_081c1922:
	.2byte	0x0000
AlchemyUncreditedEnd_081c1922:
	.4byte	0x040000bc
	.4byte	0x84400004
	ldr	r2, [pc, #608]
	ldr	r3, [r0, #52]
	cmp	r2, r3
	beq.n	.L_081c1936
	bx	lr
.L_081c1936:
	adds	r3, #1
	str	r3, [r0, #52]
	push	{r0, lr}
	ldr	r3, [r0, #56]
	cmp	r3, #0
	beq.n	.L_081c1948
	ldr	r0, [r0, #60]
	bl	.L_081c1b84
.L_081c1948:
	pop	{r0}
	push	{r4, r5, r6, r7}
	mov	r4, r8
	mov	r5, r9
	mov	r6, sl
	mov	r7, fp
	push	{r4, r5, r6, r7}
	adds	r7, r0, #0
	ldr	r0, [r7, #4]
	cmp	r0, #0
	bge.n	.L_081c1960
	b.n	.L_081c1b74
.L_081c1960:
	ldr	r0, [pc, #552]
	ldr	r0, [r0, #0]
	mov	r8, r0
	adds	r0, r7, #0
	bl	FadeOutBody
	ldr	r0, [r7, #4]
	cmp	r0, #0
	bge.n	.L_081c1974
	b.n	.L_081c1b74
.L_081c1974:
	ldrh	r0, [r7, #34]
	ldrh	r1, [r7, #32]
	adds	r0, r0, r1
	b.n	.L_081c1ac4
.L_081c197c:
	ldrb	r6, [r7, #8]
	ldr	r5, [r7, #44]
	movs	r3, #1
	movs	r4, #0
.L_081c1984:
	ldrb	r0, [r5, #0]
	movs	r1, #128
	tst	r1, r0
	bne.n	.L_081c198e
	b.n	.L_081c1aa0
.L_081c198e:
	mov	sl, r3
	orrs	r4, r3
	mov	fp, r4
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L_081c19c2
.L_081c199a:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L_081c19b6
	ldrb	r0, [r4, #16]
	cmp	r0, #0
	beq.n	.L_081c19bc
	subs	r0, #1
	strb	r0, [r4, #16]
	bne.n	.L_081c19bc
	movs	r0, #64
	orrs	r1, r0
	strb	r1, [r4, #0]
	b.n	.L_081c19bc
.L_081c19b6:
	adds	r0, r4, #0
	bl	ClearChain
.L_081c19bc:
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L_081c199a
.L_081c19c2:
	ldrb	r3, [r5, #0]
	movs	r0, #64
	tst	r0, r3
	beq.n	.L_081c1a40
	adds	r0, r5, #0
	bl	Clear64byte
	movs	r0, #128
	strb	r0, [r5, #0]
	movs	r0, #2
	strb	r0, [r5, #15]
	movs	r0, #64
	strb	r0, [r5, #19]
	movs	r0, #22
	strb	r0, [r5, #25]
	movs	r0, #1
	adds	r1, r5, #6
	strb	r0, [r1, #30]
	b.n	.L_081c1a40
.L_081c19e8:
	ldr	r2, [r5, #64]
	ldrb	r1, [r2, #0]
	cmp	r1, #128
	bcs.n	.L_081c19f4
	ldrb	r1, [r5, #7]
	b.n	.L_081c19fe
.L_081c19f4:
	adds	r2, #1
	str	r2, [r5, #64]
	cmp	r1, #189
	bcc.n	.L_081c19fe
	strb	r1, [r5, #7]
.L_081c19fe:
	cmp	r1, #207
	bcc.n	.L_081c1a14
	mov	r0, r8
	ldr	r3, [r0, #56]
	adds	r0, r1, #0
	subs	r0, #207
	adds	r1, r7, #0
	adds	r2, r5, #0
	bl	.L_081c1b84
	b.n	.L_081c1a40
.L_081c1a14:
	cmp	r1, #176
	bls.n	.L_081c1a36
	adds	r0, r1, #0
	subs	r0, #177
	strb	r0, [r7, #10]
	mov	r3, r8
	ldr	r3, [r3, #52]
	lsls	r0, r0, #2
	ldr	r3, [r3, r0]
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	.L_081c1b84
	ldrb	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L_081c1a9c
	b.n	.L_081c1a40
.L_081c1a36:
	ldr	r0, [pc, #336]
	subs	r1, #128
	adds	r1, r1, r0
	ldrb	r0, [r1, #0]
	strb	r0, [r5, #1]
.L_081c1a40:
	ldrb	r0, [r5, #1]
	cmp	r0, #0
	beq.n	.L_081c19e8
	subs	r0, #1
	strb	r0, [r5, #1]
	ldrb	r1, [r5, #25]
	cmp	r1, #0
	beq.n	.L_081c1a9c
	ldrb	r0, [r5, #23]
	cmp	r0, #0
	beq.n	.L_081c1a9c
	ldrb	r0, [r5, #28]
	cmp	r0, #0
	beq.n	.L_081c1a62
	subs	r0, #1
	strb	r0, [r5, #28]
	b.n	.L_081c1a9c
.L_081c1a62:
	ldrb	r0, [r5, #26]
	adds	r0, r0, r1
	strb	r0, [r5, #26]
	adds	r1, r0, #0
	subs	r0, #64
	lsls	r0, r0, #24
	bpl.n	.L_081c1a76
	lsls	r2, r1, #24
	asrs	r2, r2, #24
	b.n	.L_081c1a7a
.L_081c1a76:
	movs	r0, #128
	subs	r2, r0, r1
.L_081c1a7a:
	ldrb	r0, [r5, #23]
	muls	r0, r2
	asrs	r2, r0, #6
	ldrb	r0, [r5, #22]
	eors	r0, r2
	lsls	r0, r0, #24
	beq.n	.L_081c1a9c
	strb	r2, [r5, #22]
	ldrb	r0, [r5, #0]
	ldrb	r1, [r5, #24]
	cmp	r1, #0
	bne.n	.L_081c1a96
	movs	r1, #12
	b.n	.L_081c1a98
.L_081c1a96:
	movs	r1, #3
.L_081c1a98:
	orrs	r0, r1
	strb	r0, [r5, #0]
.L_081c1a9c:
	mov	r3, sl
	mov	r4, fp
.L_081c1aa0:
	subs	r6, #1
	ble.n	.L_081c1aac
	movs	r0, #80
	adds	r5, r5, r0
	lsls	r3, r3, #1
	b.n	.L_081c1984
.L_081c1aac:
	ldr	r0, [r7, #12]
	adds	r0, #1
	str	r0, [r7, #12]
	cmp	r4, #0
	bne.n	.L_081c1abe
	movs	r0, #128
	lsls	r0, r0, #24
	str	r0, [r7, #4]
	b.n	.L_081c1b74
.L_081c1abe:
	str	r4, [r7, #4]
	ldrh	r0, [r7, #34]
	subs	r0, #150
.L_081c1ac4:
	strh	r0, [r7, #34]
	cmp	r0, #150
	bcc.n	.L_081c1acc
	b.n	.L_081c197c
.L_081c1acc:
	ldrb	r2, [r7, #8]
	ldr	r5, [r7, #44]
.L_081c1ad0:
	ldrb	r0, [r5, #0]
	movs	r1, #128
	tst	r1, r0
	beq.n	.L_081c1b6a
	movs	r1, #15
	tst	r1, r0
	beq.n	.L_081c1b6a
	mov	r9, r2
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	TrkVolPitSet
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L_081c1b60
.L_081c1aee:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	bne.n	.L_081c1afe
	adds	r0, r4, #0
	bl	ClearChain
	b.n	.L_081c1b5a
.L_081c1afe:
	ldrb	r0, [r4, #1]
	movs	r6, #7
	ands	r6, r0
	ldrb	r3, [r5, #0]
	movs	r0, #3
	tst	r0, r3
	beq.n	.L_081c1b1c
	bl	ChnVolSetAsm
	cmp	r6, #0
	beq.n	.L_081c1b1c
	ldrb	r0, [r4, #29]
	movs	r1, #1
	orrs	r0, r1
	strb	r0, [r4, #29]
.L_081c1b1c:
	ldrb	r3, [r5, #0]
	movs	r0, #12
	tst	r0, r3
	beq.n	.L_081c1b5a
	ldrb	r1, [r4, #8]
	movs	r0, #8
	ldrsb	r0, [r5, r0]
	adds	r2, r1, r0
	bpl.n	.L_081c1b30
	movs	r2, #0
.L_081c1b30:
	cmp	r6, #0
	beq.n	.L_081c1b4e
	mov	r0, r8
	ldr	r3, [r0, #48]
	adds	r1, r2, #0
	ldrb	r2, [r5, #9]
	adds	r0, r6, #0
	bl	.L_081c1b84
	str	r0, [r4, #32]
	ldrb	r0, [r4, #29]
	movs	r1, #2
	orrs	r0, r1
	strb	r0, [r4, #29]
	b.n	.L_081c1b5a
.L_081c1b4e:
	adds	r1, r2, #0
	ldrb	r2, [r5, #9]
	ldr	r0, [r4, #36]
	bl	MidiKeyToFreq
	str	r0, [r4, #32]
.L_081c1b5a:
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L_081c1aee
.L_081c1b60:
	ldrb	r0, [r5, #0]
	movs	r1, #240
	ands	r0, r1
	strb	r0, [r5, #0]
	mov	r2, r9
.L_081c1b6a:
	subs	r2, #1
	ble.n	.L_081c1b74
	movs	r0, #80
	adds	r5, r5, r0
	bgt.n	.L_081c1ad0
.L_081c1b74:
	ldr	r0, [pc, #24]
	str	r0, [r7, #52]
	pop	{r0, r1, r2, r3, r4, r5, r6, r7}
	mov	r8, r0
	mov	r9, r1
	mov	sl, r2
	mov	fp, r3
	pop	{r3}
.L_081c1b84:
	.global call_r3
call_r3:
	bx	r3
AlchemyUncredited_081c1b86:
	.2byte	0x0000
AlchemyUncreditedEnd_081c1b86:
	.4byte	gClockTable
	.4byte	0x03007ff0
	.4byte	0x68736d53

	.section .text,"ax",%progbits
	.balign 2
.syntax unified
	.thumb
	.global TrackStop
	.thumb_func
TrackStop:
	push	{r4, r5, r6, lr}
	adds	r5, r1, #0
	ldrb	r1, [r5, #0]
	movs	r0, #128
	tst	r0, r1
	beq.n	.L_081c1bcc
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L_081c1bca
	movs	r6, #0
.L_081c1ba8:
	ldrb	r0, [r4, #0]
	cmp	r0, #0
	beq.n	.L_081c1bc2
	ldrb	r0, [r4, #1]
	movs	r3, #7
	ands	r0, r3
	beq.n	.L_081c1bc0
	ldr	r3, [pc, #28]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #44]
	bl	call_r3
.L_081c1bc0:
	strb	r6, [r4, #0]
.L_081c1bc2:
	str	r6, [r4, #44]
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L_081c1ba8
.L_081c1bca:
	str	r4, [r5, #32]
.L_081c1bcc:
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
AlchemyUncredited_081c1bd2:
	.2byte	0x0000
AlchemyUncreditedEnd_081c1bd2:
	.4byte	0x03007ff0

	.section .text,"ax",%progbits
	.balign 2
.syntax unified
	.thumb
	.global ChnVolSetAsm
	.thumb_func
ChnVolSetAsm:
	ldrb	r1, [r4, #18]
	movs	r0, #20
	ldrsb	r2, [r4, r0]
	movs	r3, #128
	adds	r3, r3, r2
	muls	r3, r1
	ldrb	r0, [r5, #16]
	muls	r0, r3
	asrs	r0, r0, #14
	cmp	r0, #255
	bls.n	.L_081c1bf0
	movs	r0, #255
.L_081c1bf0:
	strb	r0, [r4, #2]
	movs	r3, #127
	subs	r3, r3, r2
	muls	r3, r1
	ldrb	r0, [r5, #17]
	muls	r0, r3
	asrs	r0, r0, #14
	cmp	r0, #255
	bls.n	.L_081c1c04
	movs	r0, #255
.L_081c1c04:
	strb	r0, [r4, #3]
	bx	lr

	.section .text,"ax",%progbits
	.balign 2
.syntax unified
	.thumb
	.global MusicTrack_HandleNoteUnreferenced
	.thumb_func
MusicTrack_HandleNoteUnreferenced:
	push	{r4, r5, r6, r7, lr}
	mov	r4, r8
	mov	r5, r9
	mov	r6, sl
	mov	r7, fp
	push	{r4, r5, r6, r7}
	sub	sp, #24
	str	r1, [sp, #0]
	adds	r5, r2, #0
	ldr	r1, [pc, #484]
	ldr	r1, [r1, #0]
	str	r1, [sp, #4]
	ldr	r1, [pc, #480]
	adds	r0, r0, r1
	ldrb	r0, [r0, #0]
	strb	r0, [r5, #4]
	ldr	r3, [r5, #64]
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L_081c1c4e
	strb	r0, [r5, #5]
	adds	r3, #1
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L_081c1c4c
	strb	r0, [r5, #6]
	adds	r3, #1
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L_081c1c4c
	ldrb	r1, [r5, #4]
	adds	r1, r1, r0
	strb	r1, [r5, #4]
	adds	r3, #1
.L_081c1c4c:
	str	r3, [r5, #64]
.L_081c1c4e:
	movs	r0, #0
	str	r0, [sp, #20]
	adds	r4, r5, #0
	adds	r4, #36
	ldrb	r2, [r4, #0]
	movs	r0, #192
	tst	r0, r2
	beq.n	.L_081c1ca0
	ldrb	r3, [r5, #5]
	movs	r0, #64
	tst	r0, r2
	beq.n	.L_081c1c6e
	ldr	r1, [r5, #44]
	adds	r1, r1, r3
	ldrb	r0, [r1, #0]
	b.n	.L_081c1c70
.L_081c1c6e:
	adds	r0, r3, #0
.L_081c1c70:
	lsls	r1, r0, #1
	adds	r1, r1, r0
	lsls	r1, r1, #2
	ldr	r0, [r5, #40]
	adds	r1, r1, r0
	mov	r9, r1
	mov	r6, r9
	ldrb	r1, [r6, #0]
	movs	r0, #192
	tst	r0, r1
	beq.n	.L_081c1c88
	b.n	.L_081c1dee
.L_081c1c88:
	movs	r0, #128
	tst	r0, r2
	beq.n	.L_081c1ca4
	ldrb	r1, [r6, #3]
	movs	r0, #128
	tst	r0, r1
	beq.n	.L_081c1c9c
	subs	r1, #192
	lsls	r1, r1, #1
	str	r1, [sp, #20]
.L_081c1c9c:
	ldrb	r3, [r6, #1]
	b.n	.L_081c1ca4
.L_081c1ca0:
	mov	r9, r4
	ldrb	r3, [r5, #5]
.L_081c1ca4:
	str	r3, [sp, #8]
	ldr	r6, [sp, #0]
	ldrb	r1, [r6, #9]
	ldrb	r0, [r5, #29]
	adds	r0, r0, r1
	cmp	r0, #255
	bls.n	.L_081c1cb4
	movs	r0, #255
.L_081c1cb4:
	str	r0, [sp, #16]
	mov	r6, r9
	ldrb	r0, [r6, #0]
	movs	r6, #7
	ands	r6, r0
	str	r6, [sp, #12]
	beq.n	.L_081c1cf4
	ldr	r0, [sp, #4]
	ldr	r4, [r0, #28]
	cmp	r4, #0
	bne.n	.L_081c1ccc
	b.n	.L_081c1dee
.L_081c1ccc:
	subs	r6, #1
	lsls	r0, r6, #6
	adds	r4, r4, r0
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L_081c1d48
	movs	r0, #64
	tst	r0, r1
	bne.n	.L_081c1d48
	ldrb	r1, [r4, #19]
	ldr	r0, [sp, #16]
	cmp	r1, r0
	bcc.n	.L_081c1d48
	beq.n	.L_081c1cec
	b.n	.L_081c1dee
.L_081c1cec:
	ldr	r0, [r4, #44]
	cmp	r0, r5
	bcs.n	.L_081c1d48
	b.n	.L_081c1dee
.L_081c1cf4:
	ldr	r6, [sp, #16]
	adds	r7, r5, #0
	movs	r2, #0
	mov	r8, r2
	ldr	r4, [sp, #4]
	ldrb	r3, [r4, #6]
	adds	r4, #80
.L_081c1d02:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L_081c1d48
	movs	r0, #64
	tst	r0, r1
	beq.n	.L_081c1d1c
	cmp	r2, #0
	bne.n	.L_081c1d20
	adds	r2, #1
	ldrb	r6, [r4, #19]
	ldr	r7, [r4, #44]
	b.n	.L_081c1d3a
.L_081c1d1c:
	cmp	r2, #0
	bne.n	.L_081c1d3c
.L_081c1d20:
	ldrb	r0, [r4, #19]
	cmp	r0, r6
	bcs.n	.L_081c1d2c
	adds	r6, r0, #0
	ldr	r7, [r4, #44]
	b.n	.L_081c1d3a
.L_081c1d2c:
	bhi.n	.L_081c1d3c
	ldr	r0, [r4, #44]
	cmp	r0, r7
	bls.n	.L_081c1d38
	adds	r7, r0, #0
	b.n	.L_081c1d3a
.L_081c1d38:
	bcc.n	.L_081c1d3c
.L_081c1d3a:
	mov	r8, r4
.L_081c1d3c:
	adds	r4, #64
	subs	r3, #1
	bgt.n	.L_081c1d02
	mov	r4, r8
	cmp	r4, #0
	beq.n	.L_081c1dee
.L_081c1d48:
	adds	r0, r4, #0
	bl	ClearChain
	movs	r1, #0
	str	r1, [r4, #48]
	ldr	r3, [r5, #32]
	str	r3, [r4, #52]
	cmp	r3, #0
	beq.n	.L_081c1d5c
	str	r4, [r3, #48]
.L_081c1d5c:
	str	r4, [r5, #32]
	str	r5, [r4, #44]
	ldrb	r0, [r5, #27]
	strb	r0, [r5, #28]
	cmp	r0, r1
	beq.n	.L_081c1d6e
	adds	r1, r5, #0
	bl	clear_modM
.L_081c1d6e:
	ldr	r0, [sp, #0]
	adds	r1, r5, #0
	bl	TrkVolPitSet
	ldr	r0, [r5, #4]
	str	r0, [r4, #16]
	ldr	r0, [sp, #16]
	strb	r0, [r4, #19]
	ldr	r0, [sp, #8]
	strb	r0, [r4, #8]
	ldr	r0, [sp, #20]
	strb	r0, [r4, #20]
	mov	r6, r9
	ldrb	r0, [r6, #0]
	strb	r0, [r4, #1]
	ldr	r7, [r6, #4]
	str	r7, [r4, #36]
	ldr	r0, [r6, #8]
	str	r0, [r4, #4]
	ldrh	r0, [r5, #30]
	strh	r0, [r4, #12]
	bl	ChnVolSetAsm
	ldrb	r1, [r4, #8]
	movs	r0, #8
	ldrsb	r0, [r5, r0]
	adds	r3, r1, r0
	bpl.n	.L_081c1da8
	movs	r3, #0
.L_081c1da8:
	ldr	r6, [sp, #12]
	cmp	r6, #0
	beq.n	.L_081c1dd6
	mov	r6, r9
	ldrb	r0, [r6, #2]
	strb	r0, [r4, #30]
	ldrb	r1, [r6, #3]
	movs	r0, #128
	tst	r0, r1
	bne.n	.L_081c1dc2
	movs	r0, #112
	tst	r0, r1
	bne.n	.L_081c1dc4
.L_081c1dc2:
	movs	r1, #8
.L_081c1dc4:
	strb	r1, [r4, #31]
	ldrb	r2, [r5, #9]
	adds	r1, r3, #0
	ldr	r0, [sp, #12]
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #48]
	bl	call_r3
	b.n	.L_081c1de0
.L_081c1dd6:
	ldrb	r2, [r5, #9]
	adds	r1, r3, #0
	adds	r0, r7, #0
	bl	MidiKeyToFreq
.L_081c1de0:
	str	r0, [r4, #32]
	movs	r0, #128
	strb	r0, [r4, #0]
	ldrb	r1, [r5, #0]
	movs	r0, #240
	ands	r0, r1
	strb	r0, [r5, #0]
.L_081c1dee:
	add	sp, #24
	pop	{r0, r1, r2, r3, r4, r5, r6, r7}
	mov	r8, r0
	mov	r9, r1
	mov	sl, r2
	mov	fp, r3
	pop	{r0}
	bx	r0
AlchemyUncredited_081c1dfe:
	.2byte	0x0000
AlchemyUncreditedEnd_081c1dfe:
	.4byte	0x03007ff0
	.4byte	gClockTable

	.section .text,"ax",%progbits
	.balign 2
.syntax unified
	.thumb
	.global ply_endtie
	.thumb_func
ply_endtie:
	push	{r4, r5}
	ldr	r2, [r1, #64]
	ldrb	r3, [r2, #0]
	cmp	r3, #128
	bcs.n	.L_081c1e1a
	strb	r3, [r1, #5]
	adds	r2, #1
	str	r2, [r1, #64]
	b.n	.L_081c1e1c
.L_081c1e1a:
	ldrb	r3, [r1, #5]
.L_081c1e1c:
	ldr	r1, [r1, #32]
	cmp	r1, #0
	beq.n	.L_081c1e44
	movs	r4, #131
	movs	r5, #64
.L_081c1e26:
	ldrb	r2, [r1, #0]
	tst	r2, r4
	beq.n	.L_081c1e3e
	tst	r2, r5
	bne.n	.L_081c1e3e
	ldrb	r0, [r1, #17]
	cmp	r0, r3
	bne.n	.L_081c1e3e
	movs	r0, #64
	orrs	r2, r0
	strb	r2, [r1, #0]
	b.n	.L_081c1e44
.L_081c1e3e:
	ldr	r1, [r1, #52]
	cmp	r1, #0
	bne.n	.L_081c1e26
.L_081c1e44:
	pop	{r4, r5}
	bx	lr

	.section .text,"ax",%progbits
	.balign 2
.syntax unified
	.thumb
	.global clear_modM
	.thumb_func
clear_modM:
	movs	r2, #0
	strb	r2, [r1, #22]
	strb	r2, [r1, #26]
	ldrb	r2, [r1, #24]
	cmp	r2, #0
	bne.n	.L_081c1e58
	movs	r2, #12
	b.n	.L_081c1e5a
.L_081c1e58:
	movs	r2, #3
.L_081c1e5a:
	ldrb	r3, [r1, #0]
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	lr
	.2byte	0x0000

	.section .text,"ax",%progbits
	.balign 2
.syntax unified
	.thumb
	.global ld_r3_tp_adr_i_unchecked
	.thumb_func
ld_r3_tp_adr_i_unchecked:
	ldr	r2, [r1, #64]
	adds	r3, r2, #1
	str	r3, [r1, #64]
	ldrb	r3, [r2, #0]
	bx	lr
	.2byte	0x0000

	.section .text,"ax",%progbits
	.balign 2
.syntax unified
	.thumb
	.global ply_lfos
	.thumb_func
ply_lfos:
	mov	ip, lr
	bl	ld_r3_tp_adr_i_unchecked
	strb	r3, [r1, #25]
	cmp	r3, #0
	bne.n	.L_081c1e80
	bl	clear_modM
.L_081c1e80:
	bx	ip
AlchemyUncredited_081c1e82:
	.2byte	0x0000
	.global ply_mod
	.thumb_func
ply_mod:
AlchemyUncreditedEnd_081c1e82:
	mov	ip, lr
	bl	ld_r3_tp_adr_i_unchecked
	strb	r3, [r1, #23]
	cmp	r3, #0
	bne.n	.L_081c1e94
	bl	clear_modM
.L_081c1e94:
	bx	ip
AlchemyUncredited_081c1e96:
	.2byte	0x0000
AlchemyUncreditedEnd_081c1e96:
