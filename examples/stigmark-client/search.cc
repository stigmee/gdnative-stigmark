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

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <stigmark.h>

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

    // std::cout << "search_callback: res_ptr=" << res_ptr << std::endl;
    // std::cout << "search_callback: res_count=" << res_count << std::endl;

    for (int r = 0; r < res_count; r++)
    {
        stigmark_search_response res;

        res.collection_id = res_ptr[r].collection_id;
        res.keyword_id = res_ptr[r].keyword_id;

        // std::cout << "search_callback:    res[" << r << "].collection_id=" << res.collection_id << std::endl;
        // std::cout << "search_callback:    res[" << r << "].keyword_id=" << res.keyword_id << std::endl;
        // std::cout << "search_callback:    res[" << r << "].url_ptr=" << res_ptr[r].urls_ptr << std::endl;
        // std::cout << "search_callback:    res[" << r << "].url_count=" << res_ptr[r].urls_count << std::endl;

        for (int u = 0; u < res_ptr[r].urls_count; u++)
        {
            stigmark_search_response_url_entry url_entry;

            url_entry.id = res_ptr[r].urls_ptr[u].id;
            url_entry.url = res_ptr[r].urls_ptr[u].url;

            // std::cout << "search_callback:    res[" << r << "].url["<< u <<"].id=" << url_entry.id << std::endl;
            // std::cout << "search_callback:    res[" << r << "].url["<< u <<"].url=" << (void *)(res_ptr[r].urls_ptr[u].url) << std::endl;

            res.urls.push_back(url_entry);
        }
        collections.push_back(res);
    }

    // copy
    private_data->collections = collections;
}

int stigmark_search(const std::string &keyword, std::vector<stigmark_search_response> &response)
{
    struct search_private_data private_data;
    int err = stigmark_client_search_by_keyword(keyword.c_str(),
                                                search_callback, &private_data,
                                                1);
    if (err < 0)
    {
        std::cerr << "stigmark_search: error " << err << std::endl;
        return -1;
    }

    if (private_data.status >= 200 && private_data.status < 300)
    {
        response = private_data.collections;
        return 0;
    }

    std::cerr << "stigmark_search: failed with status=" << private_data.status << std::endl;
    return -1;
}
