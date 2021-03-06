#include <string>
#include <tuple>
#include <boost/test/unit_test.hpp>
#include <test/libsolidity/SolidityExecutionFramework.h>

using namespace std;
using namespace dev::test;

namespace dev
{
namespace solidity
{
namespace test
{

static char const* contractCode = R"DELIMITER(
pragma solidity ^0.4.0;

contract Smoke {
	uint256[] arr;
	function numberOfElements() public view returns (uint256) {
		return arr.length;
	}
}

)DELIMITER";

static unique_ptr<bytes> s_compiledContract;

class SmokeTestFramework: public SolidityExecutionFramework
{
protected:
	void deployContract(u256 const& _value = 0)
	{
		if (!s_compiledContract)
			s_compiledContract.reset(new bytes(compileContract(contractCode, "Smoke")));

		bytes args = encodeArgs();
		sendMessage(*s_compiledContract + args, true, _value);
		BOOST_REQUIRE(!m_output.empty());
	}
};

/// This is a test suite that tests optimised code!
BOOST_FIXTURE_TEST_SUITE(LitySmoke, SmokeTestFramework)

BOOST_AUTO_TEST_CASE(creation)
{
	deployContract();
	BOOST_REQUIRE(callContractFunction("numberOfElements()") == encodeArgs(u256(0)));
}

BOOST_AUTO_TEST_SUITE_END()

}
}
} // end namespaces
