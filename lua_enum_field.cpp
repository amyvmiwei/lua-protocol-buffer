#include "lua_enum_field.h"
#include "lua_helpers.h"
#include <google/protobuf/io/printer.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/stubs/strutil.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace mwpb_lua {


	namespace {

		void SetEnumVariables(const FieldDescriptor* descriptor,
			map<string, string>* variables,
			const Options & options) {
				SetCommonFieldVariables(descriptor, variables, options);
				const EnumValueDescriptor* default_value = descriptor->default_value_enum();
				(*variables)["type"] = ClassName(descriptor->enum_type(), true);
				(*variables)["default"] = SimpleItoa(default_value->number());

				(*variables)["wire_format_field_type"] =
					"" + FieldDescriptorProto_Type_Name(
					static_cast<FieldDescriptorProto_Type>(descriptor->type()));
		}

	}  // namespace

	// ===================================================================

	EnumFieldGenerator::
		EnumFieldGenerator(const FieldDescriptor* descriptor,
		const Options& options)
		: descriptor_(descriptor) {
			SetEnumVariables(descriptor, &variables_, options);
	}

	EnumFieldGenerator::~EnumFieldGenerator() {}

	void EnumFieldGenerator::
		GeneratePrivateMembers(io::Printer* printer) const {
			//printer->Print(variables_, "int $name$_;\n");
			printer->Print(variables_, "$name$_ = 0; \n");
	}

	void EnumFieldGenerator::
		GenerateAccessorDeclarations(io::Printer* printer) const {
// 			printer->Print(variables_,
// 				"inline $type$ $name$() const$deprecation$;\n"
// 				"inline void set_$name$($type$ value)$deprecation$;\n");
			printer->Print(variables_, "$classname$.get_$name$ = function()\n");
			printer->Indent();
			printer->Print(variables_, "return $classname$_Member.$name$_\n");
			printer->Outdent();
			printer->Print("end\n\n");

			printer->Print(variables_, "$classname$.set_$name$ = function(var)\n");
			printer->Indent();
			printer->Print(variables_, "$classname$_Member.$name$_ = var\n");
			printer->Print(variables_, "$classname$_Temp.$name$_ = true\n");
			printer->Outdent();
			printer->Print("end\n\n");
	}

	void EnumFieldGenerator::
		GenerateInlineAccessorDefinitions(io::Printer* printer) const {
			printer->Print(variables_,
				"inline $type$ $classname$::$name$() const {\n"
				"  return static_cast< $type$ >($name$_);\n"
				"}\n"
				"inline void $classname$::set_$name$($type$ value) {\n"
				"  assert($type$_IsValid(value));\n"
				"  set_has_$name$();\n"
				"  $name$_ = value;\n"
				"}\n");
	}

	void EnumFieldGenerator::
		GenerateClearingCode(io::Printer* printer) const {
			printer->Print(variables_, "$name$_ = $default$;\n");
	}

	void EnumFieldGenerator::
		GenerateMergingCode(io::Printer* printer) const {
			printer->Print(variables_, "set_$name$(from.$name$());\n");
	}

	void EnumFieldGenerator::
		GenerateSwappingCode(io::Printer* printer) const {
			printer->Print(variables_, "std::swap($name$_, other->$name$_);\n");
	}

	void EnumFieldGenerator::
		GenerateConstructorCode(io::Printer* printer) const {
			printer->Print(variables_, "$name$_ = $default$;\n");
	}

	void EnumFieldGenerator::
		GenerateMergeFromCodedStream(io::Printer* printer) const {
// 			printer->Print(variables_,
// 				"int value;\n"
// 				"DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<\n"
// 				"         int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(\n"
// 				"       input, &value)));\n"
// 				"if ($type$_IsValid(value)) {\n"
// 				"  set_$name$(static_cast< $type$ >(value));\n");
// 
// 
// 			if (HasUnknownFields(descriptor_->file())) {
// 				printer->Print(variables_,
// 					"} else {\n"
// 					"  mutable_unknown_fields()->AddVarint($number$, value);\n");
// 			}
// 			printer->Print(variables_,
// 				"}\n");

			printer->Print(variables_,
				"$classname$.set_$name$(pb_Read_$wire_format_field_type$());\n");
	}

	void EnumFieldGenerator::
		GenerateSerializeWithCachedSizes(io::Printer* printer) const {
// 			printer->Print(variables_,
// 				"::google::protobuf::internal::WireFormatLite::WriteEnum(\n"
// 				"  $number$, this->$name$(), output);\n");
			printer->Print(variables_,
				"pb_Write_$wire_format_field_type$($number$, $classname$.get_$name$());\n"
				);
	}

	void EnumFieldGenerator::
		GenerateSerializeWithCachedSizesToArray(io::Printer* printer) const {
			printer->Print(variables_,
				"target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(\n"
				"  $number$, this->$name$(), target);\n");
	}

	void EnumFieldGenerator::
		GenerateByteSize(io::Printer* printer) const {
			printer->Print(variables_,
				"total_size += $tag_size$ +\n"
				"  ::google::protobuf::internal::WireFormatLite::EnumSize(this->$name$());\n");
	}

	// ===================================================================

	RepeatedEnumFieldGenerator::
		RepeatedEnumFieldGenerator(const FieldDescriptor* descriptor,
		const Options& options)
		: descriptor_(descriptor) {
			SetEnumVariables(descriptor, &variables_, options);
	}

	RepeatedEnumFieldGenerator::~RepeatedEnumFieldGenerator() {}

	void RepeatedEnumFieldGenerator::
		GeneratePrivateMembers(io::Printer* printer) const {
// 			printer->Print(variables_,
// 				"::google::protobuf::RepeatedField<int> $name$_;\n");
// 			if (descriptor_->options().packed() && HasGeneratedMethods(descriptor_->file())) {
// 				printer->Print(variables_,
// 					"mutable int _$name$_cached_byte_size_;\n");
// 			}
			printer->Print(variables_,"$name$_ = {}\n");
	}

	void RepeatedEnumFieldGenerator::
		GenerateAccessorDeclarations(io::Printer* printer) const {
			//printer->Print(variables_,
			//	"inline $type$ $name$(int index) const$deprecation$;\n"
			//	"inline void set_$name$(int index, $type$ value)$deprecation$;\n"
			//	"inline void add_$name$($type$ value)$deprecation$;\n");
			//printer->Print(variables_,
			//	"inline const ::google::protobuf::RepeatedField<int>& $name$() const$deprecation$;\n"
			//	"inline ::google::protobuf::RepeatedField<int>* mutable_$name$()$deprecation$;\n");
			printer->Print(variables_, "$classname$.add_$name$ = function(var)\n");
			printer->Indent();
			printer->Print(variables_, "table.insert($classname$_Member.$name$_, var)\n");
			printer->Print(variables_, "$classname$_Temp.$name$_ = true\n");
			printer->Outdent();
			printer->Print("end\n\n");

			printer->Print(variables_, "$classname$.get_$name$ = function(index)\n");
			printer->Indent();
			printer->Print(variables_, "return $classname$_Member.$name$_[index]\n");
			printer->Outdent();
			printer->Print("end\n\n");
	}

	void RepeatedEnumFieldGenerator::
		GenerateInlineAccessorDefinitions(io::Printer* printer) const {
			printer->Print(variables_,
				"inline $type$ $classname$::$name$(int index) const {\n"
				"  return static_cast< $type$ >($name$_.Get(index));\n"
				"}\n"
				"inline void $classname$::set_$name$(int index, $type$ value) {\n"
				"  assert($type$_IsValid(value));\n"
				"  $name$_.Set(index, value);\n"
				"}\n"
				"inline void $classname$::add_$name$($type$ value) {\n"
				"  assert($type$_IsValid(value));\n"
				"  $name$_.Add(value);\n"
				"}\n");
			printer->Print(variables_,
				"inline const ::google::protobuf::RepeatedField<int>&\n"
				"$classname$::$name$() const {\n"
				"  return $name$_;\n"
				"}\n"
				"inline ::google::protobuf::RepeatedField<int>*\n"
				"$classname$::mutable_$name$() {\n"
				"  return &$name$_;\n"
				"}\n");
	}

	void RepeatedEnumFieldGenerator::
		GenerateClearingCode(io::Printer* printer) const {
			printer->Print(variables_, "$name$_.Clear();\n");
	}

	void RepeatedEnumFieldGenerator::
		GenerateMergingCode(io::Printer* printer) const {
			printer->Print(variables_, "$name$_.MergeFrom(from.$name$_);\n");
	}

	void RepeatedEnumFieldGenerator::
		GenerateSwappingCode(io::Printer* printer) const {
			printer->Print(variables_, "$name$_.Swap(&other->$name$_);\n");
	}

	void RepeatedEnumFieldGenerator::
		GenerateConstructorCode(io::Printer* printer) const {
			// Not needed for repeated fields.
	}

	void RepeatedEnumFieldGenerator::
		GenerateMergeFromCodedStream(io::Printer* printer) const {
			//// Don't use ReadRepeatedPrimitive here so that the enum can be validated.
			//printer->Print(variables_,
			//	"int value;\n"
			//	"DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<\n"
			//	"         int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(\n"
			//	"       input, &value)));\n"
			//	"if ($type$_IsValid(value)) {\n"
			//	"  add_$name$(static_cast< $type$ >(value));\n");
			//if (HasUnknownFields(descriptor_->file())) {
			//	printer->Print(variables_,
			//		"} else {\n"
			//		"  mutable_unknown_fields()->AddVarint($number$, value);\n");
			//}
			//printer->Print("}\n");

			printer->Print(variables_,
				"$classname$.set_$name$(pb_Read_$wire_format_field_type$());\n");
	}

	void RepeatedEnumFieldGenerator::
		GenerateMergeFromCodedStreamWithPacking(io::Printer* printer) const {
// 			if (!descriptor_->options().packed()) {
// 				// We use a non-inlined implementation in this case, since this path will
// 				// rarely be executed.
// 				printer->Print(variables_,
// 					"DO_((::google::protobuf::internal::WireFormatLite::ReadPackedEnumNoInline(\n"
// 					"       input,\n"
// 					"       &$type$_IsValid,\n"
// 					"       this->mutable_$name$())));\n");
// 			} else {
// 				printer->Print(variables_,
// 					"::google::protobuf::uint32 length;\n"
// 					"DO_(input->ReadVarint32(&length));\n"
// 					"::google::protobuf::io::CodedInputStream::Limit limit = "
// 					"input->PushLimit(length);\n"
// 					"while (input->BytesUntilLimit() > 0) {\n"
// 					"  int value;\n"
// 					"  DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<\n"
// 					"         int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(\n"
// 					"       input, &value)));\n"
// 					"  if ($type$_IsValid(value)) {\n"
// 					"    add_$name$(static_cast< $type$ >(value));\n"
// 					"  }\n"
// 					"}\n"
// 					"input->PopLimit(limit);\n");
// 			}

			printer->Print(variables_,
				"$classname$.set_$name$(pb_Read_$wire_format_field_type$());\n");
	}

	void RepeatedEnumFieldGenerator::
		GenerateSerializeWithCachedSizes(io::Printer* printer) const {
// 			if (descriptor_->options().packed()) {
// 				// Write the tag and the size.
// 				printer->Print(variables_,
// 					"if (this->$name$_size() > 0) {\n"
// 					"  ::google::protobuf::internal::WireFormatLite::WriteTag(\n"
// 					"    $number$,\n"
// 					"    ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED,\n"
// 					"    output);\n"
// 					"  output->WriteVarint32(_$name$_cached_byte_size_);\n"
// 					"}\n");
// 			}
// 			printer->Print(variables_,
// 				"for (int i = 0; i < this->$name$_size(); i++) {\n");
// 			if (descriptor_->options().packed()) {
// 				printer->Print(variables_,
// 					"  ::google::protobuf::internal::WireFormatLite::WriteEnumNoTag(\n"
// 					"    this->$name$(i), output);\n");
// 			} else {
// 				printer->Print(variables_,
// 					"  ::google::protobuf::internal::WireFormatLite::WriteEnum(\n"
// 					"    $number$, this->$name$(i), output);\n");
// 			}
// 			printer->Print("}\n");

// 			printer->Print(variables_,
// 				"$classname$_Member.set_$name$(pb_Read_$wire_format_field_type$());\n");

			printer->Print(variables_,
				"for i=1, $classname$.$name$_size() do\n"
				"  pb_Write_$wire_format_field_type$($number$, $classname$.get_$name$());\n"
				"end\n"
				);
	}

	void RepeatedEnumFieldGenerator::
		GenerateSerializeWithCachedSizesToArray(io::Printer* printer) const {
			if (descriptor_->options().packed()) {
				// Write the tag and the size.
				printer->Print(variables_,
					"if (this->$name$_size() > 0) {\n"
					"  target = ::google::protobuf::internal::WireFormatLite::WriteTagToArray(\n"
					"    $number$,\n"
					"    ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED,\n"
					"    target);\n"
					"  target = ::google::protobuf::io::CodedOutputStream::WriteVarint32ToArray("
					"    _$name$_cached_byte_size_, target);\n"
					"}\n");
			}
			printer->Print(variables_,
				"for (int i = 0; i < this->$name$_size(); i++) {\n");
			if (descriptor_->options().packed()) {
				printer->Print(variables_,
					"  target = ::google::protobuf::internal::WireFormatLite::WriteEnumNoTagToArray(\n"
					"    this->$name$(i), target);\n");
			} else {
				printer->Print(variables_,
					"  target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(\n"
					"    $number$, this->$name$(i), target);\n");
			}
			printer->Print("}\n");
	}

	void RepeatedEnumFieldGenerator::
		GenerateByteSize(io::Printer* printer) const {
			printer->Print(variables_,
				"{\n"
				"  int data_size = 0;\n");
			printer->Indent();
			printer->Print(variables_,
				"for (int i = 0; i < this->$name$_size(); i++) {\n"
				"  data_size += ::google::protobuf::internal::WireFormatLite::EnumSize(\n"
				"    this->$name$(i));\n"
				"}\n");

			if (descriptor_->options().packed()) {
				printer->Print(variables_,
					"if (data_size > 0) {\n"
					"  total_size += $tag_size$ +\n"
					"    ::google::protobuf::internal::WireFormatLite::Int32Size(data_size);\n"
					"}\n"
					"GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();\n"
					"_$name$_cached_byte_size_ = data_size;\n"
					"GOOGLE_SAFE_CONCURRENT_WRITES_END();\n"
					"total_size += data_size;\n");
			} else {
				printer->Print(variables_,
					"total_size += $tag_size$ * this->$name$_size() + data_size;\n");
			}
			printer->Outdent();
			printer->Print("}\n");
	}




}}}}