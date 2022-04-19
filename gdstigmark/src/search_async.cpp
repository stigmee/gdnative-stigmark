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
	static void search_async_callback(void *data, int status, stigmark_client_search_response_t *res_ptr, int res_count)
	{
		Stigmark *stigmark = static_cast<Stigmark*>(data);
		godot::Array collections;

		for (int r = 0; r < res_count; r++)
		{
			godot::Dictionary gdcol;

			gdcol["collection_id"] = res_ptr[r].collection_id;
			gdcol["keyword_id"] = res_ptr[r].keyword_id;

			godot::Array gdurls;
			for (int u = 0; u < res_ptr[r].urls_count; u++)
			{
				godot::Dictionary gdurl;

				gdurl["id"] = res_ptr[r].urls_ptr[u].id;
				gdurl["uri"] = res_ptr[r].urls_ptr[u].url;
				gdurls.push_back(gdurl);
			}

			gdcol["urls"] = gdurls;
			collections.push_back(gdcol);
		}

		stigmark->emit_search_collection(collections);
	}

	void Stigmark::emit_search_collection(godot::Array collections)
	{
		godot::Array args;
		args.push_back(collections);
		emit_signal("on_search", args);
	}

	void Stigmark::search_async(godot::String p_keyword)
	{
		godot::Godot::print("Stigmark::search_async");

		const char *keyword = p_keyword.alloc_c_string();

		int err = stigmark_client_search_by_keyword(
			keyword, search_async_callback, this, 0);

		godot::api->godot_free((void*) keyword);

		if (err < 0)
			godot::Godot::print("Stigmark::search_async failed with error");
	}
}
