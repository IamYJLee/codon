// Copyright (C) 2022-2026 Exaloop Inc. <https://exaloop.io>

#pragma once

#include <string>

#include "codon/cir/llvm/llvm.h"

namespace codon {
namespace ir {

/// Applies Metal-specific transformations and generates MSL (Metal Shading Language)
/// code from kernel functions in the given LLVM module. Unlike CUDA/PTX which uses
/// LLVM's NVPTX backend, this performs a custom LLVM IR → MSL transpilation since
/// LLVM has no MSL backend.
/// @param module LLVM module containing Metal kernel functions (marked with "kernel"
/// annotation)
/// @param mslFilename Filename for output MSL code; empty to use filename based on
/// module
void applyMetalTransformations(llvm::Module *module,
                               const std::string &mslFilename = "");

} // namespace ir
} // namespace codon
