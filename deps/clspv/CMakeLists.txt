########################################################################################################################
# Copyright (c) 2022-2022 Daemyung Jang.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
########################################################################################################################

cmake_minimum_required(VERSION 3.18)
project(clspv CXX)

add_library(clspv INTERFACE
        include/clspv/AddressSpace.h
        include/clspv/ArgKind.h
        include/clspv/Compiler.h
        include/clspv/Option.h
        include/clspv/Passes.h
        include/clspv/PushConstant.h
        include/clspv/Sampler.h
        include/clspv/SpecConstant.h
)

target_include_directories(clspv
    INTERFACE
        include
)

target_link_directories(clspv
    INTERFACE
        lib
)

target_link_libraries(clspv
    INTERFACE
        libcurses.a
        libz.a
        libclangAPINotes.a
        libclangARCMigrate.a
        libclangAST.a
        libclangASTMatchers.a
        libclangAnalysis.a
        libclangAnalysisFlowSensitive.a
        libclangAnalysisFlowSensitiveModels.a
        libclangBasic.a
        libclangCodeGen.a
        libclangCrossTU.a
        libclangDependencyScanning.a
        libclangDirectoryWatcher.a
        libclangDriver.a
        libclangDynamicASTMatchers.a
        libclangEdit.a
        libclangExtractAPI.a
        libclangFormat.a
        libclangFrontend.a
        libclangFrontendTool.a
        libclangHandleCXX.a
        libclangHandleLLVM.a
        libclangIndex.a
        libclangIndexSerialization.a
        libclangInterpreter.a
        libclangLex.a
        libclangParse.a
        libclangRewrite.a
        libclangRewriteFrontend.a
        libclangSema.a
        libclangSerialization.a
        libclangStaticAnalyzerCheckers.a
        libclangStaticAnalyzerCore.a
        libclangStaticAnalyzerFrontend.a
        libclangSupport.a
        libclangTooling.a
        libclangToolingASTDiff.a
        libclangToolingCore.a
        libclangToolingInclusions.a
        libclangToolingRefactoring.a
        libclangToolingSyntax.a
        libclangTransformer.a
        libclspv_core.a
        libLLVMAggressiveInstCombine.a
        libLLVMAnalysis.a
        libLLVMAsmParser.a
        libLLVMAsmPrinter.a
        libLLVMBinaryFormat.a
        libLLVMBitReader.a
        libLLVMBitWriter.a
        libLLVMBitstreamReader.a
        libLLVMCFGuard.a
        libLLVMCFIVerify.a
        libLLVMCodeGen.a
        libLLVMCore.a
        libLLVMCoroutines.a
        libLLVMCoverage.a
        libLLVMDWARFLinker.a
        libLLVMDWP.a
        libLLVMDebugInfoCodeView.a
        libLLVMDebugInfoDWARF.a
        libLLVMDebugInfoGSYM.a
        libLLVMDebugInfoMSF.a
        libLLVMDebugInfoPDB.a
        libLLVMDebuginfod.a
        libLLVMDemangle.a
        libLLVMDiff.a
        libLLVMDlltoolDriver.a
        libLLVMExecutionEngine.a
        libLLVMExegesis.a
        libLLVMExtensions.a
        libLLVMFileCheck.a
        libLLVMFrontendOpenACC.a
        libLLVMFrontendOpenMP.a
        libLLVMFuzzMutate.a
        libLLVMFuzzerCLI.a
        libLLVMGlobalISel.a
        libLLVMIRReader.a
        libLLVMInstCombine.a
        libLLVMInstrumentation.a
        libLLVMInterfaceStub.a
        libLLVMInterpreter.a
        libLLVMJITLink.a
        libLLVMLTO.a
        libLLVMLibDriver.a
        libLLVMLineEditor.a
        libLLVMLinker.a
        libLLVMMC.a
        libLLVMMCA.a
        libLLVMMCDisassembler.a
        libLLVMMCJIT.a
        libLLVMMCParser.a
        libLLVMMIRParser.a
        libLLVMObjCARCOpts.a
        libLLVMObjCopy.a
        libLLVMObject.a
        libLLVMObjectYAML.a
        libLLVMOption.a
        libLLVMOrcJIT.a
        libLLVMOrcShared.a
        libLLVMOrcTargetProcess.a
        libLLVMPasses.a
        libLLVMProfileData.a
        libLLVMRemarks.a
        libLLVMRuntimeDyld.a
        libLLVMScalarOpts.a
        libLLVMSelectionDAG.a
        libLLVMSupport.a
        libLLVMSymbolize.a
        libLLVMTableGen.a
        libLLVMTableGenGlobalISel.a
        libLLVMTarget.a
        libLLVMTextAPI.a
        libLLVMTransformUtils.a
        libLLVMVectorize.a
        libLLVMWindowsDriver.a
        libLLVMWindowsManifest.a
        libLLVMXRay.a
        libLLVMipo.a
)
