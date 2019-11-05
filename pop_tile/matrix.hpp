#pragma once

template <typename T>
struct matrix {
  struct base_iterator {
    base_iterator(matrix& m, size_t i) noexcept : matrix_{m}, index_{i} {}

    T& operator*() noexcept { return matrix_[index_]; }

    void move_to_next_column() noexcept { ++index_; }
    void move_to_next_row() noexcept { index_ += matrix_.columns(); }

    friend bool operator==(const base_iterator& lhs,
                           const base_iterator& rhs) noexcept {
      return &lhs.matrix_ == &rhs.matrix_ && lhs.index_ == rhs.index_;
    }
    friend bool operator!=(const base_iterator& lhs,
                           const base_iterator& rhs) noexcept {
      return !(lhs == rhs);
    }

   private:
    matrix& matrix_;
    size_t index_;
  };

  struct row_iterator : base_iterator {
    using base_iterator::base_iterator;

    row_iterator& operator++() noexcept {
      this->move_to_next_column();
      return *this;
    }
  };

  struct column_iterator : base_iterator {
    using base_iterator::base_iterator;

    column_iterator& operator++() noexcept {
      this->move_to_next_row();
      return *this;
    }
  };

  struct base_view {
    base_view(matrix& m, size_t i) noexcept : matrix_{m}, index_{i} {}

    row_iterator begin_of_row() const noexcept {
      return row_iterator{matrix_, index_ * matrix_.columns()};
    }
    row_iterator end_of_row() const noexcept {
      return row_iterator{matrix_, (index_ + 1) * matrix_.columns()};
    }
    column_iterator begin_of_column() const noexcept {
      return column_iterator{matrix_, index_};
    }
    column_iterator end_of_column() const noexcept {
      return column_iterator{matrix_,
                             matrix_.rows() * matrix_.columns() + index_};
    }

   private:
    matrix& matrix_;
    size_t index_;
  };

  struct row_view : base_view {
    using base_view::base_view;

    row_iterator begin() const noexcept { return this->begin_of_row(); }
    row_iterator end() const noexcept { return this->end_of_row(); }
  };

  struct column_view : base_view {
    using base_view::base_view;

    column_iterator begin() const noexcept { return this->begin_of_column(); }
    column_iterator end() const noexcept { return this->end_of_column(); }
  };

  struct position {
    int row;
    int column;
  };

  matrix(size_t rows, size_t columns) noexcept
      : rows_{rows}, columns_{columns}, elements_(rows * columns, T{}) {}

  size_t rows() const noexcept { return rows_; }
  size_t columns() const noexcept { return columns_; }

  const T& operator[](position pos) const noexcept {
    return elements_[index_of(pos.row, pos.column)];
  }
  T& operator[](position pos) noexcept {
    return elements_[index_of(pos.row, pos.column)];
  }

  const T& operator[](size_t index) const noexcept { return elements_[index]; }
  T& operator[](size_t index) noexcept { return elements_[index]; }

  template <typename F>
  void for_each_row(F&& f) noexcept {
    for (size_t r = 0; r < rows_; ++r) {
      std::forward<F>(f)(row_view{*this, r});
    }
  }

  template <typename F>
  void for_each_column(F&& f) noexcept {
    for (size_t c = 0; c < columns_; ++c) {
      std::forward<F>(f)(column_view{*this, c});
    }
  }

  auto begin() noexcept { return elements_.begin(); }
  auto end() noexcept { return elements_.end(); }
  auto cbegin() const noexcept { return elements_.cbegin(); }
  auto cend() const noexcept { return elements_.cend(); }

 private:
  size_t index_of(size_t row, size_t column) const noexcept {
    return row * columns_ + column;
  }

  size_t rows_;
  size_t columns_;
  std::vector<T> elements_;
};