@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_080408e4
	.thumb_func
Func_080408e4:
	push	{r0, r1, r2, r3, r6, lr}
	ldrb	r1, [r7, r4]
	strh	r3, [r7, #4]
	pop	{r0, r1, r2, r5, r7, pc}
