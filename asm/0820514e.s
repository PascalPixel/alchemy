@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_0820514e
	.thumb_func
Func_0820514e:
	push	{r0, r1, r2, r3, r4, r5, r6, r7, lr}
	cmp	r2, #23
	subs	r7, r2, #4
	ldrsh	r2, [r3, r4]
	subs	r3, r7, #3
	udf	#163
	movs	r6, #39
	str	r7, [r2, #96]
	asrs	r1, r5, #11
	adds	r7, r7, r3
	asrs	r6, r2, #30
	pop	{r2, r3, r4, r5, r6, pc}
