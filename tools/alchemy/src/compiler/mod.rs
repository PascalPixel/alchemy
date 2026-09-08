//! Golden Sun compiler policy, ownership and linking integration.
//! Portable decoding, comparison, execution and caching live in Psynergy.
pub(crate) mod build_io;
pub(crate) mod bundle;
mod bundle_data;
mod call_via_data;
pub(crate) mod canonical_json;
pub(crate) mod no_asm;
pub(crate) mod overlay;
pub(crate) mod plan;
pub(crate) mod routing;
mod routing_data;
pub(crate) mod sha256;
pub(crate) mod source_inputs;
pub(crate) mod source_paths;
pub(crate) mod symbols;
pub(crate) mod thumb;
pub(crate) mod translation_units;
