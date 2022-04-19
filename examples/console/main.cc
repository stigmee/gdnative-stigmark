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

#include <iostream>
#include <stigmark.h>
#include "waitgroup.hh"

auto debug = false;

extern void set_stdin_echo(bool enable);

void show_usage()
{
    std::cout << "usage: stigmark-client[.exe] [options] <command> [parameters]\n";
    std::cout << "commands:\n"
              << "    login <mail>                             try to login\n"
              << "    add_collection <mail>                    add collection to user's collections [thanks to -u and -k]\n"
              << "    search <keyword>                         search for urls referenced by keyword\n"
              ;
    std::cout << "options:\n"
              << "    -?, -h:                                  show this help\n"
              << "    -u <url>, --url <url>:                   add url\n"
              << "    -k <keyword>, --keyword <keyword>:       add keyword\n"
              << "    -?, -h:                                  show this help\n";
}

std::string ask_credential_and_try_login(const std::string &mail)
{
    std::cout << "type password or [Ctrl-c] to cancel: ";
    set_stdin_echo(false);
    std::string pass;
    std::cin >> pass;
    set_stdin_echo(true);
    std::cout << std::endl;

    std::string token;
    if (stigmark_login(mail, pass, token)) {
        return {};
    }
    return token;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        show_usage();
        return 0;
    }

    std::vector<std::string> urls;
    std::vector<std::string> keywords;

    while (--argc, argv++)
    {
        std::string arg = *argv;

        if (arg == "-?" || arg == "-h" || arg == "--help")
            goto usage;

        if (arg == "-u" || arg == "--url")
        {
            if (argc == 0)
            {
                std::cerr << "missing <url> parameter\n";
                return 0;
            }

            std::string url = (--argc, *++argv);
            urls.push_back(url);
            continue;
        }

        if (arg == "-k" || arg == "--keyword")
        {
            if (argc == 0)
            {
                std::cerr << "missing <keyword> parameter\n";
                return 0;
            }

            std::string keyword = (--argc, *++argv);
            keywords.push_back(keyword);
            continue;
        }

        break;
    }

    if (argc == 0)
    {
    usage:
        show_usage();
        return 0;
    }

    std::string cmd = (--argc, *argv++);

    if (cmd == "help")
        goto usage;

    if (cmd == "login")
    {
        if (argc != 1)
        {
            std::cerr << "usage: stigmark-client[.exe] [options] login <mail>\n";
            return -1;
        }

        std::string mail = (--argc, *argv++);
        std::string token = ask_credential_and_try_login(mail);
        if (!token.empty())
            std::cout << "token: " << token << std::endl;
        return 0;
    }

    if (cmd == "add_collection")
    {
        if (argc != 1)
        {
            std::cerr << "usage: stigmark-client[.exe] [options] add_collection <mail>\n";
            return -1;
        }

        if (urls.empty())
        {
            std::cerr << "error: no url to add. Please use -u,--url\n";
            return -1;
        }

        if (keywords.empty())
        {
            std::cout << "warning: no keyword to add. Please use -k,--keyword\n";
        }

        std::string mail = (--argc, *argv++);
        std::string token = ask_credential_and_try_login(mail);
        if (token.empty())
        {
            std::cerr << "could not login\n";
            return -1;
        }

        return stigmark_add_collection(token, urls, keywords);
    }
    
    if (cmd == "search")
    {
        if (argc != 1)
        {
            std::cerr << "usage: stigmark-client[.exe] [options] search <keyword>\n";
            return -1;
        }

        std::string keyword = (--argc, *argv++);
        std::vector<stigmark_search_response> urls;
        int rc = stigmark_search(keyword, urls);
        if (rc == 0)
        {
            for (auto& entry: urls) {
                std::cout << "keyword_id=" << entry.keyword_id << ", collection_id=" << entry.collection_id << ":\n";
                for (auto& url: entry.urls) {
                    std::cout << "    " << url.id << ": " << url.url << std::endl;
                }
            }
        }
        return 0;
    }

    std::cerr << "unknown '" << cmd << "' command" << std::endl;
    return -1;
}
