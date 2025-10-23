#pragma once

#include "common.h"

#include <spdlog/spdlog.h>

#include <filesystem>



namespace ark
{
	// ///////////////////////////////////////////////////////////////////////////////////////
	// log formatting helpers (not sure, whether this should go to Helper.h or stay here)

	template <typename T>
	struct ArrayFmt
	{
		const size_t n = 0;
		T const* const a = nullptr;
		ArrayFmt(const size_t n, T const* const a) noexcept : n(n), a(a) {}
		ArrayFmt(const std::vector<T>& v) noexcept : n(v.size()), a(v.data()) {}

		friend std::ostream& operator<<(std::ostream& os, const ArrayFmt& afmt)
		{
			os << "[ ";

			if (afmt.n > 0)
			{
				os << afmt.a[0];
			}

			for (size_t i = 1; i < afmt.n; ++i)
			{
				os << ", " << afmt.a[i];
			}

			return os << " ]";
		}
	};

	template <typename T>
	struct PointerFmt
	{
		T const* const p = nullptr;
		explicit PointerFmt(T const* const p) noexcept : p(p) {}

		friend std::ostream& operator<<(std::ostream& os, const PointerFmt& pfmt)
		{
			return os << *pfmt.p;
		}
	};

	template <typename T>
	struct HandleFmt
	{
		T h = nullptr;
		explicit HandleFmt(const T h) noexcept : h(h) {}

		friend std::ostream& operator<<(std::ostream& os, const HandleFmt& hfmt)
		{
			return os << ((void*)(uintptr_t)(hfmt.h));
		}
	};

	// ///////////////////////////////////////////////////////////////////////////////////////
	// regular logger

	//! Initializes logger
	extern bool InitLogger(
		const std::filesystem::path& logFilePath,
		const std::string& name = "main",
		const spdlog::level::level_enum level = spdlog::level::debug,
		const spdlog::level::level_enum flush_on = spdlog::level::err);

	//! Closes the logger
	extern void CloseLogger();

	//! sets a different logging level than specified during initialization
	extern void SetLogLevel(const spdlog::level::level_enum level);

	//! Returns the currently used logger
	extern spdlog::logger* GetLogger();

	//! flushes the logger
	extern void FlushLogger();

	template <typename T>
	inline void LogAlways(const spdlog::source_loc loc, const spdlog::level::level_enum lvl, const T& msg)
	{
		if (GetLogger())
			GetLogger()->log(loc, lvl, msg);
	}

	template <typename... Args>
	inline void LogAlways(const spdlog::source_loc loc, const spdlog::level::level_enum lvl, fmt::format_string<Args...> fmt, Args&&... args)
	{
		if (GetLogger())
			GetLogger()->log(loc, lvl, fmt, std::forward<Args>(args)...);
	}
}

#define ARK_CRITICAL(...) ark::LogAlways(spdlog::source_loc{ __FILE__, __LINE__, SPDLOG_FUNCTION }, spdlog::level::critical, __VA_ARGS__)
#define ARK_ERROR(...)    ark::LogAlways(spdlog::source_loc{ __FILE__, __LINE__, SPDLOG_FUNCTION }, spdlog::level::err,      __VA_ARGS__)
#define ARK_WARN(...)     ark::LogAlways(spdlog::source_loc{ __FILE__, __LINE__, SPDLOG_FUNCTION }, spdlog::level::warn,     __VA_ARGS__)
#define ARK_INFO(...)     ark::LogAlways(spdlog::source_loc{ __FILE__, __LINE__, SPDLOG_FUNCTION }, spdlog::level::info,     __VA_ARGS__)
#define ARK_DEBUG(...)    ark::LogAlways(spdlog::source_loc{ __FILE__, __LINE__, SPDLOG_FUNCTION }, spdlog::level::debug,    __VA_ARGS__)
#define ARK_TRACE(...)    ark::LogAlways(spdlog::source_loc{ __FILE__, __LINE__, SPDLOG_FUNCTION }, spdlog::level::trace,    __VA_ARGS__)
