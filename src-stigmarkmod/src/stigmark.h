#ifndef __GODOT_MODULE_STIGMARK_H
#define __GODOT_MODULE_STIGMARK_H

#include <Godot.hpp>
#include <Node.hpp>

namespace godot
{
    class Stigmark : public Reference
    {
        GODOT_CLASS(Stigmark, Reference);

    public:
        static void _register_methods();
        static void _init();

    public:
        String login(String p_mail, String p_pass);

        Stigmark();
    };
}

#endif // __GODOT_MODULE_STIGMARK_H
