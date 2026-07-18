@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_08064278
	.thumb_func
Func_08064278:
	push	{r1, r2, r3, r5, lr}
	strb	r7, [r6, #12]
	strh	r1, [r1, #54]
	add	r2, sp, #808
	ldr	r2, [r6, #116]
	pop	{r0, r1, r2, r3, r4, r5, r7, pc}
