#include <google/protobuf/compiler/plugin.h>

#include "lua_generator.h"

using namespace google::protobuf::compiler;
//using namespace google::protobuf::compiler::mwpb_lua;

int main(int argc, char ** argv)
{
	mwpb_lua::CLuaGenerator generator;
	return PluginMain(argc, argv, &generator);
}