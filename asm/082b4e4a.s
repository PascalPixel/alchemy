@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_082b4e4a
	.thumb_func
Func_082b4e4a:
	push	{r0, r3, r4, r6, r7, lr}
	b.n	Func_082b4d2e
	pop	{r1, r2, r3, r5, r6, r7, pc}
