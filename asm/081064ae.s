@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_081064ae
	.thumb_func
Func_081064ae:
	push	{r0, r1, r3, r5, r7, lr}
	stmia	r2!, {r0, r2, r3, r4, r5, r7}
	stmia	r0!, {r1, r6, r7}
	hlt	0x003d
	pop	{r1, r3, r4, r5, r7, pc}
