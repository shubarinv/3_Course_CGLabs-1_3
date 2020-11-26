//
// Created by Vladimir Shubarin on 26.11.2020.
//

#ifndef CGLABS__SPDLOGDUMMY_HPP_
#define CGLABS__SPDLOGDUMMY_HPP_
namespace spdlog {

template<typename FormatString, typename... Args>
void error(const FormatString &fmt, const Args &...args) {
  std::cout << fmt << std::endl;
}
template<typename FormatString, typename... Args>
void info(const FormatString &fmt, const Args &...args) {
  std::cout << fmt << std::endl;
}
template<typename FormatString, typename... Args>
void debug(const FormatString &fmt, const Args &...args) {
  std::cout << fmt << std::endl;
}
template<typename FormatString, typename... Args>
void critical(const FormatString &fmt, const Args &...args) {
  std::cout << fmt << std::endl;
}
template<typename FormatString, typename... Args>
void warn(const FormatString &fmt, const Args &...args) {
  std::cout << fmt << std::endl;
}
}
#endif //CGLABS__SPDLOGDUMMY_HPP_
