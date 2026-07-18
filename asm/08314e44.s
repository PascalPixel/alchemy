@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_08314e44
	.thumb_func
Func_08314e44:
	push	{r0, r1, r3, r4, lr}
	ble.n	Func_08314ea6
	asrs	r1, r3, #26
	tst	r3, r3
	ldr	r5, [r5, #56]
	push	{r1, r3, r4, r6, r7}
	cmp	r6, #65
	ldmia	r1, {r0, r1}
	bpl.n	Func_08314e6c
	subs	r1, #225
	subs	r3, r5, r7
	pop	{r2, r5, r6, pc}
