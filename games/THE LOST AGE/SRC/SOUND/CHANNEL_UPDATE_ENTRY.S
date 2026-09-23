@ Guarded ROM-to-IWRAM entry of the sound driver frame update. It reads a
@ PC-relative block {stack limit 0x03007900, 'Smsh', 0x03007ff0} with
@ add r1, pc and ldmia, returns 1 early when the stack pointer is below the
@ limit or when the SoundInfo whose pointer 0x03007ff0 holds does not start
@ with the 'Smsh' ID, then saves r0 and r8-r11 in a private frame (mov r1,
@ r8 ... mov r4, fp; push {r0-r4}) and tail-branches with ldr r3; bx r3 into
@ IWRAM code that restores it. Neither approved compiler emits add rd, pc, and
@ GCC 2.96 saves high registers only in its prologue, through low registers it
@ has already pushed, highest first. It is TLA's own revision of TBS
@ SOUND/DRIVER/CHANNEL_UPDATE_ENTRY.S (0x080006fc), with an added stack check.
@ The zero halfword after its early return (0x08000646) never runs, and the
@ pool words 0x00000350 and 0x04000006 at 0x080006ac are loaded by no
@ instruction; they carry no credit.
@ credit: handwritten — TLA sound driver guarded ROM-to-IWRAM frame entry
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08000630
	.global SoundDriver_EnterFrameUpdate
	.thumb_func
SoundDriver_EnterFrameUpdate:
	add	r1, pc, #20
	ldmia	r1!, {r0, r2, r3}
	mov	r1, sp
	cmp	r1, r0
	bcc.n	.L_08000642
	ldr	r0, [r3, #0]
	ldr	r3, [r0, #0]
	cmp	r2, r3
	beq.n	.L_08000654
.L_08000642:
	movs	r0, #1
	bx	lr
AlchemyUncredited_08000646:
	.2byte	0x0000
AlchemyUncreditedEnd_08000646:
	.4byte	0x03007900
	.4byte	0x68736d53
	.4byte	0x03007ff0
.L_08000654:
	push	{r5, r6, r7, lr}
	mov	r1, r8
	mov	r2, r9
	mov	r3, sl
	mov	r4, fp
	push	{r0, r1, r2, r3, r4}
	sub	sp, #24
	ldr	r3, [r0, #32]
	cmp	r3, #0
	beq.n	.L_08000670
	ldr	r0, [r0, #36]
	mov	lr, r3
	.inst.n	0xf800
	ldr	r0, [sp, #24]
.L_08000670:
	ldr	r3, [r0, #40]
	mov	lr, r3
	.inst.n	0xf800
	ldr	r0, [sp, #24]
	ldr	r3, [r0, #16]
	str	r3, [sp, #0]
	ldr	r5, [pc, #40]
	ldr	r4, [pc, #52]
	ldrb	r4, [r4, #1]
	subs	r7, r4, #1
	bls.n	.L_08000690
	ldrb	r1, [r0, #11]
	subs	r1, r1, r7
	lsls	r2, r3, #1
	muls	r2, r1
	adds	r5, r5, r2
.L_08000690:
	str	r5, [sp, #8]
	ldr	r4, [sp, #24]
	ldr	r0, [r4, #20]
	mov	r9, r0
	ldr	r0, [r4, #24]
	mov	ip, r0
	ldrb	r0, [r4, #6]
	adds	r4, #80
	ldr	r3, [pc, #0]
	bx	r3
	.4byte	0x030007b5
	.4byte	0x02003a90
AlchemyUncredited_080006ac:
	.4byte	0x00000350
	.4byte	0x04000006
AlchemyUncreditedEnd_080006ac:
	.4byte	0x03001138
