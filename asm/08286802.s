@ 関数到達証拠のない符号列。サム命令としては復号できるが、
@ スタック規約と制御流が破綻するためデータ誤検出候補として保持する。
.syntax unified
	.thumb
	.global Func_08286802
	.thumb_func
Func_08286802:
	push	{r0, r6, r7, lr}
	pop	{r1, r4, r5, pc}
