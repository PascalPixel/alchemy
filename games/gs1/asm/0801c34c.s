@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x0801c34c (156 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_0801c34c
	.thumb_func
Func_0801c34c:
	push	{r5, r6, lr}
	ldr	r3, [pc, #136]	@ (0x801c3d8)
	sub	sp, #20
	ldr	r6, [r3, #0]
	ldr	r2, [pc, #132]	@ (0x801c3dc)
	movs	r3, #8
	movs	r1, #224	@ 0xe0
	str	r3, [sp, #16]
	str	r3, [sp, #12]
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #225	@ 0xe1
	lsls	r1, r1, #1
	adds	r3, r2, r1
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	bl	Func_0808a5d0
	ldr	r3, [pc, #104]	@ (0x801c3e0)
	adds	r5, r0, #0
	adds	r5, r5, r3
	add	r0, sp, #4
	add	r1, sp, #16
	add	r2, sp, #12
	add	r3, sp, #8
	str	r0, [sp, #0]
	adds	r0, r5, #0
	bl	Func_080187ac
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #4]
	movs	r0, #30
	movs	r1, #10
	subs	r0, r0, r2
	subs	r1, r1, r3
	movs	r4, #2
	asrs	r1, r1, #1
	asrs	r0, r0, #1
	str	r1, [sp, #12]
	str	r4, [sp, #0]
	str	r0, [sp, #16]
	bl	Func_080162d4
	movs	r2, #140	@ 0x8c
	lsls	r2, r2, #2
	adds	r1, r0, #0
	adds	r3, r6, r2
	str	r1, [r3, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_0801e74c
	movs	r3, #141	@ 0x8d
	lsls	r3, r3, #2
	adds	r2, r6, r3
	movs	r1, #200	@ 0xc8
	movs	r3, #90	@ 0x5a
	strh	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #24]	@ (0x801c3e4)
	bl	Func_080041d8
	add	sp, #20
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000099b
	.4byte 0x0801c3e9
