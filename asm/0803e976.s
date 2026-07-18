@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_0803e976
	.thumb_func
Func_0803e976:
	push	{r0, r1, r2, r4, r5, r6, lr}
	stmia	r1!, {r0, r3, r4, r6}
	pop	{r3, r7, pc}
