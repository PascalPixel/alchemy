.syntax unified
	.thumb
	.global ColorBuffer_ScaleNonzeroThreeQuarters
	.thumb_func
ColorBuffer_ScaleNonzeroThreeQuarters:
	push	{r5, r6, r7, lr}
	add	r4, pc, #16
	ldmia	r4, {r4, r5, r7}
	sub	sp, #96
	mov	r6, sp
	stmia	r4!, {r5, r6, r7}
	bl	_call_via_sp
	add	sp, #96
	pop	{r5, r6, r7, pc}
	.4byte	0x040000d4
	.4byte	ColorBuffer_ScaleNonzeroThreeQuartersKernelCode
	.4byte	0x84000018
	.global ColorBuffer_HalveNonzero
	.thumb_func
ColorBuffer_HalveNonzero:
	push	{r5, r6, r7, lr}
	add	r4, pc, #16
	ldmia	r4, {r4, r5, r7}
	sub	sp, #96
	mov	r6, sp
	stmia	r4!, {r5, r6, r7}
	bl	_call_via_sp
	add	sp, #96
	pop	{r5, r6, r7, pc}
	.4byte	0x040000d4
	.4byte	ColorBuffer_HalveNonzeroKernelCode
	.4byte	0x84000018
	.global ColorBuffer_BrightenPartial
	.thumb_func
ColorBuffer_BrightenPartial:
	push	{r5, r6, r7, lr}
	add	r4, pc, #16
	ldmia	r4, {r4, r5, r7}
	sub	sp, #216
	mov	r6, sp
	stmia	r4!, {r5, r6, r7}
	bl	_call_via_sp
	add	sp, #216
	pop	{r5, r6, r7, pc}
	.4byte	0x040000d4
	.4byte	ColorBuffer_BrightenPartialKernelCode
	.4byte	0x84000036
	.global ColorBuffer_DarkenPartial
	.thumb_func
ColorBuffer_DarkenPartial:
	push	{r5, r6, r7, lr}
	add	r4, pc, #16
	ldmia	r4, {r4, r5, r7}
	sub	sp, #212
	mov	r6, sp
	stmia	r4!, {r5, r6, r7}
	bl	_call_via_sp
	add	sp, #212
	pop	{r5, r6, r7, pc}
	.4byte	0x040000d4
	.4byte	ColorBuffer_DarkenPartialKernelCode
	.4byte	0x84000035
