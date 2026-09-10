@ BIOS呼出しラッパ。ソフトウェア割込みはCでは表現不能。
@ 0x0b CpuSet: r0 転送元、r1 転送先、r2 制御語。
@ 0x19 SoundDriverVSyncOff: r0 を 0 にして呼び出す。
.syntax unified
	.thumb
	.global Func_08006864
	.thumb_func
Func_08006864:
	svc	11
	bx	lr
	.global Func_0800686a
	.thumb_func
Func_0800686a:
	movs	r0, #0
	svc	25
	bx	lr
