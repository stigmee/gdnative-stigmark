//
//  Stigmee: A 3D browser and decentralized social network.
//  Copyright 2021-2022 Philippe Anel <zexigh@gmail.com>
//
//  This file is part of Stigmee.
//
//  Project : Stigmark
//  Version : 0.0-1
//
//  Stigmee is free software: you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful, but
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "gdstigmark.hpp"

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

		godot::register_method("login", &Stigmark::login);
		godot::register_method("search", &Stigmark::search);
		godot::register_method("search_async", &Stigmark::search_async);

		godot::Dictionary signal_args;
		signal_args["collections"] = GODOT_VARIANT_TYPE_ARRAY;
		godot::String signal_name = "on_search";
		godot::register_signal<Stigmark>(signal_name, signal_args);
	}

	Stigmark::Stigmark()
	{
		// godot::Godot::print(" - Stigmark::Stigmark\n");
	}
}
