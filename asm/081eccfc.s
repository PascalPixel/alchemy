@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_081eccfc
	.thumb_func
Func_081eccfc:
	push	{r1, r4, lr}
	lsrs	r3, r3, #28
	subs	r0, #28
	asrs	r5, r7, #25
	cmp	r4, #9
	pop	{r0, r1, r2, r3, r4, r5, r6, pc}
