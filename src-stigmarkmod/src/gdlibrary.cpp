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
