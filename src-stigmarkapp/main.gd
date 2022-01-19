#
#  Stigmee: A 3D browser and decentralized social network.
#  Copyright 2021-2022 Philippe Anel <zexigh@gmail.com>
#
#  This file is part of Stigmee.
#
#  Project : Stigmark
#  Version : 0.0-1
#
#  Stigmee is free software: you can redistribute it and/or modify it
#  under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful, but
#  WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

extends Node2D

func _ready():
	var s = Stigmark.new()
	var collections = s.search("bitcoin")
	for collection in collections:
		var collection_id = collection.collection_id;
		var keyword_id = collection.keyword_id;
		var urls = collection.urls;
		print("collection: ", collection_id, ", keyword: ", keyword_id)
		for url in urls:
			print("  ", url.id, ": ", url.uri)
	pass
