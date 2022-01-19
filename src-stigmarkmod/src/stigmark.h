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

#ifndef __GODOT_MODULE_STIGMARK_H
#define __GODOT_MODULE_STIGMARK_H

#include <Godot.hpp>
#include <Node.hpp>

namespace stigmee
{
    class Stigmark : public godot::Reference
    {
        GODOT_CLASS(Stigmark, Reference);

    public:
        static void _register_methods();
        static void _init();

    public:
        godot::String login(godot::String p_mail, godot::String p_pass);
        godot::Array Stigmark::search(godot::String p_keyword);

        Stigmark();
    };
}

#endif // __GODOT_MODULE_STIGMARK_H
