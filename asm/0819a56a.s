@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_0819a56a
	.thumb_func
Func_0819a56a:
	push	{r6, r7, lr}
	pop	{r1, r2, r4}
	stmia	r0!, {r1, r2, r4}
	adds	r2, #18
	asrs	r7, r3, #27
	ldrb	r2, [r6, #26]
	adds	r5, #18
	orrs	r1, r4
	asrs	r7, r7, #21
	pop	{r0, r1, r6, r7, pc}
