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

#ifndef __STIGMARK_CLIENT_HH
#define __STIGMARK_CLIENT_HH

#include "win32.h"
#include "api/login.h"
#include "api/collection.h"
#include "api/search.h"

#ifdef __cplusplus
#include <string>
#include <vector>

extern int stigmark_login(const std::string &mail, const std::string &pass, std::string &token);

extern int stigmark_add_collection(const std::string &token,
                                   const std::vector<std::string> &urls,
                                   const std::vector<std::string> &keywords);

struct stigmark_search_response_url_entry {
    int id;
    std::string url;
};

struct stigmark_search_response {
    int collection_id;
    int keyword_id;
    std::vector<stigmark_search_response_url_entry> urls;
};

extern int stigmark_search(const std::string &keyword, std::vector<stigmark_search_response> &urls);

#endif

#endif // __STIGMARK_CLIENT_HH
