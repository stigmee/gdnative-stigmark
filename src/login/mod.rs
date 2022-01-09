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

use std::ptr::NonNull;
use libc::{c_char, c_int};
use std::ffi::{CStr, CString};

type CStringPtr = Option<NonNull<c_char>>;
type CInt = c_int;
// type CCharPtr = *const c_char;

#[repr(C)]
pub struct CCharPtr(*const u8);
impl CCharPtr {
    pub fn to_ptr(&self) -> *const u8 {
        self.0
    }
}
unsafe impl Send for CCharPtr{}
unsafe impl Sync for CCharPtr{}

mod login_impl;
use login_impl::stigmark_client_login_blocking;

// typedef void (*stigmark_client_login_callback_t)(void *data, int status, const char *token);
pub type LoginCallback = unsafe extern "C" fn(data: *const u8, status: CInt, token: *const u8);

// int stigmark_client_login(const char *mail, const char *pass, stigmark_client_login_callback_t callback, void *data, int waitsync);
#[no_mangle]
pub extern "C" fn stigmark_client_login(mail: CStringPtr, pass: CStringPtr, cb: Option<LoginCallback>, data: CCharPtr, waitsync: CInt) -> CInt {
    // mail
    if let None = mail {
        log::error!("stigmark_client_login: 'mail' is null");
        return -1;
    }
    let mail = mail.unwrap().as_ptr();
    let mail = unsafe { CStr::from_ptr(mail) };
    let mail = mail.to_str().unwrap();
    let mail = mail.to_owned();

    // pass
    if let None = pass {
        log::error!("stigmark_client_login: 'mail' is null");
        return -1;
    }
    let pass = pass.unwrap().as_ptr();
    let pass = unsafe { CStr::from_ptr(pass) };
    let pass = pass.to_str().unwrap();
    let pass = pass.to_owned();

    // cb
    if let None = cb {
        log::error!("stigmark_client_login: 'cb' is null");
        return -1;
    }
    let cb = cb.unwrap();
    let cb = move |status: i32, token: String| {
        let token_str = token.as_str();
        let token_cstring = CString::new(token_str).unwrap();
        let token_ptr = token_cstring.as_ptr();
        unsafe { cb(data.to_ptr(), status as CInt, token_ptr as *const u8) };
    };

    // async function to call
    let async_login = move || {
        let rt = Runtime::new();
        if let Err(err) = rt {
            log::error!("hget: could not create tokio runtime: {}", err);
            return;
        }

        // println!("login: user={} pass={}", mail, pass);
    
        let rt = rt.unwrap();
        rt.block_on(async {
            stigmark_client_login_blocking(mail, pass, cb).await;
        });
    };

    // execute stigmark_login function
    let waitsync: bool = waitsync != 0;
    if waitsync {
        async_login();
    } else {
        std::thread::spawn(async_login);
    }

    return 0;
}
