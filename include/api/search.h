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

#ifndef __STIGMARK_API_SEARCH_H
#define __STIGMARK_API_SEARCH_H

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct stigmark_client_search_response_url_entry
    {
        int id;
        const char *url;
    } stigmark_client_search_response_url_entry_t;

    typedef struct stigmark_client_search_response
    {
        int collection_id;
        int keyword_id;
        stigmark_client_search_response_url_entry_t *urls_ptr;
        int urls_count;
    } stigmark_client_search_response_t;

    typedef void (*stigmark_client_search_callback_t)(
        void *data, int status, stigmark_client_search_response_t *res_ptr, int res_count);
    int stigmark_client_search_by_keyword(const char *keyword,
                               stigmark_client_search_callback_t callback, void *data,
                               int waitsync);

#ifdef __cplusplus
}
#endif

#endif // __STIGMARK_API_SEARCH_H
