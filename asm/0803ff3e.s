@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_0803ff3e
	.thumb_func
Func_0803ff3e:
	push	{r0, r3, r7, lr}
	bmi.n	Func_0803ffd0
	pop	{r0, r2, r6, pc}
