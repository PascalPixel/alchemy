@ 分岐呼出しから到達し、リンクレジスタへ戻る独立関数。
@ 末尾の局所リテラルに加え、次関数末尾の共有リテラルを参照する。
.syntax unified
	.thumb
	.global Func_080f9c44
	.thumb_func
Func_080f9c44:
	ldr r0, [pc, #680]
	ldr r0, [r0, #0]
	ldr r2, [pc, #680]
	ldr r3, [r0, #0]
	subs r3, r3, r2
	cmp r3, #1
	bhi .L_080f9c84
	ldrb r1, [r0, #4]
	subs r1, #1
	strb r1, [r0, #4]
	bgt .L_080f9c84
	ldrb r1, [r0, #11]
	strb r1, [r0, #4]
	ldr r2, [pc, #40]
	ldr r1, [r2, #8]
	lsls r1, r1, #7
	bcc .L_080f9c6a
	ldr r1, [pc, #36]
	str r1, [r2, #8]
.L_080f9c6a:
	ldr r1, [r2, #20]
	lsls r1, r1, #7
	bcc .L_080f9c74
	ldr r1, [pc, #24]
	str r1, [r2, #20]
.L_080f9c74:
	movs r1, #4
	lsls r1, r1, #8
	strh r1, [r2, #10]
	strh r1, [r2, #22]
	movs r1, #182
	lsls r1, r1, #8
	strh r1, [r2, #10]
	strh r1, [r2, #22]
.L_080f9c84:
	bx lr
	.balign 4, 0
	.global LiteralPool_080f9c88
LiteralPool_080f9c88:
	.4byte 0x040000bc
	.4byte 0x84400004
