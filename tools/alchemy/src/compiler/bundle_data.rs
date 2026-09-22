pub type ExecutableDigests = (&'static str, &'static [&'static str]);
pub type TargetExecutables = (&'static str, &'static [ExecutableDigests]);
pub type HostTargets = (&'static str, &'static [TargetExecutables]);
pub type HostDigests = (&'static str, &'static [&'static str]);

// Licensed agscc f209503: TLA lowering is opt-in; both games use this bundle.
// The second entries restore source-bootstrap builds of the same pinned
// sources. Game flags and compiler families remain unchanged.
const GAME: &[ExecutableDigests] = &[
    (
        "xgcc",
        &[
            "701fbabb93c11e665550b5106f610169924315ea2be2136fb9e7b9261bfecf8b",
            "7303ec4e1b246415db0edaa888cd8d01550ca2752df825c41f9ec08e8814245f",
            "4f63bf82c5f20c6cb02186f95b63617188efa67be6e58c02f9a39c89505daa49",
        ],
    ),
    (
        "cpp0",
        &[
            "affded82f0de6e12dac30857528cd9931e0568ce7f28fadc9f519718e2e75353",
            "4edb7644d5875f3524e40d4830653ab8a232e33984bf6a5700c783f19070887c",
            "d54535b56093896a15df114b3dd55c77eb096dd4b269379b58a589c6f158a7be",
        ],
    ),
    (
        "tradcpp0",
        &[
            "d7638783dff6726bdf50a63fce1a8ca1c163d32976c285cdd94dfb2497c87ed8",
            "c54bbb4c4bc1b9eb79bbe4fdc85d75facd55fd77bb622288e77cf24d61f97214",
            "35193e2e806e478896453a4d0e9d9f8f1ef0f2d5b474d2f6e6ff2fa1302a4eb2",
        ],
    ),
    (
        "cc1",
        &[
            "2cc1e7ca0e33f55d3974df1818403f325f4adc64686eeb228d0f575b8a2f432d",
            "80cbeb50f7e0829a8e27e9492a3fcffb4c7ce6dedbe873464278414003469ee9",
            "a3d3f0d19e0ab039abc0456cb4690f9a3cf1f5007032dc550dd09952169acdd7",
        ],
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
        ],
    ),
    ("darwin-x64", &[]),
    (
        "linux-x64",
        &["9200c74552a980be35fd58c8afdbd07bb76c9b785b57bad78d8303e00d738af3"],
    ),
    ("linux-arm64", &[]),
];
