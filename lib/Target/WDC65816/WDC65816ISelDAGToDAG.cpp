//===-- WDC65816ISelDAGToDAG.cpp - A dag to dag inst selector for WDC65816 ----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines an instruction selector for the WDC65816 target.
//
//===----------------------------------------------------------------------===//

#include "WDC65816.h"
#include "WDC65816TargetMachine.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/CodeGen/TargetLowering.h"
#include "llvm/Config/llvm-config.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "WDC65816-isel"

namespace {
  struct WDC65816ISelAddressMode {
    enum {
      RegBase,
      FrameIndexBase
    } BaseType;

    struct {            // This is really a union, discriminated by BaseType!
      SDValue Reg;
      int FrameIndex;
    } Base;

    int16_t Disp;
    const GlobalValue *GV;
    const Constant *CP;
    const BlockAddress *BlockAddr;
    const char *ES;
    int JT;
    unsigned Align;    // CP alignment.

    WDC65816ISelAddressMode()
      : BaseType(RegBase), Disp(0), GV(nullptr), CP(nullptr),
        BlockAddr(nullptr), ES(nullptr), JT(-1), Align(0) {
    }

    bool hasSymbolicDisplacement() const {
      return GV != nullptr || CP != nullptr || ES != nullptr || JT != -1;
    }

#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
    LLVM_DUMP_METHOD void dump() {
      errs() << "WDC65816ISelAddressMode " << this << '\n';
      if (BaseType == RegBase && Base.Reg.getNode() != nullptr) {
        errs() << "Base.Reg ";
        Base.Reg.getNode()->dump();
      } else if (BaseType == FrameIndexBase) {
        errs() << " Base.FrameIndex " << Base.FrameIndex << '\n';
      }
      errs() << " Disp " << Disp << '\n';
      if (GV) {
        errs() << "GV ";
        GV->dump();
      } else if (CP) {
        errs() << " CP ";
        CP->dump();
        errs() << " Align" << Align << '\n';
      } else if (ES) {
        errs() << "ES ";
        errs() << ES << '\n';
      } else if (JT != -1)
        errs() << " JT" << JT << " Align" << Align << '\n';
    }
#endif
  };
}

/// WDC65816DAGToDAGISel - WDC65816 specific code to select WDC65816 machine
/// instructions for SelectionDAG operations.
///
namespace {
  class WDC65816DAGToDAGISel : public SelectionDAGISel {
  public:
    WDC65816DAGToDAGISel(WDC65816TargetMachine &TM, CodeGenOpt::Level OptLevel)
        : SelectionDAGISel(TM, OptLevel) {}

  private:
    StringRef getPassName() const override {
      return "WDC65816 DAG->DAG Pattern Instruction Selection";
    }

    bool MatchAddress(SDValue N, WDC65816ISelAddressMode &AM);
    bool MatchWrapper(SDValue N, WDC65816ISelAddressMode &AM);
    bool MatchAddressBase(SDValue N, WDC65816ISelAddressMode &AM);

    bool SelectInlineAsmMemoryOperand(const SDValue &Op, unsigned ConstraintID,
                                      std::vector<SDValue> &OutOps) override;

    // Include the pieces autogenerated from the target description.
  #include "WDC65816GenDAGISel.inc"

    // Main method to transform nodes into machine nodes.
    void Select(SDNode *N) override;

    bool tryIndexedLoad(SDNode *Op);
    bool tryIndexedBinOp(SDNode *Op, SDValue N1, SDValue N2, unsigned Opc8,
                         unsigned Opc16);

    bool SelectAddr(SDValue Addr, SDValue &Base, SDValue &Disp);
  };
}  // end anonymous namespace

/// createWDC65816ISelDag - This pass converts a legalized DAG into a
/// WDC65816-specific DAG, ready for instruction scheduling.
///
FunctionPass *llvm::createWDC65816ISelDag(WDC65816TargetMachine &TM,
                                        CodeGenOpt::Level OptLevel) {
  return new WDC65816DAGToDAGISel(TM, OptLevel);
}


/// MatchWrapper - Try to match WDC65816ISD::Wrapper node into an addressing mode.
/// These wrap things that will resolve down into a symbol reference.  If no
/// match is possible, this returns true, otherwise it returns false.
bool WDC65816DAGToDAGISel::MatchWrapper(SDValue N, WDC65816ISelAddressMode &AM) {
  // If the addressing mode already has a symbol as the displacement, we can
  // never match another symbol.
  if (AM.hasSymbolicDisplacement())
    return true;

  SDValue N0 = N.getOperand(0);

  if (GlobalAddressSDNode *G = dyn_cast<GlobalAddressSDNode>(N0)) {
    AM.GV = G->getGlobal();
    AM.Disp += G->getOffset();
    //AM.SymbolFlags = G->getTargetFlags();
  } else if (ConstantPoolSDNode *CP = dyn_cast<ConstantPoolSDNode>(N0)) {
    AM.CP = CP->getConstVal();
    AM.Align = CP->getAlignment();
    AM.Disp += CP->getOffset();
    //AM.SymbolFlags = CP->getTargetFlags();
  } else if (ExternalSymbolSDNode *S = dyn_cast<ExternalSymbolSDNode>(N0)) {
    AM.ES = S->getSymbol();
    //AM.SymbolFlags = S->getTargetFlags();
  } else if (JumpTableSDNode *J = dyn_cast<JumpTableSDNode>(N0)) {
    AM.JT = J->getIndex();
    //AM.SymbolFlags = J->getTargetFlags();
  } else {
    AM.BlockAddr = cast<BlockAddressSDNode>(N0)->getBlockAddress();
    //AM.SymbolFlags = cast<BlockAddressSDNode>(N0)->getTargetFlags();
  }
  return false;
}

/// MatchAddressBase - Helper for MatchAddress. Add the specified node to the
/// specified addressing mode without any further recursion.
bool WDC65816DAGToDAGISel::MatchAddressBase(SDValue N, WDC65816ISelAddressMode &AM) {
  // Is the base register already occupied?
  if (AM.BaseType != WDC65816ISelAddressMode::RegBase || AM.Base.Reg.getNode()) {
    // If so, we cannot select it.
    return true;
  }

  // Default, generate it as a register.
  AM.BaseType = WDC65816ISelAddressMode::RegBase;
  AM.Base.Reg = N;
  return false;
}

bool WDC65816DAGToDAGISel::MatchAddress(SDValue N, WDC65816ISelAddressMode &AM) {
  LLVM_DEBUG(errs() << "MatchAddress: "; AM.dump());

  switch (N.getOpcode()) {
  default: break;
  case ISD::Constant: {
    uint64_t Val = cast<ConstantSDNode>(N)->getSExtValue();
    AM.Disp += Val;
    return false;
  }

  case WDC65816ISD::Wrapper:
    if (!MatchWrapper(N, AM))
      return false;
    break;

  case ISD::FrameIndex:
    if (AM.BaseType == WDC65816ISelAddressMode::RegBase
        && AM.Base.Reg.getNode() == nullptr) {
      AM.BaseType = WDC65816ISelAddressMode::FrameIndexBase;
      AM.Base.FrameIndex = cast<FrameIndexSDNode>(N)->getIndex();
      return false;
    }
    break;

  case ISD::ADD: {
    WDC65816ISelAddressMode Backup = AM;
    if (!MatchAddress(N.getNode()->getOperand(0), AM) &&
        !MatchAddress(N.getNode()->getOperand(1), AM))
      return false;
    AM = Backup;
    if (!MatchAddress(N.getNode()->getOperand(1), AM) &&
        !MatchAddress(N.getNode()->getOperand(0), AM))
      return false;
    AM = Backup;

    break;
  }

  case ISD::OR:
    // Handle "X | C" as "X + C" iff X is known to have C bits clear.
    if (ConstantSDNode *CN = dyn_cast<ConstantSDNode>(N.getOperand(1))) {
      WDC65816ISelAddressMode Backup = AM;
      uint64_t Offset = CN->getSExtValue();
      // Start with the LHS as an addr mode.
      if (!MatchAddress(N.getOperand(0), AM) &&
          // Address could not have picked a GV address for the displacement.
          AM.GV == nullptr &&
          // Check to see if the LHS & C is zero.
          CurDAG->MaskedValueIsZero(N.getOperand(0), CN->getAPIntValue())) {
        AM.Disp += Offset;
        return false;
      }
      AM = Backup;
    }
    break;
  }

  return MatchAddressBase(N, AM);
}

/// SelectAddr - returns true if it is able pattern match an addressing mode.
/// It returns the operands which make up the maximal addressing mode it can
/// match by reference.
bool WDC65816DAGToDAGISel::SelectAddr(SDValue N,
                                    SDValue &Base, SDValue &Disp) {
  WDC65816ISelAddressMode AM;

  if (MatchAddress(N, AM))
    return false;

  //if (AM.BaseType == WDC65816ISelAddressMode::RegBase)
  //  if (!AM.Base.Reg.getNode())
  //    AM.Base.Reg = CurDAG->getRegister(WDC65816::SR, MVT::i16);

  //Base = (AM.BaseType == WDC65816ISelAddressMode::FrameIndexBase)
  //           ? CurDAG->getTargetFrameIndex(
  //                 AM.Base.FrameIndex,
  //                 getTargetLowering()->getPointerTy(CurDAG->getDataLayout()))
  //           : AM.Base.Reg;

  //if (AM.GV)
  //  Disp = CurDAG->getTargetGlobalAddress(AM.GV, SDLoc(N),
  //                                        MVT::i16, AM.Disp,
  //                                        0/*AM.SymbolFlags*/);
  //else if (AM.CP)
  //  Disp = CurDAG->getTargetConstantPool(AM.CP, MVT::i16,
  //                                       AM.Align, AM.Disp, 0/*AM.SymbolFlags*/);
  //else if (AM.ES)
  //  Disp = CurDAG->getTargetExternalSymbol(AM.ES, MVT::i16, 0/*AM.SymbolFlags*/);
  //else if (AM.JT != -1)
  //  Disp = CurDAG->getTargetJumpTable(AM.JT, MVT::i16, 0/*AM.SymbolFlags*/);
  //else if (AM.BlockAddr)
  //  Disp = CurDAG->getTargetBlockAddress(AM.BlockAddr, MVT::i32, 0,
  //                                       0/*AM.SymbolFlags*/);
  //else
  //  Disp = CurDAG->getTargetConstant(AM.Disp, SDLoc(N), MVT::i16);

  return true;
}

bool WDC65816DAGToDAGISel::
SelectInlineAsmMemoryOperand(const SDValue &Op, unsigned ConstraintID,
                             std::vector<SDValue> &OutOps) {
  SDValue Op0, Op1;
  switch (ConstraintID) {
  default: return true;
  case InlineAsm::Constraint_m: // memory
    if (!SelectAddr(Op, Op0, Op1))
      return true;
    break;
  }

  OutOps.push_back(Op0);
  OutOps.push_back(Op1);
  return false;
}

static bool isValidIndexedLoad(const LoadSDNode *LD) {
  ISD::MemIndexedMode AM = LD->getAddressingMode();
  if (AM != ISD::POST_INC || LD->getExtensionType() != ISD::NON_EXTLOAD)
    return false;

  EVT VT = LD->getMemoryVT();

  switch (VT.getSimpleVT().SimpleTy) {
  case MVT::i8:
    // Sanity check
    if (cast<ConstantSDNode>(LD->getOffset())->getZExtValue() != 1)
      return false;

    break;
  case MVT::i16:
    // Sanity check
    if (cast<ConstantSDNode>(LD->getOffset())->getZExtValue() != 2)
      return false;

    break;
  default:
    return false;
  }

  return true;
}

bool WDC65816DAGToDAGISel::tryIndexedLoad(SDNode *N) {
  LoadSDNode *LD = cast<LoadSDNode>(N);
  if (!isValidIndexedLoad(LD))
    return false;

  MVT VT = LD->getMemoryVT().getSimpleVT();

  //unsigned Opcode = 0;
  //switch (VT.SimpleTy) {
  //case MVT::i8:
  //  Opcode = WDC65816::MOV8rp;
  //  break;
  //case MVT::i16:
  //  Opcode = WDC65816::MOV16rp;
  //  break;
  //default:
  //  return false;
  //}

  //ReplaceNode(N,
  //            CurDAG->getMachineNode(Opcode, SDLoc(N), VT, MVT::i16, MVT::Other,
  //                                   LD->getBasePtr(), LD->getChain()));
  return true;
}

bool WDC65816DAGToDAGISel::tryIndexedBinOp(SDNode *Op, SDValue N1, SDValue N2,
                                         unsigned Opc8, unsigned Opc16) {
  if (N1.getOpcode() == ISD::LOAD &&
      N1.hasOneUse() &&
      IsLegalToFold(N1, Op, Op, OptLevel)) {
    LoadSDNode *LD = cast<LoadSDNode>(N1);
    if (!isValidIndexedLoad(LD))
      return false;

    MVT VT = LD->getMemoryVT().getSimpleVT();
    unsigned Opc = (VT == MVT::i16 ? Opc16 : Opc8);
    MachineMemOperand *MemRef = cast<MemSDNode>(N1)->getMemOperand();
    SDValue Ops0[] = { N2, LD->getBasePtr(), LD->getChain() };
    SDNode *ResNode =
      CurDAG->SelectNodeTo(Op, Opc, VT, MVT::i16, MVT::Other, Ops0);
    CurDAG->setNodeMemRefs(cast<MachineSDNode>(ResNode), {MemRef});
    // Transfer chain.
    ReplaceUses(SDValue(N1.getNode(), 2), SDValue(ResNode, 2));
    // Transfer writeback.
    ReplaceUses(SDValue(N1.getNode(), 1), SDValue(ResNode, 1));
    return true;
  }

  return false;
}


void WDC65816DAGToDAGISel::Select(SDNode *Node) {
  SDLoc dl(Node);

  // If we have a custom node, we already have selected!
  if (Node->isMachineOpcode()) {
    LLVM_DEBUG(errs() << "== "; Node->dump(CurDAG); errs() << "\n");
    Node->setNodeId(-1);
    return;
  }

  //// Few custom selection stuff.
  //switch (Node->getOpcode()) {
  //default: break;
  //case ISD::FrameIndex: {
  //  assert(Node->getValueType(0) == MVT::i16);
  //  int FI = cast<FrameIndexSDNode>(Node)->getIndex();
  //  SDValue TFI = CurDAG->getTargetFrameIndex(FI, MVT::i16);
  //  if (Node->hasOneUse()) {
  //    CurDAG->SelectNodeTo(Node, WDC65816::ADDframe, MVT::i16, TFI,
  //                         CurDAG->getTargetConstant(0, dl, MVT::i16));
  //    return;
  //  }
  //  ReplaceNode(Node, CurDAG->getMachineNode(
  //                        WDC65816::ADDframe, dl, MVT::i16, TFI,
  //                        CurDAG->getTargetConstant(0, dl, MVT::i16)));
  //  return;
  //}
  //case ISD::LOAD:
  //  if (tryIndexedLoad(Node))
  //    return;
  //  // Other cases are autogenerated.
  //  break;
  //case ISD::ADD:
  //  if (tryIndexedBinOp(Node, Node->getOperand(0), Node->getOperand(1),
  //                      WDC65816::ADD8rp, WDC65816::ADD16rp))
  //    return;
  //  else if (tryIndexedBinOp(Node, Node->getOperand(1), Node->getOperand(0),
  //                           WDC65816::ADD8rp, WDC65816::ADD16rp))
  //    return;

  //  // Other cases are autogenerated.
  //  break;
  //case ISD::SUB:
  //  if (tryIndexedBinOp(Node, Node->getOperand(0), Node->getOperand(1),
  //                      WDC65816::SUB8rp, WDC65816::SUB16rp))
  //    return;

  //  // Other cases are autogenerated.
  //  break;
  //case ISD::AND:
  //  if (tryIndexedBinOp(Node, Node->getOperand(0), Node->getOperand(1),
  //                      WDC65816::AND8rp, WDC65816::AND16rp))
  //    return;
  //  else if (tryIndexedBinOp(Node, Node->getOperand(1), Node->getOperand(0),
  //                           WDC65816::AND8rp, WDC65816::AND16rp))
  //    return;

  //  // Other cases are autogenerated.
  //  break;
  //case ISD::OR:
  //  if (tryIndexedBinOp(Node, Node->getOperand(0), Node->getOperand(1),
  //                      WDC65816::BIS8rp, WDC65816::BIS16rp))
  //    return;
  //  else if (tryIndexedBinOp(Node, Node->getOperand(1), Node->getOperand(0),
  //                           WDC65816::BIS8rp, WDC65816::BIS16rp))
  //    return;

  //  // Other cases are autogenerated.
  //  break;
  //case ISD::XOR:
  //  if (tryIndexedBinOp(Node, Node->getOperand(0), Node->getOperand(1),
  //                      WDC65816::XOR8rp, WDC65816::XOR16rp))
  //    return;
  //  else if (tryIndexedBinOp(Node, Node->getOperand(1), Node->getOperand(0),
  //                           WDC65816::XOR8rp, WDC65816::XOR16rp))
  //    return;

  //  // Other cases are autogenerated.
  //  break;
  //}

  // Select the default instruction
  SelectCode(Node);
}
