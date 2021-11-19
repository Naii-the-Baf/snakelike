#include <algorithm>
#include "terminal.h"

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif //_WIN32

namespace Terminal{

//Cursor
std::string Cursor::Up(u32 n){
    return {"\033["+std::to_string(n)+"A"};
}

std::string Cursor::Down(u32 n){
    return {"\033["+std::to_string(n)+"B"};
}

std::string Cursor::Forward(u32 n){
    return {"\033["+std::to_string(n)+"C"};
}

std::string Cursor::Backward(u32 n){
    return {"\033["+std::to_string(n)+"D"};
}

std::string Cursor::Move(u32 line, u32 column){
    return {"\033["+std::to_string(line)+";"+std::to_string(column)+"H"};
}

//Display
std::string Display::ResizeTerminalByChars(u32 rows, u32 cols){
    return {"\033[8;"+std::to_string(rows)+";"+std::to_string(cols)+"t"};
}

//Text
std::string Text::Color(u8 r, u8 g, u8 b){
    r = std::min(r, (u8)0xFF);
    g = std::min(g, (u8)0xFF);
    b = std::min(b, (u8)0xFF);
    return {"\033[38;2;"+std::to_string(r)+";"+std::to_string(g)+";"+std::to_string(b)+"m"};
}

//Background
std::string Background::Color(u8 r, u8 g, u8 b){
    r = std::min(r, (u8)0xFF);
    g = std::min(g, (u8)0xFF);
    b = std::min(b, (u8)0xFF);
    return {"\033[48;2;"+std::to_string(r)+";"+std::to_string(g)+";"+std::to_string(b)+"m"};
}

std::string Background::LineColor(u8 r, u8 g, u8 b){
    r = std::min(r, (u8)0xFF);
    g = std::min(g, (u8)0xFF);
    b = std::min(b, (u8)0xFF);
    return {"\033[48;2;"+std::to_string(r)+";"+std::to_string(g)+";"+std::to_string(b)+"m\033[K"};
}

#ifdef _WIN32
void EnableEscapeSequences(){
    HANDLE hConsole_c = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hConsole_c, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole_c, dwMode);
}
#endif //_WIN32

}//namespace Terminal
