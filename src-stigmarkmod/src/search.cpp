#include "stigmark.h"

namespace stigmee
{
	godot::Array Stigmark::search(godot::String p_keyword)
	{
		godot::Array array;

		godot::Godot::print(" - Stigmark::search\n");
		
		array.push_back(godot::String("foo"));
		array.push_back(godot::String("bar"));
		return array;
	}
}
