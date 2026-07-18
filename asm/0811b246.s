@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_0811b246
	.thumb_func
Func_0811b246:
	push	{r0, r3, r6, r7, lr}
	add	r4, sp, #688
	pop	{r2, r4, r5, r7, pc}
