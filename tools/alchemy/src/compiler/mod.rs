//! Golden Sun compiler policy, ownership and linking integration.
//! Portable decoding, comparison, execution and caching live in Psynergy.
pub(crate) mod assembly_source;
pub(crate) mod build_io;
pub(crate) mod bundle;
mod bundle_data;
pub(crate) mod no_asm;
pub(crate) mod overlay;
pub(crate) mod plan;
pub(crate) mod preprocess;
pub(crate) mod routing;
mod routing_data;
pub(crate) mod runtime;
pub(crate) mod sha256;
pub(crate) mod source_inputs;
pub(crate) mod steering;
