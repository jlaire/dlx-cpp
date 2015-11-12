#include "Sudoku.hpp"

Sudoku::Sudoku()
  : Sudoku(SudokuType())
{
}

Sudoku::Sudoku(SudokuType type)
  : Sudoku(type, default_template(type))
{
}

Sudoku::Sudoku(const std::string& str)
  : Sudoku(guess_type(str), str)
{
}

Sudoku::Sudoku(SudokuType type, const std::string& str)
  : type_(std::move(type)),
  template_(parse_template(str)),
  values_(get_values(type_, str))
{
}

std::string Sudoku::default_template(const SudokuType& type) {
  return std::string();
}

std::string Sudoku::default_template() const {
  return default_template(type_);
}

void Sudoku::set_template(const std::string& str) {
  template_ = parse_template(str);
}

bool Sudoku::is_valid() const {
  return true;
}

bool Sudoku::is_solved() const {
  return true;
}

unsigned& Sudoku::operator[](unsigned pos) {
  return values_[pos];
}

unsigned Sudoku::operator[](unsigned pos) const {
  return values_[pos];
}

std::string Sudoku::to_string() const {
  return template_;
}

SudokuType Sudoku::guess_type(const std::string& str) {
  return SudokuType();
}

std::string Sudoku::parse_template(const std::string& str) {
  return str;
}

std::vector<unsigned> Sudoku::get_values(const SudokuType& type, const std::string& str) {
  return {};
}
