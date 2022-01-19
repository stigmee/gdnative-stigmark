#include "stigmark.h"

namespace stigmee
{
	// init

	void Stigmark::_init()
	{
		// godot::Godot::print(" - Stigmark::_init\n");
	}

	void Stigmark::_register_methods()
	{
		// godot::Godot::print(" - Stigmark::_register_methods\n");
		register_method("login", &Stigmark::login);
		register_method("search", &Stigmark::search);
	}

	Stigmark::Stigmark()
	{
		// godot::Godot::print(" - Stigmark::Stigmark\n");
	}
}
