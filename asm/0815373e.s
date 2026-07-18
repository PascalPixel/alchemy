@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_0815373e
	.thumb_func
Func_0815373e:
	push	{r1, r2, r6, r7, lr}
	strh	r5, [r2, #52]
	strh	r0, [r1, #60]
	ldrh	r0, [r1, #4]
	ldrh	r0, [r1, #4]
	ldrh	r0, [r1, #4]
	ldrh	r0, [r1, #4]
	ldrh	r0, [r1, #4]
	ldrh	r0, [r1, #4]
	ldrh	r0, [r1, #4]
	strh	r0, [r1, #60]
	strh	r0, [r1, #60]
	add	r3, sp, #584
	pop	{r1, r2, r4, r5, r7, pc}
