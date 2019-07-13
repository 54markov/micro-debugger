#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Calculator

#include <boost/test/unit_test.hpp>

#include <exception/exception.hpp>
#include <debugger/i-debugger.hpp>
#include <debugger/debugger-factory.hpp>

BOOST_AUTO_TEST_SUITE(DebuggerInstructionTests)

BOOST_AUTO_TEST_CASE(NullPtr)
{
	auto debugger = DebuggerFactory::createDebugger(DebuggerType::instructions);
    BOOST_CHECK_THROW(debugger->run(nullptr), Exception);
}

BOOST_AUTO_TEST_SUITE_END()
