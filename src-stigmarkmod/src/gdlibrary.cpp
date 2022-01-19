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

#include "stigmark.h"

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o)
{
    // godot::Godot::print("godot_gdnative_init v3\n");
    godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o)
{
    // godot::Godot::print("godot_gdnative_terminate\n");
    godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle)
{
    // godot::Godot::print("godot_nativescript_init\n");
    godot::Godot::nativescript_init(handle);
    // godot::Godot::print("registering class\n");
    godot::register_class<stigmee::Stigmark>();
    // godot::Godot::print("Stigmark classname='%s'\n", godot::Stigmark::___get_class_name());
}
