pub type ExecutableDigests = (&'static str, &'static [&'static str]);
pub type TargetExecutables = (&'static str, &'static [ExecutableDigests]);
pub type HostTargets = (&'static str, &'static [TargetExecutables]);
pub type HostDigests = (&'static str, &'static [&'static str]);

// Licensed agscc f209503: GS2 lowering is opt-in; both games use this bundle.
const GAME: &[ExecutableDigests] = &[
    (
        "xgcc",
        &["701fbabb93c11e665550b5106f610169924315ea2be2136fb9e7b9261bfecf8b"],
    ),
    (
        "cpp0",
        &["affded82f0de6e12dac30857528cd9931e0568ce7f28fadc9f519718e2e75353"],
    ),
    (
        "tradcpp0",
        &["d7638783dff6726bdf50a63fce1a8ca1c163d32976c285cdd94dfb2497c87ed8"],
    ),
    (
        "cc1",
        &["2cc1e7ca0e33f55d3974df1818403f325f4adc64686eeb228d0f575b8a2f432d"],
    ),
    (
        "as",
        &["de8c6568d5742acda7dff00acdfd501d50cb767770fb77fa6d688925bb7fec61"],
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
    ("darwin-arm64", &[("gs1", GAME), ("gs2", GAME)]),
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
            // Pascal-approved rebuild of pinned, unmodified pret/agbcc
            // da598c1 on 2026-09-12; matching checks remain mandatory.
            "97d346e67ab2751e6d2d4aa81a4b85da69ec35f4b61ead9c310256618420dff7",
        ],
    ),
    ("darwin-x64", &[]),
    (
        "linux-x64",
        &["9200c74552a980be35fd58c8afdbd07bb76c9b785b57bad78d8303e00d738af3"],
    ),
    ("linux-arm64", &[]),
];
