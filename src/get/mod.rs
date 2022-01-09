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

use tokio::runtime::Runtime;

use libc::{c_char, c_int};
use std::ffi::CStr;
use std::ffi::CString;
use std::ptr::NonNull;

mod xget;
use xget::{http_get, GetErrorImpl, GetResultImpl};

#[repr(C)]
pub struct GetResult {
    status: c_int,
    body: *const c_char,
}

#[repr(C)]
pub struct GetError {
    status: c_int,
    body: *const c_char,
}
pub type GetCallback = unsafe extern "C" fn(msg: *const GetResult, err: *const GetError);

#[no_mangle]
pub extern "C" fn hget(url: Option<NonNull<c_char>>, cb: Option<GetCallback>, wait: c_int) {
    let wait: bool = wait != 0;
    if let None = cb {
        log::error!("hget: cb is null");
        return;
    }
    let cb = cb.unwrap();
    if let None = url {
        log::error!("hget: url is null");
        return;
    }
    let url = url.unwrap().as_ptr();
    log::debug!("hget: url at {:?}", url);
    let url = unsafe { CStr::from_ptr(url) };
    let url = CString::from(url); // this makes a safe copy of the buffer !!!
    log::debug!("hget: cstring::url={:?}", url);

    let get_fn = move || {
        let rt = Runtime::new();
        if let Err(err) = rt {
            log::error!("hget: could not create tokio runtime: {}", err);
            return;
        }
        let rt = rt.unwrap();

        log::debug!("hget: block_on");
        rt.block_on(async move {
            log::debug!("hget: cstr::url at {:?}", url.as_ptr());
            let url = url.to_str();
            let url = url.unwrap();
            log::debug!("hget: calling http_get with {:#?}", url);
            http_get(
                url,
                |res: Option<&GetResultImpl>, err: Option<&GetErrorImpl>| {
                    let (tmp1, tmp2);
                    unsafe {
                        cb(
                            if let Some(res) = res {
                                tmp1 = CString::new(res.body.as_str()).unwrap();
                                &GetResult {
                                    status: res.status,
                                    body: tmp1.as_ptr(),
                                }
                            } else {
                                std::ptr::null()
                            },
                            if let Some(err) = err {
                                tmp2 = CString::new(err.body.as_str()).unwrap();
                                &GetError {
                                    status: err.status,
                                    body: tmp2.as_ptr(),
                                }
                            } else {
                                std::ptr::null()
                            },
                        );
                    }
                },
            )
            .await;
            log::debug!("hget: http_get returned");
        });
        log::debug!("hget: block_on finished");
    };

    if wait {
        get_fn();
    } else {
        std::thread::spawn(get_fn);
    }
}
