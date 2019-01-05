//===-- WDC65816InstPrinter.cpp - Convert WDC65816 MCInst to assembly syntax --===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class prints an WDC65816 MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "WDC65816InstPrinter.h"
#include "WDC65816.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
using namespace llvm;

#define DEBUG_TYPE "asm-printer"

// Include the auto-generated portion of the assembly writer.
#define PRINT_ALIAS_INSTR
#include "WDC65816GenAsmWriter.inc"

void WDC65816InstPrinter::printInst(const MCInst *MI, raw_ostream &O,
                                  StringRef Annot, const MCSubtargetInfo &STI) {
  if (!printAliasInstr(MI, O))
    printInstruction(MI, O);
  printAnnotation(O, Annot);
}

void WDC65816InstPrinter::printPCRelImmOperand(const MCInst *MI, unsigned OpNo,
                                             raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isImm()) {
    int64_t Imm = Op.getImm() * 2 + 2;
    O << "$";
    if (Imm >= 0)
      O << '+';
    O << Imm;
  } else {
    assert(Op.isExpr() && "unknown pcrel immediate operand");
    Op.getExpr()->print(O, &MAI);
  }
}

void WDC65816InstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                     raw_ostream &O, const char *Modifier) {
  assert((Modifier == nullptr || Modifier[0] == 0) && "No modifiers supported");
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg()) {
    O << getRegisterName(Op.getReg());
  } else if (Op.isImm()) {
    O << '#' << Op.getImm();
  } else {
    assert(Op.isExpr() && "unknown operand kind in printOperand");
    O << '#';
    Op.getExpr()->print(O, &MAI);
  }
}

void WDC65816InstPrinter::printSrcMemOperand(const MCInst *MI, unsigned OpNo,
                                           raw_ostream &O,
                                           const char *Modifier) {
  //const MCOperand &Base = MI->getOperand(OpNo);
  //const MCOperand &Disp = MI->getOperand(OpNo+1);

  // Print displacement first

  // If the global address expression is a part of displacement field with a
  // register base, we should not emit any prefix symbol here, e.g.
  //   mov.w &foo, r1
  // vs
  //   mov.w glb(r1), r2
  // Otherwise (!) WDC65816-as will silently miscompile the output :(

  // WDC65816-TODO - printSrcMemOperand
  //if (Base.getReg() == WDC65816::SR)
  //  O << '&';

  //if (Disp.isExpr())
  //  Disp.getExpr()->print(O, &MAI);
  //else {
  //  assert(Disp.isImm() && "Expected immediate in displacement field");
  //  O << Disp.getImm();
  //}

  //// Print register base field
  //if ((Base.getReg() != WDC65816::SR) &&
  //    (Base.getReg() != WDC65816::PC))
  //  O << '(' << getRegisterName(Base.getReg()) << ')';
}

void WDC65816InstPrinter::printIndRegOperand(const MCInst *MI, unsigned OpNo,
                                           raw_ostream &O) {
  const MCOperand &Base = MI->getOperand(OpNo);
  O << "@" << getRegisterName(Base.getReg());
}

void WDC65816InstPrinter::printPostIndRegOperand(const MCInst *MI, unsigned OpNo,
                                               raw_ostream &O) {
  const MCOperand &Base = MI->getOperand(OpNo);
  O << "@" << getRegisterName(Base.getReg()) << "+";
}

void WDC65816InstPrinter::printCCOperand(const MCInst *MI, unsigned OpNo,
                                       raw_ostream &O) {
  unsigned CC = MI->getOperand(OpNo).getImm();

  switch (CC) {
  default:
   llvm_unreachable("Unsupported CC code");
  case WDC65816CC::COND_E:
   O << "eq";
   break;
  case WDC65816CC::COND_NE:
   O << "ne";
   break;
  case WDC65816CC::COND_HS:
   O << "hs";
   break;
  case WDC65816CC::COND_LO:
   O << "lo";
   break;
  case WDC65816CC::COND_GE:
   O << "ge";
   break;
  case WDC65816CC::COND_L:
   O << 'l';
   break;
  case WDC65816CC::COND_N:
   O << 'n';
   break;
  }
}
