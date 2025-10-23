#include "logger.h"

#if defined(ARK_OS_ANDROID)
#include <spdlog/sinks/android_sink.h>
#elif defined(ARK_OS_WIN)
#include <spdlog/sinks/msvc_sink.h>
//#elif defined(ARK_OS_MACOS) || defined(ARK_OS_IOS) || defined(ARK_OS_TVOS)
#endif
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/dup_filter_sink.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <filesystem>


// regular logger
static std::string gLoggerName;
static std::shared_ptr<spdlog::logger> gLogger;
static std::shared_ptr<spdlog::sinks::dist_sink_mt> gDistributionSink;


namespace ark
{
	bool InitLogger(
		const std::filesystem::path& logFilePath,
		const std::string& name,
		const spdlog::level::level_enum level,
		const spdlog::level::level_enum flush_on)
	{
		ARK_ASSERT(!gLogger);

		gLoggerName = name;

		std::vector<spdlog::sink_ptr> sinks;
#if defined(ARK_OS_ANDROID)
		sinks.emplace_back(std::make_shared<spdlog::sinks::android_sink_mt>());
#elif defined(ARK_OS_WIN)
		sinks.emplace_back(std::make_shared<spdlog::sinks::msvc_sink_mt>());
#else
		sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
#endif

		sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true));

		// create a filter sink first, it's a distribution sink - stores a vector of sinks which get called when log is called
		gDistributionSink = std::make_shared<spdlog::sinks::dup_filter_sink_mt>(std::chrono::seconds(5));
		gDistributionSink->set_sinks(sinks);

		gLogger = std::make_shared<spdlog::logger>(gLoggerName, gDistributionSink);
		gLogger->set_level(level);
		// make sure errors are always logged (even in face of crashes)
		// this calls flush on every single logger sink
		gLogger->flush_on(flush_on);

		return true;
	}

	void CloseLogger()
	{
		gLogger.reset();
		gDistributionSink.reset();
	}

	void SetLogLevel(const spdlog::level::level_enum level)
	{
		if (gLogger)
			gLogger->set_level(level);
	}

	spdlog::logger* GetLogger()
	{
		return gLogger.get();
	}

	void FlushLogger()
	{
		if (gLogger)
			gLogger->flush();
	}
}
