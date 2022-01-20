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
#include "../../include/stigmark.h"

namespace stigmee
{
	struct search_private_data
	{
		int status = -1;
		godot::Array collections;
	};

	static void search_callback(void *data, int status, stigmark_client_search_response_t *res_ptr, int res_count)
	{
		struct search_private_data *private_data = static_cast<struct search_private_data *>(data);
		private_data->status = status;

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
			private_data->collections.push_back(gdcol);
		}
	}

	godot::Array Stigmark::search(godot::String p_keyword)
	{
		struct search_private_data private_data;

		godot::Godot::print(" - Stigmark::search\n");

		const char *keyword = p_keyword.alloc_c_string();

		int err = stigmark_client_search_by_keyword(
			keyword, search_callback, &private_data, 1);
		godot::api->godot_free((void*) keyword);

		if (err < 0)
		{
			godot::Godot::print("Stigmark::search failed with error");
			return private_data.collections;
		}

		if (private_data.status >= 200 && private_data.status < 300)
		{
			return private_data.collections;
		}

		godot::Godot::print("Stigmark::search failed with status");
		return private_data.collections;
	}
}
