@ BIOS呼出しラッパ。r0を1としてソフトウェア割込み19を実行する。
@ 固定svc命令はCで表現不能なためアセンブリとして保持する。
.syntax unified
	.thumb
	.global Func_08006870
	.thumb_func
Func_08006870:
	movs	r0, #1
	svc	25
	bx	lr
