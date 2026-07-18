@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_081a2da0
	.thumb_func
Func_081a2da0:
	push	{r2, r3, r4, lr}
	pop	{r0, r4, pc}
