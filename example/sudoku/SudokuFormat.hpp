#pragma once

#include "SudokuType.hpp"

#include <string>
#include <vector>

class SudokuFormat
{
public:
  explicit SudokuFormat(std::string format);
  SudokuFormat(std::shared_ptr<SudokuType> type, std::string format);
  static SudokuFormat make_default(std::shared_ptr<SudokuType> type);
  static SudokuFormat make_compact(std::shared_ptr<SudokuType> type);
  static SudokuFormat make_oneline(std::shared_ptr<SudokuType> type);

  SudokuFormat with_labels(std::string labels) const;

  static auto count_cells(const std::string& str) -> unsigned;
  auto get_values(const std::string& str) const -> std::vector<unsigned>;

  auto is_cell(char) const -> bool;
  auto value(char) const -> unsigned;
  auto label(unsigned) const -> char;

  auto to_string() const -> std::string;
  auto to_string(const std::vector<unsigned>& values) const -> std::string;
  auto type() const -> const SudokuType&;

private:
  std::shared_ptr<SudokuType> type_;
  std::string template_;
  std::string labels_;

  static auto is_empty(char c) -> bool;
  static auto is_valid_label(char c) -> bool; 
  static auto valid_labels() -> const std::string&;
  static auto default_labels(unsigned n) -> std::string;
};
