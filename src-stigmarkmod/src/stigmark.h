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
