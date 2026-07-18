@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.tsでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_0808bc9c
	.thumb_func
Func_0808bc9c:
	ldr	r3, [pc, #128]
	movs	r2, #182
	ldr	r1, [r3, #0]
	lsls	r2, r2, #1
	adds	r3, r1, r2
	adds	r2, #2
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	adds	r3, r1, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	adds	r2, #2
	adds	r0, r0, r3
	adds	r3, r1, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	adds	r2, #2
	adds	r0, r0, r3
	adds	r3, r1, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	adds	r2, #2
	adds	r0, r0, r3
	adds	r3, r1, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	adds	r2, #2
	adds	r0, r0, r3
	adds	r3, r1, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	adds	r2, #2
	adds	r0, r0, r3
	adds	r3, r1, r2
	movs	r4, #0
	ldrsh	r2, [r3, r4]
	movs	r4, #189
	lsls	r4, r4, #1
	adds	r3, r1, r4
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	adds	r0, r0, r2
	adds	r0, r0, r3
	adds	r0, r0, r2
	movs	r2, #190
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	adds	r2, #2
	adds	r0, r0, r3
	adds	r3, r1, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	adds	r2, #2
	adds	r0, r0, r3
	adds	r3, r1, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	adds	r2, #2
	adds	r0, r0, r3
	adds	r3, r1, r2
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	adds	r0, r0, r3
	bx	lr
	.4byte 0x03001ebc
