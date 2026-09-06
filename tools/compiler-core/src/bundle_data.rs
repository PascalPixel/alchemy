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
        &["a3cd21bc5a51f11ec97909f6547719857e16e76fa361abe7520d9025be84730f"],
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
    ("darwin-arm64", &[("gs1", GS1), ("gs2", GS1)]),
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
        ],
    ),
    ("darwin-x64", &[]),
    (
        "linux-x64",
        &["9200c74552a980be35fd58c8afdbd07bb76c9b785b57bad78d8303e00d738af3"],
    ),
    ("linux-arm64", &[]),
];
