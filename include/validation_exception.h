#ifndef VALIDATION_EXCEPTION_H
#define VALIDATION_EXCEPTION_H

#include <exception>
#include <string>

/**
 * @brief Класс ValidationException
 *
 * Этот класс представляет собой пользовательское исключение для ошибок валидации.
 * Он наследуется от std::exception и переопределяет метод what(), возвращая сообщение об ошибке.
 */
class ValidationException final : public std::exception {
public:
  /**
   * @brief Конструктор принимает строку с описанием ошибки.
   *
   * @param message Сообщение об ошибке.
   */
  explicit ValidationException(const std::string& message)
      : message_(message) {}

  /**
   * @brief Возвращает описание ошибки.
   *
   * @return const char* Сообщение об ошибке.
   */
  virtual const char* what() const noexcept override {
    return message_.c_str();
  }

private:
  std::string message_;
};

#endif // VALIDATION_EXCEPTION_H
