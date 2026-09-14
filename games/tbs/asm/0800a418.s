@ 呼出側の転送元と転送長により境界を確定した再配置アーム処理。
@ 承認済みコンパイラはサム専用のため構造化アセンブリで保持する。
.syntax unified
	.arm
	.global Func_0800a418
Func_0800a418:
	bics	r1, r1, #7
	bicsne	r2, r2, #7
	bxeq	lr
	push	{r5, r6, r7, r8, r9, sl, fp}
.L_0800a428:
	mov	ip, r1
.L_0800a42c:
	ldm	r0, {r4, r5}
	add	r0, r0, r1
	ldm	r0, {r6, r7}
	add	r0, r0, r1
	ldm	r0, {r8, r9}
	add	r0, r0, r1
	ldm	r0, {sl, fp}
	add	r0, r0, r1
	stmia	r3!, {r4, r5, r6, r7, r8, r9, sl, fp}
	ldm	r0, {r4, r5}
	add	r0, r0, r1
	ldm	r0, {r6, r7}
	add	r0, r0, r1
	ldm	r0, {r8, r9}
	add	r0, r0, r1
	ldm	r0!, {sl, fp}
	add	r0, r0, r1
	stmia	r3!, {r4, r5, r6, r7, r8, r9, sl, fp}
	subs	ip, ip, #8
	subgt	r0, r0, r1, lsl #3
	bgt	.L_0800a42c
	sub	r0, r0, r1
	subs	r2, r2, #8
	bgt	.L_0800a428
	pop	{r5, r6, r7, r8, r9, sl, fp}
	bx	lr
