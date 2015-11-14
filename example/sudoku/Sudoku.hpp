#pragma once

#include "SudokuType.hpp"

#include <memory>
#include <string>
#include <vector>

class Sudoku
{
public:
  Sudoku();
  explicit Sudoku(std::shared_ptr<SudokuType> type);
  explicit Sudoku(const std::string& str);
  Sudoku(std::shared_ptr<SudokuType> type, const std::string& str);

  static std::string default_template(const SudokuType& type);
  std::string default_template() const;
  void set_template(const std::string& str);

  const SudokuType& type() const;
  unsigned size() const;

  bool is_valid() const;
  bool is_solved() const;

  unsigned& operator[](unsigned pos);
  unsigned  operator[](unsigned pos) const;

  std::string to_string() const;

private:
  std::shared_ptr<SudokuType> type_;
  std::string template_;
  std::vector<unsigned> values_;

  static auto count_cells(const std::string& str) -> unsigned;
  static auto parse_template(const SudokuType& type, const std::string& str) -> std::string;
  static auto get_values(const SudokuType& type, const std::string& str) -> std::vector<unsigned>;
};
