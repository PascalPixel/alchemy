@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_0811108c
	.thumb_func
Func_0811108c:
	push	{r4, r5, r7, lr}
	push	{r1, r2, r4, r5, r7, lr}
	pop	{r3, r4, r5, r7, pc}
