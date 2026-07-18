@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_0820aada
	.thumb_func
Func_0820aada:
	push	{r0, r1, r3, r4, r6, lr}
	pop	{r0, r1, r3, r4, pc}
