@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_08220924
	.thumb_func
Func_08220924:
	push	{r2, r5, r6, lr}
	str	r5, [r7, #72]
	add	pc, r4
