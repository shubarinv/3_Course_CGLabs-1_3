//
// Created by Vladimir Shubarin on 02.12.2020.
//
#ifndef CG_LABS_FUNCTIONS_HPP
#define CG_LABS_FUNCTIONS_HPP

#define LOGURU_WITH_STREAMS 1


#include "Libs/loguru.cpp"
typedef Vector3 Vec3;
typedef Vector2 Vec2;
typedef Color4 Color4_;
typedef Matrix4 Mat4;
void logInit([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  loguru::init(argc, argv);
  // Put every log message in "everything.log":
  loguru::add_file("main.log", loguru::Truncate, loguru::Verbosity_MAX);
}

/**
 * @brief checks if program being built on windows
 * @return true if on Windows, false otherwise
 **/
bool isWindows() {
#if defined (__WIN32__)
  return true;
#endif
  return false;
}
/**
 * @brief checks if program being built on macOS
 * @return true if on macOS, false otherwise
 **/
bool isMac() {
#if defined (__APPLE__)
  return true;
#endif
  return false;

}
/**
 * @brief checks if program being built on Linux
 * @return true if on Linux, false otherwise
 **/
bool isLinux() {
#if defined (__unix__)
  return true;
#endif
  return false;
}

#endif // CG_LABS_FUNCTIONS_HPP