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
#include <iostream>
#include <stigmark.h>

struct login_private_data
{
    int status = -1;
    std::string token;
};

static void login_callback(void *data, int status, const char *token)
{
    struct login_private_data *private_data = static_cast<struct login_private_data *>(data);
    private_data->status = status;
    private_data->token = token;
}

int stigmark_login(const std::string &mail, const std::string &pass, std::string &token)
{
    struct login_private_data private_data;

    int err = stigmark_client_login(mail.c_str(), pass.c_str(), login_callback, &private_data, 1);
    if (err < 0)
    {
        std::cerr << "login: error " << err << std::endl;
        return -1;
    }

    if (private_data.status >= 200 && private_data.status < 300)
    {
        token = private_data.token;
        return 0;
    }

    std::cerr << "login: failed with status=" << private_data.status << std::endl;
    return -1;
}
