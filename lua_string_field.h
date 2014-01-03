#ifndef __MWPB_LUA_STRING_FIELD_HEADER__
#define __MWPB_LUA_STRING_FIELD_HEADER__

#include <map>
#include <string>
#include "lua_field.h"

namespace google {
namespace protobuf {
namespace compiler {
namespace mwpb_lua {

	class StringFieldGenerator : public FieldGenerator
	{
	public:
		explicit StringFieldGenerator(const FieldDescriptor* descriptor, const Options& options);
		~StringFieldGenerator();

		// implements FieldGenerator ---------------------------------------
		void GeneratePrivateMembers(io::Printer* printer) const;
		void GenerateAccessorDeclarations(io::Printer* printer) const;
		void GenerateInlineAccessorDefinitions(io::Printer* printer) const;
		void GenerateNonInlineAccessorDefinitions(io::Printer* printer) const;
		void GenerateClearingCode(io::Printer* printer) const;
		void GenerateMergingCode(io::Printer* printer) const;
		void GenerateSwappingCode(io::Printer* printer) const;
		void GenerateConstructorCode(io::Printer* printer) const;
		void GenerateDestructorCode(io::Printer* printer) const;
		void GenerateDefaultInstanceAllocator(io::Printer* printer) const;
		void GenerateShutdownCode(io::Printer* printer) const;
		void GenerateMergeFromCodedStream(io::Printer* printer) const;
		void GenerateSerializeWithCachedSizes(io::Printer* printer) const;
		void GenerateSerializeWithCachedSizesToArray(io::Printer* printer) const;
		void GenerateByteSize(io::Printer* printer) const;

	private:
		const FieldDescriptor* descriptor_;
		map<string, string> variables_;

		GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(StringFieldGenerator);
	};


	class RepeatedStringFieldGenerator : public FieldGenerator {
	public:
		explicit RepeatedStringFieldGenerator(const FieldDescriptor* descriptor,
			const Options& options);
		~RepeatedStringFieldGenerator();

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

		GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(RepeatedStringFieldGenerator);
	};


}}}}
#endif