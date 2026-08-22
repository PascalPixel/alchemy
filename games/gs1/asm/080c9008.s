@ 遠距離呼出し用モード間ベニア。固定のldr r4,[pc]; bx r4は
@ リンカが遠距離処理へ到達するために生成する。Cでは表現不能。
.syntax unified
	.thumb
	.global Func_080c9008
	.thumb_func
Func_080c9008:
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x080e3a3d
