pub type ExecutableDigests = (&'static str, &'static [&'static str]);
pub type TargetExecutables = (&'static str, &'static [ExecutableDigests]);
pub type HostTargets = (&'static str, &'static [TargetExecutables]);
pub type HostDigests = (&'static str, &'static [&'static str]);

// Licensed agscc: GCC 2.96 with its host ports and TLA's two options, off
// by default; both games use this bundle and TLA's game code turns them on.
// Each digest is agscc c7a493c as agscc/build.sh builds it: the build does
// not depend on its folder, so built anywhere it gives these bytes.
const GAME: &[ExecutableDigests] = &[
    (
        "xgcc",
        &["e4a9d313adfa3c310bf9c24a249c83c0eb2be04217c2c7b31d3dc07ed57e4ad4"],
    ),
    (
        "cpp0",
        &["bf860686ff434508b62246f7dbc3807769440ab5410d6e3a514fb0aa8944fd2c"],
    ),
    (
        "tradcpp0",
        &["9c0c3d4f2609e0dc1c8d7b9c25a068dee15220466a664d5adc593f1c3bbe2b7f"],
    ),
    (
        "cc1",
        &["023962d33de3283a9ed48f18c880df073b180a2a1e833580659de5949a8225d8"],
    ),
    (
        "as",
        &[
            "de8c6568d5742acda7dff00acdfd501d50cb767770fb77fa6d688925bb7fec61",
            "d5d916a83c5ab6b3b109f772b6e9c51d0d4dbd5d7c5942ea8be75db1ff1c57c4",
            "a1cd0b6ced43d691b731504ada7a5a2de728f624b4dbdf3c0f7409a277ba31b8",
        ],
    ),
];
const EMPTY_TBS: &[ExecutableDigests] = &[
    ("xgcc", &[]),
    ("cpp0", &[]),
    ("tradcpp0", &[]),
    ("cc1", &[]),
    ("as", &[]),
];
const EMPTY: &[TargetExecutables] = &[("tbs", EMPTY_TBS), ("tla", EMPTY_TBS)];

pub static EXPECTED: &[HostTargets] = &[
    ("darwin-arm64", &[("tbs", GAME), ("tla", GAME)]),
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
            // editions; all 4,178 claimed TBS EN bytes independently linked.
            "1b871e9350265d6a530f26d6149818e3294a8b0231a574960226e506a7a5e677",
            // Second local host build of the same stock pret/agbcc da598c1
            // source, admitted on the same reproduction evidence as the gas
            // entry above rather than on provenance.
            "f63ca1c50e35c74b4074195fc9dc7029a950ac04eeaece75b38ae407d342bf67",
            // Pascal-approved rebuild of pinned, unmodified pret/agbcc
            // da598c1 on 2026-09-12; matching checks remain mandatory.
            "97d346e67ab2751e6d2d4aa81a4b85da69ec35f4b61ead9c310256618420dff7",
            // Source-bootstrap rebuild of the same unmodified da598c1.
            "fc60e7c849af70814e944445142eda626d19ca694a2e1d4f27a1d1d7a0f7b3cd",
            "3e9433bcfd37994d029389cf6c6ec11f7cba30e23e99f132e8f0220454f3cc32",
            // Pascal-approved pinned-source rebuild, 2026-09-28; a clean
            // host relink reproduced this exact digest.
            "dfc3612d06cfd79d88092d6f3feff952a4ce3712c394bf58dbf7ca5c405e21e9",
        ],
    ),
    ("darwin-x64", &[]),
    (
        "linux-x64",
        &["9200c74552a980be35fd58c8afdbd07bb76c9b785b57bad78d8303e00d738af3"],
    ),
    ("linux-arm64", &[]),
];

/// pret's ARM compiler, agbcc/gcc_arm from the approved da598c1 source, built
/// as pret's build.sh builds it (Pascal, 2026-09-29).
pub static AGBCC_ARM_EXPECTED: &[HostDigests] = &[
    (
        "darwin-arm64",
        &["bfd96c296dcd02aa7084ecbb9495f9f2350640e2b2e16d931059dbc0cfebbcd1"],
    ),
    ("darwin-x64", &[]),
    ("linux-x64", &[]),
    ("linux-arm64", &[]),
];
