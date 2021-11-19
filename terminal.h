#pragma once

#include <string>
#include "common_types.h"

namespace Terminal{
namespace Cursor{
std::string Up(u32 n);
std::string Down(u32 n);
std::string Forward(u32 n);
std::string Backward(u32 n);
std::string Move(u32 line, u32 column);

const std::string SavePosition =  "\033[s";
const std::string RestorePosition = "\033[u";
}//namespace Cursor

namespace Display{
const std::string Clear = "\033[2J";
const std::string ClearLine = "\033[K";
const std::string ClearNewLine = "\n\033[K";

std::string ResizeTerminalByChars(u32 rows, u32 cols);
}//namespace Display

namespace Format{
const std::string Clear = "\033[0m";

const std::string Bold = "\033[1m";
const std::string Underline = "\033[4m";
const std::string Blink = "\033[5m";
const std::string Inverse = "\033[7m";

const std::string NoBold = "\033[21m";
const std::string NoUnderline = "\033[24m";
const std::string NoBlink = "\033[25m";
const std::string NoInverse = "\033[27m";
}//namespace Format

namespace Text{
std::string Color(u8 r, u8 g, u8 b);
const std::string Clear = "\033[39m";
}//namespace Text

namespace Background{
std::string Color(u8 r, u8 g, u8 b);
std::string LineColor(u8 r, u8 g, u8 b);
const std::string Clear = "\033[49m";
}//namespace Background

#ifdef _WIN32
void EnableEscapeSequences();
#endif //_WIN32
}//namespace Terminal
