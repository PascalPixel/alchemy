.syntax unified
	.thumb
	.global ply_lfos
	.thumb_func
ply_lfos:
	mov	ip, lr
	bl	ld_r3_tp_adr_i_unchecked
	strb	r3, [r1, #25]
	cmp	r3, #0
	bne.n	.L_081c1e80
	bl	clear_modM
.L_081c1e80:
	bx	ip
AlchemyUncredited_081c1e82:
	.2byte	0x0000
	.global ply_mod
	.thumb_func
ply_mod:
AlchemyUncreditedEnd_081c1e82:
	mov	ip, lr
	bl	ld_r3_tp_adr_i_unchecked
	strb	r3, [r1, #23]
	cmp	r3, #0
	bne.n	.L_081c1e94
	bl	clear_modM
.L_081c1e94:
	bx	ip
AlchemyUncredited_081c1e96:
	.2byte	0x0000
AlchemyUncreditedEnd_081c1e96:
