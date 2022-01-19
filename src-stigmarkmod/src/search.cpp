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
		std::vector<stigmark_search_response> collections;
	};

	static void search_callback(void *data, int status, stigmark_client_search_response_t *res_ptr, int res_count)
	{
		struct search_private_data *private_data = static_cast<struct search_private_data *>(data);
		private_data->status = status;

		std::vector<stigmark_search_response> collections;

		for (int r = 0; r < res_count; r++)
		{
			stigmark_search_response res;

			res.collection_id = res_ptr[r].collection_id;
			res.keyword_id = res_ptr[r].keyword_id;

			for (int u = 0; u < res_ptr[r].urls_count; u++)
			{
				stigmark_search_response_url_entry url_entry;

				url_entry.id = res_ptr[r].urls_ptr[u].id;
				url_entry.url = res_ptr[r].urls_ptr[u].url;

				res.urls.push_back(url_entry);
			}
			collections.push_back(res);
		}

		// copy
		private_data->collections = collections;
	}

	godot::Array Stigmark::search(godot::String p_keyword)
	{
		struct search_private_data private_data;
		godot::Array array;

		godot::Godot::print(" - Stigmark::search\n");

		const char *keyword = p_keyword.alloc_c_string();

		int err = stigmark_client_search_by_keyword(
			keyword, search_callback, &private_data, 1);
		godot::api->godot_free((void*) keyword);

		if (err < 0)
		{
			array.push_back("Stigmark::search failed with error");
			array.push_back(err);
			return array;
		}

		if (private_data.status >= 200 && private_data.status < 300)
		{
			for (auto& collection: private_data.collections) {
				godot::Dictionary gdcol;

				gdcol["collection_id"] = collection.collection_id;
				gdcol["keyword_id"] = collection.keyword_id;

				godot::Array gdurls;
				for (auto& url: collection.urls) {
					godot::Dictionary gdurl;

					gdurl["id"] = url.id;
					gdurl["uri"] = url.url.c_str();
					gdurls.push_back(gdurl);
				}

				gdcol["urls"] = gdurls;
				array.push_back(gdcol);
			}
			return array;
		}

		array.push_back("Stigmark::search failed with status");
		array.push_back(private_data.status);
		return array;
	}
}
