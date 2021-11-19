#pragma once

#include <cstdio>
#include <string>
#include <fstream>

void ReadTerminal(std::FILE*& file);
void WriteTerminal(std::FILE*& file);
void ReadFile(std::FILE*& file, const std::string& filename);
void WriteFile(std::FILE*& file, const std::string& filename);
void AppendToFile(std::FILE*& file, const std::string& filename);
void ReadBinaryFile(std::FILE*& file, const std::string& filename);
void WriteBinaryFile(std::FILE*& file, const std::string& filename);
void AppendToBinaryFile(std::FILE*& file, const std::string& filename);
void CloseReadFile(std::FILE* file);
void CloseWriteFile(std::FILE* file);

void ReadFile(std::ifstream& stream, const std::string& filename);
void WriteFile(std::ofstream& stream, const std::string& filename);
void AppendToFile(std::ofstream& stream, const std::string& filename);
void ReadBinaryFile(std::ifstream& stream, const std::string& filename);
void WriteBinaryFile(std::ofstream& stream, const std::string& filename);
void CloseReadFile(std::ifstream& stream);
void CloseWriteFile(std::ofstream& stream);

std::string GetFilesInDirectory(const std::string& path, const std::string& prep = "");
