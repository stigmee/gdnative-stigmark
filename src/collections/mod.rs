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
// use std::ffi::{CStr, CString};
use std::ffi::CStr;

type CStringPtr = Option<NonNull<c_char>>;
type CStringArrayPtr = Option<NonNull<*const c_char>>;
type CInt = c_int;
// type CCharPtr = *const c_char;

#[repr(C)]
pub struct CCharPtr(*const u8);
#[allow(dead_code)]
impl CCharPtr {
    pub fn to_ptr(&self) -> *const u8 {
        self.0
    }
}
unsafe impl Send for CCharPtr{}
unsafe impl Sync for CCharPtr{}

mod add_collection_impl;
use add_collection_impl::stigmark_client_add_collection_blocking;

// typedef void (*stigmark_client_add_collection_callback_t)(void *data, int status, const char *token);
pub type AddCollectionCallback = unsafe extern "C" fn(data: *const u8, status: CInt);

// int stigmark_client_add_collection(
//      const char *token,
//      const char **urls_ptr, int urls_len,
//      const char **keywords_ptr, int keywords_len,
//      stigmark_add_collection_callback_t callback, void *data,
//      int waitsync);
#[no_mangle]
pub extern "C" fn stigmark_client_add_collection(
    token: CStringPtr,
    urls_ptr: CStringArrayPtr,
    urls_len: CInt,
    keywords_ptr: CStringArrayPtr,
    keywords_len: CInt,
    cb: Option<AddCollectionCallback>, data: CCharPtr,
    waitsync: CInt) -> CInt {

    // token
    if let None = token {
        log::error!("stigmark_client_add_collection: 'mail' is null");
        return -1;
    }
    let token = token.unwrap().as_ptr();
    let token = unsafe { CStr::from_ptr(token) };
    let token = token.to_str().unwrap();

    // urls_ptr, urls_len
    let urls_len = urls_len as usize;
    let mut urls_vec: Vec<String> = Vec::with_capacity(urls_len);
    let urls_ptr = urls_ptr.unwrap().as_ptr();
    let urls_ptr = unsafe { std::slice::from_raw_parts(urls_ptr, urls_len) };
    for i in 0..urls_len {
        let url = urls_ptr[i];
        let url = unsafe { CStr::from_ptr(url) };
        let url = url.to_str().unwrap();
        urls_vec.push(url.to_string());
    }

    // keywords_ptr, keywords_len
    let keywords_len = keywords_len as usize;
    let mut keywords_vec: Vec<String> = Vec::with_capacity(keywords_len);
    let keywords_ptr = keywords_ptr.unwrap().as_ptr();
    let keywords_ptr = unsafe { std::slice::from_raw_parts(keywords_ptr, keywords_len) };
    for i in 0..keywords_len {
        let keyword = keywords_ptr[i];
        let keyword = unsafe { CStr::from_ptr(keyword) };
        let keyword = keyword.to_str().unwrap();
        keywords_vec.push(keyword.to_string());
    }

    // cb
    if let None = cb {
        log::error!("stigmark_client_add_collection: 'cb' is null");
        return -1;
    }
    let cb = cb.unwrap();
    let cb = move |status: i32| {
        unsafe { cb(data.to_ptr(), status as CInt) };
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
            stigmark_client_add_collection_blocking(token, &urls_vec, &keywords_vec, cb).await;
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
