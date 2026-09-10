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
        &["341996a29bbf5620d4c16982369597189f0c973945f3f526904eb93ef778427e"],
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
const GS2_CC1: &str = "aa243961d5bbac94c12ba531970e428f49d0e80d4f65c78ae4310c99a8081f13";
const GS2: &[ExecutableDigests] = &[
    ("xgcc", &[GS2_XGCC]),
    ("cpp0", GS1[1].1),
    ("tradcpp0", GS1[2].1),
    ("cc1", &[GS2_CC1]),
    ("as", GS1[4].1),
];
// Restored agscc 5ec3e2e host build on linux-x64 (x86_64), unmodified GNU gas
// 2.10, built from the committed sources on this host.
const GS1_LINUX_X64: &[ExecutableDigests] = &[
    (
        "xgcc",
        &["0beedb6fae344420b2a3c309a8edecfe99b74bfcf7d4d5be17b59624efbecdfc"],
    ),
    (
        "cpp0",
        &["a028c0165e02cfc5138a9bbc59fe64e0c95cfd0bb05aa90c00ffade283df3d7b"],
    ),
    (
        "tradcpp0",
        &["9e5194d67b4f6c1b0a7c0802166cff333c72e31ad0af1f2363d5a89f23140c87"],
    ),
    (
        "cc1",
        &["a3079e72f03e031b7d8baa27c3b6cc69c7c384a288642df16f730775dc8657fa"],
    ),
    (
        "as",
        &[
            // Linux host build of the same unmodified GNU gas 2.10 source.
            // Admitted on reproduction, not on provenance, in the same form as
            // the darwin second `as` entry: if the complete GS1 EN ROM rebuilds
            // byte-identically against the approved cartridge with this
            // assembler, it is behaviourally equivalent for this target.
            "8eb4386cb65b04ccfee4c9708501c981d2f0399712545844e0f376c5a3ab33df",
        ],
    ),
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
    // GS2 is admitted on linux-x64 only when its compiler is staged by
    // `make compilers-gs2`; leave it empty here so a GS2 build still requires
    // its own admission. GS1 is admitted on this host, pinned from a green
    // full verify (byte-identical ROM).
    ("linux-x64", &[("gs1", GS1_LINUX_X64), ("gs2", EMPTY_GS1)]),
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
        &[
            "9200c74552a980be35fd58c8afdbd07bb76c9b785b57bad78d8303e00d738af3",
            // Second local host build of the same stock pret/agbcc da598c1
            // source on this box, admitted in the same reproduction form as
            // the darwin second agbcc entry.
            "f32b4f49660f05a124523080f10d9cfd5a3848197075ce87385d61c57dd5b962",
        ],
    ),
    ("linux-arm64", &[]),
];
