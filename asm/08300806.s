@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_08300806
	.thumb_func
Func_08300806:
	push	{r0, r1, r4, lr}
	movs	r1, #191
	pop	{r4, pc}
