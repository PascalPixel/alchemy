pub type ExecutableDigests = (&'static str, &'static [&'static str]);
pub type TargetExecutables = (&'static str, &'static [ExecutableDigests]);
pub type HostTargets = (&'static str, &'static [TargetExecutables]);
pub type HostDigests = (&'static str, &'static [&'static str]);

// Restored agscc 5ec3e2e host build; unmodified GNU gas 2.10.
const GS1: &[ExecutableDigests] = &[
    (
        "xgcc",
        &["7303ec4e1b246415db0edaa888cd8d01550ca2752df825c41f9ec08e8814245f"],
    ),
    (
        "cpp0",
        &["4edb7644d5875f3524e40d4830653ab8a232e33984bf6a5700c783f19070887c"],
    ),
    (
        "tradcpp0",
        &["c54bbb4c4bc1b9eb79bbe4fdc85d75facd55fd77bb622288e77cf24d61f97214"],
    ),
    (
        "cc1",
        &["84a470f973bf8b5268890fec449f4c23716f1cc185c5a354ba77cdbec6b7fd4b"],
    ),
    (
        "as",
        &[
            "a3cd21bc5a51f11ec97909f6547719857e16e76fa361abe7520d9025be84730f",
            // Second local host build of the same unmodified GNU gas 2.10
            // source. Admitted on reproduction, not on provenance: with this
            // assembler the complete GS1 EN ROM rebuilds byte-identically
            // against the approved cartridge (1,354,302 executable bytes,
            // unowned_bytes=0, rom_fallback_bytes=0). That establishes
            // behavioural equivalence for this target; it is not evidence
            // about which historical release produced the binary.
            "de8c6568d5742acda7dff00acdfd501d50cb767770fb77fa6d688925bb7fec61",
        ],
    ),
];
// GS2: the same source with agscc-gs2.unidiff applied by `make compilers-gs2`;
// reconstructed from the image, not located (see the patch header).
const GS2_XGCC: &str = "034c52208be7eaeb6c0df53e722da21eabcb5579272fc9d350222d463a2490b7";
const GS2_CC1: &str = "f574397245c50ba2b7d19e53a702406fb8666930c9ba47303155b46a80619f8d";
const GS2: &[ExecutableDigests] = &[
    (
        "xgcc",
        &[
            GS2_XGCC,
            "b88448aff89149183847ee37e0443f12ca4e147878893a7fe8bb490694e970a9",
        ],
    ),
    ("cpp0", GS1[1].1),
    ("tradcpp0", GS1[2].1),
    ("cc1", &[GS2_CC1]),
    ("as", GS1[4].1),
];
const EMPTY_GS1: &[ExecutableDigests] = &[
    ("xgcc", &[]),
    ("cpp0", &[]),
    ("tradcpp0", &[]),
    ("cc1", &[]),
    ("as", &[]),
];
const EMPTY: &[TargetExecutables] = &[("gs1", EMPTY_GS1), ("gs2", EMPTY_GS1)];

pub static EXPECTED: &[HostTargets] = &[
    ("darwin-arm64", &[("gs1", GS1), ("gs2", GS2)]),
    ("darwin-x64", EMPTY),
    // The old modified Linux bundle is not evidence for this restored route.
    ("linux-x64", EMPTY),
    ("linux-arm64", EMPTY),
];
pub static AGBCC_EXPECTED: &[HostDigests] = &[
    (
        "darwin-arm64",
        &[
            // Stock pret/agbcc da598c1: 300 identical objects across twelve
            // editions; all 4,178 claimed GS1 EN bytes independently linked.
            "1b871e9350265d6a530f26d6149818e3294a8b0231a574960226e506a7a5e677",
            // Second local host build of the same stock pret/agbcc da598c1
            // source, admitted on the same reproduction evidence as the gas
            // entry above rather than on provenance.
            "f63ca1c50e35c74b4074195fc9dc7029a950ac04eeaece75b38ae407d342bf67",
        ],
    ),
    ("darwin-x64", &[]),
    (
        "linux-x64",
        &["9200c74552a980be35fd58c8afdbd07bb76c9b785b57bad78d8303e00d738af3"],
    ),
    ("linux-arm64", &[]),
];
