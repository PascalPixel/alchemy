@ 遠距離呼出し用長分岐ベニア。固定のldr r4,[pc]; bx r4は
@ リンカが遠距離処理へ到達するために生成する。Cでは表現不能。
.syntax unified
	.thumb
	.global Func_080b5000
	.thumb_func
Func_080b5000:
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x080b56e1
