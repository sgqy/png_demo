#pragma once

#if 1
#define LOG_VERB SPDLOG_DEBUG
#define LOG_INFO SPDLOG_INFO
#define LOG_WARN SPDLOG_WARN
#define LOG_ERRS SPDLOG_ERROR
// #define LOG_FORMAT "[%T.%F][%^%l%$][%t]%@<%!>: %v"
#define LOG_FORMAT "[%t][%^%l%$]%@<%!>: %v"
#else
#define LOG_VERB spdlog::debug
#define LOG_INFO spdlog::info
#define LOG_WARN spdlog::warn
#define LOG_ERRS spdlog::error
#define LOG_FORMAT "[%t][%^%l%$]: %v"
#endif

#define INIT_LOG() (void)(spdlog::set_level(spdlog::level::trace), spdlog::set_pattern(LOG_FORMAT), 0)