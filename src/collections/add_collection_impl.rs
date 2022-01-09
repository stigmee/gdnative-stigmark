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

use hyper::{body::to_bytes, Body, Client, Method, Request, Uri};
use hyper_tls::HttpsConnector;
use serde_json::json;
use http::StatusCode;

pub async fn stigmark_client_add_collection_blocking(token: &str, urls: &Vec<String>, keywords: &Vec<String>, cb: impl FnOnce(i32)) {
    let uri = Uri::builder()
        .scheme("https")
        .authority("stigmark.stigmee.fr")
        .path_and_query("/api/v1/stigmarks")
        .build();
    if let Err(err) = uri {
        log::error!("stigmark_client_add_collection: could not parse url: {}", err);
        return;
    }
    let uri = uri.unwrap();

    let json_body = json!({"urls": urls, "keys": keywords});
    let json_body = serde_json::to_string(&json_body).unwrap();

    let req = Request::builder()
        .method(Method::POST)
        .uri(&uri)
        .header("Content-Type", "application/json")
        .header("Authorization", format!("Bearer {}", token))
        .body(Body::from(json_body));
    if let Err(err) = req {
        log::error!("stigmark_client_add_collection: could not create request: {}", err);
        return;
    }
    let req = req.unwrap();

    log::debug!("stigmark_client_add_collection: waiting {:#?}", req);
    let https = HttpsConnector::new();
    let response = Client::builder()
        .build::<_, hyper::Body>(https)
        .request(req);

    log::debug!("stigmark_client_add_collection: response");
    match response.await {
        Ok(res) => {
            let status = res.status();
            if status != StatusCode::OK && status != StatusCode::CREATED {
                cb(status.as_u16() as i32);
                return;
            }

            let (head, body) = res.into_parts();
            log::debug!("head={:#?}", head);
            match to_bytes(body).await {
                Ok(_body) => {
                    cb(status.as_u16() as i32);
                },
                Err(err) => log::debug!("could not decode body: {}", err),
            }
        },
        Err(err) => {
            log::error!("stigmark_client_add_collection: response failed: {}", err);
            cb(0);
        },
    };
}
