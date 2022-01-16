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

use http::StatusCode;
use hyper::{body::to_bytes, Body, Client, Method, Request, Uri};
use hyper_tls::HttpsConnector;
use serde::Deserialize;

#[derive(Deserialize)]
pub struct SqlCollectionByKeywordUrlEntry {
    pub id: u32,
    pub url: String,
}

#[derive(Deserialize)]
pub struct SqlCollectionByKeywordResponse {
    pub collection_id: u32,
    pub keyword_id: u32,
    pub urls: Vec<SqlCollectionByKeywordUrlEntry>,
}

pub async fn stigmark_client_search_by_keyword_blocking(
    keyword: &String,
    cb: impl FnOnce(i32, Option<&Vec<SqlCollectionByKeywordResponse>>),
) {
    let uri = Uri::builder()
        .scheme("https")
        .authority("stigmark.stigmee.fr")
        .path_and_query(
            format!(
                "/api/v1/search?q={}",
                urlencoding::encode(keyword).into_owned()
            )
            .as_str(),
        )
        .build();
    if let Err(err) = uri {
        log::error!(
            "stigmark_client_search_by_keyword_blocking: could not parse url: {}",
            err
        );
        return;
    }
    let uri = uri.unwrap();

    let req = Request::builder()
        .method(Method::GET)
        .uri(&uri)
        .body(Body::empty());
    if let Err(err) = req {
        log::error!(
            "stigmark_client_search_by_keyword_blocking: could not create request: {}",
            err
        );
        return;
    }
    let req = req.unwrap();

    log::debug!(
        "stigmark_client_search_by_keyword_blocking: waiting {:#?}",
        req
    );
    let https = HttpsConnector::new();
    let response = Client::builder()
        .build::<_, hyper::Body>(https)
        .request(req);

    log::debug!("stigmark_client_search_by_keyword_blocking: response");
    match response.await {
        Ok(res) => {
            let status = res.status();
            if status != StatusCode::OK {
                cb(status.as_u16() as i32, None);
                return;
            }

            let (head, body) = res.into_parts();
            log::debug!("head={:#?}", head);
            match to_bytes(body).await {
                Ok(body) => {
                    let body_str = std::str::from_utf8(&*body).unwrap_or("invalid body");
                    let collections: Vec<SqlCollectionByKeywordResponse> =
                        serde_json::from_str(body_str).unwrap_or(vec![]);
                    cb(status.as_u16() as i32, Some(&collections));
                }
                Err(err) => log::debug!("could not decode body: {}", err),
            }
        }
        Err(err) => {
            log::error!(
                "stigmark_client_search_by_keyword_blocking: response failed: {}",
                err
            );
            cb(0, None);
        }
    };
}
