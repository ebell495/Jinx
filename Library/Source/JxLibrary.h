/*
The Jinx library is distributed under the MIT License (MIT)
https://opensource.org/licenses/MIT
See LICENSE.TXT or Jinx.h for license details.
Copyright (c) 2016 James Boer
*/

#pragma once
#ifndef JX_LIBRARY_H__
#define JX_LIBRARY_H__


namespace Jinx
{

	class Library : public ILibrary
	{
	public:
		Library(RuntimeWPtr runtime, const String & name);

		// ILibrary interface
		bool RegisterFunction(bool publicScope, bool returnValue, std::initializer_list<String> name, FunctionCallback function) override;
		bool RegisterProperty(bool readOnly, bool publicScope, const String & name, const Variant & value) override;
		Variant GetProperty(const String & name) const override;
		void SetProperty(const String & name, const Variant & value) override;

		// Internal functions
		FunctionTable & Functions() { return m_functionTable; }
		const String & GetName() const { return m_name; }
		
		bool RegisterPropertyName(const PropertyName & propertyName, bool checkForDuplicates);
		bool PropertyNameExists(const String & name) const;
		PropertyName GetPropertyName(const String & name);

	private:

		// Private internal functions
		FunctionSignature CreateFunctionSignature(bool publicScope, bool returnValue, std::initializer_list<String> name) const;

		typedef std::map <String, PropertyName, std::less<String>, Allocator<std::pair<String, PropertyName>>> PropertyNameTable;

		// Library name
		String m_name;

		// Track function definitions
		FunctionTable m_functionTable;

		// Properties
		mutable Mutex m_propertyMutex;
		PropertyNameTable m_propertyNameTable;

		// Weak ptr to runtime system
		RuntimeWPtr m_runtime;
	};

	typedef std::shared_ptr<Library> LibraryIPtr;
};

#endif // JX_LIBRARY_H__