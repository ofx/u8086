#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <tuple>
#include <variant>
#include <numeric>
#include <stdexcept>
#include <string>

#include <gsl/span>

namespace u8086
{
namespace disassembly
{
namespace x86
{

class DecodeException : public std::runtime_error
{
public:
  DecodeException(const std::string &msg)
      : std::runtime_error(msg)
  {
#ifdef _DEBUG
    assert(false && msg)
#endif
  }
};

enum class RegisterName : uint32_t
{
  AX,
  BX,
  CX,
  DX,
  AH,
  AL,
  BH,
  BL,
  CH,
  CL,
  DH,
  DL,
  SI,
  DI,
  BP,
  SP,
  IP,
  CS,
  DS,
  ES,
  SS,
  Flags
};

enum class FlagName : uint32_t
{
  Carry = 0,
  Parity = 2,
  AuxiliaryCarry = 4,
  Zero = 6,
  Sign = 7,
  Trap = 8,
  Interrupt = 9,
  Direction = 10,
  Overflow = 11
};

enum class OperandType
{
  Destination,
  Source,
  Address
};

template <OperandType T, typename S>
struct RegOperand
{
  RegisterName Register;
};

template <OperandType T>
struct DirectAddressOperand
{
  uint16_t Segment;
  uint16_t Pointer;
};

template <OperandType T>
struct AddressOperand
{
  uint16_t Address;
};

template <typename T>
struct Immediate
{
  T Value;
};

template <typename... T>
struct Instruction
{
  const uint8_t Opcode;

  uint32_t Size;

  std::tuple<T...> Ops;

  Instruction(uint8_t opcode, uint32_t size)
      : Opcode(opcode), Size(size) {}

  template <typename O>
  auto &Op()
  {
    return std::get<O>(Ops);
  }
};

template <typename S>
using DstReg = RegOperand<OperandType::Destination, S>;
template <typename S>
using SrcReg = RegOperand<OperandType::Source, S>;

using InstructionEbGb = Instruction<DstReg<uint8_t>,
                                    SrcReg<uint8_t>>;

using InstructionEvGv = Instruction<DstReg<uint16_t>,
                                    SrcReg<uint16_t>>;

using Address = AddressOperand<OperandType::Address>;

using InstructionGbEb = Instruction<DstReg<uint8_t>,
                                    Address>;

using InstructionGvEv = Instruction<DstReg<uint16_t>,
                                    Address>;

using InstructionALIb = Instruction<DstReg<uint8_t>,
                                    Immediate<uint8_t>>;

using InstructionAXIv = Instruction<DstReg<uint16_t>,
                                    Immediate<uint16_t>>;

using InstructionES = Instruction<SrcReg<uint16_t>>;

using DirectAddress = DirectAddressOperand<OperandType::Address>;

using InstructionAp = Instruction<DirectAddress>;

const std::array<RegisterName, 8> g_decodeTable8 = {
    RegisterName::AL, RegisterName::CL, RegisterName::DL, RegisterName::BL,
    RegisterName::AH, RegisterName::CH, RegisterName::DH, RegisterName::BH};

const std::array<RegisterName, 8> g_decodeTable16 = {
    RegisterName::AX, RegisterName::CX, RegisterName::DX, RegisterName::BX,
    RegisterName::SP, RegisterName::BP, RegisterName::SI, RegisterName::DI};

// Read and advance offset
template <typename T>
static T Read(gsl::span<uint8_t> v, uint32_t &offset)
{
  if (offset + sizeof(T) > v.size())
    throw DecodeException("Memory error");

  auto bytes = v.subspan(offset, sizeof(T));

  // Convert to T
  T t = 0;
  uint32_t n = 0;
  for (auto it = bytes.begin(); it != bytes.end(); ++it, n += 8)
  {
    auto byte = *it;
    auto bs = byte << n;
    t |= byte << n;
  }

  // Advance offset
  offset += bytes.size();

  return t;
}

std::variant<InstructionEbGb, InstructionAp, InstructionEvGv, InstructionGbEb, InstructionGvEv, InstructionALIb, InstructionAXIv, InstructionES>
Decode(uint32_t offset, gsl::span<uint8_t> ram)
{
  // Checks if offset is valid wrt ram
  const auto checkOffset = [=]() {
    if (offset >= ram.size() || offset < 0)
      throw DecodeException("Memory error");
  };

  // Initial check
  checkOffset();

  // Acquire opcode, advance offset 1 byte
  uint8_t opcode = Read<uint8_t>(ram, offset);

  // Switch opcodes, decode instruction, each group of opcodes should return
  // an Instruction instance.
  switch (opcode)
  {
  // Eb Gb
  case 0x00:
  case 0x10:
  case 0x20:
  case 0x30:
  case 0x08:
  case 0x18:
  case 0x28:
  case 0x38:
  case 0x88:
  {
    auto instruction = InstructionEbGb(opcode, 2);

    const auto modregrm = Read<uint8_t>(ram, offset);

    instruction.Op<DstReg<uint8_t>>().Register =
        g_decodeTable8[modregrm & 0x7]; // R/M
    instruction.Op<SrcReg<uint8_t>>().Register =
        g_decodeTable8[(modregrm & 0x38) >> 3]; // REG
    return instruction;
  }
  break;
  // Ev Gv
  case 0x01:
  case 0x11:
  case 0x21:
  case 0x31:
  case 0x09:
  case 0x19:
  case 0x29:
  case 0x39:
  case 0x89:
  {
    auto instruction = InstructionEvGv(opcode, 2);

    const auto modregrm = Read<uint8_t>(ram, offset);

    instruction.Op<DstReg<uint16_t>>().Register =
        g_decodeTable16[modregrm & 0x7]; // R/M
    instruction.Op<SrcReg<uint16_t>>().Register =
        g_decodeTable16[(modregrm & 0x38) >> 3]; // REG;
    return instruction;
  }
  break;
  // Ap
  case 0xEA:
  case 0x9A:
  {
    auto instruction = InstructionAp(opcode, 5);
    instruction.Op<DirectAddress>().Pointer = Read<uint16_t>(ram, offset);
    instruction.Op<DirectAddress>().Segment = Read<uint16_t>(ram, offset);
    return instruction;
  }
  break;
  // Gb Eb
  case 0x02:
  case 0x12:
  case 0x22:
  case 0x32:
  case 0x84:
  case 0x86:
  case 0x0A:
  case 0x1A:
  case 0x2A:
  case 0x3A:
  case 0x8A:
  {
    auto instruction = InstructionGbEb(opcode, 4);
    const auto modregrm = Read<uint8_t>(ram, offset);
    instruction.Op<DstReg<uint8_t>>().Register =
        g_decodeTable8[(modregrm & 0x38) >> 3]; // REG
    instruction.Op<Address>().Address = Read<uint16_t>(ram, offset);
    return instruction;
  }
  break;
  // Gv Ev
  case 0x03:
  case 0x13:
  case 0x23:
  case 0x33:
  case 0x85:
  case 0x87:
  case 0x0B:
  case 0x1B:
  case 0x2B:
  case 0x3B:
  case 0x8B:
  {
    auto instruction = InstructionGvEv(opcode, 4);
    const auto modregrm = Read<uint8_t>(ram, offset);
    instruction.Op<DstReg<uint16_t>>().Register =
        g_decodeTable16[(modregrm & 0x38) >> 3]; // REG
    instruction.Op<Address>().Address = Read<uint16_t>(ram, offset);
    return instruction;
  }
  break;
  // AL Ib
  case 0x04:
  case 0x14:
  case 0x24:
  case 0x34:
  case 0xB0:
  case 0xE4:
  case 0x0C:
  case 0x1C:
  case 0x2C:
  case 0x3C:
  case 0xA8:
  {
    auto instruction = InstructionALIb(opcode, 4);
    instruction.Op<DstReg<uint8_t>>().Register = RegisterName::AL;
    instruction.Op<Immediate<uint8_t>>().Value = Read<uint8_t>(ram, offset);
    return instruction;
  }
  break;
  // AX Iv
  case 0x05:
  case 0x15:
  case 0x25:
  case 0x35:
  case 0x0D:
  case 0x1D:
  case 0x2D:
  case 0x3D:
  case 0xA9:
  case 0xB8:
  {
    auto instruction = InstructionAXIv(opcode, 4);
    instruction.Op<DstReg<uint16_t>>().Register = RegisterName::AX;
    instruction.Op<Immediate<uint16_t>>().Value = Read<uint16_t>(ram, offset);
    return instruction;
  }
  // ES
  case 0x06:
  case 0x07:
  {
    auto instruction = InstructionES(opcode, 1);
    instruction.Op<SrcReg<uint16_t>>().Register = RegisterName::ES;
    return instruction;
  }
  break;
  }

  throw DecodeException("Unimplemented");
}

} // namespace x86
} // namespace disassembly
} // namespace u8086