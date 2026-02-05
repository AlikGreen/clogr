#include <cstdint>
#define CATCH_CONFIG_MAIN
#include <iostream>
#include <catch2/catch_all.hpp>

#include "clog.h"
#include "sinks/inMemorySink.h"

TEST_CASE("basic logging")
{
    auto& sink = clogr::getDefaultLogger()->addSink<clogr::InMemorySink>();
    clogr::getDefaultLogger()->setPattern("{v}");

    clogr::info("hello");

    REQUIRE(sink.getMessages().size() == 1);
    REQUIRE(sink.getMessages()[0] == "hello");
}

TEST_CASE("log levels appear in output")
{
    auto& sink = clogr::getDefaultLogger()->addSink<clogr::InMemorySink>();
    clogr::getDefaultLogger()->setPattern("[{l}] {v}");

    clogr::info("test message");

    REQUIRE(sink.getMessages().size() == 1);
    REQUIRE(sink.getMessages()[0] == "[Info] test message");
}

TEST_CASE("formatted arguments")
{
    auto& sink = clogr::getDefaultLogger()->addSink<clogr::InMemorySink>();
    clogr::getDefaultLogger()->setPattern("{v}");

    clogr::info("count: {}", 42);

    REQUIRE(sink.getMessages().size() == 1);
    REQUIRE(sink.getMessages()[0] == "count: 42");
}

TEST_CASE("log level filtering")
{
    auto logger = clogr::getDefaultLogger();
    auto& sink = logger->addSink<clogr::InMemorySink>();

    logger->setPattern("{v}");

    SECTION("messages below threshold are ignored")
    {
        logger->setMinLevel(clogr::Level::Error);

        clogr::info("this should not show up");
        clogr::warn("this neither");
        clogr::error("this should appear");

        REQUIRE(sink.getMessages().size() == 1);
        REQUIRE(sink.getMessages()[0] == "this should appear");
    }

    SECTION("trace catches everything")
    {
        logger->setMinLevel(clogr::Level::Trace);

        clogr::trace("trace msg");
        clogr::info("info msg");

        REQUIRE(sink.getMessages().size() == 2);
    }
}

TEST_CASE("multiple sinks receive the same message")
{
    const auto logger = clogr::getDefaultLogger();
    // Clear existing sinks if necessary depending on your implementation

    auto& sink1 = logger->addSink<clogr::InMemorySink>();
    auto& sink2 = logger->addSink<clogr::InMemorySink>();

    logger->setPattern("{v}");
    clogr::info("synchronize");

    REQUIRE(sink1.getMessages().size() == 1);
    REQUIRE(sink1.getMessages()[0] == "synchronize");

    REQUIRE(sink2.getMessages().size() == 1);
    REQUIRE(sink2.getMessages()[0] == "synchronize");
}

TEST_CASE("independent logger instances")
{
    // Assuming clogr allows creating named instances
    const auto loggerA = std::make_shared<clogr::Logger>("Network");
    const auto loggerB = std::make_shared<clogr::Logger>("Database");

    auto& sinkA = loggerA->addSink<clogr::InMemorySink>();
    auto& sinkB = loggerB->addSink<clogr::InMemorySink>();

    loggerA->setPattern("A: {v}");
    loggerB->setPattern("B: {v}");

    loggerA->log(clogr::Level::Info, "sending packet");

    REQUIRE(sinkA.getMessages().size() == 1);
    REQUIRE(sinkA.getMessages()[0] == "A: sending packet");

    // Ensure sinkB is still empty
    REQUIRE(sinkB.getMessages().empty());
}

TEST_CASE("formatting edge cases")
{
    auto& sink = clogr::getDefaultLogger()->addSink<clogr::InMemorySink>();
    clogr::getDefaultLogger()->setPattern("{v}");

    SECTION("empty string")
    {
        clogr::info("");
        REQUIRE(sink.getMessages()[0].empty());
    }

    SECTION("very large message")
    {
        std::string longMsg(1000, 'x');
        clogr::info(longMsg);
        REQUIRE(sink.getMessages().back() == longMsg);
    }
}