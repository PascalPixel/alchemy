@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_0828e128
	.thumb_func
Func_0828e128:
	push	{r0, r2, r3, r4, lr}
	ldrb	r6, [r3, #4]
	asrs	r3, r7, #7
	pop	{r0, r1, r2, r7, pc}
