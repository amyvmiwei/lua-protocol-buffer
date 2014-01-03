#ifndef __MWPB_LUA_ENUM_FIELD_HEADER__
#define __MWPB_LUA_ENUM_FIELD_HEADER__

#include <map>
#include <string>
#include "lua_field.h"
#include "lua_options.h"

namespace google {
namespace protobuf {
namespace compiler {
namespace mwpb_lua {

	class EnumFieldGenerator : public FieldGenerator {
	public:
		explicit EnumFieldGenerator(const FieldDescriptor* descriptor,
			const Options& options);
		~EnumFieldGenerator();

		// implements FieldGenerator ---------------------------------------
		void GeneratePrivateMembers(io::Printer* printer) const;
		void GenerateAccessorDeclarations(io::Printer* printer) const;
		void GenerateInlineAccessorDefinitions(io::Printer* printer) const;
		void GenerateClearingCode(io::Printer* printer) const;
		void GenerateMergingCode(io::Printer* printer) const;
		void GenerateSwappingCode(io::Printer* printer) const;
		void GenerateConstructorCode(io::Printer* printer) const;
		void GenerateMergeFromCodedStream(io::Printer* printer) const;
		void GenerateSerializeWithCachedSizes(io::Printer* printer) const;
		void GenerateSerializeWithCachedSizesToArray(io::Printer* printer) const;
		void GenerateByteSize(io::Printer* printer) const;

	private:
		const FieldDescriptor* descriptor_;
		map<string, string> variables_;

		GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(EnumFieldGenerator);
	};

	class RepeatedEnumFieldGenerator : public FieldGenerator {
	public:
		explicit RepeatedEnumFieldGenerator(const FieldDescriptor* descriptor,
			const Options& options);
		~RepeatedEnumFieldGenerator();

		// implements FieldGenerator ---------------------------------------
		void GeneratePrivateMembers(io::Printer* printer) const;
		void GenerateAccessorDeclarations(io::Printer* printer) const;
		void GenerateInlineAccessorDefinitions(io::Printer* printer) const;
		void GenerateClearingCode(io::Printer* printer) const;
		void GenerateMergingCode(io::Printer* printer) const;
		void GenerateSwappingCode(io::Printer* printer) const;
		void GenerateConstructorCode(io::Printer* printer) const;
		void GenerateMergeFromCodedStream(io::Printer* printer) const;
		void GenerateMergeFromCodedStreamWithPacking(io::Printer* printer) const;
		void GenerateSerializeWithCachedSizes(io::Printer* printer) const;
		void GenerateSerializeWithCachedSizesToArray(io::Printer* printer) const;
		void GenerateByteSize(io::Printer* printer) const;

	private:
		const FieldDescriptor* descriptor_;
		map<string, string> variables_;

		GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(RepeatedEnumFieldGenerator);
	};


}}}}


#endif