/*
The Jinx library is distributed under the MIT License (MIT)
https://opensource.org/licenses/MIT
See LICENSE.TXT or Jinx.h for license details.
Copyright (c) 2016 James Boer
*/

#include "UnitTest.h"

using namespace Jinx;


TEST_CASE("Test Casts", "[Casts]")
{
	SECTION("Test variable casting")
	{
		static const char * scriptText =
			u8R"(
    
			set a to 123.456 as integer
			set b to true as string
			set c to "false" as boolean
			set d to "456" as integer
			set e to "-123.456" as number
			set f to 4 + 5 + 6 as string
			)";

		auto script = TestExecuteScript(scriptText);
		REQUIRE(script);
		REQUIRE(script->GetVariable("a").IsInteger());
		REQUIRE(script->GetVariable("a").GetInteger() == 123);
		REQUIRE(script->GetVariable("b").IsString());
		REQUIRE(script->GetVariable("b").GetString() == "true");
		REQUIRE(script->GetVariable("c").IsBoolean());
		REQUIRE(script->GetVariable("c").GetBoolean() == false);
		REQUIRE(script->GetVariable("d").IsInteger());
		REQUIRE(script->GetVariable("d").GetInteger() == 456);
		REQUIRE(script->GetVariable("e").IsNumber());
		REQUIRE(script->GetVariable("e").GetNumber() == Approx(-123.456));
		REQUIRE(script->GetVariable("f").IsString());
		REQUIRE(script->GetVariable("f").GetString() == "15");
	}
    
    SECTION("Test additional casts")
    {
        const char * scriptText =
			u8R"(
        
			set a to 12345
			set b to a as string
			set c to b as integer
			set d to true
			set e to d as string
			set f to e as boolean
			set g to false
			if g type = boolean and g type = d type
				set g to true
            end
            
            )";
            
            auto script = TestExecuteScript(scriptText);
        REQUIRE(script);
        REQUIRE(script->GetVariable("a").GetNumber() == 12345);
        REQUIRE(script->GetVariable("b").GetString() == "12345");
        REQUIRE(script->GetVariable("c").GetInteger() == 12345);
        REQUIRE(script->GetVariable("d").GetBoolean() == true);
        REQUIRE(script->GetVariable("e").GetString() == "true");
        REQUIRE(script->GetVariable("f").GetBoolean() == true);
        REQUIRE(script->GetVariable("g").GetBoolean() == true);
    }
    
}