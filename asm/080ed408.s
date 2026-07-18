@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080ed408
	.thumb_func
Func_080ed408:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	mov	r9, r1
	ldr	r1, [sp, #48]
	mov	sl, r1
	str	r2, [sp, #12]
	mov	r2, sl
	mov	r8, r3
	movs	r1, #3
	cmp	r2, #3
	bne.n	.L0
	movs	r1, #6
.L0:
	movs	r3, #12
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #4
	bne.n	.L1
	adds	r1, #3
.L1:
	cmp	r3, #8
	bne.n	.L2
	adds	r1, #4
.L2:
	cmp	r3, #12
	bne.n	.L3
	adds	r1, #3
.L3:
	cmp	r3, #0
	bne.n	.L4
	adds	r1, #1
.L4:
	movs	r3, #2
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L5
	adds	r1, #7
.L5:
	movs	r3, #1
	mov	r2, r8
	ands	r3, r2
	adds	r1, #2
	cmp	r3, #0
	bne.n	.L6
	adds	r1, #1
.L6:
	cmp	r3, #0
	beq.n	.L7
	adds	r1, #2
	adds	r1, #2
	adds	r1, #5
.L7:
	movs	r3, #1
	mov	r2, r8
	ands	r3, r2
	adds	r1, #9
	cmp	r3, #0
	bne.n	.L8
	adds	r1, #1
.L8:
	adds	r1, #9
	cmp	r3, #0
	bne.n	.L9
	adds	r1, #1
.L9:
	mov	r3, sl
	adds	r1, #3
	cmp	r3, #1
	beq.n	.L10
	cmp	r3, #1
	bcc.n	.L11
	cmp	r3, #2
	beq.n	.L10
	cmp	r3, #3
	beq.n	.L12
.L11:
	adds	r1, #2
	b.n	.L13
.L10:
	adds	r1, #4
	b.n	.L13
.L12:
	adds	r1, #6
.L13:
	movs	r3, #1
	mov	r2, r8
	ands	r3, r2
	adds	r1, #3
	cmp	r3, #0
	bne.n	.L14
	adds	r1, #2
.L14:
	mov	r3, sl
	adds	r1, #2
	cmp	r3, #1
	beq.n	.L15
	cmp	r3, #1
	bcc.n	.L16
	cmp	r3, #2
	beq.n	.L17
	cmp	r3, #3
	beq.n	.L18
.L16:
	movs	r3, #0
.L19:
	adds	r3, #1
	adds	r1, #2
	cmp	r3, #7
	ble.n	.L19
	b.n	.L20
.L15:
	adds	r1, #25
	b.n	.L20
.L17:
	adds	r1, #32
	b.n	.L20
.L18:
	adds	r1, #28
.L20:
	movs	r3, #1
	mov	r2, r8
	ands	r3, r2
	adds	r1, #1
	cmp	r3, #0
	bne.n	.L21
	adds	r1, #2
.L21:
	mov	r3, sl
	adds	r1, #5
	cmp	r3, #1
	beq.n	.L22
	cmp	r3, #1
	bcc.n	.L23
	cmp	r3, #2
	beq.n	.L22
	cmp	r3, #3
	beq.n	.L24
.L23:
	adds	r1, #2
	b.n	.L25
.L22:
	adds	r1, #4
	b.n	.L25
.L24:
	adds	r1, #6
.L25:
	adds	r1, #8
	lsls	r1, r1, #2
	bl	Func_080048b0
	ldr	r6, [pc, #796]
	adds	r5, r0, #0
	ldr	r3, [pc, #796]
	ldr	r2, [pc, #796]
	adds	r0, r6, #0
	adds	r1, r5, #0
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #12
	adds	r6, #12
	mov	r1, sl
	cmp	r1, #3
	bne.n	.L26
	ldr	r0, [pc, #780]
	adds	r1, r5, #0
	ldr	r2, [pc, #772]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #12
.L26:
	movs	r4, #12
	mov	r2, r8
	ands	r4, r2
	cmp	r4, #4
	bne.n	.L27
	ldr	r3, [pc, #752]
	adds	r0, r6, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #748]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #12
.L27:
	adds	r6, #12
	cmp	r4, #8
	bne.n	.L28
	ldr	r3, [pc, #732]
	adds	r0, r6, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #736]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #16
.L28:
	adds	r6, #16
	cmp	r4, #12
	bne.n	.L29
	ldr	r3, [pc, #712]
	adds	r0, r6, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #708]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #12
.L29:
	adds	r6, #12
	cmp	r4, #0
	bne.n	.L30
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
.L30:
	movs	r3, #2
	mov	r1, r8
	ands	r3, r1
	adds	r6, #4
	cmp	r3, #0
	beq.n	.L31
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
	ldr	r3, [r6, #4]
	stmia	r5!, {r3}
	movs	r3, #8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L32
	ldr	r3, [r6, #8]
	b.n	.L33
.L32:
	ldr	r3, [r6, #12]
.L33:
	stmia	r5!, {r3}
	ldr	r3, [r6, #16]
	stmia	r5!, {r3}
	ldr	r3, [r6, #20]
	stmia	r5!, {r3}
	movs	r2, #1
	ldr	r3, [sp, #12]
	lsls	r2, r3
	ldr	r3, [r6, #24]
	adds	r3, r3, r2
	stmia	r5!, {r3}
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	stmia	r5!, {r3}
.L31:
	adds	r6, #32
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	str	r5, [sp, #8]
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	movs	r2, #1
	mov	r1, r8
	ands	r1, r2
	cmp	r1, #0
	bne.n	.L34
	mov	r3, r9
	lsls	r2, r3
	ldr	r3, [r6, #0]
	adds	r3, r3, r2
	subs	r3, #1
	stmia	r5!, {r3}
.L34:
	adds	r6, #4
	cmp	r1, #0
	beq.n	.L35
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
	ldr	r3, [r6, #4]
	stmia	r5!, {r3}
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L36
	ldr	r3, [r6, #8]
	stmia	r5!, {r3}
	ldr	r3, [r6, #12]
	b.n	.L37
.L36:
	ldr	r3, [r6, #16]
	stmia	r5!, {r3}
	ldr	r3, [r6, #20]
.L37:
	stmia	r5!, {r3}
	ldr	r3, [r6, #24]
	stmia	r5!, {r3}
	ldr	r3, [r6, #28]
	stmia	r5!, {r3}
	ldr	r3, [pc, #560]
	mov	r2, r9
	lsls	r1, r2, #1
	ldrh	r2, [r3, r1]
	ldr	r3, [r6, #32]
	adds	r3, r3, r2
	stmia	r5!, {r3}
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L38
	ldr	r3, [r6, #36]
	b.n	.L39
.L38:
	ldr	r3, [r6, #40]
.L39:
	stmia	r5!, {r3}
	ldr	r3, [pc, #528]
	ldrh	r2, [r3, r1]
	ldr	r3, [r6, #44]
	adds	r3, r3, r2
	stmia	r5!, {r3}
.L35:
	adds	r6, #48
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	mov	fp, r5
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	ldr	r3, [pc, #488]
	ldr	r2, [pc, #508]
	adds	r0, r6, #0
	adds	r1, r5, #0
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #24
	adds	r6, #24
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	movs	r7, #1
	mov	r4, r8
	ands	r4, r7
	cmp	r4, #0
	bne.n	.L40
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
.L40:
	adds	r6, #4
	ldr	r3, [pc, #452]
	ldr	r2, [pc, #476]
	adds	r0, r6, #0
	adds	r1, r5, #0
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #20
	adds	r6, #20
	str	r5, [sp, #4]
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
	ldr	r3, [sp, #12]
	subs	r3, #3
	adds	r2, r7, #0
	lsls	r2, r3
	ldr	r3, [r6, #4]
	adds	r3, r3, r2
	subs	r3, #1
	stmia	r5!, {r3}
	mov	r2, r9
	ldr	r3, [r6, #8]
	subs	r2, #3
	lsls	r2, r2, #7
	adds	r3, r3, r2
	stmia	r5!, {r3}
	ldr	r3, [r6, #12]
	stmia	r5!, {r3}
	adds	r6, #16
	cmp	r4, #0
	bne.n	.L41
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
.L41:
	adds	r6, #4
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	mov	ip, r5
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L42
	ldr	r3, [r6, #0]
	b.n	.L43
.L42:
	ldr	r3, [r6, #4]
.L43:
	stmia	r5!, {r3}
	mov	r2, sl
	adds	r6, #8
	adds	r4, r5, #0
	cmp	r2, #1
	beq.n	.L44
	cmp	r2, #1
	bcc.n	.L45
	cmp	r2, #2
	beq.n	.L46
	cmp	r2, #3
	beq.n	.L47
.L45:
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L48
	ldr	r3, [pc, #328]
	ldr	r0, [pc, #352]
	b.n	.L49
.L48:
	ldr	r3, [pc, #320]
	ldr	r0, [pc, #352]
.L49:
	adds	r1, r5, #0
	ldr	r2, [pc, #352]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #8
	b.n	.L50
.L44:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L51
	ldr	r3, [pc, #296]
	ldr	r0, [pc, #332]
	b.n	.L52
.L51:
	ldr	r3, [pc, #288]
	ldr	r0, [pc, #332]
	b.n	.L52
.L46:
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L53
	ldr	r3, [pc, #272]
	ldr	r0, [pc, #320]
	b.n	.L52
.L53:
	ldr	r3, [pc, #268]
	ldr	r0, [pc, #316]
.L52:
	adds	r1, r5, #0
	ldr	r2, [pc, #272]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #16
	b.n	.L50
.L47:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L54
	ldr	r3, [pc, #240]
	ldr	r0, [pc, #296]
	b.n	.L55
.L54:
	ldr	r3, [pc, #236]
	ldr	r0, [pc, #292]
.L55:
	adds	r1, r5, #0
	ldr	r2, [pc, #248]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #24
.L50:
	adds	r6, #16
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	subs	r3, r4, r5
	subs	r3, #8
	ldr	r1, [pc, #276]
	ldmia	r6!, {r2}
	lsrs	r3, r3, #2
	ands	r3, r1
	adds	r2, r2, r3
	stmia	r5!, {r2}
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	movs	r3, #1
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L56
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
	ldr	r3, [r6, #4]
	stmia	r5!, {r3}
.L56:
	adds	r6, #8
	mov	r2, ip
	subs	r3, r5, r2
	subs	r3, #8
	ldr	r2, [r2, #0]
	lsrs	r3, r3, #2
	ands	r3, r1
	orrs	r2, r3
	mov	r3, ip
	str	r2, [r3, #0]
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	mov	ip, r5
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	str	r5, [sp, #0]
	mov	r1, sl
	cmp	r1, #1
	beq.n	.L57
	cmp	r1, #1
	bcc.n	.L58
	cmp	r1, #2
	beq.n	.L59
	cmp	r1, #3
	beq.n	.L60
.L58:
	ldr	r3, [pc, #156]
	ldr	r1, [pc, #156]
	movs	r7, #4
	mov	r2, r8
	movs	r4, #0
	ands	r7, r2
	mov	r9, r3
	mov	lr, r1
.L63:
	cmp	r7, #0
	beq.n	.L61
	ldr	r3, [pc, #108]
	mov	r0, r9
	b.n	.L62
.L61:
	ldr	r3, [pc, #100]
	mov	r0, lr
.L62:
	adds	r1, r5, #0
	ldr	r2, [pc, #132]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #8
	adds	r4, #1
	cmp	r4, #7
	ble.n	.L63
	b.n	.L64
.L57:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L65
	ldr	r3, [pc, #68]
	ldr	r0, [pc, #136]
	b.n	.L66
.L65:
	ldr	r3, [pc, #64]
	ldr	r0, [pc, #132]
.L66:
	adds	r1, r5, #0
	ldr	r2, [pc, #132]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #100
	b.n	.L64
.L59:
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L67
	ldr	r4, [pc, #116]
	b.n	.L68
.L67:
	ldr	r4, [pc, #116]
.L68:
	ldr	r3, [pc, #32]
	adds	r0, r4, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #112]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #64
	adds	r0, r4, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #100]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #64
	b.n	.L64
	movs	r0, r0
	.4byte 0x080edcc4
	.4byte 0x040000d4
	.4byte 0x84000003
	.4byte 0x080edcb8
	.4byte 0x84000004
	.4byte 0x080ef034
	.4byte 0x84000006
	.4byte 0x84000005
	.4byte 0x080edaf0
	.4byte 0x080edaf8
	.4byte 0x84000002
	.4byte 0x080edb10
	.4byte 0x080edb00
	.4byte 0x080edbf8
	.4byte 0x080edbe8
	.4byte 0x080edca0
	.4byte 0x080edc88
	.4byte 0x00ffffff
	.4byte 0x080edb84
	.4byte 0x080edb20
	.4byte 0x84000019
	.4byte 0x080edc48
	.4byte 0x080edc08
	.4byte 0x84000010
.L60:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L69
	ldr	r4, [pc, #408]
	b.n	.L70
.L69:
	ldr	r4, [pc, #408]
.L70:
	ldr	r3, [pc, #408]
	adds	r0, r4, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #408]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #56
	adds	r0, r4, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #396]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #56
.L64:
	adds	r6, #16
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	movs	r3, #1
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L71
	ldr	r3, [r6, #0]
	stmia	r5!, {r3}
	ldr	r3, [r6, #4]
	stmia	r5!, {r3}
.L71:
	adds	r6, #8
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	ldr	r2, [sp, #0]
	subs	r3, r2, r5
	ldr	r1, [pc, #356]
	subs	r3, #8
	ldmia	r6!, {r2}
	lsrs	r3, r3, #2
	ands	r3, r1
	adds	r2, r2, r3
	stmia	r5!, {r2}
	mov	r2, ip
	subs	r3, r5, r2
	subs	r3, #8
	ldr	r2, [r2, #0]
	lsrs	r3, r3, #2
	ands	r3, r1
	orrs	r2, r3
	mov	r3, ip
	str	r2, [r3, #0]
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	mov	ip, r5
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L72
	ldr	r3, [r6, #0]
	b.n	.L73
.L72:
	ldr	r3, [r6, #4]
.L73:
	stmia	r5!, {r3}
	mov	r2, sl
	adds	r6, #8
	adds	r4, r5, #0
	cmp	r2, #1
	beq.n	.L74
	cmp	r2, #1
	bcc.n	.L75
	cmp	r2, #2
	beq.n	.L76
	cmp	r2, #3
	beq.n	.L77
.L75:
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L78
	ldr	r3, [pc, #256]
	ldr	r0, [pc, #268]
	b.n	.L79
.L78:
	ldr	r3, [pc, #252]
	ldr	r0, [pc, #264]
.L79:
	adds	r1, r5, #0
	ldr	r2, [pc, #264]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #8
	b.n	.L80
.L74:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L81
	ldr	r3, [pc, #224]
	ldr	r0, [pc, #248]
	b.n	.L82
.L81:
	ldr	r3, [pc, #220]
	ldr	r0, [pc, #244]
	b.n	.L82
.L76:
	movs	r3, #4
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L83
	ldr	r3, [pc, #204]
	ldr	r0, [pc, #232]
	b.n	.L82
.L83:
	ldr	r3, [pc, #196]
	ldr	r0, [pc, #232]
.L82:
	adds	r1, r5, #0
	ldr	r2, [pc, #232]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #16
	b.n	.L80
.L77:
	movs	r3, #4
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L84
	ldr	r3, [pc, #172]
	ldr	r0, [pc, #212]
	b.n	.L85
.L84:
	ldr	r3, [pc, #164]
	ldr	r0, [pc, #212]
.L85:
	adds	r1, r5, #0
	ldr	r2, [pc, #212]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #24
.L80:
	adds	r6, #16
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	subs	r3, r4, r5
	subs	r3, #8
	ldr	r4, [pc, #148]
	ldmia	r6!, {r2}
	lsrs	r3, r3, #2
	ands	r3, r4
	adds	r2, r2, r3
	stmia	r5!, {r2}
	mov	r1, ip
	subs	r3, r5, r1
	subs	r3, #8
	ldr	r2, [r1, #0]
	lsrs	r3, r3, #2
	ands	r3, r4
	orrs	r2, r3
	str	r2, [r1, #0]
	ldr	r3, [pc, #112]
	ldr	r2, [pc, #168]
	adds	r0, r6, #0
	adds	r1, r5, #0
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #12
	adds	r6, #12
	ldr	r2, [sp, #4]
	subs	r3, r2, r5
	subs	r3, #8
	ldmia	r6!, {r2}
	lsrs	r3, r3, #2
	ands	r3, r4
	adds	r2, r2, r3
	stmia	r5!, {r2}
	ldr	r1, [sp, #8]
	subs	r3, r5, r1
	subs	r3, #8
	ldr	r2, [r1, #0]
	lsrs	r3, r3, #2
	ands	r3, r4
	orrs	r2, r3
	str	r2, [r1, #0]
	mov	r2, fp
	subs	r3, r5, r2
	subs	r3, #8
	ldr	r2, [r2, #0]
	lsrs	r3, r3, #2
	ands	r3, r4
	orrs	r2, r3
	mov	r3, fp
	str	r2, [r3, #0]
	ldmia	r6!, {r3}
	stmia	r5!, {r3}
	ldmia	r6!, {r3}
	str	r3, [r5, #0]
	ldr	r3, [pc, #100]
	eors	r3, r6
	negs	r2, r3
	orrs	r2, r3
	lsrs	r2, r2, #31
	movs	r0, #1
	subs	r0, r0, r2
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x080eefdc
	.4byte 0x080eefa4
	.4byte 0x040000d4
	.4byte 0x8400000e
	.4byte 0x00ffffff
	.4byte 0x080edaf0
	.4byte 0x080edaf8
	.4byte 0x84000002
	.4byte 0x080edb10
	.4byte 0x080edb00
	.4byte 0x080edbf8
	.4byte 0x080edbe8
	.4byte 0x84000004
	.4byte 0x080edca0
	.4byte 0x080edc88
	.4byte 0x84000006
	.4byte 0x84000003
	.4byte 0x080ede48
