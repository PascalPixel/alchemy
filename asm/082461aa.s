@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_082461aa
	.thumb_func
Func_082461aa:
	push	{r0, r1, r3, r4, lr}
	lsls	r2, r7, #16
	movs	r6, #45
	asrs	r2, r3, #28
	adds	r4, r3, #7
	adds	r7, r1, #5
	cmp	r4, #129
	pop	{r1, r5, pc}
