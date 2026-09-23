@ Sound sequence command helpers. 0x080f9a80 copies the 36-entry command table
@ at 0x080fb7a0 through the range check at 0x080f9a9a; the other entries read
@ command bytes and words of the track in r1. They keep their return address
@ in ip (mov ip, lr ... bx ip), let 0x080f9a98 fall through into 0x080f9a9a,
@ hand results back in r3 and branch on flags from lsrs (0x080f9a9e). Neither
@ approved compiler emits any of these. Every TBS and TLA edition has this
@ code, differing only in the relocated table address.
@ The zero halfwords of alignment at 0x080f9a96, 0x080f9abe, 0x080f9afa,
@ 0x080f9b3e, 0x080f9b4a, 0x080f9b72, 0x080f9ba2, 0x080f9bb6, 0x080f9bf2 and
@ 0x080f9bfe never run and carry no credit.
@ credit: library — TBS/TLA common sound sequence command interpreter
.syntax unified
	.thumb
.global Func_080f9a80
	.thumb_func
Func_080f9a80:
	mov	ip, lr
	movs	r1, #36
	ldr	r2, .L_080f9a80_source
.L_080f9a80_loop:
	ldr	r3, [r2, #0]
	bl	Func_080f9a9a
	stmia	r0!, {r3}
	adds	r2, #4
	subs	r1, #1
	bgt.n	.L_080f9a80_loop
	bx	ip
AlchemyUncredited_080f9a96:
	.2byte 0

	.global Func_080f9a98
	.thumb_func
Func_080f9a98:
AlchemyUncreditedEnd_080f9a96:
	ldrb	r3, [r2, #0]
	.global Func_080f9a9a
	.thumb_func
Func_080f9a9a:
	push	{r0}
	lsrs	r0, r2, #25
	bne.n	.L_080f9a80_return
	ldr	r0, .L_080f9a80_source
	cmp	r2, r0
	bcc.n	.L_080f9a80_clear
	lsrs	r0, r2, #14
	beq.n	.L_080f9a80_return
.L_080f9a80_clear:
	movs	r3, #0
.L_080f9a80_return:
	pop	{r0}
	bx	lr
	.align 2
.L_080f9a80_source:
	.4byte 0x080fb7a0

	.global Func_080f9ab4
	.thumb_func
Func_080f9ab4:
	ldr	r2, [r1, #64]
	adds	r3, r2, #1
	str	r3, [r1, #64]
	ldrb	r3, [r2, #0]
	b.n	Func_080f9a9a
AlchemyUncredited_080f9abe:
	.2byte 0

.global Func_080f9ac0
	.thumb_func
Func_080f9ac0:
AlchemyUncreditedEnd_080f9abe:
	push	{lr}
	ldr	r2, [r1, #64]
	ldrb	r0, [r2, #3]
	lsls	r0, r0, #8
	ldrb	r3, [r2, #2]
	orrs	r0, r3
	lsls	r0, r0, #8
	ldrb	r3, [r2, #1]
	orrs	r0, r3
	lsls	r0, r0, #8
	bl	Func_080f9a98
	orrs	r0, r3
	str	r0, [r1, #64]
	pop	{r0}
	bx	r0

.global Func_080f9ae0
	.thumb_func
Func_080f9ae0:
	ldrb	r2, [r1, #2]
	cmp	r2, #3
	bcs.n	.L_080f9ae0_full
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldr	r2, [r1, #64]
	adds	r2, #4
	str	r2, [r3, #68]
	ldrb	r2, [r1, #2]
	adds	r2, #1
	strb	r2, [r1, #2]
	b.n	Func_080f9ac0
.L_080f9ae0_full:
	b.n	Func_080f9a50
AlchemyUncredited_080f9afa:
	.2byte 0

.global Func_080f9afc
	.thumb_func
Func_080f9afc:
AlchemyUncreditedEnd_080f9afa:
	ldrb	r2, [r1, #2]
	cmp	r2, #0
	beq.n	.L_080f9afc_0
	subs	r2, #1
	strb	r2, [r1, #2]
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldr	r2, [r3, #68]
	str	r2, [r1, #64]
.L_080f9afc_0:
	bx	lr

.set .L_080f9b10__080f9ac2, 0x080f9ac2
	.global Func_080f9b10
	.thumb_func
Func_080f9b10:
	push	{lr}
	ldr	r2, [r1, #64]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_080f9b10_repeat
	adds	r2, #1
	str	r2, [r1, #64]
	b.n	.L_080f9b10__080f9ac2
.L_080f9b10_repeat:
	ldrb	r3, [r1, #3]
	adds	r3, #1
	strb	r3, [r1, #3]
	mov	ip, r3
	bl	Func_080f9ab4
	cmp	ip, r3
	bcs.n	.L_080f9b10_advance
	b.n	.L_080f9b10__080f9ac2
.L_080f9b10_advance:
	movs	r3, #0
	strb	r3, [r1, #3]
	adds	r2, #5
	str	r2, [r1, #64]
	pop	{r0}
	bx	r0
AlchemyUncredited_080f9b3e:
	.2byte 0

.global Func_080f9b40
	.thumb_func
Func_080f9b40:
AlchemyUncreditedEnd_080f9b3e:
	mov	ip, lr
	bl	Func_080f9ab4
	strb	r3, [r1, #29]
	bx	ip

AlchemyUncredited_080f9b4a:
	.balign 4, 0
.global Func_080f9b4c
	.thumb_func
Func_080f9b4c:
AlchemyUncreditedEnd_080f9b4a:
	mov	ip, lr
	bl	Func_080f9ab4
	lsls	r3, r3, #1
	strh	r3, [r0, #28]
	ldrh	r2, [r0, #30]
	muls	r3, r2
	lsrs	r3, r3, #8
	strh	r3, [r0, #32]
	bx	ip

.global Func_080f9b60
	.thumb_func
Func_080f9b60:
	mov	ip, lr
	bl	Func_080f9ab4
	strb	r3, [r1, #10]
	ldrb	r3, [r1, #0]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip

AlchemyUncredited_080f9b72:
	.balign 4, 0
.global Func_080f9b74
	.thumb_func
Func_080f9b74:
AlchemyUncreditedEnd_080f9b72:
	mov	ip, lr
	ldr	r2, [r1, #64]
	ldrb	r3, [r2, #0]
	adds	r2, #1
	str	r2, [r1, #64]
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsls	r2, r2, #2
	ldr	r3, [r0, #48]
	adds	r2, r2, r3
	ldr	r3, [r2, #0]
	bl	Func_080f9a9a
	str	r3, [r1, #36]
	ldr	r3, [r2, #4]
	bl	Func_080f9a9a
	str	r3, [r1, #40]
	ldr	r3, [r2, #8]
	bl	Func_080f9a9a
	str	r3, [r1, #44]
	bx	ip

AlchemyUncredited_080f9ba2:
	.balign 4, 0
.global Func_080f9ba4
	.thumb_func
Func_080f9ba4:
AlchemyUncreditedEnd_080f9ba2:
	mov	ip, lr
	bl	Func_080f9ab4
	strb	r3, [r1, #18]
	ldrb	r3, [r1, #0]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip

AlchemyUncredited_080f9bb6:
	.balign 4, 0
.global Func_080f9bb8
	.thumb_func
Func_080f9bb8:
AlchemyUncreditedEnd_080f9bb6:
	mov	ip, lr
	bl	Func_080f9ab4
	subs	r3, #64
	strb	r3, [r1, #20]
	ldrb	r3, [r1, #0]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip

.global Func_080f9bcc
	.thumb_func
Func_080f9bcc:
	mov	ip, lr
	bl	Func_080f9ab4
	subs	r3, #64
	strb	r3, [r1, #14]
	ldrb	r3, [r1, #0]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip

.global Func_080f9be0
	.thumb_func
Func_080f9be0:
	mov	ip, lr
	bl	Func_080f9ab4
	strb	r3, [r1, #15]
	ldrb	r3, [r1, #0]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip

AlchemyUncredited_080f9bf2:
	.balign 4, 0
.global Func_080f9bf4
	.thumb_func
Func_080f9bf4:
AlchemyUncreditedEnd_080f9bf2:
	mov	ip, lr
	bl	Func_080f9ab4
	strb	r3, [r1, #27]
	bx	ip

AlchemyUncredited_080f9bfe:
	.balign 4, 0
.global Func_080f9c00
	.thumb_func
Func_080f9c00:
AlchemyUncreditedEnd_080f9bfe:
	mov	ip, lr
	bl	Func_080f9ab4
	ldrb	r0, [r1, #24]
	cmp	r0, r3
	beq.n	.L_080f9c00_0
	strb	r3, [r1, #24]
	ldrb	r3, [r1, #0]
	movs	r2, #15
	orrs	r3, r2
	strb	r3, [r1, #0]
.L_080f9c00_0:
	bx	ip

.global Func_080f9c18
	.thumb_func
Func_080f9c18:
	mov	ip, lr
	bl	Func_080f9ab4
	subs	r3, #64
	strb	r3, [r1, #12]
	ldrb	r3, [r1, #0]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	ip

.global Func_080f9c2c
	.thumb_func
Func_080f9c2c:
	mov	ip, lr
	ldr	r2, [r1, #64]
	ldrb	r3, [r2, #0]
	adds	r2, #1
	ldr	r0, [pc, #8]
	adds	r0, r0, r3
	bl	Func_080f9ab6
	strb	r3, [r0, #0]
	bx	ip
	.4byte 0x04000060

