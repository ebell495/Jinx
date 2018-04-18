﻿/*
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
#ifdef _WINDOWS
#include <conio.h>
#endif

#include "../../../Source/Jinx.h"

using namespace Jinx;

#define REQUIRE assert

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
	printf("Jinx version: %s\n", Jinx::VersionString);

	// Add scope block to ensure all objects are destroyed for shutdown test
	{
		GlobalParams globalParams;
		globalParams.logSymbols = true;
		globalParams.logBytecode = true;
        globalParams.maxInstructions = 5000;
		globalParams.allocBlockSize = 1024 * 256;
		globalParams.allocFn = [](size_t size) { return malloc(size); };
		globalParams.reallocFn = [](void * p, size_t size) { return realloc(p, size); };
		globalParams.freeFn = [](void * p) { free(p); };
		Jinx::Initialize(globalParams);

		const char * scriptText =
			u8R"(
			
			import core

			function func4 {integer a}
				return call stack
			end

			function func3 {a}
				return func4 a
			end

			function func2/func22 (opt1/opt2)
				return func3 123
			end

			function func1/func11 (optional)
				return func2
			end

			set a to func1

			write line a
			
			)";

		auto script = TestExecuteScript(scriptText);
		REQUIRE(script);
		REQUIRE(script->GetVariable("a").IsCollection());
		REQUIRE(script->GetVariable("a").GetCollection()->at(1) == "root");
		REQUIRE(script->GetVariable("a").GetCollection()->at(2) == "func1/func11 (optional)");
		REQUIRE(script->GetVariable("a").GetCollection()->at(3) == "func2/func22 (opt1/opt2)");
		REQUIRE(script->GetVariable("a").GetCollection()->at(4) == "func3 {}");
		REQUIRE(script->GetVariable("a").GetCollection()->at(5) == "func4 {integer}");
	}

	Jinx::ShutDown();

	auto stats = GetMemoryStats();
	REQUIRE(stats.currentAllocatedMemory == 0);
	REQUIRE(stats.currentUsedMemory == 0);

#ifdef _WINDOWS
	printf("Press any key to continue...");
	_getch();
#endif
    
    return 0;
}