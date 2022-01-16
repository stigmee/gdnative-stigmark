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
use std::ptr::NonNull;
use std::ffi::{CStr, CString};

type CStringPtr = Option<NonNull<c_char>>;
// type CStringArrayPtr = Option<NonNull<*const c_char>>;
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
unsafe impl Send for CCharPtr {}
unsafe impl Sync for CCharPtr {}

// typedef struct stigmark_client_search_response_url_entry
// {
//     int id;
//     const char *url;
// } stigmark_client_search_response_url_entry_t;

#[repr(C)]
pub struct SearchResponseUrlEntry {
    pub id: u32,
    pub url: CCharPtr,
}

// typedef struct stigmark_client_search_response
// {
//     int collection_id;
//     int keyword_id;
//     stigmark_client_search_response_url_entry_t *urls_ptr;
//     int urls_count;
// } stigmark_client_search_response_t;

#[repr(C)]
pub struct SearchResponse {
    pub collection_id: u32,
    pub keyword_id: u32,
    pub urls_ptr: *const SearchResponseUrlEntry,
    pub urls_count: u32,
}

const SEARCH_RESPONSE_NULL: *const SearchResponse = std::ptr::null();

mod search;
use search::{stigmark_client_search_by_keyword_blocking, SqlCollectionByKeywordResponse};

// typedef void (*stigmark_client_search_callback_t)(
//     void *data, int status, stigmark_client_search_response_t *res_ptr, int res_count);
pub type SearchCallback = unsafe extern "C" fn(
    data: *const u8,
    status: CInt,
    res_ptr: *const SearchResponse,
    res_count: u32,
);

// int stigmark_client_search_by_keyword(const char *keyword,
//     stigmark_client_search_callback_t callback, void *data,
//     int waitsync);
#[no_mangle]
pub extern "C" fn stigmark_client_search_by_keyword(
    keyword: CStringPtr,
    cb: Option<SearchCallback>,
    data: CCharPtr,
    waitsync: CInt,
) -> CInt {
    // keyword
    if let None = keyword {
        log::error!("stigmark_client_search_by_keyword: 'keyword' is null");
        return -1;
    }
    let keyword = keyword.unwrap().as_ptr();
    let keyword = unsafe { CStr::from_ptr(keyword) };
    let keyword = keyword.to_str().unwrap();
    let keyword = keyword.to_owned();

    // cb
    if let None = cb {
        log::error!("stigmark_client_search_by_keyword: 'cb' is null");
        return -1;
    }
    let cb = cb.unwrap();
    let cb = move |status: i32, collections: Option<&Vec<SqlCollectionByKeywordResponse>>| {
        if let None = collections {
            unsafe { cb(data.to_ptr(), status as CInt, SEARCH_RESPONSE_NULL, 0) };
            return;
        }
        let collections = collections.unwrap();

        // println!("status: {}", status);
        // for collection in collections {
        //     println!("  - collection_id={}, keyword_id={}", collection.collection_id, collection.keyword_id);
        //     for url in &collection.urls {
        //         println!("     {}: {}", url.id, url.url);
        //     }
        // }

        let mut cstrings: Vec<CString> = vec!();
        let mut res: Vec<SearchResponse> = vec!();
        for collection in collections {
            let mut urls: Vec<SearchResponseUrlEntry> = vec!();
            for url in &collection.urls {
                let url_str = url.url.as_str();
                let cstring_id = cstrings.len();
                cstrings.push(CString::new(url_str).unwrap());
                urls.push(SearchResponseUrlEntry{
                    id: url.id,
                    url: CCharPtr(cstrings[cstring_id].as_ptr() as *const u8),
                });
            }
            res.push(SearchResponse {
                collection_id: collection.collection_id,
                keyword_id: collection.keyword_id, 
                urls_ptr: urls.as_ptr(), urls_count: urls.len() as u32 })
        }

        unsafe { cb(data.to_ptr(), status as CInt, res.as_ptr(), res.len() as u32) };
    };

    // async function to call
    let async_login = move || {
        let rt = Runtime::new();
        if let Err(err) = rt {
            log::error!("hget: could not create tokio runtime: {}", err);
            return;
        }

        let rt = rt.unwrap();
        rt.block_on(async {
            stigmark_client_search_by_keyword_blocking(&keyword, cb).await;
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
