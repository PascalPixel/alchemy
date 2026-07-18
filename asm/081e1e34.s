@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_081e1e34
	.thumb_func
Func_081e1e34:
	push	{r1, r3, r4, r6, r7, lr}
	adds	r4, #211
	cmp	r5, #94
	movs	r4, #36
	adds	r4, #94
	lsrs	r4, r4, #20
	pop	{r0, r1, r3, r4, r5, r6, r7, pc}
