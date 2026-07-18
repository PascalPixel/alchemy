@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_08135260
	.thumb_func
Func_08135260:
	push	{r0, r1, r4, r5, r7, lr}
	stmia	r3!, {r0, r1, r3, r4, r5, r7}
	pop	{r0, r1, r2, r6, r7, pc}
