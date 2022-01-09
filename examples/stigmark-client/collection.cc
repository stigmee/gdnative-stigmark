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

struct add_collection_private_data
{
    int status = -1;
};

static void add_collection_callback(void *data, int status)
{
    struct add_collection_private_data *private_data = static_cast<struct add_collection_private_data *>(data);
    private_data->status = status;
}

int stigmark_add_collection(const std::string &token, const std::vector<std::string> &urls, const std::vector<std::string> &keywords)
{
    std::cout << "add_collection: urls:\n";
    auto urls_len = urls.size();
    auto urls_ptr = std::vector<const char *>(urls_len);
    for (int i = 0; i < urls_len; i++)
    {
        auto url = urls[i].c_str();
        urls_ptr[i] = url;
        std::cout << "     " << url << std::endl;
    }

    std::cout << "add_collection: keywords:\n";
    auto keywords_len = keywords.size();
    auto keywords_ptr = std::vector<const char *>(keywords_len);
    for (int i = 0; i < keywords_len; i++)
    {
        auto keyword = keywords[i].c_str();
        keywords_ptr[i] = keyword;
        std::cout << "     " << keyword << std::endl;
    }

    struct add_collection_private_data private_data;
    int err = stigmark_client_add_collection(token.c_str(),
                                             urls_ptr.data(), urls_len,
                                             keywords_ptr.data(), keywords_len,
                                             add_collection_callback, &private_data,
                                             1);
    if (err < 0)
    {
        std::cerr << "add_collection: error " << err << std::endl;
        return -1;
    }

    if (private_data.status >= 200 && private_data.status < 300)
    {
        return 0;
    }

    std::cerr << "add_collection: failed with status=" << private_data.status << std::endl;
    return -1;
}
