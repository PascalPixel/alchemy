@ 音響処理用IWRAM転送領域内の64バイト零埋め処理。
@ 四回のstmiaを展開し、スタックを使わずr4をipへ退避する速度優先処理。
@ 通常のCコンパイラ出力ではないためアセンブリとして保持する。
.syntax unified
	.thumb
	.global Func_080f9a18
	.thumb_func
Func_080f9a18:
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
