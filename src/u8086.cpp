#include "disassembler_8086.h"

#include <getopt.h>

#include <chaiscript/chaiscript.hpp>

#include <array>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <ostream>
#include <string>
#include <unordered_map>

namespace u8086 {
namespace emulation {

class EmulationException : public std::runtime_error
{
public:
  EmulationException(const std::string& what)
    : std::runtime_error(what)
  {}
};

using namespace disassembly::x86;

class Cpu
{
public:
  virtual ~Cpu() {}

  virtual void PrintRegisters() const noexcept = 0;

  virtual void PrintFlags() noexcept = 0;

  virtual void Cycle() = 0;
};

class Cpu8086 : public Cpu
{
public:
  enum class Exception
  {
    GP,
    SS,
    PF
  };

  template<Exception E>
  class Cpu8086Exception
  {
  public:
    Cpu8086Exception() {}
  };

  Cpu8086(std::vector<uint8_t>& ram)
    : m_Ram(ram)
  {
    Initialize();
  }

  void PrintRegisters() const noexcept override
  {
    const std::unordered_map<RegisterName, std::string> names = {
      { RegisterName::AX, "AX" }, { RegisterName::BX, "BX" },
      { RegisterName::CX, "CX" }, { RegisterName::DX, "DX" },
      { RegisterName::SI, "SI" }, { RegisterName::DI, "DI" },
      { RegisterName::BP, "BP" }, { RegisterName::SP, "SP" },
      { RegisterName::IP, "IP" }, { RegisterName::CS, "CS" },
      { RegisterName::DS, "DS" }, { RegisterName::ES, "ES" },
      { RegisterName::SS, "SS" }
    };

    std::cout << "[AH] " << std::hex << std::setw(2)
              << static_cast<uint32_t>(
                   m_Registers.at(RegisterName::AX).HighByte)
              << " [AL] " << std::hex << std::setw(2)
              << static_cast<uint32_t>(m_Registers.at(RegisterName::AX).LowByte)
              << std::endl;
    std::cout << "[BH] " << std::hex << std::setw(2)
              << static_cast<uint32_t>(
                   m_Registers.at(RegisterName::BX).HighByte)
              << " [BL] " << std::hex << std::setw(2)
              << static_cast<uint32_t>(m_Registers.at(RegisterName::BX).LowByte)
              << std::endl;
    std::cout << "[CH] " << std::hex << std::setw(2)
              << static_cast<uint32_t>(
                   m_Registers.at(RegisterName::CX).HighByte)
              << " [CL] " << std::hex << std::setw(2)
              << static_cast<uint32_t>(m_Registers.at(RegisterName::CX).LowByte)
              << std::endl;
    std::cout << "[DH] " << std::hex << std::setw(2)
              << static_cast<uint32_t>(
                   m_Registers.at(RegisterName::DX).HighByte)
              << " [DL] " << std::hex << std::setw(2)
              << static_cast<uint32_t>(m_Registers.at(RegisterName::DX).LowByte)
              << std::endl;

    for (const auto pair : names) {
      std::cout << "[" << pair.second << "] " << std::hex << std::setw(2)
                << static_cast<uint32_t>(m_Registers.at(pair.first).Word)
                << std::endl;
    }
  }

  void PrintFlags() noexcept override
  {
    std::cout << "[CF] " << static_cast<bool>(Flag{ this, FlagName::Carry })
              << std::endl;
    std::cout << "[PF] " << static_cast<bool>(Flag{ this, FlagName::Parity })
              << std::endl;
    std::cout << "[AF] "
              << static_cast<bool>(Flag{ this, FlagName::AuxiliaryCarry })
              << std::endl;
    std::cout << "[ZF] " << static_cast<bool>(Flag{ this, FlagName::Zero })
              << std::endl;
    std::cout << "[SF] " << static_cast<bool>(Flag{ this, FlagName::Sign })
              << std::endl;
    std::cout << "[TF] " << static_cast<bool>(Flag{ this, FlagName::Trap })
              << std::endl;
    std::cout << "[IF] " << static_cast<bool>(Flag{ this, FlagName::Interrupt })
              << std::endl;
    std::cout << "[DF] " << static_cast<bool>(Flag{ this, FlagName::Direction })
              << std::endl;
    std::cout << "[OF] " << static_cast<bool>(Flag{ this, FlagName::Overflow })
              << std::endl;
  }

  class Execute
  {
  public:
    Execute(Cpu8086* t)
      : Cpu8086_this(t)
    {}

    void operator()(InstructionES& instruction)
    {
      const auto& srcReg =
        Cpu8086_this->RegisterWord(instruction.Op<SrcReg<uint16_t>>().Register);

      switch (instruction.Opcode) {
        // PUSH
        case 0x06:

          break;
        // POP
        case 0x07:

          break;
        default:
          throw EmulationException("Unimplemented opcode: " +
                                   std::to_string(instruction.Opcode));
      }

      // Advance instruction pointer
      Cpu8086_this->RegisterWord(RegisterName::IP) += instruction.Size;
    }

    void operator()(InstructionZvIv& instruction)
    {
      auto& dstReg =
        Cpu8086_this->RegisterWord(instruction.Op<DstReg<uint16_t>>().Register);
      const auto immediate = instruction.Op<Immediate<uint16_t>>().Value;

      switch (instruction.Opcode) {
        // ADD
        case 0x05:
          Cpu8086_this->Add(dstReg, immediate);
          break;
        // ADC
        case 0x15:

          break;
        // AND
        case 0x25:

          break;
        // XOR
        case 0x35:

          break;
        // OR
        case 0x0D:

          break;
        // SBB
        case 0x1D:

          break;
        // SUB
        case 0x2D:

          break;
        // CMP
        case 0x3D:

          break;
        // TEST
        case 0xA9:

          break;
        // MOV
        case 0xB8:
        case 0xB9:
        case 0xBA:
        case 0xBB:
        case 0xBC:
        case 0xBD:
        case 0xBE:
        case 0xBF:
          Cpu8086_this->Move(dstReg, immediate);
          break;
        default:
          throw EmulationException("Unimplemented opcode: " +
                                   std::to_string(instruction.Opcode));
      }

      // Advance instruction pointer
      Cpu8086_this->RegisterWord(RegisterName::IP) += instruction.Size;
    }

    void operator()(InstructionZbIb& instruction)
    {
      auto& dstReg =
        Cpu8086_this->RegisterByte(instruction.Op<DstReg<uint8_t>>().Register);
      const auto immediate = instruction.Op<Immediate<uint8_t>>().Value;

      switch (instruction.Opcode) {
        // ADD
        case 0x04:
          Cpu8086_this->Add(dstReg, immediate);
          break;
        // ADC
        case 0x14:

          break;
        // AND
        case 0x24:

          break;
        // XOR
        case 0x34:

          break;
        // MOV
        case 0xB0:
        case 0xB1:
        case 0xB2:
        case 0xB3:
        case 0xB4:
        case 0xB5:
        case 0xB6:
        case 0xB7:
          Cpu8086_this->Move(dstReg, immediate);
          break;
        // IN
        case 0xE4:

          break;
        // OR
        case 0x0C:

          break;
        // SBB
        case 0x1C:

          break;
        // SUB
        case 0x2C:

          break;
        // CMP
        case 0x3C:

          break;
        // TEST
        case 0xA8:

          break;
        default:
          throw EmulationException("Unimplemented opcode: " +
                                   std::to_string(instruction.Opcode));
      }

      // Advance instruction pointer
      Cpu8086_this->RegisterWord(RegisterName::IP) += instruction.Size;
    }

    void operator()(InstructionGvEv& instruction)
    {
      auto& dstReg =
        Cpu8086_this->RegisterWord(instruction.Op<DstReg<uint16_t>>().Register);
      const auto& src =
        Cpu8086_this->Ram<uint16_t>(instruction.Op<Address>().Address);

      switch (instruction.Opcode) {
        // ADD
        case 0x03:
          Cpu8086_this->Add(dstReg, src);
          break;
        // ADC
        case 0x13:

          break;
        // AND
        case 0x23:

          break;
        // XOR
        case 0x33:

          break;
        // TEST
        case 0x85:

          break;
        // XCHG
        case 0x87:

          break;
        // OR
        case 0x0B:

          break;
        // SBB
        case 0x1B:

          break;
        // SUB
        case 0x2B:

          break;
        // CMP
        case 0x3B:

          break;
        // MOV
        case 0x8B:
          Cpu8086_this->Move(dstReg, src);
          break;
        default:
          throw EmulationException("Unimplemented opcode: " +
                                   std::to_string(instruction.Opcode));
      }

      // Advance instruction pointer
      Cpu8086_this->RegisterWord(RegisterName::IP) += instruction.Size;
    }

    void operator()(InstructionGbEb& instruction)
    {
      auto& dstReg =
        Cpu8086_this->RegisterByte(instruction.Op<DstReg<uint8_t>>().Register);
      const auto src =
        Cpu8086_this->Ram<uint8_t>(instruction.Op<Address>().Address);

      switch (instruction.Opcode) {
        // ADD
        case 0x02:
          Cpu8086_this->Add(dstReg, src);
          break;
        // ADC
        case 0x12:

          break;
        // AND
        case 0x22:

          break;
        // XOR
        case 0x32:

          break;
        // TEST
        case 0x84:

          break;
        // XCHG
        case 0x86:

          break;
        // OR
        case 0x0A:

          break;
        // SBB
        case 0x1A:

          break;
        // SUB
        case 0x2A:

          break;
        // CMP
        case 0x3A:

          break;
        // MOV
        case 0x8A:
          Cpu8086_this->Move(dstReg, src);
          break;
        // CMP
        default:
          throw EmulationException("Unimplemented opcode: " +
                                   std::to_string(instruction.Opcode));
      }

      // Advance instruction pointer
      Cpu8086_this->RegisterWord(RegisterName::IP) += instruction.Size;
    }

    void operator()(InstructionAp& instruction)
    {
      const auto& cs = instruction.Op<DirectAddress>().Segment;
      const auto& ip = instruction.Op<DirectAddress>().Pointer;

      switch (instruction.Opcode) {
        // JMP
        case 0xEA:
          // TODO: Add exceptions
          // TODO: Check # cycles

          Cpu8086_this->RegisterWord(RegisterName::CS) = cs;
          Cpu8086_this->RegisterWord(RegisterName::IP) = ip;
          break;
        // CALL
        case 0x9A:

          break;
        default:
          throw EmulationException("Unimplemented opcode: " +
                                   std::to_string(instruction.Opcode));
      }
    }

    void operator()(InstructionEvGv& instruction)
    {
      auto& dstReg =
        Cpu8086_this->RegisterWord(instruction.Op<DstReg<uint16_t>>().Register);
      const auto& srcReg =
        Cpu8086_this->RegisterWord(instruction.Op<SrcReg<uint16_t>>().Register);

      switch (instruction.Opcode) {
        // ADD
        case 0x01:
          Cpu8086_this->Add(dstReg, srcReg);
          break;
        // ADC
        case 0x11:

          break;
        // AND
        case 0x21:

          break;
        // XOR
        case 0x31:

          break;
        // OR
        case 0x09:

          break;
        // SBB
        case 0x19:

          break;
        // SUB
        case 0x29:

          break;
        // CMP
        case 0x39:

          break;
        // MOV
        case 0x89:
          Cpu8086_this->Move(dstReg, srcReg);
          break;
        default:
          throw EmulationException("Unimplemented opcode: " +
                                   std::to_string(instruction.Opcode));
      }

      // Advance instruction pointer
      Cpu8086_this->RegisterWord(RegisterName::IP) += instruction.Size;
    }

    void operator()(InstructionEbGb& instruction)
    {
      auto& dstReg =
        Cpu8086_this->RegisterByte(instruction.Op<DstReg<uint8_t>>().Register);
      const auto& srcReg =
        Cpu8086_this->RegisterByte(instruction.Op<SrcReg<uint8_t>>().Register);

      switch (instruction.Opcode) {
        // ADD
        case 0x00:
          Cpu8086_this->Add(dstReg, srcReg);
          break;
        // ADC
        case 0x10:

          break;
        // AND
        case 0x20:

          break;
        // XOR
        case 0x30:

          break;
        // OR
        case 0x08:

          break;
        // SBB
        case 0x18:

          break;
        // SUB
        case 0x28:

          break;
        // CMP
        case 0x38:

          break;
        // MOV
        case 0x88:
          Cpu8086_this->Move(dstReg, srcReg);
          break;
        default:
          throw EmulationException("Unimplemented opcode: " +
                                   std::to_string(instruction.Opcode));
      }

      // Advance instruction pointer
      Cpu8086_this->RegisterWord(RegisterName::IP) += instruction.Size;
    }

  private:
    Cpu8086* Cpu8086_this;
  };

  void Cycle() override
  {
    // Fetch
    uint32_t offset = (m_Registers.at(RegisterName::CS).Word << 4) +
                      m_Registers.at(RegisterName::IP).Word;

    if (offset > 0xfffff) {
      throw EmulationException("Incorrect address: " + std::to_string(offset));
    }

    // Decode
    auto instruction = Decode(offset, m_Ram);

    // Execute
    Execute execute{ this };
    std::visit(execute, instruction);
  }

private:
  template<typename T>
  void Add(T& dst, const T& src)
  {
    static_assert(std::is_unsigned_v<T>);
    std::make_signed_t<T> res = dst + src;

    Flag{ this, FlagName::Zero } = res == 0;
    Flag{ this, FlagName::Sign } = res < 0;
    Flag{ this, FlagName::Overflow } =
      ((dst > 0) && (src > std::numeric_limits<T>::max() - dst)) ||
      ((dst < 0) && (src < std::numeric_limits<T>::min() - dst));
    Flag{ this, FlagName::Carry } = res > 0xFFFF;
    Flag{ this, FlagName::AuxiliaryCarry } = res > 0xF;

    dst = res;
  }

  template<typename T>
  void Subtract(T& dst, const T& src)
  {
    static_assert(std::is_unsigned_v<T>);
    std::make_signed_t<T> res = dst - src;

    Flag{ this, FlagName::Zero } = res == 0;
    Flag{ this, FlagName::Sign } = res < 0;
    Flag{ this, FlagName::Overflow } =
      ((dst < 0) && (src > std::numeric_limits<T>::max() + dst)) ||
      ((dst > 0) && (src < std::numeric_limits<T>::min() + dst));
    Flag{ this, FlagName::Carry } = !(res > 0xFFFF);
    Flag{ this, FlagName::AuxiliaryCarry } = !(res > 0xF);

    dst = res;
  }

  template<typename T>
  void Divide(T& dst, const T& src)
  {
    static_assert(std::is_unsigned_v<T>);
    std::make_signed_t<T> res = dst / src;

    Flag{ this, FlagName::Zero } = res == 0;
    Flag{ this, FlagName::Sign } = res < 0;
    Flag{ this, FlagName::Overflow } = false;
    Flag{ this, FlagName::Carry } = res < 0;

    dst = res;
  }

  template<typename T>
  void Multiply(T& dst, const T& src)
  {
    static_assert(std::is_unsigned_v<T>);
    std::make_signed_t<T> res = dst * src;

    Flag{ this, FlagName::Zero } = res == 0;
    Flag{ this, FlagName::Sign } = res < 0;
    Flag{ this, FlagName::Overflow } =
      (src > std::numeric_limits<T>::max() / dst) ||
      (src < std::numeric_limits<T>::min() / dst);
    Flag{ this, FlagName::Carry } = res < 0;

    dst = res;
  }

  template<typename T>
  void Move(T& dst, const T& src)
  {
    dst = src;
  }

private:
  union Register
  {
    struct
    {
      uint8_t HighByte;
      uint8_t LowByte;
    };
    uint16_t Word;
  };
  std::unordered_map<RegisterName, Register> m_Registers = {
    { RegisterName::AX, {} }, { RegisterName::BX, {} },
    { RegisterName::CX, {} }, { RegisterName::DX, {} },
    { RegisterName::SI, {} }, { RegisterName::DI, {} },
    { RegisterName::BP, {} }, { RegisterName::SP, {} },
    { RegisterName::IP, {} }, { RegisterName::CS, {} },
    { RegisterName::DS, {} }, { RegisterName::ES, {} },
    { RegisterName::SS, {} }, { RegisterName::Flags, {} }
  };

  class Flag
  {
  public:
    Flag(Cpu8086* t, FlagName name)
      : Cpu8086_this(t)
      , Name(name)
    {}

    void operator=(bool b)
    {
      const auto bit = static_cast<uint32_t>(Name);

      if (b)
        Cpu8086_this->RegisterWord(RegisterName::Flags) |= 1 << bit;
      else
        Cpu8086_this->RegisterWord(RegisterName::Flags) &= ~(1 << bit);
    }

    operator bool()
    {
      const auto bit = static_cast<uint32_t>(Name);

      return Cpu8086_this->RegisterWord(RegisterName::Flags) & (1 << bit);
    }

  private:
    Cpu8086* Cpu8086_this;

    const FlagName Name;
  };

  uint8_t& RegisterByte(RegisterName name)
  {
    switch (name) {
      case RegisterName::AH:
        return m_Registers.at(RegisterName::AX).HighByte;
      case RegisterName::AL:
        return m_Registers.at(RegisterName::AX).LowByte;
      case RegisterName::BH:
        return m_Registers.at(RegisterName::BX).HighByte;
      case RegisterName::BL:
        return m_Registers.at(RegisterName::BX).LowByte;
      case RegisterName::CH:
        return m_Registers.at(RegisterName::CX).HighByte;
      case RegisterName::CL:
        return m_Registers.at(RegisterName::CX).LowByte;
      case RegisterName::DH:
        return m_Registers.at(RegisterName::DX).HighByte;
      case RegisterName::DL:
        return m_Registers.at(RegisterName::DX).LowByte;
      default:
        throw EmulationException("Register '" +
                                 std::to_string(static_cast<uint32_t>(name)) +
                                 " is not a byte register");
    }
  }

  uint16_t& RegisterWord(RegisterName name)
  {
    switch (name) {
      case RegisterName::AX:
      case RegisterName::BX:
      case RegisterName::CX:
      case RegisterName::DX:
      case RegisterName::SI:
      case RegisterName::DI:
      case RegisterName::BP:
      case RegisterName::SP:
      case RegisterName::IP:
      case RegisterName::CS:
      case RegisterName::DS:
      case RegisterName::ES:
      case RegisterName::SS:
      case RegisterName::Flags:
        return m_Registers.at(name).Word;
      default:
        throw EmulationException("Register '" +
                                 std::to_string(static_cast<uint32_t>(name)) +
                                 " is not a word register");
    }
  }

  template<typename T>
  T& Ram(uint16_t address)
  {
    return *reinterpret_cast<T*>(&m_Ram[address]);
  }

  void Initialize()
  {
    m_Registers.at(RegisterName::CS).Word = 0xFFFF;
    m_Registers.at(RegisterName::IP).Word = 0x0000;
  }

  std::vector<uint8_t>& m_Ram;
};

class Emulator
{
public:
  Emulator(const std::string& cpuType,
           uint32_t ramSize,
           const std::string& romPath)
  {
    if (cpuType == "8086") {
      m_Cpu = std::make_unique<Cpu8086>(m_Ram);
    } else {
      throw EmulationException("Unknown cpu type: " + cpuType);
    }

    m_Ram.resize(ramSize);

    ReadRom(romPath);
    Shadow();
  }

  std::vector<uint8_t> Rom() noexcept { return m_Rom; }

  std::vector<uint8_t> Ram() noexcept { return m_Ram; }

  void PrintRegisters() noexcept { m_Cpu->PrintRegisters(); }

  void PrintFlags() noexcept { m_Cpu->PrintFlags(); }

  void Step() noexcept
  {
    try {
      m_Cpu->Cycle();
    } catch (const EmulationException& ex) {
      std::cout << "Exception in CPU cycle: " << ex.what() << std::endl;
    }
  }

private:
  std::vector<uint8_t> m_Ram;
  std::vector<uint8_t> m_Rom;

  std::unique_ptr<Cpu> m_Cpu;

  void Shadow() noexcept
  {
    auto it = m_Ram.end() - 1 - m_Rom.size();
    std::for_each(m_Rom.begin(), m_Rom.end(), [&it](auto b) {
      *it = b;
      ++it;
    });
  }

  void ReadRom(const std::string& path)
  {
    std::ifstream inf(path,
                      std::fstream::in | std::ios::binary | std::ios::ate);
    if (!inf.good()) {
      throw EmulationException("Failed to read ROM file: " + path);
    }

    const auto romSize = inf.tellg();
    std::cout << "ROM file size: " << inf.tellg() << std::endl;
    m_Rom.resize(romSize);

    auto a = m_Rom.size();

    inf.clear();
    inf.seekg(0, std::ios_base::beg);

    inf.read(reinterpret_cast<char*>(m_Rom.data()), m_Rom.size());
  }
};

} // namespace emulation

namespace interface {

using namespace chaiscript;
using namespace emulation;

struct Static
{
  static bool ShouldQuit;

  static void PrintMemory(const std::vector<uint8_t>& memory) noexcept
  {
    const uint32_t width = 40;
    uint32_t offset = 0;
    const uint32_t alignment =
      std::log(static_cast<uint32_t>(memory.size() / width) * width) /
        std::log(16) +
      1;
    std::for_each(
      memory.begin(), memory.end(), [&offset, alignment](uint8_t v) {
        if (offset % width == 0) {
          if (offset != 0) {
            std::cout << std::endl;
          }
          std::cout << std::hex << "[" << std::setw(alignment) << offset << "]";
        }

        std::cout << " " << std::hex << std::setw(2)
                  << static_cast<uint32_t>(v);
        ++offset;
      });

    std::cout << std::endl;
  }

  static void Quit() noexcept { ShouldQuit = true; }
};

bool Static::ShouldQuit = false;

class Interface
{
public:
  Interface() noexcept
  {
    // Emulator object
    {
      ModulePtr m = ModulePtr(new chaiscript::Module());
      utility::add_class<Emulator>(
        *m,
        "Emulator",
        { constructor<Emulator(
          const std::string&, uint32_t, const std::string&)>() },
        { { fun(&Emulator::Step), "Step" },
          { fun(&Emulator::Ram), "Ram" },
          { fun(&Emulator::Rom), "Rom" },
          { fun(&Emulator::PrintRegisters), "PrintRegisters" },
          { fun(&Emulator::PrintFlags), "PrintFlags" } });
      m_ChaiScript.add(m);
    }

    // Functions
    {
      m_ChaiScript.add(fun(&Static::PrintMemory), "print_memory");
      m_ChaiScript.add(fun(&Static::Quit), "quit");
    }
  }

  void operator()(const std::string& line) noexcept
  {
    try {
      m_ChaiScript(line);
    } catch (const exception::eval_error& e) {
      std::cerr << e.what() << std::endl;
    } catch (const EmulationException& e) {
      std::cerr << e.what() << std::endl;
    }
  }

  void File(const std::string& path) noexcept
  {
    try {
      m_ChaiScript.eval_file(path);
    } catch (const exception::file_not_found_error& e) {
      std::cerr << e.what() << std::endl;
    } catch (const exception::eval_error& e) {
      std::cerr << e.what() << std::endl;
    }
  }

private:
  chaiscript::ChaiScript m_ChaiScript;
};

} // namespace interface
} // namespace u8086

using namespace u8086::interface;

enum class Mode
{
  Interactive,
  Script
};

void
usage()
{
  std::cout << "Usage: u8086 [-f script-path]" << std::endl
            << "\tf: Run in script mode, load and run specified by script-path"
            << std::endl;
}

int
main(const int argc, char* const argv[]) noexcept
{
  std::cout << "μ8086" << std::endl;

  Mode mode = Mode::Interactive;

  std::string scriptFile = "";

  // Parse arguments
  {
    int opt;
    while ((opt = getopt(argc, argv, "f:")) != -1) {
      switch (opt) {
        // File mode
        case 'f':
          mode = Mode::Script;
          scriptFile = optarg;
          break;
        default:
          usage();
          return EXIT_FAILURE;
      }
    }
  }

  Interface interface;

  switch (mode) {
    case Mode::Interactive: {
      for (std::string line; !Static::ShouldQuit && std::cout << ">>> " &&
                             std::getline(std::cin, line);) {
        interface(line);
      }
    } break;
    case Mode::Script: {
      interface.File(scriptFile);
    } break;
  }

  return EXIT_SUCCESS;
}
