@ 音響列の36語転送と共有バイト検査入口。
@ 080f9a98と080f9a9aは末尾を共有するため一個のソース単位として保持する。
.syntax unified
	.thumb
	.global Func_080f9a80
	.thumb_func
Func_080f9a80:
	mov	ip, lr
	movs	r1, #36
	ldr	r2, .Lsource
.Lloop:
	ldr	r3, [r2, #0]
	bl	Func_080f9a9a
	stmia	r0!, {r3}
	adds	r2, #4
	subs	r1, #1
	bgt.n	.Lloop
	bx	ip
	.2byte 0

	.global Func_080f9a98
	.thumb_func
Func_080f9a98:
	ldrb	r3, [r2, #0]
	.global Func_080f9a9a
	.thumb_func
Func_080f9a9a:
	push	{r0}
	lsrs	r0, r2, #25
	bne.n	.Lreturn
	ldr	r0, .Lsource
	cmp	r2, r0
	bcc.n	.Lclear
	lsrs	r0, r2, #14
	beq.n	.Lreturn
.Lclear:
	movs	r3, #0
.Lreturn:
	pop	{r0}
	bx	lr
	.align 2
.Lsource:
	.4byte 0x080fb7a0

	.global Func_080f9ab4
	.thumb_func
Func_080f9ab4:
	ldr	r2, [r1, #64]
	adds	r3, r2, #1
	str	r3, [r1, #64]
	ldrb	r3, [r2, #0]
	b.n	Func_080f9a9a
	.2byte 0
