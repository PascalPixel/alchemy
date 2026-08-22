@ スタックへ転送して実行する符号付き16.16固定小数点三項内積。
@ 呼出し側が六個の成分を渡し、上位と下位を丸めず結合する。
.syntax unified
	.arm
	.global RelocatedQ16DotProduct_08007994
RelocatedQ16DotProduct_08007994:
	smull	r12, r0, r1, r0
	smlal	r12, r0, r2, r3
	ldmia	sp, {r2, r3}
	smlal	r12, r0, r2, r3
	lsl	r0, r0, #16
	orr	r0, r0, r12, lsr #16
	bx	lr
