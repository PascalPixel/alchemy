@ Loaders that run the four non-zero and partial colour kernels from the
@ stack. Each reads {DMA3 register base, kernel address, control} from its
@ own PC-relative block with add r4, pc and ldmia r4, {r4, r5, r7}, reserves
@ exactly the kernel's size on the stack, has DMA3 copy the kernel there with
@ stmia r4!, {r5, r6, r7} and calls the copy through _call_via_sp. Neither
@ approved compiler emits an add rd, pc address form; both load constants
@ one ldr at a time.
@ credit: handwritten — Thumb stack-copy loaders of the colour-buffer kernels
.syntax unified
	.set _call_via_sp, 0x080178ac
	.set ColorBuffer_ScaleNonzeroThreeQuartersKernel, 0x08001d10
	.set ColorBuffer_HalveNonzeroKernel, 0x08001d70
	.set ColorBuffer_DarkenPartialKernel, 0x08001dd0
	.set ColorBuffer_BrightenPartialKernel, 0x08001ea4
	.thumb
	.set AlchemyLoadAddress, 0x08001f7c
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
	.4byte	ColorBuffer_ScaleNonzeroThreeQuartersKernel
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
	.4byte	ColorBuffer_HalveNonzeroKernel
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
	.4byte	ColorBuffer_BrightenPartialKernel
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
	.4byte	ColorBuffer_DarkenPartialKernel
	.4byte	0x84000035
