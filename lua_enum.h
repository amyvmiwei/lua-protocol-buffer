#ifndef __MWPB_LUA_ENUM_HEADER__
#define __MWPB_LUA_ENUM_HEADER__

#include <string>
#include <google/protobuf/descriptor.h>
#include "lua_options.h"
namespace google {
	namespace protobuf {
		namespace io {
			class Printer;             // printer.h
		}
	}

namespace protobuf {
namespace compiler {
namespace mwpb_lua {

	class EnumGenerator {
	public:
		// See generator.cc for the meaning of dllexport_decl.
		explicit EnumGenerator(const EnumDescriptor* descriptor,
			const Options& options);
		~EnumGenerator();

		// Header stuff.

		// Generate header code defining the enum.  This code should be placed
		// within the enum's package namespace, but NOT within any class, even for
		// nested enums.
		// 产生头文件，定义enum，必须与enum的的包在同一个定义空间。
		void GenerateDefinition(io::Printer* printer);

		// Generate specialization of GetEnumDescriptor<MyEnum>().
		// Precondition: in ::google::protobuf namespace.
		// 生成GetEnumDescriptor专门的函数.放在::google::protobuf 命名空间中.
		void GenerateGetEnumDescriptorSpecializations(io::Printer* printer);

		// For enums nested within a message, generate code to import all the enum's
		// symbols (e.g. the enum type name, all its values, etc.) into the class's
		// namespace.  This should be placed inside the class definition in the
		// header.
		void GenerateSymbolImports(io::Printer* printer);

		// Source file stuff.

		// Generate code that initializes the global variable storing the enum's
		// descriptor.
		void GenerateDescriptorInitializer(io::Printer* printer, int index);

		// Generate non-inline methods related to the enum, such as IsValidValue().
		// Goes in the .cc file.
		// 生成函数.
		void GenerateMethods(io::Printer* printer);

	private:
		const EnumDescriptor* descriptor_;
		string classname_;
		Options options_;

		GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(EnumGenerator);
	};


}}}}



#endif