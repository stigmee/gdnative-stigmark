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

#ifndef __STIGMARK_API_COLLECTION_H
#define __STIGMARK_API_COLLECTION_H

#ifdef __cplusplus
extern "C"
{
#endif

    typedef void (*stigmark_add_collection_callback_t)(void *data, int status);
    int stigmark_client_add_collection(const char *token,
                                       const char **urls_ptr, int urls_len,
                                       const char **keywords_ptr, int keywords_len,
                                       stigmark_add_collection_callback_t callback, void *data,
                                       int waitsync);

#ifdef __cplusplus
}
#endif

#endif // __STIGMARK_API_COLLECTION_H
