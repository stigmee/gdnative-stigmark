#include "stigmark.h"

namespace stigmee
{

	// stigmark methods

	godot::String Stigmark::login(godot::String p_mail, godot::String p_pass)
	{
		// godot::Godot::print(" - Stigmark::login\n");
		return "Hello Stigmers";
	}

	godot::String Stigmark::search(godot::String p_keyword)
	{
		// godot::Godot::print(" - Stigmark::login\n");
		return "Hello Stigmers";
	}

	// init

	void Stigmark::_init()
	{
		// godot::Godot::print(" - Stigmark::_init\n");
	}

	void Stigmark::_register_methods()
	{
		// godot::Godot::print(" - Stigmark::_register_methods\n");
		register_method("login", &Stigmark::login);
	}

	Stigmark::Stigmark()
	{
		// godot::Godot::print(" - Stigmark::Stigmark\n");
	}
}
