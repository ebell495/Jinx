/*
The Jinx library is distributed under the MIT License (MIT)
https://opensource.org/licenses/MIT
See LICENSE.TXT or Jinx.h for license details.
Copyright (c) 2016 James Boer
*/

#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdexcept>

#include "../../Source/Jinx.h"

using namespace Jinx;

#ifndef NDEBUG
#define REQUIRE assert
#else
#define REQUIRE(x) { if (!(x)) throw new std::runtime_error("Failure for condition: " #x); }
#endif

Jinx::RuntimePtr TestCreateRuntime()
{
	return Jinx::CreateRuntime();
}

Jinx::ScriptPtr TestCreateScript(const char * scriptText, Jinx::RuntimePtr runtime = nullptr)
{
	if (!runtime)
		runtime = CreateRuntime();

	// Compile the text to bytecode
	auto bytecode = runtime->Compile(scriptText, "Test Script", { "core" });
	if (!bytecode)
		return nullptr;

	// Create a runtime script with the given bytecode
	return runtime->CreateScript(bytecode);
}

Jinx::ScriptPtr TestExecuteScript(const char * scriptText, Jinx::RuntimePtr runtime = nullptr)
{
	// Create a runtime script
	auto script = TestCreateScript(scriptText, runtime);
	if (!script)
		return nullptr;

	// Execute script until finished
	do
	{
		if (!script->Execute())
			return nullptr;
	}
	while (!script->IsFinished());

	return script;
}

int main(int argc, char ** argv)
{
	printf("Jinx version: %s\n", Jinx::GetVersionString().c_str());

	GlobalParams params;
	params.logBytecode = true;
	params.logSymbols = true;
	params.errorOnMaxInstrunctions = false;
	params.maxInstructions = std::numeric_limits<uint32_t>::max();
	Initialize(params);

	const char * scriptText =
		u8R"(

			-- Function declaration
			function multiply {x} by {y}
				return x * y
			end

			-- Assign a specific function to a variable 'f'
			set f to function multiply {x} by {y}

			-- Execute specified function by variable
			set x to call f with 3, 4

		)";

	auto script = TestExecuteScript(scriptText);
	REQUIRE(script);
	REQUIRE(script->GetVariable("f").IsFunction());
	REQUIRE(script->GetVariable("x") == 12);

	return 0;
}
