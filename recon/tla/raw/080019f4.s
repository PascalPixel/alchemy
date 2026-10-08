.syntax unified
	.thumb
	.global Render_BuildOamList
	.thumb_func
Render_BuildOamList:
	push	{lr}
	add	r1, pc, #16
	ldmia	r1, {r1, r2, r4}
	sub	sp, #0x140
	mov	r3, sp
	stmia	r1!, {r2, r3, r4}
	mov	lr, sp
	.inst.n	0xf800
	add	sp, #0x140
	pop	{pc}
	.4byte	0x040000d4
	.4byte	Render_BuildOamListKernelCode
	.4byte	0x84000050
