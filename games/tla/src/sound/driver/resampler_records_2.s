.syntax unified
	.arm
	.global Sound_ResamplerRecord1
Sound_ResamplerRecord1:
	asrs	r4, ip, #24
	mulne	r4, r6, r4
	rors	ip, ip, #16
	asrs	r2, ip, #24
	mulne	r2, r6, r2
	asrs	ip, fp, #24
	mlane	r4, r1, ip, r4
	rors	fp, fp, #16
	asrs	ip, fp, #24
	mlane	r2, r1, ip, r2
	b	Sound_ResamplerRecord2
