#pragma once

#include <um2/config.hpp>

#include <chrono>
#include <string>
#include <vector>

namespace um2
{

//==============================================================================
// LOG
//==============================================================================
// A simple logger class for use in host code only.
// The logger can be configured to:
//  - log messages of different verbosity levels
//  - buffer messages until a certain threshold is reached
//  - prefix messages with a timestamp
//  - colorize messages based on their verbosity level
//  - exit the program after an error is logged (or not)
//
// The logger is not thread-safe.
// Verbosity levels may be seen below in the LogVerbosity enum.

enum class LogVerbosity {
  Off = 0,   // no messages
  Error = 1, // only errors
  Warn = 2,  // errors and warnings
  Info = 3,  // errors, warnings and info
  Debug = 4, // errors, warnings, info and debug
  Trace = 5, // errors, warnings, info, debug and trace
};

using LogClock = std::chrono::system_clock;
using LogTimePoint = std::chrono::time_point<LogClock>;
using LogDuration = std::chrono::duration<double>;

// We don't want to use the default constructor for Log, nor do we want
// to call the destructor without flushing the messages first, but otherwise
// have no need for the rule of 5
// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions) justified
class Log
{

  // Options
  // We need the global log options to be accessible from anywhere in the code
  // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables) justified
  static LogVerbosity max_verbosity_level;
  static bool buffered;          // messages are buffered until flush() is called
  static bool timestamped;       // messages are prefixed with a timestamp
  static bool colorized;         // messages are colorized based on their verbosity level
  static bool exit_on_error;     // the program exits after an error is logged
  static size_t flush_threshold; // flush after this many messages

  // Data
  static LogTimePoint start_time;
  static size_t num_errors;
  static size_t num_warnings;
  static std::vector<LogVerbosity> verbosity_levels;
  static std::vector<LogTimePoint> times;
  static std::vector<std::string> messages;
  // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

  // -- Methods --

  static void
  handleMessage(LogVerbosity verbosity, std::string const & msg);

public:
  Log() = delete;

  static void
  reset();

  // -- Setters --

  static void
  setMaxVerbosityLevel(LogVerbosity val);
  static void
  setBuffered(bool val);
  static void
  setTimestamped(bool val);
  static void
  setColorized(bool val);
  static void
  setExitOnError(bool val);
  static void
  setFlushThreshold(size_t val);

  // -- Getters --

  PURE static auto
  getMaxVerbosityLevel() -> LogVerbosity;
  PURE static auto
  isBuffered() -> bool;
  PURE static auto
  isTimestamped() -> bool;
  PURE static auto
  isColorized() -> bool;
  PURE static auto
  isExitOnError() -> bool;
  PURE static auto
  getFlushThreshold() -> size_t;

  PURE static auto
  getStartTime() -> LogTimePoint;
  PURE static auto
  getNumErrors() -> size_t;
  PURE static auto
  getNumWarnings() -> size_t;

  // -- Methods --

  static void
  flush();
  static void
  error(std::string const & msg);
  static void
  warn(std::string const & msg);
  static void
  info(std::string const & msg);
  static void
  debug(std::string const & msg);
  static void
  trace(std::string const & msg);

  // -- Destructor --

  ~Log() { flush(); }

}; // class Log

#if MIN_LOG_LEVEL > 0
#  define LOG_ERROR(msg) Log::error(msg)
#else
#  define LOG_ERROR(msg)
#endif

#if MIN_LOG_LEVEL > 1
#  define LOG_WARN(msg) Log::warn(msg)
#else
#  define LOG_WARN(msg)
#endif

#if MIN_LOG_LEVEL > 2
#  define LOG_INFO(msg) Log::info(msg)
#else
#  define LOG_INFO(msg)
#endif

#if MIN_LOG_LEVEL > 3
#  define LOG_DEBUG(msg) Log::debug(msg)
#else
#  define LOG_DEBUG(msg)
#endif

#if MIN_LOG_LEVEL > 4
#  define LOG_TRACE(msg) Log::trace(msg)
#else
#  define LOG_TRACE(msg)
#endif

} // namespace um2
