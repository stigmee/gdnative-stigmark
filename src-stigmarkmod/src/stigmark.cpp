#include "stigmark.h"

using namespace godot;

// stigmark methods

String Stigmark::login(String p_mail, String p_pass)
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
