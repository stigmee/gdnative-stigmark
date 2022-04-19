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

#ifndef __WAITGROUP_HH
#define __WAITGROUP_HH

#include <chrono>
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <cassert>

// waitgroup: thank you golang
class waitgroup
{
    std::mutex mutex;
    std::condition_variable condition;
    unsigned long counter = 0;

public:
    void add()
    {
        std::lock_guard<std::mutex> lock(this->mutex);
        this->counter++;
    }

    void done()
    {
        std::lock_guard<std::mutex> lock(this->mutex);
        assert(this->counter > 0);
        this->counter--;
        this->condition.notify_all();
    }

    void wait()
    {
        std::unique_lock<std::mutex> lock(this->mutex);
        this->condition.wait(lock, [this]()
                             { return this->counter == 0; });
    }
};

#endif // __WAITGROUP_HH
